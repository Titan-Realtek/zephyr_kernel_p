/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * I3C controller test app for Hot Join / IBI reception.
 *
 * Drives the controller-mode I3C instance selected by the "i3c-controller-0"
 * devicetree alias (see the board overlay) and exercises the controller side
 * of Hot Join and IBI:
 *
 *   - Hot Join: the RTK core auto-accepts a target's Hot Join and runs ENTDAA,
 *     so a hot-joined target that is declared under the controller node (by
 *     PID) gets a dynamic address and shows up in the attached device list.
 *     `i3c_controller daa` also runs a controller-initiated DAA on demand.
 *   - IBI: `i3c_controller ibi-enable` registers an IBI callback on each addressed,
 *     IBI-capable target and sends ENEC(INTR) so the target may raise IBIs.
 *     Received IBIs are logged from the callback.
 *
 * Typical flow:
 *   1. Boot; the app enumerates and enables IBI on any present target.
 *   2. (Hot Join) power/plug the target -> it hot-joins -> `i3c_controller list` to
 *      see it -> `i3c_controller ibi-enable` to enable its IBI.
 *   3. Target raises an IBI -> logged here as "IBI from 0x.. : ...".
 */

#include <zephyr/device.h>
#include <zephyr/drivers/i3c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/sys_io.h>

LOG_MODULE_REGISTER(i3c_ctrl_test, LOG_LEVEL_INF);

#define CTRL_NODE DT_ALIAS(i3c_controller_0)

BUILD_ASSERT(DT_NODE_HAS_STATUS(CTRL_NODE, okay),
	     "alias i3c-controller-0 must point to an enabled I3C controller node");

static const struct device *const ctrl_dev = DEVICE_DT_GET(CTRL_NODE);

/* The i3c subsystem convention: a controller's dev->data starts with
 * struct i3c_driver_data, which holds the attached-device list.
 */
static struct i3c_driver_data *ctrl_data(void)
{
	return (struct i3c_driver_data *)ctrl_dev->data;
}

/* IBI received from a target. Runs in the I3C IBI workqueue thread. */
static int ctrl_ibi_cb(struct i3c_device_desc *target, struct i3c_ibi_payload *payload)
{
	if (payload != NULL && payload->payload_len > 0U) {
		LOG_INF("IBI from 0x%02x: %u byte(s), MDB=0x%02x", target->dynamic_addr,
			payload->payload_len, payload->payload[0]);
		LOG_HEXDUMP_INF(payload->payload, payload->payload_len, "IBI payload");
	} else {
		LOG_INF("IBI from 0x%02x: no payload", target->dynamic_addr);
	}

	return 0;
}

/* Run a controller-initiated dynamic address assignment. */
static int ctrl_do_daa(void)
{
	int ret = i3c_do_daa(ctrl_dev);

	LOG_INF("%s: do_daa -> %d", ctrl_dev->name, ret);
	return ret;
}

/* List the I3C devices currently attached to the controller. */
static void ctrl_list(void)
{
	struct i3c_device_desc *desc;
	bool any = false;

	SYS_SLIST_FOR_EACH_CONTAINER(&ctrl_data()->attached_dev.devices.i3c, desc, node) {
		LOG_INF("dev: dynamic-addr=0x%02x static-addr=0x%02x pid=0x%012llx bcr=0x%02x "
			"ibi-capable=%d",
			desc->dynamic_addr, desc->static_addr, (unsigned long long)desc->pid,
			desc->bcr, i3c_device_is_ibi_capable(desc) ? 1 : 0);
		any = true;
	}

	if (!any) {
		LOG_INF("%s: no attached I3C devices (run 'i3c_controller daa' or hot-join a "
			"target)",
			ctrl_dev->name);
	}
}

/* Armed by `i3c_controller ibi-enable`: auto-ENEC any target that (hot-)joins. */
static volatile bool auto_ibi;

/*
 * Enable IBI on any addressed, IBI-capable target that isn't enabled yet
 * (ibi_cb == NULL used as the "already enabled" marker). Called repeatedly by
 * the auto thread so a target that hot-joins later -- the RTK core auto-runs
 * DAA on Hot Join -- gets its ENEC(INTR) issued automatically, no extra call.
 */
static void ctrl_ibi_enable_new(void)
{
	struct i3c_device_desc *desc;

	SYS_SLIST_FOR_EACH_CONTAINER(&ctrl_data()->attached_dev.devices.i3c, desc, node) {
		int ret;

		if (desc->dynamic_addr == 0U || !i3c_device_is_ibi_capable(desc) ||
		    desc->ibi_cb != NULL) {
			continue;
		}

		desc->ibi_cb = ctrl_ibi_cb;
		ret = i3c_ibi_enable(desc);
		LOG_INF("%s: auto ENEC 0x%02x -> %d", ctrl_dev->name, desc->dynamic_addr, ret);
		if (ret != 0) {
			desc->ibi_cb = NULL; /* leave it for a later retry */
		}
	}
}

static void ctrl_ibi_disable_all(void)
{
	struct i3c_device_desc *desc;

	SYS_SLIST_FOR_EACH_CONTAINER(&ctrl_data()->attached_dev.devices.i3c, desc, node) {
		int ret;

		if (desc->ibi_cb == NULL) {
			continue;
		}

		ret = i3c_ibi_disable(desc);
		LOG_INF("%s: DISEC 0x%02x -> %d", ctrl_dev->name, desc->dynamic_addr, ret);
		desc->ibi_cb = NULL;
	}
}

/* While armed, keep enabling IBI on newly (hot-)joined targets. */
static void ctrl_auto_thread(void)
{
	while (1) {
		if (auto_ibi && device_is_ready(ctrl_dev)) {
			ctrl_ibi_enable_new();
		}
		k_sleep(K_MSEC(200));
	}
}

#define CTRL_THREAD_STACK 1024
#define CTRL_THREAD_PRIO  7
K_THREAD_DEFINE(ctrl_auto_tid, CTRL_THREAD_STACK, ctrl_auto_thread, NULL, NULL, NULL,
		CTRL_THREAD_PRIO, 0, 0);

#ifdef CONFIG_SHELL
#include <zephyr/shell/shell.h>

static int cmd_daa(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);
	return ctrl_do_daa();
}

static int cmd_list(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);
	ctrl_list();
	return 0;
}

/* Dump the controller registers that gate Hot Join acceptance. */
static int cmd_reg(const struct shell *sh, size_t argc, char **argv)
{
	uintptr_t base = DT_REG_ADDR(CTRL_NODE);
	uint32_t ccr = sys_read32(base + 0x00);   /* CCR: ROLE/PCR/DMA */
	uint32_t csr = sys_read32(base + 0x18);   /* CSR: BUSY/BBUSYN */
	uint32_t ec = sys_read32(base + 0x80);    /* EC: INT/CR/HJ/TREQ_DIS */
	uint32_t cbdr = sys_read32(base + 0x14);  /* CBDR: FST/PP/OD baud dividers */
	uint32_t tinit = sys_read32(base + 0x34); /* TINIT: THIGH_OD [31:20] */
	uint32_t bdr_od = cbdr & 0xffff;
	uint32_t bdr_pp = (cbdr >> 16) & 0xff;
	uint32_t bdr_fst = (cbdr >> 24) & 0xff;
	uint32_t thigh_od = (tinit >> 20) & 0xfff;

	shell_print(sh, "CCR=0x%08x (ROLE=%u PCR=%u)", ccr, ccr & 0x1, (ccr >> 1) & 0x1);
	shell_print(sh, "CSR=0x%08x (BUSY=%u BBUSYN=%u)", csr, csr & 0x1, (csr >> 1) & 0x1);
	shell_print(sh, "EC =0x%08x (INT/IBI=%u CR=%u HJ=%u TREQ_DIS=%u)  <-- HJ=1 means ACK HJ",
		    ec, ec & 0x1, (ec >> 1) & 0x1, (ec >> 3) & 0x1, (ec >> 16) & 0x1);
	shell_print(sh, "CBDR=0x%08x  BDR_PP=%u BDR_OD=%u BDR_FST=%u", cbdr, bdr_pp, bdr_od,
		    bdr_fst);
	shell_print(sh, "TINIT=0x%08x THIGH_OD=%u", tinit, thigh_od);
	/* 100 MHz I3C clock (PLL): PP = clk/(2*BDR_PP), OD period = BDR_OD*2/clk. */
	if (bdr_pp) {
		shell_print(sh, "  -> PP ~= %u kHz", (unsigned int)(100000U / (2U * bdr_pp)));
	}
	if (bdr_od) {
		shell_print(sh, "  -> OD ~= %u kHz", (unsigned int)(100000U / (2U * bdr_od)));
	}
	return 0;
}

static int cmd_ibi_enable(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	auto_ibi = true;
	shell_print(sh, "auto-IBI armed: any target that (hot-)joins is DAA'd (by the "
			"core) and ENEC'd automatically");
	/* Enable anything already present without waiting for the poll. */
	ctrl_ibi_enable_new();
	return 0;
}

static int cmd_ibi_disable(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	auto_ibi = false;
	ctrl_ibi_disable_all();
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	ctrl_subcmds,
	SHELL_CMD(daa, NULL, "Run dynamic address assignment (also accepts pending targets)",
		  cmd_daa),
	SHELL_CMD(list, NULL, "List attached I3C devices", cmd_list),
	SHELL_CMD(reg, NULL, "Dump CCR/CSR/EC (HJ-accept) registers", cmd_reg),
	SHELL_CMD(ibi - enable, NULL, "Arm auto-IBI: (hot-)joined targets get ENEC'd automatically",
		  cmd_ibi_enable),
	SHELL_CMD(ibi - disable, NULL, "Disarm auto-IBI and DISEC all targets", cmd_ibi_disable),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(i3c_controller, &ctrl_subcmds, "I3C controller HJ/IBI test controls", NULL);
#endif /* CONFIG_SHELL */

int main(void)
{
	printk("I3C controller test on %s\n", CONFIG_BOARD_TARGET);

	if (!device_is_ready(ctrl_dev)) {
		LOG_ERR("%s not ready", ctrl_dev->name);
		return 0;
	}

	/* Run 'i3c_controller ibi-enable' once to arm. After that, when a target sends a
	 * Hot Join the core auto-runs DAA and this app auto-ENECs it -- no further
	 * commands needed. 'i3c_controller list' shows devices; 'ibi-disable' disarms.
	 */
	LOG_INF("%s: controller ready; run 'i3c_controller ibi-enable' to arm", ctrl_dev->name);

	return 0;
}
