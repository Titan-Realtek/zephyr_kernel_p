/*
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * Copyright (c) 2025, Realtek Semiconductor Corp. All rights reserved.
 */

#ifndef RTK_I3C_INTERNAL_H_
#define RTK_I3C_INTERNAL_H_

#include "port_i3c.h"
#include "rtk_i3c.h"
#include "rtk_i3c_core.h"
#include "rtk_logging.h"
#include <stdio.h>

/**< Used internally. Indicate IBI message */
#define RTK_I3C_IBI 0x2

#define I3C_BRCT_ADDR      0x7e
#define I3C_HJ_ADDR        0x02
#define BYFM_NDF_OFFSET    20
#define BYFM_NDF_HIGH      26
#define I3C_CCC_INVALID_ID 0xff

#define ROUND_UP(x, y) (((x) + (y) - 1) / (y))

/** Primary Controller */
#define I3C_ROLE_IS_CTRL(role) ((role) == RTK_I3C_CTRL_PRIM)
/** Secondary Controller */
#define I3C_ROLE_IS_SEC(role)  ((role) == RTK_I3C_CTRL_SEC)
/** Target */
#define I3C_ROLE_IS_TAGT(role) ((role) == RTK_I3C_TAGT)

#define I3C_MSG_IS_READ(flag) ((flag) & RTK_I3C_READ)
#define I3C_MSG_IS_IBI(flag)  ((flag) & RTK_I3C_IBI)
#define I3C_MSG_IS_DMA(flag)  ((flag) & RTK_I3C_DMA)

#define I3C_STATE_IS_CMD(state) ((state) == STATE_CTRL_CMD_WRITE || (state) == STATE_CTRL_CMD_READ)
#define I3C_STATE_IS_WRITE(state)                                                                  \
	((state) == STATE_CTRL_PRV_WRITE || (state) == STATE_TAGT_PRV_WRITE ||                     \
	 (state) == STATE_TAGT_IBI)
#define I3C_STATE_IS_READ(state)                                                                   \
	((state) == STATE_CTRL_PRV_READ || (state) == STATE_TAGT_PRV_READ ||                       \
	 (state) == STATE_CTRL_IBI)

#define I3C_STATE_IS_PRV_WRITE(state)                                                              \
	((state) == STATE_CTRL_PRV_WRITE || (state) == STATE_TAGT_PRV_WRITE)

#define I3C_STATE_IS_PRV_READ(state)                                                               \
	((state) == STATE_CTRL_PRV_READ || (state) == STATE_TAGT_PRV_READ)

#define I3C_STATE_IS_IBI(state) (((state) == STATE_CTRL_IBI) || ((state) == STATE_TAGT_IBI))

#define I3C_CCC_IS_DA(ccc_id)                                                                      \
	((ccc_id) == I3C_CCC_BRCT_ENTDAA || (ccc_id) == I3C_CCC_DRCT_SETDASA ||                    \
	 (ccc_id) == I3C_CCC_BRCT_SETAASA || (ccc_id) == I3C_CCC_DRCT_SETNEWDA)

#define I3C_CCC_IS_INVALID(ccc_id) ((ccc_id) == (I3C_CCC_INVALID_ID))
#define I3C_CCC_IS_EC(ccc_id)                                                                      \
	(((ccc_id) == I3C_CCC_BRCT_ENEC) || ((ccc_id) == I3C_CCC_BRCT_DISEC) ||                    \
	 ((ccc_id) == I3C_CCC_DRCT_ENEC) || ((ccc_id) == I3C_CCC_DRCT_DISEC))

#define I3C_MODE_IS_I2C(i3c_mode) ((i3c_mode) == RTK_I3C_I2C || (i3c_mode) == RTK_I3C_I2C_10BIT)

typedef enum rtk_i3c_state {
	/* Do not change the order of the block */
	/* Disable state */
	STATE_DISABLED = 0,

	/* Idle state */
	STATE_CTRL_IDLE,
	STATE_TAGT_IDLE,

	/* Controller IBI read */
	STATE_CTRL_IBI, /*!< Controller IBI interrupt. */

	/* Read transfer */
	STATE_CTRL_CMD_READ, /*!< Ctroller command read. */
	STATE_CTRL_PRV_READ, /*!< Ctroller private read. */
	STATE_TAGT_PRV_READ, /*!<  Target private read. */

	/* Write transfer */
	STATE_CTRL_ENTDAA,
	STATE_CTRL_CMD_WRITE, /*!< Ctroller command write. */
	STATE_CTRL_PRV_WRITE, /*!< Ctroller private write. */
	STATE_TAGT_IBI,       /*!<  Target IBI transmit. */
	STATE_TAGT_PRV_WRITE, /*!<  Target private write. */
} rtk_i3c_state;

typedef enum rtk_i3c_byfm_type {
	// ADDR PKT: HDER + ACK
	BYFM_HDER_AD_ACK = ((0x0 << 31) | (0x1 << 4) | 0x1), // Header: ADDR with ACK
	BYFM_HDER_AD_NON = ((0x0 << 31) | (0x0 << 4) | 0x1), // Header: ADDR/DATA without ACK

	// DATA PKT: DATA + 9bit/ACK
	BYFM_DATA_WR_PAR = ((0x0 << 31) | (0x2 << 4) | 0x0), // WR Data with    PAR
	BYFM_DATA_WR_ACK = ((0x0 << 31) | (0x1 << 4) | 0x0), // WR Data with    ACK
	BYFM_DATA_WR_NON = ((0x0 << 31) | (0x0 << 4) | 0x0), // WR Data without ACK

	BYFM_DATA_RD_TBIT = ((0x0 << 31) | (0x4 << 4) | 0x2), // RD Data with T-bit
	BYFM_DATA_RD_ACK = ((0x0 << 31) | (0x1 << 4) | 0x2),  // RD Data with    ACK
	BYFM_DATA_RD_NON = ((0x0 << 31) | (0x0 << 4) | 0x2),  // RD Data without ACK

	BYFM_IS_END = ((0x1 << 31) | (0x0 << 4) | 0x0), // BYFM end
} rtk_i3c_byfm_type;

enum rtk_i3c_byfm_stop {
	BYFM_HAS_STOP = 0x1 << 2,
	BYFM_NON_STOP = 0x0
};

enum rtk_i3c_byfm_ftod {
	BYFM_FORCE_OD = (0x1 << 3),
	BYFM_NO_FORCE = 0x0
};

typedef enum rtk_i3c_byfm_hdr {
	BYFM_HDR_DDR = (0x1 << 7),
	BYFM_HDR_EXIT = (0x1 << 7) | (0x1 << 8),
	BYFM_SDR_MODE = 0
} rtk_i3c_byfm_hdr;

enum rtk_i3c_byfm_bfwd_num {
	BYFM_FWD_1 = 0 >> 16, // Branch -1 entry
	BYFM_FWD_2 = 1 >> 16, // Branch -2 entry
	BYFM_FWD_3 = 2 >> 16, // Branch -3 entry
	BYFM_FWD_4 = 3 >> 16, // Branch -4 entry
};

enum rtk_i3c_byfm_seq {
	BYFM_BFWD_1 = ((1 << 3) | (0 << 1) | (0 << 0)) >> 12,
	BYFM_BFWD_2 = ((1 << 3) | (1 << 1) | (0 << 0)) >> 12,
	BYFM_BFWD_3 = ((1 << 3) | (2 << 1) | (0 << 0)) >> 12,
	BYFM_BFWD_4 = ((1 << 3) | (3 << 1) | (0 << 0)) >> 12,
	BYFM_INV_BYFM = ((0 << 3) | (0 << 1) | (1 << 0)) >> 12,
	BYFM_NON_BFWD = ((0 << 3) | (0 << 1) | (0 << 0)) >> 12,
}; // rtk_i3c_byfm_seq

enum rtk_i3c_byfm_ptxc {
	BYFM_PTXC_ON = 0x1,
	BYFM_PTXC_OFF = 0x0
};

enum rtk_i3c_byfm_prxc {
	BYFM_PRXC_ON = 0x1,
	BYFM_PRXC_OFF = 0x0
};

enum rtk_i3c_req_type {
	IBI_REQ = 0x0,
	IBI_MDB_REQ = 0x1,
	HJI_REQ = 0x2,
	SEC_REQ = 0x3
};

uint8_t rtk_i3c_util_calc_parity(uint8_t dyn_addr);
int rtk_i3c_bus_get_bcr_by_addr(rtk_i3c_ctx *ctx, uint8_t dyn_adr);
int rtk_i3c_bus_get_dcr_by_addr(rtk_i3c_ctx *ctx, uint8_t dyn_addr);
#ifdef CONFIG_RTK_I3C_ETM
void rtk_i3c_bus_set_etm_enable_by_addr(rtk_i3c_ctx *ctx, uint8_t dyn_addr);
bool rtk_i3c_bus_get_etm_enable_by_addr(rtk_i3c_ctx *ctx, uint8_t dyn_addr);
void rtk_i3c_bus_clear_etm_enable(rtk_i3c_ctx *ctx);
void rtk_i3c_abort_xfer(rtk_i3c_ctx *ctx);
#endif

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

#ifdef CONFIG_RTK_I3C_DMA

/**
 * @brief Write data to TX FIFO using DMA
 *
 * @param ctx I3C context
 * @param msg I3C message structure containing data to write
 * @return int 0 on success, negative error code on failure
 */
int rtk_i3c_write_fifo_dma(rtk_i3c_ctx *ctx, rtk_i3c_msg *msg);

/**
 * @brief Read data from RX FIFO using DMA
 *
 * @param ctx I3C context
 * @param msg I3C message structure containing data to receive
 * @param read_len Number of bytes to read from FIFO
 * @return int 0 on success, negative error code on failure
 */
int rtk_i3c_read_fifo_dma(rtk_i3c_ctx *ctx, rtk_i3c_msg *msg, size_t read_len);

#endif /* CONFIG_RTK_I3C_DMA */

#define RETURN_ERROR_IF(condition, error_code)                                                     \
	do {                                                                                       \
		if (condition) {                                                                   \
			return (error_code);                                                       \
		}                                                                                  \
	} while (0)

#endif
