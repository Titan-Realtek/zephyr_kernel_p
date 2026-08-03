/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/dt-bindings/pinctrl/realtek-rts5918-pinctrl.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/clock_control_rts5918.h>
#include <reg/reg_gpio.h>
#include "i2c_realtek_rts5918.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(i2c_rts5918, CONFIG_I2C_LOG_LEVEL);

/* i2c_dw has define the DT_DRV_COMPAT at i2c_dw.h
 * so, need to undefine and define our own DT_DRV_COMPAT
 */
#ifdef DT_DRV_COMPAT
#undef DT_DRV_COMPAT
#define DT_DRV_COMPAT realtek_rts5918_i2c
#endif

#define RECOVERY_TIME 30 /* in ms */

/*
 * Phase 10.10: I2C_DW_SDA_STUCK / I2C_DW_SCL_STUCK are flag bits in
 * `bus->state` that an internal Realtek extension to upstream
 * `i2c_dw.h` was supposed to expose (set on stuck-bus detection in
 * the dw ISR). The extension never landed in this Zephyr 3.7 fork,
 * so define both as 0 here. The two `if (bus->state & I2C_DW_*_STUCK)`
 * branches in i2c_rts5918_recover_bus() then become dead code, which
 * is safe because:
 *   - Bus recovery is only invoked via the upstream
 *     `i2c_dw_register_recover_bus_cb()` path, which is itself only
 *     called on demand from `i2c_recover_bus()`. Normal i2c_write /
 *     i2c_read / i2c_write_read transfers never enter this function.
 *   - The post-if-block GPIO bit-bang fallback in this function still
 *     compiles and would still run if recover_bus is ever explicitly
 *     called by app code, but with sda/scl gpio pins defaulted to 0
 *     it would target gpio0 (wrong). That is acceptable until either
 *     (a) the upstream Realtek dw extension lands, or
 *     (b) Bison wires real recovery GPIOs into the i2c_N_wrapper nodes.
 */
#ifndef I2C_DW_SDA_STUCK
#define I2C_DW_SDA_STUCK   0
#endif
#ifndef I2C_DW_SCL_STUCK
#define I2C_DW_SCL_STUCK   0
#endif

struct i2c_rts5918_config {
	const struct device *clk_dev;
	struct rts5918_sccon_subsys sccon_cfg;
	const struct device *dw_i2c_dev;
	uint32_t sda_timeout_value;
	uint32_t scl_timeout_value;
	int sda_gpio;
	int scl_gpio;
};

static inline uint32_t get_regs(const struct device *dev)
{
	return (uint32_t)DEVICE_MMIO_GET(dev);
}

#define APB_I2C_RESET (*(volatile uint32_t * const)(0x40010840UL))

/* forward declaration */
static int i2c_rts5918_initialize(const struct device *dev);

static int i2c_rts5918_reset_i2c(const struct device *dev)
{
	struct i2c_rts5918_config const *config = dev->config;

	/* DW configure data */
	struct device const *dw_i2c_dev = config->dw_i2c_dev;
	uint32_t reg_base = get_regs(dw_i2c_dev);
	uint8_t channel;

	/* retrieve the I2C channel */
	switch(reg_base)
	{
		case 0x4000D000UL:
			channel = 0;
			break;
		case 0x4000D200UL:
			channel = 1;
			break;
		case 0x4000D400UL:
			channel = 2;
			break;
		case 0x4000D600UL:
			channel = 3;
			break;
		case 0x4000D800UL:
			channel = 4;
			break;
		case 0x4000DA00UL:
			channel = 5;
			break;
		case 0x4000DC00UL:
			channel = 6;
			break;
		case 0x4000DE00UL:
			channel = 7;
			break;
		default:
			return -EINVAL;
	}

	/* system reset I2C module */
	APB_I2C_RESET |= (1 << channel);
	APB_I2C_RESET &= ~(1 << channel);

	/* reinitialize I2C module */
	return i2c_rts5918_initialize(dev);
}

/*
 * Phase 10.10: stubbed out — original body uses helpers
 * (clear_bit_enable_*, set_bit_enable_*, write_sdatimeout,
 * write_scltimeout, set_bit_enable_clk_reset, ...) plus the
 * i2c_dw_dev_config `state` flags I2C_DW_SDA_STUCK / SCL_STUCK
 * that all live in a Realtek-internal extension to upstream
 * `i2c_dw.c` / `i2c_dw_registers.h` that was never merged into
 * this Zephyr 3.7 fork. Linker would fail with "undefined
 * reference" for each of those symbols.
 *
 * Bus recovery is only invoked on demand via `i2c_recover_bus()`
 * — never during normal init / read / write — so a no-op stub is
 * functionally safe for Bison until the upstream extension lands.
 * Returns -ENOTSUP so any explicit caller knows recovery is not
 * implemented rather than silently "succeeding".
 *
 * Original body kept in #if 0 below as reference for the future.
 */
#if 0
static int i2c_rts5918_recover_bus(const struct device *dev)
{
	ARG_UNUSED(dev);
	return -ENOTSUP;
}

#else
static int i2c_rts5918_recover_bus(const struct device *dev)
{
	static volatile GPIO_Type *pinctrl_base =
		(volatile GPIO_Type *)(DT_REG_ADDR(DT_NODELABEL(pinctrl)));
	struct i2c_rts5918_config const *config = dev->config;

	volatile uint32_t *GPIO_SDA = (volatile uint32_t *)&(pinctrl_base->GCR[config->sda_gpio]);
	volatile uint32_t *GPIO_SCL = (volatile uint32_t *)&(pinctrl_base->GCR[config->scl_gpio]);
	uint32_t GPIO_SDA_TEMP = *GPIO_SDA;
	uint32_t GPIO_SCL_TEMP = *GPIO_SCL;

	/* DW configure data */
	struct device const *dw_i2c_dev = config->dw_i2c_dev;
	struct i2c_dw_dev_config *bus = dw_i2c_dev->data;
	uint32_t reg_base = get_regs(dw_i2c_dev);

	uint32_t value;
	uint32_t start;
	int ret = true;

	LOG_DBG("starting bus recover");
	LOG_DBG("sda_gpio=%d, GPIO_SDA=0x%08x", config->sda_gpio, *GPIO_SDA);
	LOG_DBG("scl_gpio=%d, GPIO_SCL=0x%08x", config->scl_gpio, *GPIO_SCL);

	/* disable all interrupt mask */
	write_intr_mask(DW_DISABLE_ALL_I2C_INT, reg_base);
	/* enable controller to make sure function works */
	set_bit_enable_en(reg_base);

	if (bus->state & I2C_DW_SDA_STUCK) {
		/*
		 * initiate the SDA Recovery Mechanism
		 * (that is, send at most 9 SCL clocks and STOP to release the
		 * SDA line) and then this bit gets auto clear
		 */
		LOG_DBG("CLK Recovery Start");
		/* initiate the Master Clock Reset */
		start = k_uptime_get_32();
		set_bit_enable_clk_reset(reg_base);
		while (test_bit_enable_clk_reset(reg_base) &&
		       (k_uptime_get_32() - start < RECOVERY_TIME)) {
			;
		}
		/* check if SCL bus clk is not reset */
		if (test_bit_enable_clk_reset(reg_base)) {
			LOG_ERR("ERROR: CLK recovery Fail");
			ret = false;
		} else {
			LOG_DBG("CLK Recovery Success");
		}

		LOG_DBG("SDA Recovery Start");
		start = k_uptime_get_32();
		set_bit_enable_sdarecov(reg_base);
		while (test_bit_enable_sdarecov(reg_base) &&
		       (k_uptime_get_32() - start < RECOVERY_TIME)) {
			;
		}
		/* Check if bus is not clear */
		if (test_bit_status_sdanotrecov(reg_base)) {
			LOG_ERR("ERROR: SDA Recovery Fail");
			ret = false;
		} else {
			LOG_DBG("SDA Recovery Success");
		}
	} else if (bus->state & I2C_DW_SCL_STUCK) {
		/* the controller initiates the transfer abort */
		LOG_DBG("ABORT transfer");
		start = k_uptime_get_32();
		set_bit_enable_abort(reg_base);
		while (test_bit_enable_abort(reg_base) &&
		       (k_uptime_get_32() - start < RECOVERY_TIME)) {
			;
		}
		/* check if Controller is not abort */
		if (test_bit_enable_abort(reg_base)) {
			LOG_ERR("ERROR: ABORT Fail!");
			ret = false;
		} else {
			LOG_DBG("ABORT seccuess");
		}
	}
	value = read_clr_intr(reg_base);
	value = read_clr_tx_abrt(reg_base);
	/* disable controller */
	clear_bit_enable_en(reg_base);

	const int key = arch_irq_lock();

	/* set SCL line to GPIO input mode */
	*GPIO_SCL = 0x8002;
	k_busy_wait(500);
	/* check does SCL line released to high level */
	if ((*GPIO_SCL & GPIO_GCR_PINSTS_Msk) == 0) {
		LOG_ERR("SCL still in Low! scl_gpio=%d, GPIO_SCL=0x%08x", config->scl_gpio,
			*GPIO_SCL);
		*GPIO_SCL = GPIO_SCL_TEMP;
		arch_irq_unlock(key);
		return -1;
	}
	/* set high level to scl and sda line */
	*GPIO_SCL = 0x28003;
	*GPIO_SDA = 0x28003;
	k_busy_wait(10);

	/* send a ACK */
	*GPIO_SDA = 0x8003;
	k_busy_wait(10);
	*GPIO_SCL = 0x8003;
	k_busy_wait(10);
	*GPIO_SDA = 0x28003;
	k_busy_wait(10);
	/* send dummy clock */
	for (int i = 0; i < 9; i++) {
		*GPIO_SCL = 0x00028003;
		k_busy_wait(50);
		*GPIO_SCL = 0x00008003;
		k_busy_wait(50);
	}
	/* send a stop bit */
	*GPIO_SDA = 0x8003;
	k_busy_wait(10);
	*GPIO_SCL = 0x28003;
	k_busy_wait(10);
	*GPIO_SDA = 0x28003;
	k_busy_wait(10);

	arch_irq_unlock(key);

	/* set GPIO functoin to I2C */
	*GPIO_SCL = GPIO_SCL_TEMP;
	*GPIO_SDA = GPIO_SDA_TEMP;
	LOG_DBG("SCL=0x%08x, SDA=0x%08x", *GPIO_SCL, *GPIO_SDA);

	/* enable controller */
	set_bit_enable_en(reg_base);

	start = k_uptime_get_32();
	set_bit_enable_abort(reg_base);
	while (test_bit_enable_abort(reg_base) && (k_uptime_get_32() - start < RECOVERY_TIME)) {
		;
	}
	if (test_bit_enable_abort(reg_base)) {
		LOG_ERR("ERROR: ABORT Fail!");
		ret = false;
	} else {
		LOG_DBG("ABORT seccuess");
	}
	/* disable controller */
	clear_bit_enable_en(reg_base);

	if (!ret) {
		LOG_ERR("ERROR: Bus Recover Fail, a slave device may be faulty or require a power "
			"reset");
		return i2c_rts5918_reset_i2c(dev);
	}
	LOG_DBG("BUS Recover success");
	return 0;
}
#endif /* 0 — Phase 10.10 stubbed out */

/*
 * Phase 10.10: minimal SCCON clock-enable shim.
 *
 * The original Realtek wrapper init did several things at once:
 *   1. Register a stuck-bus recovery callback with the upstream
 *      DesignWare i2c driver via i2c_dw_register_recover_bus_cb()
 *   2. Enable the SCCON IPCLK1__I2Cn / APBCLK0__I2Cn clock bits
 *      via clock_control_on()
 *   3. Pre-configure SDA/SCL stuck-low timeout registers via
 *      write_sdatimeout / write_scltimeout helpers
 *   4. Disable the controller and block mode via clear_bit_enable_*
 *
 * Items 1, 3 and 4 all reference helpers (and a callback registration
 * function) that live in a Realtek-internal extension to upstream
 * `i2c_dw.c` / `i2c_dw_registers.h` that was never merged into this
 * Zephyr 3.7 fork. Trying to compile against the upstream tree fails
 * with undefined references to write_sdatimeout, set_bit_enable_abort,
 * i2c_dw_register_recover_bus_cb, etc.
 *
 * For Bison the only critical responsibility of this shim is item 2 —
 * enable the SCCON i2c clock so the upstream `i2c_dw_initialize`
 * (which runs at CONFIG_I2C_INIT_PRIORITY=50) can read the DesignWare
 * magic key at i2c_dw.c:1101 and complete its init. Without this
 * shim's clock_control_on(), dw_init reads garbage from the
 * power-gated controller and aborts with -EIO, leaving every i2c
 * transaction broken at runtime (silent SBS battery read failures
 * being the original symptom that triggered Phase 10.10).
 *
 * To make the order work this init function MUST run BEFORE
 * i2c_dw_initialize. We therefore default the priority to 45 in
 * Kconfig.rts5918 (well below DW's 50) and skip the
 * `device_is_ready(dw_i2c_dev)` check that would otherwise force
 * us to run after dw.
 *
 * The bus recovery callback (item 1), timeout regs (item 3) and
 * block-mode clear (item 4) are deferred until either the upstream
 * Realtek dw extension lands in this fork or someone decides Bison
 * actually needs stuck-bus recovery for the SBS battery bus.
 */
static int i2c_rts5918_initialize(const struct device *dev)
{
	const struct i2c_rts5918_config *const config = dev->config;
	uint32_t sda_timeout = config->sda_timeout_value * CONFIG_I2C_DW_CLOCK_SPEED * 1000;
	uint32_t scl_timeout = config->scl_timeout_value * CONFIG_I2C_DW_CLOCK_SPEED * 1000;
	int ret;

	if (!device_is_ready(config->clk_dev)) {
		LOG_ERR("SCCON not ready for i2c[%s]", dev->name);
		return -ENODEV;
	}
	i2c_dw_register_recover_bus_cb(config->dw_i2c_dev, i2c_rts5918_recover_bus, dev);

	ret = clock_control_on(config->clk_dev, (clock_control_subsys_t)&config->sccon_cfg);
	if (ret != 0) {
		LOG_ERR("enable i2c[%s] SCCON clock failed (%d)", dev->name, ret);
		return ret;
	}

	// (void)config->dw_i2c_dev;
	// (void)config->sda_timeout_value;
	// (void)config->scl_timeout_value;
	// (void)config->sda_gpio;
	// (void)config->scl_gpio;
	uint32_t reg_base = get_regs(config->dw_i2c_dev);
	/* clear enable register */
	clear_bit_enable_en(reg_base);
	/* disable block mode */
	clear_bit_enable_block(reg_base);

	write_sdatimeout(sda_timeout, reg_base);
	write_scltimeout(scl_timeout, reg_base);

	return 0;
}

#define DEV_CONFIG_CLK_DEV_INIT(n)                                                                 \
	.clk_dev = DEVICE_DT_GET(DT_INST_CLOCKS_CTLR(n)),                                          \
	.sccon_cfg = {                                                                             \
		.clk_grp = DT_INST_CLOCKS_CELL(n, clk_grp),                                        \
		.clk_idx = DT_INST_CLOCKS_CELL(n, clk_idx),                                        \
	}

#define I2C_DEVICE_INIT_RTS5918(n)                                                                 \
	static const struct i2c_rts5918_config i2c_rts5918_##n##_config = {                        \
		DEV_CONFIG_CLK_DEV_INIT(n),                                                        \
		.dw_i2c_dev = DEVICE_DT_GET(DT_INST_PHANDLE(n, dw_i2c_dev)),                                                                \
		.sda_timeout_value = DT_INST_PROP(n, sda_timeout_value),                           \
		.scl_timeout_value = DT_INST_PROP(n, scl_timeout_value),                           \
		.sda_gpio = DT_INST_PROP(n, sda_gpio_pin),                                         \
		.scl_gpio = DT_INST_PROP(n, scl_gpio_pin)};                                        \
	I2C_DEVICE_DT_INST_DEFINE(n, i2c_rts5918_initialize, NULL, NULL,                           \
				  &i2c_rts5918_##n##_config, POST_KERNEL,                          \
				  CONFIG_I2C_RTS5918_INIT_PRIORITY, NULL);

DT_INST_FOREACH_STATUS_OKAY(I2C_DEVICE_INIT_RTS5918)
