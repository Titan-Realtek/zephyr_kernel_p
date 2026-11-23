/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_SOC_REALTEK_RTS5918_REG_SYSTEM_H
#define ZEPHYR_SOC_REALTEK_RTS5918_REG_SYSTEM_H

/**
  * @brief System Controller (SYSTEM)
  */

typedef struct {                                /*!< (@ 0x40100000) SYSTEM Structure                                           */
  
  union {
    volatile uint32_t SYSCLK;                      /*!< (@ 0x00000000) SYSCLK POWER ENABLE REGISTER                               */
    
    struct {
            uint32_t            : 2;
      volatile uint32_t _USB       : 1;            /*!< [2..2] _USB Power                                                         */
      volatile uint32_t _LC        : 1;            /*!< [3..3] _LC Power                                                          */
      volatile uint32_t _XTAL32    : 1;            /*!< [4..4] _XTAL32 Power                                                      */
      volatile uint32_t _RC32KREF  : 1;            /*!< [5..5] _RC32KREF Power                                                    */
      volatile uint32_t _PECI      : 1;            /*!< [6..6] _PECI Power                                                        */
      volatile uint32_t _PLL       : 1;            /*!< [7..7] _PLL Power                                                         */
      volatile uint32_t _RC25MREF  : 1;            /*!< [8..8] RC25MREF Power                                                     */
      volatile uint32_t _RC25MCAL  : 1;            /*!< [9..9] _RC25M Calibration Power                                           */
      volatile uint32_t _RC25M     : 1;            /*!< [10..10] _RC25M Power                                                     */
      volatile uint32_t _LED_PWM   : 1;            /*!< [11..11] _LED_PWM Power                                                   */
      volatile uint32_t _I2C       : 1;            /*!< [12..12] _I2C Power                                                       */
      volatile uint32_t _LOWCLK    : 1;            /*!< [13..13] _LOWCLK Power                                                    */
      volatile uint32_t _ESPI      : 1;            /*!< [14..14] _ESPI Power                                                      */
      volatile uint32_t _APB       : 1;            /*!< [15..15] _APB Power                                                       */
            uint32_t            : 16;
    } SYSCLK_b;
  } ;
  
  union {
    volatile uint32_t SYSCLKSEL;                   /*!< (@ 0x00000004) SYSTEM CLK SELECT REGISTER                                 */
    
    struct {
            uint32_t            : 4;
      volatile uint32_t S_LALU     : 2;            /*!< [5..4] select LALU source                                                 */
      volatile uint32_t S_RC32KPAD : 2;            /*!< [7..6] select 32K clock pad source                                        */
            uint32_t            : 12;
      volatile uint32_t _LED_PWM   : 2;            /*!< [21..20] select _LED_PWM clock source                                     */
      volatile uint32_t _I2C       : 2;            /*!< [23..22] _I2C Clock Select                                                */
      volatile uint32_t LOWCLK     : 2;            /*!< [25..24] LOW Clock Select                                                 */
      volatile uint32_t _ESPI      : 2;            /*!< [27..26] _ESPI Clock Select                                               */
      volatile uint32_t APB        : 2;            /*!< [29..28] APB Clock Select                                                 */
      volatile uint32_t _SYSTEM    : 2;            /*!< [31..30] _SYSTEM Clock Select                                             */
    } SYSCLKSEL_b;
  } ;
  
  union {
    volatile uint32_t IPCLK0;                      /*!< (@ 0x00000008) IP_CLOCK_0 POWER REGISTER                                  */
    
    struct {
      volatile uint32_t _PWM8      : 1;            /*!< [0..0] _PWM8 IP Clock POWER                                               */
      volatile uint32_t _PWM7      : 1;            /*!< [1..1] _PWM7 IP Clock Power                                               */
      volatile uint32_t _PWM6      : 1;            /*!< [2..2] _PWM6 IP Clock Power                                               */
      volatile uint32_t _PWM5      : 1;            /*!< [3..3] _PWM5 IP Clock Power                                               */
      volatile uint32_t _PWM4      : 1;            /*!< [4..4] _PWM4 IP Clock Power                                               */
      volatile uint32_t _PWM3      : 1;            /*!< [5..5] _PWM3 IP Clock Power                                               */
      volatile uint32_t _PWM2      : 1;            /*!< [6..6] _PWM2 IP Clock Power                                               */
      volatile uint32_t _PWM1      : 1;            /*!< [7..7] _PWM1 IP Clock Power                                               */
      volatile uint32_t _LED4      : 1;            /*!< [8..8] _LED4 IP Clock Power                                               */
      volatile uint32_t _LED3      : 1;            /*!< [9..9] _LED3 IP Clock Power                                               */
      volatile uint32_t _LED2      : 1;            /*!< [10..10] _LED2 IP Clock Power                                             */
      volatile uint32_t _LED1      : 1;            /*!< [11..11] _LED1 IP Clock Power                                             */
      volatile uint32_t _EMI7      : 1;            /*!< [12..12] _EMI7 IP Clock Power                                             */
      volatile uint32_t _EMI6      : 1;            /*!< [13..13] _EMI6 IP Clock Power                                             */
      volatile uint32_t _EMI5      : 1;            /*!< [14..14] _EMI5 IP Clock Power                                             */
      volatile uint32_t _EMI4      : 1;            /*!< [15..15] _EMI4 IP Clock Power                                             */
      volatile uint32_t _EMI3      : 1;            /*!< [16..16] _EMI3 IP Clock Power                                             */
      volatile uint32_t _EMI2      : 1;            /*!< [17..17] _EMI2 IP Clock Power                                             */
      volatile uint32_t _EMI1      : 1;            /*!< [18..18] _EMI1 IP Clock Power                                             */
      volatile uint32_t _EMI0      : 1;            /*!< [19..19] _EMI0 IP Clock Power                                             */
      volatile uint32_t _PORT80    : 1;            /*!< [20..20] _PORT80 IP Clock Power                                           */
      volatile uint32_t _PMPORT3   : 1;            /*!< [21..21] _PMPORT3 IP Clock Power                                          */
      volatile uint32_t _PMPORT2   : 1;            /*!< [22..22] _PMPORT2 IP Clock Power                                          */
      volatile uint32_t _PMPORT1   : 1;            /*!< [23..23] _PMPORT1 IP Clock Power                                          */
      volatile uint32_t _PMPORT0   : 1;            /*!< [24..24] _PMPORT0 IP Clock Power                                          */
      volatile uint32_t _ACPI      : 1;            /*!< [25..25] _ACPI IP Clock Power                                             */
      volatile uint32_t _KBC       : 1;            /*!< [26..26] _KBC IP Clock Power                                              */
      volatile uint32_t _ESPI      : 1;            /*!< [27..27] _ESPI IP Clock Power                                             */
      volatile uint32_t _DMA       : 1;            /*!< [28..28] _DMA IP Clock Power                                              */
      volatile uint32_t _OTP       : 1;            /*!< [29..29] _OTP IP Clock Power                                              */
      volatile uint32_t KR4        : 1;            /*!< [30..30] KR4 IP Clock Power                                               */
      volatile uint32_t _BUS       : 1;            /*!< [31..31] _BUS IP Clock Power                                              */
    } IPCLK0_b;
  } ;
  
  union {
    volatile uint32_t IPCLK1;                      /*!< (@ 0x0000000C) IP_CLOCK_1 POWER EREGISTER                                 */
    
    struct {
      volatile uint32_t _TACH5     : 1;            /*!< [0..0] _TACH5 IP Clock Power                                              */
      volatile uint32_t _TACH4     : 1;            /*!< [1..1] _TACH4 IP Clock Power                                              */
      volatile uint32_t _TACH3     : 1;            /*!< [2..2] _TACH3 IP Clock Power                                              */
      volatile uint32_t _TACH2     : 1;            /*!< [3..3] _TACH2 IP Clock Power                                              */
      volatile uint32_t _TACH1     : 1;            /*!< [4..4] _TACH1 IP Clock Power                                              */
      volatile uint32_t _PS2_H3    : 1;            /*!< [5..5] _PS2_H3 IP Clock Power                                             */
      volatile uint32_t _PS2_H2    : 1;            /*!< [6..6] _PS2_H2 IP Clock Power                                             */
      volatile uint32_t _PS2_H1    : 1;            /*!< [7..7] _PS2_H1 IP Clock Power                                             */
            uint32_t            : 3;
      volatile uint32_t _I2C11     : 1;            /*!< [11..11] _I2C11 IP Clock Power                                            */
      volatile uint32_t _I2C10     : 1;            /*!< [12..12] _I2C10 IP Clock Power                                            */
      volatile uint32_t _I2C9      : 1;            /*!< [13..13] _I2C9 IP Clock Power                                             */
      volatile uint32_t _I2C8      : 1;            /*!< [14..14] _I2C8 IP Clock Power                                             */
      volatile uint32_t _I2C7      : 1;            /*!< [15..15] _I2C7 IP Clock Power                                             */
      volatile uint32_t _I2C6      : 1;            /*!< [16..16] _I2C6 IP Clock Power                                             */
      volatile uint32_t _I2C5      : 1;            /*!< [17..17] _I2C5 IP Clock Power                                             */
      volatile uint32_t _I2C4      : 1;            /*!< [18..18] _I2C4 IP Clock Power                                             */
      volatile uint32_t _I2C3      : 1;            /*!< [19..19] _I2C3 IP Clock Power                                             */
      volatile uint32_t _I2C2      : 1;            /*!< [20..20] _I2C2 IP Clock Power                                             */
      volatile uint32_t _I2C1      : 1;            /*!< [21..21] _I2C1 IP Clock Power                                             */
      volatile uint32_t _UART2     : 1;            /*!< [22..22] _UART2 IP Clock Power                                            */
      volatile uint32_t _UART1     : 1;            /*!< [23..23] _UART1 IP Clock Power                                            */
      volatile uint32_t _UART0     : 1;            /*!< [24..24] _UART0 IP Clock Power                                            */
      volatile uint32_t _ADC       : 1;            /*!< [25..25] _ADC IP Clock Power                                              */
      volatile uint32_t _PWM14     : 1;            /*!< [26..26] _PWM14 IP Clock Power                                            */
      volatile uint32_t _PWM13     : 1;            /*!< [27..27] _PWM13 IP Clock Power                                            */
      volatile uint32_t _PWM12     : 1;            /*!< [28..28] _PWM12 IP Clock Power                                            */
      volatile uint32_t _PWM11     : 1;            /*!< [29..29] _PWM11 IP Clock Power                                            */
      volatile uint32_t _PWM10     : 1;            /*!< [30..30] _PWM10 IP Clock Power                                            */
      volatile uint32_t _PWM9      : 1;            /*!< [31..31] _PWM9 IP Clock Power                                             */
    } IPCLK1_b;
  } ;
  
  union {
    volatile uint32_t IPCLK2;                      /*!< (@ 0x00000010) IP_CLOCK_2 POWER EREGISTER                                 */
    
    struct {
      volatile uint32_t _I3C2      : 1;            /*!< [0..0] _I3C2 IP Clock Power                                               */
      volatile uint32_t _I3C1      : 1;            /*!< [1..1] _I3C1 IP Clock Power                                               */
      volatile uint32_t _I3C4      : 1;            /*!< [2..2] _I3C4 IP Clock Power                                               */
      volatile uint32_t _LPT       : 1;            /*!< [3..3] _LPT IP Clock Power                                                */
      volatile uint32_t _VINLED    : 1;            /*!< [4..4] _VINLED IP Clock Power                                             */
      volatile uint32_t _I2CDBG    : 1;            /*!< [5..5] _I2CDBG IP Clock Power                                             */
      volatile uint32_t _MICROCNT  : 1;            /*!< [6..6] _MICROCNT IP Clock Power                                           */
      volatile uint32_t _CEC2      : 1;            /*!< [7..7] _CEC2 IP Clock Power                                               */
      volatile uint32_t _CEC1      : 1;            /*!< [8..8] _CEC1 IP Clock Power                                               */
      volatile uint32_t _LEDSTP    : 1;            /*!< [9..9] _LEDSTP IP Clock Power                                             */
      volatile uint32_t _SLWTMR3   : 1;            /*!< [10..10] _SLWTMR3 IP Clock Power                                          */
      volatile uint32_t _SLWTMR2   : 1;            /*!< [11..11] _SLWTMR2 IP Clock Power                                          */
      volatile uint32_t _SLWTMR1   : 1;            /*!< [12..12] _SLWTMR1 IP Clock Power                                          */
      volatile uint32_t _SLWTMR0   : 1;            /*!< [13..13] _SLWTMR0 IP Clock Power                                          */
            uint32_t            : 1;
      volatile uint32_t _KBM       : 1;            /*!< [15..15] _KBM IP Clock Power                                              */
      volatile uint32_t _TMR5      : 1;            /*!< [16..16] _TMR5 IP Clock Power                                             */
      volatile uint32_t _TMR4      : 1;            /*!< [17..17] _TMR4 IP Clock Power                                             */
      volatile uint32_t _TMR3      : 1;            /*!< [18..18] _TMR3 IP Clock Power                                             */
      volatile uint32_t _TMR2      : 1;            /*!< [19..19] _TMR2 IP Clock Power                                             */
      volatile uint32_t _TMR1      : 1;            /*!< [20..20] _TMR1 IP Clock Power                                             */
      volatile uint32_t _TMR0      : 1;            /*!< [21..21] _TMR0 IP Clock Power                                             */
      volatile uint32_t _GPIO      : 1;            /*!< [22..22] _GPIO IP Clock Power                                             */
      volatile uint32_t _TACH14    : 1;            /*!< [23..23] _TACH14 IP Clock Power                                           */
      volatile uint32_t _TACH13    : 1;            /*!< [24..24] _TACH13 IP Clock Power                                           */
      volatile uint32_t _TACH12    : 1;            /*!< [25..25] _TACH12 IP Clock Power                                           */
      volatile uint32_t _TACH11    : 1;            /*!< [26..26] _TACH11 IP Clock Power                                           */
      volatile uint32_t _TACH10    : 1;            /*!< [27..27] _TACH10 IP Clock Power                                           */
      volatile uint32_t _TACH9     : 1;            /*!< [28..28] _TACH9 IP Clock Power                                            */
      volatile uint32_t _TACH8     : 1;            /*!< [29..29] _TACH8 IP Clock Power                                            */
      volatile uint32_t _TACH7     : 1;            /*!< [30..30] _TACH7 IP Clock Power                                            */
      volatile uint32_t _TACH6     : 1;            /*!< [31..31] _TACH6 IP Clock Power                                            */
    } IPCLK2_b;
  } ;
  
  union {
    union {
      volatile uint32_t APBCLK0;                   /*!< (@ 0x00000014) APB_CLOCK_0 POWER EREGISTER                                */
      
      struct {
        volatile uint32_t _PS2_3   : 1;            /*!< [0..0] _PS2_3 APB Clock Power                                             */
        volatile uint32_t _PS2_2   : 1;            /*!< [1..1] _PS2_2 APB Clock Power                                             */
        volatile uint32_t _PS2_1   : 1;            /*!< [2..2] _PS2_1 APB Clock Power                                             */
        volatile uint32_t _LEDSTP  : 1;            /*!< [3..3] _LEDSTP APB Clock Power                                            */
        volatile uint32_t _LED4    : 1;            /*!< [4..4] _LED4 APB Clock Power                                              */
        volatile uint32_t _LED3    : 1;            /*!< [5..5] _LED3 APB Clock Power                                              */
        volatile uint32_t _LED2    : 1;            /*!< [6..6] _LED2 APB Clock Power                                              */
        volatile uint32_t _LED1    : 1;            /*!< [7..7] _LED1 APB Clock Power                                              */
        volatile uint32_t _KBM     : 1;            /*!< [8..8] _KBM APB Clock Power                                               */
        volatile uint32_t _I2CDBG  : 1;            /*!< [9..9] _I2CDBG APB Clock Power                                            */
        volatile uint32_t _I2C11   : 1;            /*!< [10..10] _I2C11 APB Clock Power                                           */
        volatile uint32_t _I2C10   : 1;            /*!< [11..11] _I2C10 APB Clock Power                                           */
        volatile uint32_t _I2C9    : 1;            /*!< [12..12] _I2C9 APB Clock Power                                            */
        volatile uint32_t _I2C8    : 1;            /*!< [13..13] _I2C8 APB Clock Power                                            */
        volatile uint32_t _I2C7    : 1;            /*!< [14..14] _I2C7 APB Clock Power                                            */
        volatile uint32_t _I2C6    : 1;            /*!< [15..15] _I2C6 APB Clock Power                                            */
        volatile uint32_t _I2C5    : 1;            /*!< [16..16] _I2C5 APB Clock Power                                            */
        volatile uint32_t _I2C4    : 1;            /*!< [17..17] _I2C4 APB Clock Power                                            */
        volatile uint32_t _I2C3    : 1;            /*!< [18..18] _I2C3 APB Clock Power                                            */
        volatile uint32_t _I2C2    : 1;            /*!< [19..19] _I2C2 APB Clock Power                                            */
        volatile uint32_t _I2C1    : 1;            /*!< [20..20] _I2C1 APB Clock Power                                            */
        volatile uint32_t _GPIO    : 1;            /*!< [21..21] _GPIO APB Clock Power                                            */
        volatile uint32_t _CEC2    : 1;            /*!< [22..22] _CEC2 APB Clock Power                                            */
        volatile uint32_t _CEC1    : 1;            /*!< [23..23] _CEC1 APB Clock Power                                            */
        volatile uint32_t _ADC     : 1;            /*!< [24..24] _ADC APB Clock Power                                             */
              uint32_t          : 7;
      } APBCLK0_b;
    } ;
    
    union {
      volatile uint32_t IPCLK3;                    /*!< (@ 0x00000014) IP_CLOCK_3 POWER EREGISTER                                 */
      
      struct {
              uint32_t          : 25;
        volatile uint32_t _RTMR    : 1;            /*!< [25..25] _RTMR IP Clock Power                                             */
        volatile uint32_t _PS2_L3  : 1;            /*!< [26..26] _PS2_L3 IP Clock Power                                           */
        volatile uint32_t _PS2_L2  : 1;            /*!< [27..27] _PS2_L2 IP Clock Power                                           */
        volatile uint32_t _PS2_L1  : 1;            /*!< [28..28] _PS2_L1 Clock Power                                              */
        volatile uint32_t _RC32KREF : 1;           /*!< [29..29] _RC32KREF IP Clock Power                                         */
        volatile uint32_t _PECI    : 1;            /*!< [30..30] _PECI IP Clock Power                                             */
        volatile uint32_t _I3C3    : 1;            /*!< [31..31] _I3C3 IP Clock Power                                             */
      } IPCLK3_b;
    } ;
  };
  
  union {
    volatile uint32_t APBCLK1;                     /*!< (@ 0x00000018) APB_CLOCK_1 POWER EREGISTER                                */
    
    struct {
      volatile uint32_t _TACH8     : 1;            /*!< [0..0] _TACH8 APB Clock Power                                             */
      volatile uint32_t _TACH7     : 1;            /*!< [1..1] _TACH7 APB Clock Power                                             */
      volatile uint32_t _TACH6     : 1;            /*!< [2..2] _TACH6 APB Clock Power                                             */
      volatile uint32_t _TACH5     : 1;            /*!< [3..3] _TACH5 APB Clock Power                                             */
      volatile uint32_t _TACH4     : 1;            /*!< [4..4] _TACH4 APB Clock Power                                             */
      volatile uint32_t _TACH3     : 1;            /*!< [5..5] _TACH3 APB Clock Power                                             */
      volatile uint32_t _TACH2     : 1;            /*!< [6..6] _TACH2 APB Clock Power                                             */
      volatile uint32_t _TACH1     : 1;            /*!< [7..7] _TACH1 APB Clock Power                                             */
      volatile uint32_t _SPIM      : 1;            /*!< [8..8] _SPIM APB Clock Power                                              */
      volatile uint32_t _SPIS      : 1;            /*!< [9..9] _SPIS APB Clock Power                                              */
      volatile uint32_t _SLWTMR3   : 1;            /*!< [10..10] _SLWTMR3 APB Clock Power                                         */
      volatile uint32_t _SLWTMR2   : 1;            /*!< [11..11] _SLWTMR2 APB Clock Power                                         */
      volatile uint32_t _SLWTMR1   : 1;            /*!< [12..12] _SLWTMR1 APB Clock Power                                         */
      volatile uint32_t _SLWTMR0   : 1;            /*!< [13..13] _SLWTMR0 APB Clock Power                                         */
      volatile uint32_t _RTMR      : 1;            /*!< [14..14] _RTMR APB Clock Power                                            */
      volatile uint32_t _RTC       : 1;            /*!< [15..15] _RTC APB Clock Power                                             */
      volatile uint32_t _PWRBTN1   : 1;            /*!< [16..16] _PWRBTN1 APB Clock Power                                         */
      volatile uint32_t _PWRBTN0   : 1;            /*!< [17..17] _PWRBTN0 APB Clock Power                                         */
      volatile uint32_t _PWM14     : 1;            /*!< [18..18] _PWM14 APB Clock Power                                           */
      volatile uint32_t _PWM13     : 1;            /*!< [19..19] _PWM13 APB Clock Power                                           */
      volatile uint32_t _PWM12     : 1;            /*!< [20..20] _PWM12 APB Clock Power                                           */
      volatile uint32_t _PWM11     : 1;            /*!< [21..21] _PWM11 APB Clock Power                                           */
      volatile uint32_t _PWM10     : 1;            /*!< [22..22] _PWM10 APB Clock Power                                           */
      volatile uint32_t _PWM9      : 1;            /*!< [23..23] _PWM9 APB Clock Power                                            */
      volatile uint32_t _PWM8      : 1;            /*!< [24..24] _PWM8 APB Clock Power                                            */
      volatile uint32_t _PWM7      : 1;            /*!< [25..25] _PWM7 APB Clock Power                                            */
      volatile uint32_t _PWM6      : 1;            /*!< [26..26] _PWM6 APB Clock Power                                            */
      volatile uint32_t _PWM5      : 1;            /*!< [27..27] _PWM5 APB Clock Power                                            */
      volatile uint32_t _PWM4      : 1;            /*!< [28..28] _PWM4 APB Clock Power                                            */
      volatile uint32_t _PWM3      : 1;            /*!< [29..29] _PWM3 APB Clock Power                                            */
      volatile uint32_t _PWM2      : 1;            /*!< [30..30] _PWM2 APB Clock Power                                            */
      volatile uint32_t _PWM1      : 1;            /*!< [31..31] _PWM1 APB Clock Power                                            */
    } APBCLK1_b;
  } ;
  
  union {
    union {
      volatile uint32_t IPCLK4;                    /*!< (@ 0x0000001C) IP_CLOCK_4 POWER EREGISTER                                 */
      
      struct {
              uint32_t          : 5;
        volatile uint32_t _USBDPHY : 1;            /*!< [5..5] _USBDPHY IP Clock Power                                            */
        volatile uint32_t _USB     : 1;            /*!< [6..6] _USB IP Clock Power                                                */
        volatile uint32_t _TRC_CLK : 1;            /*!< [7..7] _TRC_CLK IP Clock Power                                            */
        volatile uint32_t _LALU    : 1;            /*!< [8..8] _LALU IP Clock Power                                               */
              uint32_t          : 23;
      } IPCLK4_b;
    } ;
    
    union {
      volatile uint32_t APBCLK2;                   /*!< (@ 0x0000001C) APB_CLOCK_2 POWER EREGISTER                                */
      
      struct {
              uint32_t          : 9;
        volatile uint32_t _TRC     : 1;            /*!< [9..9] _TRC APB Clock Power                                               */
        volatile uint32_t _PECI    : 1;            /*!< [10..10] _PECI APB Clock Power                                            */
        volatile uint32_t _LPT     : 1;            /*!< [11..11] _LPT APB Clock Power                                             */
        volatile uint32_t _WDT     : 1;            /*!< [12..12] _WDT APB Clock Power                                             */
        volatile uint32_t _UART2   : 1;            /*!< [13..13] _UART2 APB Clock Power                                           */
        volatile uint32_t _UART1   : 1;            /*!< [14..14] _UART1 APB Clock Power                                           */
        volatile uint32_t _UART0   : 1;            /*!< [15..15] _UART0 APB Clock Power                                           */
        volatile uint32_t _TMR5    : 1;            /*!< [16..16] _TMR5 APB Clock Power                                            */
        volatile uint32_t _TMR4    : 1;            /*!< [17..17] _TMR4 APB Clock Power                                            */
        volatile uint32_t _TMR3    : 1;            /*!< [18..18] _TMR3 APB Clock Power                                            */
        volatile uint32_t _TMR2    : 1;            /*!< [19..19] _TMR2 APB Clock Power                                            */
        volatile uint32_t _TMR1    : 1;            /*!< [20..20] _TMR1 APB Clock Power                                            */
        volatile uint32_t _TMR0    : 1;            /*!< [21..21] _TMR0 APB Clock Power                                            */
        volatile uint32_t _TACH14  : 1;            /*!< [22..22] _TACH14 APB Clock Power                                          */
        volatile uint32_t _TACH13  : 1;            /*!< [23..23] _TACH13 APB Clock Power                                          */
        volatile uint32_t _TACH12  : 1;            /*!< [24..24] _TACH12 APB Clock Power                                          */
        volatile uint32_t _TACH11  : 1;            /*!< [25..25] _TACH11 APB Clock Power                                          */
        volatile uint32_t _TACH10  : 1;            /*!< [26..26] _TACH10 APB Clock Power                                          */
        volatile uint32_t _TACH9   : 1;            /*!< [27..27] _TACH9 APB Clock Power                                           */
              uint32_t          : 4;
      } APBCLK2_b;
    } ;
  };
  
  union {
    volatile uint32_t IPDIV0;                      /*!< (@ 0x00000020) IP_CLOCK_DIVIDER_0 REGISTER                                */
    
    struct {
      volatile uint32_t _ESPI      : 2;            /*!< [1..0] _ESPI IP Clock Divider                                             */
            uint32_t            : 22;
      volatile uint32_t _DMA       : 2;            /*!< [25..24] _DMA IP Clock Divider                                            */
      volatile uint32_t _OTP       : 2;            /*!< [27..26] _OTP IP Clock Divider                                            */
            uint32_t            : 2;
      volatile uint32_t _BUS       : 2;            /*!< [31..30] _BUS IP Clock Divider                                            */
    } IPDIV0_b;
  } ;
  
  union {
    volatile uint32_t IPDIV1;                      /*!< (@ 0x00000024) IP_CLOCK_DIVIDER_1 REGISTER                                */
    
    struct {
      volatile uint32_t _PWM8      : 2;            /*!< [1..0] _PWM8 IP Clock Divider                                             */
      volatile uint32_t _PWM7      : 2;            /*!< [3..2] _PWM7 IP Clock Divider                                             */
      volatile uint32_t _PWM6      : 2;            /*!< [5..4] _PWM6 IP Clock Divider                                             */
      volatile uint32_t _PWM5      : 2;            /*!< [7..6] _PWM5 IP Clock Divider                                             */
      volatile uint32_t _PWM4      : 2;            /*!< [9..8] _PWM4 IP Clock Divider                                             */
      volatile uint32_t _PWM3      : 2;            /*!< [11..10] _PWM3 IP Clock Divider                                           */
      volatile uint32_t _PWM2      : 2;            /*!< [13..12] _PWM2 IP Clock Divider                                           */
      volatile uint32_t _PWM1      : 2;            /*!< [15..14] _PWM1 IP Clock Divider                                           */
      volatile uint32_t _LED4      : 2;            /*!< [17..16] _LED4 IP Clock Divider                                           */
      volatile uint32_t _LED3      : 2;            /*!< [19..18] _LED3 IP Clock Divider                                           */
      volatile uint32_t _LED2      : 2;            /*!< [21..20] _LED2 IP Clock Divider                                           */
      volatile uint32_t _LED1      : 2;            /*!< [23..22] _LED1 IP Clock Divider                                           */
      volatile uint32_t _EMI7      : 2;            /*!< [25..24] _EMI7 IP Clock Divider                                           */
      volatile uint32_t _EMI6      : 2;            /*!< [27..26] _EMI6 IP Clock Divider                                           */
      volatile uint32_t _EMI5      : 2;            /*!< [29..28] _EMI5 IP Clock Divider                                           */
      volatile uint32_t _EMI4      : 2;            /*!< [31..30] _EMI4 IP Clock Divider                                           */
    } IPDIV1_b;
  } ;
  
  union {
    volatile uint32_t IPDIV2;                      /*!< (@ 0x00000028) IP_CLOCK_DIVIDER_2 REGISTER                                */
    
    struct {
      volatile uint32_t _I2C5      : 2;            /*!< [1..0] _I2C5 IP Clock Divider                                             */
      volatile uint32_t _I2C4      : 2;            /*!< [3..2] _I2C4 IP Clock Divider                                             */
      volatile uint32_t _I2C3      : 2;            /*!< [5..4] _I2C3 IP Clock Divider                                             */
      volatile uint32_t _I2C2      : 2;            /*!< [7..6] _I2C2 IP Clock Divider                                             */
      volatile uint32_t _I2C1      : 2;            /*!< [9..8] _I2C1 IP Clock Divider                                             */
      volatile uint32_t _UART2     : 2;            /*!< [11..10] _UART2 IP Clock Divider                                          */
      volatile uint32_t _UART1     : 2;            /*!< [13..12] _UART1 IP Clock Divider                                          */
      volatile uint32_t _UART0     : 2;            /*!< [15..14] _UART0 IP Clock Divider                                          */
      volatile uint32_t _ADC       : 3;            /*!< [18..16] _ADC IP Clock Divider                                            */
      volatile uint32_t _PWM14     : 2;            /*!< [20..19] _PWM14 IP Clock Divider                                          */
      volatile uint32_t _PWM13     : 2;            /*!< [22..21] _PWM13 IP Clock Divider                                          */
      volatile uint32_t _PWM12     : 2;            /*!< [24..23] _PWM12 IP Clock Divider                                          */
      volatile uint32_t _PWM11     : 2;            /*!< [26..25] _PWM11 IP Clock Divider                                          */
      volatile uint32_t _PWM10     : 2;            /*!< [28..27] _PWM10 IP Clock Divider                                          */
      volatile uint32_t _PWM9      : 2;            /*!< [30..29] _PWM9 IP Clock Divider                                           */
            uint32_t            : 1;
    } IPDIV2_b;
  } ;
  
  union {
    volatile uint32_t IPDIV3;                      /*!< (@ 0x0000002C) IP_CLOCK_DIVIDER_3 REGISTER                                */
    
    struct {
            uint32_t            : 20;
      volatile uint32_t _I2C11     : 2;            /*!< [21..20] _I2C11 IP Clock Divider                                          */
      volatile uint32_t _I2C10     : 2;            /*!< [23..22] _I2C10 IP Clock Divider                                          */
      volatile uint32_t _I2C9      : 2;            /*!< [25..24] _I2C9 IP Clock Divider                                           */
      volatile uint32_t _I2C8      : 2;            /*!< [27..26] _I2C8 IP Clock Divider                                           */
      volatile uint32_t _I2C7      : 2;            /*!< [29..28] _I2C7 IP Clock Divider                                           */
      volatile uint32_t _I2C6      : 2;            /*!< [31..30] _I2C6 IP Clock Divider                                           */
    } IPDIV3_b;
  } ;
  
  union {
    volatile uint32_t IPDIV4;                      /*!< (@ 0x00000030) IP_CLOCK_DIVIDER_4 REGISTER                                */
    
    struct {
            uint32_t            : 7;
      volatile uint32_t _TMR5      : 4;            /*!< [10..7] _TMR5 IP Clock Divider                                            */
      volatile uint32_t _TMR4      : 4;            /*!< [14..11] _TMR4 IP Clock Divider                                           */
      volatile uint32_t _TMR3      : 4;            /*!< [18..15] _TMR3 IP Clock Divider                                           */
      volatile uint32_t _TMR2      : 4;            /*!< [22..19] _TMR2 IP Clock Divider                                           */
      volatile uint32_t _TMR1      : 4;            /*!< [26..23] _TMR1 IP Clock Divider                                           */
      volatile uint32_t _TMR0      : 4;            /*!< [30..27] _TMR0 IP Clock Divider                                           */
            uint32_t            : 1;
    } IPDIV4_b;
  } ;
  
  union {
    union {
      volatile uint32_t APBDIV0;                   /*!< (@ 0x00000034) APB_CLOCK_DIVIDER_0 REGISTER                               */
      
      struct {
        volatile uint32_t _I2C2    : 2;            /*!< [1..0] _I2C2 APB Clock Divider                                            */
        volatile uint32_t _I2C1    : 2;            /*!< [3..2] _I2C1 APB Clock Divider                                            */
        volatile uint32_t _GPIO    : 2;            /*!< [5..4] _GPIO APB Clock Divider                                            */
        volatile uint32_t _CEC2    : 2;            /*!< [7..6] _CEC2 APB Clock Divider                                            */
        volatile uint32_t _CEC1    : 2;            /*!< [9..8] _CEC1 APB Clock Divider                                            */
        volatile uint32_t _ADC     : 2;            /*!< [11..10] _ADC APB Clock Divider                                           */
              uint32_t          : 20;
      } APBDIV0_b;
    } ;
    
    union {
      volatile uint32_t IPDIV5;                    /*!< (@ 0x00000034) IP_CLOCK_DIVIDER_5 REGISTER                                */
      
      struct {
              uint32_t          : 18;
        volatile uint32_t _I3C3    : 2;            /*!< [19..18] _I3C3 IP Clock Divider                                           */
        volatile uint32_t _I3C2    : 2;            /*!< [21..20] _I3C2 IP Clock Divider                                           */
        volatile uint32_t _I3C1    : 2;            /*!< [23..22] _I3C1 IP Clock Divider                                           */
        volatile uint32_t _I3C4    : 2;            /*!< [25..24] _I3C4 IP Clock Divider                                           */
              uint32_t          : 6;
      } IPDIV5_b;
    } ;
  };
  
  union {
    volatile uint32_t APBDIV1;                     /*!< (@ 0x00000038) APB_CLOCK_DIVIDER_1 REGISTER                               */
    
    struct {
      volatile uint32_t _LEDSTP    : 2;            /*!< [1..0] _LEDSTP APB Clock Divider                                          */
      volatile uint32_t _LED4      : 2;            /*!< [3..2] _LED4 APB Clock Divider                                            */
      volatile uint32_t _LED3      : 2;            /*!< [5..4] _LED3 APB Clock Divider                                            */
      volatile uint32_t _LED2      : 2;            /*!< [7..6] _LED2 APB Clock Divider                                            */
      volatile uint32_t _LED1      : 2;            /*!< [9..8] _LED1 APB Clock Divider                                            */
      volatile uint32_t _KBM       : 2;            /*!< [11..10] _KBM APB Clock Divider                                           */
      volatile uint32_t _I2CDBG    : 2;            /*!< [13..12] _I2CDBG APB Clock Divider                                        */
      volatile uint32_t _I2C11     : 2;            /*!< [15..14] _I2C11 APB Clock Divider                                         */
      volatile uint32_t _I2C10     : 2;            /*!< [17..16] _I2C10 APB Clock Divider                                         */
      volatile uint32_t _I2C9      : 2;            /*!< [19..18] _I2C9 APB Clock Divider                                          */
      volatile uint32_t _I2C8      : 2;            /*!< [21..20] _I2C8 APB Clock Divider                                          */
      volatile uint32_t _I2C7      : 2;            /*!< [23..22] _I2C7 APB Clock Divider                                          */
      volatile uint32_t _I2C6      : 2;            /*!< [25..24] _I2C6 APB Clock Divider                                          */
      volatile uint32_t _I2C5      : 2;            /*!< [27..26] _I2C5 APB Clock Divider                                          */
      volatile uint32_t _I2C4      : 2;            /*!< [29..28] _I2C4 APB Clock Divider                                          */
      volatile uint32_t _I2C3      : 2;            /*!< [31..30] _I2C3 APB Clock Divider                                          */
    } APBDIV1_b;
  } ;
  
  union {
    volatile uint32_t APBDIV2;                     /*!< (@ 0x0000003C) APB_CLOCK_DIVIDER_2 REGISTER                               */
    
    struct {
      volatile uint32_t _PWM13     : 2;            /*!< [1..0] _PWM13 APB Clock Divider                                           */
      volatile uint32_t _PWM12     : 2;            /*!< [3..2] _PWM12 APB Clock Divider                                           */
      volatile uint32_t _PWM11     : 2;            /*!< [5..4] _PWM11 APB Clock Divider                                           */
      volatile uint32_t _PWM10     : 2;            /*!< [7..6] _PWM10 APB Clock Divider                                           */
      volatile uint32_t _PWM9      : 2;            /*!< [9..8] _PWM9 APB Clock Divider                                            */
      volatile uint32_t _PWM8      : 2;            /*!< [11..10] _PWM8 APB Clock Divider                                          */
      volatile uint32_t _PWM7      : 2;            /*!< [13..12] _PWM7 APB Clock Divider                                          */
      volatile uint32_t _PWM6      : 2;            /*!< [15..14] _PWM6 APB Clock Divider                                          */
      volatile uint32_t _PWM5      : 2;            /*!< [17..16] _PWM5 APB Clock Divider                                          */
      volatile uint32_t _PWM4      : 2;            /*!< [19..18] _PWM4 APB Clock Divider                                          */
      volatile uint32_t _PWM3      : 2;            /*!< [21..20] _PWM3 APB Clock Divider                                          */
      volatile uint32_t _PWM2      : 2;            /*!< [23..22] _PWM2 APB Clock Divider                                          */
      volatile uint32_t _PWM1      : 2;            /*!< [25..24] _PWM1 APB Clock Divider                                          */
      volatile uint32_t _PS2_3     : 2;            /*!< [27..26] _PS2_3 APB Clock Divider                                         */
      volatile uint32_t _PS2_2     : 2;            /*!< [29..28] _PS2_2 APB Clock Divider                                         */
      volatile uint32_t _PS2_1     : 2;            /*!< [31..30] _PS2_1 APB Clock Divider                                         */
    } APBDIV2_b;
  } ;
  
  union {
    volatile uint32_t APBDIV3;                     /*!< (@ 0x00000040) APB_CLOCK_DIVIDER_3 REGISTER                               */
    
    struct {
      volatile uint32_t _TACH5     : 2;            /*!< [1..0] _TACH5 APB Clock Divider                                           */
      volatile uint32_t _TACH4     : 2;            /*!< [3..2] _TACH4 APB Clock Divider                                           */
      volatile uint32_t _TACH3     : 2;            /*!< [5..4] _TACH3 APB Clock Divider                                           */
      volatile uint32_t _TACH2     : 2;            /*!< [7..6] _TACH2 APB Clock Divider                                           */
      volatile uint32_t _TACH1     : 2;            /*!< [9..8] _TACH1 APB Clock Divider                                           */
      volatile uint32_t _SPIM      : 2;            /*!< [11..10] _SPIM APB Clock Divider                                          */
      volatile uint32_t _SPIS      : 2;            /*!< [13..12] _SPIS APB Clock Divider                                          */
      volatile uint32_t _SLWTMR3   : 2;            /*!< [15..14] _SLWTMR3 APB Clock Divider                                       */
      volatile uint32_t _SLWTMR2   : 2;            /*!< [17..16] _SLWTMR2 APB Clock Divider                                       */
      volatile uint32_t _SLWTMR1   : 2;            /*!< [19..18] _SLWTMR1 APB Clock Divider                                       */
      volatile uint32_t _SLWTMR0   : 2;            /*!< [21..20] _SLWTMR0 APB Clock Divider                                       */
      volatile uint32_t _RTMR      : 2;            /*!< [23..22] _RTMR APB Clock Divider                                          */
      volatile uint32_t _RTC       : 2;            /*!< [25..24] _RTC APB Clock Divider                                           */
      volatile uint32_t _PWRBTN1   : 2;            /*!< [27..26] _PWRBTN1 APB Clock Divider                                       */
      volatile uint32_t _PWRBTN0   : 2;            /*!< [29..28] _PWRBTN0 APB Clock Divider                                       */
      volatile uint32_t _PWM14     : 2;            /*!< [31..30] _PWM14 APB Clock Divider                                         */
    } APBDIV3_b;
  } ;
  
  union {
    volatile uint32_t APBDIV4;                     /*!< (@ 0x00000044) APB_CLOCK_DIVIDER_4 REGISTER                               */
    
    struct {
      volatile uint32_t _UART0     : 2;            /*!< [1..0] _UART0 APB Clock Divider                                           */
      volatile uint32_t _TMR5      : 2;            /*!< [3..2] _TMR5 APB Clock Divider                                            */
      volatile uint32_t _TMR4      : 2;            /*!< [5..4] _TMR4 APB Clock Divider                                            */
      volatile uint32_t _TMR3      : 2;            /*!< [7..6] _TMR3 APB Clock Divider                                            */
      volatile uint32_t _TMR2      : 2;            /*!< [9..8] _TMR2 APB Clock Divider                                            */
      volatile uint32_t _TMR1      : 2;            /*!< [11..10] _TMR1 APB Clock Divider                                          */
      volatile uint32_t _TMR0      : 2;            /*!< [13..12] _TMR0 APB Clock Divider                                          */
      volatile uint32_t _TACH14    : 2;            /*!< [15..14] _TACH14 APB Clock Divider                                        */
      volatile uint32_t _TACH13    : 2;            /*!< [17..16] _TACH13 APB Clock Divider                                        */
      volatile uint32_t _TACH12    : 2;            /*!< [19..18] _TACH12 APB Clock Divider                                        */
      volatile uint32_t _TACH11    : 2;            /*!< [21..20] _TACH11 APB Clock Divider                                        */
      volatile uint32_t _TACH10    : 2;            /*!< [23..22] _TACH10 APB Clock Divider                                        */
      volatile uint32_t _TACH9     : 2;            /*!< [25..24] _TACH9 APB Clock Divider                                         */
      volatile uint32_t _TACH8     : 2;            /*!< [27..26] _TACH8 APB Clock Divider                                         */
      volatile uint32_t _TACH7     : 2;            /*!< [29..28] _TACH7 APB Clock Divider                                         */
      volatile uint32_t _TACH6     : 2;            /*!< [31..30] _TACH6 APB Clock Divider                                         */
    } APBDIV4_b;
  } ;
  
  union {
    union {
      volatile uint32_t APBDIV5;                   /*!< (@ 0x00000048) APB_CLOCK_DIVIDER_5 REGISTER                               */
      
      struct {
              uint32_t          : 11;
        volatile uint32_t _PECI    : 2;            /*!< [12..11] _PECI APB Clock Divider                                          */
        volatile uint32_t _LPT     : 2;            /*!< [14..13] _LPT APB Clock Divider                                           */
        volatile uint32_t _WDT     : 2;            /*!< [16..15] _WDT APB Clock Divider                                           */
        volatile uint32_t _UART2   : 2;            /*!< [18..17] _UART2 APB Clock Divider                                         */
        volatile uint32_t _UART1   : 2;            /*!< [20..19] _UART1 APB Clock Divider                                         */
              uint32_t          : 11;
      } APBDIV5_b;
    } ;
    
    union {
      volatile uint32_t IPDIV6;                    /*!< (@ 0x00000048) IP_CLOCK_DIVIDER_6 REGISTER                                */
      
      struct {
              uint32_t          : 6;
        volatile uint32_t _USB     : 1;            /*!< [6..6] _USB IP Clock Divider                                              */
              uint32_t          : 25;
      } IPDIV6_b;
    } ;
  };
  
  union {
    volatile uint32_t CLKGATING0;                  /*!< (@ 0x0000004C) IP_CLOCK_GATING_0 EREGISTER                                */
    
    struct {
      volatile uint32_t _PWM8      : 1;            /*!< [0..0] _PWM8 IP Clock Gating                                              */
      volatile uint32_t _PWM7      : 1;            /*!< [1..1] _PWM7 IP Clock Gating                                              */
      volatile uint32_t _PWM6      : 1;            /*!< [2..2] _PWM6 IP Clock Gating                                              */
      volatile uint32_t _PWM5      : 1;            /*!< [3..3] _PWM5 IP Clock Gating                                              */
      volatile uint32_t _PWM4      : 1;            /*!< [4..4] _PWM4 IP Clock Gating                                              */
      volatile uint32_t _PWM3      : 1;            /*!< [5..5] _PWM3 IP Clock Gating                                              */
      volatile uint32_t _PWM2      : 1;            /*!< [6..6] _PWM2 IP Clock Gating                                              */
      volatile uint32_t _PWM1      : 1;            /*!< [7..7] _PWM1 IP Clock Gating                                              */
      volatile uint32_t _LED4      : 1;            /*!< [8..8] _LED4 IP Clock Gating                                              */
      volatile uint32_t _LED3      : 1;            /*!< [9..9] _LED3 IP Clock Gating                                              */
      volatile uint32_t _LED2      : 1;            /*!< [10..10] _LED2 IP Clock Gating                                            */
      volatile uint32_t _LED1      : 1;            /*!< [11..11] _LED1 IP Clock Gating                                            */
      volatile uint32_t _EMI7      : 1;            /*!< [12..12] _EMI7 IP Clock Gating                                            */
      volatile uint32_t _EMI6      : 1;            /*!< [13..13] _EMI6 IP Clock Gating                                            */
      volatile uint32_t _EMI5      : 1;            /*!< [14..14] _EMI5 IP Clock Gating                                            */
      volatile uint32_t _EMI4      : 1;            /*!< [15..15] _EMI4 IP Clock Gating                                            */
      volatile uint32_t _EMI3      : 1;            /*!< [16..16] _EMI3 IP Clock Gating                                            */
      volatile uint32_t _EMI2      : 1;            /*!< [17..17] _EMI2 IP Clock Gating                                            */
      volatile uint32_t _EMI1      : 1;            /*!< [18..18] _EMI1 IP Clock Gating                                            */
      volatile uint32_t _EMI0      : 1;            /*!< [19..19] _EMI0 IP Clock Gating                                            */
      volatile uint32_t _PORT80    : 1;            /*!< [20..20] _PORT80 IP Clock Gating                                          */
      volatile uint32_t _PMPORT3   : 1;            /*!< [21..21] _PMPORT3 IP Clock Gating                                         */
      volatile uint32_t _PMPORT2   : 1;            /*!< [22..22] _PMPORT2 IP Clock Gating                                         */
      volatile uint32_t _PMPORT1   : 1;            /*!< [23..23] _PMPORT1 IP Clock Gating                                         */
      volatile uint32_t _PMPORT0   : 1;            /*!< [24..24] _PMPORT0 IP Clock Gating                                         */
      volatile uint32_t _ACPI      : 1;            /*!< [25..25] _ACPI IP Clock Gating                                            */
      volatile uint32_t _KBC       : 1;            /*!< [26..26] _KBC IP Clock Gating                                             */
      volatile uint32_t _ESPI      : 1;            /*!< [27..27] _ESPI IP Clock Gating                                            */
      volatile uint32_t _DMA       : 1;            /*!< [28..28] _DMA IP Clock Gating                                             */
      volatile uint32_t PUF_OTP    : 1;            /*!< [29..29] PUF_OTP IP Clock Gating                                          */
            uint32_t            : 1;
      volatile uint32_t _BUS       : 1;            /*!< [31..31] _BUS IP Clock Gating                                             */
    } CLKGATING0_b;
  } ;
  
  union {
    volatile uint32_t CLKGATING1;                  /*!< (@ 0x00000050) IP_CLOCK_GATING_1 EREGISTER                                */
    
    struct {
      volatile uint32_t _TACH5     : 1;            /*!< [0..0] _TACH5 IP Clock Gating                                             */
      volatile uint32_t _TACH4     : 1;            /*!< [1..1] _TACH4 IP Clock Gating                                             */
      volatile uint32_t _TACH3     : 1;            /*!< [2..2] _TACH3 IP Clock Gating                                             */
      volatile uint32_t _TACH2     : 1;            /*!< [3..3] _TACH2 IP Clock Gating                                             */
      volatile uint32_t _TACH1     : 1;            /*!< [4..4] _TACH1 IP Clock Gating                                             */
      volatile uint32_t _PS2_H3    : 1;            /*!< [5..5] _PS2_H3 IP Clock Gating                                            */
      volatile uint32_t _PS2_H2    : 1;            /*!< [6..6] _PS2_H2 IP Clock Gating                                            */
      volatile uint32_t _PS2_H1    : 1;            /*!< [7..7] _PS2_H1 IP Clock Gating                                            */
      volatile uint32_t SWD        : 1;            /*!< [8..8] SWD IP Clock Gating                                                */
      volatile uint32_t REF25M     : 1;            /*!< [9..9] REF25M IP Clock Gating                                             */
      volatile uint32_t RC24M      : 1;            /*!< [10..10] RC24M IP Clock Gating                                            */
      volatile uint32_t _I2C11     : 1;            /*!< [11..11] _I2C11 IP Clock Gating                                           */
      volatile uint32_t _I2C10     : 1;            /*!< [12..12] _I2C10 IP Clock Gating                                           */
      volatile uint32_t _I2C9      : 1;            /*!< [13..13] _I2C9 IP Clock Gating                                            */
      volatile uint32_t _I2C8      : 1;            /*!< [14..14] _I2C8 IP Clock Gating                                            */
      volatile uint32_t _I2C7      : 1;            /*!< [15..15] _I2C7 IP Clock Gating                                            */
      volatile uint32_t _I2C6      : 1;            /*!< [16..16] _I2C6 IP Clock Gating                                            */
      volatile uint32_t _I2C5      : 1;            /*!< [17..17] _I2C5 IP Clock Gating                                            */
      volatile uint32_t _I2C4      : 1;            /*!< [18..18] _I2C4 IP Clock Gating                                            */
      volatile uint32_t _I2C3      : 1;            /*!< [19..19] _I2C3 IP Clock Gating                                            */
      volatile uint32_t _I2C2      : 1;            /*!< [20..20] _I2C2 IP Clock Gating                                            */
      volatile uint32_t _I2C1      : 1;            /*!< [21..21] _I2C1 IP Clock Gating                                            */
      volatile uint32_t _UART2     : 1;            /*!< [22..22] _UART2 IP Clock Gating                                           */
      volatile uint32_t _UART1     : 1;            /*!< [23..23] _UART1 IP Clock Gating                                           */
      volatile uint32_t _UART0     : 1;            /*!< [24..24] _UART0 IP Clock Gating                                           */
      volatile uint32_t _ADC       : 1;            /*!< [25..25] _ADC IP Clock Gating                                             */
      volatile uint32_t _PWM14     : 1;            /*!< [26..26] _PWM14 IP Clock Gating                                           */
      volatile uint32_t _PWM13     : 1;            /*!< [27..27] _PWM13 IP Clock Gating                                           */
      volatile uint32_t _PWM12     : 1;            /*!< [28..28] _PWM12 IP Clock Gating                                           */
      volatile uint32_t _PWM11     : 1;            /*!< [29..29] _PWM11 IP Clock Gating                                           */
      volatile uint32_t _PWM10     : 1;            /*!< [30..30] _PWM10 IP Clock Gating                                           */
      volatile uint32_t _PWM9      : 1;            /*!< [31..31] _PWM9 IP Clock Gating                                            */
    } CLKGATING1_b;
  } ;
  
  union {
    volatile uint32_t CLKGATING2;                  /*!< (@ 0x00000054) IP_CLOCK_GATING_2 EREGISTER                                */
    
    struct {
      volatile uint32_t _I3C2      : 1;            /*!< [0..0] _I3C2 IP Clock Gating                                              */
      volatile uint32_t _I3C1      : 1;            /*!< [1..1] _I3C1 IP Clock Gating                                              */
      volatile uint32_t _I3C4      : 1;            /*!< [2..2] _I3C4 IP Clock Gating                                              */
      volatile uint32_t _LPT       : 1;            /*!< [3..3] _LPT IP Clock Gating                                               */
      volatile uint32_t _VINLED    : 1;            /*!< [4..4] _VINLED IP Clock Gating                                            */
      volatile uint32_t _I2CDBG    : 1;            /*!< [5..5] _I2CDBG IP Clock Gating                                            */
      volatile uint32_t _MICROCNT  : 1;            /*!< [6..6] _MICROCNT IP Clock Gating                                          */
      volatile uint32_t _CEC2      : 1;            /*!< [7..7] _CEC2 IP Clock Gating                                              */
      volatile uint32_t _CEC1      : 1;            /*!< [8..8] _CEC1 IP Clock Gating                                              */
      volatile uint32_t _LEDSTP    : 1;            /*!< [9..9] _LEDSTP IP Clock Gating                                            */
      volatile uint32_t _SLWTMR3   : 1;            /*!< [10..10] _SLWTMR3 IP Clock Gating                                         */
      volatile uint32_t _SLWTMR2   : 1;            /*!< [11..11] _SLWTMR2 IP Clock Gating                                         */
      volatile uint32_t _SLWTMR1   : 1;            /*!< [12..12] _SLWTMR1 IP Clock Gating                                         */
      volatile uint32_t _SLWTMR0   : 1;            /*!< [13..13] _SLWTMR0 IP Clock Gating                                         */
      volatile uint32_t _KBMSCAN   : 1;            /*!< [14..14] _KBMSCAN IP Clock Gating                                         */
      volatile uint32_t _KBM       : 1;            /*!< [15..15] _KBM IP Clock Gating                                             */
      volatile uint32_t _TMR5      : 1;            /*!< [16..16] _TMR5 IP Clock Gating                                            */
      volatile uint32_t _TMR4      : 1;            /*!< [17..17] _TMR4 IP Clock Gating                                            */
      volatile uint32_t _TMR3      : 1;            /*!< [18..18] _TMR3 IP Clock Gating                                            */
      volatile uint32_t _TMR2      : 1;            /*!< [19..19] _TMR2 IP Clock Gating                                            */
      volatile uint32_t _TMR1      : 1;            /*!< [20..20] _TMR1 IP Clock Gating                                            */
      volatile uint32_t _TMR0      : 1;            /*!< [21..21] _TMR0 IP Clock Gating                                            */
      volatile uint32_t _GPIO      : 1;            /*!< [22..22] _GPIO IP Clock Gating                                            */
      volatile uint32_t _TACH14    : 1;            /*!< [23..23] _TACH14 IP Clock Gating                                          */
      volatile uint32_t _TACH13    : 1;            /*!< [24..24] _TACH13 IP Clock Gating                                          */
      volatile uint32_t _TACH12    : 1;            /*!< [25..25] _TACH12 IP Clock Gating                                          */
      volatile uint32_t _TACH11    : 1;            /*!< [26..26] _TACH11 IP Clock Gating                                          */
      volatile uint32_t _TACH10    : 1;            /*!< [27..27] _TACH10 IP Clock Gating                                          */
      volatile uint32_t _TACH9     : 1;            /*!< [28..28] _TACH9 IP Clock Gating                                           */
      volatile uint32_t _TACH8     : 1;            /*!< [29..29] _TACH8 IP Clock Gating                                           */
      volatile uint32_t _TACH7     : 1;            /*!< [30..30] _TACH7 IP Clock Gating                                           */
      volatile uint32_t _TACH6     : 1;            /*!< [31..31] _TACH6 IP Clock Gating                                           */
    } CLKGATING2_b;
  } ;
  
  union {
    union {
      volatile uint32_t APBGATING0;                /*!< (@ 0x00000058) APB_CLOCK_GATING_0 EREGISTER                               */
      
      struct {
        volatile uint32_t _PS2_3   : 1;            /*!< [0..0] _PS2_3 APB Clock Gating                                            */
        volatile uint32_t _PS2_2   : 1;            /*!< [1..1] _PS2_2 APB Clock Gating                                            */
        volatile uint32_t _PS2_1   : 1;            /*!< [2..2] _PS2_1 APB Clock Gating                                            */
        volatile uint32_t _LEDSTP  : 1;            /*!< [3..3] _LEDSTP APB Clock Gating                                           */
        volatile uint32_t _LED4    : 1;            /*!< [4..4] _LED4 APB Clock Gating                                             */
        volatile uint32_t _LED3    : 1;            /*!< [5..5] _LED3 APB Clock Gating                                             */
        volatile uint32_t _LED2    : 1;            /*!< [6..6] _LED2 APB Clock Gating                                             */
        volatile uint32_t _LED1    : 1;            /*!< [7..7] _LED1 APB Clock Gating                                             */
        volatile uint32_t _KBM     : 1;            /*!< [8..8] _KBM APB Clock Gating                                              */
        volatile uint32_t _I2CDBG  : 1;            /*!< [9..9] _I2CDBG APB Clock Gating                                           */
        volatile uint32_t _I2C11   : 1;            /*!< [10..10] _I2C11 APB Clock Gating                                          */
        volatile uint32_t _I2C10   : 1;            /*!< [11..11] _I2C10 APB Clock Gating                                          */
        volatile uint32_t _I2C9    : 1;            /*!< [12..12] _I2C9 APB Clock Gating                                           */
        volatile uint32_t _I2C8    : 1;            /*!< [13..13] _I2C8 APB Clock Gating                                           */
        volatile uint32_t _I2C7    : 1;            /*!< [14..14] _I2C7 APB Clock Gating                                           */
        volatile uint32_t _I2C6    : 1;            /*!< [15..15] _I2C6 APB Clock Gating                                           */
        volatile uint32_t _I2C5    : 1;            /*!< [16..16] _I2C5 APB Clock Gating                                           */
        volatile uint32_t _I2C4    : 1;            /*!< [17..17] _I2C4 APB Clock Gating                                           */
        volatile uint32_t _I2C3    : 1;            /*!< [18..18] _I2C3 APB Clock Gating                                           */
        volatile uint32_t _I2C2    : 1;            /*!< [19..19] _I2C2 APB Clock Gating                                           */
        volatile uint32_t _I2C1    : 1;            /*!< [20..20] _I2C1 APB Clock Gating                                           */
        volatile uint32_t _GPIO    : 1;            /*!< [21..21] _GPIO APB Clock Gating                                           */
        volatile uint32_t _CEC2    : 1;            /*!< [22..22] _CEC2 APB Clock Gating                                           */
        volatile uint32_t _CEC1    : 1;            /*!< [23..23] _CEC1 APB Clock Gating                                           */
        volatile uint32_t _ADC     : 1;            /*!< [24..24] _ADC APB Clock Gating                                            */
              uint32_t          : 7;
      } APBGATING0_b;
    } ;
    
    union {
      volatile uint32_t CLKGATING3;                /*!< (@ 0x00000058) IP_CLOCK_GATING_3 EREGISTER                                */
      
      struct {
              uint32_t          : 25;
        volatile uint32_t _RTMR    : 1;            /*!< [25..25] _RTMR IP Clock Gating                                            */
        volatile uint32_t _PS2_L3  : 1;            /*!< [26..26] _PS2_L3 IP Clock Gating                                          */
        volatile uint32_t _PS2_L2  : 1;            /*!< [27..27] _PS2_L2 IP Clock Gating                                          */
        volatile uint32_t _PS2_L1  : 1;            /*!< [28..28] _PS2_L1 IP Clock Gating                                          */
        volatile uint32_t REF_32K  : 1;            /*!< [29..29] REF_32K IP Clock Gating                                          */
        volatile uint32_t _PECI    : 1;            /*!< [30..30] _PECI IP Clock Gating                                            */
        volatile uint32_t _I3C3    : 1;            /*!< [31..31] _I3C3 IP Clock Gating                                            */
      } CLKGATING3_b;
    } ;
  };
  
  union {
    volatile uint32_t APBGATING1;                  /*!< (@ 0x0000005C) APB_CLOCK_GATING_1 EREGISTER                               */
    
    struct {
      volatile uint32_t _TACH8     : 1;            /*!< [0..0] _TACH8 APB Clock Gating                                            */
      volatile uint32_t _TACH7     : 1;            /*!< [1..1] _TACH7 APB Clock Gating                                            */
      volatile uint32_t _TACH6     : 1;            /*!< [2..2] _TACH6 APB Clock Gating                                            */
      volatile uint32_t _TACH5     : 1;            /*!< [3..3] _TACH5 APB Clock Gating                                            */
      volatile uint32_t _TACH4     : 1;            /*!< [4..4] _TACH4 APB Clock Gating                                            */
      volatile uint32_t _TACH3     : 1;            /*!< [5..5] _TACH3 APB Clock Gating                                            */
      volatile uint32_t _TACH2     : 1;            /*!< [6..6] _TACH2 APB Clock Gating                                            */
      volatile uint32_t _TACH1     : 1;            /*!< [7..7] _TACH1 APB Clock Gating                                            */
      volatile uint32_t _SPIM      : 1;            /*!< [8..8] _SPIM APB Clock Gating                                             */
      volatile uint32_t _SPIS      : 1;            /*!< [9..9] _SPIS APB Clock Gating                                             */
      volatile uint32_t _SLWTMR3   : 1;            /*!< [10..10] _SLWTMR3 APB Clock Gating                                        */
      volatile uint32_t _SLWTMR2   : 1;            /*!< [11..11] _SLWTMR2 APB Clock Gating                                        */
      volatile uint32_t _SLWTMR1   : 1;            /*!< [12..12] _SLWTMR1 APB Clock Gating                                        */
      volatile uint32_t _SLWTMR0   : 1;            /*!< [13..13] _SLWTMR0 APB Clock Gating                                        */
      volatile uint32_t _RTMR      : 1;            /*!< [14..14] _RTMR APB Clock Gating                                           */
      volatile uint32_t _RTC       : 1;            /*!< [15..15] _RTC APB Clock Gating                                            */
      volatile uint32_t _PWRBTN1   : 1;            /*!< [16..16] _PWRBTN1 APB Clock Gating                                        */
      volatile uint32_t _PWRBTN0   : 1;            /*!< [17..17] _PWRBTN0 APB Clock Gating                                        */
      volatile uint32_t _PWM14     : 1;            /*!< [18..18] _PWM14 APB Clock Gating                                          */
      volatile uint32_t _PWM13     : 1;            /*!< [19..19] _PWM13 APB Clock Gating                                          */
      volatile uint32_t _PWM12     : 1;            /*!< [20..20] _PWM12 APB Clock Gating                                          */
      volatile uint32_t _PWM11     : 1;            /*!< [21..21] _PWM11 APB Clock Gating                                          */
      volatile uint32_t _PWM10     : 1;            /*!< [22..22] _PWM10 APB Clock Gating                                          */
      volatile uint32_t _PWM9      : 1;            /*!< [23..23] _PWM9 APB Clock Gating                                           */
      volatile uint32_t _PWM8      : 1;            /*!< [24..24] _PWM8 APB Clock Gating                                           */
      volatile uint32_t _PWM7      : 1;            /*!< [25..25] _PWM7 APB Clock Gating                                           */
      volatile uint32_t _PWM6      : 1;            /*!< [26..26] _PWM6 APB Clock Gating                                           */
      volatile uint32_t _PWM5      : 1;            /*!< [27..27] _PWM5 APB Clock Gating                                           */
      volatile uint32_t _PWM4      : 1;            /*!< [28..28] _PWM4 APB Clock Gating                                           */
      volatile uint32_t _PWM3      : 1;            /*!< [29..29] _PWM3 APB Clock Gating                                           */
      volatile uint32_t _PWM2      : 1;            /*!< [30..30] _PWM2 APB Clock Gating                                           */
      volatile uint32_t _PWM1      : 1;            /*!< [31..31] _PWM1 APB Clock Gating                                           */
    } APBGATING1_b;
  } ;
  
  union {
    union {
      volatile uint32_t CLKGATING4;                /*!< (@ 0x00000060) IP_CLOCK_GATING_4 EREGISTER                                */
      
      struct {
              uint32_t          : 5;
        volatile uint32_t _USBDHY  : 1;            /*!< [5..5] _USBDHY IP Clock Gating                                            */
        volatile uint32_t _USB     : 1;            /*!< [6..6] _USB IP Clock Gating                                               */
        volatile uint32_t _TRC     : 1;            /*!< [7..7] _TRC IP Clock Gating                                               */
        volatile uint32_t _LALU    : 1;            /*!< [8..8] _LALU IP Clock Gating                                              */
              uint32_t          : 23;
      } CLKGATING4_b;
    } ;
    
    union {
      volatile uint32_t APBGATING2;                /*!< (@ 0x00000060) APB_CLOCK_GATING_2 EREGISTER                               */
      
      struct {
              uint32_t          : 9;
        volatile uint32_t _TRC     : 1;            /*!< [9..9] _TRC APB Clock Gating                                              */
        volatile uint32_t _PECI    : 1;            /*!< [10..10] _PECI APB Clock Gating                                           */
        volatile uint32_t _LPT     : 1;            /*!< [11..11] _LPT APB Clock Gating                                            */
        volatile uint32_t _WDT     : 1;            /*!< [12..12] _WDT APB Clock Gating                                            */
        volatile uint32_t _UART2   : 1;            /*!< [13..13] _UART2 APB Clock Gating                                          */
        volatile uint32_t _UART1   : 1;            /*!< [14..14] _UART1 APB Clock Gating                                          */
        volatile uint32_t _UART0   : 1;            /*!< [15..15] _UART0 APB Clock Gating                                          */
        volatile uint32_t _TMR5    : 1;            /*!< [16..16] _TMR5 APB Clock Gating                                           */
        volatile uint32_t _TMR4    : 1;            /*!< [17..17] _TMR4 APB Clock Gating                                           */
        volatile uint32_t _TMR3    : 1;            /*!< [18..18] _TMR3 APB Clock Gating                                           */
        volatile uint32_t _TMR2    : 1;            /*!< [19..19] _TMR2 APB Clock Gating                                           */
        volatile uint32_t _TMR1    : 1;            /*!< [20..20] _TMR1 APB Clock Gating                                           */
        volatile uint32_t _TMR0    : 1;            /*!< [21..21] _TMR0 APB Clock Gating                                           */
        volatile uint32_t _TACH14  : 1;            /*!< [22..22] _TACH14 APB Clock Gating                                         */
        volatile uint32_t _TACH13  : 1;            /*!< [23..23] _TACH13 APB Clock Gating                                         */
        volatile uint32_t _TACH12  : 1;            /*!< [24..24] _TACH12 APB Clock Gating                                         */
        volatile uint32_t _TACH11  : 1;            /*!< [25..25] _TACH11 APB Clock Gating                                         */
        volatile uint32_t _TACH10  : 1;            /*!< [26..26] _TACH10 APB Clock Gating                                         */
        volatile uint32_t _TACH9   : 1;            /*!< [27..27] _TACH9 APB Clock Gating                                          */
              uint32_t          : 4;
      } APBGATING2_b;
    } ;
  };
  
  union {
    volatile uint32_t IPRST0;                      /*!< (@ 0x00000064) IP_RESET_0 EREGISTER                                       */
    
    struct {
      volatile uint32_t _PWM8      : 1;            /*!< [0..0] _PWM8 IP Reset                                                     */
      volatile uint32_t _PWM7      : 1;            /*!< [1..1] _PWM7 IP Reset                                                     */
      volatile uint32_t _PWM6      : 1;            /*!< [2..2] _PWM6 IP Reset                                                     */
      volatile uint32_t _PWM5      : 1;            /*!< [3..3] _PWM5 IP Reset                                                     */
      volatile uint32_t _PWM4      : 1;            /*!< [4..4] _PWM4 IP Reset                                                     */
      volatile uint32_t _PWM3      : 1;            /*!< [5..5] _PWM3 IP Reset                                                     */
      volatile uint32_t _PWM2      : 1;            /*!< [6..6] _PWM2 IP Reset                                                     */
      volatile uint32_t _PWM1      : 1;            /*!< [7..7] _PWM1 IP Reset                                                     */
      volatile uint32_t _LED4      : 1;            /*!< [8..8] _LED4 IP Reset                                                     */
      volatile uint32_t _LED3      : 1;            /*!< [9..9] _LED3 IP Reset                                                     */
      volatile uint32_t _LED2      : 1;            /*!< [10..10] _LED2 IP Reset                                                   */
      volatile uint32_t _LED1      : 1;            /*!< [11..11] _LED1 IP Reset                                                   */
      volatile uint32_t _EMI7      : 1;            /*!< [12..12] _EMI7 IP Reset                                                   */
      volatile uint32_t _EMI6      : 1;            /*!< [13..13] _EMI6 IP Reset                                                   */
      volatile uint32_t _EMI5      : 1;            /*!< [14..14] _EMI5 IP Reset                                                   */
      volatile uint32_t _EMI4      : 1;            /*!< [15..15] _EMI4 IP Reset                                                   */
      volatile uint32_t _EMI3      : 1;            /*!< [16..16] _EMI3 IP Reset                                                   */
      volatile uint32_t _EMI2      : 1;            /*!< [17..17] _EMI2 IP Reset                                                   */
      volatile uint32_t _EMI1      : 1;            /*!< [18..18] _EMI1 IP Reset                                                   */
      volatile uint32_t _EMI0      : 1;            /*!< [19..19] _EMI0 IP Reset                                                   */
      volatile uint32_t _PORT80    : 1;            /*!< [20..20] _PORT80 IP Reset                                                 */
      volatile uint32_t _PMPORT3   : 1;            /*!< [21..21] _PMPORT3 IP Reset                                                */
      volatile uint32_t _PMPORT2   : 1;            /*!< [22..22] _PMPORT2 IP Reset                                                */
      volatile uint32_t _PMPORT1   : 1;            /*!< [23..23] _PMPORT1 IP Reset                                                */
      volatile uint32_t _PMPORT0   : 1;            /*!< [24..24] _PMPORT0 IP Reset                                                */
      volatile uint32_t _ACPI      : 1;            /*!< [25..25] _ACPI IP Reset                                                   */
      volatile uint32_t _KBC       : 1;            /*!< [26..26] _KBC IP Reset                                                    */
      volatile uint32_t _ESPI      : 1;            /*!< [27..27] _ESPI IP Reset                                                   */
      volatile uint32_t _DMA       : 1;            /*!< [28..28] _DMA IP Reset                                                    */
      volatile uint32_t PUF_OTP    : 1;            /*!< [29..29] PUF_OTP IP Reset                                                 */
            uint32_t            : 1;
      volatile uint32_t _BUS       : 1;            /*!< [31..31] _BUS IP Reset                                                    */
    } IPRST0_b;
  } ;
  
  union {
    volatile uint32_t IPRST1;                      /*!< (@ 0x00000068) IP_RESET_1 EREGISTER                                       */
    
    struct {
      volatile uint32_t _TACH5     : 1;            /*!< [0..0] _TACH5 IP Reset                                                    */
      volatile uint32_t _TACH4     : 1;            /*!< [1..1] _TACH4 IP Reset                                                    */
      volatile uint32_t _TACH3     : 1;            /*!< [2..2] _TACH3 IP Reset                                                    */
      volatile uint32_t _TACH2     : 1;            /*!< [3..3] _TACH2 IP Reset                                                    */
      volatile uint32_t _TACH1     : 1;            /*!< [4..4] _TACH1 IP Reset                                                    */
      volatile uint32_t _PS2_H3    : 1;            /*!< [5..5] _PS2_H3 IP Reset                                                   */
      volatile uint32_t _PS2_H2    : 1;            /*!< [6..6] _PS2_H2 IP Reset                                                   */
      volatile uint32_t _PS2_H1    : 1;            /*!< [7..7] _PS2_H1 IP Reset                                                   */
      volatile uint32_t RST        : 1;            /*!< [8..8] RST IP Reset                                                       */
      volatile uint32_t RST25M     : 1;            /*!< [9..9] RST25M IP Reset                                                    */
      volatile uint32_t RST24M     : 1;            /*!< [10..10] RST24M IP Reset                                                  */
      volatile uint32_t _I2C11     : 1;            /*!< [11..11] _I2C11 IP Reset                                                  */
      volatile uint32_t _I2C10     : 1;            /*!< [12..12] _I2C10 IP Reset                                                  */
      volatile uint32_t _I2C9      : 1;            /*!< [13..13] _I2C9 IP Reset                                                   */
      volatile uint32_t _I2C8      : 1;            /*!< [14..14] _I2C8 IP Reset                                                   */
      volatile uint32_t _I2C7      : 1;            /*!< [15..15] _I2C7 IP Reset                                                   */
      volatile uint32_t _I2C6      : 1;            /*!< [16..16] _I2C6 IP Reset                                                   */
      volatile uint32_t _I2C5      : 1;            /*!< [17..17] _I2C5 IP Reset                                                   */
      volatile uint32_t _I2C4      : 1;            /*!< [18..18] _I2C4 IP Reset                                                   */
      volatile uint32_t _I2C3      : 1;            /*!< [19..19] _I2C3 IP Reset                                                   */
      volatile uint32_t _I2C2      : 1;            /*!< [20..20] _I2C2 IP Reset                                                   */
      volatile uint32_t _I2C1      : 1;            /*!< [21..21] _I2C1 IP Reset                                                   */
      volatile uint32_t _UART2     : 1;            /*!< [22..22] _UART2 IP Reset                                                  */
      volatile uint32_t _UART1     : 1;            /*!< [23..23] _UART1 IP Reset                                                  */
      volatile uint32_t _UART0     : 1;            /*!< [24..24] _UART0 IP Reset                                                  */
      volatile uint32_t _ADC       : 1;            /*!< [25..25] _ADC IP Reset                                                    */
      volatile uint32_t _PWM14     : 1;            /*!< [26..26] _PWM14 IP Reset                                                  */
      volatile uint32_t _PWM13     : 1;            /*!< [27..27] _PWM13 IP Reset                                                  */
      volatile uint32_t _PWM12     : 1;            /*!< [28..28] _PWM12 IP Reset                                                  */
      volatile uint32_t _PWM11     : 1;            /*!< [29..29] _PWM11 IP Reset                                                  */
      volatile uint32_t _PWM10     : 1;            /*!< [30..30] _PWM10 IP Reset                                                  */
      volatile uint32_t _PWM9      : 1;            /*!< [31..31] _PWM9 IP Reset                                                   */
    } IPRST1_b;
  } ;
  
  union {
    volatile uint32_t IPRST2;                      /*!< (@ 0x0000006C) IP_RESET_2 EREGISTER                                       */
    
    struct {
      volatile uint32_t _I3C2      : 1;            /*!< [0..0] _I3C2 IP Reset                                                     */
      volatile uint32_t _I3C1      : 1;            /*!< [1..1] _I3C1 IP Reset                                                     */
      volatile uint32_t _I3C4      : 1;            /*!< [2..2] _I3C4 IP Reset                                                     */
      volatile uint32_t _LPT       : 1;            /*!< [3..3] _LPT IP Reset                                                      */
      volatile uint32_t _VINLED    : 1;            /*!< [4..4] _VINLED IP Reset                                                   */
      volatile uint32_t _I2CDBG    : 1;            /*!< [5..5] _I2CDBG IP Reset                                                   */
      volatile uint32_t _MICROCNT  : 1;            /*!< [6..6] _MICROCNT IP Reset                                                 */
      volatile uint32_t _CEC2      : 1;            /*!< [7..7] _CEC2 IP Reset                                                     */
      volatile uint32_t _CEC1      : 1;            /*!< [8..8] _CEC1 IP Reset                                                     */
      volatile uint32_t _LEDSTP    : 1;            /*!< [9..9] _LEDSTP IP Reset                                                   */
      volatile uint32_t _SLWTMR3   : 1;            /*!< [10..10] _SLWTMR3 IP Reset                                                */
      volatile uint32_t _SLWTMR2   : 1;            /*!< [11..11] _SLWTMR2 IP Reset                                                */
      volatile uint32_t _SLWTMR1   : 1;            /*!< [12..12] _SLWTMR1 IP Reset                                                */
      volatile uint32_t _SLWTMR0   : 1;            /*!< [13..13] _SLWTMR0 IP Reset                                                */
      volatile uint32_t _KBMSCAN   : 1;            /*!< [14..14] _KBMSCAN IP Reset                                                */
      volatile uint32_t _KBM       : 1;            /*!< [15..15] _KBM IP Reset                                                    */
      volatile uint32_t _TMR5      : 1;            /*!< [16..16] _TMR5 IP Reset                                                   */
      volatile uint32_t _TMR4      : 1;            /*!< [17..17] _TMR4 IP Reset                                                   */
      volatile uint32_t _TMR3      : 1;            /*!< [18..18] _TMR3 IP Reset                                                   */
      volatile uint32_t _TMR2      : 1;            /*!< [19..19] _TMR2 IP Reset                                                   */
      volatile uint32_t _TMR1      : 1;            /*!< [20..20] _TMR1 IP Reset                                                   */
      volatile uint32_t _TMR0      : 1;            /*!< [21..21] _TMR0 IP Reset                                                   */
      volatile uint32_t _GPIO      : 1;            /*!< [22..22] _GPIO IP Reset                                                   */
      volatile uint32_t _TACH14    : 1;            /*!< [23..23] _TACH14 IP Reset                                                 */
      volatile uint32_t _TACH13    : 1;            /*!< [24..24] _TACH13 IP Reset                                                 */
      volatile uint32_t _TACH12    : 1;            /*!< [25..25] _TACH12 IP Reset                                                 */
      volatile uint32_t _TACH11    : 1;            /*!< [26..26] _TACH11 IP Reset                                                 */
      volatile uint32_t _TACH10    : 1;            /*!< [27..27] _TACH10 IP Reset                                                 */
      volatile uint32_t _TACH9     : 1;            /*!< [28..28] _TACH9 IP Reset                                                  */
      volatile uint32_t _TACH8     : 1;            /*!< [29..29] _TACH8 IP Reset                                                  */
      volatile uint32_t _TACH7     : 1;            /*!< [30..30] _TACH7 IP Reset                                                  */
      volatile uint32_t _TACH6     : 1;            /*!< [31..31] _TACH6 IP Reset                                                  */
    } IPRST2_b;
  } ;
  
  union {
    union {
      volatile uint32_t APBRST0;                   /*!< (@ 0x00000070) APB_RESET_0 EREGISTER                                      */
      
      struct {
        volatile uint32_t _PS2_3   : 1;            /*!< [0..0] _PS2_3 APB Reset                                                   */
        volatile uint32_t _PS2_2   : 1;            /*!< [1..1] _PS2_2 APB Reset                                                   */
        volatile uint32_t _PS2_1   : 1;            /*!< [2..2] _PS2_1 APB Reset                                                   */
        volatile uint32_t _LEDSTP  : 1;            /*!< [3..3] _LEDSTP APB Reset                                                  */
        volatile uint32_t _LED4    : 1;            /*!< [4..4] _LED4 APB Reset                                                    */
        volatile uint32_t _LED3    : 1;            /*!< [5..5] _LED3 APB Reset                                                    */
        volatile uint32_t _LED2    : 1;            /*!< [6..6] _LED2 APB Reset                                                    */
        volatile uint32_t _LED1    : 1;            /*!< [7..7] _LED1 APB Reset                                                    */
        volatile uint32_t _KBM     : 1;            /*!< [8..8] _KBM APB Reset                                                     */
        volatile uint32_t _I2CDBG  : 1;            /*!< [9..9] _I2CDBG APB Reset                                                  */
        volatile uint32_t _I2C11   : 1;            /*!< [10..10] _I2C11 APB Reset                                                 */
        volatile uint32_t _I2C10   : 1;            /*!< [11..11] _I2C10 APB Reset                                                 */
        volatile uint32_t _I2C9    : 1;            /*!< [12..12] _I2C9 APB Reset                                                  */
        volatile uint32_t _I2C8    : 1;            /*!< [13..13] _I2C8 APB Reset                                                  */
        volatile uint32_t _I2C7    : 1;            /*!< [14..14] _I2C7 APB Reset                                                  */
        volatile uint32_t _I2C6    : 1;            /*!< [15..15] _I2C6 APB Reset                                                  */
        volatile uint32_t _I2C5    : 1;            /*!< [16..16] _I2C5 APB Reset                                                  */
        volatile uint32_t _I2C4    : 1;            /*!< [17..17] _I2C4 APB Reset                                                  */
        volatile uint32_t _I2C3    : 1;            /*!< [18..18] _I2C3 APB Reset                                                  */
        volatile uint32_t _I2C2    : 1;            /*!< [19..19] _I2C2 APB Reset                                                  */
        volatile uint32_t _I2C1    : 1;            /*!< [20..20] _I2C1 APB Reset                                                  */
        volatile uint32_t _GPIO    : 1;            /*!< [21..21] _GPIO APB Reset                                                  */
        volatile uint32_t _CEC2    : 1;            /*!< [22..22] _CEC2 APB Reset                                                  */
        volatile uint32_t _CEC1    : 1;            /*!< [23..23] _CEC1 APB Reset                                                  */
        volatile uint32_t _ADC     : 1;            /*!< [24..24] _ADC APB Reset                                                   */
              uint32_t          : 7;
      } APBRST0_b;
    } ;
    
    union {
      volatile uint32_t IPRST3;                    /*!< (@ 0x00000070) IP_RESET_3 EREGISTER                                       */
      
      struct {
              uint32_t          : 25;
        volatile uint32_t _RTMR    : 1;            /*!< [25..25] _RTMR IP Reset                                                   */
        volatile uint32_t _PS2_L3  : 1;            /*!< [26..26] _PS2_L3 IP Reset                                                 */
        volatile uint32_t _PS2_L2  : 1;            /*!< [27..27] _PS2_L2 IP Reset                                                 */
        volatile uint32_t _PS2_L1  : 1;            /*!< [28..28] _PS2_L1 IP Resetg                                                */
        volatile uint32_t REF32K   : 1;            /*!< [29..29] REF32K IP Reset                                                  */
        volatile uint32_t _PECI    : 1;            /*!< [30..30] _PECI IP Reset                                                   */
        volatile uint32_t _I3C3    : 1;            /*!< [31..31] _I3C3 IP Reset                                                   */
      } IPRST3_b;
    } ;
  };
  
  union {
    volatile uint32_t APBRST1;                     /*!< (@ 0x00000074) APB_RESET_1 EREGISTER                                      */
    
    struct {
      volatile uint32_t _TACH8     : 1;            /*!< [0..0] _TACH8 APB Reset                                                   */
      volatile uint32_t _TACH7     : 1;            /*!< [1..1] _TACH7 APB Reset                                                   */
      volatile uint32_t _TACH6     : 1;            /*!< [2..2] _TACH6 APB Reset                                                   */
      volatile uint32_t _TACH5     : 1;            /*!< [3..3] _TACH5 APB Reset                                                   */
      volatile uint32_t _TACH4     : 1;            /*!< [4..4] _TACH4 APB Reset                                                   */
      volatile uint32_t _TACH3     : 1;            /*!< [5..5] _TACH3 APB Reset                                                   */
      volatile uint32_t _TACH2     : 1;            /*!< [6..6] _TACH2 APB Reset                                                   */
      volatile uint32_t _TACH1     : 1;            /*!< [7..7] _TACH1 APB Reset                                                   */
      volatile uint32_t _SPIM      : 1;            /*!< [8..8] _SPIM APB Reset                                                    */
      volatile uint32_t _SPIS      : 1;            /*!< [9..9] _SPIS APB Reset                                                    */
      volatile uint32_t _SLWTMR3   : 1;            /*!< [10..10] _SLWTMR3 APB Reset                                               */
      volatile uint32_t _SLWTMR2   : 1;            /*!< [11..11] _SLWTMR2 APB Reset                                               */
      volatile uint32_t _SLWTMR1   : 1;            /*!< [12..12] _SLWTMR1 APB Reset                                               */
      volatile uint32_t _SLWTMR0   : 1;            /*!< [13..13] _SLWTMR0 APB Reset                                               */
      volatile uint32_t _RTMR      : 1;            /*!< [14..14] _RTMR APB Reset                                                  */
      volatile uint32_t _RTC       : 1;            /*!< [15..15] _RTC APB Reset                                                   */
            uint32_t            : 2;
      volatile uint32_t _PWM14     : 1;            /*!< [18..18] _PWM14 APB Reset                                                 */
      volatile uint32_t _PWM13     : 1;            /*!< [19..19] _PWM13 APB Reset                                                 */
      volatile uint32_t _PWM12     : 1;            /*!< [20..20] _PWM12 APB Reset                                                 */
      volatile uint32_t _PWM11     : 1;            /*!< [21..21] _PWM11 APB Reset                                                 */
      volatile uint32_t _PWM10     : 1;            /*!< [22..22] _PWM10 APB Reset                                                 */
      volatile uint32_t _PWM9      : 1;            /*!< [23..23] _PWM9 APB Reset                                                  */
      volatile uint32_t _PWM8      : 1;            /*!< [24..24] _PWM8 APB Reset                                                  */
      volatile uint32_t _PWM7      : 1;            /*!< [25..25] _PWM7 APB Reset                                                  */
      volatile uint32_t _PWM6      : 1;            /*!< [26..26] _PWM6 APB Reset                                                  */
      volatile uint32_t _PWM5      : 1;            /*!< [27..27] _PWM5 APB Reset                                                  */
      volatile uint32_t _PWM4      : 1;            /*!< [28..28] _PWM4 APB Reset                                                  */
      volatile uint32_t _PWM3      : 1;            /*!< [29..29] _PWM3 APB Reset                                                  */
      volatile uint32_t _PWM2      : 1;            /*!< [30..30] _PWM2 APB Reset                                                  */
      volatile uint32_t _PWM1      : 1;            /*!< [31..31] _PWM1 APB Reset                                                  */
    } APBRST1_b;
  } ;
  
  union {
    union {
      volatile uint32_t IPRST4;                    /*!< (@ 0x00000078) IP_RESET_4 EREGISTER                                       */
      
      struct {
        volatile uint32_t _USBDHY  : 1;            /*!< [0..0] _USBDHY IP Reset                                                   */
              uint32_t          : 5;
        volatile uint32_t _USB     : 1;            /*!< [6..6] _USB IP Reset                                                      */
        volatile uint32_t _TRC     : 1;            /*!< [7..7] _TRC IP Reset                                                      */
        volatile uint32_t _LALU    : 1;            /*!< [8..8] _LALU IP Reset                                                     */
              uint32_t          : 23;
      } IPRST4_b;
    } ;
    
    union {
      volatile uint32_t APBRST2;                   /*!< (@ 0x00000078) APB_RESET_2 EREGISTER                                      */
      
      struct {
              uint32_t          : 9;
        volatile uint32_t _TRC     : 1;            /*!< [9..9] _TRC APB Reset                                                     */
        volatile uint32_t _PECI    : 1;            /*!< [10..10] _PECI APB Reset                                                  */
        volatile uint32_t _LPT     : 1;            /*!< [11..11] _LPT APB Reset                                                   */
              uint32_t          : 1;
        volatile uint32_t _UART2   : 1;            /*!< [13..13] _UART2 APB Reset                                                 */
        volatile uint32_t _UART1   : 1;            /*!< [14..14] _UART1 APB Reset                                                 */
        volatile uint32_t _UART0   : 1;            /*!< [15..15] _UART0 APB Reset                                                 */
        volatile uint32_t _TMR5    : 1;            /*!< [16..16] _TMR5 APB Reset                                                  */
        volatile uint32_t _TMR4    : 1;            /*!< [17..17] _TMR4 APB Reset                                                  */
        volatile uint32_t _TMR3    : 1;            /*!< [18..18] _TMR3 APB Reset                                                  */
        volatile uint32_t _TMR2    : 1;            /*!< [19..19] _TMR2 APB Reset                                                  */
        volatile uint32_t _TMR1    : 1;            /*!< [20..20] _TMR1 APB Reset                                                  */
        volatile uint32_t _TMR0    : 1;            /*!< [21..21] _TMR0 APB Reset                                                  */
        volatile uint32_t _TACH14  : 1;            /*!< [22..22] _TACH14 APB Reset                                                */
        volatile uint32_t _TACH13  : 1;            /*!< [23..23] _TACH13 APB Reset                                                */
        volatile uint32_t _TACH12  : 1;            /*!< [24..24] _TACH12 APB Reset                                                */
        volatile uint32_t _TACH11  : 1;            /*!< [25..25] _TACH11 APB Reset                                                */
        volatile uint32_t _TACH10  : 1;            /*!< [26..26] _TACH10 APB Reset                                                */
        volatile uint32_t _TACH9   : 1;            /*!< [27..27] _TACH9 APB Reset                                                 */
              uint32_t          : 4;
      } APBRST2_b;
    } ;
  };
  
  union {
    volatile uint32_t SOFTRST0;                    /*!< (@ 0x0000007C) SOFT_RESET_0 EREGISTER                                     */
    
    struct {
      volatile uint32_t _PWM7      : 1;            /*!< [0..0] _PWM7 Soft Reset                                                   */
      volatile uint32_t _PWM6      : 1;            /*!< [1..1] _PWM6 Soft Reset                                                   */
      volatile uint32_t _PWM5      : 1;            /*!< [2..2] _PWM5 Soft Reset                                                   */
      volatile uint32_t _PWM4      : 1;            /*!< [3..3] _PWM4 Soft Reset                                                   */
      volatile uint32_t _PWM3      : 1;            /*!< [4..4] _PWM3 Soft Reset                                                   */
      volatile uint32_t _PWM2      : 1;            /*!< [5..5] _PWM2 Soft Reset                                                   */
      volatile uint32_t _PWM1      : 1;            /*!< [6..6] _PWM1 Soft Reset                                                   */
            uint32_t            : 3;
      volatile uint32_t _LEDSTP    : 1;            /*!< [10..10] _LEDSTP Soft Reset                                               */
      volatile uint32_t _LED4      : 1;            /*!< [11..11] _LED4 Soft Reset                                                 */
      volatile uint32_t _LED3      : 1;            /*!< [12..12] _LED3 Soft Reset                                                 */
      volatile uint32_t _LED2      : 1;            /*!< [13..13] _LED2 Soft Reset                                                 */
      volatile uint32_t _LED1      : 1;            /*!< [14..14] _LED1 Soft Reset                                                 */
      volatile uint32_t _KBM       : 1;            /*!< [15..15] _KBM Soft Reset                                                  */
      volatile uint32_t _I2CDBG    : 1;            /*!< [16..16] _I2CDBG Soft Reset                                               */
      volatile uint32_t _I2C11     : 1;            /*!< [17..17] _I2C11 Soft Reset                                                */
      volatile uint32_t _I2C10     : 1;            /*!< [18..18] _I2C10 Soft Reset                                                */
      volatile uint32_t _I2C9      : 1;            /*!< [19..19] _I2C9 Soft Reset                                                 */
      volatile uint32_t _I2C8      : 1;            /*!< [20..20] _I2C8 Soft Reset                                                 */
      volatile uint32_t _I2C7      : 1;            /*!< [21..21] _I2C7 Soft Reset                                                 */
      volatile uint32_t _I2C6      : 1;            /*!< [22..22] _I2C6 Soft Reset                                                 */
      volatile uint32_t _I2C5      : 1;            /*!< [23..23] _I2C5 Soft Reset                                                 */
      volatile uint32_t _I2C4      : 1;            /*!< [24..24] _I2C4 Soft Reset                                                 */
      volatile uint32_t _I2C3      : 1;            /*!< [25..25] _I2C3 Soft Reset                                                 */
      volatile uint32_t _I2C2      : 1;            /*!< [26..26] _I2C2 Soft Reset                                                 */
      volatile uint32_t _I2C1      : 1;            /*!< [27..27] _I2C1 Soft Reset                                                 */
      volatile uint32_t _GPIO      : 1;            /*!< [28..28] _GPIO Soft Reset                                                 */
      volatile uint32_t _CEC2      : 1;            /*!< [29..29] _CEC2 Soft Reset                                                 */
      volatile uint32_t _CEC1      : 1;            /*!< [30..30] _CEC1 Soft Reset                                                 */
      volatile uint32_t _ADC       : 1;            /*!< [31..31] _ADC Soft Reset                                                  */
    } SOFTRST0_b;
  } ;
  
  union {
    volatile uint32_t SOFTRST1;                    /*!< (@ 0x00000080) SOFT_RESET_1 EREGISTER                                     */
    
    struct {
      volatile uint32_t TMR0       : 1;            /*!< [0..0] TMR0 Soft Reset                                                    */
      volatile uint32_t _TACH14    : 1;            /*!< [1..1] _TACH14 Soft Reset                                                 */
      volatile uint32_t _TACH13    : 1;            /*!< [2..2] _TACH13 Soft Reset                                                 */
      volatile uint32_t _TACH12    : 1;            /*!< [3..3] _TACH12 Soft Reset                                                 */
      volatile uint32_t _TACH11    : 1;            /*!< [4..4] _TACH11 Soft Reset                                                 */
      volatile uint32_t _TACH10    : 1;            /*!< [5..5] _TACH10 Soft Reset                                                 */
      volatile uint32_t _TACH9     : 1;            /*!< [6..6] _TACH9 Soft Reset                                                  */
      volatile uint32_t _TACH8     : 1;            /*!< [7..7] _TACH8 Soft Reset                                                  */
      volatile uint32_t _TACH7     : 1;            /*!< [8..8] _TACH7 Soft Reset                                                  */
      volatile uint32_t _TACH6     : 1;            /*!< [9..9] _TACH6 Soft Reset                                                  */
      volatile uint32_t _TACH5     : 1;            /*!< [10..10] _TACH5 Soft Reset                                                */
      volatile uint32_t _TACH4     : 1;            /*!< [11..11] _TACH4 Soft Reset                                                */
      volatile uint32_t _TACH3     : 1;            /*!< [12..12] _TACH3 Soft Reset                                                */
      volatile uint32_t _TACH2     : 1;            /*!< [13..13] _TACH2 Soft Reset                                                */
      volatile uint32_t _TACH1     : 1;            /*!< [14..14] _TACH1 Soft Reset                                                */
      volatile uint32_t _SPIM      : 1;            /*!< [15..15] _SPIM Soft Reset                                                 */
      volatile uint32_t _SPIS      : 1;            /*!< [16..16] _SPIS Soft Reset                                                 */
      volatile uint32_t _SLWTMR3   : 1;            /*!< [17..17] _SLWTMR3 Soft Reset                                              */
      volatile uint32_t _SLWTMR2   : 1;            /*!< [18..18] _SLWTMR2 Soft Reset                                              */
      volatile uint32_t _SLWTMR1   : 1;            /*!< [19..19] _SLWTMR1 Soft Reset                                              */
      volatile uint32_t _SLWTMR0   : 1;            /*!< [20..20] _SLWTMR0 Soft Reset                                              */
      volatile uint32_t _RTMR      : 1;            /*!< [21..21] _RTMR Soft Reset                                                 */
            uint32_t            : 3;
      volatile uint32_t _PWM14     : 1;            /*!< [25..25] _PWM14 Soft Reset                                                */
      volatile uint32_t _PWM13     : 1;            /*!< [26..26] _PWM13 Soft Reset                                                */
      volatile uint32_t _PWM12     : 1;            /*!< [27..27] _PWM12 Soft Reset                                                */
      volatile uint32_t _PWM11     : 1;            /*!< [28..28] _PWM11 Soft Reset                                                */
      volatile uint32_t _PWM10     : 1;            /*!< [29..29] _PWM10 Soft Reset                                                */
      volatile uint32_t _PWM9      : 1;            /*!< [30..30] _PWM9 Soft Reset                                                 */
      volatile uint32_t _PWM8      : 1;            /*!< [31..31] _PWM8 Soft Reset                                                 */
    } SOFTRST1_b;
  } ;
  
  union {
    volatile uint32_t SOFTRST2;                    /*!< (@ 0x00000084) SOFT_RESET_2 EREGISTER                                     */
    
    struct {
      volatile uint32_t _TRC       : 1;            /*!< [0..0] _TRC Soft Reset                                                    */
      volatile uint32_t _PECI      : 1;            /*!< [1..1] _PECI Soft Reset                                                   */
      volatile uint32_t _LPT       : 1;            /*!< [2..2] _LPT Soft Reset                                                    */
            uint32_t            : 1;
      volatile uint32_t _UART2     : 1;            /*!< [4..4] _UART2 Soft Reset                                                  */
      volatile uint32_t _UART1     : 1;            /*!< [5..5] _UART1 Soft Reset                                                  */
      volatile uint32_t _UART0     : 1;            /*!< [6..6] _UART0 Soft Reset                                                  */
      volatile uint32_t _TMR5      : 1;            /*!< [7..7] _TMR5 Soft Reset                                                   */
      volatile uint32_t _TMR4      : 1;            /*!< [8..8] _TMR4 Soft Reset                                                   */
      volatile uint32_t _TMR3      : 1;            /*!< [9..9] _TMR3 Soft Reset                                                   */
      volatile uint32_t _TMR2      : 1;            /*!< [10..10] _TMR2 Soft Reset                                                 */
      volatile uint32_t _TMR1      : 1;            /*!< [11..11] _TMR1 Soft Reset                                                 */
            uint32_t            : 20;
    } SOFTRST2_b;
  } ;
  volatile const uint32_t  RESERVED[30];
  
  union {
    volatile uint32_t CPU;                         /*!< (@ 0x00000100) CPU REGISTER                                               */
    
    struct {
      volatile uint32_t RSTALL     : 1;            /*!< [0..0] Soft Reset All Regsiters                                           */
      volatile uint32_t LDO2REGRST : 1;            /*!< [1..1] Reset LDO2 Register                                                */
      volatile uint32_t PADRSTLOG  : 1;            /*!< [2..2] Pad Reset Log                                                      */
      volatile uint32_t LDO2RST    : 1;            /*!< [3..3] LDO2 Reset                                                         */
      volatile uint32_t CPUNUM     : 6;            /*!< [9..4] CPU NUM                                                            */
      volatile uint32_t CPUINIRST  : 1;            /*!< [10..10] CPU Initial Access Address Reset                                 */
      volatile uint32_t CPUINIREM  : 1;            /*!< [11..11] CPU Initial Access Address Remain                                */
            uint32_t            : 11;
      volatile uint32_t RESETINLOCK : 1;           /*!< [23..23] Lock pad_reset_in                                                */
            uint32_t            : 8;
    } CPU_b;
  } ;
  
  union {
    volatile uint32_t SLPCTRL;                     /*!< (@ 0x00000104) SYSTEM SLEEP CONTROL REGISTER                              */
    
    struct {
            uint32_t            : 1;
      volatile uint32_t SLPMDSEL   : 1;            /*!< [1..1] Sleep Mode Selection                                               */
      volatile uint32_t ESPIWKEN   : 1;            /*!< [2..2] ESPI Wake-up Enable                                                */
      volatile uint32_t PS2WKEN    : 1;            /*!< [3..3] PS2 Wake-up Enable                                                 */
      volatile uint32_t I2CWKEN    : 1;            /*!< [4..4] I2C Wake-up Enable                                                 */
      volatile uint32_t GPIOWKEN   : 1;            /*!< [5..5] GPIO Wake-up Enable                                                */
      volatile uint32_t _SPISWKEN  : 1;            /*!< [6..6] SPIS Wake-up Enable                                                */
            uint32_t            : 2;
      volatile uint32_t SYSLOCKBIT : 1;            /*!< [9..9] Debug lock bit                                                     */
            uint32_t            : 22;
    } SLPCTRL_b;
  } ;
  volatile const uint32_t  RESERVED1;
  
  union {
    volatile uint32_t VIVOCTRL;                    /*!< (@ 0x0000010C) VINVOUT CONTROL REGISTER                                   */
    
    struct {
      volatile uint32_t VIN0MD     : 1;            /*!< [0..0] VIN0 Mode Selection                                                */
      volatile uint32_t VIN1MD     : 1;            /*!< [1..1] VIN1 Mode Selection                                                */
      volatile uint32_t VIN2MD     : 1;            /*!< [2..2] VIN2 Mode Selection                                                */
      volatile uint32_t VIN3MD     : 1;            /*!< [3..3] VIN3 Mode Selection                                                */
      volatile uint32_t VIN4MD     : 1;            /*!< [4..4] VIN4 Mode Selection                                                */
      volatile uint32_t VIN5MD     : 1;            /*!< [5..5] VI5 Mode Selection                                                 */
      volatile const uint32_t VIN0STS    : 1;            /*!< [6..6] VIN0 Status                                                        */
      volatile const uint32_t VIN1STS    : 1;            /*!< [7..7] VIN1 Status                                                        */
      volatile const uint32_t VIN2STS    : 1;            /*!< [8..8] VIN2 Status                                                        */
      volatile const uint32_t VIN3STS    : 1;            /*!< [9..9] VIN3 Status                                                        */
      volatile const uint32_t VIN4STS    : 1;            /*!< [10..10] VIN4 Status                                                      */
      volatile const uint32_t VIN5STS    : 1;            /*!< [11..11] VIN5 Status                                                      */
      volatile uint32_t VIN0POL    : 1;            /*!< [12..12] VIN0 Polarity                                                    */
      volatile uint32_t VIN1POL    : 1;            /*!< [13..13] VIN1 Polarity                                                    */
      volatile uint32_t VIN2POL    : 1;            /*!< [14..14] VIN2 Polarity                                                    */
      volatile uint32_t VIN3POL    : 1;            /*!< [15..15] VIN3 Polarity                                                    */
      volatile uint32_t VODEVALUE  : 1;            /*!< [16..16] VOUT default value                                               */
      volatile uint32_t VIN5POL    : 1;            /*!< [17..17] VIN5 Polarity                                                    */
      volatile uint32_t VOUTDATA   : 1;            /*!< [18..18] Slect VOUT data                                                  */
      volatile uint32_t LEDBLINKEN : 1;            /*!< [19..19] Enable H/W led blink after select Vin log                        */
      volatile uint32_t SELLEDPAD  : 4;            /*!< [23..20] Select LED pad out force blinking                                */
      volatile uint32_t SELLEDPAD_IN : 3;          /*!< [26..24] Select Vin source for hardware LED blinking                      */
      volatile uint32_t VIVOLOG    : 2;            /*!< [28..27] VIN VOUT LOG                                                     */
      volatile const uint32_t BLINKMODE  : 2;            /*!< [30..29] Select led blink mode                                            */
      volatile uint32_t VOUTMD     : 1;            /*!< [31..31] VOUT Mode Selection                                              */
    } VIVOCTRL_b;
  } ;
  
  union {
    volatile const uint32_t VIVOMON;                     /*!< (@ 0x00000110) VIN VOUT MONITOR REGISTER                                  */
    
    struct {
      volatile const uint32_t VIN0MD     : 1;            /*!< [0..0] VIN0 Mode Monitor                                                  */
      volatile const uint32_t VIN1MD     : 1;            /*!< [1..1] VIN1 Mode Monitor                                                  */
      volatile const uint32_t VIN2MD     : 1;            /*!< [2..2] VIN2 Mode Monitor                                                  */
      volatile const uint32_t VIN3MD     : 1;            /*!< [3..3] VIN3 Mode Monitor                                                  */
      volatile const uint32_t VIN4MD     : 1;            /*!< [4..4] VIN4 Mode Monitor                                                  */
      volatile const uint32_t VIN5MD     : 1;            /*!< [5..5] VIN5 Mode Monitor                                                  */
      volatile const uint32_t VIN0STS    : 1;            /*!< [6..6] VIN0 Status Monitor                                                */
      volatile const uint32_t VIN1STS    : 1;            /*!< [7..7] VIN1 Status Monitor                                                */
      volatile const uint32_t VIN2STS    : 1;            /*!< [8..8] VIN2 Status Monitor                                                */
      volatile const uint32_t VIN3STS    : 1;            /*!< [9..9] VIN3 Status Monitor                                                */
      volatile const uint32_t VIN4STS    : 1;            /*!< [10..10] VIN4 Status Monitor                                              */
      volatile const uint32_t VIN5STS    : 1;            /*!< [11..11] VIN5 Status Monitor                                              */
      volatile const uint32_t VIN0POL    : 1;            /*!< [12..12] VIN0 Polarity Monitor                                            */
      volatile const uint32_t VIN1POL    : 1;            /*!< [13..13] VIN1 Polarity Monitor                                            */
      volatile const uint32_t VIN2POL    : 1;            /*!< [14..14] VIN2 Polarity Monitor                                            */
      volatile const uint32_t VIN3POL    : 1;            /*!< [15..15] VIN3 Polarity Monitor                                            */
      volatile const uint32_t VODEVALUE  : 1;            /*!< [16..16] VOUT Default Value                                               */
      volatile const uint32_t VIN5POL    : 1;            /*!< [17..17] VIN5 Polarity Monitor                                            */
      volatile const uint32_t VOUTDATA   : 1;            /*!< [18..18] Select VOUT Data Monitor                                         */
      volatile const uint32_t BLINKMODE  : 2;            /*!< [20..19] Led Blink Mode Status                                            */
            uint32_t            : 5;
      volatile const uint32_t ENVIWR     : 1;            /*!< [26..26] Enable VIN Register Write Monitor                                */
      volatile const uint32_t VIVOLOG    : 2;            /*!< [28..27] VIN VOUT LOG Monitor                                             */
      volatile const uint32_t VBATTOP    : 1;            /*!< [29..29] VBAT top status out data                                         */
      volatile const uint32_t HIT1HZ     : 1;            /*!< [30..30] 1 Hz _TMR Status                                                 */
      volatile const uint32_t VOUTMD     : 1;            /*!< [31..31] VOUT MODE SELECTION MONITOR                                      */
    } VIVOMON_b;
  } ;
  
  union {
    volatile uint32_t LDO2WR;                      /*!< (@ 0x00000114) LDO2 WRITE REGISTER                                        */
    
    struct {
      volatile uint32_t VIWR       : 1;            /*!< [0..0] Write VIN Register Enable                                          */
      volatile uint32_t VIVOLOGWR  : 1;            /*!< [1..1] Write VIN VOUT LOG Register Enable                                 */
      volatile uint32_t VOUTWR     : 1;            /*!< [2..2] Write VOUT Register Enable                                         */
      volatile uint32_t LDO2WREN   : 1;            /*!< [3..3] Write LDO2 Register Enable                                         */
      volatile uint32_t _RC32KWR   : 1;            /*!< [4..4] Write _RC32K Register Enable                                       */
            uint32_t            : 27;
    } LDO2WR_b;
  } ;
  
  union {
    volatile uint32_t LDOCTRL;                     /*!< (@ 0x00000118) LDO CONTROL REGISTER                                       */
    
    struct {
            uint32_t            : 3;
      volatile uint32_t LDO2PWREN  : 1;            /*!< [3..3] LDO2 Power Enable                                                  */
            uint32_t            : 3;
      volatile uint32_t LDO3PWREN  : 1;            /*!< [7..7] LDO3 Power Enable                                                  */
            uint32_t            : 21;
      volatile uint32_t LDO0CFG    : 1;            /*!< [29..29] LDO0 configuration select                                        */
      volatile uint32_t LDO2CFG    : 1;            /*!< [30..30] LDO2 configuration select                                        */
      volatile uint32_t LDO3CFG    : 1;            /*!< [31..31] LDO3 configuration select                                        */
    } LDOCTRL_b;
  } ;
  volatile const uint32_t  RESERVED2;
	volatile uint32_t PLLCTRL;                     /*!< (@ 0x00000120) PLL CONTROL REGISTER                                       */

  volatile const uint32_t  RESERVED3;
  
  union {
    volatile uint32_t RC32KCTRL;                   /*!< (@ 0x00000128) RC32K CONTROL REGISTER                                     */
    
    struct {
      volatile uint32_t PWREN      : 1;            /*!< [0..0] RC32K Power Enable                                                 */
      volatile uint32_t CAL        : 6;            /*!< [6..1] RC32K Calibration Coefficient                                      */
            uint32_t            : 23;
      volatile uint32_t REGWR_32K  : 1;            /*!< [30..30] RC32K Register Write Enable                                      */
            uint32_t            : 1;
    } RC32KCTRL_b;
  } ;
  volatile const uint32_t  RESERVED9[2];
  
  union {
    volatile uint32_t IXCINPUTSEL;                 /*!< (@ 0x00000134) IxC Input Signal Select REGISTER                           */
    
    struct {
      volatile uint32_t _I2C1_SCL_SEL : 1;         /*!< [0..0] SELECT _I2C01 SCL _GPIO                                            */
      volatile uint32_t _I2C1_SDA_SEL : 1;         /*!< [1..1] SELECT _I2C01 SDA _GPIO                                            */
      volatile uint32_t _I2C3_SCL_SEL : 2;         /*!< [3..2] SELECT _I2C03 SCL _GPIO                                            */
      volatile uint32_t _I2C3_SDA_SEL : 1;         /*!< [4..4] SELECT _I2C03 SDA _GPIO                                            */
      volatile uint32_t _I2C4_SCL_SEL : 1;         /*!< [5..5] SELECT _I2C04 SCL _GPIO                                            */
      volatile uint32_t _I2C4_SDA_SEL : 1;         /*!< [6..6] SELECT _I2C04 SDA _GPIO                                            */
      volatile uint32_t _I2C6_SCL_SEL : 1;         /*!< [7..7] SELECT _I2C06 SCL _GPIO                                            */
      volatile uint32_t _I2C6_SDA_SEL : 1;         /*!< [8..8] SELECT _I2C06 SDA _GPIO                                            */
      volatile uint32_t _I2C8_SCL_SEL : 1;         /*!< [9..9] SELECT _I2C08 SCL _GPIO                                            */
      volatile uint32_t _I2C8_SDA_SEL : 1;         /*!< [10..10] SELECT _I2C08 SDA _GPIO                                          */
      volatile uint32_t _I2C9_SCL_SEL : 1;         /*!< [11..11] SELECT _I2C09 SCL _GPIO                                          */
      volatile uint32_t _I2C9_SDA_SEL : 1;         /*!< [12..12] SELECT _I2C09 SDA _GPIO                                          */
      volatile uint32_t _I2C10_SCL_SEL : 1;        /*!< [13..13] SELECT _I2C10 SCL _GPIO                                          */
      volatile uint32_t _I2C10_SDA_SEL : 1;        /*!< [14..14] SELECT _I2C10 SDA _GPIO                                          */
      volatile uint32_t _I2C11_SCL_SEL : 1;        /*!< [15..15] SELECT _I2C11 SCL _GPIO                                          */
      volatile uint32_t _I2C11_SDA_SEL : 1;        /*!< [16..16] SELECT _I2C11 SDA _GPIO                                          */
      volatile uint32_t _I3C04_SCL_SEL : 1;        /*!< [17..17] SELECT _I3C04 SCL _GPIO                                          */
      volatile uint32_t _I3C04_SDA_SEL : 1;        /*!< [18..18] SELECT _I3C04 SDA _GPIO                                          */
            uint32_t            : 13;
    } IXCINPUTSEL_b;
  } ;
  
  union {
    volatile uint32_t _UART__LPT_INPUT_SEL;        /*!< (@ 0x00000138) _UART _LPT Input Signal Select REGISTER                    */
    
    struct {
      volatile uint32_t _LPT_ACK_SEL : 1;          /*!< [0..0] SELECT _LPT ACK _GPIO                                              */
      volatile uint32_t _LPT__BUSY_SEL : 1;        /*!< [1..1] SELECT _LPT _BUSY _GPIO                                            */
      volatile uint32_t _LPT_D0_SEL : 1;           /*!< [2..2] SELECT _LPT_D0 _GPIO                                               */
      volatile uint32_t _LPT_D1_SEL : 1;           /*!< [3..3] SELECT _LPT_D1 _GPIO                                               */
      volatile uint32_t _LPT_D2_SEL : 1;           /*!< [4..4] SELECT _LPT_D2 _GPIO                                               */
      volatile uint32_t _LPT_D3_SEL : 1;           /*!< [5..5] SELECT _LPT_D3 _GPIO                                               */
      volatile uint32_t _LPT_D4_SEL : 1;           /*!< [6..6] SELECT _LPT_D4 _GPIO                                               */
      volatile uint32_t _LPT_D5_SEL : 1;           /*!< [7..7] SELECT _LPT_D5 _GPIO                                               */
      volatile uint32_t _LPT_D6_SEL : 1;           /*!< [8..8] SELECT _LPT_D6 _GPIO                                               */
      volatile uint32_t _LPT_D7_SEL : 1;           /*!< [9..9] SELECT _LPT_D7 _GPIO                                               */
      volatile uint32_t _LPT_ERR_SEL : 1;          /*!< [10..10] SELECT _LPT_ERR _GPIO                                            */
      volatile uint32_t _LPT_PE_SEL : 1;           /*!< [11..11] SELECT _LPT_PE _GPIO                                             */
      volatile uint32_t _LPT_SLCT_SEL : 1;         /*!< [12..12] SELECT _LPT_SLCT _GPIO                                           */
      volatile uint32_t _UART01_RX_SEL : 1;        /*!< [13..13] SELECT _UART01 RX _GPIO                                          */
      volatile uint32_t _UART01_DCD_SEL : 2;       /*!< [15..14] SELECT _UART01 DCD _GPIO                                         */
      volatile uint32_t _UART01_CTS_SEL : 2;       /*!< [17..16] SELECT _UART01 CTS _GPIO                                         */
      volatile uint32_t _UART01_DSR_SEL : 1;       /*!< [18..18] SELECT _UART01 DSR _GPIO                                         */
      volatile uint32_t _UART01_RI_SEL : 2;        /*!< [20..19] SELECT _UART01 RI _GPIO                                          */
      volatile uint32_t _UART02_CTS_SEL : 2;       /*!< [22..21] SELECT _UART02 CTS _GPIO                                         */
      volatile uint32_t _UART02_DCD_SEL : 2;       /*!< [24..23] SELECT _UART02 DCD _GPIO                                         */
      volatile uint32_t _UART02_DSR_SEL : 2;       /*!< [26..25] SELECT _UART02 DSR _GPIO                                         */
      volatile uint32_t _UART02_RI_SEL : 1;        /*!< [27..27] SELECT _UART02 RI _GPIO                                          */
      volatile uint32_t _UART02_RX_SEL : 1;        /*!< [28..28] SELECT _UART02 RX _GPIO                                          */
            uint32_t            : 3;
    } _UART__LPT_INPUT_SEL_b;
  } ;
  
  union {
    volatile uint32_t IP_INPUT_SEL;                /*!< (@ 0x0000013C) IP Input Signal Select REGISTER                            */
    
    struct {
      volatile uint32_t _TACH01_SEL : 2;           /*!< [1..0] SELECT _TACH01 _GPIO                                               */
      volatile uint32_t _TACH02_SEL : 2;           /*!< [3..2] SELECT _TACH02 _GPIO                                               */
      volatile uint32_t _TACH03_SEL : 2;           /*!< [5..4] SELECT _TACH03 _GPIO                                               */
      volatile uint32_t _TACH04_SEL : 2;           /*!< [7..6] SELECT _TACH04 _GPIO                                               */
      volatile uint32_t _TACH05_SEL : 2;           /*!< [9..8] SELECT _TACH05 _GPIO                                               */
      volatile uint32_t _TACH06_SEL : 2;           /*!< [11..10] SELECT _TACH06 _GPIO                                             */
      volatile uint32_t _TACH07_SEL : 1;           /*!< [12..12] SELECT _TACH07 _GPIO                                             */
      volatile uint32_t _TACH08_SEL : 1;           /*!< [13..13] SELECT _TACH08 _GPIO                                             */
            uint32_t            : 4;
      volatile uint32_t _CEC1_SEL  : 1;            /*!< [18..18] SELECT _CEC1 _GPIO                                               */
      volatile uint32_t _CEC2_SEL  : 1;            /*!< [19..19] SELECT _CEC2 _GPIO                                               */
      volatile uint32_t ESPI_RST_SEL : 1;          /*!< [20..20] SELECT ESPI_RST _GPIO                                            */
      volatile uint32_t LPC_RST_SEL : 1;           /*!< [21..21] SELECT LPC_RST _GPIO                                             */
      volatile uint32_t KSI6_SEL   : 1;            /*!< [22..22] SELECT KSI6 _GPIO                                                */
      volatile uint32_t KSI7_SEL   : 1;            /*!< [23..23] SELECT KSI7 _GPIO                                                */
      volatile uint32_t _LEDSTP_SEL : 1;           /*!< [24..24] SELECT _LEDSTP _GPIO                                             */
      volatile uint32_t _PS2_CLK1_SEL : 2;         /*!< [26..25] SELECT PS2 CLK01 _GPIO                                           */
      volatile uint32_t _PS2_DATA1_SEL : 1;        /*!< [27..27] SELECT PS2 DATA1 _GPIO                                           */
      volatile uint32_t _PS2_CLK2_SEL : 1;         /*!< [28..28] SELECT PS2 CLK2 _GPIO                                            */
      volatile uint32_t _PS2_DATA2_SEL : 1;        /*!< [29..29] SELECT PS2 DATA2 _GPIO                                           */
      volatile uint32_t _PWRBTN_SEL : 2;           /*!< [31..30] SELECT _PWRBTN _GPIO                                             */
    } IP_INPUT_SEL_b;
  } ;
  
  union {
    volatile uint32_t SPIC_INPUT_SEL;              /*!< (@ 0x00000140) SPIC Input Signal Select REGISTER                          */
    
    struct {
      volatile uint32_t _SPIC01_IO0_SEL : 2;       /*!< [1..0] SELECT SPIC01 IO0 GPIO                                             */
      volatile uint32_t _SPIC01_IO1_SEL : 2;       /*!< [3..2] SELECT SPIC01 IO1 GPIO                                             */
      volatile uint32_t _SPIC01_IO2_SEL : 2;       /*!< [5..4] SELECT SPIC01 IO2 GPIO                                             */
      volatile uint32_t _SPIC01_IO3_SEL : 2;       /*!< [7..6] SELECT SPIC01 IO3 GPIO                                             */
            uint32_t            : 24;
    } SPIC_INPUT_SEL_b;
  } ;
  volatile const uint32_t  RESERVED4[11];
  
  union {
    volatile uint32_t SYSPERCTRL;                  /*!< (@ 0x00000170) System Peripheral Control REGISTER                         */
    
    struct {
      volatile uint32_t PLLBYPASS  : 1;            /*!< [0..0] PLL Bypass                                                         */
      volatile uint32_t DIOEN      : 1;            /*!< [1..1] DIO Enable                                                         */
            uint32_t            : 30;
    } SYSPERCTRL_b;
  } ;
  volatile const uint32_t  RESERVED5[237];
  
  union {
    volatile uint32_t WUE;                         /*!< (@ 0x00000528) WAKE UP ENABLE REGISTER                                    */
    
    struct {
      volatile uint32_t _ESPIRST   : 1;            /*!< [0..0] ESPI RESET WAKE UP ENABLE                                          */
      volatile uint32_t _ESPI      : 1;            /*!< [1..1] ESPI WAKE UP ENABLE                                                */
      volatile uint32_t _PECI      : 1;            /*!< [2..2] PECI WAKE UP ENABLE                                                */
            uint32_t            : 29;
    } WUE_b;
  } ;
  volatile const uint32_t  RESERVED6[181];
  volatile uint32_t SPI_FLASH_IO0;
  volatile uint32_t SPI_FLASH_IO1;
  volatile uint32_t SPI_FLASH_IO2;  
  volatile uint32_t SPI_FLASH_IO3A;
  volatile uint32_t SPI_FLASH_CSN;
  volatile uint32_t SPI_FLASH_SCLK; 
  volatile uint32_t PECI_PAD; 
  volatile uint32_t VREF_VTT_PAD; 
  volatile uint32_t ESPI_CLK_PAD;
  volatile uint32_t ESPI_IO0_PAD;
  volatile uint32_t ESPI_IO1_PAD;
  volatile uint32_t ESPI_IO2_PAD;
  volatile uint32_t ESPI_IO3_PAD;
  volatile const uint32_t  RESERVED7[52];
  union {
    volatile uint32_t _32KSRC;                     /*!< (@ 0x00000904) 32K CLK SOURCE REGISTER                                    */
    
    struct {
            uint32_t            : 31;
      volatile uint32_t S_32KSRC   : 1;            /*!< [31..31] 32K Clock Source                                                 */
    } _32KSRC_b;
  } ;
  volatile const uint32_t  RESERVED8[62];
  
  union {
    volatile uint32_t VBATIPCLK;                   /*!< (@ 0x00000A00) VBAT IP CLK REGISTER                                       */
    
    struct {
      volatile uint32_t _RTCPWR    : 1;            /*!< [0..0] _RTC IP Clock Power                                                */
      volatile uint32_t _PWRBTN1PWR : 1;           /*!< [1..1] _PWRBTN1 IP Clock Power                                            */
      volatile uint32_t _PWRBTN0PWR : 1;           /*!< [2..2] _PWRBTN0 IP Clock Power                                            */
      volatile uint32_t _WDTPWR    : 1;            /*!< [3..3] _WDT Clock Power                                                   */
      volatile uint32_t _RC32KPWR  : 1;            /*!< [4..4] _RC32K Power                                                       */
      volatile uint32_t _PWRBTN1RST : 1;           /*!< [5..5] _PWRBTN1 IP Reset                                                  */
      volatile uint32_t _PWRBTN0RST : 1;           /*!< [6..6] _PWRBTN0 IP Reset                                                  */
      volatile uint32_t _WDTRST    : 1;            /*!< [7..7] _WDT IP Reset                                                      */
      volatile uint32_t _RTCRST    : 1;            /*!< [8..8] _RTC IP Reset                                                      */
      volatile uint32_t _RC32KRST  : 1;            /*!< [9..9] _RC32K IP Reset                                                    */
      volatile uint32_t _PWRBTN1GAT : 1;           /*!< [10..10] _PWRBTN1 IP Clock Gating                                         */
      volatile uint32_t _PWRBTN0GAT : 1;           /*!< [11..11] _PWRBTN0 IP Clock Gating                                         */
      volatile uint32_t _WDTGAT    : 1;            /*!< [12..12] _WDT IP Clock Gating                                             */
      volatile uint32_t _RTCGAT    : 1;            /*!< [13..13] _RTC IP Clock Gating                                             */
      volatile uint32_t _RC32KGAT  : 1;            /*!< [14..14] _RC32K IP Clock Gating                                           */
            uint32_t            : 2;
      volatile uint32_t S_RC32K    : 2;            /*!< [18..17] select 32K clock source                                          */
      volatile uint32_t _RC32KCAL  : 1;            /*!< [19..19] _RC32K Calibration Power                                         */
      volatile uint32_t _RC32K     : 1;            /*!< [20..20] _RC32K IP Clock Power                                            */
            uint32_t            : 11;
    } VBATIPCLK_b;
  } ;
} SYSTEM_Type;                                  /*!< Size = 2564 (0xa04)                                                       */

#define SYSTEM_SYSCLK__USB_Pos                          (2UL)                   
#define SYSTEM_SYSCLK__USB_Msk                          BIT(SYSTEM_SYSCLK__USB_Pos)                 
#define SYSTEM_SYSCLK__LC_Pos                           (3UL)                   
#define SYSTEM_SYSCLK__LC_Msk                           BIT(SYSTEM_SYSCLK__LC_Pos)                 
#define SYSTEM_SYSCLK__XTAL32_Pos                       (4UL)                   
#define SYSTEM_SYSCLK__XTAL32_Msk                       BIT(SYSTEM_SYSCLK__XTAL32_Pos)                
#define SYSTEM_SYSCLK__RC32KREF_Pos                     (5UL)                   
#define SYSTEM_SYSCLK__RC32KREF_Msk                     BIT(SYSTEM_SYSCLK__RC32KREF_Pos)                
#define SYSTEM_SYSCLK__PECI_Pos                         (6UL)                   
#define SYSTEM_SYSCLK__PECI_Msk                         BIT(SYSTEM_SYSCLK__PECI_Pos)                
#define SYSTEM_SYSCLK__PLL_Pos                          (7UL)                   
#define SYSTEM_SYSCLK__PLL_Msk                          BIT(SYSTEM_SYSCLK__PLL_Pos)                
#define SYSTEM_SYSCLK__RC25MREF_Pos                     (8UL)                   
#define SYSTEM_SYSCLK__RC25MREF_Msk                     BIT(SYSTEM_SYSCLK__RC25MREF_Pos)               
#define SYSTEM_SYSCLK__RC25MCAL_Pos                     (9UL)                   
#define SYSTEM_SYSCLK__RC25MCAL_Msk                     BIT(SYSTEM_SYSCLK__RC25MCAL_Pos)               
#define SYSTEM_SYSCLK__RC25M_Pos                        (10UL)                  
#define SYSTEM_SYSCLK__RC25M_Msk                        BIT(SYSTEM_SYSCLK__RC25M_Pos)               
#define SYSTEM_SYSCLK__LED_PWM_Pos                      (11UL)                  
#define SYSTEM_SYSCLK__LED_PWM_Msk                      BIT(SYSTEM_SYSCLK__LED_PWM_Pos)               
#define SYSTEM_SYSCLK__I2C_Pos                          (12UL)                  
#define SYSTEM_SYSCLK__I2C_Msk                          BIT(SYSTEM_SYSCLK__I2C_Pos)              
#define SYSTEM_SYSCLK__LOWCLK_Pos                       (13UL)                  
#define SYSTEM_SYSCLK__LOWCLK_Msk                       BIT(SYSTEM_SYSCLK__LOWCLK_Pos)              
#define SYSTEM_SYSCLK__ESPI_Pos                         (14UL)                  
#define SYSTEM_SYSCLK__ESPI_Msk                         BIT(SYSTEM_SYSCLK__ESPI_Pos)              
#define SYSTEM_SYSCLK__APB_Pos                          (15UL)                  
#define SYSTEM_SYSCLK__APB_Msk                          BIT(SYSTEM_SYSCLK__APB_Pos)              
#define SYSTEM_SYSCLKSEL_S_LALU_Pos                     (4UL)                   
#define SYSTEM_SYSCLKSEL_S_LALU_Msk                     GENMASK(5, 4)                
#define SYSTEM_SYSCLKSEL_S_RC32KPAD_Pos                 (6UL)                   
#define SYSTEM_SYSCLKSEL_S_RC32KPAD_Msk                 GENMASK(7, 6)                
#define SYSTEM_SYSCLKSEL__LED_PWM_Pos                   (20UL)                  
#define SYSTEM_SYSCLKSEL__LED_PWM_Msk                   GENMASK(21, 20)            
#define SYSTEM_SYSCLKSEL__I2C_Pos                       (22UL)                  
#define SYSTEM_SYSCLKSEL__I2C_Msk                       GENMASK(23, 22)            
#define SYSTEM_SYSCLKSEL_LOWCLK_Pos                     (24UL)                  
#define SYSTEM_SYSCLKSEL_LOWCLK_Msk                     GENMASK(25, 24)           
#define SYSTEM_SYSCLKSEL__ESPI_Pos                      (26UL)                  
#define SYSTEM_SYSCLKSEL__ESPI_Msk                      GENMASK(27, 26)           
#define SYSTEM_SYSCLKSEL_APB_Pos                        (28UL)                  
#define SYSTEM_SYSCLKSEL_APB_Msk                        GENMASK(29, 28)          
#define SYSTEM_SYSCLKSEL__SYSTEM_Pos                    (30UL)                  
#define SYSTEM_SYSCLKSEL__SYSTEM_Msk                    GENMASK(31, 30)          
#define SYSTEM_IPCLK0__PWM8_Pos                         (0UL)                   
#define SYSTEM_IPCLK0__PWM8_Msk                         BIT(SYSTEM_IPCLK0__PWM8_Pos)                 
#define SYSTEM_IPCLK0__PWM7_Pos                         (1UL)                   
#define SYSTEM_IPCLK0__PWM7_Msk                         BIT(SYSTEM_IPCLK0__PWM7_Pos)                 
#define SYSTEM_IPCLK0__PWM6_Pos                         (2UL)                   
#define SYSTEM_IPCLK0__PWM6_Msk                         BIT(SYSTEM_IPCLK0__PWM6_Pos)                 
#define SYSTEM_IPCLK0__PWM5_Pos                         (3UL)                   
#define SYSTEM_IPCLK0__PWM5_Msk                         BIT(SYSTEM_IPCLK0__PWM5_Pos)                 
#define SYSTEM_IPCLK0__PWM4_Pos                         (4UL)                   
#define SYSTEM_IPCLK0__PWM4_Msk                         BIT(SYSTEM_IPCLK0__PWM4_Pos)                
#define SYSTEM_IPCLK0__PWM3_Pos                         (5UL)                   
#define SYSTEM_IPCLK0__PWM3_Msk                         BIT(SYSTEM_IPCLK0__PWM3_Pos)                
#define SYSTEM_IPCLK0__PWM2_Pos                         (6UL)                   
#define SYSTEM_IPCLK0__PWM2_Msk                         BIT(SYSTEM_IPCLK0__PWM2_Pos)                
#define SYSTEM_IPCLK0__PWM1_Pos                         (7UL)                   
#define SYSTEM_IPCLK0__PWM1_Msk                         BIT(SYSTEM_IPCLK0__PWM1_Pos)                
#define SYSTEM_IPCLK0__LED4_Pos                         (8UL)                   
#define SYSTEM_IPCLK0__LED4_Msk                         BIT(SYSTEM_IPCLK0__LED4_Pos)               
#define SYSTEM_IPCLK0__LED3_Pos                         (9UL)                   
#define SYSTEM_IPCLK0__LED3_Msk                         BIT(SYSTEM_IPCLK0__LED3_Pos)               
#define SYSTEM_IPCLK0__LED2_Pos                         (10UL)                  
#define SYSTEM_IPCLK0__LED2_Msk                         BIT(SYSTEM_IPCLK0__LED2_Pos)               
#define SYSTEM_IPCLK0__LED1_Pos                         (11UL)                  
#define SYSTEM_IPCLK0__LED1_Msk                         BIT(SYSTEM_IPCLK0__LED1_Pos)               
#define SYSTEM_IPCLK0__EMI7_Pos                         (12UL)                  
#define SYSTEM_IPCLK0__EMI7_Msk                         BIT(SYSTEM_IPCLK0__EMI7_Pos)              
#define SYSTEM_IPCLK0__EMI6_Pos                         (13UL)                  
#define SYSTEM_IPCLK0__EMI6_Msk                         BIT(SYSTEM_IPCLK0__EMI6_Pos)              
#define SYSTEM_IPCLK0__EMI5_Pos                         (14UL)                  
#define SYSTEM_IPCLK0__EMI5_Msk                         BIT(SYSTEM_IPCLK0__EMI5_Pos)              
#define SYSTEM_IPCLK0__EMI4_Pos                         (15UL)                  
#define SYSTEM_IPCLK0__EMI4_Msk                         BIT(SYSTEM_IPCLK0__EMI4_Pos)              
#define SYSTEM_IPCLK0__EMI3_Pos                         (16UL)                  
#define SYSTEM_IPCLK0__EMI3_Msk                         BIT(SYSTEM_IPCLK0__EMI3_Pos)             
#define SYSTEM_IPCLK0__EMI2_Pos                         (17UL)                  
#define SYSTEM_IPCLK0__EMI2_Msk                         BIT(SYSTEM_IPCLK0__EMI2_Pos)             
#define SYSTEM_IPCLK0__EMI1_Pos                         (18UL)                  
#define SYSTEM_IPCLK0__EMI1_Msk                         BIT(SYSTEM_IPCLK0__EMI1_Pos)             
#define SYSTEM_IPCLK0__EMI0_Pos                         (19UL)                  
#define SYSTEM_IPCLK0__EMI0_Msk                         BIT(SYSTEM_IPCLK0__EMI0_Pos)             
#define SYSTEM_IPCLK0__PORT80_Pos                       (20UL)                  
#define SYSTEM_IPCLK0__PORT80_Msk                       BIT(SYSTEM_IPCLK0__PORT80_Pos)            
#define SYSTEM_IPCLK0__PMPORT3_Pos                      (21UL)                  
#define SYSTEM_IPCLK0__PMPORT3_Msk                      BIT(SYSTEM_IPCLK0__PMPORT3_Pos)            
#define SYSTEM_IPCLK0__PMPORT2_Pos                      (22UL)                  
#define SYSTEM_IPCLK0__PMPORT2_Msk                      BIT(SYSTEM_IPCLK0__PMPORT2_Pos)            
#define SYSTEM_IPCLK0__PMPORT1_Pos                      (23UL)                  
#define SYSTEM_IPCLK0__PMPORT1_Msk                      BIT(SYSTEM_IPCLK0__PMPORT1_Pos)            
#define SYSTEM_IPCLK0__PMPORT0_Pos                      (24UL)                  
#define SYSTEM_IPCLK0__PMPORT0_Msk                      BIT(SYSTEM_IPCLK0__PMPORT0_Pos)           
#define SYSTEM_IPCLK0__ACPI_Pos                         (25UL)                  
#define SYSTEM_IPCLK0__ACPI_Msk                         BIT(SYSTEM_IPCLK0__ACPI_Pos)           
#define SYSTEM_IPCLK0__KBC_Pos                          (26UL)                  
#define SYSTEM_IPCLK0__KBC_Msk                          BIT(SYSTEM_IPCLK0__KBC_Pos)           
#define SYSTEM_IPCLK0__ESPI_Pos                         (27UL)                  
#define SYSTEM_IPCLK0__ESPI_Msk                         BIT(SYSTEM_IPCLK0__ESPI_Pos)           
#define SYSTEM_IPCLK0__DMA_Pos                          (28UL)                  
#define SYSTEM_IPCLK0__DMA_Msk                          BIT(SYSTEM_IPCLK0__DMA_Pos)          
#define SYSTEM_IPCLK0__OTP_Pos                          (29UL)                  
#define SYSTEM_IPCLK0__OTP_Msk                          BIT(SYSTEM_IPCLK0__OTP_Pos)          
#define SYSTEM_IPCLK0_KR4_Pos                           (30UL)                  
#define SYSTEM_IPCLK0_KR4_Msk                           BIT(SYSTEM_IPCLK0_KR4_Pos)          
#define SYSTEM_IPCLK0__BUS_Pos                          (31UL)                  
#define SYSTEM_IPCLK0__BUS_Msk                          BIT(SYSTEM_IPCLK0__BUS_Pos)          
#define SYSTEM_IPCLK1__TACH5_Pos                        (0UL)                   
#define SYSTEM_IPCLK1__TACH5_Msk                        BIT(SYSTEM_IPCLK1__TACH5_Pos)                 
#define SYSTEM_IPCLK1__TACH4_Pos                        (1UL)                   
#define SYSTEM_IPCLK1__TACH4_Msk                        BIT(SYSTEM_IPCLK1__TACH4_Pos)                 
#define SYSTEM_IPCLK1__TACH3_Pos                        (2UL)                   
#define SYSTEM_IPCLK1__TACH3_Msk                        BIT(SYSTEM_IPCLK1__TACH3_Pos)                 
#define SYSTEM_IPCLK1__TACH2_Pos                        (3UL)                   
#define SYSTEM_IPCLK1__TACH2_Msk                        BIT(SYSTEM_IPCLK1__TACH2_Pos)                 
#define SYSTEM_IPCLK1__TACH1_Pos                        (4UL)                   
#define SYSTEM_IPCLK1__TACH1_Msk                        BIT(SYSTEM_IPCLK1__TACH1_Pos)                
#define SYSTEM_IPCLK1__PS2_H3_Pos                       (5UL)                   
#define SYSTEM_IPCLK1__PS2_H3_Msk                       BIT(SYSTEM_IPCLK1__PS2_H3_Pos)                
#define SYSTEM_IPCLK1__PS2_H2_Pos                       (6UL)                   
#define SYSTEM_IPCLK1__PS2_H2_Msk                       BIT(SYSTEM_IPCLK1__PS2_H2_Pos)                
#define SYSTEM_IPCLK1__PS2_H1_Pos                       (7UL)                   
#define SYSTEM_IPCLK1__PS2_H1_Msk                       BIT(SYSTEM_IPCLK1__PS2_H1_Pos)                
#define SYSTEM_IPCLK1__I2C11_Pos                        (11UL)                  
#define SYSTEM_IPCLK1__I2C11_Msk                        BIT(SYSTEM_IPCLK1__I2C11_Pos)               
#define SYSTEM_IPCLK1__I2C10_Pos                        (12UL)                  
#define SYSTEM_IPCLK1__I2C10_Msk                        BIT(SYSTEM_IPCLK1__I2C10_Pos)              
#define SYSTEM_IPCLK1__I2C9_Pos                         (13UL)                  
#define SYSTEM_IPCLK1__I2C9_Msk                         BIT(SYSTEM_IPCLK1__I2C9_Pos)              
#define SYSTEM_IPCLK1__I2C8_Pos                         (14UL)                  
#define SYSTEM_IPCLK1__I2C8_Msk                         BIT(SYSTEM_IPCLK1__I2C8_Pos)              
#define SYSTEM_IPCLK1__I2C7_Pos                         (15UL)                  
#define SYSTEM_IPCLK1__I2C7_Msk                         BIT(SYSTEM_IPCLK1__I2C7_Pos)              
#define SYSTEM_IPCLK1__I2C6_Pos                         (16UL)                  
#define SYSTEM_IPCLK1__I2C6_Msk                         BIT(SYSTEM_IPCLK1__I2C6_Pos)             
#define SYSTEM_IPCLK1__I2C5_Pos                         (17UL)                  
#define SYSTEM_IPCLK1__I2C5_Msk                         BIT(SYSTEM_IPCLK1__I2C5_Pos)             
#define SYSTEM_IPCLK1__I2C4_Pos                         (18UL)                  
#define SYSTEM_IPCLK1__I2C4_Msk                         BIT(SYSTEM_IPCLK1__I2C4_Pos)             
#define SYSTEM_IPCLK1__I2C3_Pos                         (19UL)                  
#define SYSTEM_IPCLK1__I2C3_Msk                         BIT(SYSTEM_IPCLK1__I2C3_Pos)             
#define SYSTEM_IPCLK1__I2C2_Pos                         (20UL)                  
#define SYSTEM_IPCLK1__I2C2_Msk                         BIT(SYSTEM_IPCLK1__I2C2_Pos)            
#define SYSTEM_IPCLK1__I2C1_Pos                         (21UL)                  
#define SYSTEM_IPCLK1__I2C1_Msk                         BIT(SYSTEM_IPCLK1__I2C1_Pos)            
#define SYSTEM_IPCLK1__UART2_Pos                        (22UL)                  
#define SYSTEM_IPCLK1__UART2_Msk                        BIT(SYSTEM_IPCLK1__UART2_Pos)            
#define SYSTEM_IPCLK1__UART1_Pos                        (23UL)                  
#define SYSTEM_IPCLK1__UART1_Msk                        BIT(SYSTEM_IPCLK1__UART1_Pos)            
#define SYSTEM_IPCLK1__UART0_Pos                        (24UL)                  
#define SYSTEM_IPCLK1__UART0_Msk                        BIT(SYSTEM_IPCLK1__UART0_Pos)           
#define SYSTEM_IPCLK1__ADC_Pos                          (25UL)                  
#define SYSTEM_IPCLK1__ADC_Msk                          BIT(SYSTEM_IPCLK1__ADC_Pos)           
#define SYSTEM_IPCLK1__PWM14_Pos                        (26UL)                  
#define SYSTEM_IPCLK1__PWM14_Msk                        BIT(SYSTEM_IPCLK1__PWM14_Pos)           
#define SYSTEM_IPCLK1__PWM13_Pos                        (27UL)                  
#define SYSTEM_IPCLK1__PWM13_Msk                        BIT(SYSTEM_IPCLK1__PWM13_Pos)           
#define SYSTEM_IPCLK1__PWM12_Pos                        (28UL)                  
#define SYSTEM_IPCLK1__PWM12_Msk                        BIT(SYSTEM_IPCLK1__PWM12_Pos)          
#define SYSTEM_IPCLK1__PWM11_Pos                        (29UL)                  
#define SYSTEM_IPCLK1__PWM11_Msk                        BIT(SYSTEM_IPCLK1__PWM11_Pos)          
#define SYSTEM_IPCLK1__PWM10_Pos                        (30UL)                  
#define SYSTEM_IPCLK1__PWM10_Msk                        BIT(SYSTEM_IPCLK1__PWM10_Pos)          
#define SYSTEM_IPCLK1__PWM9_Pos                         (31UL)                  
#define SYSTEM_IPCLK1__PWM9_Msk                         BIT(SYSTEM_IPCLK1__PWM9_Pos)          
#define SYSTEM_IPCLK2__I3C2_Pos                         (0UL)                   
#define SYSTEM_IPCLK2__I3C2_Msk                         BIT(SYSTEM_IPCLK2__I3C2_Pos)                 
#define SYSTEM_IPCLK2__I3C1_Pos                         (1UL)                   
#define SYSTEM_IPCLK2__I3C1_Msk                         BIT(SYSTEM_IPCLK2__I3C1_Pos)                 
#define SYSTEM_IPCLK2__I3C4_Pos                         (2UL)                   
#define SYSTEM_IPCLK2__I3C4_Msk                         BIT(SYSTEM_IPCLK2__I3C4_Pos)                 
#define SYSTEM_IPCLK2__LPT_Pos                          (3UL)                   
#define SYSTEM_IPCLK2__LPT_Msk                          BIT(SYSTEM_IPCLK2__LPT_Pos)                 
#define SYSTEM_IPCLK2__VINLED_Pos                       (4UL)                   
#define SYSTEM_IPCLK2__VINLED_Msk                       BIT(SYSTEM_IPCLK2__VINLED_Pos)                
#define SYSTEM_IPCLK2__I2CDBG_Pos                       (5UL)                   
#define SYSTEM_IPCLK2__I2CDBG_Msk                       BIT(SYSTEM_IPCLK2__I2CDBG_Pos)                
#define SYSTEM_IPCLK2__MICROCNT_Pos                     (6UL)                   
#define SYSTEM_IPCLK2__MICROCNT_Msk                     BIT(SYSTEM_IPCLK2__MICROCNT_Pos)                
#define SYSTEM_IPCLK2__CEC2_Pos                         (7UL)                   
#define SYSTEM_IPCLK2__CEC2_Msk                         BIT(SYSTEM_IPCLK2__CEC2_Pos)                
#define SYSTEM_IPCLK2__CEC1_Pos                         (8UL)                   
#define SYSTEM_IPCLK2__CEC1_Msk                         BIT(SYSTEM_IPCLK2__CEC1_Pos)               
#define SYSTEM_IPCLK2__LEDSTP_Pos                       (9UL)                   
#define SYSTEM_IPCLK2__LEDSTP_Msk                       BIT(SYSTEM_IPCLK2__LEDSTP_Pos)               
#define SYSTEM_IPCLK2__SLWTMR3_Pos                      (10UL)                  
#define SYSTEM_IPCLK2__SLWTMR3_Msk                      BIT(SYSTEM_IPCLK2__SLWTMR3_Pos)               
#define SYSTEM_IPCLK2__SLWTMR2_Pos                      (11UL)                  
#define SYSTEM_IPCLK2__SLWTMR2_Msk                      BIT(SYSTEM_IPCLK2__SLWTMR2_Pos)               
#define SYSTEM_IPCLK2__SLWTMR1_Pos                      (12UL)                  
#define SYSTEM_IPCLK2__SLWTMR1_Msk                      BIT(SYSTEM_IPCLK2__SLWTMR1_Pos)              
#define SYSTEM_IPCLK2__SLWTMR0_Pos                      (13UL)                  
#define SYSTEM_IPCLK2__SLWTMR0_Msk                      BIT(SYSTEM_IPCLK2__SLWTMR0_Pos)              
#define SYSTEM_IPCLK2__KBM_Pos                          (15UL)                  
#define SYSTEM_IPCLK2__KBM_Msk                          BIT(SYSTEM_IPCLK2__KBM_Pos)              
#define SYSTEM_IPCLK2__TMR5_Pos                         (16UL)                  
#define SYSTEM_IPCLK2__TMR5_Msk                         BIT(SYSTEM_IPCLK2__TMR5_Pos)             
#define SYSTEM_IPCLK2__TMR4_Pos                         (17UL)                  
#define SYSTEM_IPCLK2__TMR4_Msk                         BIT(SYSTEM_IPCLK2__TMR4_Pos)             
#define SYSTEM_IPCLK2__TMR3_Pos                         (18UL)                  
#define SYSTEM_IPCLK2__TMR3_Msk                         BIT(SYSTEM_IPCLK2__TMR3_Pos)             
#define SYSTEM_IPCLK2__TMR2_Pos                         (19UL)                  
#define SYSTEM_IPCLK2__TMR2_Msk                         BIT(SYSTEM_IPCLK2__TMR2_Pos)             
#define SYSTEM_IPCLK2__TMR1_Pos                         (20UL)                  
#define SYSTEM_IPCLK2__TMR1_Msk                         BIT(SYSTEM_IPCLK2__TMR1_Pos)            
#define SYSTEM_IPCLK2__TMR0_Pos                         (21UL)                  
#define SYSTEM_IPCLK2__TMR0_Msk                         BIT(SYSTEM_IPCLK2__TMR0_Pos)            
#define SYSTEM_IPCLK2__GPIO_Pos                         (22UL)                  
#define SYSTEM_IPCLK2__GPIO_Msk                         BIT(SYSTEM_IPCLK2__GPIO_Pos)            
#define SYSTEM_IPCLK2__TACH14_Pos                       (23UL)                  
#define SYSTEM_IPCLK2__TACH14_Msk                       BIT(SYSTEM_IPCLK2__TACH14_Pos)            
#define SYSTEM_IPCLK2__TACH13_Pos                       (24UL)                  
#define SYSTEM_IPCLK2__TACH13_Msk                       BIT(SYSTEM_IPCLK2__TACH13_Pos)           
#define SYSTEM_IPCLK2__TACH12_Pos                       (25UL)                  
#define SYSTEM_IPCLK2__TACH12_Msk                       BIT(SYSTEM_IPCLK2__TACH12_Pos)           
#define SYSTEM_IPCLK2__TACH11_Pos                       (26UL)                  
#define SYSTEM_IPCLK2__TACH11_Msk                       BIT(SYSTEM_IPCLK2__TACH11_Pos)           
#define SYSTEM_IPCLK2__TACH10_Pos                       (27UL)                  
#define SYSTEM_IPCLK2__TACH10_Msk                       BIT(SYSTEM_IPCLK2__TACH10_Pos)           
#define SYSTEM_IPCLK2__TACH9_Pos                        (28UL)                  
#define SYSTEM_IPCLK2__TACH9_Msk                        BIT(SYSTEM_IPCLK2__TACH9_Pos)          
#define SYSTEM_IPCLK2__TACH8_Pos                        (29UL)                  
#define SYSTEM_IPCLK2__TACH8_Msk                        BIT(SYSTEM_IPCLK2__TACH8_Pos)          
#define SYSTEM_IPCLK2__TACH7_Pos                        (30UL)                  
#define SYSTEM_IPCLK2__TACH7_Msk                        BIT(SYSTEM_IPCLK2__TACH7_Pos)          
#define SYSTEM_IPCLK2__TACH6_Pos                        (31UL)                  
#define SYSTEM_IPCLK2__TACH6_Msk                        BIT(SYSTEM_IPCLK2__TACH6_Pos)          
#define SYSTEM_APBCLK0__PS2_3_Pos                       (0UL)                   
#define SYSTEM_APBCLK0__PS2_3_Msk                       BIT(SYSTEM_APBCLK0__PS2_3_Pos)                 
#define SYSTEM_APBCLK0__PS2_2_Pos                       (1UL)                   
#define SYSTEM_APBCLK0__PS2_2_Msk                       BIT(SYSTEM_APBCLK0__PS2_2_Pos)                 
#define SYSTEM_APBCLK0__PS2_1_Pos                       (2UL)                   
#define SYSTEM_APBCLK0__PS2_1_Msk                       BIT(SYSTEM_APBCLK0__PS2_1_Pos)                 
#define SYSTEM_APBCLK0__LEDSTP_Pos                      (3UL)                   
#define SYSTEM_APBCLK0__LEDSTP_Msk                      BIT(SYSTEM_APBCLK0__LEDSTP_Pos)                 
#define SYSTEM_APBCLK0__LED4_Pos                        (4UL)                   
#define SYSTEM_APBCLK0__LED4_Msk                        BIT(SYSTEM_APBCLK0__LED4_Pos)                
#define SYSTEM_APBCLK0__LED3_Pos                        (5UL)                   
#define SYSTEM_APBCLK0__LED3_Msk                        BIT(SYSTEM_APBCLK0__LED3_Pos)                
#define SYSTEM_APBCLK0__LED2_Pos                        (6UL)                   
#define SYSTEM_APBCLK0__LED2_Msk                        BIT(SYSTEM_APBCLK0__LED2_Pos)                
#define SYSTEM_APBCLK0__LED1_Pos                        (7UL)                   
#define SYSTEM_APBCLK0__LED1_Msk                        BIT(SYSTEM_APBCLK0__LED1_Pos)                
#define SYSTEM_APBCLK0__KBM_Pos                         (8UL)                   
#define SYSTEM_APBCLK0__KBM_Msk                         BIT(SYSTEM_APBCLK0__KBM_Pos)               
#define SYSTEM_APBCLK0__I2CDBG_Pos                      (9UL)                   
#define SYSTEM_APBCLK0__I2CDBG_Msk                      BIT(SYSTEM_APBCLK0__I2CDBG_Pos)               
#define SYSTEM_APBCLK0__I2C11_Pos                       (10UL)                  
#define SYSTEM_APBCLK0__I2C11_Msk                       BIT(SYSTEM_APBCLK0__I2C11_Pos)               
#define SYSTEM_APBCLK0__I2C10_Pos                       (11UL)                  
#define SYSTEM_APBCLK0__I2C10_Msk                       BIT(SYSTEM_APBCLK0__I2C10_Pos)               
#define SYSTEM_APBCLK0__I2C9_Pos                        (12UL)                  
#define SYSTEM_APBCLK0__I2C9_Msk                        BIT(SYSTEM_APBCLK0__I2C9_Pos)              
#define SYSTEM_APBCLK0__I2C8_Pos                        (13UL)                  
#define SYSTEM_APBCLK0__I2C8_Msk                        BIT(SYSTEM_APBCLK0__I2C8_Pos)              
#define SYSTEM_APBCLK0__I2C7_Pos                        (14UL)                  
#define SYSTEM_APBCLK0__I2C7_Msk                        BIT(SYSTEM_APBCLK0__I2C7_Pos)              
#define SYSTEM_APBCLK0__I2C6_Pos                        (15UL)                  
#define SYSTEM_APBCLK0__I2C6_Msk                        BIT(SYSTEM_APBCLK0__I2C6_Pos)              
#define SYSTEM_APBCLK0__I2C5_Pos                        (16UL)                  
#define SYSTEM_APBCLK0__I2C5_Msk                        BIT(SYSTEM_APBCLK0__I2C5_Pos)             
#define SYSTEM_APBCLK0__I2C4_Pos                        (17UL)                  
#define SYSTEM_APBCLK0__I2C4_Msk                        BIT(SYSTEM_APBCLK0__I2C4_Pos)             
#define SYSTEM_APBCLK0__I2C3_Pos                        (18UL)                  
#define SYSTEM_APBCLK0__I2C3_Msk                        BIT(SYSTEM_APBCLK0__I2C3_Pos)             
#define SYSTEM_APBCLK0__I2C2_Pos                        (19UL)                  
#define SYSTEM_APBCLK0__I2C2_Msk                        BIT(SYSTEM_APBCLK0__I2C2_Pos)             
#define SYSTEM_APBCLK0__I2C1_Pos                        (20UL)                  
#define SYSTEM_APBCLK0__I2C1_Msk                        BIT(SYSTEM_APBCLK0__I2C1_Pos)            
#define SYSTEM_APBCLK0__GPIO_Pos                        (21UL)                  
#define SYSTEM_APBCLK0__GPIO_Msk                        BIT(SYSTEM_APBCLK0__GPIO_Pos)            
#define SYSTEM_APBCLK0__CEC2_Pos                        (22UL)                  
#define SYSTEM_APBCLK0__CEC2_Msk                        BIT(SYSTEM_APBCLK0__CEC2_Pos)            
#define SYSTEM_APBCLK0__CEC1_Pos                        (23UL)                  
#define SYSTEM_APBCLK0__CEC1_Msk                        BIT(SYSTEM_APBCLK0__CEC1_Pos)            
#define SYSTEM_APBCLK0__ADC_Pos                         (24UL)                  
#define SYSTEM_APBCLK0__ADC_Msk                         BIT(SYSTEM_APBCLK0__ADC_Pos)           
#define SYSTEM_IPCLK3__RTMR_Pos                         (25UL)                  
#define SYSTEM_IPCLK3__RTMR_Msk                         BIT(SYSTEM_IPCLK3__RTMR_Pos)           
#define SYSTEM_IPCLK3__PS2_L3_Pos                       (26UL)                  
#define SYSTEM_IPCLK3__PS2_L3_Msk                       BIT(SYSTEM_IPCLK3__PS2_L3_Pos)           
#define SYSTEM_IPCLK3__PS2_L2_Pos                       (27UL)                  
#define SYSTEM_IPCLK3__PS2_L2_Msk                       BIT(SYSTEM_IPCLK3__PS2_L2_Pos)           
#define SYSTEM_IPCLK3__PS2_L1_Pos                       (28UL)                  
#define SYSTEM_IPCLK3__PS2_L1_Msk                       BIT(SYSTEM_IPCLK3__PS2_L1_Pos)          
#define SYSTEM_IPCLK3__RC32KREF_Pos                     (29UL)                  
#define SYSTEM_IPCLK3__RC32KREF_Msk                     BIT(SYSTEM_IPCLK3__RC32KREF_Pos)          
#define SYSTEM_IPCLK3__PECI_Pos                         (30UL)                  
#define SYSTEM_IPCLK3__PECI_Msk                         BIT(SYSTEM_IPCLK3__PECI_Pos)          
#define SYSTEM_IPCLK3__I3C3_Pos                         (31UL)                  
#define SYSTEM_IPCLK3__I3C3_Msk                         BIT(SYSTEM_IPCLK3__I3C3_Pos)          
#define SYSTEM_APBCLK1__TACH8_Pos                       (0UL)                   
#define SYSTEM_APBCLK1__TACH8_Msk                       BIT(SYSTEM_APBCLK1__TACH8_Pos)                 
#define SYSTEM_APBCLK1__TACH7_Pos                       (1UL)                   
#define SYSTEM_APBCLK1__TACH7_Msk                       BIT(SYSTEM_APBCLK1__TACH7_Pos)                 
#define SYSTEM_APBCLK1__TACH6_Pos                       (2UL)                   
#define SYSTEM_APBCLK1__TACH6_Msk                       BIT(SYSTEM_APBCLK1__TACH6_Pos)                 
#define SYSTEM_APBCLK1__TACH5_Pos                       (3UL)                   
#define SYSTEM_APBCLK1__TACH5_Msk                       BIT(SYSTEM_APBCLK1__TACH5_Pos)                 
#define SYSTEM_APBCLK1__TACH4_Pos                       (4UL)                   
#define SYSTEM_APBCLK1__TACH4_Msk                       BIT(SYSTEM_APBCLK1__TACH4_Pos)                
#define SYSTEM_APBCLK1__TACH3_Pos                       (5UL)                   
#define SYSTEM_APBCLK1__TACH3_Msk                       BIT(SYSTEM_APBCLK1__TACH3_Pos)                
#define SYSTEM_APBCLK1__TACH2_Pos                       (6UL)                   
#define SYSTEM_APBCLK1__TACH2_Msk                       BIT(SYSTEM_APBCLK1__TACH2_Pos)                
#define SYSTEM_APBCLK1__TACH1_Pos                       (7UL)                   
#define SYSTEM_APBCLK1__TACH1_Msk                       BIT(SYSTEM_APBCLK1__TACH1_Pos)                
#define SYSTEM_APBCLK1__SPIM_Pos                        (8UL)                   
#define SYSTEM_APBCLK1__SPIM_Msk                        BIT(SYSTEM_APBCLK1__SPIM_Pos)               
#define SYSTEM_APBCLK1__SPIS_Pos                        (9UL)                   
#define SYSTEM_APBCLK1__SPIS_Msk                        BIT(SYSTEM_APBCLK1__SPIS_Pos)               
#define SYSTEM_APBCLK1__SLWTMR3_Pos                     (10UL)                  
#define SYSTEM_APBCLK1__SLWTMR3_Msk                     BIT(SYSTEM_APBCLK1__SLWTMR3_Pos)               
#define SYSTEM_APBCLK1__SLWTMR2_Pos                     (11UL)                  
#define SYSTEM_APBCLK1__SLWTMR2_Msk                     BIT(SYSTEM_APBCLK1__SLWTMR2_Pos)               
#define SYSTEM_APBCLK1__SLWTMR1_Pos                     (12UL)                  
#define SYSTEM_APBCLK1__SLWTMR1_Msk                     BIT(SYSTEM_APBCLK1__SLWTMR1_Pos)              
#define SYSTEM_APBCLK1__SLWTMR0_Pos                     (13UL)                  
#define SYSTEM_APBCLK1__SLWTMR0_Msk                     BIT(SYSTEM_APBCLK1__SLWTMR0_Pos)              
#define SYSTEM_APBCLK1__RTMR_Pos                        (14UL)                  
#define SYSTEM_APBCLK1__RTMR_Msk                        BIT(SYSTEM_APBCLK1__RTMR_Pos)              
#define SYSTEM_APBCLK1__RTC_Pos                         (15UL)                  
#define SYSTEM_APBCLK1__RTC_Msk                         BIT(SYSTEM_APBCLK1__RTC_Pos)              
#define SYSTEM_APBCLK1__PWRBTN1_Pos                     (16UL)                  
#define SYSTEM_APBCLK1__PWRBTN1_Msk                     BIT(SYSTEM_APBCLK1__PWRBTN1_Pos)             
#define SYSTEM_APBCLK1__PWRBTN0_Pos                     (17UL)                  
#define SYSTEM_APBCLK1__PWRBTN0_Msk                     BIT(SYSTEM_APBCLK1__PWRBTN0_Pos)             
#define SYSTEM_APBCLK1__PWM14_Pos                       (18UL)                  
#define SYSTEM_APBCLK1__PWM14_Msk                       BIT(SYSTEM_APBCLK1__PWM14_Pos)             
#define SYSTEM_APBCLK1__PWM13_Pos                       (19UL)                  
#define SYSTEM_APBCLK1__PWM13_Msk                       BIT(SYSTEM_APBCLK1__PWM13_Pos)             
#define SYSTEM_APBCLK1__PWM12_Pos                       (20UL)                  
#define SYSTEM_APBCLK1__PWM12_Msk                       BIT(SYSTEM_APBCLK1__PWM12_Pos)            
#define SYSTEM_APBCLK1__PWM11_Pos                       (21UL)                  
#define SYSTEM_APBCLK1__PWM11_Msk                       BIT(SYSTEM_APBCLK1__PWM11_Pos)            
#define SYSTEM_APBCLK1__PWM10_Pos                       (22UL)                  
#define SYSTEM_APBCLK1__PWM10_Msk                       BIT(SYSTEM_APBCLK1__PWM10_Pos)            
#define SYSTEM_APBCLK1__PWM9_Pos                        (23UL)                  
#define SYSTEM_APBCLK1__PWM9_Msk                        BIT(SYSTEM_APBCLK1__PWM9_Pos)            
#define SYSTEM_APBCLK1__PWM8_Pos                        (24UL)                  
#define SYSTEM_APBCLK1__PWM8_Msk                        BIT(SYSTEM_APBCLK1__PWM8_Pos)           
#define SYSTEM_APBCLK1__PWM7_Pos                        (25UL)                  
#define SYSTEM_APBCLK1__PWM7_Msk                        BIT(SYSTEM_APBCLK1__PWM7_Pos)           
#define SYSTEM_APBCLK1__PWM6_Pos                        (26UL)                  
#define SYSTEM_APBCLK1__PWM6_Msk                        BIT(SYSTEM_APBCLK1__PWM6_Pos)           
#define SYSTEM_APBCLK1__PWM5_Pos                        (27UL)                  
#define SYSTEM_APBCLK1__PWM5_Msk                        BIT(SYSTEM_APBCLK1__PWM5_Pos)           
#define SYSTEM_APBCLK1__PWM4_Pos                        (28UL)                  
#define SYSTEM_APBCLK1__PWM4_Msk                        BIT(SYSTEM_APBCLK1__PWM4_Pos)          
#define SYSTEM_APBCLK1__PWM3_Pos                        (29UL)                  
#define SYSTEM_APBCLK1__PWM3_Msk                        BIT(SYSTEM_APBCLK1__PWM3_Pos)          
#define SYSTEM_APBCLK1__PWM2_Pos                        (30UL)                  
#define SYSTEM_APBCLK1__PWM2_Msk                        BIT(SYSTEM_APBCLK1__PWM2_Pos)          
#define SYSTEM_APBCLK1__PWM1_Pos                        (31UL)                  
#define SYSTEM_APBCLK1__PWM1_Msk                        BIT(SYSTEM_APBCLK1__PWM1_Pos)          
#define SYSTEM_IPCLK4__USBDPHY_Pos                      (5UL)                   
#define SYSTEM_IPCLK4__USBDPHY_Msk                      BIT(SYSTEM_IPCLK4__USBDPHY_Pos)                
#define SYSTEM_IPCLK4__USB_Pos                          (6UL)                   
#define SYSTEM_IPCLK4__USB_Msk                          BIT(SYSTEM_IPCLK4__USB_Pos)                
#define SYSTEM_IPCLK4__TRC_CLK_Pos                      (7UL)                   
#define SYSTEM_IPCLK4__TRC_CLK_Msk                      BIT(SYSTEM_IPCLK4__TRC_CLK_Pos)                
#define SYSTEM_IPCLK4__LALU_Pos                         (8UL)                   
#define SYSTEM_IPCLK4__LALU_Msk                         BIT(SYSTEM_IPCLK4__LALU_Pos)               
#define SYSTEM_APBCLK2__TRC_Pos                         (9UL)                   
#define SYSTEM_APBCLK2__TRC_Msk                         BIT(SYSTEM_APBCLK2__TRC_Pos)               
#define SYSTEM_APBCLK2__PECI_Pos                        (10UL)                  
#define SYSTEM_APBCLK2__PECI_Msk                        BIT(SYSTEM_APBCLK2__PECI_Pos)               
#define SYSTEM_APBCLK2__LPT_Pos                         (11UL)                  
#define SYSTEM_APBCLK2__LPT_Msk                         BIT(SYSTEM_APBCLK2__LPT_Pos)               
#define SYSTEM_APBCLK2__WDT_Pos                         (12UL)                  
#define SYSTEM_APBCLK2__WDT_Msk                         BIT(SYSTEM_APBCLK2__WDT_Pos)              
#define SYSTEM_APBCLK2__UART2_Pos                       (13UL)                  
#define SYSTEM_APBCLK2__UART2_Msk                       BIT(SYSTEM_APBCLK2__UART2_Pos)              
#define SYSTEM_APBCLK2__UART1_Pos                       (14UL)                  
#define SYSTEM_APBCLK2__UART1_Msk                       BIT(SYSTEM_APBCLK2__UART1_Pos)              
#define SYSTEM_APBCLK2__UART0_Pos                       (15UL)                  
#define SYSTEM_APBCLK2__UART0_Msk                       BIT(SYSTEM_APBCLK2__UART0_Pos)              
#define SYSTEM_APBCLK2__TMR5_Pos                        (16UL)                  
#define SYSTEM_APBCLK2__TMR5_Msk                        BIT(SYSTEM_APBCLK2__TMR5_Pos)             
#define SYSTEM_APBCLK2__TMR4_Pos                        (17UL)                  
#define SYSTEM_APBCLK2__TMR4_Msk                        BIT(SYSTEM_APBCLK2__TMR4_Pos)             
#define SYSTEM_APBCLK2__TMR3_Pos                        (18UL)                  
#define SYSTEM_APBCLK2__TMR3_Msk                        BIT(SYSTEM_APBCLK2__TMR3_Pos)             
#define SYSTEM_APBCLK2__TMR2_Pos                        (19UL)                  
#define SYSTEM_APBCLK2__TMR2_Msk                        BIT(SYSTEM_APBCLK2__TMR2_Pos)             
#define SYSTEM_APBCLK2__TMR1_Pos                        (20UL)                  
#define SYSTEM_APBCLK2__TMR1_Msk                        BIT(SYSTEM_APBCLK2__TMR1_Pos)            
#define SYSTEM_APBCLK2__TMR0_Pos                        (21UL)                  
#define SYSTEM_APBCLK2__TMR0_Msk                        BIT(SYSTEM_APBCLK2__TMR0_Pos)            
#define SYSTEM_APBCLK2__TACH14_Pos                      (22UL)                  
#define SYSTEM_APBCLK2__TACH14_Msk                      BIT(SYSTEM_APBCLK2__TACH14_Pos)            
#define SYSTEM_APBCLK2__TACH13_Pos                      (23UL)                  
#define SYSTEM_APBCLK2__TACH13_Msk                      BIT(SYSTEM_APBCLK2__TACH13_Pos)            
#define SYSTEM_APBCLK2__TACH12_Pos                      (24UL)                  
#define SYSTEM_APBCLK2__TACH12_Msk                      BIT(SYSTEM_APBCLK2__TACH12_Pos)           
#define SYSTEM_APBCLK2__TACH11_Pos                      (25UL)                  
#define SYSTEM_APBCLK2__TACH11_Msk                      BIT(SYSTEM_APBCLK2__TACH11_Pos)           
#define SYSTEM_APBCLK2__TACH10_Pos                      (26UL)                  
#define SYSTEM_APBCLK2__TACH10_Msk                      BIT(SYSTEM_APBCLK2__TACH10_Pos)           
#define SYSTEM_APBCLK2__TACH9_Pos                       (27UL)                  
#define SYSTEM_APBCLK2__TACH9_Msk                       BIT(SYSTEM_APBCLK2__TACH9_Pos)           
#define SYSTEM_IPDIV0__ESPI_Pos                         (0UL)                   
#define SYSTEM_IPDIV0__ESPI_Msk                         GENMASK(1, 0)                 
#define SYSTEM_IPDIV0__DMA_Pos                          (24UL)                  
#define SYSTEM_IPDIV0__DMA_Msk                          GENMASK(25, 24)           
#define SYSTEM_IPDIV0__OTP_Pos                          (26UL)                  
#define SYSTEM_IPDIV0__OTP_Msk                          GENMASK(27, 26)           
#define SYSTEM_IPDIV0__BUS_Pos                          (30UL)                  
#define SYSTEM_IPDIV0__BUS_Msk                          GENMASK(31, 30)          
#define SYSTEM_IPDIV1__PWM8_Pos                         (0UL)                   
#define SYSTEM_IPDIV1__PWM8_Msk                         GENMASK(1, 0)                 
#define SYSTEM_IPDIV1__PWM7_Pos                         (2UL)                   
#define SYSTEM_IPDIV1__PWM7_Msk                         GENMASK(3, 2)                 
#define SYSTEM_IPDIV1__PWM6_Pos                         (4UL)                   
#define SYSTEM_IPDIV1__PWM6_Msk                         GENMASK(5, 4)                
#define SYSTEM_IPDIV1__PWM5_Pos                         (6UL)                   
#define SYSTEM_IPDIV1__PWM5_Msk                         GENMASK(7, 6)                
#define SYSTEM_IPDIV1__PWM4_Pos                         (8UL)                   
#define SYSTEM_IPDIV1__PWM4_Msk                         GENMASK(9, 8)               
#define SYSTEM_IPDIV1__PWM3_Pos                         (10UL)                  
#define SYSTEM_IPDIV1__PWM3_Msk                         GENMASK(11, 10)               
#define SYSTEM_IPDIV1__PWM2_Pos                         (12UL)                  
#define SYSTEM_IPDIV1__PWM2_Msk                         GENMASK(13, 12)              
#define SYSTEM_IPDIV1__PWM1_Pos                         (14UL)                  
#define SYSTEM_IPDIV1__PWM1_Msk                         GENMASK(15, 14)              
#define SYSTEM_IPDIV1__LED4_Pos                         (16UL)                  
#define SYSTEM_IPDIV1__LED4_Msk                         GENMASK(17, 16)             
#define SYSTEM_IPDIV1__LED3_Pos                         (18UL)                  
#define SYSTEM_IPDIV1__LED3_Msk                         GENMASK(19, 18)             
#define SYSTEM_IPDIV1__LED2_Pos                         (20UL)                  
#define SYSTEM_IPDIV1__LED2_Msk                         GENMASK(21, 20)            
#define SYSTEM_IPDIV1__LED1_Pos                         (22UL)                  
#define SYSTEM_IPDIV1__LED1_Msk                         GENMASK(23, 22)            
#define SYSTEM_IPDIV1__EMI7_Pos                         (24UL)                  
#define SYSTEM_IPDIV1__EMI7_Msk                         GENMASK(25, 24)           
#define SYSTEM_IPDIV1__EMI6_Pos                         (26UL)                  
#define SYSTEM_IPDIV1__EMI6_Msk                         GENMASK(27, 26)           
#define SYSTEM_IPDIV1__EMI5_Pos                         (28UL)                  
#define SYSTEM_IPDIV1__EMI5_Msk                         GENMASK(29, 28)          
#define SYSTEM_IPDIV1__EMI4_Pos                         (30UL)                  
#define SYSTEM_IPDIV1__EMI4_Msk                         GENMASK(31, 30)          
#define SYSTEM_IPDIV2__I2C5_Pos                         (0UL)                   
#define SYSTEM_IPDIV2__I2C5_Msk                         GENMASK(1, 0)                 
#define SYSTEM_IPDIV2__I2C4_Pos                         (2UL)                   
#define SYSTEM_IPDIV2__I2C4_Msk                         GENMASK(3, 2)                 
#define SYSTEM_IPDIV2__I2C3_Pos                         (4UL)                   
#define SYSTEM_IPDIV2__I2C3_Msk                         GENMASK(5, 4)                
#define SYSTEM_IPDIV2__I2C2_Pos                         (6UL)                   
#define SYSTEM_IPDIV2__I2C2_Msk                         GENMASK(7, 6)                
#define SYSTEM_IPDIV2__I2C1_Pos                         (8UL)                   
#define SYSTEM_IPDIV2__I2C1_Msk                         GENMASK(9, 8)               
#define SYSTEM_IPDIV2__UART2_Pos                        (10UL)                  
#define SYSTEM_IPDIV2__UART2_Msk                        GENMASK(11, 10)               
#define SYSTEM_IPDIV2__UART1_Pos                        (12UL)                  
#define SYSTEM_IPDIV2__UART1_Msk                        GENMASK(13, 12)              
#define SYSTEM_IPDIV2__UART0_Pos                        (14UL)                  
#define SYSTEM_IPDIV2__UART0_Msk                        GENMASK(15, 14)              
#define SYSTEM_IPDIV2__ADC_Pos                          (16UL)                  
#define SYSTEM_IPDIV2__ADC_Msk                          GENMASK(18, 16)             
#define SYSTEM_IPDIV2__PWM14_Pos                        (19UL)                  
#define SYSTEM_IPDIV2__PWM14_Msk                        GENMASK(20, 19)            
#define SYSTEM_IPDIV2__PWM13_Pos                        (21UL)                  
#define SYSTEM_IPDIV2__PWM13_Msk                        GENMASK(22, 21)            
#define SYSTEM_IPDIV2__PWM12_Pos                        (23UL)                  
#define SYSTEM_IPDIV2__PWM12_Msk                        GENMASK(24, 23)           
#define SYSTEM_IPDIV2__PWM11_Pos                        (25UL)                  
#define SYSTEM_IPDIV2__PWM11_Msk                        GENMASK(26, 25)           
#define SYSTEM_IPDIV2__PWM10_Pos                        (27UL)                  
#define SYSTEM_IPDIV2__PWM10_Msk                        GENMASK(28, 27)          
#define SYSTEM_IPDIV2__PWM9_Pos                         (29UL)                  
#define SYSTEM_IPDIV2__PWM9_Msk                         GENMASK(30, 29)          
#define SYSTEM_IPDIV3__I2C11_Pos                        (20UL)                  
#define SYSTEM_IPDIV3__I2C11_Msk                        GENMASK(21, 20)            
#define SYSTEM_IPDIV3__I2C10_Pos                        (22UL)                  
#define SYSTEM_IPDIV3__I2C10_Msk                        GENMASK(23, 22)            
#define SYSTEM_IPDIV3__I2C9_Pos                         (24UL)                  
#define SYSTEM_IPDIV3__I2C9_Msk                         GENMASK(25, 24)           
#define SYSTEM_IPDIV3__I2C8_Pos                         (26UL)                  
#define SYSTEM_IPDIV3__I2C8_Msk                         GENMASK(27, 26)           
#define SYSTEM_IPDIV3__I2C7_Pos                         (28UL)                  
#define SYSTEM_IPDIV3__I2C7_Msk                         GENMASK(29, 28)          
#define SYSTEM_IPDIV3__I2C6_Pos                         (30UL)                  
#define SYSTEM_IPDIV3__I2C6_Msk                         GENMASK(31, 30)          
#define SYSTEM_IPDIV4__TMR5_Pos                         (7UL)                   
#define SYSTEM_IPDIV4__TMR5_Msk                         GENMASK(10, 7)               
#define SYSTEM_IPDIV4__TMR4_Pos                         (11UL)                  
#define SYSTEM_IPDIV4__TMR4_Msk                         GENMASK(14, 11)              
#define SYSTEM_IPDIV4__TMR3_Pos                         (15UL)                  
#define SYSTEM_IPDIV4__TMR3_Msk                         GENMASK(18, 15)             
#define SYSTEM_IPDIV4__TMR2_Pos                         (19UL)                  
#define SYSTEM_IPDIV4__TMR2_Msk                         GENMASK(22, 19)            
#define SYSTEM_IPDIV4__TMR1_Pos                         (23UL)                  
#define SYSTEM_IPDIV4__TMR1_Msk                         GENMASK(26, 23)           
#define SYSTEM_IPDIV4__TMR0_Pos                         (27UL)                  
#define SYSTEM_IPDIV4__TMR0_Msk                         GENMASK(30, 27)         
#define SYSTEM_APBDIV0__I2C2_Pos                        (0UL)                   
#define SYSTEM_APBDIV0__I2C2_Msk                        GENMASK(1, 0)                 
#define SYSTEM_APBDIV0__I2C1_Pos                        (2UL)                   
#define SYSTEM_APBDIV0__I2C1_Msk                        GENMASK(3, 2)                 
#define SYSTEM_APBDIV0__GPIO_Pos                        (4UL)                   
#define SYSTEM_APBDIV0__GPIO_Msk                        GENMASK(5, 4)                
#define SYSTEM_APBDIV0__CEC2_Pos                        (6UL)                   
#define SYSTEM_APBDIV0__CEC2_Msk                        GENMASK(7, 6)                
#define SYSTEM_APBDIV0__CEC1_Pos                        (8UL)                   
#define SYSTEM_APBDIV0__CEC1_Msk                        GENMASK(9, 8)               
#define SYSTEM_APBDIV0__ADC_Pos                         (10UL)                  
#define SYSTEM_APBDIV0__ADC_Msk                         GENMASK(11, 10)               
#define SYSTEM_IPDIV5__I3C3_Pos                         (18UL)                  
#define SYSTEM_IPDIV5__I3C3_Msk                         GENMASK(19, 18)             
#define SYSTEM_IPDIV5__I3C2_Pos                         (20UL)                  
#define SYSTEM_IPDIV5__I3C2_Msk                         GENMASK(21, 20)            
#define SYSTEM_IPDIV5__I3C1_Pos                         (22UL)                  
#define SYSTEM_IPDIV5__I3C1_Msk                         GENMASK(23, 22)            
#define SYSTEM_IPDIV5__I3C4_Pos                         (24UL)                  
#define SYSTEM_IPDIV5__I3C4_Msk                         GENMASK(25, 24)           
#define SYSTEM_APBDIV1__LEDSTP_Pos                      (0UL)                   
#define SYSTEM_APBDIV1__LEDSTP_Msk                      GENMASK(1, 0)                 
#define SYSTEM_APBDIV1__LED4_Pos                        (2UL)                   
#define SYSTEM_APBDIV1__LED4_Msk                        GENMASK(3, 2)                 
#define SYSTEM_APBDIV1__LED3_Pos                        (4UL)                   
#define SYSTEM_APBDIV1__LED3_Msk                        GENMASK(5, 4)                
#define SYSTEM_APBDIV1__LED2_Pos                        (6UL)                   
#define SYSTEM_APBDIV1__LED2_Msk                        GENMASK(7, 6)                
#define SYSTEM_APBDIV1__LED1_Pos                        (8UL)                   
#define SYSTEM_APBDIV1__LED1_Msk                        GENMASK(9, 8)               
#define SYSTEM_APBDIV1__KBM_Pos                         (10UL)                  
#define SYSTEM_APBDIV1__KBM_Msk                         GENMASK(11, 10)               
#define SYSTEM_APBDIV1__I2CDBG_Pos                      (12UL)                  
#define SYSTEM_APBDIV1__I2CDBG_Msk                      GENMASK(13, 12)              
#define SYSTEM_APBDIV1__I2C11_Pos                       (14UL)                  
#define SYSTEM_APBDIV1__I2C11_Msk                       GENMASK(15, 14)              
#define SYSTEM_APBDIV1__I2C10_Pos                       (16UL)                  
#define SYSTEM_APBDIV1__I2C10_Msk                       GENMASK(17, 16)             
#define SYSTEM_APBDIV1__I2C9_Pos                        (18UL)                  
#define SYSTEM_APBDIV1__I2C9_Msk                        GENMASK(19, 18)             
#define SYSTEM_APBDIV1__I2C8_Pos                        (20UL)                  
#define SYSTEM_APBDIV1__I2C8_Msk                        GENMASK(21, 20)            
#define SYSTEM_APBDIV1__I2C7_Pos                        (22UL)                  
#define SYSTEM_APBDIV1__I2C7_Msk                        GENMASK(23, 22)            
#define SYSTEM_APBDIV1__I2C6_Pos                        (24UL)                  
#define SYSTEM_APBDIV1__I2C6_Msk                        GENMASK(25, 24)           
#define SYSTEM_APBDIV1__I2C5_Pos                        (26UL)                  
#define SYSTEM_APBDIV1__I2C5_Msk                        GENMASK(27, 26)           
#define SYSTEM_APBDIV1__I2C4_Pos                        (28UL)                  
#define SYSTEM_APBDIV1__I2C4_Msk                        GENMASK(29, 28)          
#define SYSTEM_APBDIV1__I2C3_Pos                        (30UL)                  
#define SYSTEM_APBDIV1__I2C3_Msk                        GENMASK(31, 30)          
#define SYSTEM_APBDIV2__PWM13_Pos                       (0UL)                   
#define SYSTEM_APBDIV2__PWM13_Msk                       GENMASK(1, 0)                 
#define SYSTEM_APBDIV2__PWM12_Pos                       (2UL)                   
#define SYSTEM_APBDIV2__PWM12_Msk                       GENMASK(3, 2)                 
#define SYSTEM_APBDIV2__PWM11_Pos                       (4UL)                   
#define SYSTEM_APBDIV2__PWM11_Msk                       GENMASK(5, 4)                
#define SYSTEM_APBDIV2__PWM10_Pos                       (6UL)                   
#define SYSTEM_APBDIV2__PWM10_Msk                       GENMASK(7, 6)                
#define SYSTEM_APBDIV2__PWM9_Pos                        (8UL)                   
#define SYSTEM_APBDIV2__PWM9_Msk                        GENMASK(9, 8)               
#define SYSTEM_APBDIV2__PWM8_Pos                        (10UL)                  
#define SYSTEM_APBDIV2__PWM8_Msk                        GENMASK(11, 10)               
#define SYSTEM_APBDIV2__PWM7_Pos                        (12UL)                  
#define SYSTEM_APBDIV2__PWM7_Msk                        GENMASK(13, 12)              
#define SYSTEM_APBDIV2__PWM6_Pos                        (14UL)                  
#define SYSTEM_APBDIV2__PWM6_Msk                        GENMASK(15, 14)              
#define SYSTEM_APBDIV2__PWM5_Pos                        (16UL)                  
#define SYSTEM_APBDIV2__PWM5_Msk                        GENMASK(17, 16)             
#define SYSTEM_APBDIV2__PWM4_Pos                        (18UL)                  
#define SYSTEM_APBDIV2__PWM4_Msk                        GENMASK(19, 18)             
#define SYSTEM_APBDIV2__PWM3_Pos                        (20UL)                  
#define SYSTEM_APBDIV2__PWM3_Msk                        GENMASK(21, 20)            
#define SYSTEM_APBDIV2__PWM2_Pos                        (22UL)                  
#define SYSTEM_APBDIV2__PWM2_Msk                        GENMASK(23, 22)            
#define SYSTEM_APBDIV2__PWM1_Pos                        (24UL)                  
#define SYSTEM_APBDIV2__PWM1_Msk                        GENMASK(25, 24)           
#define SYSTEM_APBDIV2__PS2_3_Pos                       (26UL)                  
#define SYSTEM_APBDIV2__PS2_3_Msk                       GENMASK(27, 26)           
#define SYSTEM_APBDIV2__PS2_2_Pos                       (28UL)                  
#define SYSTEM_APBDIV2__PS2_2_Msk                       GENMASK(29, 28)          
#define SYSTEM_APBDIV2__PS2_1_Pos                       (30UL)                  
#define SYSTEM_APBDIV2__PS2_1_Msk                       GENMASK(31, 30)          
#define SYSTEM_APBDIV3__TACH5_Pos                       (0UL)                   
#define SYSTEM_APBDIV3__TACH5_Msk                       GENMASK(1, 0)                 
#define SYSTEM_APBDIV3__TACH4_Pos                       (2UL)                   
#define SYSTEM_APBDIV3__TACH4_Msk                       GENMASK(3, 2)                 
#define SYSTEM_APBDIV3__TACH3_Pos                       (4UL)                   
#define SYSTEM_APBDIV3__TACH3_Msk                       GENMASK(5, 4)                
#define SYSTEM_APBDIV3__TACH2_Pos                       (6UL)                   
#define SYSTEM_APBDIV3__TACH2_Msk                       GENMASK(7, 6)                
#define SYSTEM_APBDIV3__TACH1_Pos                       (8UL)                   
#define SYSTEM_APBDIV3__TACH1_Msk                       GENMASK(9, 8)               
#define SYSTEM_APBDIV3__SPIM_Pos                        (10UL)                  
#define SYSTEM_APBDIV3__SPIM_Msk                        GENMASK(11, 10)               
#define SYSTEM_APBDIV3__SPIS_Pos                        (12UL)                  
#define SYSTEM_APBDIV3__SPIS_Msk                        GENMASK(13, 12)              
#define SYSTEM_APBDIV3__SLWTMR3_Pos                     (14UL)                  
#define SYSTEM_APBDIV3__SLWTMR3_Msk                     GENMASK(15, 14)              
#define SYSTEM_APBDIV3__SLWTMR2_Pos                     (16UL)                  
#define SYSTEM_APBDIV3__SLWTMR2_Msk                     GENMASK(17, 16)             
#define SYSTEM_APBDIV3__SLWTMR1_Pos                     (18UL)                  
#define SYSTEM_APBDIV3__SLWTMR1_Msk                     GENMASK(19, 18)             
#define SYSTEM_APBDIV3__SLWTMR0_Pos                     (20UL)                  
#define SYSTEM_APBDIV3__SLWTMR0_Msk                     GENMASK(21, 20)            
#define SYSTEM_APBDIV3__RTMR_Pos                        (22UL)                  
#define SYSTEM_APBDIV3__RTMR_Msk                        GENMASK(23, 22)            
#define SYSTEM_APBDIV3__RTC_Pos                         (24UL)                  
#define SYSTEM_APBDIV3__RTC_Msk                         GENMASK(25, 24)           
#define SYSTEM_APBDIV3__PWRBTN1_Pos                     (26UL)                  
#define SYSTEM_APBDIV3__PWRBTN1_Msk                     GENMASK(27, 26)           
#define SYSTEM_APBDIV3__PWRBTN0_Pos                     (28UL)                  
#define SYSTEM_APBDIV3__PWRBTN0_Msk                     GENMASK(29, 28)          
#define SYSTEM_APBDIV3__PWM14_Pos                       (30UL)                  
#define SYSTEM_APBDIV3__PWM14_Msk                       GENMASK(31, 30)          
#define SYSTEM_APBDIV4__UART0_Pos                       (0UL)                   
#define SYSTEM_APBDIV4__UART0_Msk                       GENMASK(1, 0)                 
#define SYSTEM_APBDIV4__TMR5_Pos                        (2UL)                   
#define SYSTEM_APBDIV4__TMR5_Msk                        GENMASK(3, 2)                 
#define SYSTEM_APBDIV4__TMR4_Pos                        (4UL)                   
#define SYSTEM_APBDIV4__TMR4_Msk                        GENMASK(5, 4)                
#define SYSTEM_APBDIV4__TMR3_Pos                        (6UL)                   
#define SYSTEM_APBDIV4__TMR3_Msk                        GENMASK(7, 6)                
#define SYSTEM_APBDIV4__TMR2_Pos                        (8UL)                   
#define SYSTEM_APBDIV4__TMR2_Msk                        GENMASK(9, 8)               
#define SYSTEM_APBDIV4__TMR1_Pos                        (10UL)                  
#define SYSTEM_APBDIV4__TMR1_Msk                        GENMASK(11, 10)               
#define SYSTEM_APBDIV4__TMR0_Pos                        (12UL)                  
#define SYSTEM_APBDIV4__TMR0_Msk                        GENMASK(13, 12)              
#define SYSTEM_APBDIV4__TACH14_Pos                      (14UL)                  
#define SYSTEM_APBDIV4__TACH14_Msk                      GENMASK(15, 14)              
#define SYSTEM_APBDIV4__TACH13_Pos                      (16UL)                  
#define SYSTEM_APBDIV4__TACH13_Msk                      GENMASK(17, 16)             
#define SYSTEM_APBDIV4__TACH12_Pos                      (18UL)                  
#define SYSTEM_APBDIV4__TACH12_Msk                      GENMASK(19, 18)             
#define SYSTEM_APBDIV4__TACH11_Pos                      (20UL)                  
#define SYSTEM_APBDIV4__TACH11_Msk                      GENMASK(21, 20)            
#define SYSTEM_APBDIV4__TACH10_Pos                      (22UL)                  
#define SYSTEM_APBDIV4__TACH10_Msk                      GENMASK(23, 22)            
#define SYSTEM_APBDIV4__TACH9_Pos                       (24UL)                  
#define SYSTEM_APBDIV4__TACH9_Msk                       GENMASK(25, 24)           
#define SYSTEM_APBDIV4__TACH8_Pos                       (26UL)                  
#define SYSTEM_APBDIV4__TACH8_Msk                       GENMASK(27, 26)           
#define SYSTEM_APBDIV4__TACH7_Pos                       (28UL)                  
#define SYSTEM_APBDIV4__TACH7_Msk                       GENMASK(29, 28)          
#define SYSTEM_APBDIV4__TACH6_Pos                       (30UL)                  
#define SYSTEM_APBDIV4__TACH6_Msk                       GENMASK(31, 30)          
#define SYSTEM_APBDIV5__PECI_Pos                        (11UL)                  
#define SYSTEM_APBDIV5__PECI_Msk                        GENMASK(12, 11)              
#define SYSTEM_APBDIV5__LPT_Pos                         (13UL)                  
#define SYSTEM_APBDIV5__LPT_Msk                         GENMASK(14, 13)              
#define SYSTEM_APBDIV5__WDT_Pos                         (15UL)                  
#define SYSTEM_APBDIV5__WDT_Msk                         GENMASK(16, 15)             
#define SYSTEM_APBDIV5__UART2_Pos                       (17UL)                  
#define SYSTEM_APBDIV5__UART2_Msk                       GENMASK(18, 17)             
#define SYSTEM_APBDIV5__UART1_Pos                       (19UL)                  
#define SYSTEM_APBDIV5__UART1_Msk                       GENMASK(20, 19)            
#define SYSTEM_IPDIV6__USB_Pos                          (6UL)                   
#define SYSTEM_IPDIV6__USB_Msk                          BIT(SYSTEM_IPDIV6__USB_Pos)                
#define SYSTEM_CLKGATING0__PWM8_Pos                     (0UL)                   
#define SYSTEM_CLKGATING0__PWM8_Msk                     BIT(SYSTEM_CLKGATING0__PWM8_Pos)                 
#define SYSTEM_CLKGATING0__PWM7_Pos                     (1UL)                   
#define SYSTEM_CLKGATING0__PWM7_Msk                     BIT(SYSTEM_CLKGATING0__PWM7_Pos)                 
#define SYSTEM_CLKGATING0__PWM6_Pos                     (2UL)                   
#define SYSTEM_CLKGATING0__PWM6_Msk                     BIT(SYSTEM_CLKGATING0__PWM6_Pos)                 
#define SYSTEM_CLKGATING0__PWM5_Pos                     (3UL)                   
#define SYSTEM_CLKGATING0__PWM5_Msk                     BIT(SYSTEM_CLKGATING0__PWM5_Pos)                 
#define SYSTEM_CLKGATING0__PWM4_Pos                     (4UL)                   
#define SYSTEM_CLKGATING0__PWM4_Msk                     BIT(SYSTEM_CLKGATING0__PWM4_Pos)                
#define SYSTEM_CLKGATING0__PWM3_Pos                     (5UL)                   
#define SYSTEM_CLKGATING0__PWM3_Msk                     BIT(SYSTEM_CLKGATING0__PWM3_Pos)                
#define SYSTEM_CLKGATING0__PWM2_Pos                     (6UL)                   
#define SYSTEM_CLKGATING0__PWM2_Msk                     BIT(SYSTEM_CLKGATING0__PWM2_Pos)                
#define SYSTEM_CLKGATING0__PWM1_Pos                     (7UL)                   
#define SYSTEM_CLKGATING0__PWM1_Msk                     BIT(SYSTEM_CLKGATING0__PWM1_Pos)                
#define SYSTEM_CLKGATING0__LED4_Pos                     (8UL)                   
#define SYSTEM_CLKGATING0__LED4_Msk                     BIT(SYSTEM_CLKGATING0__LED4_Pos)               
#define SYSTEM_CLKGATING0__LED3_Pos                     (9UL)                   
#define SYSTEM_CLKGATING0__LED3_Msk                     BIT(SYSTEM_CLKGATING0__LED3_Pos)               
#define SYSTEM_CLKGATING0__LED2_Pos                     (10UL)                  
#define SYSTEM_CLKGATING0__LED2_Msk                     BIT(SYSTEM_CLKGATING0__LED2_Pos)               
#define SYSTEM_CLKGATING0__LED1_Pos                     (11UL)                  
#define SYSTEM_CLKGATING0__LED1_Msk                     BIT(SYSTEM_CLKGATING0__LED1_Pos)               
#define SYSTEM_CLKGATING0__EMI7_Pos                     (12UL)                  
#define SYSTEM_CLKGATING0__EMI7_Msk                     BIT(SYSTEM_CLKGATING0__EMI7_Pos)              
#define SYSTEM_CLKGATING0__EMI6_Pos                     (13UL)                  
#define SYSTEM_CLKGATING0__EMI6_Msk                     BIT(SYSTEM_CLKGATING0__EMI6_Pos)              
#define SYSTEM_CLKGATING0__EMI5_Pos                     (14UL)                  
#define SYSTEM_CLKGATING0__EMI5_Msk                     BIT(SYSTEM_CLKGATING0__EMI5_Pos)              
#define SYSTEM_CLKGATING0__EMI4_Pos                     (15UL)                  
#define SYSTEM_CLKGATING0__EMI4_Msk                     BIT(SYSTEM_CLKGATING0__EMI4_Pos)              
#define SYSTEM_CLKGATING0__EMI3_Pos                     (16UL)                  
#define SYSTEM_CLKGATING0__EMI3_Msk                     BIT(SYSTEM_CLKGATING0__EMI3_Pos)             
#define SYSTEM_CLKGATING0__EMI2_Pos                     (17UL)                  
#define SYSTEM_CLKGATING0__EMI2_Msk                     BIT(SYSTEM_CLKGATING0__EMI2_Pos)             
#define SYSTEM_CLKGATING0__EMI1_Pos                     (18UL)                  
#define SYSTEM_CLKGATING0__EMI1_Msk                     BIT(SYSTEM_CLKGATING0__EMI1_Pos)             
#define SYSTEM_CLKGATING0__EMI0_Pos                     (19UL)                  
#define SYSTEM_CLKGATING0__EMI0_Msk                     BIT(SYSTEM_CLKGATING0__EMI0_Pos)             
#define SYSTEM_CLKGATING0__PORT80_Pos                   (20UL)                  
#define SYSTEM_CLKGATING0__PORT80_Msk                   BIT(SYSTEM_CLKGATING0__PORT80_Pos)            
#define SYSTEM_CLKGATING0__PMPORT3_Pos                  (21UL)                  
#define SYSTEM_CLKGATING0__PMPORT3_Msk                  BIT(SYSTEM_CLKGATING0__PMPORT3_Pos)            
#define SYSTEM_CLKGATING0__PMPORT2_Pos                  (22UL)                  
#define SYSTEM_CLKGATING0__PMPORT2_Msk                  BIT(SYSTEM_CLKGATING0__PMPORT2_Pos)            
#define SYSTEM_CLKGATING0__PMPORT1_Pos                  (23UL)                  
#define SYSTEM_CLKGATING0__PMPORT1_Msk                  BIT(SYSTEM_CLKGATING0__PMPORT1_Pos)            
#define SYSTEM_CLKGATING0__PMPORT0_Pos                  (24UL)                  
#define SYSTEM_CLKGATING0__PMPORT0_Msk                  BIT(SYSTEM_CLKGATING0__PMPORT0_Pos)           
#define SYSTEM_CLKGATING0__ACPI_Pos                     (25UL)                  
#define SYSTEM_CLKGATING0__ACPI_Msk                     BIT(SYSTEM_CLKGATING0__ACPI_Pos)           
#define SYSTEM_CLKGATING0__KBC_Pos                      (26UL)                  
#define SYSTEM_CLKGATING0__KBC_Msk                      BIT(SYSTEM_CLKGATING0__KBC_Pos)           
#define SYSTEM_CLKGATING0__ESPI_Pos                     (27UL)                  
#define SYSTEM_CLKGATING0__ESPI_Msk                     BIT(SYSTEM_CLKGATING0__ESPI_Pos)           
#define SYSTEM_CLKGATING0__DMA_Pos                      (28UL)                  
#define SYSTEM_CLKGATING0__DMA_Msk                      BIT(SYSTEM_CLKGATING0__DMA_Pos)          
#define SYSTEM_CLKGATING0_PUF_OTP_Pos                   (29UL)                  
#define SYSTEM_CLKGATING0_PUF_OTP_Msk                   BIT(SYSTEM_CLKGATING0_PUF_OTP_Pos)          
#define SYSTEM_CLKGATING0__BUS_Pos                      (31UL)                  
#define SYSTEM_CLKGATING0__BUS_Msk                      BIT(SYSTEM_CLKGATING0__BUS_Pos)          
#define SYSTEM_CLKGATING1__TACH5_Pos                    (0UL)                   
#define SYSTEM_CLKGATING1__TACH5_Msk                    BIT(SYSTEM_CLKGATING1__TACH5_Pos)                 
#define SYSTEM_CLKGATING1__TACH4_Pos                    (1UL)                   
#define SYSTEM_CLKGATING1__TACH4_Msk                    BIT(SYSTEM_CLKGATING1__TACH4_Pos)                 
#define SYSTEM_CLKGATING1__TACH3_Pos                    (2UL)                   
#define SYSTEM_CLKGATING1__TACH3_Msk                    BIT(SYSTEM_CLKGATING1__TACH3_Pos)                 
#define SYSTEM_CLKGATING1__TACH2_Pos                    (3UL)                   
#define SYSTEM_CLKGATING1__TACH2_Msk                    BIT(SYSTEM_CLKGATING1__TACH2_Pos)                 
#define SYSTEM_CLKGATING1__TACH1_Pos                    (4UL)                   
#define SYSTEM_CLKGATING1__TACH1_Msk                    BIT(SYSTEM_CLKGATING1__TACH1_Pos)                
#define SYSTEM_CLKGATING1__PS2_H3_Pos                   (5UL)                   
#define SYSTEM_CLKGATING1__PS2_H3_Msk                   BIT(SYSTEM_CLKGATING1__PS2_H3_Pos)                
#define SYSTEM_CLKGATING1__PS2_H2_Pos                   (6UL)                   
#define SYSTEM_CLKGATING1__PS2_H2_Msk                   BIT(SYSTEM_CLKGATING1__PS2_H2_Pos)                
#define SYSTEM_CLKGATING1__PS2_H1_Pos                   (7UL)                   
#define SYSTEM_CLKGATING1__PS2_H1_Msk                   BIT(SYSTEM_CLKGATING1__PS2_H1_Pos)                
#define SYSTEM_CLKGATING1_SWD_Pos                       (8UL)                   
#define SYSTEM_CLKGATING1_SWD_Msk                       BIT(SYSTEM_CLKGATING1_SWD_Pos)               
#define SYSTEM_CLKGATING1_REF25M_Pos                    (9UL)                   
#define SYSTEM_CLKGATING1_REF25M_Msk                    BIT(SYSTEM_CLKGATING1_REF25M_Pos)               
#define SYSTEM_CLKGATING1_RC24M_Pos                     (10UL)                  
#define SYSTEM_CLKGATING1_RC24M_Msk                     BIT(SYSTEM_CLKGATING1_RC24M_Pos)               
#define SYSTEM_CLKGATING1__I2C11_Pos                    (11UL)                  
#define SYSTEM_CLKGATING1__I2C11_Msk                    BIT(SYSTEM_CLKGATING1__I2C11_Pos)               
#define SYSTEM_CLKGATING1__I2C10_Pos                    (12UL)                  
#define SYSTEM_CLKGATING1__I2C10_Msk                    BIT(SYSTEM_CLKGATING1__I2C10_Pos)              
#define SYSTEM_CLKGATING1__I2C9_Pos                     (13UL)                  
#define SYSTEM_CLKGATING1__I2C9_Msk                     BIT(SYSTEM_CLKGATING1__I2C9_Pos)              
#define SYSTEM_CLKGATING1__I2C8_Pos                     (14UL)                  
#define SYSTEM_CLKGATING1__I2C8_Msk                     BIT(SYSTEM_CLKGATING1__I2C8_Pos)              
#define SYSTEM_CLKGATING1__I2C7_Pos                     (15UL)                  
#define SYSTEM_CLKGATING1__I2C7_Msk                     BIT(SYSTEM_CLKGATING1__I2C7_Pos)              
#define SYSTEM_CLKGATING1__I2C6_Pos                     (16UL)                  
#define SYSTEM_CLKGATING1__I2C6_Msk                     BIT(SYSTEM_CLKGATING1__I2C6_Pos)             
#define SYSTEM_CLKGATING1__I2C5_Pos                     (17UL)                  
#define SYSTEM_CLKGATING1__I2C5_Msk                     BIT(SYSTEM_CLKGATING1__I2C5_Pos)             
#define SYSTEM_CLKGATING1__I2C4_Pos                     (18UL)                  
#define SYSTEM_CLKGATING1__I2C4_Msk                     BIT(SYSTEM_CLKGATING1__I2C4_Pos)             
#define SYSTEM_CLKGATING1__I2C3_Pos                     (19UL)                  
#define SYSTEM_CLKGATING1__I2C3_Msk                     BIT(SYSTEM_CLKGATING1__I2C3_Pos)             
#define SYSTEM_CLKGATING1__I2C2_Pos                     (20UL)                  
#define SYSTEM_CLKGATING1__I2C2_Msk                     BIT(SYSTEM_CLKGATING1__I2C2_Pos)            
#define SYSTEM_CLKGATING1__I2C1_Pos                     (21UL)                  
#define SYSTEM_CLKGATING1__I2C1_Msk                     BIT(SYSTEM_CLKGATING1__I2C1_Pos)            
#define SYSTEM_CLKGATING1__UART2_Pos                    (22UL)                  
#define SYSTEM_CLKGATING1__UART2_Msk                    BIT(SYSTEM_CLKGATING1__UART2_Pos)            
#define SYSTEM_CLKGATING1__UART1_Pos                    (23UL)                  
#define SYSTEM_CLKGATING1__UART1_Msk                    BIT(SYSTEM_CLKGATING1__UART1_Pos)            
#define SYSTEM_CLKGATING1__UART0_Pos                    (24UL)                  
#define SYSTEM_CLKGATING1__UART0_Msk                    BIT(SYSTEM_CLKGATING1__UART0_Pos)           
#define SYSTEM_CLKGATING1__ADC_Pos                      (25UL)                  
#define SYSTEM_CLKGATING1__ADC_Msk                      BIT(SYSTEM_CLKGATING1__ADC_Pos)           
#define SYSTEM_CLKGATING1__PWM14_Pos                    (26UL)                  
#define SYSTEM_CLKGATING1__PWM14_Msk                    BIT(SYSTEM_CLKGATING1__PWM14_Pos)           
#define SYSTEM_CLKGATING1__PWM13_Pos                    (27UL)                  
#define SYSTEM_CLKGATING1__PWM13_Msk                    BIT(SYSTEM_CLKGATING1__PWM13_Pos)           
#define SYSTEM_CLKGATING1__PWM12_Pos                    (28UL)                  
#define SYSTEM_CLKGATING1__PWM12_Msk                    BIT(SYSTEM_CLKGATING1__PWM12_Pos)          
#define SYSTEM_CLKGATING1__PWM11_Pos                    (29UL)                  
#define SYSTEM_CLKGATING1__PWM11_Msk                    BIT(SYSTEM_CLKGATING1__PWM11_Pos)          
#define SYSTEM_CLKGATING1__PWM10_Pos                    (30UL)                  
#define SYSTEM_CLKGATING1__PWM10_Msk                    BIT(SYSTEM_CLKGATING1__PWM10_Pos)          
#define SYSTEM_CLKGATING1__PWM9_Pos                     (31UL)                  
#define SYSTEM_CLKGATING1__PWM9_Msk                     BIT(SYSTEM_CLKGATING1__PWM9_Pos)          
#define SYSTEM_CLKGATING2__I3C2_Pos                     (0UL)                   
#define SYSTEM_CLKGATING2__I3C2_Msk                     BIT(SYSTEM_CLKGATING2__I3C2_Pos)                 
#define SYSTEM_CLKGATING2__I3C1_Pos                     (1UL)                   
#define SYSTEM_CLKGATING2__I3C1_Msk                     BIT(SYSTEM_CLKGATING2__I3C1_Pos)                 
#define SYSTEM_CLKGATING2__I3C4_Pos                     (2UL)                   
#define SYSTEM_CLKGATING2__I3C4_Msk                     BIT(SYSTEM_CLKGATING2__I3C4_Pos)                 
#define SYSTEM_CLKGATING2__LPT_Pos                      (3UL)                   
#define SYSTEM_CLKGATING2__LPT_Msk                      BIT(SYSTEM_CLKGATING2__LPT_Pos)                 
#define SYSTEM_CLKGATING2__VINLED_Pos                   (4UL)                   
#define SYSTEM_CLKGATING2__VINLED_Msk                   BIT(SYSTEM_CLKGATING2__VINLED_Pos)                
#define SYSTEM_CLKGATING2__I2CDBG_Pos                   (5UL)                   
#define SYSTEM_CLKGATING2__I2CDBG_Msk                   BIT(SYSTEM_CLKGATING2__I2CDBG_Pos)                
#define SYSTEM_CLKGATING2__MICROCNT_Pos                 (6UL)                   
#define SYSTEM_CLKGATING2__MICROCNT_Msk                 BIT(SYSTEM_CLKGATING2__MICROCNT_Pos)                
#define SYSTEM_CLKGATING2__CEC2_Pos                     (7UL)                   
#define SYSTEM_CLKGATING2__CEC2_Msk                     BIT(SYSTEM_CLKGATING2__CEC2_Pos)                
#define SYSTEM_CLKGATING2__CEC1_Pos                     (8UL)                   
#define SYSTEM_CLKGATING2__CEC1_Msk                     BIT(SYSTEM_CLKGATING2__CEC1_Pos)               
#define SYSTEM_CLKGATING2__LEDSTP_Pos                   (9UL)                   
#define SYSTEM_CLKGATING2__LEDSTP_Msk                   BIT(SYSTEM_CLKGATING2__LEDSTP_Pos)               
#define SYSTEM_CLKGATING2__SLWTMR3_Pos                  (10UL)                  
#define SYSTEM_CLKGATING2__SLWTMR3_Msk                  BIT(SYSTEM_CLKGATING2__SLWTMR3_Pos)               
#define SYSTEM_CLKGATING2__SLWTMR2_Pos                  (11UL)                  
#define SYSTEM_CLKGATING2__SLWTMR2_Msk                  BIT(SYSTEM_CLKGATING2__SLWTMR2_Pos)               
#define SYSTEM_CLKGATING2__SLWTMR1_Pos                  (12UL)                  
#define SYSTEM_CLKGATING2__SLWTMR1_Msk                  BIT(SYSTEM_CLKGATING2__SLWTMR1_Pos)              
#define SYSTEM_CLKGATING2__SLWTMR0_Pos                  (13UL)                  
#define SYSTEM_CLKGATING2__SLWTMR0_Msk                  BIT(SYSTEM_CLKGATING2__SLWTMR0_Pos)              
#define SYSTEM_CLKGATING2__KBMSCAN_Pos                  (14UL)                  
#define SYSTEM_CLKGATING2__KBMSCAN_Msk                  BIT(SYSTEM_CLKGATING2__KBMSCAN_Pos)              
#define SYSTEM_CLKGATING2__KBM_Pos                      (15UL)                  
#define SYSTEM_CLKGATING2__KBM_Msk                      BIT(SYSTEM_CLKGATING2__KBM_Pos)              
#define SYSTEM_CLKGATING2__TMR5_Pos                     (16UL)                  
#define SYSTEM_CLKGATING2__TMR5_Msk                     BIT(SYSTEM_CLKGATING2__TMR5_Pos)             
#define SYSTEM_CLKGATING2__TMR4_Pos                     (17UL)                  
#define SYSTEM_CLKGATING2__TMR4_Msk                     BIT(SYSTEM_CLKGATING2__TMR4_Pos)             
#define SYSTEM_CLKGATING2__TMR3_Pos                     (18UL)                  
#define SYSTEM_CLKGATING2__TMR3_Msk                     BIT(SYSTEM_CLKGATING2__TMR3_Pos)             
#define SYSTEM_CLKGATING2__TMR2_Pos                     (19UL)                  
#define SYSTEM_CLKGATING2__TMR2_Msk                     BIT(SYSTEM_CLKGATING2__TMR2_Pos)             
#define SYSTEM_CLKGATING2__TMR1_Pos                     (20UL)                  
#define SYSTEM_CLKGATING2__TMR1_Msk                     BIT(SYSTEM_CLKGATING2__TMR1_Pos)            
#define SYSTEM_CLKGATING2__TMR0_Pos                     (21UL)                  
#define SYSTEM_CLKGATING2__TMR0_Msk                     BIT(SYSTEM_CLKGATING2__TMR0_Pos)            
#define SYSTEM_CLKGATING2__GPIO_Pos                     (22UL)                  
#define SYSTEM_CLKGATING2__GPIO_Msk                     BIT(SYSTEM_CLKGATING2__GPIO_Pos)            
#define SYSTEM_CLKGATING2__TACH14_Pos                   (23UL)                  
#define SYSTEM_CLKGATING2__TACH14_Msk                   BIT(SYSTEM_CLKGATING2__TACH14_Pos)            
#define SYSTEM_CLKGATING2__TACH13_Pos                   (24UL)                  
#define SYSTEM_CLKGATING2__TACH13_Msk                   BIT(SYSTEM_CLKGATING2__TACH13_Pos)           
#define SYSTEM_CLKGATING2__TACH12_Pos                   (25UL)                  
#define SYSTEM_CLKGATING2__TACH12_Msk                   BIT(SYSTEM_CLKGATING2__TACH12_Pos)           
#define SYSTEM_CLKGATING2__TACH11_Pos                   (26UL)                  
#define SYSTEM_CLKGATING2__TACH11_Msk                   BIT(SYSTEM_CLKGATING2__TACH11_Pos)           
#define SYSTEM_CLKGATING2__TACH10_Pos                   (27UL)                  
#define SYSTEM_CLKGATING2__TACH10_Msk                   BIT(SYSTEM_CLKGATING2__TACH10_Pos)           
#define SYSTEM_CLKGATING2__TACH9_Pos                    (28UL)                  
#define SYSTEM_CLKGATING2__TACH9_Msk                    BIT(SYSTEM_CLKGATING2__TACH9_Pos)          
#define SYSTEM_CLKGATING2__TACH8_Pos                    (29UL)                  
#define SYSTEM_CLKGATING2__TACH8_Msk                    BIT(SYSTEM_CLKGATING2__TACH8_Pos)          
#define SYSTEM_CLKGATING2__TACH7_Pos                    (30UL)                  
#define SYSTEM_CLKGATING2__TACH7_Msk                    BIT(SYSTEM_CLKGATING2__TACH7_Pos)          
#define SYSTEM_CLKGATING2__TACH6_Pos                    (31UL)                  
#define SYSTEM_CLKGATING2__TACH6_Msk                    BIT(SYSTEM_CLKGATING2__TACH6_Pos)          
#define SYSTEM_APBGATING0__PS2_3_Pos                    (0UL)                   
#define SYSTEM_APBGATING0__PS2_3_Msk                    BIT(SYSTEM_APBGATING0__PS2_3_Pos)                 
#define SYSTEM_APBGATING0__PS2_2_Pos                    (1UL)                   
#define SYSTEM_APBGATING0__PS2_2_Msk                    BIT(SYSTEM_APBGATING0__PS2_2_Pos)                 
#define SYSTEM_APBGATING0__PS2_1_Pos                    (2UL)                   
#define SYSTEM_APBGATING0__PS2_1_Msk                    BIT(SYSTEM_APBGATING0__PS2_1_Pos)                 
#define SYSTEM_APBGATING0__LEDSTP_Pos                   (3UL)                   
#define SYSTEM_APBGATING0__LEDSTP_Msk                   BIT(SYSTEM_APBGATING0__LEDSTP_Pos)                 
#define SYSTEM_APBGATING0__LED4_Pos                     (4UL)                   
#define SYSTEM_APBGATING0__LED4_Msk                     BIT(SYSTEM_APBGATING0__LED4_Pos)                
#define SYSTEM_APBGATING0__LED3_Pos                     (5UL)                   
#define SYSTEM_APBGATING0__LED3_Msk                     BIT(SYSTEM_APBGATING0__LED3_Pos)                
#define SYSTEM_APBGATING0__LED2_Pos                     (6UL)                   
#define SYSTEM_APBGATING0__LED2_Msk                     BIT(SYSTEM_APBGATING0__LED2_Pos)                
#define SYSTEM_APBGATING0__LED1_Pos                     (7UL)                   
#define SYSTEM_APBGATING0__LED1_Msk                     BIT(SYSTEM_APBGATING0__LED1_Pos)                
#define SYSTEM_APBGATING0__KBM_Pos                      (8UL)                   
#define SYSTEM_APBGATING0__KBM_Msk                      BIT(SYSTEM_APBGATING0__KBM_Pos)               
#define SYSTEM_APBGATING0__I2CDBG_Pos                   (9UL)                   
#define SYSTEM_APBGATING0__I2CDBG_Msk                   BIT(SYSTEM_APBGATING0__I2CDBG_Pos)               
#define SYSTEM_APBGATING0__I2C11_Pos                    (10UL)                  
#define SYSTEM_APBGATING0__I2C11_Msk                    BIT(SYSTEM_APBGATING0__I2C11_Pos)               
#define SYSTEM_APBGATING0__I2C10_Pos                    (11UL)                  
#define SYSTEM_APBGATING0__I2C10_Msk                    BIT(SYSTEM_APBGATING0__I2C10_Pos)               
#define SYSTEM_APBGATING0__I2C9_Pos                     (12UL)                  
#define SYSTEM_APBGATING0__I2C9_Msk                     BIT(SYSTEM_APBGATING0__I2C9_Pos)              
#define SYSTEM_APBGATING0__I2C8_Pos                     (13UL)                  
#define SYSTEM_APBGATING0__I2C8_Msk                     BIT(SYSTEM_APBGATING0__I2C8_Pos)              
#define SYSTEM_APBGATING0__I2C7_Pos                     (14UL)                  
#define SYSTEM_APBGATING0__I2C7_Msk                     BIT(SYSTEM_APBGATING0__I2C7_Pos)              
#define SYSTEM_APBGATING0__I2C6_Pos                     (15UL)                  
#define SYSTEM_APBGATING0__I2C6_Msk                     BIT(SYSTEM_APBGATING0__I2C6_Pos)              
#define SYSTEM_APBGATING0__I2C5_Pos                     (16UL)                  
#define SYSTEM_APBGATING0__I2C5_Msk                     BIT(SYSTEM_APBGATING0__I2C5_Pos)             
#define SYSTEM_APBGATING0__I2C4_Pos                     (17UL)                  
#define SYSTEM_APBGATING0__I2C4_Msk                     BIT(SYSTEM_APBGATING0__I2C4_Pos)             
#define SYSTEM_APBGATING0__I2C3_Pos                     (18UL)                  
#define SYSTEM_APBGATING0__I2C3_Msk                     BIT(SYSTEM_APBGATING0__I2C3_Pos)             
#define SYSTEM_APBGATING0__I2C2_Pos                     (19UL)                  
#define SYSTEM_APBGATING0__I2C2_Msk                     BIT(SYSTEM_APBGATING0__I2C2_Pos)             
#define SYSTEM_APBGATING0__I2C1_Pos                     (20UL)                  
#define SYSTEM_APBGATING0__I2C1_Msk                     BIT(SYSTEM_APBGATING0__I2C1_Pos)            
#define SYSTEM_APBGATING0__GPIO_Pos                     (21UL)                  
#define SYSTEM_APBGATING0__GPIO_Msk                     BIT(SYSTEM_APBGATING0__GPIO_Pos)            
#define SYSTEM_APBGATING0__CEC2_Pos                     (22UL)                  
#define SYSTEM_APBGATING0__CEC2_Msk                     BIT(SYSTEM_APBGATING0__CEC2_Pos)            
#define SYSTEM_APBGATING0__CEC1_Pos                     (23UL)                  
#define SYSTEM_APBGATING0__CEC1_Msk                     BIT(SYSTEM_APBGATING0__CEC1_Pos)            
#define SYSTEM_APBGATING0__ADC_Pos                      (24UL)                  
#define SYSTEM_APBGATING0__ADC_Msk                      BIT(SYSTEM_APBGATING0__ADC_Pos)           
#define SYSTEM_CLKGATING3__RTMR_Pos                     (25UL)                  
#define SYSTEM_CLKGATING3__RTMR_Msk                     BIT(SYSTEM_CLKGATING3__RTMR_Pos)           
#define SYSTEM_CLKGATING3__PS2_L3_Pos                   (26UL)                  
#define SYSTEM_CLKGATING3__PS2_L3_Msk                   BIT(SYSTEM_CLKGATING3__PS2_L3_Pos)           
#define SYSTEM_CLKGATING3__PS2_L2_Pos                   (27UL)                  
#define SYSTEM_CLKGATING3__PS2_L2_Msk                   BIT(SYSTEM_CLKGATING3__PS2_L2_Pos)           
#define SYSTEM_CLKGATING3__PS2_L1_Pos                   (28UL)                  
#define SYSTEM_CLKGATING3__PS2_L1_Msk                   BIT(SYSTEM_CLKGATING3__PS2_L1_Pos)          
#define SYSTEM_CLKGATING3_REF_32K_Pos                   (29UL)                  
#define SYSTEM_CLKGATING3_REF_32K_Msk                   BIT(SYSTEM_CLKGATING3_REF_32K_Pos)          
#define SYSTEM_CLKGATING3__PECI_Pos                     (30UL)                  
#define SYSTEM_CLKGATING3__PECI_Msk                     BIT(SYSTEM_CLKGATING3__PECI_Pos)          
#define SYSTEM_CLKGATING3__I3C3_Pos                     (31UL)                  
#define SYSTEM_CLKGATING3__I3C3_Msk                     BIT(SYSTEM_CLKGATING3__I3C3_Pos)          
#define SYSTEM_APBGATING1__TACH8_Pos                    (0UL)                   
#define SYSTEM_APBGATING1__TACH8_Msk                    BIT(SYSTEM_APBGATING1__TACH8_Pos)                 
#define SYSTEM_APBGATING1__TACH7_Pos                    (1UL)                   
#define SYSTEM_APBGATING1__TACH7_Msk                    BIT(SYSTEM_APBGATING1__TACH7_Pos)                 
#define SYSTEM_APBGATING1__TACH6_Pos                    (2UL)                   
#define SYSTEM_APBGATING1__TACH6_Msk                    BIT(SYSTEM_APBGATING1__TACH6_Pos)                 
#define SYSTEM_APBGATING1__TACH5_Pos                    (3UL)                   
#define SYSTEM_APBGATING1__TACH5_Msk                    BIT(SYSTEM_APBGATING1__TACH5_Pos)                 
#define SYSTEM_APBGATING1__TACH4_Pos                    (4UL)                   
#define SYSTEM_APBGATING1__TACH4_Msk                    BIT(SYSTEM_APBGATING1__TACH4_Pos)                
#define SYSTEM_APBGATING1__TACH3_Pos                    (5UL)                   
#define SYSTEM_APBGATING1__TACH3_Msk                    BIT(SYSTEM_APBGATING1__TACH3_Pos)                
#define SYSTEM_APBGATING1__TACH2_Pos                    (6UL)                   
#define SYSTEM_APBGATING1__TACH2_Msk                    BIT(SYSTEM_APBGATING1__TACH2_Pos)                
#define SYSTEM_APBGATING1__TACH1_Pos                    (7UL)                   
#define SYSTEM_APBGATING1__TACH1_Msk                    BIT(SYSTEM_APBGATING1__TACH1_Pos)                
#define SYSTEM_APBGATING1__SPIM_Pos                     (8UL)                   
#define SYSTEM_APBGATING1__SPIM_Msk                     BIT(SYSTEM_APBGATING1__SPIM_Pos)               
#define SYSTEM_APBGATING1__SPIS_Pos                     (9UL)                   
#define SYSTEM_APBGATING1__SPIS_Msk                     BIT(SYSTEM_APBGATING1__SPIS_Pos)               
#define SYSTEM_APBGATING1__SLWTMR3_Pos                  (10UL)                  
#define SYSTEM_APBGATING1__SLWTMR3_Msk                  BIT(SYSTEM_APBGATING1__SLWTMR3_Pos)               
#define SYSTEM_APBGATING1__SLWTMR2_Pos                  (11UL)                  
#define SYSTEM_APBGATING1__SLWTMR2_Msk                  BIT(SYSTEM_APBGATING1__SLWTMR2_Pos)               
#define SYSTEM_APBGATING1__SLWTMR1_Pos                  (12UL)                  
#define SYSTEM_APBGATING1__SLWTMR1_Msk                  BIT(SYSTEM_APBGATING1__SLWTMR1_Pos)              
#define SYSTEM_APBGATING1__SLWTMR0_Pos                  (13UL)                  
#define SYSTEM_APBGATING1__SLWTMR0_Msk                  BIT(SYSTEM_APBGATING1__SLWTMR0_Pos)              
#define SYSTEM_APBGATING1__RTMR_Pos                     (14UL)                  
#define SYSTEM_APBGATING1__RTMR_Msk                     BIT(SYSTEM_APBGATING1__RTMR_Pos)              
#define SYSTEM_APBGATING1__RTC_Pos                      (15UL)                  
#define SYSTEM_APBGATING1__RTC_Msk                      BIT(SYSTEM_APBGATING1__RTC_Pos)              
#define SYSTEM_APBGATING1__PWRBTN1_Pos                  (16UL)                  
#define SYSTEM_APBGATING1__PWRBTN1_Msk                  BIT(SYSTEM_APBGATING1__PWRBTN1_Pos)             
#define SYSTEM_APBGATING1__PWRBTN0_Pos                  (17UL)                  
#define SYSTEM_APBGATING1__PWRBTN0_Msk                  BIT(SYSTEM_APBGATING1__PWRBTN0_Pos)             
#define SYSTEM_APBGATING1__PWM14_Pos                    (18UL)                  
#define SYSTEM_APBGATING1__PWM14_Msk                    BIT(SYSTEM_APBGATING1__PWM14_Pos)             
#define SYSTEM_APBGATING1__PWM13_Pos                    (19UL)                  
#define SYSTEM_APBGATING1__PWM13_Msk                    BIT(SYSTEM_APBGATING1__PWM13_Pos)             
#define SYSTEM_APBGATING1__PWM12_Pos                    (20UL)                  
#define SYSTEM_APBGATING1__PWM12_Msk                    BIT(SYSTEM_APBGATING1__PWM12_Pos)            
#define SYSTEM_APBGATING1__PWM11_Pos                    (21UL)                  
#define SYSTEM_APBGATING1__PWM11_Msk                    BIT(SYSTEM_APBGATING1__PWM11_Pos)            
#define SYSTEM_APBGATING1__PWM10_Pos                    (22UL)                  
#define SYSTEM_APBGATING1__PWM10_Msk                    BIT(SYSTEM_APBGATING1__PWM10_Pos)            
#define SYSTEM_APBGATING1__PWM9_Pos                     (23UL)                  
#define SYSTEM_APBGATING1__PWM9_Msk                     BIT(SYSTEM_APBGATING1__PWM9_Pos)            
#define SYSTEM_APBGATING1__PWM8_Pos                     (24UL)                  
#define SYSTEM_APBGATING1__PWM8_Msk                     BIT(SYSTEM_APBGATING1__PWM8_Pos)           
#define SYSTEM_APBGATING1__PWM7_Pos                     (25UL)                  
#define SYSTEM_APBGATING1__PWM7_Msk                     BIT(SYSTEM_APBGATING1__PWM7_Pos)           
#define SYSTEM_APBGATING1__PWM6_Pos                     (26UL)                  
#define SYSTEM_APBGATING1__PWM6_Msk                     BIT(SYSTEM_APBGATING1__PWM6_Pos)           
#define SYSTEM_APBGATING1__PWM5_Pos                     (27UL)                  
#define SYSTEM_APBGATING1__PWM5_Msk                     BIT(SYSTEM_APBGATING1__PWM5_Pos)           
#define SYSTEM_APBGATING1__PWM4_Pos                     (28UL)                  
#define SYSTEM_APBGATING1__PWM4_Msk                     BIT(SYSTEM_APBGATING1__PWM4_Pos)          
#define SYSTEM_APBGATING1__PWM3_Pos                     (29UL)                  
#define SYSTEM_APBGATING1__PWM3_Msk                     BIT(SYSTEM_APBGATING1__PWM3_Pos)          
#define SYSTEM_APBGATING1__PWM2_Pos                     (30UL)                  
#define SYSTEM_APBGATING1__PWM2_Msk                     BIT(SYSTEM_APBGATING1__PWM2_Pos)          
#define SYSTEM_APBGATING1__PWM1_Pos                     (31UL)                  
#define SYSTEM_APBGATING1__PWM1_Msk                     BIT(SYSTEM_APBGATING1__PWM1_Pos)          
#define SYSTEM_CLKGATING4__USBDHY_Pos                   (5UL)                   
#define SYSTEM_CLKGATING4__USBDHY_Msk                   BIT(SYSTEM_CLKGATING4__USBDHY_Pos)                
#define SYSTEM_CLKGATING4__USB_Pos                      (6UL)                   
#define SYSTEM_CLKGATING4__USB_Msk                      BIT(SYSTEM_CLKGATING4__USB_Pos)                
#define SYSTEM_CLKGATING4__TRC_Pos                      (7UL)                   
#define SYSTEM_CLKGATING4__TRC_Msk                      BIT(SYSTEM_CLKGATING4__TRC_Pos)                
#define SYSTEM_CLKGATING4__LALU_Pos                     (8UL)                   
#define SYSTEM_CLKGATING4__LALU_Msk                     BIT(SYSTEM_CLKGATING4__LALU_Pos)               
#define SYSTEM_APBGATING2__TRC_Pos                      (9UL)                   
#define SYSTEM_APBGATING2__TRC_Msk                      BIT(SYSTEM_APBGATING2__TRC_Pos)               
#define SYSTEM_APBGATING2__PECI_Pos                     (10UL)                  
#define SYSTEM_APBGATING2__PECI_Msk                     BIT(SYSTEM_APBGATING2__PECI_Pos)               
#define SYSTEM_APBGATING2__LPT_Pos                      (11UL)                  
#define SYSTEM_APBGATING2__LPT_Msk                      BIT(SYSTEM_APBGATING2__LPT_Pos)               
#define SYSTEM_APBGATING2__WDT_Pos                      (12UL)                  
#define SYSTEM_APBGATING2__WDT_Msk                      BIT(SYSTEM_APBGATING2__WDT_Pos)              
#define SYSTEM_APBGATING2__UART2_Pos                    (13UL)                  
#define SYSTEM_APBGATING2__UART2_Msk                    BIT(SYSTEM_APBGATING2__UART2_Pos)              
#define SYSTEM_APBGATING2__UART1_Pos                    (14UL)                  
#define SYSTEM_APBGATING2__UART1_Msk                    BIT(SYSTEM_APBGATING2__UART1_Pos)              
#define SYSTEM_APBGATING2__UART0_Pos                    (15UL)                  
#define SYSTEM_APBGATING2__UART0_Msk                    BIT(SYSTEM_APBGATING2__UART0_Pos)              
#define SYSTEM_APBGATING2__TMR5_Pos                     (16UL)                  
#define SYSTEM_APBGATING2__TMR5_Msk                     BIT(SYSTEM_APBGATING2__TMR5_Pos)             
#define SYSTEM_APBGATING2__TMR4_Pos                     (17UL)                  
#define SYSTEM_APBGATING2__TMR4_Msk                     BIT(SYSTEM_APBGATING2__TMR4_Pos)             
#define SYSTEM_APBGATING2__TMR3_Pos                     (18UL)                  
#define SYSTEM_APBGATING2__TMR3_Msk                     BIT(SYSTEM_APBGATING2__TMR3_Pos)             
#define SYSTEM_APBGATING2__TMR2_Pos                     (19UL)                  
#define SYSTEM_APBGATING2__TMR2_Msk                     BIT(SYSTEM_APBGATING2__TMR2_Pos)             
#define SYSTEM_APBGATING2__TMR1_Pos                     (20UL)                  
#define SYSTEM_APBGATING2__TMR1_Msk                     BIT(SYSTEM_APBGATING2__TMR1_Pos)            
#define SYSTEM_APBGATING2__TMR0_Pos                     (21UL)                  
#define SYSTEM_APBGATING2__TMR0_Msk                     BIT(SYSTEM_APBGATING2__TMR0_Pos)            
#define SYSTEM_APBGATING2__TACH14_Pos                   (22UL)                  
#define SYSTEM_APBGATING2__TACH14_Msk                   BIT(SYSTEM_APBGATING2__TACH14_Pos)            
#define SYSTEM_APBGATING2__TACH13_Pos                   (23UL)                  
#define SYSTEM_APBGATING2__TACH13_Msk                   BIT(SYSTEM_APBGATING2__TACH13_Pos)            
#define SYSTEM_APBGATING2__TACH12_Pos                   (24UL)                  
#define SYSTEM_APBGATING2__TACH12_Msk                   BIT(SYSTEM_APBGATING2__TACH12_Pos)           
#define SYSTEM_APBGATING2__TACH11_Pos                   (25UL)                  
#define SYSTEM_APBGATING2__TACH11_Msk                   BIT(SYSTEM_APBGATING2__TACH11_Pos)           
#define SYSTEM_APBGATING2__TACH10_Pos                   (26UL)                  
#define SYSTEM_APBGATING2__TACH10_Msk                   BIT(SYSTEM_APBGATING2__TACH10_Pos)           
#define SYSTEM_APBGATING2__TACH9_Pos                    (27UL)                  
#define SYSTEM_APBGATING2__TACH9_Msk                    BIT(SYSTEM_APBGATING2__TACH9_Pos)           
#define SYSTEM_IPRST0__PWM8_Pos                         (0UL)                   
#define SYSTEM_IPRST0__PWM8_Msk                         BIT(SYSTEM_IPRST0__PWM8_Pos)                 
#define SYSTEM_IPRST0__PWM7_Pos                         (1UL)                   
#define SYSTEM_IPRST0__PWM7_Msk                         BIT(SYSTEM_IPRST0__PWM7_Pos)                 
#define SYSTEM_IPRST0__PWM6_Pos                         (2UL)                   
#define SYSTEM_IPRST0__PWM6_Msk                         BIT(SYSTEM_IPRST0__PWM6_Pos)                 
#define SYSTEM_IPRST0__PWM5_Pos                         (3UL)                   
#define SYSTEM_IPRST0__PWM5_Msk                         BIT(SYSTEM_IPRST0__PWM5_Pos)                 
#define SYSTEM_IPRST0__PWM4_Pos                         (4UL)                   
#define SYSTEM_IPRST0__PWM4_Msk                         BIT(SYSTEM_IPRST0__PWM4_Pos)                
#define SYSTEM_IPRST0__PWM3_Pos                         (5UL)                   
#define SYSTEM_IPRST0__PWM3_Msk                         BIT(SYSTEM_IPRST0__PWM3_Pos)                
#define SYSTEM_IPRST0__PWM2_Pos                         (6UL)                   
#define SYSTEM_IPRST0__PWM2_Msk                         BIT(SYSTEM_IPRST0__PWM2_Pos)                
#define SYSTEM_IPRST0__PWM1_Pos                         (7UL)                   
#define SYSTEM_IPRST0__PWM1_Msk                         BIT(SYSTEM_IPRST0__PWM1_Pos)                
#define SYSTEM_IPRST0__LED4_Pos                         (8UL)                   
#define SYSTEM_IPRST0__LED4_Msk                         BIT(SYSTEM_IPRST0__LED4_Pos)               
#define SYSTEM_IPRST0__LED3_Pos                         (9UL)                   
#define SYSTEM_IPRST0__LED3_Msk                         BIT(SYSTEM_IPRST0__LED3_Pos)               
#define SYSTEM_IPRST0__LED2_Pos                         (10UL)                  
#define SYSTEM_IPRST0__LED2_Msk                         BIT(SYSTEM_IPRST0__LED2_Pos)               
#define SYSTEM_IPRST0__LED1_Pos                         (11UL)                  
#define SYSTEM_IPRST0__LED1_Msk                         BIT(SYSTEM_IPRST0__LED1_Pos)               
#define SYSTEM_IPRST0__EMI7_Pos                         (12UL)                  
#define SYSTEM_IPRST0__EMI7_Msk                         BIT(SYSTEM_IPRST0__EMI7_Pos)              
#define SYSTEM_IPRST0__EMI6_Pos                         (13UL)                  
#define SYSTEM_IPRST0__EMI6_Msk                         BIT(SYSTEM_IPRST0__EMI6_Pos)              
#define SYSTEM_IPRST0__EMI5_Pos                         (14UL)                  
#define SYSTEM_IPRST0__EMI5_Msk                         BIT(SYSTEM_IPRST0__EMI5_Pos)              
#define SYSTEM_IPRST0__EMI4_Pos                         (15UL)                  
#define SYSTEM_IPRST0__EMI4_Msk                         BIT(SYSTEM_IPRST0__EMI4_Pos)              
#define SYSTEM_IPRST0__EMI3_Pos                         (16UL)                  
#define SYSTEM_IPRST0__EMI3_Msk                         BIT(SYSTEM_IPRST0__EMI3_Pos)             
#define SYSTEM_IPRST0__EMI2_Pos                         (17UL)                  
#define SYSTEM_IPRST0__EMI2_Msk                         BIT(SYSTEM_IPRST0__EMI2_Pos)             
#define SYSTEM_IPRST0__EMI1_Pos                         (18UL)                  
#define SYSTEM_IPRST0__EMI1_Msk                         BIT(SYSTEM_IPRST0__EMI1_Pos)             
#define SYSTEM_IPRST0__EMI0_Pos                         (19UL)                  
#define SYSTEM_IPRST0__EMI0_Msk                         BIT(SYSTEM_IPRST0__EMI0_Pos)             
#define SYSTEM_IPRST0__PORT80_Pos                       (20UL)                  
#define SYSTEM_IPRST0__PORT80_Msk                       BIT(SYSTEM_IPRST0__PORT80_Pos)            
#define SYSTEM_IPRST0__PMPORT3_Pos                      (21UL)                  
#define SYSTEM_IPRST0__PMPORT3_Msk                      BIT(SYSTEM_IPRST0__PMPORT3_Pos)            
#define SYSTEM_IPRST0__PMPORT2_Pos                      (22UL)                  
#define SYSTEM_IPRST0__PMPORT2_Msk                      BIT(SYSTEM_IPRST0__PMPORT2_Pos)            
#define SYSTEM_IPRST0__PMPORT1_Pos                      (23UL)                  
#define SYSTEM_IPRST0__PMPORT1_Msk                      BIT(SYSTEM_IPRST0__PMPORT1_Pos)            
#define SYSTEM_IPRST0__PMPORT0_Pos                      (24UL)                  
#define SYSTEM_IPRST0__PMPORT0_Msk                      BIT(SYSTEM_IPRST0__PMPORT0_Pos)           
#define SYSTEM_IPRST0__ACPI_Pos                         (25UL)                  
#define SYSTEM_IPRST0__ACPI_Msk                         BIT(SYSTEM_IPRST0__ACPI_Pos)           
#define SYSTEM_IPRST0__KBC_Pos                          (26UL)                  
#define SYSTEM_IPRST0__KBC_Msk                          BIT(SYSTEM_IPRST0__KBC_Pos)           
#define SYSTEM_IPRST0__ESPI_Pos                         (27UL)                  
#define SYSTEM_IPRST0__ESPI_Msk                         BIT(SYSTEM_IPRST0__ESPI_Pos)           
#define SYSTEM_IPRST0__DMA_Pos                          (28UL)                  
#define SYSTEM_IPRST0__DMA_Msk                          SYSTEM_IPRST0__DMA_Pos)          
#define SYSTEM_IPRST0_PUF_OTP_Pos                       (29UL)                  
#define SYSTEM_IPRST0_PUF_OTP_Msk                       BIT(SYSTEM_IPRST0_PUF_OTP_Pos)          
#define SYSTEM_IPRST0__BUS_Pos                          (31UL)                  
#define SYSTEM_IPRST0__BUS_Msk                          BIT(SYSTEM_IPRST0__BUS_Pos)          
#define SYSTEM_IPRST1__TACH5_Pos                        (0UL)                   
#define SYSTEM_IPRST1__TACH5_Msk                        BIT(SYSTEM_IPRST1__TACH5_Pos)                 
#define SYSTEM_IPRST1__TACH4_Pos                        (1UL)                   
#define SYSTEM_IPRST1__TACH4_Msk                        BIT(SYSTEM_IPRST1__TACH4_Pos)                 
#define SYSTEM_IPRST1__TACH3_Pos                        (2UL)                   
#define SYSTEM_IPRST1__TACH3_Msk                        BIT(SYSTEM_IPRST1__TACH3_Pos)                 
#define SYSTEM_IPRST1__TACH2_Pos                        (3UL)                   
#define SYSTEM_IPRST1__TACH2_Msk                        BIT(SYSTEM_IPRST1__TACH2_Pos)                 
#define SYSTEM_IPRST1__TACH1_Pos                        (4UL)                   
#define SYSTEM_IPRST1__TACH1_Msk                        BIT(SYSTEM_IPRST1__TACH1_Pos)                
#define SYSTEM_IPRST1__PS2_H3_Pos                       (5UL)                   
#define SYSTEM_IPRST1__PS2_H3_Msk                       BIT(SYSTEM_IPRST1__PS2_H3_Pos)                
#define SYSTEM_IPRST1__PS2_H2_Pos                       (6UL)                   
#define SYSTEM_IPRST1__PS2_H2_Msk                       BIT(SYSTEM_IPRST1__PS2_H2_Pos)                
#define SYSTEM_IPRST1__PS2_H1_Pos                       (7UL)                   
#define SYSTEM_IPRST1__PS2_H1_Msk                       BIT(SYSTEM_IPRST1__PS2_H1_Pos)                
#define SYSTEM_IPRST1_RST_Pos                           (8UL)                   
#define SYSTEM_IPRST1_RST_Msk                           BIT(SYSTEM_IPRST1_RST_Pos)               
#define SYSTEM_IPRST1_RST25M_Pos                        (9UL)                   
#define SYSTEM_IPRST1_RST25M_Msk                        BIT(SYSTEM_IPRST1_RST25M_Pos)               
#define SYSTEM_IPRST1_RST24M_Pos                        (10UL)                  
#define SYSTEM_IPRST1_RST24M_Msk                        BIT(SYSTEM_IPRST1_RST24M_Pos)               
#define SYSTEM_IPRST1__I2C11_Pos                        (11UL)                  
#define SYSTEM_IPRST1__I2C11_Msk                        BIT(SYSTEM_IPRST1__I2C11_Pos)               
#define SYSTEM_IPRST1__I2C10_Pos                        (12UL)                  
#define SYSTEM_IPRST1__I2C10_Msk                        BIT(SYSTEM_IPRST1__I2C10_Pos)              
#define SYSTEM_IPRST1__I2C9_Pos                         (13UL)                  
#define SYSTEM_IPRST1__I2C9_Msk                         BIT(SYSTEM_IPRST1__I2C9_Pos)              
#define SYSTEM_IPRST1__I2C8_Pos                         (14UL)                  
#define SYSTEM_IPRST1__I2C8_Msk                         BIT(SYSTEM_IPRST1__I2C8_Pos)              
#define SYSTEM_IPRST1__I2C7_Pos                         (15UL)                  
#define SYSTEM_IPRST1__I2C7_Msk                         BIT(SYSTEM_IPRST1__I2C7_Pos)              
#define SYSTEM_IPRST1__I2C6_Pos                         (16UL)                  
#define SYSTEM_IPRST1__I2C6_Msk                         BIT(SYSTEM_IPRST1__I2C6_Pos)             
#define SYSTEM_IPRST1__I2C5_Pos                         (17UL)                  
#define SYSTEM_IPRST1__I2C5_Msk                         BIT(SYSTEM_IPRST1__I2C5_Pos)             
#define SYSTEM_IPRST1__I2C4_Pos                         (18UL)                  
#define SYSTEM_IPRST1__I2C4_Msk                         BIT(SYSTEM_IPRST1__I2C4_Pos)             
#define SYSTEM_IPRST1__I2C3_Pos                         (19UL)                  
#define SYSTEM_IPRST1__I2C3_Msk                         BIT(SYSTEM_IPRST1__I2C3_Pos)             
#define SYSTEM_IPRST1__I2C2_Pos                         (20UL)                  
#define SYSTEM_IPRST1__I2C2_Msk                         BIT(SYSTEM_IPRST1__I2C2_Pos)            
#define SYSTEM_IPRST1__I2C1_Pos                         (21UL)                  
#define SYSTEM_IPRST1__I2C1_Msk                         BIT(SYSTEM_IPRST1__I2C1_Pos)            
#define SYSTEM_IPRST1__UART2_Pos                        (22UL)                  
#define SYSTEM_IPRST1__UART2_Msk                        BIT(SYSTEM_IPRST1__UART2_Pos)            
#define SYSTEM_IPRST1__UART1_Pos                        (23UL)                  
#define SYSTEM_IPRST1__UART1_Msk                        BIT(SYSTEM_IPRST1__UART1_Pos)            
#define SYSTEM_IPRST1__UART0_Pos                        (24UL)                  
#define SYSTEM_IPRST1__UART0_Msk                        BIT(SYSTEM_IPRST1__UART0_Pos)           
#define SYSTEM_IPRST1__ADC_Pos                          (25UL)                  
#define SYSTEM_IPRST1__ADC_Msk                          BIT(SYSTEM_IPRST1__ADC_Pos)           
#define SYSTEM_IPRST1__PWM14_Pos                        (26UL)                  
#define SYSTEM_IPRST1__PWM14_Msk                        BIT(SYSTEM_IPRST1__PWM14_Pos)           
#define SYSTEM_IPRST1__PWM13_Pos                        (27UL)                  
#define SYSTEM_IPRST1__PWM13_Msk                        BIT(SYSTEM_IPRST1__PWM13_Pos)           
#define SYSTEM_IPRST1__PWM12_Pos                        (28UL)                  
#define SYSTEM_IPRST1__PWM12_Msk                        BIT(SYSTEM_IPRST1__PWM12_Pos)          
#define SYSTEM_IPRST1__PWM11_Pos                        (29UL)                  
#define SYSTEM_IPRST1__PWM11_Msk                        BIT(SYSTEM_IPRST1__PWM11_Pos)          
#define SYSTEM_IPRST1__PWM10_Pos                        (30UL)                  
#define SYSTEM_IPRST1__PWM10_Msk                        BIT(SYSTEM_IPRST1__PWM10_Pos)          
#define SYSTEM_IPRST1__PWM9_Pos                         (31UL)                  
#define SYSTEM_IPRST1__PWM9_Msk                         BIT(SYSTEM_IPRST1__PWM9_Pos)          
#define SYSTEM_IPRST2__I3C2_Pos                         (0UL)                   
#define SYSTEM_IPRST2__I3C2_Msk                         BIT(SYSTEM_IPRST2__I3C2_Pos)                 
#define SYSTEM_IPRST2__I3C1_Pos                         (1UL)                   
#define SYSTEM_IPRST2__I3C1_Msk                         BIT(SYSTEM_IPRST2__I3C1_Pos)                 
#define SYSTEM_IPRST2__I3C4_Pos                         (2UL)                   
#define SYSTEM_IPRST2__I3C4_Msk                         BIT(SYSTEM_IPRST2__I3C4_Pos)                 
#define SYSTEM_IPRST2__LPT_Pos                          (3UL)                   
#define SYSTEM_IPRST2__LPT_Msk                          BIT(SYSTEM_IPRST2__LPT_Pos)                 
#define SYSTEM_IPRST2__VINLED_Pos                       (4UL)                   
#define SYSTEM_IPRST2__VINLED_Msk                       BIT(SYSTEM_IPRST2__VINLED_Pos)                
#define SYSTEM_IPRST2__I2CDBG_Pos                       (5UL)                   
#define SYSTEM_IPRST2__I2CDBG_Msk                       BIT(SYSTEM_IPRST2__I2CDBG_Pos)                
#define SYSTEM_IPRST2__MICROCNT_Pos                     (6UL)                   
#define SYSTEM_IPRST2__MICROCNT_Msk                     BIT(SYSTEM_IPRST2__MICROCNT_Pos)                
#define SYSTEM_IPRST2__CEC2_Pos                         (7UL)                   
#define SYSTEM_IPRST2__CEC2_Msk                         BIT(SYSTEM_IPRST2__CEC2_Pos)                
#define SYSTEM_IPRST2__CEC1_Pos                         (8UL)                   
#define SYSTEM_IPRST2__CEC1_Msk                         BIT(SYSTEM_IPRST2__CEC1_Pos)               
#define SYSTEM_IPRST2__LEDSTP_Pos                       (9UL)                   
#define SYSTEM_IPRST2__LEDSTP_Msk                       BIT(SYSTEM_IPRST2__LEDSTP_Pos)               
#define SYSTEM_IPRST2__SLWTMR3_Pos                      (10UL)                  
#define SYSTEM_IPRST2__SLWTMR3_Msk                      BIT(SYSTEM_IPRST2__SLWTMR3_Pos)               
#define SYSTEM_IPRST2__SLWTMR2_Pos                      (11UL)                  
#define SYSTEM_IPRST2__SLWTMR2_Msk                      BIT(SYSTEM_IPRST2__SLWTMR2_Pos)               
#define SYSTEM_IPRST2__SLWTMR1_Pos                      (12UL)                  
#define SYSTEM_IPRST2__SLWTMR1_Msk                      BIT(SYSTEM_IPRST2__SLWTMR1_Pos)              
#define SYSTEM_IPRST2__SLWTMR0_Pos                      (13UL)                  
#define SYSTEM_IPRST2__SLWTMR0_Msk                      BIT(SYSTEM_IPRST2__SLWTMR0_Pos)              
#define SYSTEM_IPRST2__KBMSCAN_Pos                      (14UL)                  
#define SYSTEM_IPRST2__KBMSCAN_Msk                      BIT(SYSTEM_IPRST2__KBMSCAN_Pos)              
#define SYSTEM_IPRST2__KBM_Pos                          (15UL)                  
#define SYSTEM_IPRST2__KBM_Msk                          BIT(SYSTEM_IPRST2__KBM_Pos)              
#define SYSTEM_IPRST2__TMR5_Pos                         (16UL)                  
#define SYSTEM_IPRST2__TMR5_Msk                         BIT(SYSTEM_IPRST2__TMR5_Pos)             
#define SYSTEM_IPRST2__TMR4_Pos                         (17UL)                  
#define SYSTEM_IPRST2__TMR4_Msk                         BIT(SYSTEM_IPRST2__TMR4_Pos)             
#define SYSTEM_IPRST2__TMR3_Pos                         (18UL)                  
#define SYSTEM_IPRST2__TMR3_Msk                         BIT(SYSTEM_IPRST2__TMR3_Pos)             
#define SYSTEM_IPRST2__TMR2_Pos                         (19UL)                  
#define SYSTEM_IPRST2__TMR2_Msk                         SYSTEM_IPRST2__TMR2_Pos)             
#define SYSTEM_IPRST2__TMR1_Pos                         (20UL)                  
#define SYSTEM_IPRST2__TMR1_Msk                         BIT(SYSTEM_IPRST2__TMR1_Pos)            
#define SYSTEM_IPRST2__TMR0_Pos                         (21UL)                  
#define SYSTEM_IPRST2__TMR0_Msk                         BIT(SYSTEM_IPRST2__TMR0_Pos)            
#define SYSTEM_IPRST2__GPIO_Pos                         (22UL)                  
#define SYSTEM_IPRST2__GPIO_Msk                         BIT(SYSTEM_IPRST2__GPIO_Pos)            
#define SYSTEM_IPRST2__TACH14_Pos                       (23UL)                  
#define SYSTEM_IPRST2__TACH14_Msk                       BIT(SYSTEM_IPRST2__TACH14_Pos)            
#define SYSTEM_IPRST2__TACH13_Pos                       (24UL)                  
#define SYSTEM_IPRST2__TACH13_Msk                       BIT(SYSTEM_IPRST2__TACH13_Pos)           
#define SYSTEM_IPRST2__TACH12_Pos                       (25UL)                  
#define SYSTEM_IPRST2__TACH12_Msk                       BIT(SYSTEM_IPRST2__TACH12_Pos)           
#define SYSTEM_IPRST2__TACH11_Pos                       (26UL)                  
#define SYSTEM_IPRST2__TACH11_Msk                       BIT(SYSTEM_IPRST2__TACH11_Pos)           
#define SYSTEM_IPRST2__TACH10_Pos                       (27UL)                  
#define SYSTEM_IPRST2__TACH10_Msk                       BIT(SYSTEM_IPRST2__TACH10_Pos)           
#define SYSTEM_IPRST2__TACH9_Pos                        (28UL)                  
#define SYSTEM_IPRST2__TACH9_Msk                        BIT(SYSTEM_IPRST2__TACH9_Pos)          
#define SYSTEM_IPRST2__TACH8_Pos                        (29UL)                  
#define SYSTEM_IPRST2__TACH8_Msk                        BIT(SYSTEM_IPRST2__TACH8_Pos)          
#define SYSTEM_IPRST2__TACH7_Pos                        (30UL)                  
#define SYSTEM_IPRST2__TACH7_Msk                        BIT(SYSTEM_IPRST2__TACH7_Pos)          
#define SYSTEM_IPRST2__TACH6_Pos                        (31UL)                  
#define SYSTEM_IPRST2__TACH6_Msk                        BIT(SYSTEM_IPRST2__TACH6_Pos)          
#define SYSTEM_APBRST0__PS2_3_Pos                       (0UL)                   
#define SYSTEM_APBRST0__PS2_3_Msk                       BIT(SYSTEM_APBRST0__PS2_3_Pos)                 
#define SYSTEM_APBRST0__PS2_2_Pos                       (1UL)                   
#define SYSTEM_APBRST0__PS2_2_Msk                       BIT(SYSTEM_APBRST0__PS2_2_Pos)                 
#define SYSTEM_APBRST0__PS2_1_Pos                       (2UL)                   
#define SYSTEM_APBRST0__PS2_1_Msk                       BIT(SYSTEM_APBRST0__PS2_1_Pos)                 
#define SYSTEM_APBRST0__LEDSTP_Pos                      (3UL)                   
#define SYSTEM_APBRST0__LEDSTP_Msk                      BIT(SYSTEM_APBRST0__LEDSTP_Pos)                 
#define SYSTEM_APBRST0__LED4_Pos                        (4UL)                   
#define SYSTEM_APBRST0__LED4_Msk                        BIT(SYSTEM_APBRST0__LED4_Pos)                
#define SYSTEM_APBRST0__LED3_Pos                        (5UL)                   
#define SYSTEM_APBRST0__LED3_Msk                        BIT(SYSTEM_APBRST0__LED3_Pos)                
#define SYSTEM_APBRST0__LED2_Pos                        (6UL)                   
#define SYSTEM_APBRST0__LED2_Msk                        BIT(SYSTEM_APBRST0__LED2_Pos)                
#define SYSTEM_APBRST0__LED1_Pos                        (7UL)                   
#define SYSTEM_APBRST0__LED1_Msk                        BIT(SYSTEM_APBRST0__LED1_Pos)                
#define SYSTEM_APBRST0__KBM_Pos                         (8UL)                   
#define SYSTEM_APBRST0__KBM_Msk                         BIT(SYSTEM_APBRST0__KBM_Pos)               
#define SYSTEM_APBRST0__I2CDBG_Pos                      (9UL)                   
#define SYSTEM_APBRST0__I2CDBG_Msk                      BIT(SYSTEM_APBRST0__I2CDBG_Pos)               
#define SYSTEM_APBRST0__I2C11_Pos                       (10UL)                  
#define SYSTEM_APBRST0__I2C11_Msk                       BIT(SYSTEM_APBRST0__I2C11_Pos)               
#define SYSTEM_APBRST0__I2C10_Pos                       (11UL)                  
#define SYSTEM_APBRST0__I2C10_Msk                       BIT(SYSTEM_APBRST0__I2C10_Pos)               
#define SYSTEM_APBRST0__I2C9_Pos                        (12UL)                  
#define SYSTEM_APBRST0__I2C9_Msk                        BIT(SYSTEM_APBRST0__I2C9_Pos)              
#define SYSTEM_APBRST0__I2C8_Pos                        (13UL)                  
#define SYSTEM_APBRST0__I2C8_Msk                        BIT(SYSTEM_APBRST0__I2C8_Pos)              
#define SYSTEM_APBRST0__I2C7_Pos                        (14UL)                  
#define SYSTEM_APBRST0__I2C7_Msk                        BIT(SYSTEM_APBRST0__I2C7_Pos)              
#define SYSTEM_APBRST0__I2C6_Pos                        (15UL)                  
#define SYSTEM_APBRST0__I2C6_Msk                        BIT(SYSTEM_APBRST0__I2C6_Pos)              
#define SYSTEM_APBRST0__I2C5_Pos                        (16UL)                  
#define SYSTEM_APBRST0__I2C5_Msk                        BIT(SYSTEM_APBRST0__I2C5_Pos)             
#define SYSTEM_APBRST0__I2C4_Pos                        (17UL)                  
#define SYSTEM_APBRST0__I2C4_Msk                        BIT(SYSTEM_APBRST0__I2C4_Pos)             
#define SYSTEM_APBRST0__I2C3_Pos                        (18UL)                  
#define SYSTEM_APBRST0__I2C3_Msk                        BIT(SYSTEM_APBRST0__I2C3_Pos)             
#define SYSTEM_APBRST0__I2C2_Pos                        (19UL)                  
#define SYSTEM_APBRST0__I2C2_Msk                        BIT(SYSTEM_APBRST0__I2C2_Pos)             
#define SYSTEM_APBRST0__I2C1_Pos                        (20UL)                  
#define SYSTEM_APBRST0__I2C1_Msk                        BIT(SYSTEM_APBRST0__I2C1_Pos)            
#define SYSTEM_APBRST0__GPIO_Pos                        (21UL)                  
#define SYSTEM_APBRST0__GPIO_Msk                        BIT(SYSTEM_APBRST0__GPIO_Pos)            
#define SYSTEM_APBRST0__CEC2_Pos                        (22UL)                  
#define SYSTEM_APBRST0__CEC2_Msk                        BIT(SYSTEM_APBRST0__CEC2_Pos)            
#define SYSTEM_APBRST0__CEC1_Pos                        (23UL)                  
#define SYSTEM_APBRST0__CEC1_Msk                        BIT(SYSTEM_APBRST0__CEC1_Pos)            
#define SYSTEM_APBRST0__ADC_Pos                         (24UL)                  
#define SYSTEM_APBRST0__ADC_Msk                         BIT(SYSTEM_APBRST0__ADC_Pos)           
#define SYSTEM_IPRST3__RTMR_Pos                         (25UL)                  
#define SYSTEM_IPRST3__RTMR_Msk                         BIT(SYSTEM_IPRST3__RTMR_Pos)           
#define SYSTEM_IPRST3__PS2_L3_Pos                       (26UL)                  
#define SYSTEM_IPRST3__PS2_L3_Msk                       BIT(SYSTEM_IPRST3__PS2_L3_Pos)           
#define SYSTEM_IPRST3__PS2_L2_Pos                       (27UL)                  
#define SYSTEM_IPRST3__PS2_L2_Msk                       BIT(SYSTEM_IPRST3__PS2_L2_Pos)           
#define SYSTEM_IPRST3__PS2_L1_Pos                       (28UL)                  
#define SYSTEM_IPRST3__PS2_L1_Msk                       BIT(SYSTEM_IPRST3__PS2_L1_Pos)          
#define SYSTEM_IPRST3_REF32K_Pos                        (29UL)                  
#define SYSTEM_IPRST3_REF32K_Msk                        BIT(SYSTEM_IPRST3_REF32K_Pos)          
#define SYSTEM_IPRST3__PECI_Pos                         (30UL)                  
#define SYSTEM_IPRST3__PECI_Msk                         BIT(SYSTEM_IPRST3__PECI_Pos)          
#define SYSTEM_IPRST3__I3C3_Pos                         (31UL)                  
#define SYSTEM_IPRST3__I3C3_Msk                         BIT(SYSTEM_IPRST3__I3C3_Pos)          
#define SYSTEM_APBRST1__TACH8_Pos                       (0UL)                   
#define SYSTEM_APBRST1__TACH8_Msk                       BIT(SYSTEM_APBRST1__TACH8_Pos)                 
#define SYSTEM_APBRST1__TACH7_Pos                       (1UL)                   
#define SYSTEM_APBRST1__TACH7_Msk                       BIT(SYSTEM_APBRST1__TACH7_Pos)                 
#define SYSTEM_APBRST1__TACH6_Pos                       (2UL)                   
#define SYSTEM_APBRST1__TACH6_Msk                       BIT(SYSTEM_APBRST1__TACH6_Pos)                 
#define SYSTEM_APBRST1__TACH5_Pos                       (3UL)                   
#define SYSTEM_APBRST1__TACH5_Msk                       BIT(SYSTEM_APBRST1__TACH5_Pos)                 
#define SYSTEM_APBRST1__TACH4_Pos                       (4UL)                   
#define SYSTEM_APBRST1__TACH4_Msk                       BIT(SYSTEM_APBRST1__TACH4_Pos)                
#define SYSTEM_APBRST1__TACH3_Pos                       (5UL)                   
#define SYSTEM_APBRST1__TACH3_Msk                       BIT(SYSTEM_APBRST1__TACH3_Pos)                
#define SYSTEM_APBRST1__TACH2_Pos                       (6UL)                   
#define SYSTEM_APBRST1__TACH2_Msk                       BIT(SYSTEM_APBRST1__TACH2_Pos)                
#define SYSTEM_APBRST1__TACH1_Pos                       (7UL)                   
#define SYSTEM_APBRST1__TACH1_Msk                       BIT(SYSTEM_APBRST1__TACH1_Pos)                
#define SYSTEM_APBRST1__SPIM_Pos                        (8UL)                   
#define SYSTEM_APBRST1__SPIM_Msk                        BIT(SYSTEM_APBRST1__SPIM_Pos)               
#define SYSTEM_APBRST1__SPIS_Pos                        (9UL)                   
#define SYSTEM_APBRST1__SPIS_Msk                        BIT(SYSTEM_APBRST1__SPIS_Pos)               
#define SYSTEM_APBRST1__SLWTMR3_Pos                     (10UL)                  
#define SYSTEM_APBRST1__SLWTMR3_Msk                     BIT(SYSTEM_APBRST1__SLWTMR3_Pos)               
#define SYSTEM_APBRST1__SLWTMR2_Pos                     (11UL)                  
#define SYSTEM_APBRST1__SLWTMR2_Msk                     BIT(SYSTEM_APBRST1__SLWTMR2_Pos)               
#define SYSTEM_APBRST1__SLWTMR1_Pos                     (12UL)                  
#define SYSTEM_APBRST1__SLWTMR1_Msk                     BIT(SYSTEM_APBRST1__SLWTMR1_Pos)              
#define SYSTEM_APBRST1__SLWTMR0_Pos                     (13UL)                  
#define SYSTEM_APBRST1__SLWTMR0_Msk                     BIT(SYSTEM_APBRST1__SLWTMR0_Pos)              
#define SYSTEM_APBRST1__RTMR_Pos                        (14UL)                  
#define SYSTEM_APBRST1__RTMR_Msk                        BIT(SYSTEM_APBRST1__RTMR_Pos)              
#define SYSTEM_APBRST1__RTC_Pos                         (15UL)                  
#define SYSTEM_APBRST1__RTC_Msk                         BIT(SYSTEM_APBRST1__RTC_Pos)              
#define SYSTEM_APBRST1__PWM14_Pos                       (18UL)                  
#define SYSTEM_APBRST1__PWM14_Msk                       BIT(SYSTEM_APBRST1__PWM14_Pos)             
#define SYSTEM_APBRST1__PWM13_Pos                       (19UL)                  
#define SYSTEM_APBRST1__PWM13_Msk                       BIT(SYSTEM_APBRST1__PWM13_Pos)             
#define SYSTEM_APBRST1__PWM12_Pos                       (20UL)                  
#define SYSTEM_APBRST1__PWM12_Msk                       BIT(SYSTEM_APBRST1__PWM12_Pos)            
#define SYSTEM_APBRST1__PWM11_Pos                       (21UL)                  
#define SYSTEM_APBRST1__PWM11_Msk                       BIT(SYSTEM_APBRST1__PWM11_Pos)            
#define SYSTEM_APBRST1__PWM10_Pos                       (22UL)                  
#define SYSTEM_APBRST1__PWM10_Msk                       BIT(SYSTEM_APBRST1__PWM10_Pos)            
#define SYSTEM_APBRST1__PWM9_Pos                        (23UL)                  
#define SYSTEM_APBRST1__PWM9_Msk                        BIT(SYSTEM_APBRST1__PWM9_Pos)            
#define SYSTEM_APBRST1__PWM8_Pos                        (24UL)                  
#define SYSTEM_APBRST1__PWM8_Msk                        BIT(SYSTEM_APBRST1__PWM8_Pos)           
#define SYSTEM_APBRST1__PWM7_Pos                        (25UL)                  
#define SYSTEM_APBRST1__PWM7_Msk                        BIT(SYSTEM_APBRST1__PWM7_Pos)           
#define SYSTEM_APBRST1__PWM6_Pos                        (26UL)                  
#define SYSTEM_APBRST1__PWM6_Msk                        BIT(SYSTEM_APBRST1__PWM6_Pos)           
#define SYSTEM_APBRST1__PWM5_Pos                        (27UL)                  
#define SYSTEM_APBRST1__PWM5_Msk                        BIT(SYSTEM_APBRST1__PWM5_Pos)           
#define SYSTEM_APBRST1__PWM4_Pos                        (28UL)                  
#define SYSTEM_APBRST1__PWM4_Msk                        BIT(SYSTEM_APBRST1__PWM4_Pos)          
#define SYSTEM_APBRST1__PWM3_Pos                        (29UL)                  
#define SYSTEM_APBRST1__PWM3_Msk                        BIT(SYSTEM_APBRST1__PWM3_Pos)          
#define SYSTEM_APBRST1__PWM2_Pos                        (30UL)                  
#define SYSTEM_APBRST1__PWM2_Msk                        BIT(SYSTEM_APBRST1__PWM2_Pos)          
#define SYSTEM_APBRST1__PWM1_Pos                        (31UL)                  
#define SYSTEM_APBRST1__PWM1_Msk                        BIT(SYSTEM_APBRST1__PWM1_Pos)          
#define SYSTEM_IPRST4__USBDHY_Pos                       (0UL)                   
#define SYSTEM_IPRST4__USBDHY_Msk                       BIT(SYSTEM_IPRST4__USBDHY_Pos)                 
#define SYSTEM_IPRST4__USB_Pos                          (6UL)                   
#define SYSTEM_IPRST4__USB_Msk                          BIT(SYSTEM_IPRST4__USB_Pos)                
#define SYSTEM_IPRST4__TRC_Pos                          (7UL)                   
#define SYSTEM_IPRST4__TRC_Msk                          BIT(SYSTEM_IPRST4__TRC_Pos)                
#define SYSTEM_IPRST4__LALU_Pos                         (8UL)                   
#define SYSTEM_IPRST4__LALU_Msk                         BIT(SYSTEM_IPRST4__LALU_Pos)               
#define SYSTEM_APBRST2__TRC_Pos                         (9UL)                   
#define SYSTEM_APBRST2__TRC_Msk                         BIT(SYSTEM_APBRST2__TRC_Pos)               
#define SYSTEM_APBRST2__PECI_Pos                        (10UL)                  
#define SYSTEM_APBRST2__PECI_Msk                        BIT(SYSTEM_APBRST2__PECI_Pos)               
#define SYSTEM_APBRST2__LPT_Pos                         (11UL)                  
#define SYSTEM_APBRST2__LPT_Msk                         BIT(SYSTEM_APBRST2__LPT_Pos)               
#define SYSTEM_APBRST2__UART2_Pos                       (13UL)                  
#define SYSTEM_APBRST2__UART2_Msk                       BIT(SYSTEM_APBRST2__UART2_Pos)              
#define SYSTEM_APBRST2__UART1_Pos                       (14UL)                  
#define SYSTEM_APBRST2__UART1_Msk                       BIT(SYSTEM_APBRST2__UART1_Pos)              
#define SYSTEM_APBRST2__UART0_Pos                       (15UL)                  
#define SYSTEM_APBRST2__UART0_Msk                       BIT(SYSTEM_APBRST2__UART0_Pos)              
#define SYSTEM_APBRST2__TMR5_Pos                        (16UL)                  
#define SYSTEM_APBRST2__TMR5_Msk                        BIT(SYSTEM_APBRST2__TMR5_Pos)             
#define SYSTEM_APBRST2__TMR4_Pos                        (17UL)                  
#define SYSTEM_APBRST2__TMR4_Msk                        BIT(SYSTEM_APBRST2__TMR4_Pos)             
#define SYSTEM_APBRST2__TMR3_Pos                        (18UL)                  
#define SYSTEM_APBRST2__TMR3_Msk                        BIT(SYSTEM_APBRST2__TMR3_Pos)             
#define SYSTEM_APBRST2__TMR2_Pos                        (19UL)                  
#define SYSTEM_APBRST2__TMR2_Msk                        BIT(SYSTEM_APBRST2__TMR2_Pos)             
#define SYSTEM_APBRST2__TMR1_Pos                        (20UL)                  
#define SYSTEM_APBRST2__TMR1_Msk                        BIT(SYSTEM_APBRST2__TMR1_Pos)            
#define SYSTEM_APBRST2__TMR0_Pos                        (21UL)                  
#define SYSTEM_APBRST2__TMR0_Msk                        BIT(SYSTEM_APBRST2__TMR0_Pos)            
#define SYSTEM_APBRST2__TACH14_Pos                      (22UL)                  
#define SYSTEM_APBRST2__TACH14_Msk                      BIT(SYSTEM_APBRST2__TACH14_Pos)            
#define SYSTEM_APBRST2__TACH13_Pos                      (23UL)                  
#define SYSTEM_APBRST2__TACH13_Msk                      BIT(SYSTEM_APBRST2__TACH13_Pos)            
#define SYSTEM_APBRST2__TACH12_Pos                      (24UL)                  
#define SYSTEM_APBRST2__TACH12_Msk                      BIT(SYSTEM_APBRST2__TACH12_Pos)           
#define SYSTEM_APBRST2__TACH11_Pos                      (25UL)                  
#define SYSTEM_APBRST2__TACH11_Msk                      BIT(SYSTEM_APBRST2__TACH11_Pos)           
#define SYSTEM_APBRST2__TACH10_Pos                      (26UL)                  
#define SYSTEM_APBRST2__TACH10_Msk                      BIT(SYSTEM_APBRST2__TACH10_Pos)           
#define SYSTEM_APBRST2__TACH9_Pos                       (27UL)                  
#define SYSTEM_APBRST2__TACH9_Msk                       BIT(SYSTEM_APBRST2__TACH9_Pos)           
#define SYSTEM_SOFTRST0__PWM7_Pos                       (0UL)                   
#define SYSTEM_SOFTRST0__PWM7_Msk                       BIT(SYSTEM_SOFTRST0__PWM7_Pos)                 
#define SYSTEM_SOFTRST0__PWM6_Pos                       (1UL)                   
#define SYSTEM_SOFTRST0__PWM6_Msk                       BIT(SYSTEM_SOFTRST0__PWM6_Pos)                 
#define SYSTEM_SOFTRST0__PWM5_Pos                       (2UL)                   
#define SYSTEM_SOFTRST0__PWM5_Msk                       BIT(SYSTEM_SOFTRST0__PWM5_Pos)                 
#define SYSTEM_SOFTRST0__PWM4_Pos                       (3UL)                   
#define SYSTEM_SOFTRST0__PWM4_Msk                       BIT(SYSTEM_SOFTRST0__PWM4_Pos)                 
#define SYSTEM_SOFTRST0__PWM3_Pos                       (4UL)                   
#define SYSTEM_SOFTRST0__PWM3_Msk                       BIT(SYSTEM_SOFTRST0__PWM3_Pos)                
#define SYSTEM_SOFTRST0__PWM2_Pos                       (5UL)                   
#define SYSTEM_SOFTRST0__PWM2_Msk                       BIT(SYSTEM_SOFTRST0__PWM2_Pos)                
#define SYSTEM_SOFTRST0__PWM1_Pos                       (6UL)                   
#define SYSTEM_SOFTRST0__PWM1_Msk                       BIT(SYSTEM_SOFTRST0__PWM1_Pos)                
#define SYSTEM_SOFTRST0__LEDSTP_Pos                     (10UL)                  
#define SYSTEM_SOFTRST0__LEDSTP_Msk                     BIT(SYSTEM_SOFTRST0__LEDSTP_Pos)               
#define SYSTEM_SOFTRST0__LED4_Pos                       (11UL)                  
#define SYSTEM_SOFTRST0__LED4_Msk                       BIT(SYSTEM_SOFTRST0__LED4_Pos)               
#define SYSTEM_SOFTRST0__LED3_Pos                       (12UL)                  
#define SYSTEM_SOFTRST0__LED3_Msk                       BIT(SYSTEM_SOFTRST0__LED3_Pos)              
#define SYSTEM_SOFTRST0__LED2_Pos                       (13UL)                  
#define SYSTEM_SOFTRST0__LED2_Msk                       BIT(SYSTEM_SOFTRST0__LED2_Pos)              
#define SYSTEM_SOFTRST0__LED1_Pos                       (14UL)                  
#define SYSTEM_SOFTRST0__LED1_Msk                       BIT(SYSTEM_SOFTRST0__LED1_Pos)              
#define SYSTEM_SOFTRST0__KBM_Pos                        (15UL)                  
#define SYSTEM_SOFTRST0__KBM_Msk                        BIT(SYSTEM_SOFTRST0__KBM_Pos)              
#define SYSTEM_SOFTRST0__I2CDBG_Pos                     (16UL)                  
#define SYSTEM_SOFTRST0__I2CDBG_Msk                     BIT(SYSTEM_SOFTRST0__I2CDBG_Pos)             
#define SYSTEM_SOFTRST0__I2C11_Pos                      (17UL)                  
#define SYSTEM_SOFTRST0__I2C11_Msk                      BIT(SYSTEM_SOFTRST0__I2C11_Pos)             
#define SYSTEM_SOFTRST0__I2C10_Pos                      (18UL)                  
#define SYSTEM_SOFTRST0__I2C10_Msk                      BIT(SYSTEM_SOFTRST0__I2C10_Pos)             
#define SYSTEM_SOFTRST0__I2C9_Pos                       (19UL)                  
#define SYSTEM_SOFTRST0__I2C9_Msk                       BIT(SYSTEM_SOFTRST0__I2C9_Pos)             
#define SYSTEM_SOFTRST0__I2C8_Pos                       (20UL)                  
#define SYSTEM_SOFTRST0__I2C8_Msk                       BIT(SYSTEM_SOFTRST0__I2C8_Pos)            
#define SYSTEM_SOFTRST0__I2C7_Pos                       (21UL)                  
#define SYSTEM_SOFTRST0__I2C7_Msk                       BIT(SYSTEM_SOFTRST0__I2C7_Pos)            
#define SYSTEM_SOFTRST0__I2C6_Pos                       (22UL)                  
#define SYSTEM_SOFTRST0__I2C6_Msk                       BIT(SYSTEM_SOFTRST0__I2C6_Pos)            
#define SYSTEM_SOFTRST0__I2C5_Pos                       (23UL)                  
#define SYSTEM_SOFTRST0__I2C5_Msk                       BIT(SYSTEM_SOFTRST0__I2C5_Pos)            
#define SYSTEM_SOFTRST0__I2C4_Pos                       (24UL)                  
#define SYSTEM_SOFTRST0__I2C4_Msk                       BIT(SYSTEM_SOFTRST0__I2C4_Pos)           
#define SYSTEM_SOFTRST0__I2C3_Pos                       (25UL)                  
#define SYSTEM_SOFTRST0__I2C3_Msk                       BIT(SYSTEM_SOFTRST0__I2C3_Pos)           
#define SYSTEM_SOFTRST0__I2C2_Pos                       (26UL)                  
#define SYSTEM_SOFTRST0__I2C2_Msk                       BIT(SYSTEM_SOFTRST0__I2C2_Pos)           
#define SYSTEM_SOFTRST0__I2C1_Pos                       (27UL)                  
#define SYSTEM_SOFTRST0__I2C1_Msk                       BIT(SYSTEM_SOFTRST0__I2C1_Pos)           
#define SYSTEM_SOFTRST0__GPIO_Pos                       (28UL)                  
#define SYSTEM_SOFTRST0__GPIO_Msk                       BIT(SYSTEM_SOFTRST0__GPIO_Pos)          
#define SYSTEM_SOFTRST0__CEC2_Pos                       (29UL)                  
#define SYSTEM_SOFTRST0__CEC2_Msk                       BIT(SYSTEM_SOFTRST0__CEC2_Pos)          
#define SYSTEM_SOFTRST0__CEC1_Pos                       (30UL)                  
#define SYSTEM_SOFTRST0__CEC1_Msk                       BIT(SYSTEM_SOFTRST0__CEC1_Pos)          
#define SYSTEM_SOFTRST0__ADC_Pos                        (31UL)                  
#define SYSTEM_SOFTRST0__ADC_Msk                        BIT(SYSTEM_SOFTRST0__ADC_Pos)          
#define SYSTEM_SOFTRST1_TMR0_Pos                        (0UL)                   
#define SYSTEM_SOFTRST1_TMR0_Msk                        BIT(SYSTEM_SOFTRST1_TMR0_Pos)                 
#define SYSTEM_SOFTRST1__TACH14_Pos                     (1UL)                   
#define SYSTEM_SOFTRST1__TACH14_Msk                     BIT(SYSTEM_SOFTRST1__TACH14_Pos)                 
#define SYSTEM_SOFTRST1__TACH13_Pos                     (2UL)                   
#define SYSTEM_SOFTRST1__TACH13_Msk                     BIT(SYSTEM_SOFTRST1__TACH13_Pos)                 
#define SYSTEM_SOFTRST1__TACH12_Pos                     (3UL)                   
#define SYSTEM_SOFTRST1__TACH12_Msk                     BIT(SYSTEM_SOFTRST1__TACH12_Pos)                 
#define SYSTEM_SOFTRST1__TACH11_Pos                     (4UL)                   
#define SYSTEM_SOFTRST1__TACH11_Msk                     BIT(SYSTEM_SOFTRST1__TACH11_Pos)                
#define SYSTEM_SOFTRST1__TACH10_Pos                     (5UL)                   
#define SYSTEM_SOFTRST1__TACH10_Msk                     BIT(SYSTEM_SOFTRST1__TACH10_Pos)                
#define SYSTEM_SOFTRST1__TACH9_Pos                      (6UL)                   
#define SYSTEM_SOFTRST1__TACH9_Msk                      BIT(SYSTEM_SOFTRST1__TACH9_Pos)                
#define SYSTEM_SOFTRST1__TACH8_Pos                      (7UL)                   
#define SYSTEM_SOFTRST1__TACH8_Msk                      BIT(SYSTEM_SOFTRST1__TACH8_Pos)                
#define SYSTEM_SOFTRST1__TACH7_Pos                      (8UL)                   
#define SYSTEM_SOFTRST1__TACH7_Msk                      BIT(SYSTEM_SOFTRST1__TACH7_Pos)               
#define SYSTEM_SOFTRST1__TACH6_Pos                      (9UL)                   
#define SYSTEM_SOFTRST1__TACH6_Msk                      BIT(SYSTEM_SOFTRST1__TACH6_Pos)               
#define SYSTEM_SOFTRST1__TACH5_Pos                      (10UL)                  
#define SYSTEM_SOFTRST1__TACH5_Msk                      BIT(SYSTEM_SOFTRST1__TACH5_Pos)               
#define SYSTEM_SOFTRST1__TACH4_Pos                      (11UL)                  
#define SYSTEM_SOFTRST1__TACH4_Msk                      BIT(SYSTEM_SOFTRST1__TACH4_Pos)               
#define SYSTEM_SOFTRST1__TACH3_Pos                      (12UL)                  
#define SYSTEM_SOFTRST1__TACH3_Msk                      BIT(SYSTEM_SOFTRST1__TACH3_Pos)              
#define SYSTEM_SOFTRST1__TACH2_Pos                      (13UL)                  
#define SYSTEM_SOFTRST1__TACH2_Msk                      BIT(SYSTEM_SOFTRST1__TACH2_Pos)              
#define SYSTEM_SOFTRST1__TACH1_Pos                      (14UL)                  
#define SYSTEM_SOFTRST1__TACH1_Msk                      BIT(SYSTEM_SOFTRST1__TACH1_Pos)              
#define SYSTEM_SOFTRST1__SPIM_Pos                       (15UL)                  
#define SYSTEM_SOFTRST1__SPIM_Msk                       BIT(SYSTEM_SOFTRST1__SPIM_Pos)              
#define SYSTEM_SOFTRST1__SPIS_Pos                       (16UL)                  
#define SYSTEM_SOFTRST1__SPIS_Msk                       BIT(SYSTEM_SOFTRST1__SPIS_Pos)             
#define SYSTEM_SOFTRST1__SLWTMR3_Pos                    (17UL)                  
#define SYSTEM_SOFTRST1__SLWTMR3_Msk                    BIT(SYSTEM_SOFTRST1__SLWTMR3_Pos)             
#define SYSTEM_SOFTRST1__SLWTMR2_Pos                    (18UL)                  
#define SYSTEM_SOFTRST1__SLWTMR2_Msk                    BIT(SYSTEM_SOFTRST1__SLWTMR2_Pos)             
#define SYSTEM_SOFTRST1__SLWTMR1_Pos                    (19UL)                  
#define SYSTEM_SOFTRST1__SLWTMR1_Msk                    BIT(SYSTEM_SOFTRST1__SLWTMR1_Pos)             
#define SYSTEM_SOFTRST1__SLWTMR0_Pos                    (20UL)                  
#define SYSTEM_SOFTRST1__SLWTMR0_Msk                    BIT(SYSTEM_SOFTRST1__SLWTMR0_Pos)            
#define SYSTEM_SOFTRST1__RTMR_Pos                       (21UL)                  
#define SYSTEM_SOFTRST1__RTMR_Msk                       BIT(SYSTEM_SOFTRST1__RTMR_Pos)            
#define SYSTEM_SOFTRST1__PWM14_Pos                      (25UL)                  
#define SYSTEM_SOFTRST1__PWM14_Msk                      BIT(SYSTEM_SOFTRST1__PWM14_Pos)           
#define SYSTEM_SOFTRST1__PWM13_Pos                      (26UL)                  
#define SYSTEM_SOFTRST1__PWM13_Msk                      BIT(SYSTEM_SOFTRST1__PWM13_Pos)           
#define SYSTEM_SOFTRST1__PWM12_Pos                      (27UL)                  
#define SYSTEM_SOFTRST1__PWM12_Msk                      BIT(SYSTEM_SOFTRST1__PWM12_Pos)           
#define SYSTEM_SOFTRST1__PWM11_Pos                      (28UL)                  
#define SYSTEM_SOFTRST1__PWM11_Msk                      BIT(SYSTEM_SOFTRST1__PWM11_Pos)          
#define SYSTEM_SOFTRST1__PWM10_Pos                      (29UL)                  
#define SYSTEM_SOFTRST1__PWM10_Msk                      BIT(SYSTEM_SOFTRST1__PWM10_Pos)          
#define SYSTEM_SOFTRST1__PWM9_Pos                       (30UL)                  
#define SYSTEM_SOFTRST1__PWM9_Msk                       BIT(SYSTEM_SOFTRST1__PWM9_Pos)          
#define SYSTEM_SOFTRST1__PWM8_Pos                       (31UL)                  
#define SYSTEM_SOFTRST1__PWM8_Msk                       BIT(SYSTEM_SOFTRST1__PWM8_Pos)          
#define SYSTEM_SOFTRST2__TRC_Pos                        (0UL)                   
#define SYSTEM_SOFTRST2__TRC_Msk                        BIT(SYSTEM_SOFTRST2__TRC_Pos)                 
#define SYSTEM_SOFTRST2__PECI_Pos                       (1UL)                   
#define SYSTEM_SOFTRST2__PECI_Msk                       BIT(SYSTEM_SOFTRST2__PECI_Pos)                 
#define SYSTEM_SOFTRST2__LPT_Pos                        (2UL)                   
#define SYSTEM_SOFTRST2__LPT_Msk                        BIT(SYSTEM_SOFTRST2__LPT_Pos)                 
#define SYSTEM_SOFTRST2__UART2_Pos                      (4UL)                   
#define SYSTEM_SOFTRST2__UART2_Msk                      BIT(SYSTEM_SOFTRST2__UART2_Pos)                
#define SYSTEM_SOFTRST2__UART1_Pos                      (5UL)                   
#define SYSTEM_SOFTRST2__UART1_Msk                      BIT(SYSTEM_SOFTRST2__UART1_Pos)                
#define SYSTEM_SOFTRST2__UART0_Pos                      (6UL)                   
#define SYSTEM_SOFTRST2__UART0_Msk                      BIT(SYSTEM_SOFTRST2__UART0_Pos)                
#define SYSTEM_SOFTRST2__TMR5_Pos                       (7UL)                   
#define SYSTEM_SOFTRST2__TMR5_Msk                       BIT(SYSTEM_SOFTRST2__TMR5_Pos)                
#define SYSTEM_SOFTRST2__TMR4_Pos                       (8UL)                   
#define SYSTEM_SOFTRST2__TMR4_Msk                       BIT(SYSTEM_SOFTRST2__TMR4_Pos)               
#define SYSTEM_SOFTRST2__TMR3_Pos                       (9UL)                   
#define SYSTEM_SOFTRST2__TMR3_Msk                       BIT(SYSTEM_SOFTRST2__TMR3_Pos)               
#define SYSTEM_SOFTRST2__TMR2_Pos                       (10UL)                  
#define SYSTEM_SOFTRST2__TMR2_Msk                       BIT(SYSTEM_SOFTRST2__TMR2_Pos)               
#define SYSTEM_SOFTRST2__TMR1_Pos                       (11UL)                  
#define SYSTEM_SOFTRST2__TMR1_Msk                       BIT(SYSTEM_SOFTRST2__TMR1_Pos)               
                                                        
#define SYSTEM_SLPCTRL_SLPMDSEL_Pos                     (1UL)                   
#define SYSTEM_SLPCTRL_SLPMDSEL_Msk                     BIT(SYSTEM_SLPCTRL_SLPMDSEL_Pos)                 
#define SYSTEM_SLPCTRL_ESPIWKEN_Pos                     (2UL)                   
#define SYSTEM_SLPCTRL_ESPIWKEN_Msk                     BIT(SYSTEM_SLPCTRL_ESPIWKEN_Pos)                 
#define SYSTEM_SLPCTRL_PS2WKEN_Pos                      (3UL)                   
#define SYSTEM_SLPCTRL_PS2WKEN_Msk                      BIT(SYSTEM_SLPCTRL_PS2WKEN_Pos)                 
#define SYSTEM_SLPCTRL_I2CWKEN_Pos                      (4UL)                   
#define SYSTEM_SLPCTRL_I2CWKEN_Msk                      BIT(SYSTEM_SLPCTRL_I2CWKEN_Pos)                
#define SYSTEM_SLPCTRL_GPIOWKEN_Pos                     (5UL)                   
#define SYSTEM_SLPCTRL_GPIOWKEN_Msk                     BIT(SYSTEM_SLPCTRL_GPIOWKEN_Pos)                
#define SYSTEM_SLPCTRL__SPISWKEN_Pos                    (6UL)                   
#define SYSTEM_SLPCTRL__SPISWKEN_Msk                    BIT(SYSTEM_SLPCTRL__SPISWKEN_Pos)                
#define SYSTEM_SLPCTRL_SYSLOCKBIT_Pos                   (9UL)                   
#define SYSTEM_SLPCTRL_SYSLOCKBIT_Msk                   BIT(SYSTEM_SLPCTRL_SYSLOCKBIT_Pos)               
                                                        
#define SYSTEM_IXCINPUTSEL__I2C1_SCL_SEL_Pos     		(0UL)            
#define SYSTEM_IXCINPUTSEL__I2C1_SCL_SEL_Msk            BIT(SYSTEM_IXCINPUTSEL__I2C1_SCL_SEL_Pos)       
#define SYSTEM_IXCINPUTSEL__I2C1_SDA_SEL_Pos            (1UL)         
#define SYSTEM_IXCINPUTSEL__I2C1_SDA_SEL_Msk            BIT(SYSTEM_IXCINPUTSEL__I2C1_SDA_SEL_Pos)       
#define SYSTEM_IXCINPUTSEL__I2C3_SCL_SEL_Pos            (2UL)         
#define SYSTEM_IXCINPUTSEL__I2C3_SCL_SEL_Msk            GENMASK(3, 2)   
#define SYSTEM_IXCINPUTSEL__I2C3_SDA_SEL_Pos            (4UL)         
#define SYSTEM_IXCINPUTSEL__I2C3_SDA_SEL_Msk            BIT(SYSTEM_IXCINPUTSEL__I2C3_SDA_SEL_Pos)      
#define SYSTEM_IXCINPUTSEL__I2C4_SCL_SEL_Pos            (5UL)         
#define SYSTEM_IXCINPUTSEL__I2C4_SCL_SEL_Msk            BIT(SYSTEM_IXCINPUTSEL__I2C4_SCL_SEL_Pos)      
#define SYSTEM_IXCINPUTSEL__I2C4_SDA_SEL_Pos            (6UL)         
#define SYSTEM_IXCINPUTSEL__I2C4_SDA_SEL_Msk            BIT(SYSTEM_IXCINPUTSEL__I2C4_SDA_SEL_Pos)      
#define SYSTEM_IXCINPUTSEL__I2C6_SCL_SEL_Pos            (7UL)         
#define SYSTEM_IXCINPUTSEL__I2C6_SCL_SEL_Msk            BIT(SYSTEM_IXCINPUTSEL__I2C6_SCL_SEL_Pos)      
#define SYSTEM_IXCINPUTSEL__I2C6_SDA_SEL_Pos            (8UL)         
#define SYSTEM_IXCINPUTSEL__I2C6_SDA_SEL_Msk            BIT(SYSTEM_IXCINPUTSEL__I2C6_SDA_SEL_Pos)     
#define SYSTEM_IXCINPUTSEL__I2C8_SCL_SEL_Pos            (9UL)         
#define SYSTEM_IXCINPUTSEL__I2C8_SCL_SEL_Msk            BIT(SYSTEM_IXCINPUTSEL__I2C8_SCL_SEL_Pos)     
#define SYSTEM_IXCINPUTSEL__I2C8_SDA_SEL_Pos            (10UL)        
#define SYSTEM_IXCINPUTSEL__I2C8_SDA_SEL_Msk            BIT(SYSTEM_IXCINPUTSEL__I2C8_SDA_SEL_Pos)     
#define SYSTEM_IXCINPUTSEL__I2C9_SCL_SEL_Pos            (11UL)        
#define SYSTEM_IXCINPUTSEL__I2C9_SCL_SEL_Msk            BIT(SYSTEM_IXCINPUTSEL__I2C9_SCL_SEL_Pos)     
#define SYSTEM_IXCINPUTSEL__I2C9_SDA_SEL_Pos            (12UL)        
#define SYSTEM_IXCINPUTSEL__I2C9_SDA_SEL_Msk            BIT(SYSTEM_IXCINPUTSEL__I2C9_SDA_SEL_Pos)    
#define SYSTEM_IXCINPUTSEL__I2C10_SCL_SEL_Pos        	(13UL)       
#define SYSTEM_IXCINPUTSEL__I2C10_SCL_SEL_Msk           BIT(SYSTEM_IXCINPUTSEL__I2C10_SCL_SEL_Pos) 
#define SYSTEM_IXCINPUTSEL__I2C10_SDA_SEL_Pos           (14UL)     
#define SYSTEM_IXCINPUTSEL__I2C10_SDA_SEL_Msk           BIT(SYSTEM_IXCINPUTSEL__I2C10_SDA_SEL_Pos) 
#define SYSTEM_IXCINPUTSEL__I2C11_SCL_SEL_Pos           (15UL)     
#define SYSTEM_IXCINPUTSEL__I2C11_SCL_SEL_Msk           BIT(SYSTEM_IXCINPUTSEL__I2C11_SCL_SEL_Pos) 
#define SYSTEM_IXCINPUTSEL__I2C11_SDA_SEL_Pos           (16UL)     
#define SYSTEM_IXCINPUTSEL__I2C11_SDA_SEL_Msk           BIT(SYSTEM_IXCINPUTSEL__I2C11_SDA_SEL_Pos)
#define SYSTEM_IXCINPUTSEL__I3C04_SCL_SEL_Pos           (17UL)     
#define SYSTEM_IXCINPUTSEL__I3C04_SCL_SEL_Msk           BIT(SYSTEM_IXCINPUTSEL__I3C04_SCL_SEL_Pos)
#define SYSTEM_IXCINPUTSEL__I3C04_SDA_SEL_Pos           (18UL)     
#define SYSTEM_IXCINPUTSEL__I3C04_SDA_SEL_Msk           BIT(SYSTEM_IXCINPUTSEL__I3C04_SDA_SEL_Pos)
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_ACK_SEL_Pos 	(0UL)        
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_ACK_SEL_Msk 	BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT_ACK_SEL_Pos)      
#define SYSTEM__UART__LPT_INPUT_SEL__LPT__BUSY_SEL_Pos 	(1UL)      
#define SYSTEM__UART__LPT_INPUT_SEL__LPT__BUSY_SEL_Msk 	BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT__BUSY_SEL_Pos)    
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D0_SEL_Pos 	(2UL)         
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D0_SEL_Msk     BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT_D0_SEL_Pos)   
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D1_SEL_Pos     (3UL)     
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D1_SEL_Msk     BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT_D1_SEL_Pos)   
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D2_SEL_Pos     (4UL)     
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D2_SEL_Msk     BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT_D2_SEL_Pos)  
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D3_SEL_Pos     (5UL)     
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D3_SEL_Msk     BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT_D3_SEL_Pos)  
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D4_SEL_Pos     (6UL)     
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D4_SEL_Msk     BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT_D4_SEL_Pos)  
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D5_SEL_Pos     (7UL)     
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D5_SEL_Msk     BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT_D5_SEL_Pos)  
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D6_SEL_Pos     (8UL)     
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D6_SEL_Msk     BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT_D6_SEL_Pos) 
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D7_SEL_Pos     (9UL)     
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_D7_SEL_Msk     BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT_D7_SEL_Pos) 
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_ERR_SEL_Pos 	(10UL)       
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_ERR_SEL_Msk 	BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT_ERR_SEL_Pos)    
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_PE_SEL_Pos 	(11UL)        
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_PE_SEL_Msk 	BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT_PE_SEL_Pos)     
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_SLCT_SEL_Pos 	(12UL)      
#define SYSTEM__UART__LPT_INPUT_SEL__LPT_SLCT_SEL_Msk 	BIT(SYSTEM__UART__LPT_INPUT_SEL__LPT_SLCT_SEL_Pos)  
#define SYSTEM__UART__LPT_INPUT_SEL__UART01_RX_SEL_Pos 	(13UL)     
#define SYSTEM__UART__LPT_INPUT_SEL__UART01_RX_SEL_Msk 	BIT(SYSTEM__UART__LPT_INPUT_SEL__UART01_RX_SEL_Pos) 
#define SYSTEM__UART__LPT_INPUT_SEL__UART01_DCD_SEL_Pos (14UL)    
#define SYSTEM__UART__LPT_INPUT_SEL__UART01_DCD_SEL_Msk GENMASK(15, 14)
#define SYSTEM__UART__LPT_INPUT_SEL__UART01_CTS_SEL_Pos (16UL)    
#define SYSTEM__UART__LPT_INPUT_SEL__UART01_CTS_SEL_Msk GENMASK(17, 16)
#define SYSTEM__UART__LPT_INPUT_SEL__UART01_DSR_SEL_Pos (18UL)    
#define SYSTEM__UART__LPT_INPUT_SEL__UART01_DSR_SEL_Msk BIT(SYSTEM__UART__LPT_INPUT_SEL__UART01_DSR_SEL_Pos)
#define SYSTEM__UART__LPT_INPUT_SEL__UART01_RI_SEL_Pos 	(19UL)     
#define SYSTEM__UART__LPT_INPUT_SEL__UART01_RI_SEL_Msk 	GENMASK(20, 19)
#define SYSTEM__UART__LPT_INPUT_SEL__UART02_CTS_SEL_Pos (21UL)    
#define SYSTEM__UART__LPT_INPUT_SEL__UART02_CTS_SEL_Msk GENMASK(22, 21)
#define SYSTEM__UART__LPT_INPUT_SEL__UART02_DCD_SEL_Pos (23UL)    
#define SYSTEM__UART__LPT_INPUT_SEL__UART02_DCD_SEL_Msk GENMASK(24, 23)
#define SYSTEM__UART__LPT_INPUT_SEL__UART02_DSR_SEL_Pos (25UL)    
#define SYSTEM__UART__LPT_INPUT_SEL__UART02_DSR_SEL_Msk GENMASK(26, 25)
#define SYSTEM__UART__LPT_INPUT_SEL__UART02_RI_SEL_Pos 	(27UL)     
#define SYSTEM__UART__LPT_INPUT_SEL__UART02_RI_SEL_Msk 	BIT(SYSTEM__UART__LPT_INPUT_SEL__UART02_DSR_SEL_Msk)
#define SYSTEM__UART__LPT_INPUT_SEL__UART02_RX_SEL_Pos 	(28UL)     
#define SYSTEM__UART__LPT_INPUT_SEL__UART02_RX_SEL_Msk 	BIT(SYSTEM__UART__LPT_INPUT_SEL__UART02_RX_SEL_Pos)
#define SYSTEM_IP_INPUT_SEL__TACH01_SEL_Pos 			(0UL)                 
#define SYSTEM_IP_INPUT_SEL__TACH01_SEL_Msk 			GENMASK(1, 0)               
#define SYSTEM_IP_INPUT_SEL__TACH02_SEL_Pos             (2UL)           
#define SYSTEM_IP_INPUT_SEL__TACH02_SEL_Msk             GENMASK(3, 2)   
#define SYSTEM_IP_INPUT_SEL__TACH03_SEL_Pos             (4UL)           
#define SYSTEM_IP_INPUT_SEL__TACH03_SEL_Msk             GENMASK(5, 4)   
#define SYSTEM_IP_INPUT_SEL__TACH04_SEL_Pos             (6UL)           
#define SYSTEM_IP_INPUT_SEL__TACH04_SEL_Msk             GENMASK(7, 6)   
#define SYSTEM_IP_INPUT_SEL__TACH05_SEL_Pos             (8UL)           
#define SYSTEM_IP_INPUT_SEL__TACH05_SEL_Msk             GENMASK(9, 8)   
#define SYSTEM_IP_INPUT_SEL__TACH06_SEL_Pos             (10UL)          
#define SYSTEM_IP_INPUT_SEL__TACH06_SEL_Msk             GENMASK(11, 10) 
#define SYSTEM_IP_INPUT_SEL__TACH07_SEL_Pos             (12UL)          
#define SYSTEM_IP_INPUT_SEL__TACH07_SEL_Msk             BIT(SYSTEM_IP_INPUT_SEL__TACH07_SEL_Pos)      
#define SYSTEM_IP_INPUT_SEL__TACH08_SEL_Pos             (13UL)          
#define SYSTEM_IP_INPUT_SEL__TACH08_SEL_Msk             BIT(SYSTEM_IP_INPUT_SEL__TACH08_SEL_Pos)      
#define SYSTEM_IP_INPUT_SEL__CEC1_SEL_Pos      			(18UL)             
#define SYSTEM_IP_INPUT_SEL__CEC1_SEL_Msk               BIT(SYSTEM_IP_INPUT_SEL__CEC1_SEL_Pos)
#define SYSTEM_IP_INPUT_SEL__CEC2_SEL_Pos               (19UL)     
#define SYSTEM_IP_INPUT_SEL__CEC2_SEL_Msk               BIT(SYSTEM_IP_INPUT_SEL__CEC2_SEL_Pos)
#define SYSTEM_IP_INPUT_SEL_ESPI_RST_SEL_Pos    		(20UL)            
#define SYSTEM_IP_INPUT_SEL_ESPI_RST_SEL_Msk            BIT(SYSTEM_IP_INPUT_SEL_ESPI_RST_SEL_Pos)
#define SYSTEM_IP_INPUT_SEL_LPC_RST_SEL_Pos      		(21UL)           
#define SYSTEM_IP_INPUT_SEL_LPC_RST_SEL_Msk             BIT(SYSTEM_IP_INPUT_SEL_LPC_RST_SEL_Pos)
#define SYSTEM_IP_INPUT_SEL_KSI6_SEL_Pos        		(22UL)            
#define SYSTEM_IP_INPUT_SEL_KSI6_SEL_Msk                BIT(SYSTEM_IP_INPUT_SEL_KSI6_SEL_Pos)    
#define SYSTEM_IP_INPUT_SEL_KSI7_SEL_Pos                (23UL)          
#define SYSTEM_IP_INPUT_SEL_KSI7_SEL_Msk                BIT(SYSTEM_IP_INPUT_SEL_KSI7_SEL_Pos)    
#define SYSTEM_IP_INPUT_SEL__LEDSTP_SEL_Pos          	(24UL)       
#define SYSTEM_IP_INPUT_SEL__LEDSTP_SEL_Msk             BIT(SYSTEM_IP_INPUT_SEL__LEDSTP_SEL_Pos)
#define SYSTEM_IP_INPUT_SEL__PS2_CLK1_SEL_Pos    		(25UL)           
#define SYSTEM_IP_INPUT_SEL__PS2_CLK1_SEL_Msk           GENMASK(26, 25)
#define SYSTEM_IP_INPUT_SEL__PS2_DATA1_SEL_Pos 			(27UL)             
#define SYSTEM_IP_INPUT_SEL__PS2_DATA1_SEL_Msk 			BIT(SYSTEM_IP_INPUT_SEL__PS2_DATA1_SEL_Pos)      
#define SYSTEM_IP_INPUT_SEL__PS2_CLK2_SEL_Pos 			(28UL)              
#define SYSTEM_IP_INPUT_SEL__PS2_CLK2_SEL_Msk 			BIT(SYSTEM_IP_INPUT_SEL__PS2_CLK2_SEL_Pos)      
#define SYSTEM_IP_INPUT_SEL__PS2_DATA2_SEL_Pos 			(29UL)             
#define SYSTEM_IP_INPUT_SEL__PS2_DATA2_SEL_Msk 			BIT(SYSTEM_IP_INPUT_SEL__PS2_DATA2_SEL_Pos)     
#define SYSTEM_IP_INPUT_SEL__PWRBTN_SEL_Pos 			(30UL)                
#define SYSTEM_IP_INPUT_SEL__PWRBTN_SEL_Msk 			GENMASK(31, 30)        
#define SYSTEM_SPIC_INPUT_SEL__SPIC01_IO0_SEL_Pos 		(0UL)           
#define SYSTEM_SPIC_INPUT_SEL__SPIC01_IO0_SEL_Msk 		GENMASK(1, 0)         
#define SYSTEM_SPIC_INPUT_SEL__SPIC01_IO1_SEL_Pos 		(2UL)           
#define SYSTEM_SPIC_INPUT_SEL__SPIC01_IO1_SEL_Msk 		GENMASK(3, 2)         
#define SYSTEM_SPIC_INPUT_SEL__SPIC01_IO2_SEL_Pos 		(4UL)           
#define SYSTEM_SPIC_INPUT_SEL__SPIC01_IO2_SEL_Msk 		GENMASK(5, 4)        
#define SYSTEM_SPIC_INPUT_SEL__SPIC01_IO3_SEL_Pos 		(6UL)           
#define SYSTEM_SPIC_INPUT_SEL__SPIC01_IO3_SEL_Msk 		GENMASK(7, 6)        
#define SYSTEM_SYSPERCTRL_PLLBYPASS_Pos                 (0UL)                   
#define SYSTEM_SYSPERCTRL_PLLBYPASS_Msk                 BIT(SYSTEM_SYSPERCTRL_PLLBYPASS_Pos)                 
#define SYSTEM_SYSPERCTRL_DIOEN_Pos                     (1UL)                   
#define SYSTEM_SYSPERCTRL_DIOEN_Msk                     BIT(SYSTEM_SYSPERCTRL_DIOEN_Pos)                 
#define SYSTEM_WUE__ESPIRST_Pos                         (0UL)                   
#define SYSTEM_WUE__ESPIRST_Msk                         BIT(SYSTEM_WUE__ESPIRST_Pos)                 
#define SYSTEM_WUE__ESPI_Pos                            (1UL)                   
#define SYSTEM_WUE__ESPI_Msk                            BIT(SYSTEM_WUE__ESPI_Pos)                 
#define SYSTEM_WUE__PECI_Pos                            (2UL)                   
#define SYSTEM_WUE__PECI_Msk                            BIT(SYSTEM_WUE__PECI_Pos)                 
#define SYSTEM__32KSRC_S_32KSRC_Pos                     (31UL)                  
#define SYSTEM__32KSRC_S_32KSRC_Msk                     BIT(SYSTEM__32KSRC_S_32KSRC_Pos)          
#define SYSTEM_VBATIPCLK__RTCPWR_Pos                    (0UL)                   
#define SYSTEM_VBATIPCLK__RTCPWR_Msk                    BIT(SYSTEM_VBATIPCLK__RTCPWR_Pos)                 
#define SYSTEM_VBATIPCLK__PWRBTN1PWR_Pos                (1UL)                   
#define SYSTEM_VBATIPCLK__PWRBTN1PWR_Msk                BIT(SYSTEM_VBATIPCLK__PWRBTN1PWR_Pos)                 
#define SYSTEM_VBATIPCLK__PWRBTN0PWR_Pos                (2UL)                   
#define SYSTEM_VBATIPCLK__PWRBTN0PWR_Msk                BIT(SYSTEM_VBATIPCLK__PWRBTN0PWR_Pos)                 
#define SYSTEM_VBATIPCLK__WDTPWR_Pos                    (3UL)                   
#define SYSTEM_VBATIPCLK__WDTPWR_Msk                    BIT(SYSTEM_VBATIPCLK__WDTPWR_Pos)                 
#define SYSTEM_VBATIPCLK__RC32KPWR_Pos                  (4UL)                   
#define SYSTEM_VBATIPCLK__RC32KPWR_Msk                  BIT(SYSTEM_VBATIPCLK__RC32KPWR_Pos)                
#define SYSTEM_VBATIPCLK__PWRBTN1RST_Pos                (5UL)                   
#define SYSTEM_VBATIPCLK__PWRBTN1RST_Msk                BIT(SYSTEM_VBATIPCLK__PWRBTN1RST_Pos)                
#define SYSTEM_VBATIPCLK__PWRBTN0RST_Pos                (6UL)                   
#define SYSTEM_VBATIPCLK__PWRBTN0RST_Msk                BIT(SYSTEM_VBATIPCLK__PWRBTN0RST_Pos)                
#define SYSTEM_VBATIPCLK__WDTRST_Pos                    (7UL)                   
#define SYSTEM_VBATIPCLK__WDTRST_Msk                    BIT(SYSTEM_VBATIPCLK__WDTRST_Pos)                
#define SYSTEM_VBATIPCLK__RTCRST_Pos                    (8UL)                   
#define SYSTEM_VBATIPCLK__RTCRST_Msk                    BIT(SYSTEM_VBATIPCLK__RTCRST_Pos)               
#define SYSTEM_VBATIPCLK__RC32KRST_Pos                  (9UL)                   
#define SYSTEM_VBATIPCLK__RC32KRST_Msk                  BIT(SYSTEM_VBATIPCLK__RC32KRST_Pos)               
#define SYSTEM_VBATIPCLK__PWRBTN1GAT_Pos                (10UL)                  
#define SYSTEM_VBATIPCLK__PWRBTN1GAT_Msk                BIT(SYSTEM_VBATIPCLK__PWRBTN1GAT_Pos)               
#define SYSTEM_VBATIPCLK__PWRBTN0GAT_Pos                (11UL)                  
#define SYSTEM_VBATIPCLK__PWRBTN0GAT_Msk                BIT(SYSTEM_VBATIPCLK__PWRBTN0GAT_Pos)               
#define SYSTEM_VBATIPCLK__WDTGAT_Pos                    (12UL)                  
#define SYSTEM_VBATIPCLK__WDTGAT_Msk                    BIT(SYSTEM_VBATIPCLK__WDTGAT_Pos)              
#define SYSTEM_VBATIPCLK__RTCGAT_Pos                    (13UL)                  
#define SYSTEM_VBATIPCLK__RTCGAT_Msk                    BIT(SYSTEM_VBATIPCLK__RTCGAT_Pos)              
#define SYSTEM_VBATIPCLK__RC32KGAT_Pos                  (14UL)                  
#define SYSTEM_VBATIPCLK__RC32KGAT_Msk                  BIT(SYSTEM_VBATIPCLK__RC32KGAT_Pos)              
#define SYSTEM_VBATIPCLK_S_RC32K_Pos                    (17UL)                  
#define SYSTEM_VBATIPCLK_S_RC32K_Msk                    GENMASK(18, 17)             
#define SYSTEM_VBATIPCLK__RC32KCAL_Pos                  (19UL)                  
#define SYSTEM_VBATIPCLK__RC32KCAL_Msk                  BIT(SYSTEM_VBATIPCLK__RC32KCAL_Pos)             
#define SYSTEM_VBATIPCLK__RC32K_Pos                     (20UL)                  
#define SYSTEM_VBATIPCLK__RC32K_Msk                     BIT(SYSTEM_VBATIPCLK__RC32K_Pos)            
#define SYSTEM_LDOCTRL_LDO2PWREN_Pos      (3UL)                    
#define SYSTEM_LDOCTRL_LDO2PWREN_Msk      BIT(SYSTEM_LDOCTRL_LDO2PWREN_Pos)                   
#define SYSTEM_LDOCTRL_LDO3PWREN_Pos      (7UL)                    
#define SYSTEM_LDOCTRL_LDO3PWREN_Msk      BIT(SYSTEM_LDOCTRL_LDO3PWREN_Pos)                 
#define SYSTEM_LDOCTRL_LDO0CFG_Pos        (29UL)                  
#define SYSTEM_LDOCTRL_LDO0CFG_Msk        BIT(SYSTEM_LDOCTRL_LDO0CFG_Pos)            
#define SYSTEM_LDOCTRL_LDO2CFG_Pos        (30UL)                   
#define SYSTEM_LDOCTRL_LDO2CFG_Msk        BIT(SYSTEM_LDOCTRL_LDO2CFG_Pos)            
#define SYSTEM_LDOCTRL_LDO3CFG_Pos        (31UL)                    
#define SYSTEM_LDOCTRL_LDO3CFG_Msk        BIT(SYSTEM_LDOCTRL_LDO3CFG_Pos)           

#define SYSTEM_PLLCTRL_PWREN_Pos          (0UL)                  
#define SYSTEM_PLLCTRL_PWREN_Msk          BIT(SYSTEM_PLLCTRL_PWREN_Pos)               
#define SYSTEM_PLLCTRL_ARDY_Pos           (19UL)                
#define SYSTEM_PLLCTRL_ARDY_Msk           BIT(SYSTEM_PLLCTRL_ARDY_Pos)           
#define SYSTEM_PLLCTRL_DRDY_Pos           (20UL)               
#define SYSTEM_PLLCTRL_DRDY_Msk           BIT(SYSTEM_PLLCTRL_DRDY_Pos)
#define SYSTEM_ESPI_CLK_PAD_DRIVING_Pos   (1UL)
#define SYSTEM_ESPI_CLK_PAD_DRIVING_Msk   BIT(SYSTEM_ESPI_CLK_PAD_DRIVING_Pos)
#define SYSTEM_ESPI_PAD_INPUTDET_Pos   (0UL)
#define SYSTEM_ESPI_PAD_INPUTDET_Msk   BIT(SYSTEM_ESPI_PAD_INPUTDET_Pos)
#define RTS5918_SCCON_REG_BASE ((SYSTEM_Type *)(DT_REG_ADDR(DT_NODELABEL(sccon))))

#endif /* ZEPHYR_SOC_REALTEK_RTS5918_REG_SYSTEM_H */
