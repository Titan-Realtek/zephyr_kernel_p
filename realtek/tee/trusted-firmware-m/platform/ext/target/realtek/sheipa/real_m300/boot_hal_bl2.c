/*
 * Copyright (c) 2024, Realtek. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * RTS5918 boot HAL for BL2 (MCUboot).
 *
 * Unlike the AN521 reference this platform was cloned from, the RTS5918 SPI
 * NOR flash is NOT memory-mapped: it is reached through the SPIC controller in
 * command mode (see cmsis_drivers/Driver_Flash.c) and cannot be executed in
 * place. MCUboot validates each image directly from flash, but the validated
 * image must be COPIED into its linked SRAM execution address before we hand
 * control over, and the jump address computed by do_boot() must point at that
 * SRAM copy rather than at the raw flash offset.
 *
 * We accomplish both here:
 *   - boot_platform_post_load(): copy each validated primary slot from flash
 *     into the SRAM address the image was linked/signed for.
 *   - flash_device_base(): return the SRAM alias base so do_boot() computes
 *     vt = base + image_off + hdr_size = S_CODE_START in SRAM.
 *
 * Only headers guaranteed to be on the platform_bl2 include path are used
 * (boot_hal.h / Driver_Flash.h / flash_layout.h / region_defs.h), so flash is
 * accessed through the CMSIS Driver_Flash API instead of the mcuboot flash_map
 * layer (whose headers are private to the bl2 target).
 */

#include <stdint.h>
#include "boot_hal.h"
#include "Driver_Flash.h"
#include "flash_layout.h"
#include "region_defs.h"
#include "tfm_hal_device_header.h"

extern ARM_DRIVER_FLASH FLASH_DEV_NAME;

/*
 * SRAM header address each image is linked/signed to run from, verified
 * against the signed .hex files:
 *   S  : 0x2000B000  (S_IMAGE_LOAD_BASE 0x20000000 + FLASH_AREA_0_OFFSET 0xB000)
 *   NS : 0x20038000  (0x20000000 + FLASH_AREA_1_OFFSET 0x38000)
 *
 * Both runtime images use the clean 0x20000000 SRAM flash-alias base so their
 * vector tables are 128-byte aligned (S vectors land at 0x2000B400, NS at
 * 0x20038400). They must NOT use S_ROM_ALIAS_BASE (0x20000020): that carries
 * BL2's 0x20-byte RLTK header offset, which is only meaningful to the bootROM
 * that loads BL2 - see region_defs.h S_IMAGE_LOAD_BASE. Cortex-M33 VTOR[6:0]
 * are RES0, so a 0x2000B420 vector base is masked to 0x2000B400 and every
 * exception vector shifts down 8 slots (the "Reserved Exception 0x0B" crash).
 *
 * NOTE: the Zephyr NS image is linked with base 0x20000000. region_defs.h
 * NS_ROM_ALIAS_BASE (0x10000000) is stale and NOT used here.
 */
#define NS_SRAM_LOAD_BASE   (0x20000000u)

/* MCUboot image_header prefix - only the fields needed to bound the copy. */
struct rtk_image_header {
    uint32_t ih_magic;            /* 0x00 */
    uint32_t ih_load_addr;        /* 0x04 */
    uint16_t ih_hdr_size;         /* 0x08 */
    uint16_t ih_protect_tlv_size; /* 0x0A */
    uint32_t ih_img_size;         /* 0x0C */
    uint32_t ih_flags;            /* 0x10 */
};

/*
 * do_boot() computes the jump address as:
 *     vt = flash_base + rsp->br_image_off + rsp->br_hdr->ih_hdr_size
 * Returning the clean secure SRAM load base makes that resolve to S_CODE_START
 * (0x20000000 + 0xB000 + 0x400 = 0x2000B400), i.e. the SRAM copy produced by
 * boot_platform_post_load(). This MUST match region_defs.h S_IMAGE_LOAD_BASE,
 * NOT S_ROM_ALIAS_BASE (0x20000020) which would put the vector table at the
 * VTOR-misaligned 0x2000B420. flash_device_base() is used nowhere else (flash
 * reads go through the 0-based Driver_Flash API), so this override is safe.
 */
int flash_device_base(uint8_t fd_id, uintptr_t *ret)
{
    (void)fd_id;
    *ret = S_IMAGE_LOAD_BASE;   /* 0x20000000 -> S_CODE_START 0x2000B400 */
    return 0;
}

int boot_platform_post_load(uint32_t image_id)
{
    struct rtk_image_header hdr;
    uint32_t flash_off;
    uintptr_t dst;
    uint32_t copy_len;
    int32_t  rc;

    switch (image_id) {
    case 0: /* Secure */
        flash_off = FLASH_AREA_0_OFFSET;                              /* 0xB000  */
        dst = (uintptr_t)(S_IMAGE_LOAD_BASE + FLASH_AREA_0_OFFSET);   /* 0x2000B000 */
        break;
    case 1: /* Non-secure */
        flash_off = FLASH_AREA_1_OFFSET;                              /* 0x38000 */
        dst = (uintptr_t)(NS_SRAM_LOAD_BASE + FLASH_AREA_1_OFFSET);   /* 0x20038000 */
        break;
    default:
        return 0;
    }

    /* Read the MCUboot header to bound the copy to header + image body. */
    rc = FLASH_DEV_NAME.ReadData(flash_off, &hdr, sizeof(hdr));
    if (rc < 0 || (uint32_t)rc != sizeof(hdr)) {
        return -1;
    }

    /* TLVs are only needed for verification (already done), so skip them. */
    copy_len = (uint32_t)hdr.ih_hdr_size + hdr.ih_img_size;

    rc = FLASH_DEV_NAME.ReadData(flash_off, (void *)dst, copy_len);
    if (rc < 0 || (uint32_t)rc != copy_len) {
        return -1;
    }

    __DSB();
    __ISB();
    return 0;
}
