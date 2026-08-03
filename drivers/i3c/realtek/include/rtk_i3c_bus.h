/*
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * Copyright (c) 2025, Realtek Semiconductor Corp. All rights reserved.
 */

#ifndef RTK_I3C_BUS_H_
#define RTK_I3C_BUS_H_

#include "rtk_i3c.h"

#define I3C_BUS_TAGT_NAME_LEN 16

/** @brief I3C target item information mainted by controller. */
typedef struct rtk_i3c_bus_tagt_item {
	char name[I3C_BUS_TAGT_NAME_LEN]; /**< Target name */
	bool active;                      /**< Indicate the target is active */
	rtk_i3c_tagt_info info;           /**< Target info */
} rtk_i3c_bus_tagt_item;

/**
 * @brief Initialize target table entry
 *
 * @param ctx Pointer to I3C context
 * @param tagt_id Target ID index
 * @param name Target name string
 * @param dyn_addr Dynamic address (0 if not assigned)
 * @param stc_addr Static address (0 if none)
 * @param pid Provisioned ID (48-bit)
 * @param is_i2c True if I2C device, false if I3C device
 * @return 0 on success, error code on failure
 */
int rtk_i3c_bus_init_tagt_table(rtk_i3c_ctx *ctx, uint8_t tagt_id, const char *name,
				uint8_t dyn_addr, uint16_t stc_addr, uint64_t pid, bool is_i2c);

/**
 * @brief Get target address
 *
 * @param ctx Pointer to I3C context
 * @param tagt_id Target ID index
 * @param dyn_addr Pointer to store dynamic address (can be NULL)
 * @param stc_addr Pointer to store static address (can be NULL)
 * @return true on success, false on failure
 */
bool rtk_i3c_bus_get_tagt_addr(rtk_i3c_ctx *ctx, uint8_t tagt_id, uint8_t *dyn_addr,
			       uint16_t *stc_addr);

/**
 * @brief Set target address
 *
 * @param ctx Pointer to I3C context
 * @param tagt_id Target ID index
 * @param dyn_addr Dynamic address
 * @param stc_addr Static address
 */
void rtk_i3c_bus_set_tagt_addr(rtk_i3c_ctx *ctx, uint8_t tagt_id, uint8_t dyn_addr,
			       uint16_t stc_addr);

/**
 * @brief Handle DAA (Dynamic Address Assignment) phase
 *
 * Called during ENTDAA to process target characteristics and assign address
 *
 * @param ctx Pointer to I3C context
 * @param tagt_char_info Target characteristics info
 * @param dyn_addr Pointer to store assigned dynamic address
 */
void rtk_i3c_bus_handle_daa(rtk_i3c_ctx *ctx, rtk_i3c_tagt_char_info *const tagt_char_info,
			    uint8_t *dyn_addr);

/**
 * @brief Re-attach target with new address. Used only when new_da is assigned.
 *
 * @param ctx Pointer to I3C context
 * @param old_da Old dynamic address
 * @param new_da New dynamic address
 * @param new_sa New static address (0 if none)
 * @return 0 on success, error code on failure
 */
int rtk_i3c_bus_reattach_tagt(rtk_i3c_ctx *ctx, uint8_t old_da, uint8_t new_da, uint16_t new_sa);

/**
 * @brief Check if address slot is occupied
 *
 * @param ctx Pointer to I3C context
 * @param addr Address to check
 * @return true if occupied, false if free
 */
bool rtk_i3c_bus_is_addr_slot_occupied(rtk_i3c_ctx *ctx, uint8_t addr);

/**
 * @brief Allocate a free dynamic address
 *
 * @param ctx Pointer to I3C context
 * @return Allocated address, 0 if none available
 */
uint8_t rtk_i3c_bus_alloc_addr(rtk_i3c_ctx *ctx);

/**
 * @brief Free a dynamic address
 *
 * @param ctx Pointer to I3C context
 * @param addr Address to free
 */
void rtk_i3c_bus_free_addr(rtk_i3c_ctx *ctx, uint8_t addr);

/**
 * @brief Initialize address slot bitmap
 *
 * @param ctx Pointer to I3C context
 */
void rtk_i3c_bus_addr_slot_init(rtk_i3c_ctx *ctx);

#endif
