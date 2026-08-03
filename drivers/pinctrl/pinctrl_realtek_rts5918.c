/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2024 Realtek Semiconductor Corporation, SIBG-SD7
 * Author: Lin Yu-Cheng <lin_yu_cheng@realtek.com>
 */

#define DT_DRV_COMPAT realtek_rts5918_pinctrl

#include <zephyr/drivers/pinctrl.h>
#include <zephyr/dt-bindings/pinctrl/realtek-rts5918-pinctrl.h>
#include <reg/reg_gpio.h>
#include <reg/reg_system.h>
#include <stdio.h>

#define REALTEK_RTS5918_PINMUX_GET_GPIO_PIN(n)                                                     \
	(((((n) >> REALTEK_RTS5918_GPIO_LOW_POS) & REALTEK_RTS5918_GPIO_LOW_MSK)) |                \
	 (((((n) >> REALTEK_RTS5918_GPIO_HIGH_POS) & REALTEK_RTS5918_GPIO_HIGH_MSK)) << 5))

#define PURE_PINMUX_MASK                   (GENMASK(31, 24) | GENMASK(17, 8) | GENMASK(2, 0))
#define REALTEK_RTS5918_GET_PURE_PINMUX(n) (n & PURE_PINMUX_MASK)

static volatile GPIO_Type *pinctrl_base =
	(volatile GPIO_Type *)(DT_REG_ADDR(DT_NODELABEL(pinctrl)));

static volatile uint32_t *iopad_base = (volatile uint32_t *)(0x40091000);

/* from bit 3 to 7 & bit 18 to 23
 * there will be 11bits for us to use
 * two bits of the MSB will be used to select register 	
 */

int pinctrl_configure_pins(const pinctrl_soc_pin_t *pins, uint8_t pin_cnt, uintptr_t reg)
{
	ARG_UNUSED(reg);
	uint32_t pin, pinmux, func, pin_sel, bit_length_msk, bit_value_msk_shift, bit_value_shift;
	SYSTEM_Type *sys_reg = RTS5918_SCCON_REG_BASE;

	for (uint8_t i = 0U; i < pin_cnt; i++) {
		pinmux = (uint32_t)pins[i];
		pin = REALTEK_RTS5918_PINMUX_GET_GPIO_PIN(pinmux);
		func = REALTEK_RTS5918_GET_PURE_PINMUX(pinmux);
		pin_sel = (pin & PIN_SEL_MSK) >> PIN_SEL_POS;
		
		if(pin_sel){
			if(pin & BIT_LENGTH_MSK){
				bit_length_msk = 0x3;
			} else {
				bit_length_msk = 0x1;
			}
			bit_value_msk_shift = bit_length_msk << ( pin & BIT_SHIFT_MSK);
			bit_value_shift = (((pin & BIT_VALUE_MSK) >> BIT_VALUE_POS) << ( pin & BIT_SHIFT_MSK));
			switch (pin_sel){
				case IXC_INPUT:
					sys_reg -> IXCINPUTSEL &= ~bit_value_msk_shift ;
					sys_reg -> IXCINPUTSEL |= bit_value_shift;
					printf("IXC_INPUT %x\r\n",bit_value_shift);
					printf("IXC_INPUT %x\r\n", (uint32_t) &(sys_reg -> IXCINPUTSEL));
					break;
				case UART_LPT:
					sys_reg -> _UART__LPT_INPUT_SEL &= ~bit_value_msk_shift ;
					sys_reg -> _UART__LPT_INPUT_SEL |= bit_value_shift;
					printf("UART_LPT %x\r\n",bit_value_shift);
					printf("UART_LPT %x\r\n", (uint32_t) &(sys_reg -> _UART__LPT_INPUT_SEL));
					break;
				case IP_INPUT:
					sys_reg -> IP_INPUT_SEL &= ~bit_value_msk_shift ;
					sys_reg -> IP_INPUT_SEL |= bit_value_shift;
					//printf("IP_INPUT %x\r\n",bit_value_shift);
					//printf("IP_INPUT %x\r\n", (uint32_t) &(sys_reg -> IP_INPUT_SEL));
					break;
				case SPIC_INPUT:
					sys_reg->SPI_FLASH_IO0 = 0x1;
					sys_reg->SPI_FLASH_IO1 = 0x1;
					sys_reg->SPI_FLASH_IO2 = 0x1;
					sys_reg->SPI_FLASH_IO3A = 0x1;
					sys_reg->SPI_FLASH_CSN = 0x1;
					sys_reg->SPI_FLASH_SCLK = 0x1;
					sys_reg -> SPIC_INPUT_SEL &= ~bit_value_msk_shift ;
					sys_reg -> SPIC_INPUT_SEL |= bit_value_shift;
					printf("SPIC_INPUT %x\r\n",bit_value_shift);
					printf("SPIC_INPUT %x\r\n", (uint32_t) &(sys_reg -> SPIC_INPUT_SEL));
					break;
				default:
					return -EINVAL;
					break;
			}
		} else {
			if((func & REALTEK_RTS5918_FUNC_MSK) == 0x00 ){
				pinctrl_base->GCR[pin] = 0;
			}else{
				pinctrl_base->GCR[pin] = func;
			}
		}
	}
	return 0;
}
