/*
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * Copyright (c) 2025, Realtek Semiconductor Corp. All rights reserved.
 */

#include "rtk_i3c.h"
#include "rtk_i3c_config.h"
#include "rtk_i3c_core.h"
#include "rtk_i3c_internal.h"
#include "rtk_i3c_bus.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#ifdef CONFIG_RTK_I3C
#define MAX_BYFM_NDF 64

void rtk_i3c_isr(rtk_i3c_ctx *);

/**********************************************************************************************************************
 * Internal Functions
 **********************************************************************************************************************/

uint8_t rtk_i3c_util_calc_parity(uint8_t dyn_addr)
{
	uint8_t parity = 0;

	dyn_addr &= 0x7F;

	for (int i = 0; i < 7; i++) {
		parity ^= (dyn_addr >> i) & 1;
	}

	parity = ~parity & 1;

	return parity;
}

static void rtk_i3c_calc_frame_msg(rtk_i3c_msg *msg, uint16_t addr, rtk_i3c_mode i3c_mode)
{
	ASSERT(msg->data != NULL && msg->len >= 2);

	if (i3c_mode == RTK_I3C_HDR_DDR) {
		msg->data[0] = ((msg->flags & RTK_I3C_READ) << 7);
		msg->data[1] = (uint8_t)(addr << 1);
		msg->len = 2;
	} else if (i3c_mode == RTK_I3C_I2C_10BIT) {
		/* 10-bit address format:
		 * byte0: 0b 11110 | (first 2bits) | 0
		 * byte1: low 8 bits
		 */
		msg->data[0] = (uint8_t)(0xF0 | ((addr >> 7) & 0x06));
		msg->data[1] = (uint8_t)(addr & 0xFF);
		msg->len = 2;
	} else {
		/* I3C SDR: RTK_I3C_SDR_MODE, I2C 7-bit: RTK_I3C_I2C */
		msg->data[0] = (uint8_t)(addr << 1 | (msg->flags & RTK_I3C_READ));
		msg->len = 1;
	}
}

static int rtk_i3c_write_fifo_cpu(const rtk_i3c_ctx *ctx, rtk_i3c_msg *msg)
{
	ASSERT(ctx != NULL && msg != NULL);

	uint8_t *data = msg->data + msg->count;
	size_t len = msg->len - msg->count;

	rtk_i3c_core *core = ctx->core;

	while (len > 0) {
		if ((uintptr_t)data % sizeof(uint32_t) == 0 &&
		    msg->count + sizeof(uint32_t) <= msg->len) {
			if (rtk_i3c_core_get_txfl(ctx->core) + sizeof(uint32_t) >
			    RTK_I3C_FIFO_DEPTH) {
				LOG_ERR("Fifo will be full. %zu bytes has been written\n",
					msg->count);
				return RTK_I3C_TXFIFO_WILL_FULL;
			}
			rtk_i3c_core_write_fifo_word(core, *(uint32_t *)data);
			data += sizeof(uint32_t);
			msg->count += sizeof(uint32_t);
			len -= sizeof(uint32_t);
		} else {
			if (rtk_i3c_core_get_txfl(ctx->core) + sizeof(uint8_t) >
			    RTK_I3C_FIFO_DEPTH) {
				LOG_ERR("Fifo will be full. %zu bytes has been written\n",
					msg->count);
				return RTK_I3C_TXFIFO_WILL_FULL;
			}
			rtk_i3c_core_write_fifo_byte(core, *data);
			data++;
			msg->count++;
			len--;
		}
	}
	return 0;
}

static int rtk_i3c_read_fifo_cpu(const rtk_i3c_ctx *ctx, rtk_i3c_msg *msg, size_t read_len)
{
	ASSERT(ctx != NULL && msg != NULL);

	uint8_t *data = msg->data + msg->count;

	rtk_i3c_core *core = ctx->core;

	while (read_len > 0) {
		if ((uintptr_t)data % sizeof(uint32_t) == 0 &&
		    msg->count + sizeof(uint32_t) <= msg->len && read_len >= sizeof(uint32_t)) {
			/* read the aligned word from fifo to the buffer */
			rtk_i3c_core_read_fifo_word(core, (uint32_t *)data);
			msg->count += sizeof(uint32_t);
			data += sizeof(uint32_t);
			read_len -= sizeof(uint32_t);
		} else {
			rtk_i3c_core_read_fifo_byte(core, data);
			msg->count++;
			data++;
			read_len--;
		}
	}

	return 0;
}

static inline int rtk_i3c_write_fifo(rtk_i3c_ctx *ctx, rtk_i3c_msg *msg)
{
	LOG_DBG("expected %zu bytes\n", msg->len - msg->count);
#ifdef CONFIG_RTK_I3C_DMA
	if (I3C_MSG_IS_DMA(msg->flags)) {
		return rtk_i3c_write_fifo_dma(ctx, msg);
	} else
#endif
	{
		return rtk_i3c_write_fifo_cpu(ctx, msg);
	}
}

static inline int rtk_i3c_read_fifo(rtk_i3c_ctx *ctx, rtk_i3c_rx_buffer *buffer, size_t read_len,
				    bool is_ibi)
{
	int ret = -1;
	size_t count = buffer->msg.count;
	if (buffer->msg.count + read_len > buffer->msg.len || buffer->msg.data == NULL) {
		LOG_DBG("requesting new buffer\n");
		if (buffer->buffer_requested) {
			/* Application do not provide new buffer. */
			LOG_ERR("Read buffer overflowed and the application do not "
				"provide a new buffer! Transfer aborted!\n");
			rtk_i3c_core_flush_rx(ctx->core);
			return RTK_I3C_READ_BUFFER_FULL;
		}
		rtk_i3c_callback_args args = {
			.event = is_ibi ? RTK_I3C_EVENT_IBI_READ_BUFFER_FULL
					: RTK_I3C_EVENT_READ_BUFFER_FULL,
			.ctx = ctx->cfg->common_cfg.ctx,
		};

		buffer->buffer_requested = true;
		if (ctx->cfg->common_cfg.callback != NULL) {
			ctx->cfg->common_cfg.callback(&args);
		}
		/* ensure the count is not modified by application */
		if (buffer->msg.count != count) {
			LOG_WRN("Application modified msg.count in callback, resetting\n");
		}
		buffer->msg.count = count;

		if (buffer->msg.count + read_len > buffer->msg.len || buffer->msg.data == NULL) {
			/* Application do not provide new buffer. */
			return RTK_I3C_READ_BUFFER_FULL;
		}
		buffer->buffer_requested = false;
#ifdef CONFIG_RTK_I3C_DMA
		if (I3C_MSG_IS_DMA(buffer->msg.flags)) {
			/* in dma mode, the read_len is determined by the buffer length */
			read_len = buffer->msg.len - buffer->msg.count;
		}
#endif
	}
	LOG_DBG("expected %zu bytes\n", read_len);
#ifdef CONFIG_RTK_I3C_DMA
	if (I3C_MSG_IS_DMA(buffer->msg.flags)) {
		ret = rtk_i3c_read_fifo_dma(ctx, &buffer->msg, read_len);
	} else
#endif
	{
		ret = rtk_i3c_read_fifo_cpu(ctx, &buffer->msg, read_len);
		LOG_DBG("finished");
		LOG_HEX(buffer->msg.data + buffer->msg.count - read_len, read_len);
	}
	return ret;
}

static int rtk_i3c_write_byfm_and_fifo(rtk_i3c_ctx *ctx, uint32_t byfm, rtk_i3c_msg *msg)
{
	RETURN_ERROR_IF(rtk_i3c_core_wait_byfm_fifo(ctx->core) != 0, RTK_I3C_TIMEOUT);
	rtk_i3c_core_write_byfm(ctx->core, byfm);
	return rtk_i3c_write_fifo(ctx, msg);
}

static int rtk_i3c_write_i3c_frame(rtk_i3c_ctx *ctx, bool is_read, rtk_i3c_mode i3c_mode)
{
	uint32_t byfm =
		BYFM_HDER_AD_ACK | BYFM_NON_STOP | BYFM_NON_BFWD | BYFM_NO_FORCE |
		((i3c_mode == RTK_I3C_HDR_DDR) ? (BYFM_HDR_DDR | (0x2 << BYFM_NDF_OFFSET))
					       : (BYFM_SDR_MODE | (0x1 << BYFM_NDF_OFFSET)));

	/* send 7e/w or 7e/r for ENTDAA */
	uint8_t frame[2];

	rtk_i3c_msg msg = {
		.data = frame,
		.len = sizeof(frame),
		.flags = is_read,
		.count = 0,
	};

	rtk_i3c_calc_frame_msg(&msg, I3C_BRCT_ADDR, i3c_mode);
	return rtk_i3c_write_byfm_and_fifo(ctx, byfm, &msg);
}

static int rtk_i3c_write_cmd_frame(rtk_i3c_ctx *ctx, uint8_t ccc_id, bool is_end_frame,
				   rtk_i3c_mode i3c_mode)
{
	(void)i3c_mode;

	uint32_t byfm = BYFM_DATA_WR_PAR | BYFM_NON_BFWD | BYFM_SDR_MODE | BYFM_PTXC_OFF |
			BYFM_PRXC_OFF | (0x1 << BYFM_NDF_OFFSET) |
			(is_end_frame ? BYFM_HAS_STOP : BYFM_NON_STOP);

	rtk_i3c_msg msg = {
		.data = &ccc_id,
		.len = 1,
		.count = 0,
	};

	LOG_DBG("ccc_id: 0x%02x\n", ccc_id);
	return rtk_i3c_write_byfm_and_fifo(ctx, byfm, &msg);
}

static int rtk_i3c_write_addr_frame(rtk_i3c_ctx *ctx, uint16_t addr, bool is_read,
				    bool is_end_frame, rtk_i3c_mode i3c_mode)
{
	/* I2C 10-bit and hdr-ddr address uses NDF=2, others use NDF=1 */
	uint32_t ndf = (i3c_mode == RTK_I3C_I2C_10BIT || i3c_mode == RTK_I3C_HDR_DDR) ? 2 : 1;

	uint32_t byfm = BYFM_HDER_AD_ACK | BYFM_NON_BFWD |
			((i3c_mode == RTK_I3C_HDR_DDR) ? BYFM_HDR_DDR : 0) |
			(ndf << BYFM_NDF_OFFSET) | (is_end_frame ? BYFM_HAS_STOP : BYFM_NON_STOP);

	uint8_t frame[2];
	int ret = 0;

	rtk_i3c_msg msg = {
		.data = frame,
		.len = sizeof(frame),
		.flags = is_read,
		.count = 0,
	};

	rtk_i3c_calc_frame_msg(&msg, addr, i3c_mode);

	if ((ret = rtk_i3c_write_byfm_and_fifo(ctx, byfm, &msg)) < 0) {
		return ret;
	}

	if (i3c_mode == RTK_I3C_I2C_10BIT && is_read) {
		msg.data[0] |= RTK_I3C_READ;
		msg.count = 0;
		msg.len = 1;
		BIT_FIELD_SET(byfm, BYFM_NDF_OFFSET, BYFM_NDF_HIGH, msg.len);
		ret = rtk_i3c_write_byfm_and_fifo(ctx, byfm, &msg);
	}

	return ret;
}

static int rtk_i3c_write_data_frame(rtk_i3c_ctx *ctx, rtk_i3c_msg msg, bool is_end_frame,
				    rtk_i3c_mode i3c_mode)
{
	uint32_t byfm = 0;
	size_t msg_len = msg.len, msg_count = msg.count;
	int ret = 0;

	if (I3C_MSG_IS_READ(msg.flags) || I3C_MSG_IS_IBI(msg.flags)) {
		if (ctx->ccc_id == I3C_CCC_BRCT_ENTDAA) {
			byfm |= BYFM_DATA_RD_NON | BYFM_FORCE_OD;
		} else if (i3c_mode != RTK_I3C_SDR_MODE) {
			byfm |= BYFM_DATA_RD_ACK;
		} else {
			byfm |= BYFM_DATA_RD_TBIT;
		}
	} else {
		if (ctx->ccc_id == I3C_CCC_BRCT_ENTDAA) {
			byfm |= BYFM_DATA_WR_ACK | BYFM_FORCE_OD;
		} else if (i3c_mode != RTK_I3C_SDR_MODE) {
			byfm |= BYFM_DATA_WR_ACK;
		} else {
			byfm |= BYFM_DATA_WR_PAR;
		}
	}

	byfm |= BYFM_NON_BFWD | BYFM_SDR_MODE | BYFM_PTXC_OFF | BYFM_PRXC_OFF |
		((i3c_mode == RTK_I3C_HDR_DDR) ? BYFM_HDR_DDR : BYFM_SDR_MODE);

	while (msg_len > 0) {
		/* Split the message into multiple frames with maximum of length MAX_BYFM_NDF */
		if (msg_len > MAX_BYFM_NDF) {
			msg.len = MAX_BYFM_NDF;
		} else {
			msg.len = msg_len;
			byfm |= ((is_end_frame) ? BYFM_HAS_STOP : BYFM_NON_STOP);
		}
		BIT_FIELD_SET(byfm, BYFM_NDF_OFFSET, BYFM_NDF_HIGH, msg.len);
		if (rtk_i3c_core_wait_byfm_fifo(ctx->core) != 0) {
			ret = RTK_I3C_TIMEOUT;
			goto exit;
		}
		rtk_i3c_core_write_byfm(ctx->core, byfm);
		if (!I3C_MSG_IS_READ(msg.flags)) {
			if ((ret = rtk_i3c_write_fifo(ctx, &msg)) < 0) {
				goto exit;
			}
		}
		msg_len -= msg.len;
		msg_count += msg.len;
		msg.data += msg.len;
		msg.count = 0;
	}

	ret = msg_count;

exit:
	return ret;
}

static void rtk_i3c_write_end_frame(rtk_i3c_core *core)
{
	uint32_t byfm = BYFM_IS_END | BYFM_SDR_MODE;

	rtk_i3c_core_wait_byfm_fifo(core);
	rtk_i3c_core_write_byfm(core, byfm);
}

static bool rtk_i3c_is_ccc_supported(uint8_t id)
{
	switch (id) {
	case I3C_CCC_DRCT_SETNEWDA:
	case I3C_CCC_DRCT_SETDASA:
	case I3C_CCC_BRCT_SETAASA:
	case I3C_CCC_BRCT_ENTDAA:
	case I3C_CCC_BRCT_RSTDAA:
	case I3C_CCC_BRCT_ENTHDR0:
	case I3C_CCC_BRCT_ENEC:
	case I3C_CCC_BRCT_DISEC:
	case I3C_CCC_BRCT_SETMWL:
	case I3C_CCC_BRCT_SETMRL:
	case I3C_CCC_BRCT_ENDXFER:
	case I3C_CCC_DRCT_ENEC:
	case I3C_CCC_DRCT_DISEC:
	case I3C_CCC_DRCT_SETMWL:
	case I3C_CCC_DRCT_SETMRL:
	case I3C_CCC_DRCT_GETMWL:
	case I3C_CCC_DRCT_GETMRL:
	case I3C_CCC_DRCT_GETPID:
	case I3C_CCC_DRCT_GETBCR:
	case I3C_CCC_DRCT_GETDCR:
	case I3C_CCC_DRCT_ENDXFER:
		return true;
	default:
		return false;
	}
}

static rtk_i3c_rx_buffer *rtk_i3c_set_buffer(rtk_i3c_ctx *ctx, const rtk_i3c_msg *msg)
{
	rtk_i3c_rx_buffer *xfer_buffer;

	if (I3C_MSG_IS_IBI(msg->flags)) {
		xfer_buffer = &ctx->ibi_buffer;
		xfer_buffer->buffer_requested = false;
	} else if (I3C_MSG_IS_READ(msg->flags)) {
		/* If this is a read transfer, update the rx buffer descriptor. */
		/* Disable the rxne isr to ensure updating the read buffer is not
		 * interrupted.. */
		xfer_buffer = &ctx->rx_buffer;
		ctx->rx_buffer.buffer_requested = false;
		bool previous = rtk_i3c_core_set_rxne_isr(ctx->core, false);
		rtk_i3c_core_set_rxne_isr(ctx->core, previous);
	} else {
		/* If this is a write transfer, update the tx buffer descriptor. */
		xfer_buffer = (rtk_i3c_rx_buffer *)&ctx->tx_buffer;
	}
	xfer_buffer->msg = *msg;

	return xfer_buffer;
}

static int rtk_i3c_common_init(rtk_i3c_ctx *ctx, rtk_i3c_common_cfg *cfg)
{
	ASSERT(ctx != NULL && cfg != NULL);

	RETURN_ERROR_IF(cfg->i3c_freq_hz < 100000000U || cfg->i3c_freq_hz > 400000000U,
			RTK_I3C_INVAL_PARAM);

	memset(ctx, 0, sizeof(rtk_i3c_ctx));
	ctx->state = STATE_DISABLED;
	ctx->ccc_id = I3C_CCC_INVALID_ID;

	if ((ctx->core = (void *)plat_i3c_get_base(cfg->instance_id)) == NULL) {
		LOG_ERR("Invalid instance_id: %d", cfg->instance_id);
		return RTK_I3C_INVAL_PARAM;
	}

	rtk_i3c_core_set_role(ctx->core, cfg->role);
	if (cfg->role == RTK_I3C_CTRL_I2C) {
		rtk_i3c_core_set_force_od(ctx->core, true);
	}

	rtk_i3c_core_set_intr(ctx->core, false);
	rtk_i3c_core_set_ibi_cap(ctx->core, &cfg->ibi);
	rtk_i3c_core_set_timing(ctx->core, &cfg->timing, cfg->i3c_freq_hz);

	plat_i3c_init_isr(ctx, (uintptr_t)rtk_i3c_isr);
	rtk_i3c_core_set_global_enable(ctx->core, true);

	return 0;
}

static void rtk_i3c_common_deinit(rtk_i3c_ctx *ctx)
{
	rtk_i3c_core_set_global_enable(ctx->core, false);
	rtk_i3c_core_set_intr(ctx->core, false);
}

/**********************************************************************************************************************
 * Driver Info Dump Functions
 **********************************************************************************************************************/

/**
 * @brief Dump I3C driver information
 * @param ctx Pointer to I3C context
 */
void rtk_i3c_dump_info(const rtk_i3c_ctx *ctx)
{
	const rtk_i3c_cfg *cfg = ctx->cfg;
	const rtk_i3c_common_cfg *common_cfg = &cfg->common_cfg;

	/* Driver version */
	LOG_INF("========== I3C Driver Info ==========\n");
	LOG_INF("Version    : v%d.%d.%d\n", RTK_I3C_VERSION_MAJOR, RTK_I3C_VERSION_MINOR,
		RTK_I3C_VERSION_PATCH);
	LOG_INF("Build Time : %s %s\n", __DATE__, __TIME__);

	/* Role */
	LOG_INF("Role       : %s\n",
		(common_cfg->role == RTK_I3C_TAGT)
			? "Target"
			: ((common_cfg->role == RTK_I3C_CTRL_I2C) ? "I2C Controller"
								  : "Controller"));

	/* Baudrate (only for controller) */
#ifdef CONFIG_RTK_I3C_CTRL
	LOG_INF("I3C Baud   : %" PRIu32 "Hz (PP) / %" PRIu32 "Hz (OD)\n",
		cfg->bitrate_cfg.i3c_pp_baud_hz, cfg->bitrate_cfg.i3c_od_baud_hz);
	LOG_INF("I2C Baud   : %" PRIu32 "Hz\n", cfg->bitrate_cfg.i2c_baud_hz);
#endif

	/* Features */
	LOG_INF("Features   : "
#ifdef CONFIG_RTK_I3C_CTRL
		"CTRL=y, "
#else
		"CTRL=n, "
#endif
#ifdef CONFIG_RTK_I3C_TAGT
		"TAGT=y, "
#else
		"TAGT=n, "
#endif
#ifdef CONFIG_RTK_I3C_IBI
		"IBI=y, "
#else
		"IBI=n, "
#endif
#ifdef CONFIG_RTK_I3C_DMA
		"DMA=y\n"
#else
		"DMA=n\n"
#endif
	);

	/* Instance and FIFO */
	LOG_INF("Instance   : %d\n", common_cfg->instance_id);
	LOG_INF("FIFO Depth : %d\n", RTK_I3C_FIFO_DEPTH);
	LOG_INF("=====================================\n");
}

#ifdef CONFIG_RTK_I3C_CTRL

static inline int rtk_i3c_enter_hdr(rtk_i3c_ctx *ctx, rtk_i3c_mode i3c_mode)
{
	rtk_i3c_ccc ccc = {
		.msg = NULL,
		.id = I3C_CCC_BRCT_ENTHDR0 + __builtin_ctz(i3c_mode),
	};

	int ret = rtk_i3c_do_ccc(ctx, &ccc, 0, true);
	ctx->state = STATE_CTRL_IDLE;
	/* do not record ccc_id to avoid hdr_ddr transfer event to be a command event */
	ctx->ccc_id = I3C_CCC_INVALID_ID;

	return ret;
}

static inline uint8_t rtk_i3c_get_hdr_mode(rtk_i3c_ctx *ctx)
{
	/* Currently, only support hdr_ddr */
	return rtk_i3c_core_get_hdr_ddr(ctx->core);
}

static void rtk_i3c_exit_hdr(rtk_i3c_ctx *ctx)
{
	rtk_i3c_core_wait_byfm_fifo(ctx->core);

	rtk_i3c_core_write_byfm(ctx->core, BYFM_HDR_EXIT | BYFM_HAS_STOP | BYFM_DATA_WR_PAR);
}

/**
 * Init the I3C transfer.
 *
 * The function performs the following actions to properly Init an
 * I3C transaction:
 *   1. Send the 0x7e frame.
 *   2. enter hdr as needed.
 *   3. Set i2c mode as needed.
 *
 */
static void rtk_i3c_init_xfer(struct rtk_i3c_ctx *ctx, int i3c_mode)
{
	uint8_t cur_hdr_mode = rtk_i3c_get_hdr_mode(ctx);

	/* Check if the i3c_mode is different from current i3c mode */
	if (i3c_mode != cur_hdr_mode) {
		if (cur_hdr_mode) {
			rtk_i3c_exit_hdr(ctx);
		}
		if (i3c_mode == RTK_I3C_HDR_DDR) {
			rtk_i3c_enter_hdr(ctx, RTK_I3C_HDR_DDR);
		}
	}

	rtk_i3c_core_set_i2c(ctx->core, I3C_MODE_IS_I2C(i3c_mode));

	rtk_i3c_core_start_xfer(ctx->core);
	if (i3c_mode == RTK_I3C_SDR_MODE) {
		/* the i3c_frame is done in enter_hdr */
		rtk_i3c_write_i3c_frame(ctx, RTK_I3C_WRITE, i3c_mode);
	}
}

/**
 * Complete the I3C transfer.
 *
 * The function performs the following actions to properly terminate an
 * I3C transaction:
 *   1. Send the header‑exit pattern.
 *   2. Send the end‑frame.
 *
 */
static int rtk_i3c_complete_xfer(struct rtk_i3c_ctx *ctx, int i3c_mode, bool restart)
{
	if (!restart) {
		if (i3c_mode == RTK_I3C_HDR_DDR) {
			rtk_i3c_exit_hdr(ctx);
		}
		rtk_i3c_write_end_frame(ctx->core);
		return (rtk_i3c_core_wait_xfer_done(ctx->core) != 0) ? RTK_I3C_TIMEOUT : 0;
	} else if (i3c_mode == RTK_I3C_HDR_DDR) {
		/* always issue end in HDR-DDR flow */
		rtk_i3c_write_end_frame(ctx->core);
	}

	return 0;
}

/**********************************************************************************************************************
 * External Functions
 **********************************************************************************************************************/

int rtk_i3c_ctrl_init(rtk_i3c_ctx *ctx, rtk_i3c_cfg *cfg)
{
	ASSERT(ctx != NULL);
	ASSERT(cfg->addr_slot != NULL && cfg->tagt_table != NULL);
	int ret = 0;

	if ((ret = rtk_i3c_common_init(ctx, &cfg->common_cfg)) != 0) {
		LOG_ERR("failed!\n");
		return ret;
	}
	rtk_i3c_core_set_baud_rate(ctx->core, &cfg->bitrate_cfg, cfg->common_cfg.i3c_freq_hz);
	ctx->cfg = cfg;

	/* Enable controller */
	ctx->state = STATE_CTRL_IDLE;
	LOG_INF("Driver initialized\n");
	rtk_i3c_dump_info(ctx);
	rtk_i3c_core_set_intr(ctx->core, true);

	return 0;
}

int rtk_i3c_ctrl_deinit(rtk_i3c_ctx *ctx)
{
	RETURN_ERROR_IF(ctx->state != STATE_CTRL_IDLE, RTK_I3C_BUSY);

	rtk_i3c_common_deinit(ctx);
	return 0;
}

void rtk_i3c_set_hj_accept(rtk_i3c_ctx *ctx, bool enable)
{
	rtk_i3c_core_set_hj_accept(ctx->core, enable);
}

void rtk_i3c_set_ibi_mdb(rtk_i3c_ctx *ctx, uint8_t dyn_addr, bool ibi_payload)
{
	ASSERT(ctx != NULL && ctx->core != NULL);
	rtk_i3c_core_set_mdb(ctx->core, dyn_addr, ibi_payload);
}

int rtk_i3c_ctrl_recover(rtk_i3c_ctx *ctx)
{
	ASSERT(ctx != NULL);
	RETURN_ERROR_IF(ctx->cfg == NULL, RTK_I3C_NOT_ENABLED);

	/* Drop any in-flight transfer: flush BYFM/TX/RX FIFOs, clear all pending
	 * interrupts, and reset the software transfer state back to controller
	 * idle so a wedged bus can accept new transfers.
	 */
	rtk_i3c_core_flush_all(ctx->core);
	rtk_i3c_core_clear_isr(ctx->core, 0xFFFFFFFFU);

	ctx->rx_buffer = (rtk_i3c_rx_buffer){0};
	ctx->tx_buffer = (rtk_i3c_tx_buffer){0};
	ctx->ibi_buffer = (rtk_i3c_rx_buffer){0};
	ctx->ccc_id = I3C_CCC_INVALID_ID;
	ctx->state = STATE_CTRL_IDLE;

	return 0;
}

void rtk_i3c_get_config(rtk_i3c_ctx *ctx, rtk_i3c_cfg *cfg)
{
	ASSERT(cfg != NULL);
	if (ctx == NULL || ctx->cfg == NULL) {
		cfg->common_cfg.instance_id = 0;
		cfg->common_cfg.role = RTK_I3C_CTRL_PRIM;
#if defined(RTK_I3C_FREQ_HZ) && (RTK_I3C_FREQ_HZ != 0)
		cfg->common_cfg.i3c_freq_hz = RTK_I3C_FREQ_HZ;
#else
		cfg->common_cfg.i3c_freq_hz = 125000000U;
#endif
#ifdef RTK_I3C_ENABLE_HJ
		cfg->common_cfg.ibi.enable_hj = true;
#else
		cfg->common_cfg.ibi.enable_hj = false;
#endif
#ifdef RTK_I3C_ENABLE_IBI
		cfg->common_cfg.ibi.enable_ibi = true;
#else
		cfg->common_cfg.ibi.enable_ibi = false;
#endif
#ifdef RTK_I3C_ENABLE_CR
		cfg->common_cfg.ibi.enable_cr = true;
#else
		cfg->common_cfg.ibi.enable_cr = false;
#endif
#ifdef RTK_I3C_LEGACY_I2C
		cfg->common_cfg.timing.bus_free_ns = ((RTK_I3C_I3C_OD_BAUD_HZ <= 100000)    ? 4700
						      : (RTK_I3C_I3C_OD_BAUD_HZ <= 400000)  ? 1300
						      : (RTK_I3C_I3C_OD_BAUD_HZ <= 1000000) ? 500
											    : 1300);
#else
		cfg->common_cfg.timing.bus_free_ns = 1300;
#endif
		cfg->common_cfg.timing.bus_idle_ns = 200000;
		cfg->common_cfg.timing.bus_available_ns = 2000;
		cfg->common_cfg.timing.i3c_od_baud_hz = RTK_I3C_I3C_OD_BAUD_HZ;
		cfg->common_cfg.callback = NULL;
		cfg->common_cfg.ctx = NULL;

#ifdef CONFIG_RTK_I3C_CTRL
#if defined(RTK_I3C_I2C_BAUD_HZ) && (RTK_I3C_I2C_BAUD_HZ != 0)
		cfg->bitrate_cfg.i2c_baud_hz = RTK_I3C_I2C_BAUD_HZ;
#else
		cfg->bitrate_cfg.i2c_baud_hz = 400000U;
#endif
#if defined(RTK_I3C_I3C_PP_BAUD_HZ) && (RTK_I3C_I3C_PP_BAUD_HZ != 0)
		cfg->bitrate_cfg.i3c_pp_baud_hz = RTK_I3C_I3C_PP_BAUD_HZ;
#else
		cfg->bitrate_cfg.i3c_pp_baud_hz = 12500000U;
#endif
#if defined(RTK_I3C_I3C_OD_BAUD_HZ) && (RTK_I3C_I3C_OD_BAUD_HZ != 0)
		cfg->bitrate_cfg.i3c_od_baud_hz = RTK_I3C_I3C_OD_BAUD_HZ;
#else
		cfg->bitrate_cfg.i3c_od_baud_hz = 2500000U;
#endif
#endif

#ifdef CONFIG_RTK_I3C_TAGT
		cfg->tagt_info.dyn_addr = 0x10;
		cfg->tagt_info.stc_addr = 0x10;
		cfg->tagt_info.char_info.pid = 0x04ba00001000;
		cfg->tagt_info.char_info.bcr = 0x1f;
		cfg->tagt_info.char_info.dcr = 0xc6;
#endif
	} else {
		*cfg = *ctx->cfg;
	}
}

/**********************************************************************************************************************
 * Internal DAA Functions
 **********************************************************************************************************************/

/**
 * @brief Send ENTDAA CCC command
 * @param ctx Pointer to I3C context
 * @return 0 on success, error code on failure
 */
static inline int rtk_i3c_daa_send_ccc(rtk_i3c_ctx *ctx)
{
	rtk_i3c_ccc ccc = {
		.id = I3C_CCC_BRCT_ENTDAA,
		.msg = NULL,
	};

	return rtk_i3c_do_ccc(ctx, &ccc, 0, true);
}

/**
 * @brief Send read characteristics info request
 * @param ctx Pointer to I3C context
 * @return 0 on success, error code on failure
 */
static inline int rtk_i3c_daa_read_char_info(rtk_i3c_ctx *ctx)
{
	int ret = 0;

	rtk_i3c_msg msg = {
		.data = (uint8_t *)&ctx->tagt_char_info,
		.len = I3C_TAGT_CHAR_INFO_SIZE,
		.flags = RTK_I3C_READ,
		.count = 0,
	};

	rtk_i3c_set_buffer(ctx, &msg);

	/* Issue the target to read the characteristics info */
	ctx->rx_buffer.msg.count = 0;
	if ((ret = rtk_i3c_write_i3c_frame(ctx, RTK_I3C_READ, RTK_I3C_SDR_MODE)) < 0) {
		return ret;
	}

	return rtk_i3c_write_data_frame(ctx, ctx->rx_buffer.msg, false, RTK_I3C_SDR_MODE);
}

/**
 * @brief Process DAA phase - read target characteristics and assign address
 *
 * Reads PID/BCR/DCR from target, invokes callback (or default handler) to get
 * assigned dynamic address, then writes address back to target.
 *
 * @param ctx Pointer to I3C context
 * @param is_last_tagt True if this is the last target in DAA sequence
 * @return 0 on success, error code on failure
 */
static inline int rtk_i3c_daa_process(rtk_i3c_ctx *ctx, bool is_last_tagt)
{
	rtk_i3c_callback_args args = {
		.ctx = ctx->cfg->common_cfg.ctx,
		.event = RTK_I3C_EVENT_ADDRESS_ASSIGNMENT_PHASE,
		.tagt_char_info = &ctx->tagt_char_info,
	};

	ctx->tagt_char_info.pid = rtk_i3c_util_get_be48((uint8_t *)&ctx->tagt_char_info);

	if (ctx->cfg->common_cfg.callback != NULL) {
		ctx->cfg->common_cfg.callback(&args);
	} else {
		/* If no callback, use default handler */
		rtk_i3c_bus_handle_daa(ctx, &ctx->tagt_char_info, &args.dyn_addr);
	}

	uint8_t addr = args.dyn_addr << 1 | rtk_i3c_util_calc_parity(args.dyn_addr);
	rtk_i3c_msg msg = {
		.data = &addr,
		.len = 1,
		.count = 0,
		.flags = RTK_I3C_WRITE,
	};
	return rtk_i3c_write_data_frame(ctx, msg, is_last_tagt, RTK_I3C_SDR_MODE);
}

/**********************************************************************************************************************
 * Public API Functions
 **********************************************************************************************************************/

int rtk_i3c_do_daa(rtk_i3c_ctx *ctx)
{
	ASSERT(ctx != NULL);
	int ret = RTK_I3C_XFER_TERMINATION;
	int num_tagts = 0;

	/* Step 1: Send ENTDAA CCC */
	if ((ret = rtk_i3c_daa_send_ccc(ctx)) != 0) {
		return ret;
	}

	/* Disable interrupts for blocking mode */
	rtk_i3c_core_set_rxnak_isr(ctx->core, false);
	rtk_i3c_core_set_rxne_isr(ctx->core, false);

	/* Step 2 & 3: Loop to read characteristics and process */
	for (int i = 0; i < RTK_I3C_MAX_TAGT_COUNT; i++) {
		if ((ret = rtk_i3c_daa_read_char_info(ctx)) < 0) {
			goto exit;
		}

		/* Check if DAA is complete (RXNAK) */
		if (rtk_i3c_core_get_rxnak(ctx->core)) {
			/* Controller DAA done: no (more) targets answered the 0x7E/R.
			 * This is the normal ENTDAA termination (including an empty bus),
			 * not an error -- return success.
			 */
			rtk_i3c_core_clear_isr(ctx->core, I3C_ISR_RXNAK_MASK);
			ret = 0;
			goto exit;
		}

		uint32_t wait_count = 1000000;
		while ((rtk_i3c_core_get_rx_fifo_len(ctx->core) < I3C_TAGT_CHAR_INFO_SIZE) &&
		       !rtk_i3c_core_get_rxnak(ctx->core) && (wait_count-- > 0)) {
			;
		}

		if (rtk_i3c_core_get_rxnak(ctx->core)) {
			/* RXNAK after issuing the read: DAA finished normally. */
			rtk_i3c_core_clear_isr(ctx->core, I3C_ISR_RXNAK_MASK);
			ret = 0;
			goto exit;
		}

		if (rtk_i3c_core_get_rx_fifo_len(ctx->core) < I3C_TAGT_CHAR_INFO_SIZE) {
			LOG_ERR("DAA timed out waiting for target characteristics, rxfl=%u\n",
				rtk_i3c_core_get_rx_fifo_len(ctx->core));
			ret = RTK_I3C_XFER_TERMINATION;
			goto exit;
		}

		ret = rtk_i3c_read_fifo(ctx, &ctx->rx_buffer, I3C_TAGT_CHAR_INFO_SIZE, false);
		if (ret < 0) {
			goto exit;
		}

		if (ctx->rx_buffer.msg.count == I3C_TAGT_CHAR_INFO_SIZE) {
			ret = rtk_i3c_daa_process(ctx, i == (RTK_I3C_MAX_TAGT_COUNT - 1));
			if (ret < 0) {
				goto exit;
			}
			ret = 0;
			num_tagts++;
		} else {
			LOG_ERR("Received %zu bytes of characteristics info, expected: 8!\n",
				ctx->rx_buffer.msg.count);
			ret = RTK_I3C_XFER_TERMINATION;
			goto exit;
		}
	}

	rtk_i3c_write_end_frame(ctx->core);
	ret = (rtk_i3c_core_wait_xfer_done(ctx->core) != 0) ? RTK_I3C_TIMEOUT : 0;

exit:
	ctx->state = STATE_CTRL_IDLE;
	ctx->rx_buffer = (rtk_i3c_rx_buffer){0};
	ctx->tx_buffer = (rtk_i3c_tx_buffer){0};
	rtk_i3c_core_flush_all(ctx->core);

	/* Re-enable interrupts */
	rtk_i3c_core_set_rxnak_isr(ctx->core, true);
	rtk_i3c_core_set_rxne_isr(ctx->core, true);

	return ret;
}

int rtk_i3c_do_ccc(rtk_i3c_ctx *ctx, const rtk_i3c_ccc *ccc, uint8_t i3c_mode, bool restart)
{
	ASSERT(ccc != NULL && ctx != NULL && ctx->core != NULL);
	/*ensure broadcast ccc message is always write*/
	ASSERT(ccc->msg == NULL ||
	       (ccc->msg != NULL && ccc->msg->data != NULL && !I3C_MSG_IS_READ(ccc->msg->flags)));

	RETURN_ERROR_IF(ccc->num_tagts != 0 && IS_CCC_BRCT(ccc->id), RTK_I3C_INVAL_PARAM);
	RETURN_ERROR_IF((ccc->num_tagts == 0 || ccc->tagt == NULL) && !IS_CCC_BRCT(ccc->id),
			RTK_I3C_INVAL_PARAM);
	RETURN_ERROR_IF(!rtk_i3c_is_ccc_supported(ccc->id), RTK_I3C_NOT_SUPPORTED);
	RETURN_ERROR_IF(i3c_mode != RTK_I3C_SDR_MODE, RTK_I3C_INVAL_PARAM);

	int ret = RTK_I3C_XFER_TERMINATION;
	bool is_end_frame = false;
	rtk_i3c_rx_buffer *xfer_buffer = NULL;

	ctx->ccc_id = ccc->id;
	rtk_i3c_init_xfer(ctx, i3c_mode);
	if (ctx->state == STATE_CTRL_IDLE && ccc->num_tagts == 0) {
		ctx->state = STATE_CTRL_CMD_WRITE;
	}

	/* write ccc id */
	is_end_frame = (ccc->msg == NULL && ccc->num_tagts == 0) ? !restart : false;
	if ((ret = rtk_i3c_write_cmd_frame(ctx, ccc->id, is_end_frame, i3c_mode)) < 0) {
		goto exit_err;
	}

	/* write CCC message */
	if (ccc->msg != NULL) {
		xfer_buffer = rtk_i3c_set_buffer(ctx, ccc->msg);
		is_end_frame = (ccc->num_tagts == 0) ? !restart : false;
		if ((ret = rtk_i3c_write_data_frame(ctx, xfer_buffer->msg, is_end_frame,
						    i3c_mode)) < 0) {
			goto exit_err;
		}
	}

	/* Direct CCC msg for targets */
	for (uint8_t i = 0; i < ccc->num_tagts; i++) {
		ctx->state = (I3C_MSG_IS_READ(ccc->tagt[i].msg->flags)) ? STATE_CTRL_CMD_READ
									: STATE_CTRL_CMD_WRITE;
		is_end_frame = (i == ccc->num_tagts - 1) ? !restart : false;
		if ((ret = rtk_i3c_ctrl_xfer(ctx, &ccc->tagt[i], i3c_mode, !is_end_frame)) != 0) {
			goto exit_err;
		}
	}

	ret = 0;

exit_err:
	if (rtk_i3c_complete_xfer(ctx, i3c_mode, restart) != 0 && ret == 0) {
		ret = RTK_I3C_TIMEOUT;
	}

	if (ctx->state == STATE_CTRL_CMD_READ || ctx->state == STATE_CTRL_CMD_WRITE) {
		ctx->state = STATE_CTRL_IDLE;
	}

	if (ret < 0) {
		rtk_i3c_core_flush_all(ctx->core);
	}

	return ret;
}

int rtk_i3c_ctrl_xfer(rtk_i3c_ctx *ctx, const rtk_i3c_tagt *tagt, uint8_t i3c_mode, bool restart)
{
	ASSERT(tagt == NULL ||
	       (tagt->msg != NULL && tagt->msg->data != NULL && tagt->msg->len != 0));
	RETURN_ERROR_IF(tagt != NULL && (tagt->msg->len > INT_MAX), RTK_I3C_NOT_SUPPORTED);
	/* HDR-DDR data length should be even */
	RETURN_ERROR_IF(i3c_mode == RTK_I3C_HDR_DDR && (tagt->msg->len & 1), RTK_I3C_NOT_SUPPORTED);
	RETURN_ERROR_IF(i3c_mode >= RTK_I3C_MODE_MAX, RTK_I3C_INVAL_PARAM);
	RETURN_ERROR_IF((tagt == NULL) && (restart == false) && (i3c_mode == RTK_I3C_HDR_DDR) &&
				(rtk_i3c_get_hdr_mode(ctx) == RTK_I3C_SDR_MODE),
			RTK_I3C_INVAL_PARAM);

	int ret = RTK_I3C_XFER_TERMINATION;
	bool do_complete = I3C_STATE_IS_CMD(ctx->state) ? false : !restart;
	rtk_i3c_rx_buffer *xfer_buffer = NULL;

	if (ctx->state == STATE_CTRL_IDLE) {
		do_complete = true;
		rtk_i3c_init_xfer(ctx, i3c_mode);
	}
	/* Disable nack isr to ensure the message flow is not interrupted */
	rtk_i3c_core_set_rxnak_isr(ctx->core, false);

	if (tagt == NULL) {
		goto exit;
	}

	LOG_DBG("Controller %d %s target 0x%02x %zu bytes (%s)\n", ctx->cfg->common_cfg.instance_id,
		I3C_MSG_IS_READ(tagt->msg->flags) ? "read from" : "write to", tagt->addr,
		tagt->msg->len, I3C_MSG_IS_DMA(tagt->msg->flags) ? "DMA" : "CPU");

	if ((ret = rtk_i3c_write_addr_frame(ctx, tagt->addr, I3C_MSG_IS_READ(tagt->msg->flags),
					    false, i3c_mode)) < 0) {
		goto exit_nack;
	}
	if (ctx->state == STATE_CTRL_IDLE) {
		ctx->state = I3C_MSG_IS_READ(tagt->msg->flags) ? STATE_CTRL_PRV_READ
							       : STATE_CTRL_PRV_WRITE;
	}
	xfer_buffer = rtk_i3c_set_buffer(ctx, tagt->msg);
	/* issue end to hardware is just for simplfing the flow, not indicating HDR exit */
	if ((ret = rtk_i3c_write_data_frame(ctx, xfer_buffer->msg,
					    (!restart) || (i3c_mode == RTK_I3C_HDR_DDR),
					    i3c_mode)) < 0) {
		goto exit_nack;
	}
	if (!I3C_MSG_IS_READ(tagt->msg->flags)) {
		xfer_buffer->msg.count = ret;
	}
	if (rtk_i3c_core_get_rxnak(ctx->core)) {
		/* end frame will be auto sent by hw */
		restart = true;
		LOG_ERR("%s target address: 0x%02x is nacked!\n",
			I3C_MSG_IS_READ(tagt->msg->flags) ? "Read from" : "Write to", tagt->addr);
		ret = RTK_I3C_XFER_TERMINATION;
		goto exit_nack;
	}

exit:
	ret = 0;
	if (do_complete && rtk_i3c_complete_xfer(ctx, i3c_mode, restart) != 0) {
		ret = RTK_I3C_TIMEOUT;
	}

exit_nack:
	rtk_i3c_core_set_rxnak_isr(ctx->core, true);
	if (ret < 0) {
		rtk_i3c_core_flush_all(ctx->core);
	}

	return ret;
}

#ifdef CONFIG_RTK_I3C_IBI

/**
 * Read IBI data. Only for controller
 */
int rtk_i3c_ibi_read(rtk_i3c_ctx *ctx, rtk_i3c_msg *msg)
{
	ASSERT(ctx != NULL && msg != NULL && msg->data != NULL);
	RETURN_ERROR_IF(ctx->state != STATE_CTRL_IDLE && ctx->state != STATE_CTRL_IBI,
			RTK_I3C_BUSY);

	rtk_i3c_rx_buffer *xfer_buffer;

	LOG_DBG("Controller %d read ibi interrupt (%s)!\n", ctx->cfg->common_cfg.instance_id,
		I3C_MSG_IS_DMA(msg->flags) ? "DMA" : "CPU");

	rtk_i3c_core_set_ibi_isr(ctx->core, false);
	msg->flags |= RTK_I3C_IBI;
	xfer_buffer = rtk_i3c_set_buffer(ctx, msg);
	xfer_buffer->msg.flags |= RTK_I3C_READ;
	rtk_i3c_core_set_ibi_isr(ctx->core, true);

	return 0;
}

#endif /* CONFIG_RTK_I3C_IBI */
#endif /* CONFIG_RTK_I3C_CTRL */

#ifdef CONFIG_RTK_I3C_TAGT

int rtk_i3c_tagt_init(rtk_i3c_ctx *ctx, rtk_i3c_cfg *cfg)
{
	ASSERT(ctx != NULL && cfg != NULL);
	int ret = 0;

	if ((ret = rtk_i3c_common_init(ctx, &cfg->common_cfg)) != 0) {
		LOG_ERR("failed!\n");
		return ret;
	}

	rtk_i3c_core_tagt_set_resp_info(ctx->core, &cfg->tagt_info.resp_info);
	rtk_i3c_core_tagt_set_char_info(ctx->core, &cfg->tagt_info.char_info);
	rtk_i3c_core_set_addr(ctx->core, cfg->tagt_info.dyn_addr, cfg->tagt_info.stc_addr);
	rtk_i3c_core_set_i2c(ctx->core, cfg->tagt_info.is_i2c);

	ctx->cfg = cfg;

	/* Enable target */
	ctx->state = STATE_TAGT_IDLE;
	LOG_INF("Driver initialized\n");
	rtk_i3c_dump_info(ctx);
	rtk_i3c_core_set_intr(ctx->core, true);

	return 0;
}

int rtk_i3c_tagt_xfer(rtk_i3c_ctx *ctx, rtk_i3c_msg *msg)
{
	ASSERT(ctx != NULL && msg != NULL);
	rtk_i3c_rx_buffer *xfer_buffer = NULL;
	int ret = 0;

	/* A write (TX preload / read response) may preempt an armed-but-idle RX
	 * buffer: the target keeps RX armed between transfers, but when the
	 * application answers a controller read it takes over the bus. Only allowed
	 * before any write bytes have arrived (count == 0) so an in-progress
	 * controller write is never clobbered.
	 */
	bool tx_preempt_rx = (ctx->state == STATE_TAGT_PRV_READ && !I3C_MSG_IS_READ(msg->flags) &&
			      ctx->rx_buffer.msg.count == 0U);

	if (ctx->state == STATE_TAGT_PRV_WRITE && ctx->tx_buffer.msg.data == msg->data &&
	    ctx->tx_buffer.msg.flags == msg->flags && ctx->tx_buffer.msg.len == msg->len) {
		xfer_buffer = (rtk_i3c_rx_buffer *)&ctx->tx_buffer;
	} else if (ctx->state != STATE_TAGT_IDLE && !tx_preempt_rx) {
		return RTK_I3C_BUSY;
	}

	LOG_DBG("Target %d start to %s %zu bytes (%s)!\n", ctx->cfg->common_cfg.instance_id,
		I3C_MSG_IS_READ(msg->flags) ? "read" : "write", msg->len,
		I3C_MSG_IS_DMA(msg->flags) ? "DMA" : "CPU");
	if (!xfer_buffer) {
		xfer_buffer = rtk_i3c_set_buffer(ctx, msg);
	}

	if (I3C_MSG_IS_READ(msg->flags)) {
		ctx->state = STATE_TAGT_PRV_READ;
	} else {
		ctx->state = STATE_TAGT_PRV_WRITE;
		if ((ret = rtk_i3c_write_fifo(ctx, &xfer_buffer->msg)) < 0) {
			goto exit;
		}
		/* Keep STATE_TAGT_PRV_WRITE so the controller read that drains the
		 * preloaded FIFO is recognized as a write completion in the DONE ISR.
		 */
	}

	ret = 0;

exit:
	return ret;
}

int rtk_i3c_tagt_deinit(rtk_i3c_ctx *ctx)
{
	RETURN_ERROR_IF(ctx->state != STATE_TAGT_IDLE, RTK_I3C_BUSY);

	rtk_i3c_common_deinit(ctx);
	LOG_INF("Driver deinitialized\n");
	return 0;
}

#ifdef CONFIG_RTK_I3C_IBI

/**
 * Write IBI data. Only for target.
 */
int rtk_i3c_ibi_write(rtk_i3c_ctx *ctx, rtk_i3c_ibi_type ibi_type, rtk_i3c_msg *msg)
{
	ASSERT(ctx != NULL);
	int ret = 0;

	RETURN_ERROR_IF(I3C_ROLE_IS_CTRL(ctx->cfg->common_cfg.role), RTK_I3C_NOT_SUPPORTED);
	/* A target that keeps RX armed between transfers sits in STATE_TAGT_PRV_READ.
	 * Allow an IBI/HJ/CR to preempt that idle arm (no write bytes received yet),
	 * the same way a TX read-response can; the RX arm is restored afterwards.
	 */
	bool ibi_preempt_rx = (ctx->state == STATE_TAGT_PRV_READ && ctx->rx_buffer.msg.count == 0U);
	RETURN_ERROR_IF(ctx->state != STATE_TAGT_IDLE && !ibi_preempt_rx, RTK_I3C_BUSY);
	bool invalid_payload = (ctx->cfg->tagt_info.char_info.bcr & I3C_BCR_IBI_PAYLOAD)
				       ? (msg == NULL || msg->len == 0)
				       : (msg != NULL && msg->len > 0);
	RETURN_ERROR_IF(invalid_payload && ibi_type == RTK_I3C_IBI_INTR, RTK_I3C_INVAL_PARAM);
	RETURN_ERROR_IF(!(ctx->cfg->tagt_info.char_info.bcr & I3C_BCR_CR_CAP) &&
				ibi_type == RTK_I3C_IBI_CTRL_REQ,
			RTK_I3C_INVAL_PARAM);

	uint8_t addr = 0;
	bool is_read = false;

	rtk_i3c_rx_buffer *xfer_buffer;

	if (ibi_type == RTK_I3C_IBI_HOT_JOIN) {
		if (!ctx->cfg->common_cfg.ibi.enable_hj) {
			LOG_ERR("Hot join is not enabled!\n");
			return RTK_I3C_NOT_ENABLED;
		}
		LOG_DBG("Target %d issue hot join request!\n", ctx->cfg->common_cfg.instance_id);
		addr = I3C_HJ_ADDR;
		is_read = false;
	} else if (ibi_type == RTK_I3C_IBI_INTR) {
		if (!ctx->cfg->common_cfg.ibi.enable_ibi) {
			LOG_ERR("IBI interrupt is not enabled!\n");
			return RTK_I3C_NOT_ENABLED;
		}
		LOG_DBG("Target %d issue ibi request!\n", ctx->cfg->common_cfg.instance_id);
		addr = ctx->cfg->tagt_info.dyn_addr;
		is_read = true;
	} else if (ibi_type == RTK_I3C_IBI_CTRL_REQ) {
		if (!ctx->cfg->common_cfg.ibi.enable_cr) {
			LOG_ERR("Controller role request is not enabled!\n");
			return RTK_I3C_NOT_ENABLED;
		}
		LOG_DBG("Target %d issue controller role request!\n",
			ctx->cfg->common_cfg.instance_id);
		addr = ctx->cfg->tagt_info.dyn_addr;
		is_read = false;
	}

	if ((ret = rtk_i3c_write_addr_frame(ctx, addr, is_read, false, RTK_I3C_SDR_MODE)) < 0) {
		return ret;
	}

	ctx->state = STATE_TAGT_IBI;

	LOG_DBG("Target %d ibi write %zu bytes (%s)\n", ctx->cfg->common_cfg.instance_id,
		msg != NULL ? msg->len : 0,
		msg != NULL && I3C_MSG_IS_DMA(msg->flags) ? "DMA" : "CPU");

	if (ibi_type == RTK_I3C_IBI_INTR) {
		msg->flags |= RTK_I3C_IBI;
		xfer_buffer = rtk_i3c_set_buffer(ctx, msg);
		if ((ret = rtk_i3c_write_data_frame(ctx, xfer_buffer->msg, false,
						    RTK_I3C_SDR_MODE)) < 0) {
			LOG_DBG("with %zu bytes\n", msg->len);
			return ret;
		}
		xfer_buffer->msg.count = ret;
	}
	rtk_i3c_core_start_xfer(ctx->core);

	return 0;
}

#endif /* CONFIG_RTK_I3C_IBI */
#endif /* CONFIG_RTK_I3C_TAGT */

/**
 * @brief The done isr is generated
 * @details when controller issue the byfm end, or the transaction
 * is nacked or early terminated by target. when target meet the stop condition.
 *      It handles the following event:
 *          @c RTK_I3C_EVENT_ADDRESS_ASSIGNMENT_COMPLETE
 *          @c RTK_I3C_EVENT_COMMAND_COMPLETE
 *          @c RTK_I3C_EVENT_WRITE_COMPLETE when tx fifo is written by cpu,
 *              this event will not be triggerd if controller do the repeated start.
 *
 * @param ctx
 */
static __always_inline void rtk_i3c_done_isr(rtk_i3c_ctx *ctx)
{
	LOG_DBG("\n");
	rtk_i3c_callback_args args = {
		.ctx = ctx->cfg->common_cfg.ctx,
	};

	if (ctx->state == STATE_CTRL_IBI) {
		return;
	}

#ifdef CONFIG_RTK_I3C_TAGT
	if (!I3C_ROLE_IS_CTRL(ctx->cfg->common_cfg.role)) {
		uint32_t trans_info = rtk_i3c_core_get_trinf(ctx->core);
		bool is_ccc = BIT_FIELD_GET(trans_info, 15, 15);
		uint8_t ccc_id = BIT_FIELD_GET(trans_info, 24, 31);

		ctx->ccc_id = is_ccc ? ccc_id : I3C_CCC_INVALID_ID;
	}
#endif /* CONFIG_RTK_I3C_TAGT */

	args.ccc_id = ctx->ccc_id;
	ctx->ccc_id = I3C_CCC_INVALID_ID;

	if (I3C_CCC_IS_DA(args.ccc_id)) {
		/* Address assignment CCC */
		args.event = RTK_I3C_EVENT_ADDRESS_ASSIGNMENT_COMPLETE;
#ifdef CONFIG_RTK_I3C_TAGT
		if (!I3C_ROLE_IS_CTRL(ctx->cfg->common_cfg.role)) {
			uint32_t addr_info = rtk_i3c_core_get_dsa(ctx->core);
			args.dyn_addr = BIT_FIELD_GET(addr_info, 0, 6);
			ctx->cfg->tagt_info.dyn_addr = args.dyn_addr;
		}
#endif /* CONFIG_RTK_I3C_TAGT */
	} else if (!I3C_CCC_IS_INVALID(args.ccc_id)) {
		/* Other CCC */
		args.event = RTK_I3C_EVENT_COMMAND_COMPLETE;
#ifdef CONFIG_RTK_I3C_TAGT
		if (I3C_CCC_IS_EC(args.ccc_id)) {
			rtk_i3c_core_get_ibi_cap(ctx->core, &ctx->cfg->common_cfg.ibi);
		}
	} else if (ctx->state == STATE_TAGT_IBI) {
		/* Target IBI transmission completed successfully */
		args.event = RTK_I3C_EVENT_IBI_WRITE_COMPLETE;
		args.count = ctx->ibi_buffer.msg.count;
		args.len = ctx->ibi_buffer.msg.len;
		ctx->ibi_buffer = (rtk_i3c_rx_buffer){0};
	} else if (ctx->state == STATE_TAGT_PRV_READ) {
		/* Target received a private write from the controller. Complete on
		 * STOP/DONE since the controller may write fewer bytes than the
		 * prepared RX buffer length.
		 */
		args.event = RTK_I3C_EVENT_READ_COMPLETE;
		args.count = ctx->rx_buffer.msg.count;
		args.len = ctx->rx_buffer.msg.len;
		ctx->rx_buffer = (rtk_i3c_rx_buffer){0};
#endif /* CONFIG_RTK_I3C_TAGT */
	} else if (I3C_STATE_IS_WRITE(ctx->state) && !I3C_MSG_IS_DMA(ctx->tx_buffer.msg.flags) &&
		   ctx->tx_buffer.msg.count == ctx->tx_buffer.msg.len) {
		/* Write transfer done. Only triggerd when count==len to avoid hdr-exit pattern
		 * clear the buffer */
		args.count = ctx->tx_buffer.msg.count;
		args.len = ctx->tx_buffer.msg.len;
		args.event = RTK_I3C_EVENT_WRITE_COMPLETE;
		ctx->tx_buffer = (rtk_i3c_tx_buffer){0};
	} else {
		/* Unrecogonized Event */
		return;
	}

	if (!I3C_ROLE_IS_CTRL(ctx->cfg->common_cfg.role)) {
#ifdef CONFIG_RTK_I3C_TAGT
		if (args.event == RTK_I3C_EVENT_ADDRESS_ASSIGNMENT_COMPLETE &&
		    ctx->rx_buffer.msg.data != NULL && ctx->rx_buffer.msg.len != 0) {
			/* Keep a previously armed target RX buffer valid across
			 * ENTDAA/SETNEWDA. The echo app may arm RX before the
			 * controller assigns a dynamic address.
			 */
			ctx->state = STATE_TAGT_PRV_READ;
		} else {
			ctx->state = STATE_TAGT_IDLE;
		}
#endif /* CONFIG_RTK_I3C_TAGT */
	} else {
		ctx->state = STATE_CTRL_IDLE;
	}
	if (ctx->cfg->common_cfg.callback != NULL) {
		ctx->cfg->common_cfg.callback(&args);
	}
}

/**
 * @brief The rxne isr is generated when rx fifo is not empty.
 *        It handles the following event:
 *          @c RTK_I3C_EVENT_READ_COMPLETE when rx fifo is read by cpu
 * @param ctx
 */
static __always_inline void rtk_i3c_rxne_isr(rtk_i3c_ctx *ctx)
{
	LOG_DBG("\n");
	/* Read data from the FIFO. */
	uint16_t read_len = 0;

	if ((read_len = rtk_i3c_core_get_rx_fifo_len(ctx->core)) == 0) {
		return;
	}

#ifdef CONFIG_RTK_I3C_TAGT
	if (I3C_ROLE_IS_TAGT(ctx->cfg->common_cfg.role) && ctx->state != STATE_TAGT_PRV_READ &&
	    ctx->ccc_id != I3C_CCC_BRCT_ENTDAA) {
		LOG_WRN("Target RXNE ignored: state=%d rxfl=%u", ctx->state, read_len);
		rtk_i3c_core_flush_rx(ctx->core);
		return;
	}
#endif /* CONFIG_RTK_I3C_TAGT */

#ifdef CONFIG_RTK_I3C_DMA
	if (I3C_MSG_IS_DMA(ctx->rx_buffer.msg.flags)) {
		read_len = ctx->rx_buffer.msg.len;
	}
#endif

	if (rtk_i3c_read_fifo(ctx, &ctx->rx_buffer, read_len, false) < 0) {
		return;
	}

	if (ctx->ccc_id == I3C_CCC_BRCT_ENTDAA) {
		/* DAA processing */
		rtk_i3c_daa_process(ctx, true);
		rtk_i3c_write_end_frame(ctx->core);
		goto exit;
	}

#ifdef CONFIG_RTK_I3C_DMA
	if (I3C_MSG_IS_DMA(ctx->rx_buffer.msg.flags)) {
		return;
	}
#endif
	rtk_i3c_callback_args args = {
		.ctx = ctx->cfg->common_cfg.ctx,
	};

	args.len = ctx->rx_buffer.msg.len;
	if (ctx->rx_buffer.msg.len != 0 && ctx->rx_buffer.msg.count != 0 &&
	    !(I3C_ROLE_IS_TAGT(ctx->cfg->common_cfg.role) && ctx->state == STATE_TAGT_PRV_READ)) {
		/* Controller read transfers complete when the requested data is
		 * received. Target private writes complete on STOP/DONE because the
		 * controller may write fewer bytes than the prepared RX buffer length.
		 */
		args.count = ctx->rx_buffer.msg.count;
		args.event = RTK_I3C_EVENT_READ_COMPLETE;
		if (ctx->cfg->common_cfg.callback != NULL) {
			ctx->cfg->common_cfg.callback(&args);
		}
	}

exit:
	if (ctx->rx_buffer.msg.count == ctx->rx_buffer.msg.len &&
	    !(I3C_ROLE_IS_TAGT(ctx->cfg->common_cfg.role) && ctx->state == STATE_TAGT_PRV_READ)) {
		ctx->state = (!I3C_ROLE_IS_CTRL(ctx->cfg->common_cfg.role)) ? STATE_TAGT_IDLE
									    : STATE_CTRL_IDLE;
		ctx->rx_buffer = (rtk_i3c_rx_buffer){0};
	}
}

/**
 * @brief The rxnak isr is generated
 *        when controller recieved an nack from target.
 *        It handles the following event:
 *          @c RTK_I3C_EVENT_INTERNAL_ERROR
 * @param ctx
 */
static __always_inline void rtk_i3c_rxnak_isr(rtk_i3c_ctx *ctx)
{
	LOG_DBG("\n");

	if (!I3C_ROLE_IS_CTRL(ctx->cfg->common_cfg.role)) {
		if (ctx->state == STATE_TAGT_IBI) {
			/* The target's IBI/HJ/CR address was NACKed (rejected) by the
			 * controller. Discard the partially-queued IBI frame (BYFM) and
			 * payload (TXDA) so a retry starts from a clean FIFO instead of
			 * appending to stale bytes, return the engine to idle, and report
			 * it as an accept failure so the caller can retry.
			 */
			rtk_i3c_core_flush_byfm(ctx->core);
			rtk_i3c_core_flush_tx(ctx->core);
			ctx->state = STATE_TAGT_IDLE;
			rtk_i3c_callback_args args = {
				.ctx = ctx->cfg->common_cfg.ctx,
				.event = RTK_I3C_EVENT_ARBITRATE_FAIL,
			};
			if (ctx->cfg->common_cfg.callback != NULL) {
				ctx->cfg->common_cfg.callback(&args);
			}
			return;
		}
		/* Otherwise this is a normal host-read NACK ending a read; ignore it
		 * (target transfer completion is handled by DONE) and do not force the
		 * target into the controller idle state.
		 */
		return;
	}

	// clear state
	ctx->state = STATE_CTRL_IDLE;
	rtk_i3c_callback_args args = {
		.ctx = ctx->cfg->common_cfg.ctx,
		.event = RTK_I3C_EVENT_INTERNAL_ERROR,
	};
	if (ctx->cfg->common_cfg.callback != NULL) {
		ctx->cfg->common_cfg.callback(&args);
	}
}

#ifdef CONFIG_RTK_I3C_IBI
/**
 * @brief RXIBI ISR - Regular IBI interrupt handler
 *
 * Triggered when a Target sends an IBI (In-Band Interrupt) to Controller.
 * This is the normal IBI path (RnW=1, sending interrupt data).
 *
 * @param ctx Pointer to I3C context
 */
static __always_inline void rtk_i3c_rxibi_isr(rtk_i3c_ctx *ctx)
{
	LOG_DBG("\n");

	uint32_t trans_info = rtk_i3c_core_get_trinf(ctx->core);
	uint8_t ibi_addr = BIT_FIELD_GET(trans_info, 0, 6);
	uint8_t mdb = BIT_FIELD_GET(trans_info, 24, 31);
	uint16_t read_len = 0;
	int bcr = 0;

	/*
	 * A Hot Join arbitrates on the reserved address 0x02 and can also raise
	 * the RXIBI interrupt, but it is handled by the HJ ISR (which runs ENTDAA)
	 * and has no target BCR. Skip it here so the BCR lookup does not log a
	 * spurious "addr 2 not found" error.
	 */
	if (ibi_addr == I3C_HJ_ADDR) {
		return;
	}

	if ((bcr = rtk_i3c_bus_get_bcr_by_addr(ctx, ibi_addr)) < 0) {
		return;
	};

	rtk_i3c_callback_args args = {
		.event = RTK_I3C_EVENT_IBI_READ_COMPLETE,
		.ibi_type = RTK_I3C_IBI_INTR,
		.ibi_addr = ibi_addr,
		.ctx = ctx->cfg->common_cfg.ctx,
	};
	ctx->state = STATE_CTRL_IBI;

	(void)args;

	if (bcr & I3C_BCR_IBI_PAYLOAD) {
		/* bcr[2] = 1, receive ibi payload */
#ifdef CONFIG_RTK_I3C_DMA
		/* mdb is received by i3c not by dma */
		if (I3C_MSG_IS_DMA(ctx->ibi_buffer.msg.flags)) {
			read_len = ctx->ibi_buffer.msg.len > 0 ? ctx->ibi_buffer.msg.len - 1 : 0;
		} else
#endif
		{
			read_len = rtk_i3c_core_get_rx_fifo_len(ctx->core);
		}
		/* Reserved for mdb */
		ctx->ibi_buffer.msg.count++;
		if (rtk_i3c_read_fifo(ctx, &ctx->ibi_buffer, read_len, true) < 0) {
			return;
		}
		*ctx->ibi_buffer.msg.data = mdb;
	}

#ifdef CONFIG_RTK_I3C_DMA
	if (I3C_MSG_IS_DMA(ctx->ibi_buffer.msg.flags)) {
		return;
	}
#endif
	args.len = ctx->ibi_buffer.msg.len;
	/* IBI read done */
	args.count = ctx->ibi_buffer.msg.count;
	if (ctx->cfg->common_cfg.callback != NULL) {
		ctx->cfg->common_cfg.callback(&args);
	}
	if (ctx->ibi_buffer.msg.count == ctx->ibi_buffer.msg.len) {
		ctx->state = (!I3C_ROLE_IS_CTRL(ctx->cfg->common_cfg.role)) ? STATE_TAGT_IDLE
									    : STATE_CTRL_IDLE;
		ctx->ibi_buffer = (rtk_i3c_rx_buffer){0};
	}
}

/**
 * @brief HJ ISR - Hot Join interrupt handler
 *
 * Triggered when a new Target requests to join the I3C bus.
 * The new Target sends 7'h02 (Hot Join address) with RnW=0.
 *
 * @param ctx Pointer to I3C context
 */
static __always_inline void rtk_i3c_hj_isr(rtk_i3c_ctx *ctx)
{
	LOG_DBG("\n");

	/* Send ENTDAA CCC command */
	rtk_i3c_daa_send_ccc(ctx);

	/* Send read characteristics info request */
	rtk_i3c_daa_read_char_info(ctx);

	/* Return immediately - DAA processing continues in RXNE ISR */
}

/**
 * @brief CR ISR - Controller Role Request interrupt handler
 *
 * Triggered when a Secondary Controller requests bus control.
 * The requesting device sends its Dynamic Address with RnW=0.
 *
 * @param ctx Pointer to I3C context
 */
static __always_inline void rtk_i3c_cr_isr(rtk_i3c_ctx *ctx)
{
	LOG_DBG("\n");
	uint8_t ibi_addr = BIT_FIELD_GET(rtk_i3c_core_get_trinf(ctx->core), 0, 6);

	rtk_i3c_callback_args args = {
		.event = RTK_I3C_EVENT_IBI_READ_COMPLETE,
		.ibi_type = RTK_I3C_IBI_CTRL_REQ,
		.ibi_addr = ibi_addr,
		.ctx = ctx->cfg->common_cfg.ctx,
	};
	if (ctx->cfg->common_cfg.callback != NULL) {
		ctx->cfg->common_cfg.callback(&args);
	}
}

/**
 * @brief DAF ISR - Data Arbitration Fail interrupt handler
 *
 * Triggered when Target loses arbitration during IBI transmission.
 * Another device won the arbitration, this Target should stop transmitting.
 *
 * @param ctx Pointer to I3C context
 */
static __always_inline void rtk_i3c_daf_isr(rtk_i3c_ctx *ctx)
{
	LOG_DBG("\n");

	/* For Target: DAF means arbitration lost during IBI transmission.
	 * Discard the partially-queued IBI frame (BYFM) and payload (TXDA) so a
	 * retry starts from a clean FIFO instead of appending to stale bytes.
	 */
	rtk_i3c_core_flush_byfm(ctx->core);
	rtk_i3c_core_flush_tx(ctx->core);
	ctx->state = STATE_TAGT_IDLE;

	rtk_i3c_callback_args args = {
		.event = RTK_I3C_EVENT_ARBITRATE_FAIL,
		.ctx = ctx->cfg->common_cfg.ctx,
	};
	if (ctx->cfg->common_cfg.callback != NULL) {
		ctx->cfg->common_cfg.callback(&args);
	}
}
#endif /* CONFIG_RTK_I3C_IBI */

/**
 * @brief I3C interrupt service routine. This function dispatch
 *        different event to sub isr handlers.
 *
 * @param ctx
 */
void rtk_i3c_isr(rtk_i3c_ctx *ctx)
{
	uint32_t isr_bits = rtk_i3c_core_get_isr(ctx->core);

	LOG_DBG("Instance %d, ISR=0x%08" PRIx32 "\n", ctx->cfg->common_cfg.instance_id, isr_bits);

#ifdef CONFIG_RTK_I3C_IBI
	/* IBI type interrupts (independent handlers) */
	if (isr_bits & I3C_ISR_RXIBI_MASK) {
		rtk_i3c_rxibi_isr(ctx);
	}
	if (isr_bits & I3C_ISR_HJ_MASK) {
		rtk_i3c_hj_isr(ctx);
	}
	if (isr_bits & I3C_ISR_CR_MASK) {
		rtk_i3c_cr_isr(ctx);
	}
	if (isr_bits & I3C_ISR_DAF_MASK) {
		rtk_i3c_daf_isr(ctx);
	}
#endif /* CONFIG_RTK_I3C_IBI */

	if (isr_bits & I3C_ISR_RXNE_MASK) {
		rtk_i3c_rxne_isr(ctx);
	}
	if (isr_bits & I3C_ISR_RXNAK_MASK) {
		rtk_i3c_rxnak_isr(ctx);
	}
	if ((isr_bits & I3C_ISR_DONE_MASK) && !I3C_ISR_IS_HJorCR(isr_bits)) {
		rtk_i3c_done_isr(ctx);
	}
	rtk_i3c_core_clear_isr(ctx->core, isr_bits);
}
#endif /* CONFIG_RTK_I3C */
