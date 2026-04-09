/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __ZEPHYR_INCLUDE_DRIVERS_RTS5918_FLASH_API_EX_H__
#define __ZEPHYR_INCLUDE_DRIVERS_RTS5918_FLASH_API_EX_H__

#include <zephyr/device.h>

enum flash_rts5918_ex_ops {
	FLASH_RTS5918_EX_OP_WR_ENABLE = FLASH_EX_OP_VENDOR_BASE,
	FLASH_RTS5918_EX_OP_WR_DISABLE,
	FLASH_RTS5918_EX_OP_WR_SR,
	FLASH_RTS5918_EX_OP_WR_SR2,
	FLASH_RTS5918_EX_OP_RD_SR,
	FLASH_RTS5918_EX_OP_RD_SR2,
	FLASH_RTS5918_EX_OP_SET_WP,
	FLASH_RTS5918_EX_OP_GET_WP,
    FLASH_RTS5918_EX_OP_3BYTE_MODE,
    FLASH_RTS5918_EX_OP_4BYTE_MODE,
};

/*
 * Erase one sector on a SAF target flash. Implemented in
 * drivers/flash/flash_realtek_rts5918.c and called from the eSPI
 * SAF RX path in drivers/espi/espi_realtek_rts5918.c, so the
 * prototype lives in this shared header.
 */
void flash_rts5918_saf_erase_sector_handler(const struct device *dev,
					    const uint32_t address);

#endif /* __ZEPHYR_INCLUDE_DRIVERS_RTS5918_FLASH_API_EX_H__ */