/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2024 Realtek Semiconductor Corporation, SIBG-SD7
 * Author: Lin Yu-Cheng <lin_yu_cheng@realtek.com>
 */

#include <zephyr/kernel.h>
#include <zephyr/pm/pm.h>

#include "device_power.h"

static void rts5918_light_sleep(void)
{

}

static void rts5918_heavy_sleep(void)
{


}

void pm_state_set(enum pm_state state, uint8_t substate_id)
{

	switch (state) {
	case PM_STATE_SUSPEND_TO_IDLE:
		rts5918_light_sleep();
		break;
	case PM_STATE_SUSPEND_TO_RAM:
		rts5918_heavy_sleep();
		break;
	default:
		break;
	}
}

void pm_state_exit_post_ops(enum pm_state state, uint8_t substate_id)
{
	ARG_UNUSED(state);
	ARG_UNUSED(substate_id);

	irq_unlock(0);
}
