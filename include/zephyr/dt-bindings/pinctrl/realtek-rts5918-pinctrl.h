/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2024 Realtek Semiconductor Corporation, SIBG-SD7
 * Author: Lin Yu-Cheng <lin_yu_cheng@realtek.com>
 */

#ifndef ZEPHYR_INCLUDE_DT_BINDINGS_PINCTRL_REALTEK_RTS5918_PINCTRL_H_
#define ZEPHYR_INCLUDE_DT_BINDINGS_PINCTRL_REALTEK_RTS5918_PINCTRL_H_

#include <zephyr/dt-bindings/dt-util.h>

#define REALTEK_RTS5918_GPIO_INOUT     BIT(0) /* IN/OUT : 0 input 1 output */
#define REALTEK_RTS5918_GPIO_PINON     BIT(1) /* Input_detect : 1 enable 0 disable */
#define REALTEK_RTS5918_GPIO_VOLT      BIT(2) /* Pin Volt : 1 1.8V 0 3.3V */
#define REALTEK_RTS5918_FUNC_MSK	   GENMASK(10, 8)
#define REALTEK_RTS5918_FUNC0          0      /* GPIO mode */
#define REALTEK_RTS5918_FUNC1          BIT(8) /* Function mode use BIT0~2 */
#define REALTEK_RTS5918_FUNC2          BIT(9)
#define REALTEK_RTS5918_FUNC3          ((BIT(8)) | (BIT(9)))
#define REALTEK_RTS5918_FUNC4          BIT(10)
#define REALTEK_RTS5918_FUNC5          (BIT(10) | BIT(8))
#define REALTEK_RTS5918_FUNC6          (BIT(10) | BIT(9))
#define REALTEK_RTS5918_FUNC7          (BIT(10) | BIT(9) | BIT(8))

#define REALTEK_RTS5918_INPUT_OUTPUT_POS    0
#define REALTEK_RTS5918_INPUT_DETECTION_POS 1
#define REALTEK_RTS5918_VOLTAGE_POS         2
#define REALTEK_RTS5918_DRV_STR_POS         11
#define REALTEK_RTS5918_SLEW_RATE_POS       12
#define REALTEK_RTS5918_PD_POS              13
#define REALTEK_RTS5918_PU_POS              14
#define REALTEK_RTS5918_SCHMITTER_POS       15
#define REALTEK_RTS5918_TYPE_POS            16
#define REALTEK_RTS5918_HIGH_LOW_POS        17

#define REALTEK_RTS5918_GPIO_HIGH_POS 18
#define REALTEK_RTS5918_GPIO_HIGH_MSK 0x3f
#define REALTEK_RTS5918_GPIO_LOW_POS  3
#define REALTEK_RTS5918_GPIO_LOW_MSK  0x1f

#define FUNC0 REALTEK_RTS5918_FUNC0
#define FUNC1 REALTEK_RTS5918_FUNC1
#define FUNC2 REALTEK_RTS5918_FUNC2
#define FUNC3 REALTEK_RTS5918_FUNC3
#define FUNC4 REALTEK_RTS5918_FUNC4
#define FUNC5 REALTEK_RTS5918_FUNC5
#define FUNC6 REALTEK_RTS5918_FUNC6
#define FUNC7 REALTEK_RTS5918_FUNC7

#define PIN_SEL_POS 8
#define PIN_SEL_MSK 0x700
#define BIT_LENGTH_POS 7
#define BIT_LENGTH_MSK BIT(BIT_LENGTH_POS)
#define BIT_VALUE_POS 5
#define BIT_VALUE_MSK 0x60
#define BIT_SHIFT_POS 0
#define BIT_SHIFT_MSK 0x1F

#define NORMAL_PIN 0
#define IXC_INPUT 1
#define	UART_LPT 2
#define	IP_INPUT 3
#define	SPIC_INPUT 4

#define BIT_LENGTH_1 0
#define BIT_LENGTH_2 1

#define REALTEK_RTS5918_PINMUX(n, f)                                                               \
	(((((n) >> 5) & REALTEK_RTS5918_GPIO_HIGH_MSK) << REALTEK_RTS5918_GPIO_HIGH_POS) |         \
	 (((n) & REALTEK_RTS5918_GPIO_LOW_MSK) << REALTEK_RTS5918_GPIO_LOW_POS) | (f))

#define REALTEK_RTS5918_PIN_SEL(sel, length, value, shift)                           \
	(((sel<<PIN_SEL_POS)& PIN_SEL_MSK) | ((length<<BIT_LENGTH_POS)&BIT_LENGTH_MSK) |  \
	((value<<BIT_VALUE_POS)&BIT_VALUE_MSK) | ((shift<<BIT_SHIFT_POS)&BIT_SHIFT_MSK))

#endif /* ZEPHYR_INCLUDE_DT_BINDINGS_PINCTRL_REALTEK_RTS5918_PINCTRL_H_ */
