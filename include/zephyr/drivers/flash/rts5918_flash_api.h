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
	/*
	 * Select the chip-select line used by subsequent flash_* ops on
	 * this controller. `in` carries the CS index (0 or 1). The driver
	 * maps it to SER = BIT(cs). Per-controller state, protected by the
	 * driver's internal sem so read/write/erase on different CS cannot
	 * interleave. Caller is responsible for re-selecting after power
	 * transitions.
	 */
	FLASH_RTS5918_EX_OP_SELECT_CS,
	/*
	 * W25Q-style Security Register access (vendor SPI commands 0x48 /
	 * 0x42 / 0x44). Winbond-compatible NOR chips expose three 256-byte
	 * pages at addresses 0x001000 / 0x002000 / 0x003000 that live
	 * outside the main array and are addressed only via these
	 * dedicated opcodes. `in` points to a `struct
	 * flash_rts5918_sec_reg_op`; for ERASE the `length` and `buf`
	 * fields are ignored and the `address` picks the 256-byte page
	 * to wipe. The driver handles WREN + WIP polling internally.
	 */
	FLASH_RTS5918_EX_OP_SEC_REG_READ,
	FLASH_RTS5918_EX_OP_SEC_REG_WRITE,
	FLASH_RTS5918_EX_OP_SEC_REG_ERASE,
};

/**
 * Payload passed to FLASH_RTS5918_EX_OP_SEC_REG_{READ,WRITE,ERASE} via
 * the ex_op `in` parameter.
 *
 * address : 24-bit W25Q-style Security Register address. Upper byte is
 *           the page index (0x01/0x02/0x03), lower byte is the offset
 *           within the 256-byte page.
 * length  : number of bytes to transfer; capped at 256 per the W25Q
 *           spec. Ignored for ERASE.
 * buf     : read-into buffer (READ) or write-from buffer (WRITE).
 *           Ignored for ERASE.
 */
struct flash_rts5918_sec_reg_op {
	uint32_t address;
	uint32_t length;
	void *buf;
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