/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT realtek_rts5918_pwm

#include <soc.h>
#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/clock_control_rts5918.h>

#include "reg/reg_pwm.h"

LOG_MODULE_REGISTER(pwm, CONFIG_PWM_LOG_LEVEL);

#define PWM_CYCLE_PER_SEC MHZ(50)

struct pwm_rts5918_config {
	volatile struct pwm_regs *pwm_regs;
	uint32_t pwm_clk_grp;
	uint32_t pwm_clk_idx;
	const struct device *clk_dev;
	const struct pinctrl_dev_config *pcfg;
	const bool pwm_invert;
};

static int pwm_rts5918_set_cycles(const struct device *dev, uint32_t channel,
				  uint32_t period_cycles, uint32_t pulse_cycles, pwm_flags_t flags)
{
	const struct pwm_rts5918_config *const pwm_config = dev->config;
	volatile struct pwm_regs *pwm_regs = pwm_config->pwm_regs;

	uint32_t pwm_div, pwm_duty;


	pwm_div = period_cycles;
	pwm_duty = pulse_cycles;
	if (pwm_regs == 0x40252D00) {
		*(volatile uint32_t *)(0x40252D08) = ((5) & (0xFFF)); // Set DIV
		*(volatile uint32_t *)(0x40252D04) = (pwm_duty & (0xFF)); // Set Duty
	} else {
		pwm_regs->div = pwm_div;
		pwm_regs->duty = pwm_duty;
	}

	LOG_DBG("period_cycles=%d, pulse_cycles=%d, pwm_div=%d, pwm_duty=%d", period_cycles,
		pulse_cycles, pwm_div, pwm_duty);

	if (flags == PWM_POLARITY_INVERTED) {
		if (pwm_regs == 0x40252D00) {
			*(volatile uint32_t *)(0x40252D14) |= (0x1 << 29); // Set Revert
		} else {
			pwm_regs->ctrl |= PWM_CTRL_INVT;
		}
	}
	if (pwm_regs == 0x40252D00) {
		*(volatile uint32_t *)(0x40252D00) |= (0x3); // Set as PWM mode
		*(volatile uint32_t *)(0x40252D14) |= (0x1 << 31); // Enable LEDPWM
	} else {
		pwm_regs->ctrl |= PWM_CTRL_EN;
	}

	return 0;
}

static int pwm_rts5918_get_cycles_per_sec(const struct device *dev, uint32_t channel,
					  uint64_t *cycles)
{
	const struct pwm_rts5918_config *const pwm_config = dev->config;
	volatile struct pwm_regs *pwm_regs = pwm_config->pwm_regs;

	if (cycles) {
		if (pwm_regs == 0x40252D00) {
			*cycles = MHZ(8);
		} else {
			*cycles = PWM_CYCLE_PER_SEC;
		}
	}

	return 0;
}

// static DEVICE_API(pwm, pwm_rts5918_driver_api) = {
static const struct pwm_driver_api pwm_rts5918_driver_api = {
	.set_cycles = pwm_rts5918_set_cycles,
	.get_cycles_per_sec = pwm_rts5918_get_cycles_per_sec,
};

static int pwm_rts5918_init(const struct device *dev)
{
	const struct pwm_rts5918_config *const pwm_config = dev->config;
	volatile struct pwm_regs *pwm_regs = pwm_config->pwm_regs;
	struct rts5918_sccon_subsys sccon;

	int rc = 0;
#ifdef CONFIG_PINCTRL
	rc = pinctrl_apply_state(pwm_config->pcfg, PINCTRL_STATE_DEFAULT);
	if (rc < 0) {
		LOG_ERR("PWM pinctrl setup failed (%d)", rc);
		return rc;
	}
#endif
#ifdef CONFIG_CLOCK_CONTROL
	if (!device_is_ready(pwm_config->clk_dev)) {
		return -ENODEV;
	}

	sccon.clk_grp = pwm_config->pwm_clk_grp;
	sccon.clk_idx = pwm_config->pwm_clk_idx;
	rc = clock_control_on(pwm_config->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc != 0) {
		return rc;
	}
#endif
	if(pwm_config->pwm_invert) {
		if (pwm_regs == 0x40252D00) {
		*(volatile uint32_t *)(0x40252D14) |= (0x1 << 29); // Set Revert
		*(volatile uint32_t *)(0x40252D00) |= (0x3); // Set as PWM mode
		*(volatile uint32_t *)(0x40252D14) |= (0x1 << 31); // Enable LEDPWM
		} else {
			pwm_regs->ctrl |= PWM_CTRL_INVT;
		}
	}
	return rc;
}

#define RTS5918_PWM_PINCTRL_DEF(inst) PINCTRL_DT_INST_DEFINE(inst)

#define RTS5918_PWM_CONFIG(inst)                                                                   \
	static struct pwm_rts5918_config pwm_rts5918_config_##inst = {                             \
		.pwm_regs = (struct pwm_regs *)DT_INST_REG_ADDR(inst),                             \
		.pwm_clk_grp = DT_INST_CLOCKS_CELL(inst, clk_grp),                                 \
		.pwm_clk_idx = DT_INST_CLOCKS_CELL(inst, clk_idx),                                 \
		.clk_dev = DEVICE_DT_GET(DT_INST_CLOCKS_CTLR(inst)),                               \
		.pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(inst),                                      \
		.pwm_invert = DT_INST_PROP(inst, pwm_invert),									   \
	};

#define RTS5918_PWM_DEVICE_INIT(index)                                                             \
	RTS5918_PWM_PINCTRL_DEF(index);                                                            \
	RTS5918_PWM_CONFIG(index);                                                                 \
	DEVICE_DT_INST_DEFINE(index, &pwm_rts5918_init, NULL, NULL, &pwm_rts5918_config_##index,   \
			      POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE,                     \
			      &pwm_rts5918_driver_api);

DT_INST_FOREACH_STATUS_OKAY(RTS5918_PWM_DEVICE_INIT)
