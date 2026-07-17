/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * I3C target echo sample.
 *
 * Registers an echo target on every I3C target device exposed through the
 * "i3c-target-<n>" devicetree aliases: whatever the controller writes is
 * stored and returned on the next read. Useful as a companion target for
 * exercising an I3C controller (e.g. via the `i3c` shell) in a
 * controller<->target loopback.
 *
 * The board provides the alias->node mapping (see the board overlay), so this
 * source stays board-agnostic.
 *
 * Flow (target byte-callback API):
 *   1. The target driver arms its RX buffer when configured as a target.
 *   2. Controller writes N bytes -> write_received_cb() accumulates them.
 *   3. STOP -> stop_cb() latches N and wakes the echo worker thread.
 *   4. The worker preloads the N bytes into the TX FIFO via
 *      i3c_target_tx_write() (this cannot run in the stop_cb ISR context,
 *      hence the dedicated thread).
 *   5. Controller reads N bytes -> the driver returns the preloaded data.
 *
 * Echo is clean up to the target FIFO/buffer depth (64 bytes); larger writes
 * may arrive in multiple RX batches and are not reassembled here.
 */

#include <zephyr/device.h>
#include <zephyr/drivers/i3c.h>
#include <zephyr/drivers/i3c/target_device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

LOG_MODULE_REGISTER(i3c_target_test, LOG_LEVEL_INF);

#define ECHO_BUF_SIZE     256
#define ECHO_THREAD_STACK 1024
#define ECHO_THREAD_PRIO  7

struct echo_target {
	/* Must stay first for CONTAINER_OF() from the callbacks. */
	struct i3c_target_config cfg;
	const struct device *dev;
	struct k_sem stop_sem;
	uint8_t buf[ECHO_BUF_SIZE];
	volatile uint16_t rx_len;   /* bytes of the in-progress write */
	volatile uint16_t echo_len; /* bytes latched at STOP, to echo back */
};

/* Support up to four aliased echo targets: i3c-target-0 .. i3c-target-3. */
#define ECHO_MAX_TARGETS 4

/* Populated by each worker at startup so the shell can address targets. */
static struct echo_target *echo_registry[ECHO_MAX_TARGETS];

static int echo_write_received(struct i3c_target_config *cfg, uint8_t val)
{
	struct echo_target *echo = CONTAINER_OF(cfg, struct echo_target, cfg);

	if (echo->rx_len < sizeof(echo->buf)) {
		echo->buf[echo->rx_len++] = val;
	}

	return 0;
}

static int echo_stop(struct i3c_target_config *cfg)
{
	struct echo_target *echo = CONTAINER_OF(cfg, struct echo_target, cfg);

	/* Latch this write's length for the worker and reset for the next
	 * write. i3c_target_tx_write() takes a mutex, so it cannot be called
	 * from this (ISR) context -- defer it to the echo thread.
	 */
	echo->echo_len = echo->rx_len;
	echo->rx_len = 0U;
	k_sem_give(&echo->stop_sem);

	return 0;
}

static const struct i3c_target_callbacks echo_callbacks = {
	.write_received_cb = echo_write_received,
	.stop_cb = echo_stop,
};

/* Request a hot join on this target. Best-effort; returns the driver code. */
static int echo_raise_hj(struct echo_target *echo)
{
	struct i3c_ibi hj = {
		.ibi_type = I3C_IBI_HOTJOIN,
	};
	int ret = i3c_ibi_raise(echo->dev, &hj);

	LOG_INF("%s: hot join -> %d", echo->dev->name, ret);

	return ret;
}

#define ECHO_IBI_MDB 0xAAU

/*
 * Raise a target-interrupt IBI carrying the given payload. Whether a payload is
 * required/forbidden depends on the target's BCR "IBI payload" bit (bit 2):
 *   - BCR bit2 = 1  -> a payload (>=1 byte, byte0 = MDB) is required
 *   - BCR bit2 = 0  -> no payload is allowed
 * The driver rejects a mismatch with -EINVAL. Needs an assigned dynamic address.
 */
static int echo_raise_ibi_payload(struct echo_target *echo, uint8_t *payload, uint16_t len)
{
	struct i3c_ibi ibi = {
		.ibi_type = I3C_IBI_TARGET_INTR,
		.payload = payload,
		.payload_len = len,
	};
	int ret;

	if (echo->cfg.address == 0U) {
		LOG_WRN("%s: no dynamic address assigned, cannot raise IBI", echo->dev->name);
		return -EINVAL;
	}

	ret = i3c_ibi_raise(echo->dev, &ibi);
	LOG_INF("%s: ibi (da 0x%02x, payload %u bytes) -> %d", echo->dev->name, echo->cfg.address,
		len, ret);

	return ret;
}

/* Default IBI used at startup / by `i3c_target ibi`: MDB only (case 2). */
static int echo_raise_ibi(struct echo_target *echo)
{
	static uint8_t mdb = ECHO_IBI_MDB;

	return echo_raise_ibi_payload(echo, &mdb, 1U);
}

/*
 * One-shot startup sequence: request hot join, then (once the controller has
 * assigned a dynamic address) raise a single IBI. Useful for exercising an I3C
 * analyzer/controller. Best-effort: if no controller responds, the steps time
 * out and the target still serves echo afterwards. The same steps can be
 * re-triggered at any time via the `i3c_target hj` / `i3c_target ibi` shell cmds.
 */
static void echo_startup_events(struct echo_target *echo)
{
	/* Give the bus/controller a moment to come up before hot-joining. */
	k_sleep(K_MSEC(500));

	echo_raise_hj(echo);

	/* Wait for the controller to assign a dynamic address (post-DAA). */
	for (int i = 0; i < 100 && echo->cfg.address == 0U; i++) {
		k_sleep(K_MSEC(10));
	}

	if (echo->cfg.address == 0U) {
		LOG_WRN("%s: no dynamic address assigned, skipping IBI", echo->dev->name);
		return;
	}

	echo_raise_ibi(echo);
}

static void echo_worker(void *p1, void *p2, void *p3)
{
	struct echo_target *echo = p1;
	int idx = (int)(uintptr_t)p2;

	ARG_UNUSED(p3);

	k_sem_init(&echo->stop_sem, 0, 1);

	if (!device_is_ready(echo->dev)) {
		LOG_ERR("%s not ready", echo->dev->name);
		return;
	}

	if (i3c_target_register(echo->dev, &echo->cfg) != 0) {
		LOG_ERR("%s: failed to register echo target", echo->dev->name);
		return;
	}

	/* Expose to the shell now that the target is registered. */
	echo_registry[idx] = echo;

	LOG_INF("%s: echo target ready", echo->dev->name);

	// echo_startup_events(echo);

	while (1) {
		k_sem_take(&echo->stop_sem, K_FOREVER);

		if (echo->echo_len == 0U) {
			continue;
		}

		int ret = i3c_target_tx_write(echo->dev, echo->buf, echo->echo_len);

		if (ret < 0) {
			LOG_ERR("%s: tx preload failed (%d)", echo->dev->name, ret);
		} else {
			LOG_INF("%s: echo %u bytes armed", echo->dev->name, echo->echo_len);
		}
	}
}

/* Instantiate an echo target + worker for one aliased target node. */
#define ECHO_TARGET_DEFINE(idx)                                                                    \
	static struct echo_target echo_##idx = {                                                   \
		.dev = DEVICE_DT_GET(DT_ALIAS(i3c_target_##idx)),                                  \
		.cfg = {.callbacks = &echo_callbacks},                                             \
	};                                                                                         \
	K_THREAD_DEFINE(echo_thread_##idx, ECHO_THREAD_STACK, echo_worker, &echo_##idx,            \
			(void *)(uintptr_t)(idx), NULL, ECHO_THREAD_PRIO, 0, 0)

/*
 * Only instantiate when the aliased node exists AND is enabled: DEVICE_DT_GET()
 * on a disabled/absent node has no device instance and fails to link
 * (__device_dts_ord_N undeclared). DT_NODE_EXISTS() is true even for disabled
 * nodes, so it is not sufficient here.
 */
#define ECHO_TARGET_IF_ALIAS(idx)                                                                  \
	COND_CODE_1(DT_NODE_HAS_STATUS(DT_ALIAS(i3c_target_##idx), okay),                          \
		    (ECHO_TARGET_DEFINE(idx);), ())

/* i3c-target-0 .. i3c-target-3 (see ECHO_MAX_TARGETS). */
ECHO_TARGET_IF_ALIAS(0)
ECHO_TARGET_IF_ALIAS(1)
ECHO_TARGET_IF_ALIAS(2)
ECHO_TARGET_IF_ALIAS(3)

#ifdef CONFIG_SHELL
#include <stdlib.h>
#include <zephyr/shell/shell.h>

/*
 * Apply `fn` to one target (when `arg` names an index) or to every registered
 * target (when `arg` is NULL). Lets `i3c_target hj`/`ibi` target all or one.
 */
static int echo_for_each(const struct shell *sh, const char *arg, int (*fn)(struct echo_target *))
{
	if (arg != NULL) {
		int idx = atoi(arg);

		if (idx < 0 || idx >= ECHO_MAX_TARGETS || echo_registry[idx] == NULL) {
			shell_error(sh, "no echo target %s", arg);
			return -ENODEV;
		}

		fn(echo_registry[idx]);
		return 0;
	}

	bool any = false;

	for (size_t i = 0; i < ECHO_MAX_TARGETS; i++) {
		if (echo_registry[i] != NULL) {
			fn(echo_registry[i]);
			any = true;
		}
	}

	if (!any) {
		shell_error(sh, "no echo targets registered");
		return -ENODEV;
	}

	return 0;
}

static int cmd_echo_status(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	for (size_t i = 0; i < ECHO_MAX_TARGETS; i++) {
		struct echo_target *echo = echo_registry[i];

		if (echo == NULL) {
			continue;
		}

		shell_print(sh, "[%u] %s  dynamic-addr=0x%02x", (unsigned int)i, echo->dev->name,
			    echo->cfg.address);
	}

	return 0;
}

static int cmd_echo_hj(const struct shell *sh, size_t argc, char **argv)
{
	return echo_for_each(sh, argc > 1 ? argv[1] : NULL, echo_raise_hj);
}

/* Case 1: IBI with no payload. Requires the target BCR IBI-payload bit = 0,
 * otherwise the driver rejects it with -EINVAL.
 */
static int echo_raise_ibi_none(struct echo_target *echo)
{
	return echo_raise_ibi_payload(echo, NULL, 0U);
}

/*
 * Case 3: IBI with MDB + the largest payload a single FIFO pre-fill allows.
 *
 * A target cannot stall SCL and I3C TX has no bus flow control, and the core
 * pre-fills the TXDA FIFO once (no TX-threshold IRQ / no DMA here). The IBI
 * address frame consumes 1 byte of the shared 256-byte TXDA FIFO (SDR address =
 * 1 byte), so the usable IBI payload is 256 - 1 = 255 bytes (payload includes
 * the MDB). Using the full 256 returns -ENOSPC (the FIFO guard reserves a whole
 * 32-bit word, so only 252 get written). A larger IBI needs the DMA path.
 */
#define ECHO_IBI_BIG_LEN 255U
static int echo_raise_ibi_big(struct echo_target *echo)
{
	static uint8_t buf[ECHO_IBI_BIG_LEN];

	buf[0] = ECHO_IBI_MDB;
	for (size_t i = 1U; i < sizeof(buf); i++) {
		buf[i] = (uint8_t)i;
	}

	return echo_raise_ibi_payload(echo, buf, (uint16_t)sizeof(buf));
}

static int cmd_echo_ibi(const struct shell *sh, size_t argc, char **argv)
{
	/* Case 2: MDB only. */
	return echo_for_each(sh, argc > 1 ? argv[1] : NULL, echo_raise_ibi);
}

static int cmd_echo_ibi_none(const struct shell *sh, size_t argc, char **argv)
{
	return echo_for_each(sh, argc > 1 ? argv[1] : NULL, echo_raise_ibi_none);
}

static int cmd_echo_ibi_big(const struct shell *sh, size_t argc, char **argv)
{
	return echo_for_each(sh, argc > 1 ? argv[1] : NULL, echo_raise_ibi_big);
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	echo_subcmds,
	SHELL_CMD(status, NULL, "List registered echo targets and their addresses",
		  cmd_echo_status),
	SHELL_CMD_ARG(hj, NULL, "Raise hot join. Usage: i3c_target hj [target-index]", cmd_echo_hj,
		      1, 1),
	SHELL_CMD_ARG(ibi, NULL, "Case 2: IBI with MDB only. Usage: i3c_target ibi [target-index]",
		      cmd_echo_ibi, 1, 1),
	SHELL_CMD_ARG(ibi_none, NULL,
		      "Case 1: IBI without payload (needs BCR bit2=0). "
		      "Usage: i3c_target ibi_none [target-index]",
		      cmd_echo_ibi_none, 1, 1),
	SHELL_CMD_ARG(ibi_big, NULL,
		      "Case 3: IBI with MDB + max single-FIFO payload (~60B; larger needs DMA). "
		      "Usage: i3c_target ibi_big [target-index]",
		      cmd_echo_ibi_big, 1, 1),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(i3c_target, &echo_subcmds, "I3C target-side test controls", NULL);
#endif /* CONFIG_SHELL */

int main(void)
{
	printk("I3C target echo sample on %s\n", CONFIG_BOARD_TARGET);
	return 0;
}
