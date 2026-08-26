#ifndef PORT_I3C_H_
#define PORT_I3C_H_

#include "rtk_i3c.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/**********************************************************************************************************************
 * Portable Layer
 **********************************************************************************************************************/

/**
 * @brief I3C interrupt service routine
 *
 * This function is the main ISR dispatcher for the I3C peripheral.
 * It handles various I3C events including:
 * - RX FIFO not empty (RXNE)
 * - RX NACK (RXNAK)
 * - Transfer done (DONE)
 * - IBI interrupts (RXIBI, HJ, CR, DAF)
 *
 * @param ctx Pointer to the I3C context structure
 */
void rtk_i3c_isr(rtk_i3c_ctx *ctx);

/**
 * @brief Get the base address of I3C peripheral
 *
 * Returns the hardware register base address for the specified I3C instance.
 * This is a platform-specific function that must be implemented by the BSP.
 *
 * @param instance_id The I3C peripheral instance number (e.g., 0, 1, 2)
 * @return uintptr_t Base address of the I3C peripheral registers, or 0 if invalid
 */
uintptr_t plat_i3c_get_base(uint8_t instance_id);

/**
 * @brief Initialize I3C interrupt
 *
 * Configures and enables the I3C interrupt.
 * This is a platform-specific function that must be implemented by the BSP.
 *
 * @param ctx Pointer to the I3C context structure
 * @param vector Interrupt handler function pointer to register
 */
void plat_i3c_init_isr(rtk_i3c_ctx *ctx, uintptr_t vector);

/**
 * @brief Post-power-on I3C hardware initialisation
 *
 * Called from the middle layer (mdl_i3c_hub) AFTER B_ON has been asserted
 * by IO_SET_POutBOn. Performs pinctrl, clock, IRQ, i3c_configure, bus init,
 * DAA, and Hot-Join enable. Safe to call multiple times.
 *
 * @param dev I3C device instance
 * @return 0 on success, negative errno on failure
 */
int i3c_realtek_hw_reinit(const struct device *dev);

#endif
