/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2024 Realtek Semiconductor Corporation, SIBG-SD7
 * Author: Lin Yu-Cheng <lin_yu_cheng@realtek.com> / Titan Chen
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>
#include "debug_swj.h"

LOG_MODULE_REGISTER(soc, CONFIG_SOC_LOG_LEVEL);

#if defined(CONFIG_RTS5918_ON_ENTER_CPU_IDLE_HOOK)
bool z_arm_on_enter_cpu_idle(void)
{
	/* Returning false prevent device goes to sleep mode */
	return false;
}
#endif

/**
 * @brief Perform basic hardware initialization at boot.
 *
 * This needs to be run from the very beginning.
 */
void soc_early_init_hook(void)
{
	if (!IS_ENABLED(CONFIG_RTS5918_DEBUG_SWJ)) {
		return;
	}

	int ret;

	/* Apply device related preinit configuration */
	ret = swj_connector_init();
	if (ret < 0) {
		LOG_ERR("SWJ init failed");
	}
}

static int soc_disable_vout_130(void)
{
    *((uint32_t*)0x4010010c) |= BIT(31);
    *((uint32_t*)0x40100114) |= BIT(2);
    k_busy_wait(10000);
    *((uint32_t*)0x40100114) &= ~BIT(2);
}
SYS_INIT(soc_disable_vout_130, PRE_KERNEL_2, 5);
 

SYS_INIT(soc_early_init_hook, PRE_KERNEL_1, 0);
