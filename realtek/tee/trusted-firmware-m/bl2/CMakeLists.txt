#-------------------------------------------------------------------------------
# Copyright (c) 2020-2024, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.21)

project("Bootloader" VERSION 0.1.0 LANGUAGES C ASM)

add_executable(bl2
    src/flash_map.c
    $<$<BOOL:${DEFAULT_MCUBOOT_SECURITY_COUNTERS}>:src/security_cnt.c>
    $<$<BOOL:${DEFAULT_MCUBOOT_FLASH_MAP}>:src/default_flash_map.c>
    $<$<BOOL:${MCUBOOT_DATA_SHARING}>:src/shared_data.c>
    $<$<BOOL:${PLATFORM_DEFAULT_PROVISIONING}>:src/provisioning.c>
    $<$<BOOL:${MCUBOOT_USE_PSA_CRYPTO}>:src/thin_psa_crypto_core.c>
    $<$<BOOL:${CONFIG_GNU_SYSCALL_STUB_ENABLED}>:${CMAKE_SOURCE_DIR}/platform/ext/common/syscalls_stub.c>
)

add_subdirectory(ext/mcuboot)

set_target_properties(bl2
    PROPERTIES
        SUFFIX ".axf"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(bl2
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${MCUBOOT_PATH}/boot/bootutil/src>
)

target_link_libraries(bl2
    PRIVATE
        tfm_boot_status
        $<$<BOOL:${TEST_BL2}>:mcuboot_tests>
        platform_bl2
)

target_compile_options(bl2
    PRIVATE
        ${BL2_COMPILER_CP_FLAG}
)

target_link_options(bl2
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wl,-Map=${CMAKE_BINARY_DIR}/bin/bl2.map>
        $<$<C_COMPILER_ID:ARMClang>:--map>
        $<$<C_COMPILER_ID:IAR>:--map\;${CMAKE_BINARY_DIR}/bin/bl2.map>
        ${BL2_LINKER_CP_OPTION}
)

target_compile_definitions(bl2
    PRIVATE
        $<$<BOOL:${DEFAULT_MCUBOOT_FLASH_MAP}>:DEFAULT_MCUBOOT_FLASH_MAP>
        $<$<BOOL:${PLATFORM_PSA_ADAC_SECURE_DEBUG}>:PLATFORM_PSA_ADAC_SECURE_DEBUG>
        $<$<BOOL:${TEST_BL2}>:TEST_BL2>
        $<$<BOOL:${TFM_PARTITION_FIRMWARE_UPDATE}>:TFM_PARTITION_FIRMWARE_UPDATE>
        $<$<AND:$<BOOL:${CONFIG_TFM_BOOT_STORE_MEASUREMENTS}>,$<NOT:$<BOOL:${CONFIG_TFM_BOOT_STORE_ENCODED_MEASUREMENTS}>>>:TFM_MEASURED_BOOT_API>
)

add_convert_to_bin_target(bl2)

################## Realtek RTS5918 bootROM image header + sign #################
# The RTS5918 bootROM loads the first-stage image (BL2) from SPI flash into
# SRAM and verifies its RLTK header + signature before jumping in. Wrap the
# raw bl2.bin with that header + an ECDSA-P384 signature so the bootROM will
# accept and launch it. Mirrors the Zephyr application path in
# soc/realtek/ec/common/CMakeLists.txt (image_gen_w_sign.py).
#
# Defaults to ON only for realtek platforms; a no-op for every other TF-M
# target.
if (TFM_PLATFORM MATCHES "realtek")
    set(_RTK_SIGN_BL2_DEFAULT ON)
else()
    set(_RTK_SIGN_BL2_DEFAULT OFF)
endif()

set(RTK_RTS5918_SIGN_BL2 ${_RTK_SIGN_BL2_DEFAULT} CACHE BOOL
    "Wrap bl2.bin with the RTS5918 bootROM header + ECDSA signature")

# The imgtool lives in the Zephyr tree, three levels up from the TF-M root
# (…/zephyr_kernel_p/realtek/tee/trusted-firmware-m -> …/zephyr_kernel_p).
set(RTK_RTS5918_IMGTOOL_DIR
    "${CMAKE_SOURCE_DIR}/../../../soc/realtek/ec/common/rts5918_imgtool"
    CACHE PATH "Directory containing image_gen_w_sign.py, pk.bin, sk.bin")

# !!! IMPORTANT !!! Load address the bootROM copies the image to; execution
# jumps to (load_addr + 32-byte RLTK header), so this MUST equal BL2's
# vector-table link address minus 0x20 (RLTK_HDR_SIZE).
#
# RTS5918 executes from the single 0x20000000 SRAM. BL2 links at
# S_ROM_ALIAS_BASE = 0x20000020 (flash_layout.h), so:
#   -L = 0x20000000  ->  exec = 0x20000000 + 0x20 = 0x20000020 = BL2_CODE_START
# The 0x20-byte RLTK header occupies 0x20000000..0x20000020.
# Keep -L and S_ROM_ALIAS_BASE in sync or the signed image will not boot.
set(RTK_RTS5918_BL2_LOAD_ADDRESS "0x20000000" CACHE STRING
    "bootROM load address for the BL2 image (must equal BL2 link address - 0x20)")

# Header SPI settings, matched to boards/realtek/rts5918_bison_defconfig
# (DIV8 -> 6.25 MHz -> 3; normal read -> 0x03).
set(RTK_RTS5918_SPI_FREQ  "3"    CACHE STRING "SPI freq: 0=50M 1=25M 2=12.5M 3=6.25M")
set(RTK_RTS5918_SPI_RDCMD "0x03" CACHE STRING "SPI read cmd: 0x03/0x3B/0x6B")

if (RTK_RTS5918_SIGN_BL2)
    find_package(Python3)

    get_target_property(_bl2_bin_dir bl2 RUNTIME_OUTPUT_DIRECTORY)

    if (NOT EXISTS ${RTK_RTS5918_IMGTOOL_DIR}/image_gen_w_sign.py)
        message(WARNING
            "RTK_RTS5918_SIGN_BL2 is ON but image_gen_w_sign.py was not found at "
            "${RTK_RTS5918_IMGTOOL_DIR}. Set -DRTK_RTS5918_IMGTOOL_DIR=<path>.")
    endif()

    message(STATUS
        "RTS5918 BL2 signing: load address = ${RTK_RTS5918_BL2_LOAD_ADDRESS} "
        "(exec = load + 0x20 = 0x20000020 = S_ROM_ALIAS_BASE). Keep -L and "
        "S_ROM_ALIAS_BASE (flash_layout.h) in sync or the signed image will not boot.")

    # Runs after bl2.bin is produced by add_convert_to_bin_target()'s bl2_bin
    # target. The imgtool drops intermediate files (hdr/hash/tlv/signature) in
    # the CWD, so run it inside the bin/ output dir.
    add_custom_command(TARGET bl2_bin POST_BUILD
        COMMENT "Signing bl2.bin -> bl2.rts5918.bin with the RTS5918 bootROM image tool"
        WORKING_DIRECTORY ${_bl2_bin_dir}
        COMMAND ${Python3_EXECUTABLE} ${RTK_RTS5918_IMGTOOL_DIR}/image_gen_w_sign.py
            -I ${_bl2_bin_dir}/bl2.bin
            -O ${_bl2_bin_dir}/bl2.rts5918.bin
            -L ${RTK_RTS5918_BL2_LOAD_ADDRESS}
            -F ${RTK_RTS5918_SPI_FREQ}
            -R ${RTK_RTS5918_SPI_RDCMD}
            -H SHA2_384
            -s ${RTK_RTS5918_IMGTOOL_DIR}/sk.bin
            -p ${RTK_RTS5918_IMGTOOL_DIR}/pk.bin
            -k SHA2_384
            -S ECDSA
    )
endif()

############################### BOOT HAL # #####################################

add_library(bl2_hal INTERFACE)

target_include_directories(bl2_hal
    INTERFACE
        include
)

############################### MBEDCRYPTO #####################################

add_library(bl2_mbedcrypto_config INTERFACE)

if(NOT ${MCUBOOT_SIGNATURE_TYPE} STREQUAL "")
    string(REGEX MATCH "[0-9]*$" SIG_LEN ${MCUBOOT_SIGNATURE_TYPE})
    string(REGEX MATCH "^[A-Z]*" SIG_TYPE ${MCUBOOT_SIGNATURE_TYPE})
endif()

# FixMe: The MBEDTLS_CONFIG_FILE and MBEDTLS_PSA_CRYPTO_CONFIG_FILE should be
# put in a dedicated target that can be linked by other targets, for example
# bl2 provisioning related targets, to be able to include correctly psa/crypto.h
target_compile_definitions(bl2_mbedcrypto_config
    INTERFACE
        $<$<STREQUAL:${SIG_TYPE},RSA>:MCUBOOT_SIGN_RSA>
        $<$<STREQUAL:${SIG_TYPE},RSA>:MCUBOOT_SIGN_RSA_LEN=${SIG_LEN}>
        $<$<BOOL:${MCUBOOT_USE_PSA_CRYPTO}>:MCUBOOT_USE_PSA_CRYPTO>
        $<$<BOOL:${MCUBOOT_BUILTIN_KEY}>:MCUBOOT_BUILTIN_KEY>
        $<$<STREQUAL:${SIG_TYPE},EC>:MCUBOOT_SIGN_EC${SIG_LEN}>
        MBEDTLS_CONFIG_FILE="${MCUBOOT_MBEDCRYPTO_CONFIG_FILEPATH}"
        MBEDTLS_PSA_CRYPTO_CONFIG_FILE="${MCUBOOT_PSA_CRYPTO_CONFIG_FILEPATH}"
        # Workaround for https://github.com/ARMmbed/mbedtls/issues/1077
        $<$<OR:$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8-m.base>,$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv6-m>>:MULADDC_CANNOT_USE_R7>
        $<$<BOOL:${CC312_LEGACY_DRIVER_API_ENABLED}>:CC312_LEGACY_DRIVER_API_ENABLED>
)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
set(ENABLE_TESTING OFF)
set(ENABLE_PROGRAMS OFF)
set(MBEDTLS_FATAL_WARNINGS OFF)
set(ENABLE_DOCS OFF)
set(INSTALL_MBEDTLS_HEADERS OFF)
set(LIB_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/mbedcrypto/install)
set(GEN_FILES OFF)

# Set the prefix to be used by mbedTLS targets
set(MBEDTLS_TARGET_PREFIX bl2_)

# Mbedcrypto in Debug builds uses too much memory. As a compromise, if `Debug` build type
# is selected mbedcrypto will build under `RelWithDebInfo` which preserves debug
# symbols while optimizing space.
set(SAVED_BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE ${MBEDCRYPTO_BUILD_TYPE})
add_subdirectory(${MBEDCRYPTO_PATH} ${CMAKE_CURRENT_BINARY_DIR}/mbedcrypto EXCLUDE_FROM_ALL)
set(CMAKE_BUILD_TYPE ${SAVED_BUILD_TYPE} CACHE STRING "Build type: [Debug, Release, RelWithDebInfo, MinSizeRel]" FORCE)

if(NOT TARGET ${MBEDTLS_TARGET_PREFIX}mbedcrypto)
    message(FATAL_ERROR "Target ${MBEDTLS_TARGET_PREFIX}mbedcrypto does not exist. Have the patches in ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto been applied to the mbedcrypto repo at ${MBEDCRYPTO_PATH} ?
    Hint: The command might be `cd ${MBEDCRYPTO_PATH} && git apply ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/*.patch`")
endif()

target_link_libraries(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PUBLIC
        bl2_mbedcrypto_config
)

target_link_libraries(${MBEDTLS_TARGET_PREFIX}p256m
    PUBLIC
        ${MBEDTLS_TARGET_PREFIX}mbedcrypto
)

target_include_directories(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PUBLIC
        ${MBEDCRYPTO_PATH}/library
)

target_compile_options(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PRIVATE
        ${BL2_COMPILER_CP_FLAG}
)

target_compile_options(${MBEDTLS_TARGET_PREFIX}p256m
    PRIVATE
        ${BL2_COMPILER_CP_FLAG}
)

############################### CODE SHARING ###################################

if (TFM_CODE_SHARING)
    target_share_symbols(bl2 ${CMAKE_CURRENT_SOURCE_DIR}/bl2_shared_symbols.txt)

    if (NOT EXISTS ${MBEDCRYPTO_PATH}/library/code_share.c)
        message(FATAL_ERROR "File ${MBEDCRYPTO_PATH}/library/code_share.c does not exist.
        Have the patch ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/0002-Enable-crypto-code-sharing-between-independent-binar.patch
        been applied to the mbedcrypto repo at ${MBEDCRYPTO_PATH}?
        Hint: The command might be `cd ${MBEDCRYPTO_PATH} && git apply ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/*.patch`")
    endif()
endif()
