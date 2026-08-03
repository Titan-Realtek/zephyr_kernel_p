/*
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * Copyright (c) 2025, Realtek Semiconductor Corp. All rights reserved.
 */

#ifndef RTK_I3C_UTIL_H_
#define RTK_I3C_UTIL_H_

#include "rtk_i3c.h"

#define MAX_TAGT_NAME_LEN 16

/** @brief I3C target item information mainted by controller. */
typedef struct rtk_i3c_util_tagt_item {
	char name[MAX_TAGT_NAME_LEN]; /**< Target name */
	bool active;                  /**< Indicate the target is active */
	rtk_i3c_tagt_info info;       /**< Target info */
} rtk_i3c_util_tagt_item;

void rtk_i3c_util_init_tagt_table(rtk_i3c_ctx *ctx, uint8_t tagt_id, const char *name,
				  uint8_t dyn_addr, uint8_t stc_addr, uint64_t pid);

void rtk_i3c_util_addr_slot_init(rtk_i3c_ctx *ctx);

bool rtk_i3c_util_get_tagt_addr(rtk_i3c_ctx *ctx, uint8_t tagt_id, uint8_t *dyn_addr,
				uint8_t *stc_addr);

void rtk_i3c_util_set_tagt_addr(rtk_i3c_ctx *ctx, uint8_t tagt_id, uint8_t dyn_addr,
				uint8_t stc_addr);

void rtk_i3c_util_handle_daa(rtk_i3c_ctx *ctx, rtk_i3c_tagt_char_info *const tagt_char_info,
			     uint8_t *dyn_addr);

int rtk_i3c_util_reattach_tagt(rtk_i3c_ctx *ctx, uint8_t old_da, uint8_t new_da, uint8_t new_sa);

bool rtk_i3c_util_is_addr_slot_occupied(rtk_i3c_ctx *ctx, uint8_t addr);
uint8_t rtk_i3c_util_alloc_addr(rtk_i3c_ctx *ctx);
void rtk_i3c_util_free_addr(rtk_i3c_ctx *ctx, uint8_t addr);

uint8_t rtk_i3c_util_calc_parity(uint8_t dyn_addr);

/**
 * @brief get 48 bits value from big endian
 *
 * @param val
 * @return uint64_t
 */
static inline uint64_t rtk_i3c_util_get_be48(uint8_t val[6])
{
	return (uint64_t)val[0] << 40 | (uint64_t)val[1] << 32 | (uint64_t)val[2] << 24 |
	       (uint64_t)val[3] << 16 | (uint64_t)val[4] << 8 | (uint64_t)val[5];
}

#endif
