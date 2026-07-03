/*
 * Copyright (c) 2016-2019 Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "arm_uart_drv.h"

#include <stddef.h>

/* UART register map structure */
struct _arm_uart_reg_map_t {
    union {
        volatile uint32_t RBR;
        volatile uint32_t THR;
        volatile uint32_t DLL;
    };

    union {
        volatile uint32_t DLH;
        volatile uint32_t IER;
    };

    union {
        volatile uint32_t IIR;
        volatile uint32_t FCR;
    };

    volatile uint32_t LCR;
    volatile uint32_t MCR;
    volatile uint32_t LSR;
    volatile uint32_t MSR;
    volatile uint32_t SCR;
};

typedef enum {
    ParityNone = 0,
    ParityOdd = 1,
    ParityEven = 2,
    ParityForced1 = 3,
    ParityForced0 = 4
} SerialParity;

/* CTRL Register */
#define ARM_UART_TX_EN       (1ul << 0)
#define ARM_UART_RX_EN       (1ul << 1)
#define ARM_UART_TX_INTR_EN  (1ul << 2)
#define ARM_UART_RX_INTR_EN  (1ul << 1)

/* STATE Register */
#define ARM_UART_TX_BF  (1ul << 5)
#define ARM_UART_RX_BF  (1ul << 1)

/* INTSTATUS Register */
#define ARM_UART_TX_INTR  (1ul << 0)
#define ARM_UART_RX_INTR  (1ul << 1)

/* UART state definitions */
#define ARM_UART_INITIALIZED  (1ul << 0)

static void serial_format(struct arm_uart_dev_t* dev, int data_bits, SerialParity parity, int stop_bits)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;

    int parity_enable, parity_select;
    switch (parity) {
        case ParityNone: parity_enable = 0; parity_select = 0; break;
        case ParityOdd : parity_enable = 1; parity_select = 0; break;
        case ParityEven: parity_enable = 1; parity_select = 1; break;
        case ParityForced1: parity_enable = 1; parity_select = 2; break;
        case ParityForced0: parity_enable = 1; parity_select = 3; break;
        default:
            return;
    }

    switch (data_bits) {
        case 5: data_bits = 0; break;
        case 6: data_bits = 1; break;
        case 7: data_bits = 2; break;
        case 8: data_bits = 3; break;
        default:
            return;
    }

    switch (stop_bits) {
        case 1: stop_bits = 0; break;
        case 2: stop_bits = 1; break;
        default:
            return;
    }

    p_uart->LCR = data_bits            << 0
                | stop_bits            << 2
                | parity_enable        << 3
                | parity_select        << 4;

}


enum arm_uart_error_t arm_uart_init(struct arm_uart_dev_t* dev,
                                    uint32_t system_clk)
{
    struct _arm_uart_reg_map_t* p_uart =
                                 (struct _arm_uart_reg_map_t*)dev->cfg->base;
    if(system_clk == 0) {
        return ARM_UART_ERR_INVALID_ARG;
    }

    // Disable UART before changing registers
    // disable irqs
    p_uart->IER = 0 << 0  // Rx Data available irq enable
                | 0 << 1  // Tx Fifo empty irq enable
                | 0 << 2; // Rx Line Status irq enable

    /* Sets system clock */
    dev->data->system_clk = system_clk;

    /* Sets baudrate and format */
    arm_uart_set_baudrate(dev, dev->cfg->default_baudrate);
    serial_format(dev, 8, ParityNone, 1);

    dev->data->state = ARM_UART_INITIALIZED;

    return ARM_UART_ERR_NONE;
}

enum arm_uart_error_t arm_uart_set_baudrate(struct arm_uart_dev_t* dev,
                                            uint32_t baudrate)
{
    uint32_t bauddiv;
    struct _arm_uart_reg_map_t* p_uart =
                                 (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(baudrate == 0) {
        return ARM_UART_ERR_INVALID_BAUD;
    }

    /* Sets baudrate */
    bauddiv = (dev->data->system_clk / 16 / baudrate);
    dev->data->baudrate = baudrate;

    // set LCR[DLAB] to enable writing to divider registers
    p_uart->LCR |= (1 << 7);

    // set BDH and BDL
    p_uart->DLH = (bauddiv >> 8) & 0xff;
    p_uart->DLL = (bauddiv >> 0) & 0xff;

    p_uart->LCR &= ~(1 << 7);

    return ARM_UART_ERR_NONE;
}

uint32_t arm_uart_get_baudrate(struct arm_uart_dev_t* dev)
{
    return dev->data->baudrate;
}

enum arm_uart_error_t arm_uart_set_clock(struct arm_uart_dev_t* dev,
                                         uint32_t system_clk)
{
    if(system_clk == 0) {
        return ARM_UART_ERR_INVALID_ARG;
    }

    if(!(dev->data->state & ARM_UART_INITIALIZED)) {
        return ARM_UART_ERR_NOT_INIT;
    }

    /* Sets system clock */
    dev->data->system_clk = system_clk;

    /* Updates baudrate divider */
    arm_uart_set_baudrate(dev, dev->cfg->default_baudrate);

    /* Enables receiver and transmitter */
    return ARM_UART_ERR_NONE;
}

enum arm_uart_error_t arm_uart_read(struct arm_uart_dev_t* dev,
                                                                 uint8_t* byte)
{
    struct _arm_uart_reg_map_t* p_uart =
                                 (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(!(p_uart->LSR & ARM_UART_RX_BF)) {
        return ARM_UART_ERR_NOT_READY;
    }

    /* Reads data */
    *byte = (uint8_t)p_uart->RBR;

    return ARM_UART_ERR_NONE;
}

enum arm_uart_error_t arm_uart_write(struct arm_uart_dev_t* dev,
                                                                  uint8_t byte)
{
    struct _arm_uart_reg_map_t* p_uart =
                                 (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(!(p_uart->LSR & ARM_UART_TX_BF)) {
        return ARM_UART_ERR_NOT_READY;
    }

    /* Sends data */
    p_uart->THR = byte;

    return ARM_UART_ERR_NONE;
}

enum arm_uart_error_t arm_uart_irq_tx_enable(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                 (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(!(dev->data->state & ARM_UART_INITIALIZED)) {
        return ARM_UART_ERR_NOT_INIT;
    }

    p_uart->IER |= ARM_UART_TX_INTR_EN;

    return ARM_UART_ERR_NONE;
}

void arm_uart_irq_tx_disable(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                 (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(dev->data->state & ARM_UART_INITIALIZED ) {
        p_uart->IER &= ~ARM_UART_TX_INTR_EN;
    }
}

uint32_t arm_uart_tx_ready(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                 (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(!(dev->data->state & ARM_UART_INITIALIZED)) {
        return 0;
    }

    return (p_uart->LSR & ARM_UART_TX_BF);
}

enum arm_uart_error_t arm_uart_irq_rx_enable(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                 (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(!(dev->data->state & ARM_UART_INITIALIZED)) {
        return ARM_UART_ERR_NOT_INIT;
    }

    p_uart->IER |= ARM_UART_RX_INTR_EN;

    return ARM_UART_ERR_NONE;
}

void arm_uart_irq_rx_disable(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                 (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(dev->data->state & ARM_UART_INITIALIZED) {
        p_uart->IER &= ~ARM_UART_RX_INTR_EN;
    }
}

uint32_t arm_uart_rx_ready(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                  (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(!(dev->data->state & ARM_UART_INITIALIZED)) {
        return 0;
    }

    return (p_uart->LSR & ARM_UART_RX_BF);
}

void arm_uart_clear_interrupt(struct arm_uart_dev_t* dev,
                              enum arm_uart_irq_t irq)
{
    struct _arm_uart_reg_map_t* p_uart =
                                 (struct _arm_uart_reg_map_t*)dev->cfg->base;
    uint32_t IIR;

    if(dev->data->state & ARM_UART_INITIALIZED) {
        /* Clears pending interrupts */
        IIR = p_uart->IIR & 0xf;
    }
}
