/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2024 Realtek Semiconductor Corporation, SIBG-SD7
 * Author: Lin Yu-Cheng <lin_yu_cheng@realtek.com>
 */

#ifndef ZEPHYR_INCLUDE_DT_BINDINGS_RTS5918_CLOCK_H_
#define ZEPHYR_INCLUDE_DT_BINDINGS_RTS5918_CLOCK_H_

#define    RTS5918_CLK_ESPI 		0
#define    RTS5918_CLK_LEDPWM       1
#define    RTS5918_CLK_LOW          2
#define    RTS5918_CLK_I2C          3
#define    RTS5918_CLK_RC25M        4
#define    RTS5918_CLK_I3C          5
#define    RTS5918_CLK_PECI         6
#define    RTS5918_CLK_PS2_32K      7
#define    RTS5918_CLK_APB          8

#define    RTS5918_ESPI_PWR       0
#define    RTS5918_KBC_PWR        1
#define    RTS5918_ACPI_PWR       2
#define    RTS5918_686C_PWR       3
#define    RTS5918_6A6E_PWR       4
#define    RTS5918_7478_PWR       5
#define    RTS5918_7A7C_PWR       6
#define    RTS5918_DBG80_PWR      7
#define    RTS5918_EMI0_PWR       8
#define    RTS5918_EMI1_PWR       9
#define    RTS5918_EMI2_PWR       10
#define    RTS5918_EMI3_PWR       11
#define    RTS5918_EMI4_PWR       12
#define    RTS5918_EMI5_PWR       13
#define    RTS5918_EMI6_PWR       14
#define    RTS5918_EMI7_PWR       15
#define    RTS5918_LED1_PWR       16
#define    RTS5918_LED2_PWR       17
#define    RTS5918_LED3_PWR       18
#define    RTS5918_LED4_PWR       19
#define    RTS5918_PWM1_PWR       20
#define    RTS5918_PWM2_PWR       21
#define    RTS5918_PWM3_PWR       22
#define    RTS5918_PWM4_PWR       23
#define    RTS5918_PWM5_PWR       24
#define    RTS5918_PWM6_PWR       25
#define    RTS5918_PWM7_PWR       26
#define    RTS5918_PWM8_PWR       27
#define    RTS5918_PWM9_PWR       28
#define    RTS5918_PWM10_PWR      29
#define    RTS5918_PWM11_PWR      30
#define    RTS5918_PWM12_PWR      31
#define    RTS5918_PWM13_PWR      32
#define    RTS5918_PWM14_PWR      33
#define    RTS5918_ADC_PWR        34
#define    RTS5918_UART0_PWR      35
#define    RTS5918_UART1_PWR      36
#define    RTS5918_UART2_PWR      37
#define    RTS5918_I2C1_PWR       38
#define    RTS5918_I2C2_PWR       39
#define    RTS5918_I2C3_PWR       40
#define    RTS5918_I2C4_PWR       41
#define    RTS5918_I2C5_PWR       42
#define    RTS5918_I2C6_PWR       43
#define    RTS5918_I2C7_PWR       44
#define    RTS5918_I2C8_PWR       45
#define    RTS5918_I2C9_PWR       46
#define    RTS5918_I2C10_PWR      47
#define    RTS5918_I2C11_PWR      48
#define    RTS5918_PS2_1M_1_PWR   49
#define    RTS5918_PS2_1M_2_PWR   50
#define    RTS5918_PS2_1M_3_PWR   51
#define    RTS5918_TACH1_PWR      52
#define    RTS5918_TACH2_PWR      53
#define    RTS5918_TACH3_PWR      54
#define    RTS5918_TACH4_PWR      55
#define    RTS5918_TACH5_PWR      56
#define    RTS5918_TACH6_PWR      57
#define    RTS5918_TACH7_PWR      58
#define    RTS5918_TACH8_PWR      59
#define    RTS5918_TACH9_PWR      60
#define    RTS5918_TACH10_PWR     61
#define    RTS5918_TACH11_PWR     62
#define    RTS5918_TACH12_PWR     63
#define    RTS5918_TACH13_PWR     64
#define    RTS5918_TACH14_PWR     65
#define    RTS5918_GPIO_PWR       66
#define    RTS5918_TIMER0_PWR     67
#define    RTS5918_TIMER1_PWR     68
#define    RTS5918_TIMER2_PWR     69
#define    RTS5918_TIMER3_PWR     70
#define    RTS5918_TIMER4_PWR     71
#define    RTS5918_TIMER5_PWR     72
#define    RTS5918_KBM_PWR        73
#define    RTS5918_SLWTMR0_PWR    74
#define    RTS5918_SLWTMR1_PWR    75
#define    RTS5918_SLWTMR2_PWR    76
#define    RTS5918_SLWTMR3_PWR    77
#define    RTS5918_LEDSTP_PWR     78
#define    RTS5918_CEC1_PWR       79
#define    RTS5918_CEC2_PWR       80
#define    RTS5918_MICROCNT_PWR   81
#define    RTS5918_I2CDBG_PWR     82
#define    RTS5918_VINLED_PWR     83
#define    RTS5918_LPT_PWR        84
#define    RTS5918_I3C1_PWR       85
#define    RTS5918_I3C2_PWR       86
#define    RTS5918_I3C3_PWR       87
#define    RTS5918_I3C4_PWR       88
#define    RTS5918_PECI_PWR       89
#define    RTS5918_PS2_32K_1_PWR  90
#define    RTS5918_PS2_32K_2_PWR  91
#define    RTS5918_PS2_32K_3_PWR  92
#define    RTS5918_RTMR_PWR       93
#define    RTS5918_PWRBTN0_PWR    94
#define    RTS5918_PWRBTN1_PWR    95
#define    RTS5918_RTC_PWR        96
#define    RTS5918_SPIS_PWR       97
#define    RTS5918_SPIM_PWR       98
#define    RTS5918_WDT_PWR        99
#define    RTS5918_TRC_PWR		  100

#endif /* ZEPHYR_INCLUDE_DT_BINDINGS_RTS5918_CLOCK_H_ */
