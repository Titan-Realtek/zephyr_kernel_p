/*
 * Copyright (c) 2022-2024, Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file is derivative of CMSIS V5.9.0 system_ARMCM33.c
 * Git SHA: 2b7495b8535bdcb306dac29b9ded4cfb679d7e5c
 */

#include "tfm_hal_device_header.h"

 /* Define clocks */
#define  XTAL             (40000000UL)
#define  PERIPHERAL_XTAL  (50000000UL)
#define  SYSTEM_CLOCK     (XTAL/2)
/* rts5918 ns16550 UART0 reference clock is 25 MHz (see zephyr dts node
 * uart@40251600 clock-frequency = <25000000>). PeripheralClock is used ONLY
 * as the baud-divisor seed in arm_uart_init() (Driver_USART.c), and the arm
 * driver TRUNCATES:  bauddiv = PeripheralClock / 16 / baud.
 *
 * With a true 25 MHz seed, 25e6/16/115200 = 13.56 -> 13, i.e. 120192 baud
 * (+4.3%). Zephyr's ns16550 instead ROUNDS ((pclk + baud*8)/baud >>4) -> 14,
 * i.e. 111607 baud (-3.1%). To transmit at the SAME divisor Zephyr ships
 * (14 = the field-proven value for the 115200 host), seed the truncating
 * calc so it lands on 14 exactly: 14*16*115200 = 25804800. The physical
 * clock is still 25 MHz, so the wire baud is 25e6/(16*14) = 111607 (-3.1%,
 * well within the ~+/-5% NS16550 receiver tolerance). This is a divisor-
 * alignment seed, NOT a claim about the real peripheral clock. */
#define  PERIPHERAL_CLOCK (25804800UL)

/*----------------------------------------------------------------------------
  rts5918 UART0 debug-console bring-up (bare-metal, no Zephyr clock/pinctrl)

  BL2 (bl2_main.c) calls stdio_init() -> Driver_USART0 baudrate programming
  BEFORE boot_platform_init(). If UART0's APB/IP clock is still gated at that
  point the divisor writes never latch, so the clock + TX pin MUST be brought
  up here in SystemInit() (runs from the reset handler, before main()).

  Values mirror the Zephyr rts5918 drivers:
    - clock_control_rts5918_sccon.c  (RTS5918_CLK_LOW / RTS5918_UART0_PWR)
    - pinctrl_realtek_rts5918.c      (GCR[pin] = FUNC1 for uart00_tx_gpio140)
 *----------------------------------------------------------------------------*/
#define REG32(a)                (*(volatile uint32_t *)(a))

/* SYSTEM (sccon) @ 0x40100000 - see soc/realtek/ec/rts5918/reg/reg_system.h */
#define RTS5918_SYS_BASE        (0x40100000UL)
#define RTS5918_SYS_SYSCLK      (RTS5918_SYS_BASE + 0x00UL) /* bit13 LOWCLK   */
#define RTS5918_SYS_IPCLK1      (RTS5918_SYS_BASE + 0x0CUL) /* bit24 UART0    */
#define RTS5918_SYS_APBCLK2     (RTS5918_SYS_BASE + 0x1CUL) /* bit15 UART0    */
#define RTS5918_SYSCLK_LOWCLK   (1UL << 13)
#define RTS5918_IPCLK1_UART0    (1UL << 24)
#define RTS5918_APBCLK2_UART0   (1UL << 15)

/* pin-controller @ 0x40230000 - one 32-bit GCR per GPIO (reg-stride 4).
 * Bison debug TX is uart00_tx_gpio140 -> GCR[140] @ 0x40230000 + 140*4.
 * FUNC1 = MFCTRL(bits[10:8]) = 1 selects the UART0 TX alternate function. */
#define RTS5918_GPIO_BASE       (0x40230000UL)
#define RTS5918_UART0_TX_PIN    (140UL)
#define RTS5918_GCR_UART0_TX    (RTS5918_GPIO_BASE + (RTS5918_UART0_TX_PIN * 4UL))
#define RTS5918_PINMUX_FUNC1    (1UL << 8)

static void rts5918_uart0_console_init(void)
{
    /* 1) Ungate UART0: enable the LOW clock domain, then the UART0 IP and
     *    APB clocks (matches the sccon driver's RTS5918_UART0_PWR on-path). */
    REG32(RTS5918_SYS_SYSCLK)  |= RTS5918_SYSCLK_LOWCLK;
    REG32(RTS5918_SYS_IPCLK1)  |= RTS5918_IPCLK1_UART0;
    REG32(RTS5918_SYS_APBCLK2) |= RTS5918_APBCLK2_UART0;

    /* 2) Route GPIO140 to the UART0 TX function (FUNC1). */
    REG32(RTS5918_GCR_UART0_TX) = RTS5918_PINMUX_FUNC1;
}

/* System Core Clock Variable */
uint32_t SystemCoreClock = SYSTEM_CLOCK;
uint32_t PeripheralClock = PERIPHERAL_CLOCK;

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
extern const VECTOR_TABLE_Type __VECTOR_TABLE[496];

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)
{
    SystemCoreClock = SYSTEM_CLOCK;
}

/* System initialization function */
void SystemInit(void)
{
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
    SCB->VTOR = (uint32_t) &(__VECTOR_TABLE[0]);
#endif

#if defined (__FPU_USED) && (__FPU_USED == 1U)
    SCB->CPACR |= ((3U << 10U*2U) |           /* enable CP10 Full Access */
                   (3U << 11U*2U)  );         /* enable CP11 Full Access */
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

    SystemCoreClock = SYSTEM_CLOCK;
    PeripheralClock = PERIPHERAL_CLOCK;

    /* Bring the debug UART0 clock + TX pin up before main()/stdio_init(). */
    rts5918_uart0_console_init();
}
