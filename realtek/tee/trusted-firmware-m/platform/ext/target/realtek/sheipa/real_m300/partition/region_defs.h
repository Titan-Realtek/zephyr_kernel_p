/*
 * Copyright (c) 2017-2022 Arm Limited. All rights reserved.
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

#ifndef __REGION_DEFS_H__
#define __REGION_DEFS_H__

#include "flash_layout.h"

#define S_IMAGE_LOAD_ADDRESS    (0x2000B000)
#define NS_IMAGE_LOAD_ADDRESS   (0x20038000)

#define BL2_HEAP_SIZE           (0x00001000)
#define BL2_MSP_STACK_SIZE      (0x00001800)

#ifdef ENABLE_HEAP
#define S_HEAP_SIZE             (0x00000200)
#endif

#ifdef TFM_FIH_PROFILE_ON
#define S_MSP_STACK_SIZE        (0x00000A40)
#else
#define S_MSP_STACK_SIZE        (0x00000800)
#endif
#define S_PSP_STACK_SIZE        (0x00000800)

#define NS_HEAP_SIZE            (0x00001000)
#define NS_STACK_SIZE           (0x000001E0)

/* MPC granularity is 128 KB on AN521 Castor MPS2 FPGA image. Alignment
 * of partitions is defined in accordance with this constraint.
 */

#ifdef BL2
#ifndef LINK_TO_SECONDARY_PARTITION
#define S_IMAGE_PRIMARY_PARTITION_OFFSET   (FLASH_AREA_0_OFFSET)
#define S_IMAGE_SECONDARY_PARTITION_OFFSET (FLASH_AREA_2_OFFSET)
#else
#define S_IMAGE_PRIMARY_PARTITION_OFFSET   (FLASH_AREA_2_OFFSET)
#define S_IMAGE_SECONDARY_PARTITION_OFFSET (FLASH_AREA_0_OFFSET)
#endif /* !LINK_TO_SECONDARY_PARTITION */
#else
#define S_IMAGE_PRIMARY_PARTITION_OFFSET (0x0)
#endif /* BL2 */

#ifndef LINK_TO_SECONDARY_PARTITION
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET (FLASH_AREA_0_OFFSET \
                                           + FLASH_S_PARTITION_SIZE)
#else
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET (FLASH_AREA_2_OFFSET \
                                           + FLASH_S_PARTITION_SIZE)
#endif /* !LINK_TO_SECONDARY_PARTITION */

/* Boot partition structure if MCUBoot is used:
 * 0x0_0000 Bootloader header
 * 0x0_0400 Image area
 * 0x7_0000 Trailer
 */
/* IMAGE_CODE_SIZE is the space available for the software binary image.
 * It is less than the FLASH_S_PARTITION_SIZE + FLASH_NS_PARTITION_SIZE
 * because we reserve space for the image header and trailer introduced
 * by the bootloader.
 */

#if (!defined(MCUBOOT_IMAGE_NUMBER) || (MCUBOOT_IMAGE_NUMBER == 1)) && \
    (NS_IMAGE_PRIMARY_PARTITION_OFFSET > S_IMAGE_PRIMARY_PARTITION_OFFSET)
/* If secure image and nonsecure image are concatenated, and nonsecure image
 * locates at the higher memory range, then the secure image does not need
 * the trailer area.
 */
#define IMAGE_S_CODE_SIZE \
            (FLASH_S_PARTITION_SIZE - BL2_HEADER_SIZE)
#else
#define IMAGE_S_CODE_SIZE \
            (FLASH_S_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)
#endif

#define IMAGE_NS_CODE_SIZE \
            (FLASH_NS_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)

/* Alias definitions for secure and non-secure areas*/
#define S_ROM_ALIAS(x)  (S_ROM_ALIAS_BASE + (x))
#define NS_ROM_ALIAS(x) (NS_ROM_ALIAS_BASE + (x))

#define S_RAM_ALIAS(x)  (S_RAM_ALIAS_BASE + (x))
#define NS_RAM_ALIAS(x) (NS_RAM_ALIAS_BASE + (x))

/* Secure regions */
#define S_IMAGE_PRIMARY_AREA_OFFSET \
             (S_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
/*
 * The secure runtime image is a plain MCUboot image and has NO 0x20-byte RLTK
 * header - that wrapper exists only on the BL2 image (consumed by the bootROM).
 * It therefore must NOT inherit BL2's RLTK offset: S_ROM_ALIAS_BASE is
 * 0x20000020, and using it here would link the vector table at 0x2000B420.
 * On Cortex-M33 VTOR[6:0] are RES0, so writing 0x2000B420 is silently masked
 * to 0x2000B400 - shifting every exception vector down by 8 slots. The first
 * SVC (TFM_SVC_SPM_INIT) then vectors into the HardFault stub and TF-M reports
 * "FATAL ERROR: Reserved Exception 0x0000000B" with all fault-status registers
 * clear. Link the S image from the clean 128-byte-aligned SRAM flash-alias base
 * 0x20000000 instead (exactly what boot_hal_bl2.c already uses for NS), so the
 * vector table lands at 0x2000B400. This is fully decoupled from BL2:
 * S_ROM_ALIAS_BASE, BL2_CODE_START and bl2 -L are unchanged.
 */
#define S_IMAGE_LOAD_BASE   (0x20000000)
#define S_CODE_START    (S_IMAGE_LOAD_BASE + S_IMAGE_PRIMARY_AREA_OFFSET)
#define S_CODE_SIZE     (IMAGE_S_CODE_SIZE)
#define S_CODE_LIMIT    (S_CODE_START + S_CODE_SIZE - 1)

/* Size of vector table: 139 interrupt handlers + 4 bytes MPS initial value */
#define S_CODE_VECTOR_TABLE_SIZE    (0x230)

#define S_DATA_START    (S_RAM_ALIAS(0x0))
#define S_DATA_SIZE     (0xD000)      /* 52 KB */
#define S_DATA_LIMIT    (S_DATA_START + S_DATA_SIZE - 1)

/* Non-secure regions */
#define NS_IMAGE_PRIMARY_AREA_OFFSET \
                        (NS_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define NS_CODE_START   (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_AREA_OFFSET))
#define NS_CODE_SIZE    (IMAGE_NS_CODE_SIZE)
#define NS_CODE_LIMIT   (NS_CODE_START + NS_CODE_SIZE - 1)

#define NS_DATA_START   (NS_RAM_ALIAS(0x0))
#if defined(PSA_API_TEST_NS) && !defined(PSA_API_TEST_IPC)
#define DEV_APIS_TEST_NVMEM_REGION_SIZE  0x400
#define NS_DATA_SIZE    (0x1B000 - DEV_APIS_TEST_NVMEM_REGION_SIZE)
#else
#define NS_DATA_SIZE    (0x40000)     /* 104 KB (was 0x1B000; 1K reclaimed for BL2 growth) */
#endif
#define NS_DATA_LIMIT   (NS_DATA_START + NS_DATA_SIZE - 1)

/* NS partition information is used for MPC and SAU configuration */
#define NS_PARTITION_START \
            (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_PARTITION_OFFSET))
#define NS_PARTITION_SIZE (FLASH_NS_PARTITION_SIZE)

/* Secondary partition for new images in case of firmware upgrade.
 *
 * IMPORTANT (RTS5918 single-SRAM): this region must NOT be aliased through
 * NS_ROM_ALIAS_BASE. NS_ROM_ALIAS_BASE was changed 0x10000000 -> 0x20000000 so
 * that the NS runtime image (VTOR/MSP, SAU NS-code region) resolves to its real
 * SRAM load address (0x20038000, matching boot_hal_bl2.c NS_SRAM_LOAD_BASE).
 * But this platform has only ONE 832KB SRAM at 0x20000000; there is no in-SRAM
 * upgrade slot. Aliasing the secondary partition through the SRAM base put it at
 *   NS_ROM_ALIAS(0xA8000) = 0x200A8000 .. 0x20144FFF (size 0x9D000)
 * which (a) overlaps the live NS_DATA SAU region (0x200B5020..0x200CF01F) and
 * (b) runs past the end of SRAM (0x200D0000). tfm_common_s.ld links
 * Load$$LR$$LR_SECONDARY_PARTITION$$Base = SECONDARY_PARTITION_START, and
 * target_cfg.c sau_cfg[] adds an enabled SAU NS region there. Two overlapping
 * enabled SAU regions make the ARMv8-M attribution unit treat the whole overlap
 * as SECURE (SAU programming error), so the first NS interrupt-stack push into
 * NS_DATA faulted with SecureFault AUVIOL (SFSR=0x48, SFAR~=initial MSP).
 * The secondary/upgrade slot lives in external flash (the legacy 0x10000000
 * alias), never in the runtime SRAM, so pin it to its own base. This is a
 * dead/non-decoding address at runtime; the ER_SECONDARY_PARTITION region is
 * UNINIT (address-space bookkeeping only), so no data is placed there - exactly
 * how it sat harmlessly before NS_ROM_ALIAS_BASE was moved. Decoupled the same
 * way S_IMAGE_LOAD_BASE is decoupled from S_ROM_ALIAS_BASE above.
 */
#define SECONDARY_PARTITION_ALIAS_BASE (0x10000000)
#define SECONDARY_PARTITION_START \
            (SECONDARY_PARTITION_ALIAS_BASE + S_IMAGE_SECONDARY_PARTITION_OFFSET)
#define SECONDARY_PARTITION_SIZE (FLASH_S_PARTITION_SIZE + \
                                  FLASH_NS_PARTITION_SIZE)

#ifdef BL2
/* Bootloader regions */
#define BL2_CODE_START    (S_ROM_ALIAS(FLASH_AREA_BL2_OFFSET))
#define BL2_CODE_SIZE     (FLASH_AREA_BL2_SIZE)
#define BL2_CODE_LIMIT    (BL2_CODE_START + BL2_CODE_SIZE - 1)

#define BL2_DATA_START    (S_RAM_ALIAS(0x0))
#define BL2_DATA_SIZE     (0xA000)      /* 40 KB, transient (overlaps S data) */
#define BL2_DATA_LIMIT    (BL2_DATA_START + BL2_DATA_SIZE - 1)
#endif /* BL2 */



/* Shared symbol area between bootloader and runtime firmware. Global variables
 * in the shared code can be placed here.
 */
#ifdef CODE_SHARING
#define SHARED_SYMBOL_AREA_BASE S_RAM_ALIAS_BASE
#define SHARED_SYMBOL_AREA_SIZE 0x20
#else
#define SHARED_SYMBOL_AREA_BASE S_RAM_ALIAS_BASE
#define SHARED_SYMBOL_AREA_SIZE 0x0
#endif /* CODE_SHARING */

/* Shared data area between bootloader and runtime firmware.
 * These areas are allocated at the beginning of the RAM, it is overlapping
 * with TF-M Secure code's MSP stack
 */
#define BOOT_TFM_SHARED_DATA_BASE (SHARED_SYMBOL_AREA_BASE + \
                                   SHARED_SYMBOL_AREA_SIZE)
#define BOOT_TFM_SHARED_DATA_SIZE (0x400)
#define BOOT_TFM_SHARED_DATA_LIMIT (BOOT_TFM_SHARED_DATA_BASE + \
                                    BOOT_TFM_SHARED_DATA_SIZE - 1)

#endif /* __REGION_DEFS_H__ */
