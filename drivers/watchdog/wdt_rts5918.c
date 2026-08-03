/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2024 Realtek Semiconductor Corporation, SIBG-SD7
 * Author: Dylan Hsieh <dylan.hsieh@realtek.com>
 */

#define DT_DRV_COMPAT realtek_rts5918_watchdog

#include <soc.h>
#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/clock_control_rts5918.h>
#include <zephyr/irq.h>

#define LOG_LEVEL CONFIG_WDT_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(wdt_rts5918);

#include "reg/reg_wdt.h"

#define WDT_MAX_CNT   256

struct wdt_rts5918_config {
	uint32_t base;
	uint32_t div;
	const struct device *clk_dev;
	uint32_t clk_grp;
	uint32_t clk_idx;
};

struct wdt_rts5918_data {
	wdt_callback_t callback;
	bool timeout_installed;
	uint32_t timeout;
};

static void wdt_rts5918_isr(const struct device *dev)
{
	const struct wdt_rts5918_config *const cfg = dev->config;
	struct wdt_rts5918_data *data = (struct wdt_rts5918_data *)(dev->data);
	WDT_Type *wdt_reg = (WDT_Type *)cfg->base;

	LOG_DBG("WDT ISR");

	wdt_reg->CTRL |= WDT_CTRL_CLRRSTFLAG_Msk;

	if (data->callback) {
		data->callback(dev, 0);
	}
}

static int wdt_rts5918_setup(const struct device *dev, uint8_t options)
{
	const struct wdt_rts5918_config *const config = dev->config;
	struct wdt_rts5918_data *data = (struct wdt_rts5918_data *)(dev->data);
	WDT_Type *wdt_reg = (WDT_Type *)config->base;

	if (!data->timeout_installed) {
		LOG_ERR("No valid WDT timeout installed");
		return -EINVAL;
	}

	if (wdt_reg->CTRL & WDT_CTRL_EN_Msk) {
		LOG_ERR("WDT is already running");
		return -EBUSY;
	}

	if ((options & WDT_OPT_PAUSE_IN_SLEEP) != 0) {
		LOG_ERR("WDT_OPT_PAUSE_IN_SLEEP is not supported");
		return -ENOTSUP;
	}

	if (options & WDT_OPT_PAUSE_HALTED_BY_DBG) {
		LOG_ERR("Pause when halted by debugger not supported");
		return -ENOTSUP;
	}

	irq_enable(DT_INST_IRQN(0));

	wdt_reg->INTEN = WDT_INTEN_WDTINTEN_Msk;
	wdt_reg->CTRL |= (WDT_CTRL_CLRRSTFLAG_Msk | WDT_CTRL_RELOAD_Msk);
	wdt_reg->CTRL |= WDT_CTRL_EN_Msk;

	LOG_DBG("WDT setup and enabled");

	return 0;
}

static int wdt_rts5918_disable(const struct device *dev)
{
	const struct wdt_rts5918_config *const config = dev->config;
	struct wdt_rts5918_data *data = (struct wdt_rts5918_data *)(dev->data);
	WDT_Type *wdt_reg = (WDT_Type *)config->base;

	if (!(wdt_reg->CTRL & WDT_CTRL_EN_Msk)) {
		return -EALREADY;
	}

	wdt_reg->INTEN = 0ul;
	wdt_reg->CTRL |= WDT_CTRL_CLRRSTFLAG_Msk;
	wdt_reg->CTRL &= ~WDT_CTRL_EN_Msk;

	data->timeout_installed = false;

	LOG_DBG("WDT disabled");

	return 0;
}

static int wdt_rts5918_install_timeout(const struct device *dev,
				       const struct wdt_timeout_cfg *config)
{
	const struct wdt_rts5918_config *const cfg = dev->config;
	struct wdt_rts5918_data *data = (struct wdt_rts5918_data *)(dev->data);
	WDT_Type *wdt_reg = (WDT_Type *)cfg->base;

	uint32_t timeout;
	uint32_t max, min;

	LOG_DBG("WDT intstall timeout");

	if (wdt_reg->CTRL & WDT_CTRL_EN_Msk) {
		LOG_ERR("WDT is already running");
		return -EBUSY;
	}

	if (config->window.min > 0U) {
		LOG_ERR("Lower limit of watchdog is not supported, keep it zero");
		data->timeout_installed = false;
		return -EINVAL;
	}

	switch (config->flags) {
	case WDT_FLAG_RESET_SOC:
		wdt_reg->CTRL |= WDT_CTRL_RSTEN_Msk;
		break;
	case WDT_FLAG_RESET_NONE:
		wdt_reg->CTRL &= ~WDT_CTRL_RSTEN_Msk;
		break;
	case WDT_FLAG_RESET_CPU_CORE:
		LOG_ERR("WDT_FLAG_RESET_CPU_CORE is not supported\n");
		break;
	default:
		LOG_ERR("Unsupported watchdog config Flag\n");
		return -ENOTSUP;
	}

	timeout = config->window.max * 1000U;
	min =  (cfg->div * 1000000) / 32768UL;
	max = min * WDT_MAX_CNT;

	LOG_DBG("min: %d ms, max: %d ms", min / 1000U, max / 1000U);
	if ((timeout < min) || (timeout > max)) {
		LOG_ERR("Invalid timeout value allowed range:"
			"%d ms to %d ms", min / 1000U, max / 1000U);
		return -EINVAL;
	}

	wdt_reg->CNT = timeout / min;

	data->callback = config->callback;
	data->timeout_installed = true;

	LOG_DBG("DIV: 0x%08x, CNT: 0x%08x", wdt_reg->DIV, wdt_reg->CNT);

	return 0;
}

static int wdt_rts5918_feed(const struct device *dev, int channel_id)
{
	const struct wdt_rts5918_config *const cfg = dev->config;
	WDT_Type *wdt_reg = (WDT_Type *)cfg->base;

	ARG_UNUSED(dev);
	ARG_UNUSED(channel_id);

	if (!(wdt_reg->CTRL & WDT_CTRL_EN_Msk)) {
		return -EINVAL;
	}

	wdt_reg->CTRL |= WDT_CTRL_RELOAD_Msk;

	LOG_DBG("WDT feed");

	return 0;
}

static const struct wdt_driver_api wdt_rts5918_api = {
	.setup = wdt_rts5918_setup,
	.disable = wdt_rts5918_disable,
	.install_timeout = wdt_rts5918_install_timeout,
	.feed = wdt_rts5918_feed,
};

static int wdt_rts5918_init(const struct device *dev)
{
	int rc;

	const struct wdt_rts5918_config *const cfg = dev->config;
	struct rts5918_sccon_subsys sccon;
	WDT_Type *wdt_reg = (WDT_Type *)cfg->base;

	LOG_DBG("WDT init");

	if (!device_is_ready(cfg->clk_dev)) {
		return -ENODEV;
	}

	sccon.clk_grp = cfg->clk_grp;
	sccon.clk_idx = cfg->clk_idx;
	rc = clock_control_on(cfg->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc != 0) {
		return rc;
	}

	if (IS_ENABLED(CONFIG_WDT_DISABLE_AT_BOOT)) {
		wdt_rts5918_disable(dev);
	}

	wdt_reg->CTRL = 0ul;
	wdt_reg->DIV = cfg->div;

	wdt_reg->CTRL |= WDT_CTRL_CLRRSTFLAG_Msk;
	NVIC_ClearPendingIRQ(DT_INST_IRQN(0));

	IRQ_CONNECT(DT_INST_IRQN(0), 0, wdt_rts5918_isr, DEVICE_DT_INST_GET(0), 0);
	irq_enable(DT_INST_IRQN(0));

	return 0;
}

static const struct wdt_rts5918_config wdt_rts5918_cfg = {
	.base = DT_INST_REG_ADDR(0),
	.div = DT_INST_PROP(0, clk_divider),
	.clk_dev = DEVICE_DT_GET(DT_INST_CLOCKS_CTLR(0)),
	.clk_grp = DT_CLOCKS_CELL_BY_NAME(DT_NODELABEL(wdog), watchdog, clk_grp),
	.clk_idx = DT_CLOCKS_CELL_BY_NAME(DT_NODELABEL(wdog), watchdog, clk_idx),
};

static struct wdt_rts5918_data wdt_rts5918_dev_data;

DEVICE_DT_INST_DEFINE(0, wdt_rts5918_init, NULL, &wdt_rts5918_dev_data, &wdt_rts5918_cfg,
		      PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &wdt_rts5918_api);