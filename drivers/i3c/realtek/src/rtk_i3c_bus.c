/*
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * Copyright (c) 2025, Realtek Semiconductor Corp. All rights reserved.
 */

#include "rtk_i3c_bus.h"
#include "rtk_i3c.h"
#include "rtk_i3c_config.h"
#include "rtk_i3c_internal.h"
#include "rtk_logging.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#ifdef CONFIG_RTK_I3C_CTRL
#define START_DYN_ADDR 8
#define I3C_PID_MASK   ((1ULL << 48) - 1)

/* Each item of array addr_slot has length of 32 bits */
#define I3C_ADDR_SLOT_BITS 32
/* Total size of array addr_slot */
#define I3C_ADDR_SLOT_SIZE ROUND_UP(RTK_I3C_MAX_DYN_ADDR, I3C_ADDR_SLOT_BITS)

typedef enum {
	SLOT_FREE = 0,
	SLOT_OCCUPY,
} slot_ops;

int rtk_i3c_bus_find_tagt_by_char_info(rtk_i3c_bus_tagt_item *tagt_table,
				       rtk_i3c_tagt_char_info *tagt_char_info)
{
	for (int i = 0; i < RTK_I3C_MAX_TAGT_COUNT; i++) {
		if (tagt_table[i].info.char_info.pid == tagt_char_info->pid) {
			if (tagt_table[i].active) {
				LOG_WRN("Find target %d is active!\n", i);
			} else {
				tagt_table[i].info.char_info = *tagt_char_info;
			}
			return i;
		}
	}
	return -1;
}

int rtk_i3c_bus_find_tagt_by_addr(rtk_i3c_bus_tagt_item *tagt_table, uint8_t dyn_addr)
{
	for (int i = 0; i < RTK_I3C_MAX_TAGT_COUNT; i++) {
		if (tagt_table[i].info.dyn_addr == dyn_addr) {
			if (tagt_table[i].active) {
				LOG_WRN("Find target %d is active!\n", i);
			}
			return i;
		}
	}
	return -1;
}

int rtk_i3c_bus_alloc_tagt(rtk_i3c_bus_tagt_item *tagt_table,
			   rtk_i3c_tagt_char_info *tagt_char_info)
{
	for (int i = 0; i < RTK_I3C_MAX_TAGT_COUNT; i++) {
		if (!tagt_table[i].active && !tagt_table[i].info.is_i2c) {
			tagt_table[i].info.char_info = *tagt_char_info;
			return i;
		}
	}
	return -1;
}

/**
 * @brief Change addr slots status
 *
 * @param addr dynamic addr
 * @param slot_ops 1 to occupy, 0 to free
 */
static void rtk_i3c_bus_set_addr_slot(uint32_t *addr_slot, uint8_t addr, slot_ops slot_ops)
{
	int bit_idx = addr / I3C_ADDR_SLOT_BITS;
	int bit_off = addr % I3C_ADDR_SLOT_BITS;

	BIT_FIELD_SET(addr_slot[bit_idx], bit_off, bit_off, slot_ops);
}

/**********************************************************************************************************************
 * Utilities Functions
 **********************************************************************************************************************/
bool rtk_i3c_bus_is_addr_slot_occupied(rtk_i3c_ctx *ctx, uint8_t addr)
{
	ASSERT(ctx != NULL && ctx->cfg != NULL && ctx->cfg->addr_slot != NULL);
	if (addr > RTK_I3C_MAX_DYN_ADDR) {
		return true;
	}

	uint32_t *addr_slot = (uint32_t *)ctx->cfg->addr_slot;

	int bit_idx = addr / I3C_ADDR_SLOT_BITS;
	int bit_off = addr % I3C_ADDR_SLOT_BITS;

	return BIT_FIELD_GET(addr_slot[bit_idx], bit_off, bit_off);
}

uint8_t rtk_i3c_bus_alloc_addr(rtk_i3c_ctx *ctx)
{
	ASSERT(ctx != NULL && ctx->cfg != NULL && ctx->cfg->addr_slot != NULL);
	uint32_t *addr_slot = (uint32_t *)ctx->cfg->addr_slot;

	for (int i = START_DYN_ADDR; i <= RTK_I3C_MAX_DYN_ADDR; i++) {
		if (!rtk_i3c_bus_is_addr_slot_occupied(ctx, i)) {
			rtk_i3c_bus_set_addr_slot(addr_slot, i, SLOT_OCCUPY);
			return i;
		}
	}

	return 0;
}

void rtk_i3c_bus_free_addr(rtk_i3c_ctx *ctx, uint8_t addr)
{
	ASSERT(ctx != NULL && ctx->cfg != NULL && ctx->cfg->addr_slot != NULL);
	uint32_t *addr_slot = (uint32_t *)ctx->cfg->addr_slot;

	rtk_i3c_bus_set_addr_slot(addr_slot, addr, SLOT_FREE);
}

void rtk_i3c_bus_addr_slot_init(rtk_i3c_ctx *ctx)
{
	ASSERT(ctx != NULL && ctx->cfg != NULL && ctx->cfg->addr_slot != NULL);
	uint32_t *addr_slot = (uint32_t *)ctx->cfg->addr_slot;

	memset(addr_slot, 0, I3C_ADDR_SLOT_SIZE * sizeof(addr_slot[0]));

	/* Reserve I3C prohibited addresses per MIPI-I3C v1.1.1 Section 5.1.2.2.5 */
	/* I3C reserved addresses: 0x00, 0x01, 0x02, 0x7E, 0x7F */
	rtk_i3c_bus_set_addr_slot(addr_slot, 0x00, SLOT_OCCUPY);
	rtk_i3c_bus_set_addr_slot(addr_slot, 0x01, SLOT_OCCUPY);
	rtk_i3c_bus_set_addr_slot(addr_slot, 0x02, SLOT_OCCUPY);
	rtk_i3c_bus_set_addr_slot(addr_slot, 0x7E, SLOT_OCCUPY);
	rtk_i3c_bus_set_addr_slot(addr_slot, 0x7F, SLOT_OCCUPY);

	/* Single-bit error detection addresses (adjacent to 0x7E) */
	rtk_i3c_bus_set_addr_slot(addr_slot, 0x3E, SLOT_OCCUPY);
	rtk_i3c_bus_set_addr_slot(addr_slot, 0x5E, SLOT_OCCUPY);
	rtk_i3c_bus_set_addr_slot(addr_slot, 0x6E, SLOT_OCCUPY);
	rtk_i3c_bus_set_addr_slot(addr_slot, 0x76, SLOT_OCCUPY);
	rtk_i3c_bus_set_addr_slot(addr_slot, 0x7A, SLOT_OCCUPY);
	rtk_i3c_bus_set_addr_slot(addr_slot, 0x7C, SLOT_OCCUPY);
}

bool rtk_i3c_bus_get_tagt_addr(rtk_i3c_ctx *ctx, uint8_t tagt_id, uint8_t *dyn_addr,
			       uint16_t *stc_addr)
{
	ASSERT(ctx != NULL && ctx->cfg != NULL && ctx->cfg->tagt_table != NULL);

	rtk_i3c_bus_tagt_item *tagt_table = (rtk_i3c_bus_tagt_item *)ctx->cfg->tagt_table;
	if (tagt_id >= RTK_I3C_MAX_TAGT_COUNT) {
		return false;
	}
	if (dyn_addr) {
		*dyn_addr = tagt_table[tagt_id].info.dyn_addr;
	}
	if (stc_addr) {
		*stc_addr = tagt_table[tagt_id].info.stc_addr;
	}
	return true;
}

void rtk_i3c_bus_set_tagt_addr(rtk_i3c_ctx *ctx, uint8_t tagt_id, uint8_t dyn_addr,
			       uint16_t stc_addr)
{
	ASSERT(ctx != NULL && ctx->cfg != NULL && ctx->cfg->tagt_table != NULL);

	rtk_i3c_bus_tagt_item *tagt_table = (rtk_i3c_bus_tagt_item *)ctx->cfg->tagt_table;
	if (tagt_id >= RTK_I3C_MAX_TAGT_COUNT) {
		return;
	}

	if (dyn_addr != 0) {
		tagt_table[tagt_id].info.dyn_addr = dyn_addr;
	}

	if (stc_addr != 0) {
		tagt_table[tagt_id].info.stc_addr = stc_addr;
	}
}

int rtk_i3c_bus_get_bcr_by_addr(rtk_i3c_ctx *ctx, uint8_t dyn_addr)
{
	ASSERT(ctx != NULL && ctx->cfg != NULL && ctx->cfg->tagt_table != NULL);
	rtk_i3c_bus_tagt_item *tagt_table = ctx->cfg->tagt_table;
	int tagt_id = -1;
	if ((tagt_id = rtk_i3c_bus_find_tagt_by_addr(tagt_table, dyn_addr)) < 0) {
		LOG_ERR("Target addr: %" PRIx8 " not found!\n", dyn_addr);
		return RTK_I3C_TAGT_NOT_FOUND;
	}

	return tagt_table[tagt_id].info.char_info.bcr;
}

int rtk_i3c_bus_reattach_tagt(rtk_i3c_ctx *ctx, uint8_t old_da, uint8_t new_da, uint16_t new_sa)
{
	ASSERT(ctx != NULL && ctx->cfg != NULL && ctx->cfg->tagt_table != NULL);
	ASSERT(new_da != 0 || new_sa != 0);

	rtk_i3c_bus_tagt_item *find_tagt = NULL;
	rtk_i3c_bus_tagt_item *tagt_table = ctx->cfg->tagt_table;
	int find_tagt_id = -1;

	/*TODO: find target by id */
	if ((find_tagt_id = rtk_i3c_bus_find_tagt_by_addr(ctx->cfg->tagt_table, old_da)) < 0) {
		LOG_ERR("Target addr: %" PRIx8 " not found!\n", old_da);
		return RTK_I3C_TAGT_NOT_FOUND;
	}

	find_tagt = &tagt_table[find_tagt_id];
	rtk_i3c_bus_set_addr_slot(ctx->cfg->addr_slot, old_da, SLOT_FREE);
	rtk_i3c_bus_set_addr_slot(ctx->cfg->addr_slot, new_da, SLOT_OCCUPY);
	if (new_da) {
		find_tagt->info.dyn_addr = new_da;
	}
	if (new_sa) {
		find_tagt->info.stc_addr = new_sa;
	}
	return 0;
}

void rtk_i3c_bus_handle_daa(rtk_i3c_ctx *ctx, rtk_i3c_tagt_char_info *const tagt_char_info,
			    uint8_t *dyn_addr)
{
	ASSERT(dyn_addr != NULL);
	ASSERT(ctx != NULL && ctx->cfg != NULL && ctx->cfg->tagt_table != NULL);
	ASSERT(ctx != NULL && ctx->cfg != NULL && ctx->cfg->addr_slot != NULL);

	rtk_i3c_bus_tagt_item *tagt_table = (rtk_i3c_bus_tagt_item *)ctx->cfg->tagt_table;
	int find_tagt_id = -1;
	rtk_i3c_bus_tagt_item *find_tagt = NULL;
	rtk_i3c_tagt_info *find_tagt_info = NULL;

	if ((find_tagt_id = rtk_i3c_bus_find_tagt_by_char_info(tagt_table, tagt_char_info)) < 0) {
		/* find and occupy an non-active target slot */
		find_tagt_id = rtk_i3c_bus_alloc_tagt(tagt_table, tagt_char_info);
		if (find_tagt_id < 0) {
			LOG_ERR("Alloc new target slot failed! \n");
			goto exit_error;
		}
	}

	find_tagt = &tagt_table[find_tagt_id];
	find_tagt_info = &find_tagt->info;

	if (find_tagt_info->dyn_addr == 0) {
		find_tagt_info->dyn_addr = rtk_i3c_bus_alloc_addr(ctx);
	}

	if (find_tagt_info->dyn_addr != 0) {
		find_tagt->active = true;
		*dyn_addr = find_tagt_info->dyn_addr;
		rtk_i3c_core_set_mdb(ctx->core, find_tagt_info->dyn_addr,
				     BIT_FIELD_GET(find_tagt_info->char_info.bcr, 2, 2));
		LOG_INF("Target PID: 0x%" PRIx64 " has assigned to dynamic address: "
			"0x%02x with target table slot: %d\n",
			(uint64_t)tagt_char_info->pid, *dyn_addr, find_tagt_id);
		return;
	}

exit_error:
	LOG_ERR("Target PID: %" PRIx64 " failed\n", (uint64_t)tagt_char_info->pid);
}

int rtk_i3c_bus_init_tagt_table(rtk_i3c_ctx *ctx, uint8_t tagt_id, const char *name,
				uint8_t dyn_addr, uint16_t stc_addr, uint64_t pid, bool is_i2c)
{
	ASSERT(tagt_id < RTK_I3C_MAX_TAGT_COUNT);
	ASSERT(ctx != NULL && ctx->cfg != NULL && ctx->cfg->tagt_table != NULL);
	ASSERT(ctx != NULL && ctx->cfg != NULL && ctx->cfg->addr_slot != NULL);
	ASSERT(is_i2c == 1 || dyn_addr != 0);

	rtk_i3c_bus_tagt_item *tagt_table = (rtk_i3c_bus_tagt_item *)ctx->cfg->tagt_table;
	uint32_t *addr_slot = (uint32_t *)ctx->cfg->addr_slot;

	if (name != NULL) {
		for (int i = 0; i < I3C_BUS_TAGT_NAME_LEN; i++) {
			tagt_table[tagt_id].name[i] = name[i];
			if (name[i] == (uint8_t)0x0) {
				break;
			}
		}
	}

	if (pid != 0) {
		tagt_table[tagt_id].info.char_info.pid = pid & I3C_PID_MASK;
	}

	if (!is_i2c) {
		if (rtk_i3c_bus_is_addr_slot_occupied(ctx, dyn_addr)) {
			LOG_ERR("addr: 0x%02x is occupied!\n", dyn_addr);
			return RTK_I3C_ADDR_OCCUPIED;
		}
		rtk_i3c_bus_set_addr_slot(addr_slot, dyn_addr, SLOT_OCCUPY);
	} else {
		tagt_table[tagt_id].info.is_i2c = 1;
	}

	/*
	 * Only reserve a real static address. stc_addr == 0 means "no static
	 * address" (ENTDAA/hot-join-only device); it is a reserved I3C address, not
	 * a slot to occupy -- reserving it would make a second static-less device
	 * collide on 0x00. Also skip when stc_addr == dyn_addr (SETDASA case: the
	 * slot was just marked above as dyn_addr), so we do not flag the device's
	 * own address as occupied.
	 */
	if (stc_addr != 0 && stc_addr <= RTK_I3C_MAX_DYN_ADDR && stc_addr != dyn_addr) {
		if (rtk_i3c_bus_is_addr_slot_occupied(ctx, stc_addr)) {
			LOG_ERR("addr: 0x%02x is occupied!\n", stc_addr);
			return RTK_I3C_ADDR_OCCUPIED;
		}
		rtk_i3c_bus_set_addr_slot(addr_slot, stc_addr, SLOT_OCCUPY);
	}
	rtk_i3c_bus_set_tagt_addr(ctx, tagt_id, dyn_addr, stc_addr);

	return 0;
}

#endif /* CONFIG_RTK_I3C_CTRL */
