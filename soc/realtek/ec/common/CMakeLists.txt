# SPDX-License-Identifier: Apache-2.0
#
# Copyright (c) 2024 Realtek Semiconductor Corporation, SIBG-SD7
#

zephyr_include_directories(.)

# --- Handle RTS5918 img tool ---
if (CONFIG_REALTEK_RTS5918_IMG_HEADER)
    set(RTS_BIN_NAME ${CONFIG_KERNEL_BIN_NAME}.rts5918.bin)
    set(RTS_PAD_BIN_NAME ${CONFIG_KERNEL_BIN_NAME}.rts5918.pad.bin)

    # Set tool base
    set(IMG_TOOL_DIR ${ZEPHYR_BASE}/soc/realtek/ec/common/rts5918_imgtool)
    set(IMG_TOOL_PATH ${IMG_TOOL_DIR}/image_gen_w_sign.py)

    set(IMG_GEN_COMMAND
        -I ${PROJECT_BINARY_DIR}/zephyr.bin
        -O ${PROJECT_BINARY_DIR}/${RTS_BIN_NAME}
        -L ${CONFIG_REALTEK_RTS_IMG_HEADER_LOAD_ADDRESS}
        -H SHA2_384
        -s ${IMG_TOOL_DIR}/sk.bin
        -p ${IMG_TOOL_DIR}/pk.bin
        -k SHA2_384
        -S ECDSA
    )
    set(NEEDS_IMG_GEN TRUE)
endif()

if (NEEDS_IMG_GEN)
    set_property(GLOBAL APPEND PROPERTY extra_post_build_commands
    COMMAND ${PYTHON_EXECUTABLE} ${IMG_TOOL_PATH} ${IMG_GEN_COMMAND}
    COMMAND ${PYTHON_EXECUTABLE} ${IMG_TOOL_DIR}/hex_dump.py
                ${PROJECT_BINARY_DIR}/${RTS_BIN_NAME}
    COMMAND ${CMAKE_OBJCOPY} -I binary -O binary 
        --gap-fill 0xFF --pad-to 0x1000000 
        ${PROJECT_BINARY_DIR}/${RTS_BIN_NAME} 
        ${PROJECT_BINARY_DIR}/${RTS_PAD_BIN_NAME}
    )

endif()
