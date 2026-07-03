/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2024 Realtek Semiconductor Corporation, SIBG-SD7
 * Author: Lin Yu-Cheng <lin_yu_cheng@realtek.com>
 */

#define DT_DRV_COMPAT realtek_rts5918_sccon

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/clock_control/clock_control_rts5918.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/dt-bindings/clock/rts5918_clock.h>

#include <reg/reg_system.h>

LOG_MODULE_REGISTER(sccon, CONFIG_CLOCK_CONTROL_LOG_LEVEL);

#define RC25M_FREQ DT_PROP(DT_INST_CLOCKS_CTLR_BY_NAME(0, rc25m), clock_frequency)
#define PLL_FREQ   DT_PROP(DT_INST_CLOCKS_CTLR_BY_NAME(0, pll), clock_frequency)
#define RTS5918_MAXIMUM_POLLING_TIME_US (50 * USEC_PER_MSEC)
#define ESPI_ALL_MSK 																	\
	(SYSTEM_IPCLK0__ESPI_Msk | SYSTEM_IPCLK0__KBC_Msk | SYSTEM_IPCLK0__ACPI_Msk	     \
	| SYSTEM_IPCLK0__PMPORT0_Msk | SYSTEM_IPCLK0__PMPORT1_Msk | SYSTEM_IPCLK0__PMPORT2_Msk  	\
	| SYSTEM_IPCLK0__PMPORT3_Msk | SYSTEM_IPCLK0__PORT80_Msk | SYSTEM_IPCLK0__EMI0_Msk			\
	| SYSTEM_IPCLK0__EMI1_Msk | SYSTEM_IPCLK0__EMI2_Msk | SYSTEM_IPCLK0__EMI3_Msk			\
	| SYSTEM_IPCLK0__EMI4_Msk | SYSTEM_IPCLK0__EMI5_Msk | SYSTEM_IPCLK0__EMI6_Msk			\
	| SYSTEM_IPCLK0__EMI7_Msk )

#define LED_ALL_MSK_CLK0																	\
	(SYSTEM_IPCLK0__LED1_Msk | SYSTEM_IPCLK0__LED2_Msk | SYSTEM_IPCLK0__LED3_Msk	     \
	| SYSTEM_IPCLK0__LED4_Msk | SYSTEM_IPCLK0__PWM1_Msk | SYSTEM_IPCLK0__PWM2_Msk  	\
	| SYSTEM_IPCLK0__PWM3_Msk | SYSTEM_IPCLK0__PWM4_Msk | SYSTEM_IPCLK0__PWM5_Msk			\
	| SYSTEM_IPCLK0__PWM6_Msk | SYSTEM_IPCLK0__PWM7_Msk | SYSTEM_IPCLK0__PWM8_Msk )

#define LED_ALL_MSK_CLK1																	\
	(SYSTEM_IPCLK1__PWM9_Msk | SYSTEM_IPCLK1__PWM10_Msk | SYSTEM_IPCLK1__PWM11_Msk	     \
	| SYSTEM_IPCLK1__PWM12_Msk | SYSTEM_IPCLK1__PWM13_Msk | SYSTEM_IPCLK1__PWM14_Msk )

#define LOW_ALL_MSK_CLK1																	\
	(SYSTEM_IPCLK1__ADC_Msk | SYSTEM_IPCLK1__UART0_Msk | SYSTEM_IPCLK1__UART1_Msk	     \
	| SYSTEM_IPCLK1__UART2_Msk )

#define I2C_ALL_MSK_CLK1																	\
	(SYSTEM_IPCLK1__I2C1_Msk | SYSTEM_IPCLK1__I2C2_Msk | SYSTEM_IPCLK1__I2C3_Msk	     \
	| SYSTEM_IPCLK1__I2C4_Msk | SYSTEM_IPCLK1__I2C5_Msk | SYSTEM_IPCLK1__I2C6_Msk 			\
	| SYSTEM_IPCLK1__I2C7_Msk | SYSTEM_IPCLK1__I2C8_Msk | SYSTEM_IPCLK1__I2C9_Msk			\
	| SYSTEM_IPCLK1__I2C10_Msk | SYSTEM_IPCLK1__I2C11_Msk  )

#define RC25M_ALL_MSK_CLK1																	\
	(SYSTEM_IPCLK1__PS2_H1_Msk | SYSTEM_IPCLK1__PS2_H2_Msk | SYSTEM_IPCLK1__PS2_H3_Msk	     \
	| SYSTEM_IPCLK1__TACH1_Msk | SYSTEM_IPCLK1__TACH2_Msk | SYSTEM_IPCLK1__TACH3_Msk 		\
	| SYSTEM_IPCLK1__TACH4_Msk | SYSTEM_IPCLK1__TACH5_Msk )

#define RC25M_ALL_MSK_CLK2																	\
	(SYSTEM_IPCLK2__TACH6_Msk | SYSTEM_IPCLK2__TACH7_Msk | SYSTEM_IPCLK2__TACH8_Msk	     \
	| SYSTEM_IPCLK2__TACH9_Msk | SYSTEM_IPCLK2__TACH10_Msk | SYSTEM_IPCLK2__TACH11_Msk 			\
	| SYSTEM_IPCLK2__TACH12_Msk | SYSTEM_IPCLK2__TACH13_Msk | SYSTEM_IPCLK2__TACH14_Msk			\
	| SYSTEM_IPCLK2__GPIO_Msk | SYSTEM_IPCLK2__TMR0_Msk | SYSTEM_IPCLK2__TMR1_Msk			\
	| SYSTEM_IPCLK2__TMR2_Msk | SYSTEM_IPCLK2__TMR3_Msk | SYSTEM_IPCLK2__TMR4_Msk			\
	| SYSTEM_IPCLK2__TMR5_Msk | SYSTEM_IPCLK2__KBM_Msk | SYSTEM_IPCLK2__SLWTMR0_Msk			\
	| SYSTEM_IPCLK2__SLWTMR1_Msk | SYSTEM_IPCLK2__SLWTMR2_Msk | SYSTEM_IPCLK2__SLWTMR3_Msk			\
	| SYSTEM_IPCLK2__LEDSTP_Msk | SYSTEM_IPCLK2__CEC1_Msk | SYSTEM_IPCLK2__CEC2_Msk			\
	| SYSTEM_IPCLK2__MICROCNT_Msk | SYSTEM_IPCLK2__I2CDBG_Msk | SYSTEM_IPCLK2__VINLED_Msk \
	| SYSTEM_IPCLK2__LPT_Msk)

#define RC32K_ALL_MSK_CLK3																	\
	(SYSTEM_IPCLK3__PS2_L1_Msk | SYSTEM_IPCLK3__PS2_L2_Msk | SYSTEM_IPCLK3__PS2_L3_Msk	     \
	| SYSTEM_IPCLK3__RTMR_Msk )

struct rts5918_sccon_config {
	uint32_t reg_base;
	uint8_t sysclk_src;
	uint8_t sysclk_div;
};

static int rts5918_periph_clock_control(const struct device *dev, clock_control_subsys_t sub_system,
					bool on_off)
{
	const struct rts5918_sccon_config *const config = dev->config;
	struct rts5918_sccon_subsys *subsys = (struct rts5918_sccon_subsys *)sub_system;

	volatile SYSTEM_Type *sys_reg = (volatile SYSTEM_Type *)config->reg_base;

	uint32_t clk_grp = subsys->clk_grp;
	uint32_t clk_idx = subsys->clk_idx;

	switch (clk_grp) {
		case RTS5918_CLK_ESPI:
			if( true == on_off ){
				sys_reg->SYSCLK |= SYSTEM_SYSCLK__ESPI_Msk;
				sys_reg->SYSCLKSEL |= (0x1 << SYSTEM_SYSCLKSEL__ESPI_Pos);
				sys_reg->SYSCLKSEL |= (0x1 << SYSTEM_SYSCLKSEL_APB_Pos);
				*(volatile uint32_t *)(0x40100230) |= (0x1 << 25);
				*(volatile uint32_t *)(0x40100230) |= (0x1 << 26);
				sys_reg->ESPI_CLK_PAD |= SYSTEM_ESPI_CLK_PAD_DRIVING_Msk;
				sys_reg->ESPI_CLK_PAD |= SYSTEM_ESPI_PAD_INPUTDET_Msk;
				sys_reg->ESPI_IO0_PAD |= SYSTEM_ESPI_CLK_PAD_DRIVING_Msk;
				sys_reg->ESPI_IO0_PAD |= SYSTEM_ESPI_PAD_INPUTDET_Msk;
				sys_reg->ESPI_IO1_PAD |= SYSTEM_ESPI_CLK_PAD_DRIVING_Msk;
				sys_reg->ESPI_IO1_PAD |= SYSTEM_ESPI_PAD_INPUTDET_Msk;
				sys_reg->ESPI_IO2_PAD |= SYSTEM_ESPI_CLK_PAD_DRIVING_Msk;
				sys_reg->ESPI_IO2_PAD |= SYSTEM_ESPI_PAD_INPUTDET_Msk;
				sys_reg->ESPI_IO3_PAD |= SYSTEM_ESPI_CLK_PAD_DRIVING_Msk;
				sys_reg->ESPI_IO3_PAD |= SYSTEM_ESPI_PAD_INPUTDET_Msk;
				switch (clk_idx) {
				case RTS5918_ESPI_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__ESPI_Msk;
					break;
				case RTS5918_KBC_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__KBC_Msk;
					break;
				case RTS5918_ACPI_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__ACPI_Msk;
					break;
				case RTS5918_686C_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PMPORT0_Msk;
					break;
				case RTS5918_6A6E_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PMPORT1_Msk;
					break;
				case RTS5918_7478_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PMPORT2_Msk;
					break;
				case RTS5918_7A7C_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PMPORT3_Msk;
					break;
				case RTS5918_DBG80_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PORT80_Msk;
					break;
				case RTS5918_EMI0_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__EMI0_Msk;
					break;
				case RTS5918_EMI1_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__EMI1_Msk;
					break;
				case RTS5918_EMI2_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__EMI2_Msk;
					break;
				case RTS5918_EMI3_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__EMI3_Msk;
					break;
				case RTS5918_EMI4_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__EMI4_Msk;
					break;
				case RTS5918_EMI5_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__EMI5_Msk;
					break;
				case RTS5918_EMI6_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__EMI6_Msk;
					break;
				case RTS5918_EMI7_PWR:
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__EMI7_Msk;
					break;
				}
			} else {
				switch (clk_idx) {
				case RTS5918_ESPI_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__ESPI_Msk;
					break;
				case RTS5918_KBC_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__KBC_Msk;
					break;
				case RTS5918_ACPI_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__ACPI_Msk;
					break;
				case RTS5918_686C_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PMPORT0_Msk;
					break;
				case RTS5918_6A6E_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PMPORT1_Msk;
					break;
				case RTS5918_7478_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PMPORT2_Msk;
					break;
				case RTS5918_7A7C_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PMPORT3_Msk;
					break;
				case RTS5918_DBG80_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PORT80_Msk;
					break;
				case RTS5918_EMI0_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__EMI0_Msk;
					break;
				case RTS5918_EMI1_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__EMI1_Msk;
					break;
				case RTS5918_EMI2_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__EMI2_Msk;
					break;
				case RTS5918_EMI3_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__EMI3_Msk;
					break;
				case RTS5918_EMI4_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__EMI4_Msk;
					break;
				case RTS5918_EMI5_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__EMI5_Msk;
					break;
				case RTS5918_EMI6_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__EMI6_Msk;
					break;
				case RTS5918_EMI7_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__EMI7_Msk;
					break;
				}
				if((sys_reg->IPCLK0 & ESPI_ALL_MSK) == 0x0){
					sys_reg->SYSCLK &= ~SYSTEM_SYSCLK__ESPI_Msk;
				}
			}
			break;
		case RTS5918_CLK_LEDPWM:
			if( true == on_off ){
				sys_reg->SYSCLK |= SYSTEM_SYSCLK__LED_PWM_Msk;
				sys_reg->SYSCLKSEL |= (0x0 << 20); // RC25M
				switch (clk_idx) {
				case RTS5918_LED1_PWR:
					sys_reg->IPDIV1_b._LED1 = 0; // 25M / DIV780
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__LED1_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__LED1_Msk;
					break;
				case RTS5918_LED2_PWR:
					sys_reg->IPDIV1_b._LED2 = 0; // 25M / DIV780
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__LED2_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__LED2_Msk;
					break;
				case RTS5918_LED3_PWR:
					sys_reg->IPDIV1_b._LED3 = 0; // 25M / DIV780
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__LED3_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__LED3_Msk;
					break;
				case RTS5918_LED4_PWR:
					sys_reg->IPDIV1_b._LED4 = 0; // 25M / DIV780
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__LED4_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__LED4_Msk;
					break;
				case RTS5918_PWM1_PWR:
					sys_reg->IPDIV1 |= (0x10 << 14); // 25M / DIV1
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PWM1_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM1_Msk;
					break;
				case RTS5918_PWM2_PWR:
					sys_reg->IPDIV1 |= (0x10 << 12); // 25M / DIV1
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PWM2_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM2_Msk;
					break;
				case RTS5918_PWM3_PWR:
					sys_reg->IPDIV1 |= (0x10 << 10); // 25M / DIV1
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PWM3_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM3_Msk;
					break;
				case RTS5918_PWM4_PWR:
					sys_reg->IPDIV1 |= (0x10 << 8); // 25M / DIV1
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PWM4_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM4_Msk;
					break;
				case RTS5918_PWM5_PWR:
					sys_reg->IPDIV1 |= (0x10 << 6); // 25M / DIV1
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PWM5_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM5_Msk;
					break;
				case RTS5918_PWM6_PWR:
					sys_reg->IPDIV1 |= (0x10 << 4); // 25M / DIV1
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PWM6_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM6_Msk;
					break;
				case RTS5918_PWM7_PWR:
					sys_reg->IPDIV1 |= (0x10 << 2); // 25M / DIV1
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PWM7_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM7_Msk;
					break;
				case RTS5918_PWM8_PWR:
					sys_reg->IPDIV1 |= (0x10 << 0); // 25M / DIV1
					sys_reg->IPCLK0 |= SYSTEM_IPCLK0__PWM8_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM8_Msk;
					break;
				case RTS5918_PWM9_PWR:
					sys_reg->IPDIV2 |= (0x10 << 29); // 25M / DIV1
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__PWM9_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM9_Msk;
					break;
				case RTS5918_PWM10_PWR:
					sys_reg->IPDIV2 |= (0x10 << 27); // 25M / DIV1
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__PWM10_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM10_Msk;
					break;
				case RTS5918_PWM11_PWR:
					sys_reg->IPDIV2 |= (0x10 << 25); // 25M / DIV1
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__PWM11_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM11_Msk;
					break;
				case RTS5918_PWM12_PWR:
					sys_reg->IPDIV2 |= (0x10 << 23); // 25M / DIV1
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__PWM12_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM12_Msk;
					break;
				case RTS5918_PWM13_PWR:
					sys_reg->IPDIV2 |= (0x10 << 21); // 25M / DIV1
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__PWM13_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM13_Msk;
					break;
				case RTS5918_PWM14_PWR:
					sys_reg->IPDIV2 |= (0x10 << 19); // 25M / DIV1
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__PWM14_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWM14_Msk;
					break;
				}
			} else {
				switch (clk_idx) {
				case RTS5918_LED1_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__LED1_Msk;
					break;
				case RTS5918_LED2_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__LED2_Msk;
					break;
				case RTS5918_LED3_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__LED3_Msk;
					break;
				case RTS5918_LED4_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__LED4_Msk;
					break;
				case RTS5918_PWM1_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PWM1_Msk;
					break;
				case RTS5918_PWM2_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PWM2_Msk;
					break;
				case RTS5918_PWM3_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PWM3_Msk;
					break;
				case RTS5918_PWM4_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PWM4_Msk;
					break;
				case RTS5918_PWM5_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PWM5_Msk;
					break;
				case RTS5918_PWM6_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PWM6_Msk;
					break;
				case RTS5918_PWM7_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PWM7_Msk;
					break;
				case RTS5918_PWM8_PWR:
					sys_reg->IPCLK0 &= ~SYSTEM_IPCLK0__PWM8_Msk;
					break;
				case RTS5918_PWM9_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__PWM9_Msk;
					break;
				case RTS5918_PWM10_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__PWM10_Msk;
					break;
				case RTS5918_PWM11_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__PWM11_Msk;
					break;
				case RTS5918_PWM12_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__PWM12_Msk;
					break;
				case RTS5918_PWM13_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__PWM13_Msk;
					break;
				case RTS5918_PWM14_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__PWM14_Msk;
					break;
				}
				if(((sys_reg->IPCLK0 & LED_ALL_MSK_CLK0) == 0x0) && ((sys_reg->IPCLK1 & LED_ALL_MSK_CLK1) == 0x0)){
					sys_reg->SYSCLK &= ~SYSTEM_SYSCLK__LED_PWM_Msk;
				}
			}
			break;
		case RTS5918_CLK_LOW:
			if( true == on_off ){
				sys_reg->SYSCLK |= SYSTEM_SYSCLK__LOWCLK_Msk;
				switch (clk_idx) {
				case RTS5918_ADC_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__ADC_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__ADC_Msk;
					break;
				case RTS5918_UART0_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__UART0_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__UART0_Msk;
					break;
				case RTS5918_UART1_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__UART1_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__UART1_Msk;
					break;
				case RTS5918_UART2_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__UART2_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__UART2_Msk;
					break;
				}
			} else {
				switch (clk_idx) {
				case RTS5918_ADC_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__ADC_Msk;
					break;
				case RTS5918_UART0_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__UART0_Msk;
					break;
				case RTS5918_UART1_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__UART1_Msk;
					break;
				case RTS5918_UART2_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__UART2_Msk;
					break;
				}
				if((sys_reg->IPCLK1 & LOW_ALL_MSK_CLK1) == 0x0){
					sys_reg->SYSCLK &= ~SYSTEM_SYSCLK__LOWCLK_Msk;
				}
			}
			break;
		case RTS5918_CLK_I2C:
			if( true == on_off ){
				sys_reg->SYSCLK |= SYSTEM_SYSCLK__I2C_Msk;
				//sys_reg->SYSCLKSEL |= (0x01 << SYSTEM_SYSCLKSEL__I2C_Pos);
				switch (clk_idx) {
				case RTS5918_I2C1_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__I2C1_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__I2C1_Msk;
					break;
				case RTS5918_I2C2_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__I2C2_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__I2C2_Msk;
					break;
				case RTS5918_I2C3_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__I2C3_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__I2C3_Msk;
					break;
				case RTS5918_I2C4_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__I2C4_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__I2C4_Msk;
					break;
				case RTS5918_I2C5_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__I2C5_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__I2C5_Msk;
					break;
				case RTS5918_I2C6_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__I2C6_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__I2C6_Msk;
					break;
				case RTS5918_I2C7_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__I2C7_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__I2C7_Msk;
					break;
				case RTS5918_I2C8_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__I2C8_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__I2C8_Msk;
					break;
				case RTS5918_I2C9_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__I2C9_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__I2C9_Msk;
					break;
				case RTS5918_I2C10_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__I2C10_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__I2C10_Msk;
					break;
				case RTS5918_I2C11_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__I2C11_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__I2C11_Msk;
					break;
				}
			} else {
				switch (clk_idx) {
				case RTS5918_I2C1_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__I2C1_Msk;
					break;
				case RTS5918_I2C2_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__I2C2_Msk;
					break;
				case RTS5918_I2C3_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__I2C3_Msk;
					break;
				case RTS5918_I2C4_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__I2C4_Msk;
					break;
				case RTS5918_I2C5_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__I2C5_Msk;
					break;
				case RTS5918_I2C6_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__I2C6_Msk;
					break;
				case RTS5918_I2C7_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__I2C7_Msk;
					break;
				case RTS5918_I2C8_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__I2C8_Msk;
					break;
				case RTS5918_I2C9_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__I2C9_Msk;
					break;
				case RTS5918_I2C10_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__I2C10_Msk;
					break;
				case RTS5918_I2C11_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__I2C11_Msk;
					break;
				}
				if((sys_reg->IPCLK1 & I2C_ALL_MSK_CLK1) == 0x0){
					sys_reg->SYSCLK &= ~SYSTEM_SYSCLK__I2C_Msk;
				}
			}
			break;
		case RTS5918_CLK_RC25M:
			if( true == on_off ){
				sys_reg->SYSCLK |= SYSTEM_SYSCLK__RC25M_Msk;
				switch (clk_idx) {
				case RTS5918_PS2_1M_1_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__PS2_H1_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__PS2_1_Msk;
					break;
				case RTS5918_PS2_1M_2_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__PS2_H2_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__PS2_2_Msk;
					break;
				case RTS5918_PS2_1M_3_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__PS2_H3_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__PS2_3_Msk;
					break;
				case RTS5918_TACH1_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__TACH1_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__TACH1_Msk;
					break;
				case RTS5918_TACH2_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__TACH2_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__TACH2_Msk;
					break;
				case RTS5918_TACH3_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__TACH3_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__TACH3_Msk;
					break;
				case RTS5918_TACH4_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__TACH4_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__TACH4_Msk;
					break;
				case RTS5918_TACH5_PWR:
					sys_reg->IPCLK1 |= SYSTEM_IPCLK1__TACH5_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__TACH5_Msk;
					break;
				case RTS5918_TACH6_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TACH6_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__TACH6_Msk;
					break;
				case RTS5918_TACH7_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TACH7_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__TACH7_Msk;
					break;
				case RTS5918_TACH8_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TACH8_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__TACH8_Msk;
					break;
				case RTS5918_TACH9_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TACH9_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TACH9_Msk;
					break;
				case RTS5918_TACH10_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TACH10_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TACH10_Msk;
					break;
				case RTS5918_TACH11_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TACH11_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TACH11_Msk;
					break;
				case RTS5918_TACH12_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TACH12_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TACH12_Msk;
					break;
				case RTS5918_TACH13_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TACH13_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TACH13_Msk;
					break;
				case RTS5918_TACH14_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TACH14_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TACH14_Msk;
					break;
				case RTS5918_GPIO_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__GPIO_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__GPIO_Msk;
					break;
				case RTS5918_TIMER0_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TMR0_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TMR0_Msk;
					break;
				case RTS5918_TIMER1_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TMR1_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TMR1_Msk;
					break;
				case RTS5918_TIMER2_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TMR2_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TMR2_Msk;
					break;
				case RTS5918_TIMER3_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TMR3_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TMR3_Msk;
					break;
				case RTS5918_TIMER4_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TMR4_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TMR4_Msk;
					break;
				case RTS5918_TIMER5_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TMR5_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TMR5_Msk;
					break;
				case RTS5918_KBM_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__KBM_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__KBM_Msk;
					break;
	/*          // wait new header
				case RTS5918_LPC_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__TACH14_Msk;
					break;
	*/
				case RTS5918_SLWTMR0_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__SLWTMR0_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__SLWTMR0_Msk;
					break;
				case RTS5918_SLWTMR1_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__SLWTMR1_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__SLWTMR1_Msk;
					break;
				case RTS5918_SLWTMR2_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__SLWTMR2_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__SLWTMR2_Msk;
					break;
				case RTS5918_SLWTMR3_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__SLWTMR3_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__SLWTMR3_Msk;
					break;
				case RTS5918_LEDSTP_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__LEDSTP_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__LEDSTP_Msk;
					break;
				case RTS5918_CEC1_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__CEC1_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__CEC1_Msk;
					break;
				case RTS5918_CEC2_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__CEC2_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__CEC2_Msk;
					break;
				case RTS5918_MICROCNT_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__MICROCNT_Msk;
					break;
				case RTS5918_I2CDBG_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__I2CDBG_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__I2CDBG_Msk;
					break;
				case RTS5918_VINLED_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__VINLED_Msk;
					break;
				case RTS5918_LPT_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__LPT_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__LPT_Msk;
					break;
				}
			} else {
				switch (clk_idx) {
				case RTS5918_PS2_1M_1_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__PS2_H1_Msk;
					break;
				case RTS5918_PS2_1M_2_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__PS2_H2_Msk;
					break;
				case RTS5918_PS2_1M_3_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__PS2_H3_Msk;
					break;
				case RTS5918_TACH1_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__TACH1_Msk;
					break;
				case RTS5918_TACH2_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__TACH2_Msk;
					break;
				case RTS5918_TACH3_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__TACH3_Msk;
					break;
				case RTS5918_TACH4_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__TACH4_Msk;
					break;
				case RTS5918_TACH5_PWR:
					sys_reg->IPCLK1 &= ~SYSTEM_IPCLK1__TACH5_Msk;
					break;
				case RTS5918_TACH6_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TACH6_Msk;
					break;
				case RTS5918_TACH7_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TACH7_Msk;
					break;
				case RTS5918_TACH8_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TACH8_Msk;
					break;
				case RTS5918_TACH9_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TACH9_Msk;
					break;
				case RTS5918_TACH10_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TACH10_Msk;
					break;
				case RTS5918_TACH11_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TACH11_Msk;
					break;
				case RTS5918_TACH12_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TACH12_Msk;
					break;
				case RTS5918_TACH13_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TACH13_Msk;
					break;
				case RTS5918_TACH14_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TACH14_Msk;
					break;
				case RTS5918_GPIO_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__GPIO_Msk;
					break;
				case RTS5918_TIMER0_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TMR0_Msk;
					break;
				case RTS5918_TIMER1_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TMR1_Msk;
					break;
				case RTS5918_TIMER2_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TMR2_Msk;
					break;
				case RTS5918_TIMER3_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TMR3_Msk;
					break;
				case RTS5918_TIMER4_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TMR4_Msk;
					break;
				case RTS5918_TIMER5_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TMR5_Msk;
					break;
				case RTS5918_KBM_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__KBM_Msk;

					break;
	/*          // wait new header
				case RTS5918_LPC_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__TACH14_Msk;
					break;
	*/
				case RTS5918_SLWTMR0_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__SLWTMR0_Msk;
					break;
				case RTS5918_SLWTMR1_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__SLWTMR1_Msk;
					break;
				case RTS5918_SLWTMR2_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__SLWTMR2_Msk;
					break;
				case RTS5918_SLWTMR3_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__SLWTMR3_Msk;
					break;
				case RTS5918_LEDSTP_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__LEDSTP_Msk;
					break;
				case RTS5918_CEC1_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__CEC1_Msk;
					break;
				case RTS5918_CEC2_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__CEC2_Msk;
					break;
				case RTS5918_MICROCNT_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__MICROCNT_Msk;
					break;
				case RTS5918_I2CDBG_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__I2CDBG_Msk;
					break;
				case RTS5918_VINLED_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__VINLED_Msk;
					break;
				case RTS5918_LPT_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__LPT_Msk;
					break;
				}
				if(((sys_reg->IPCLK1 & RC25M_ALL_MSK_CLK1) == 0x0) && ((sys_reg->IPCLK2 & RC25M_ALL_MSK_CLK2) == 0x0)){
				sys_reg->SYSCLK &= ~SYSTEM_SYSCLK__RC25M_Msk;
				}
			}
			break;
		case RTS5918_CLK_I3C:
			if( true == on_off ){
				sys_reg->SYSCLK |= SYSTEM_SYSCLK__PLL_Msk;
				switch (clk_idx) {
				case RTS5918_I3C1_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__I3C1_Msk;
					break;
				case RTS5918_I3C2_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__I3C2_Msk;
					break;
				case RTS5918_I3C3_PWR:
					sys_reg->IPCLK3 |= SYSTEM_IPCLK3__I3C3_Msk;
					break;
				case RTS5918_I3C4_PWR:
					sys_reg->IPCLK2 |= SYSTEM_IPCLK2__I3C4_Msk;
					break;
				}
			} else {
				switch (clk_idx) {
				case RTS5918_I3C1_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__I3C1_Msk;
					break;
				case RTS5918_I3C2_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__I3C2_Msk;
					break;
				case RTS5918_I3C3_PWR:
					sys_reg->IPCLK3 &= ~SYSTEM_IPCLK3__I3C3_Msk;
					break;
				case RTS5918_I3C4_PWR:
					sys_reg->IPCLK2 &= ~SYSTEM_IPCLK2__I3C4_Msk;
					break;
				}
			}
			break;
		case RTS5918_CLK_PECI:
			if( true == on_off ){
				sys_reg->SYSCLK |= SYSTEM_SYSCLK__PECI_Msk;
				sys_reg->LDOCTRL |= SYSTEM_LDOCTRL_LDO3PWREN_Msk;
				switch (clk_idx) {
				case RTS5918_PECI_PWR:
					sys_reg->IPCLK3 |= SYSTEM_IPCLK3__PECI_Msk;
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__PECI_Msk;
					break;
				}
			} else {
				switch (clk_idx) {
				case RTS5918_PECI_PWR:
					sys_reg->IPCLK3 &= ~SYSTEM_IPCLK3__PECI_Msk;
					break;
				}
				sys_reg->LDOCTRL &= ~SYSTEM_LDOCTRL_LDO3PWREN_Msk;
				sys_reg->SYSCLK &= ~SYSTEM_SYSCLK__PECI_Msk;
			}
			break;
		case RTS5918_CLK_PS2_32K:
			if( true == on_off ){
				sys_reg->SYSCLK |= SYSTEM_SYSCLK__XTAL32_Msk;
				switch (clk_idx) {
				case RTS5918_PS2_32K_1_PWR:
					sys_reg->IPCLK3 |= SYSTEM_IPCLK3__PS2_L1_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__PS2_1_Msk;
					break;
				case RTS5918_PS2_32K_2_PWR:
					sys_reg->IPCLK3 |= SYSTEM_IPCLK3__PS2_L2_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__PS2_2_Msk;
					break;
				case RTS5918_PS2_32K_3_PWR:
					sys_reg->IPCLK3 |= SYSTEM_IPCLK3__PS2_L3_Msk;
					sys_reg->APBCLK0 |= SYSTEM_APBCLK0__PS2_3_Msk;
					break;
				case RTS5918_RTMR_PWR:
					sys_reg->IPCLK3 |= SYSTEM_IPCLK3__RTMR_Msk;
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__RTMR_Msk;
					break;
				}
			} else {
				switch (clk_idx) {
				case RTS5918_PS2_32K_1_PWR:
					sys_reg->IPCLK3 &= ~SYSTEM_IPCLK3__PS2_L1_Msk;
					break;
				case RTS5918_PS2_32K_2_PWR:
					sys_reg->IPCLK3 &= ~SYSTEM_IPCLK3__PS2_L2_Msk;
					break;
				case RTS5918_PS2_32K_3_PWR:
					sys_reg->IPCLK3 &= ~SYSTEM_IPCLK3__PS2_L3_Msk;
					break;
				case RTS5918_RTMR_PWR:
					sys_reg->IPCLK3 &= ~SYSTEM_IPCLK3__RTMR_Msk;
					break;
				}
				if((sys_reg->IPCLK3 & RC32K_ALL_MSK_CLK3) == 0x0){
					sys_reg->SYSCLK &= ~SYSTEM_SYSCLK__XTAL32_Msk;
				}
			}
			break;
		case RTS5918_CLK_APB:
			if( true == on_off ){
				sys_reg->SYSCLK |= SYSTEM_SYSCLK__APB_Msk;
				switch (clk_idx) {
				case RTS5918_PWRBTN0_PWR:
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWRBTN0_Msk;
					break;
				case RTS5918_PWRBTN1_PWR:
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__PWRBTN1_Msk;
					break;
				case RTS5918_RTC_PWR:
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__RTC_Msk;
					break;
				case RTS5918_SPIS_PWR:
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__SPIS_Msk;
					break;
				case RTS5918_SPIM_PWR:
					sys_reg->APBCLK1 |= SYSTEM_APBCLK1__SPIM_Msk;
					break;
				case RTS5918_WDT_PWR:
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__WDT_Msk;
					break;
				case RTS5918_TRC_PWR:
					sys_reg->APBCLK2 |= SYSTEM_APBCLK2__TRC_Msk;
					break;
				}
			} else {
				switch (clk_idx) {
				case RTS5918_PWRBTN0_PWR:
					sys_reg->APBCLK1 &= ~SYSTEM_APBCLK1__PWRBTN0_Msk;
					break;
				case RTS5918_PWRBTN1_PWR:
					sys_reg->APBCLK1 &= ~SYSTEM_APBCLK1__PWRBTN1_Msk;
					break;
				case RTS5918_RTC_PWR:
					sys_reg->APBCLK1 &= ~SYSTEM_APBCLK1__RTC_Msk;
					break;
				case RTS5918_SPIS_PWR:
					sys_reg->APBCLK1 &= ~SYSTEM_APBCLK1__SPIS_Msk;
					break;
				case RTS5918_SPIM_PWR:
					sys_reg->APBCLK1 &= ~SYSTEM_APBCLK1__SPIM_Msk;
					break;
				case RTS5918_WDT_PWR:
					sys_reg->APBCLK2 &= ~SYSTEM_APBCLK2__WDT_Msk;
					break;
				case RTS5918_TRC_PWR:
					sys_reg->APBCLK2 &= ~SYSTEM_APBCLK2__TRC_Msk;
					break;
				}
			}
			break;
		default:
			LOG_ERR("Unknown peripheral group #%d", clk_grp);
			return -EINVAL;
	}

	return 0;
}

static int rts5918_clock_control_on(const struct device *dev, clock_control_subsys_t sub_system)
{
	return rts5918_periph_clock_control(dev, sub_system, true);
}

static int rts5918_clock_control_off(const struct device *dev, clock_control_subsys_t sub_system)
{
	return rts5918_periph_clock_control(dev, sub_system, false);
}

static int rts5918_clock_control_get_rate(const struct device *dev,
					  clock_control_subsys_t sub_system, uint32_t *rate)
{
	return 0;
}
/*
static DEVICE_API(clock_control, rts5918_clock_control_api) = {
	.on = rts5918_clock_control_on,
	.off = rts5918_clock_control_off,
	.get_rate = rts5918_clock_control_get_rate,
};
*/
static struct clock_control_driver_api rts5918_clock_control_api = {
	.on = rts5918_clock_control_on,
	.off = rts5918_clock_control_off,
	.get_rate = rts5918_clock_control_get_rate,
};
static int rts5918_clock_control_init(const struct device *dev)
{
	const struct rts5918_sccon_config *const config = dev->config;
	uint32_t wf_cycle_count = k_us_to_cyc_ceil32(RTS5918_MAXIMUM_POLLING_TIME_US);
	uint32_t wf_start = k_cycle_get_32();
	uint32_t wf_now = wf_start;
	volatile SYSTEM_Type *sys_reg = (volatile SYSTEM_Type *)config->reg_base;

	sys_reg->SYSCLK |= SYSTEM_SYSCLK__APB_Msk;
	if( (sys_reg->PLLCTRL & SYSTEM_PLLCTRL_DRDY_Msk) == 0x0){
		sys_reg->PLLCTRL |= SYSTEM_PLLCTRL_PWREN_Msk;
	}

	while( ((sys_reg->PLLCTRL & SYSTEM_PLLCTRL_DRDY_Msk) == 0x0) && (wf_cycle_count > (wf_now - wf_start))){
		wf_now = k_cycle_get_32();
	}

	if( (sys_reg->PLLCTRL & SYSTEM_PLLCTRL_DRDY_Msk) == 0x0 ){
		LOG_ERR("system clk select PLL fail");
		return -EIO;
	} else {
		sys_reg->SYSCLK |= SYSTEM_SYSCLK__PLL_Msk;
		sys_reg->SYSCLKSEL |= config->sysclk_src << SYSTEM_SYSCLKSEL__SYSTEM_Pos;
	}

	/* Enable LDO2 & LDO3 */
    sys_reg->LDOCTRL_b.LDO2PWREN = 1;
    sys_reg->LDOCTRL_b.LDO3PWREN = 1;
    sys_reg->LDO2WR_b.LDO2WREN = 1;
    sys_reg->LDO2WR_b.LDO2WREN = 0;

    /* Enable RC32K */
    sys_reg->RC32KCTRL_b.PWREN = 1;
    sys_reg->LDO2WR_b._RC32KWR = 1;
    sys_reg->LDO2WR_b._RC32KWR = 0;

	return 0;
}

const struct rts5918_sccon_config rts5918_sccon_config = {
	.reg_base = DT_INST_REG_ADDR_BY_IDX(0, 0),
	.sysclk_src = 1,
	.sysclk_div = 0,
};

DEVICE_DT_INST_DEFINE(0, &rts5918_clock_control_init, NULL, NULL, &rts5918_sccon_config,
		      PRE_KERNEL_1, CONFIG_CLOCK_CONTROL_INIT_PRIORITY, &rts5918_clock_control_api);
