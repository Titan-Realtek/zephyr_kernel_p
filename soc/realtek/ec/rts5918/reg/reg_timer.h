/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @brief 32-bit Timer Controller (TMR)
 */

struct timer32_type {
	uint32_t ldcnt;
	uint32_t cnt;
	uint32_t ctrl;
	uint32_t intsts;
};

/* CTRL */
#define TIMER32_CTRL_EN BIT(0)

#define TIMER32_CTRL_MDSELS_ONESHOT 0
#define TIMER32_CTRL_MDSELS_PERIOD  BIT(1)

#define TIMER32_CTRL_INTEN_EN BIT(2)
/* INTSTS */
#define TIMER32_INTSTS_STS     (0UL)