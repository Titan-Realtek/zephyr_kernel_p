/*
 * Copyright (c) 2017-2022 Arm Limited. All rights reserved.
 * Copyright (c) 2020 Cypress Semiconductor Corporation. All rights reserved.
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

#ifndef __FLASH_LAYOUT_H__
#define __FLASH_LAYOUT_H__

/* Flash layout on MPS2 AN521 with BL2 (multiple image boot):
 *
 * 0x0000_0000 BL2 - MCUBoot (0.5 MB)
 * 0x0008_0000 Secure image     primary slot (0.5 MB)
 * 0x0010_0000 Non-secure image primary slot (0.5 MB)
 * 0x0018_0000 Secure image     secondary slot (0.5 MB)
 * 0x0020_0000 Non-secure image secondary slot (0.5 MB)
 * 0x0028_0000 Scratch area (0.5 MB)
 * 0x0030_0000 Protected Storage Area (20 KB)
 * 0x0030_5000 Internal Trusted Storage Area (16 KB)
 * 0x0030_9000 OTP / NV counters area (8 KB)
 * 0x0030_B000 Unused (980 KB)
 *
 * Flash layout on MPS2 AN521 with BL2 (single image boot):
 *
 * 0x0000_0000 BL2 - MCUBoot (0.5 MB)
 * 0x0008_0000 Primary image area (1 MB):
 *    0x0008_0000 Secure     image primary
 *    0x0010_0000 Non-secure image primary
 * 0x0018_0000 Secondary image area (1 MB):
 *    0x0018_0000 Secure     image secondary
 *    0x0020_0000 Non-secure image secondary
 * 0x0028_0000 Scratch area (1 MB)
 * 0x0038_0000 Protected Storage Area (20 KB)
 * 0x0038_5000 Internal Trusted Storage Area (16 KB)
 * 0x0038_9000 OTP / NV counters area (8 KB)
 * 0x0038_B000 Unused (468 KB)
 *
 * Flash layout on MPS2 AN521, if BL2 not defined:
 *
 * 0x0000_0000 Secure     image (1 MB)
 * 0x0010_0000 Non-secure image (1 MB)
 */

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_retarget.h to access flash related defines. To resolve this
 * some of the values are redefined here with different names, these are marked
 * with comment.
 */

/* Size of a Secure and of a Non-secure image */
#define FLASH_S_PARTITION_SIZE          (0x2D000) /* S partition: 180 KB */
#define FLASH_NS_PARTITION_SIZE         (0x70000) /* NS partition: 448 KB */

#if (FLASH_S_PARTITION_SIZE > FLASH_NS_PARTITION_SIZE)
#define FLASH_MAX_PARTITION_SIZE FLASH_S_PARTITION_SIZE
#else
#define FLASH_MAX_PARTITION_SIZE FLASH_NS_PARTITION_SIZE
#endif
/* Sector size of the flash hardware; same as FLASH0_SECTOR_SIZE */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x1000)     /* 4 KB */
/* Same as FLASH0_SIZE */
#define FLASH_TOTAL_SIZE                (0x00400000) /* 4 MB */

/* Flash layout info for BL2 bootloader */
/* Same as FLASH0_BASE_S */
/*
 * All FLASH_AREA_*_OFFSET below are region-relative (start at 0). The EC image
 * lives at a physical byte offset inside the larger host-shared BIOS SPI NOR;
 * that offset is applied at the SPI transfer layer by FLASH_REGION_BASE_OFFSET
 * in cmsis_drivers/Driver_Flash.c (currently 0x10FC000), NOT here. Keep this at
 * 0x0 so the SRAM-alias arithmetic and MCUboot flash_map stay unchanged.
 */
#define FLASH_BASE_ADDRESS              (0x000)

/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_PRIMARY and
 * IMAGE_SECONDARY, SCRATCH is used as a temporary storage during image
 * swapping.
 */
#define FLASH_AREA_BL2_OFFSET      (0x0)
/* 0xB000 (44K): signed bl2.rts5918.bin = 0xA057 (raw 0x9F30 + RLTK hdr 0x20 +
 * ECDSA TLV) overran the old 0xA000 by 0x57 into the S slot. Grew BL2 by one
 * 0x1000 sector; the SRAM cost is reclaimed by trimming NS data 0x1B000->0x1A000
 * (see S/NS_RAM_ALIAS_BASE below and rts5918.dtsi sram0). */
#define FLASH_AREA_BL2_SIZE        (0xB000)

#if !defined(MCUBOOT_IMAGE_NUMBER) || (MCUBOOT_IMAGE_NUMBER == 1)
/* Secure + Non-secure image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)
/* Secure + Non-secure secondary slot */
#define FLASH_AREA_2_ID            (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_2_OFFSET        (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_2_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)
/* Scratch area */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)
/* The maximum number of status entries supported by the bootloader. */
#define MCUBOOT_STATUS_MAX_ENTRIES ((FLASH_S_PARTITION_SIZE + \
                                     FLASH_NS_PARTITION_SIZE) / \
                                    FLASH_AREA_SCRATCH_SIZE)
/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS    ((FLASH_S_PARTITION_SIZE + \
                                     FLASH_NS_PARTITION_SIZE) / \
                                    FLASH_AREA_IMAGE_SECTOR_SIZE)
#elif (MCUBOOT_IMAGE_NUMBER == 2)
/* Secure image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE)
/* Non-secure image primary slot */
#define FLASH_AREA_1_ID            (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_1_OFFSET        (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_1_SIZE          (FLASH_NS_PARTITION_SIZE)
/* Secure image secondary slot */
#define FLASH_AREA_2_ID            (FLASH_AREA_1_ID + 1)
#define FLASH_AREA_2_OFFSET        (FLASH_AREA_1_OFFSET + FLASH_AREA_1_SIZE)
#define FLASH_AREA_2_SIZE          (FLASH_S_PARTITION_SIZE)
/* Non-secure image secondary slot */
#define FLASH_AREA_3_ID            (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_3_OFFSET        (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_3_SIZE          (FLASH_NS_PARTITION_SIZE)
/* Scratch area */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_3_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_3_OFFSET + FLASH_AREA_3_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (FLASH_MAX_PARTITION_SIZE)
/* The maximum number of status entries supported by the bootloader. */
#define MCUBOOT_STATUS_MAX_ENTRIES (FLASH_MAX_PARTITION_SIZE / \
                                    FLASH_AREA_SCRATCH_SIZE)
/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS    (FLASH_MAX_PARTITION_SIZE / \
                                    FLASH_AREA_IMAGE_SECTOR_SIZE)
#else /* MCUBOOT_IMAGE_NUMBER > 2 */
#error "Only MCUBOOT_IMAGE_NUMBER 1 and 2 are supported!"
#endif /* MCUBOOT_IMAGE_NUMBER */

/* Protected Storage (PS) Service definitions */
#define FLASH_PS_AREA_OFFSET            (FLASH_AREA_SCRATCH_OFFSET + \
                                         FLASH_AREA_SCRATCH_SIZE)
#define FLASH_PS_AREA_SIZE              (0x5000)   /* 20 KB */

/* Internal Trusted Storage (ITS) Service definitions */
#define FLASH_ITS_AREA_OFFSET           (FLASH_PS_AREA_OFFSET + \
                                         FLASH_PS_AREA_SIZE)
#define FLASH_ITS_AREA_SIZE             (0x4000)   /* 16 KB */

/* OTP_definitions */
#define FLASH_OTP_NV_COUNTERS_AREA_OFFSET (FLASH_ITS_AREA_OFFSET + \
                                           FLASH_ITS_AREA_SIZE)
#define FLASH_OTP_NV_COUNTERS_AREA_SIZE   (FLASH_AREA_IMAGE_SECTOR_SIZE * 2)
#define FLASH_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_AREA_IMAGE_SECTOR_SIZE

/* Offset and size definition in flash area used by assemble.py */
#define SECURE_IMAGE_OFFSET             (0x0)
#define SECURE_IMAGE_MAX_SIZE           FLASH_S_PARTITION_SIZE

#define NON_SECURE_IMAGE_OFFSET         (SECURE_IMAGE_OFFSET + \
                                         SECURE_IMAGE_MAX_SIZE)
#define NON_SECURE_IMAGE_MAX_SIZE       FLASH_NS_PARTITION_SIZE

/* Flash device name used by BL2
 * Name is defined in flash driver file: Driver_Flash.c
 */
#define FLASH_DEV_NAME Driver_FLASH0
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_FLASH_PROGRAM_UNIT       (0x1)

/* Protected Storage (PS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M PS Integration Guide.
 */
#define TFM_HAL_PS_FLASH_DRIVER Driver_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Base address of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_ADDR    FLASH_PS_AREA_OFFSET
/* Size of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_SIZE    FLASH_PS_AREA_SIZE
#define PS_RAM_FS_SIZE                TFM_HAL_PS_FLASH_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_PS_SECTORS_PER_BLOCK  (1)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_PS_PROGRAM_UNIT       (0x1)

/* Internal Trusted Storage (ITS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M ITS Integration Guide. The ITS should be in the internal flash, but is
 * allocated in the external flash just for development platforms that don't
 * have internal flash available.
 */
#define TFM_HAL_ITS_FLASH_DRIVER Driver_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Base address of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_ADDR    FLASH_ITS_AREA_OFFSET
/* Size of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_SIZE    FLASH_ITS_AREA_SIZE
#define ITS_RAM_FS_SIZE                TFM_HAL_ITS_FLASH_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_ITS_SECTORS_PER_BLOCK  (1)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_ITS_PROGRAM_UNIT       (0x1)

/* OTP / NV counter definitions */
#define TFM_OTP_NV_COUNTERS_AREA_SIZE   (FLASH_OTP_NV_COUNTERS_AREA_SIZE / 2)
#define TFM_OTP_NV_COUNTERS_AREA_ADDR   FLASH_OTP_NV_COUNTERS_AREA_OFFSET
#define TFM_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_OTP_NV_COUNTERS_SECTOR_SIZE
#define TFM_OTP_NV_COUNTERS_BACKUP_AREA_ADDR (TFM_OTP_NV_COUNTERS_AREA_ADDR + \
                                              TFM_OTP_NV_COUNTERS_AREA_SIZE)

/* Code images loaded into the single 0x20000000 SRAM.
 * BL2's vector table (BL2_CODE_START = S_ROM_ALIAS(FLASH_AREA_BL2_OFFSET=0))
 * must land exactly where the bootROM jumps after loading the signed image:
 *   jump = RTK_RTS5918_BL2_LOAD_ADDRESS (-L) + RLTK_HDR_SIZE(0x20)
 *        = 0x20000000 + 0x20 = 0x20000020
 * The 0x20-byte RLTK header occupies 0x20000000..0x20000020.
 * Keep -L (bl2/CMakeLists.txt) = S_ROM_ALIAS_BASE - 0x20 or BL2 faults
 * before the first BOOT_LOG.
 */
#define S_ROM_ALIAS_BASE  (0x20000020)
/* NS code lives in the SAME single physical SRAM as everything else. BL2
 * (boot_hal_bl2.c NS_SRAM_LOAD_BASE) loads the NS image to
 *   0x20000000 + FLASH_AREA_1_OFFSET(0x38000) = 0x20038000
 * and the Zephyr NS image is linked/signed for that 0x20000000 base. This SoC
 * has NO separate 0x10000000 NS ROM alias - S/NS are distinguished purely by
 * SAU/MPC attributes on the one 0x20000000 SRAM. The old 0x10000000 value is
 * stale (boot_hal_bl2.c says so): it made NS_PARTITION_START/NS_CODE_START and
 * memory_regions.non_secure_code_start resolve to 0x10038000, so SPM's
 * tfm_hal_get_ns_MSP() dereferenced a non-decoding address and faulted with an
 * imprecise BusFault during nspm ctx init / NS-agent (pid 2) launch. Use the
 * real SRAM base so NS_PARTITION_START = 0x20038000 (matches BL2's load dst)
 * and NS_CODE_START = 0x20038400 (matches the 128-aligned NS vector table).
 */
#define NS_ROM_ALIAS_BASE (0x20000000)

/* RW data lives in the SAME physical SRAM, stacked ABOVE the loaded code
 * images (BL2 44K + S 180K + NS 448K, base 0x20000020, end at 0x200A8020):
 *   S  data: 0x200A8020 .. 0x200B5020 (52K)    [BL2 data overlaps here, transient]
 *   NS data: 0x200B5020 .. 0x200CF020 (104K)
 * All within the single 832K SRAM (0x20000000 .. 0x200D0000), ~4K headroom.
 * Bases moved up 0x1000 vs the original map because BL2 grew 0xA000->0xB000;
 * NS data shrank 0x1B000->0x1A000 (region_defs.h) so the top stays at 0x200CF020.
 */
#define S_RAM_ALIAS_BASE  (0x200A8020)
#define NS_RAM_ALIAS_BASE (0x200B5020)

#define TOTAL_ROM_SIZE FLASH_TOTAL_SIZE
#define TOTAL_RAM_SIZE (0xD0000)      /* 832 KB physical SRAM */

#define IMAGE_EXECUTABLE_RAM_START  (0x20000000)
#define IMAGE_EXECUTABLE_RAM_SIZE   (0x140000)

#endif /* __FLASH_LAYOUT_H__ */
