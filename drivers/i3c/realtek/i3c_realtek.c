/*
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * Copyright (c) 2025, Realtek Semiconductor Corp. All rights reserved.
 */

#define DT_DRV_COMPAT realtek_rts5918_i3c

#include <errno.h>
#include <string.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/clock_control_rts5918.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/i3c.h>
#include <zephyr/drivers/i3c/target_device.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/irq.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

#include "rtk_i3c.h"
#include "rtk_i3c_bus.h"
#include "port_i3c.h"

LOG_MODULE_REGISTER(i3c_realtek, CONFIG_I3C_REALTEK_LOG_LEVEL);

#define I3C_REALTEK_ADDR_SLOT_BITS    32U
#define I3C_REALTEK_ADDR_SLOT_WORDS   ROUND_UP(RTK_I3C_MAX_DYN_ADDR + 1U, I3C_REALTEK_ADDR_SLOT_BITS)
#define I3C_REALTEK_MAX_DEVS          RTK_I3C_MAX_TAGT_COUNT
/* Internal RX staging buffer for controller-initiated writes to this target.
 * Sized to cover the largest validated private/legacy transfer (256 bytes).
 */
#define I3C_REALTEK_RX_BUF_SIZE       256U
#define I3C_REALTEK_IBI_TIMEOUT       K_MSEC(100)
/* When a target-raised IBI/HJ is rejected (NACKed) or loses arbitration, retry
 * a few times with a short backoff before giving up. Rejection is transient
 * (the controller may be busy), unlike a timeout (no active controller).
 *
 * Hot Join gets its own, more persistent policy: joining the bus matters more,
 * and I3C requires a longer wait after a rejected hot join before retrying.
 */
#define I3C_REALTEK_IBI_RETRY_MAX     3
#define I3C_REALTEK_IBI_RETRY_BACKOFF K_MSEC(1)
#define I3C_REALTEK_HJ_RETRY_MAX      5
#define I3C_REALTEK_HJ_RETRY_BACKOFF  K_MSEC(10)

/* Delay before re-sending Hot-Join after the controller re-enables it (ENEC),
 * so the ENEC frame's STOP completes and the bus settles to the Bus Available
 * Condition first. The precise electrical wait is enforced in hardware (TAVAL /
 * bus_available_ns); this just defers past the in-flight CCC.
 */
#define I3C_REALTEK_HJ_REENABLE_DELAY K_MSEC(1)

struct i3c_realtek_config {
	struct i3c_driver_config common;
	uintptr_t base;
	const struct pinctrl_dev_config *pincfg;
	const struct device *clock_dev;
	struct rts5918_sccon_subsys sccon_cfg;
	uint32_t i3c_freq_hz;
	uint32_t i3c_od_scl_hz;
	uint8_t instance_id;
	uint8_t role;
	uint8_t static_addr;
	uint8_t dynamic_addr;
	uint64_t pid;
	uint8_t bcr;
	uint8_t dcr;
	uint16_t max_read_len;
	uint16_t max_write_len;
	void (*irq_config_func)(const struct device *dev);
};

struct i3c_realtek_data {
	struct i3c_driver_data common;
	struct k_mutex bus_lock;
	struct k_sem ccc_end;
	struct k_sem xfer_end;
	struct k_sem ibi_sem;
	int ibi_status;           /* result of the in-flight IBI/HJ/CR, read after ibi_sem */
	bool hj_pending;          /* a hot-join raise is waiting; DAA completion means success */
	bool hj_wanted;           /* target wants to hot-join and has no dynamic address yet */
	bool hj_enabled_prev;     /* last observed HJ-enable state, to spot DISEC->ENEC */
	const struct device *dev; /* back-pointer for the deferred HJ-retry work */
	struct k_work_delayable hj_retry_work; /* re-send HJ after controller re-enables it */
	uint32_t num_xfer;
	rtk_i3c_ctx rtk_ctx;
	rtk_i3c_cfg rtk_cfg;
	rtk_i3c_bus_tagt_item tagt_table[I3C_REALTEK_MAX_DEVS];
	uint32_t addr_slots[I3C_REALTEK_ADDR_SLOT_WORDS];
	struct i3c_target_config *target_config;
	uint8_t rx_buf[I3C_REALTEK_RX_BUF_SIZE];
#ifdef CONFIG_I3C_USE_IBI
	/* Controller-side staging buffer for IBI payloads received from targets
	 * (byte 0 is the MDB). Re-armed after each delivered IBI.
	 */
	uint8_t ibi_rx_buf[CONFIG_I3C_IBI_MAX_PAYLOAD_SIZE];
#endif
};

static const struct device *i3c_realtek_devices[DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT)];

static int i3c_realtek_err_to_errno(int ret)
{
	if (ret == 0) {
		return 0;
	}

	switch (ret) {
	case RTK_I3C_INVAL_PARAM:
		return -EINVAL;
	case RTK_I3C_NOT_SUPPORTED:
		return -ENOTSUP;
	case RTK_I3C_BUSY:
		return -EBUSY;
	case RTK_I3C_NOT_ENABLED:
		return -ENODEV;
	case RTK_I3C_TXFIFO_WILL_FULL:
	case RTK_I3C_READ_BUFFER_FULL:
		return -ENOSPC;
	case RTK_I3C_TIMEOUT:
		return -ETIMEDOUT;
	default:
		return -EIO;
	}
}

uintptr_t plat_i3c_get_base(uint8_t instance_id)
{
	const struct device *dev;

	if (instance_id >= ARRAY_SIZE(i3c_realtek_devices)) {
		return 0;
	}

	dev = i3c_realtek_devices[instance_id];
	if (dev == NULL) {
		return 0;
	}

	return ((const struct i3c_realtek_config *)dev->config)->base;
}

void plat_i3c_init_isr(rtk_i3c_ctx *ctx, uintptr_t vector)
{
	ARG_UNUSED(ctx);
	ARG_UNUSED(vector);
}

static void i3c_realtek_isr(const struct device *dev)
{
	struct i3c_realtek_data *data = dev->data;

	rtk_i3c_isr(&data->rtk_ctx);
}

static int i3c_realtek_attach_i3c_device(const struct device *dev, struct i3c_device_desc *target)
{
	struct i3c_realtek_data *data = dev->data;
	const char *name = target->dev ? target->dev->name : "i3c";
	/* The subsystem no longer passes the attach address (I3C API 4.4.1); derive
	 * it the same way i3c_attach_i3c_device() does: dynamic if assigned, else
	 * static.
	 */
	uint8_t addr = target->dynamic_addr ? target->dynamic_addr : target->static_addr;

	for (uint8_t i = 0; i < I3C_REALTEK_MAX_DEVS; i++) {
		if (data->tagt_table[i].info.char_info.pid == 0 ||
		    data->tagt_table[i].info.char_info.pid == target->pid) {
			int ret = i3c_realtek_err_to_errno(rtk_i3c_bus_init_tagt_table(
				&data->rtk_ctx, i, name, addr, target->static_addr, target->pid,
				false));

			if (ret == -EINVAL && target->static_addr == addr) {
				ret = 0;
			}

			return ret;
		}
	}

	return -ENOSPC;
}

static int i3c_realtek_reattach_i3c_device(const struct device *dev, struct i3c_device_desc *target,
					   uint8_t old_dyn_addr)
{
	struct i3c_realtek_data *data = dev->data;

	return i3c_realtek_err_to_errno(rtk_i3c_bus_reattach_tagt(
		&data->rtk_ctx, old_dyn_addr, target->dynamic_addr, target->static_addr));
}

static int i3c_realtek_detach_i3c_device(const struct device *dev, struct i3c_device_desc *target)
{
	struct i3c_realtek_data *data = dev->data;

	if (target->dynamic_addr != 0U) {
		rtk_i3c_bus_free_addr(&data->rtk_ctx, target->dynamic_addr);
	}

	return 0;
}

/*
 * Legacy I2C support: a plain I2C device on the I3C bus is addressed with the
 * I2C protocol (open-drain, no CCC). These back the Zephyr i2c_* API and the
 * subsystem's attach/detach of devicetree-declared I2C children.
 */
static int i3c_realtek_i2c_configure(const struct device *dev, uint32_t dev_config)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(dev_config);
	/* The I2C SCL rate comes from the devicetree (i2c-scl-hz) at init; accept
	 * the request without reconfiguring the bus on the fly.
	 */
	return 0;
}

static int i3c_realtek_i2c_transfer(const struct device *dev, struct i2c_msg *msgs,
				    uint8_t num_msgs, uint16_t addr)
{
	struct i3c_realtek_data *data = dev->data;
	int ret = 0;

	if (msgs == NULL) {
		return -EINVAL;
	}

	k_mutex_lock(&data->bus_lock, K_FOREVER);

	for (uint8_t i = 0; i < num_msgs; i++) {
		/* A zero-length message is an address-only presence check (the i2c
		 * shell `scan` command issues a zero-length write per address). Probe
		 * the address and report the target ACK/NACK.
		 */
		if (msgs[i].len == 0U) {
			ret = i3c_realtek_err_to_errno(
				rtk_i3c_ctrl_probe(&data->rtk_ctx, (uint8_t)addr, RTK_I3C_I2C));
			if (ret != 0) {
				break;
			}
			continue;
		}

		if (msgs[i].buf == NULL) {
			ret = -EINVAL;
			break;
		}

		rtk_i3c_msg msg = {
			.data = msgs[i].buf,
			.len = msgs[i].len,
			.count = 0U,
			.flags = (msgs[i].flags & I2C_MSG_READ) ? RTK_I3C_READ : RTK_I3C_WRITE,
		};
		rtk_i3c_tagt tagt = {
			.addr = (uint8_t)addr,
			.msg = &msg,
		};
		/* Legacy I2C SDR (no CCC). Keep the bus (repeated start) unless this
		 * message is flagged to end with a STOP.
		 */
		bool restart = ((msgs[i].flags & I2C_MSG_STOP) == 0U);

		ret = i3c_realtek_err_to_errno(
			rtk_i3c_ctrl_xfer(&data->rtk_ctx, &tagt, RTK_I3C_I2C, restart));
		if (ret != 0) {
			break;
		}
	}

	k_mutex_unlock(&data->bus_lock);

	return ret;
}

static int i3c_realtek_attach_i2c_device(const struct device *dev, struct i3c_i2c_device_desc *desc)
{
	struct i3c_realtek_data *data = dev->data;

	/* Record the I2C device in the core target table (reserves its address).
	 * Reuse the slot already holding this I2C address, else take an empty one.
	 */
	for (uint8_t i = 0; i < I3C_REALTEK_MAX_DEVS; i++) {
		rtk_i3c_bus_tagt_item *slot = &data->tagt_table[i];
		bool empty = !slot->info.is_i2c && slot->info.char_info.pid == 0U &&
			     slot->info.dyn_addr == 0U && slot->info.stc_addr == 0U;
		bool same = slot->info.is_i2c && slot->info.stc_addr == desc->addr;

		if (empty || same) {
			return i3c_realtek_err_to_errno(rtk_i3c_bus_init_tagt_table(
				&data->rtk_ctx, i, "i2c", 0U, desc->addr, 0U, true));
		}
	}

	return -ENOSPC;
}

static int i3c_realtek_detach_i2c_device(const struct device *dev, struct i3c_i2c_device_desc *desc)
{
	struct i3c_realtek_data *data = dev->data;

	if (desc->addr != 0U) {
		rtk_i3c_bus_free_addr(&data->rtk_ctx, desc->addr);
	}

	return 0;
}

static int i3c_realtek_address_slots_init(const struct device *dev)
{
	const struct i3c_realtek_config *config = dev->config;
	struct i3c_realtek_data *data = dev->data;
	uint8_t controller_da;
	int ret;

	ret = i3c_addr_slots_init(dev);
	if (ret != 0) {
		return ret;
	}

	if (config->role != RTK_I3C_CTRL_PRIM) {
		return 0;
	}

	if (config->dynamic_addr != 0U &&
	    i3c_addr_slots_is_free(&data->common.attached_dev.addr_slots, config->dynamic_addr)) {
		controller_da = config->dynamic_addr;
	} else {
		controller_da =
			i3c_addr_slots_next_free_find(&data->common.attached_dev.addr_slots, 0);
	}

	if (controller_da == 0U) {
		return -ENOSPC;
	}

	data->common.ctrl_config.is_secondary = false;
	i3c_addr_slots_mark_i3c(&data->common.attached_dev.addr_slots, controller_da);

	return 0;
}

static void i3c_realtek_handle_daa_phase(const struct device *dev,
					 rtk_i3c_tagt_char_info *tagt_char_info, uint8_t *dyn_addr)
{
	const struct i3c_realtek_config *config = dev->config;
	struct i3c_realtek_data *data = dev->data;
	struct i3c_device_desc *target = NULL;
	uint8_t assigned_addr = 0U;
	int ret;

	ret = i3c_dev_list_daa_addr_helper(&data->common.attached_dev.addr_slots,
					   &config->common.dev_list, tagt_char_info->pid, false,
					   false, &target, &assigned_addr);
	if (ret != 0) {
		assigned_addr = 0U;
	}

	if (target == NULL) {
		for (uint8_t i = 0; i < config->common.dev_list.num_i3c; i++) {
			struct i3c_device_desc *desc = &config->common.dev_list.i3c[i];

			if (desc->dynamic_addr == 0U || desc->dynamic_addr == desc->static_addr) {
				target = desc;
				assigned_addr = desc->init_dynamic_addr;
				break;
			}
		}
	}

	if (assigned_addr != 0U) {
		*dyn_addr = assigned_addr;
	} else {
		rtk_i3c_bus_handle_daa(&data->rtk_ctx, tagt_char_info, dyn_addr);
		assigned_addr = *dyn_addr;
	}

	if (target != NULL) {
		target->dynamic_addr = assigned_addr;
		target->bcr = tagt_char_info->bcr;
		target->dcr = tagt_char_info->dcr;

		/* Program the HW per-target IBI payload bit for this address. The
		 * bus-layer path (rtk_i3c_bus_handle_daa) does this for undeclared
		 * devices; declared devices bound here must set it too, otherwise a
		 * payload-carrying IBI (BCR bit2=1) stalls after the address ACK
		 * because the core never clocks the MDB/payload in.
		 */
		rtk_i3c_set_ibi_mdb(&data->rtk_ctx, assigned_addr,
				    (tagt_char_info->bcr & I3C_BCR_IBI_PAYLOAD) != 0);

		for (uint8_t i = 0; i < I3C_REALTEK_MAX_DEVS; i++) {
			if (!data->tagt_table[i].active ||
			    data->tagt_table[i].info.char_info.pid == tagt_char_info->pid ||
			    data->tagt_table[i].info.char_info.pid == target->pid) {
				data->tagt_table[i].active = true;
				data->tagt_table[i].info.char_info = *tagt_char_info;
				data->tagt_table[i].info.dyn_addr = assigned_addr;
				data->tagt_table[i].info.stc_addr = target->static_addr;
				break;
			}
		}
	}
}

/*
 * Arm the target RX path with a fresh staging buffer. Routed through
 * rtk_i3c_tagt_xfer() so the core enters STATE_TAGT_PRV_READ: the core's
 * target RXNE handler flushes incoming bytes unless it is in that state, so a
 * plain buffer-descriptor update is not enough. Must be called while the core
 * is idle (at configure time, or after a completed transfer).
 */
static void i3c_realtek_arm_rx(struct i3c_realtek_data *data)
{
	rtk_i3c_msg msg = {
		.data = data->rx_buf,
		.len = sizeof(data->rx_buf),
		.count = 0U,
		.flags = RTK_I3C_READ,
	};
	unsigned int key;

	/* rtk_i3c_tagt_xfer() mutates the shared core state/buffer. Guard against
	 * the completion ISR (which also arms RX) interleaving with a thread-context
	 * arm; a plain mutex would not lock out the ISR. Harmless (nested) when this
	 * is already called from ISR context.
	 */
	key = irq_lock();
	(void)rtk_i3c_tagt_xfer(&data->rtk_ctx, &msg);
	irq_unlock(key);
}

/*
 * Hand the core a fresh buffer mid-transfer, in response to READ_BUFFER_FULL.
 * The core is still receiving (STATE_TAGT_PRV_READ) and preserves the running
 * byte count across this callback, so only the buffer descriptor is refreshed.
 */
static void i3c_realtek_refill_rx(struct i3c_realtek_data *data)
{
	data->rtk_ctx.rx_buffer.msg.data = data->rx_buf;
	data->rtk_ctx.rx_buffer.msg.len = sizeof(data->rx_buf);
	data->rtk_ctx.rx_buffer.msg.flags = RTK_I3C_READ;
	data->rtk_ctx.rx_buffer.buffer_requested = false;
}

#ifdef CONFIG_I3C_USE_IBI
/*
 * Controller side: arm the IBI receive buffer so a target-raised IBI's payload
 * can be captured. Re-armed after each IBI is delivered. irq_lock guards the
 * shared core state against the completion ISR (see i3c_realtek_arm_rx).
 */
static void i3c_realtek_arm_ibi_rx(struct i3c_realtek_data *data)
{
	rtk_i3c_msg msg = {
		.data = data->ibi_rx_buf,
		.len = sizeof(data->ibi_rx_buf),
		.count = 0U,
		.flags = RTK_I3C_READ,
	};
	unsigned int key;

	key = irq_lock();
	(void)rtk_i3c_ibi_read(&data->rtk_ctx, &msg);
	irq_unlock(key);
}
#endif /* CONFIG_I3C_USE_IBI */

static void i3c_realtek_hal_callback(rtk_i3c_callback_args *const args)
{
	const struct device *dev = (const struct device *)args->ctx;
	struct i3c_realtek_data *data = dev->data;
	const struct i3c_target_callbacks *target_cb;

	switch (args->event) {
	case RTK_I3C_EVENT_ADDRESS_ASSIGNMENT_PHASE:
		i3c_realtek_handle_daa_phase(dev, args->tagt_char_info, &args->dyn_addr);
		break;
	case RTK_I3C_EVENT_ADDRESS_ASSIGNMENT_COMPLETE:
		if (((const struct i3c_realtek_config *)dev->config)->role == RTK_I3C_TAGT) {
			data->rtk_cfg.tagt_info.dyn_addr = args->dyn_addr;
			/* Got an address: no need to (re)try Hot-Join anymore. */
			data->hj_wanted = false;
			/* Let a registered target app observe its assigned address. */
			if (data->target_config != NULL) {
				data->target_config->address = args->dyn_addr;
			}
			/* A hot join that is accepted flows straight into ENTDAA, so the
			 * controller assigns an address instead of ending with a plain
			 * IBI_WRITE_COMPLETE. Treat DAA completion as the hot-join success
			 * signal for a waiting ibi_raise().
			 */
			if (data->hj_pending) {
				data->ibi_status = 0;
				k_sem_give(&data->ibi_sem);
			}
		}
		break;
	case RTK_I3C_EVENT_READ_BUFFER_FULL:
		/* Core needs (more) RX space mid-transfer: refresh the buffer. */
		i3c_realtek_refill_rx(data);
		break;
	case RTK_I3C_EVENT_READ_COMPLETE:
		data->num_xfer = args->count;
		/* Target role: deliver the bytes written to us by the controller.
		 * Controllers do their own read into the caller's buffer and have
		 * no registered target_config, so this block is skipped for them.
		 */
		if (data->target_config != NULL && data->target_config->callbacks != NULL) {
			target_cb = data->target_config->callbacks;
			/* The RX is fully buffered, so the whole controller write
			 * surfaces here at once.
			 */
#ifdef CONFIG_I3C_TARGET_BUFFER_MODE
			/* Buffer-mode apps (e.g. MCTP-over-I3C) take the whole write
			 * in one shot. Prefer this over the byte-by-byte replay when a
			 * buffer callback is registered.
			 */
			if (target_cb->buf_write_received_cb != NULL) {
				target_cb->buf_write_received_cb(data->target_config, data->rx_buf,
								 args->count);
			} else
#endif
			{
				/* Replay the standard byte-oriented target callback
				 * sequence the API defines (write_requested ->
				 * write_received per byte) so apps that key off
				 * write_requested_cb (e.g. to reset their RX buffer /
				 * mark the transfer direction) work unchanged.
				 */
				if (target_cb->write_requested_cb != NULL) {
					target_cb->write_requested_cb(data->target_config);
				}
				if (target_cb->write_received_cb != NULL) {
					for (uint32_t i = 0; i < args->count; i++) {
						target_cb->write_received_cb(data->target_config,
									     data->rx_buf[i]);
					}
				}
			}
			/* STOP terminates the write; buffer-mode apps (MCTP) act on it
			 * to hand the received packet up the stack.
			 */
			if (target_cb->stop_cb != NULL) {
				target_cb->stop_cb(data->target_config);
			}
			/* Re-arm RX so back-to-back controller writes are captured. If
			 * the app instead answers with a read (i3c_target_tx_write), that
			 * preloaded write preempts this idle RX arm in rtk_i3c_tagt_xfer().
			 */
			i3c_realtek_arm_rx(data);
		}
		k_sem_give(&data->xfer_end);
		break;
	case RTK_I3C_EVENT_WRITE_COMPLETE:
		data->num_xfer = args->count;
		k_sem_give(&data->xfer_end);
		if (data->target_config != NULL && data->target_config->callbacks != NULL) {
			target_cb = data->target_config->callbacks;
			/* The controller-read data was supplied up front via
			 * i3c_target_tx_write(), so the read callback is not used to
			 * fetch data here; it is replayed only so apps that mark the
			 * transfer direction (paired with stop_cb) behave correctly.
			 */
#ifdef CONFIG_I3C_TARGET_BUFFER_MODE
			/* Buffer-mode apps get the buffer-oriented read notification.
			 * Data was preloaded via i3c_target_tx_write(), so ptr/len/
			 * hdr_mode are NULL: this only signals the read, matching the
			 * npcx / it51xxx target drivers.
			 */
			if (target_cb->buf_read_requested_cb != NULL) {
				target_cb->buf_read_requested_cb(data->target_config, NULL, NULL,
								 NULL);
			} else
#endif
			{
				if (target_cb->read_requested_cb != NULL) {
					uint8_t first = 0U;

					target_cb->read_requested_cb(data->target_config, &first);
				}
			}
			if (target_cb->stop_cb != NULL) {
				target_cb->stop_cb(data->target_config);
			}
			/* Read-back done; re-arm RX for the next controller write. */
			i3c_realtek_arm_rx(data);
		}
		break;
	case RTK_I3C_EVENT_COMMAND_COMPLETE:
		data->num_xfer = args->count;
		/* Target: an ENEC/DISEC just updated our HJ-enable state (done_isr
		 * refreshed common_cfg.ibi). Per the I3C spec, a target that saw
		 * DISEC(DISHJ) and still lacks a dynamic address may re-send its
		 * Hot-Join once ENEC(ENHJ) re-enables it. Detect that disable->enable
		 * transition and, if we still want to join, defer a HJ retry.
		 */
		if (((const struct i3c_realtek_config *)dev->config)->role == RTK_I3C_TAGT) {
			bool hj_now = data->rtk_cfg.common_cfg.ibi.enable_hj;

			if (!data->hj_enabled_prev && hj_now && data->hj_wanted &&
			    data->rtk_cfg.tagt_info.dyn_addr == 0U) {
				k_work_reschedule(&data->hj_retry_work,
						  I3C_REALTEK_HJ_REENABLE_DELAY);
			}
			data->hj_enabled_prev = hj_now;
		}
		/* A CCC from the controller (e.g. ENEC/DISEC to enable/disable
		 * IBI) leaves the core in STATE_TAGT_IDLE. Restore the resting RX
		 * arm so a following controller private write is accepted instead
		 * of dropped ("Target RXNE ignored"); mirrors the re-arm done on
		 * the other target completion events.
		 */
		if (data->target_config != NULL) {
			i3c_realtek_arm_rx(data);
		}
		k_sem_give(&data->ccc_end);
		break;
	case RTK_I3C_EVENT_IBI_WRITE_COMPLETE:
		/* Target-raised IBI/HJ/CR was accepted by the controller. The IBI
		 * preempted the resting RX arm, so restore it for controller writes.
		 */
		data->num_xfer = args->count;
		data->ibi_status = 0;
		if (data->target_config != NULL) {
			i3c_realtek_arm_rx(data);
		}
		k_sem_give(&data->ibi_sem);
		break;
	case RTK_I3C_EVENT_IBI_READ_COMPLETE:
		/* Controller received an IBI from a target. */
		data->num_xfer = args->count;
#if defined(CONFIG_I3C_USE_IBI) && defined(CONFIG_I3C_IBI_WORKQUEUE)
		if (args->ibi_type == RTK_I3C_IBI_INTR) {
			struct i3c_device_desc *ibi_target =
				i3c_dev_list_i3c_addr_find(dev, args->ibi_addr);

			if (ibi_target != NULL && ibi_target->ibi_cb != NULL) {
				/* Payload includes the MDB as byte 0. */
				(void)i3c_ibi_work_enqueue_target_irq(ibi_target, data->ibi_rx_buf,
								      args->count);
			}
			/* Re-arm the staging buffer for the next IBI. */
			i3c_realtek_arm_ibi_rx(data);
		}
#endif
		k_sem_give(&data->xfer_end);
		break;
	case RTK_I3C_EVENT_ARBITRATE_FAIL:
		/* Target lost arbitration during an IBI/HJ/CR; the core already reset
		 * to idle. Wake the (synchronous) ibi_raise waiter so it fails fast
		 * instead of waiting out the timeout, and restore the resting RX arm.
		 */
		data->ibi_status = -EAGAIN;
		if (data->target_config != NULL) {
			i3c_realtek_arm_rx(data);
		}
		k_sem_give(&data->ibi_sem);
		break;
	default:
		break;
	}
}

/*
 * Determine the bus mode from the legacy I2C devices declared on this instance,
 * inspect every I2C device's LVR and
 * pick the most restrictive mode. A bus with no I2C devices stays PURE I3C.
 * This is what selects legacy-I2C-compatible bus timing, per instance.
 */
static enum i3c_bus_mode i3c_realtek_bus_mode(const struct i3c_dev_list *dev_list)
{
	enum i3c_bus_mode mode = I3C_BUS_MODE_PURE;

	for (int i = 0; i < dev_list->num_i2c; i++) {
		switch (I3C_LVR_I2C_DEV_IDX(dev_list->i2c[i].lvr)) {
		case I3C_LVR_I2C_DEV_IDX_0:
			if (mode < I3C_BUS_MODE_MIXED_FAST) {
				mode = I3C_BUS_MODE_MIXED_FAST;
			}
			break;
		case I3C_LVR_I2C_DEV_IDX_1:
			if (mode < I3C_BUS_MODE_MIXED_LIMITED) {
				mode = I3C_BUS_MODE_MIXED_LIMITED;
			}
			break;
		case I3C_LVR_I2C_DEV_IDX_2:
			if (mode < I3C_BUS_MODE_MIXED_SLOW) {
				mode = I3C_BUS_MODE_MIXED_SLOW;
			}
			break;
		default:
			mode = I3C_BUS_MODE_INVALID;
			break;
		}
	}

	return mode;
}

static int i3c_realtek_configure(const struct device *dev, enum i3c_config_type type,
				 void *bus_config)
{
	const struct i3c_realtek_config *config = dev->config;
	struct i3c_realtek_data *data = dev->data;
	struct i3c_config_controller *ctrl_cfg;
	struct i3c_config_target *target_cfg;
	int ret = 0;

	if (bus_config == NULL) {
		return -EINVAL;
	}

	k_mutex_lock(&data->bus_lock, K_FOREVER);

	switch (type) {
	case I3C_CONFIG_CONTROLLER:
		ctrl_cfg = bus_config;
		if (ctrl_cfg->is_secondary ||
		    (ctrl_cfg->supported_hdr != 0U && ctrl_cfg->supported_hdr != I3C_MSG_HDR_DDR)) {
			ret = -ENOTSUP;
			break;
		}

		data->rtk_cfg.common_cfg.role = RTK_I3C_CTRL_PRIM;
		data->rtk_cfg.bitrate_cfg.i3c_pp_baud_hz =
			ctrl_cfg->scl.i3c ? ctrl_cfg->scl.i3c : RTK_I3C_I3C_PP_BAUD_HZ;
		data->rtk_cfg.bitrate_cfg.i3c_od_baud_hz = config->i3c_od_scl_hz;
		/* Keep OD high/setup-hold timing in sync with the OD SCL rate. */
		data->rtk_cfg.common_cfg.timing.i3c_od_baud_hz = config->i3c_od_scl_hz;
		data->rtk_cfg.bitrate_cfg.i2c_baud_hz =
			ctrl_cfg->scl.i2c ? ctrl_cfg->scl.i2c : RTK_I3C_I2C_BAUD_HZ;

		/* Per-instance: use legacy-I2C-compatible timing only if this bus
		 * actually carries I2C devices (derived from the devicetree LVRs).
		 * The bus-free time then follows the OD SCL rate, matching I2C tBUF.
		 */
		data->rtk_cfg.common_cfg.timing.legacy_i2c =
			i3c_realtek_bus_mode(&config->common.dev_list) != I3C_BUS_MODE_PURE;
		if (data->rtk_cfg.common_cfg.timing.legacy_i2c) {
			uint32_t od = config->i3c_od_scl_hz;

			data->rtk_cfg.common_cfg.timing.bus_free_ns = (od <= 100000)    ? 4700
								      : (od <= 400000)  ? 1300
								      : (od <= 1000000) ? 500
											: 1300;
		} else {
			data->rtk_cfg.common_cfg.timing.bus_free_ns = 1300;
		}
		ret = i3c_realtek_err_to_errno(rtk_i3c_ctrl_init(&data->rtk_ctx, &data->rtk_cfg));
		break;
	case I3C_CONFIG_TARGET:
		target_cfg = bus_config;
		if (!target_cfg->enabled) {
			ret = -ENOTSUP;
			break;
		}

		data->rtk_cfg.common_cfg.role = RTK_I3C_TAGT;
		data->rtk_cfg.tagt_info.dyn_addr = 0U;
		data->rtk_cfg.tagt_info.stc_addr = target_cfg->static_addr;
		data->rtk_cfg.tagt_info.char_info.pid = target_cfg->pid;
		data->rtk_cfg.tagt_info.char_info.bcr = target_cfg->bcr;
		data->rtk_cfg.tagt_info.char_info.dcr = target_cfg->dcr;
		data->rtk_cfg.tagt_info.resp_info.max_read_len = target_cfg->max_read_len;
		data->rtk_cfg.tagt_info.resp_info.max_write_len = target_cfg->max_write_len;
		/* IPLS (EC[31:24]) is the IBI payload size this target reports via
		 * GETMRL when BCR[2]==1. It is a capacity ceiling, not a per-IBI
		 * length (the actual length is T-bit terminated at runtime), so
		 * advertise the driver's IBI buffer size. resp_info.ibi_payload_len
		 * defaults to 0, and set_resp_info writes it into IPLS, clobbering the
		 * HW reset default (0x10); left at 0 the controller reads no payload
		 * and even the mandatory MDB never reaches the bus.
		 */
		data->rtk_cfg.tagt_info.resp_info.ibi_payload_len =
			(target_cfg->bcr & I3C_BCR_IBI_PAYLOAD) ? CONFIG_I3C_IBI_MAX_PAYLOAD_SIZE : 0U;
		data->rtk_cfg.tagt_info.resp_info.hdr_mode = target_cfg->supported_hdr;
		ret = i3c_realtek_err_to_errno(rtk_i3c_tagt_init(&data->rtk_ctx, &data->rtk_cfg));
		if (ret == 0) {
			/* tagt_init zeroes the context, so arm RX afterwards. */
			i3c_realtek_arm_rx(data);
		}
		break;
	default:
		ret = -ENOTSUP;
		break;
	}

	k_mutex_unlock(&data->bus_lock);

	return ret;
}

static int i3c_realtek_config_get(const struct device *dev, enum i3c_config_type type,
				  void *bus_config)
{
	struct i3c_realtek_data *data = dev->data;

	if (bus_config == NULL) {
		return -EINVAL;
	}

	switch (type) {
	case I3C_CONFIG_CONTROLLER:
		*(struct i3c_config_controller *)bus_config = data->common.ctrl_config;
		return 0;
	case I3C_CONFIG_TARGET: {
		struct i3c_config_target *target_cfg = bus_config;

		target_cfg->enabled = true;
		target_cfg->static_addr = data->rtk_cfg.tagt_info.stc_addr;
		target_cfg->pid = data->rtk_cfg.tagt_info.char_info.pid;
		target_cfg->bcr = data->rtk_cfg.tagt_info.char_info.bcr;
		target_cfg->dcr = data->rtk_cfg.tagt_info.char_info.dcr;
		target_cfg->max_read_len = data->rtk_cfg.tagt_info.resp_info.max_read_len;
		target_cfg->max_write_len = data->rtk_cfg.tagt_info.resp_info.max_write_len;
		target_cfg->supported_hdr = data->rtk_cfg.tagt_info.resp_info.hdr_mode;
		return 0;
	}
	default:
		return -ENOTSUP;
	}
}

static int i3c_realtek_do_daa(const struct device *dev)
{
	struct i3c_realtek_data *data = dev->data;
	int ret;

	k_mutex_lock(&data->bus_lock, K_FOREVER);
	ret = i3c_realtek_err_to_errno(rtk_i3c_do_daa(&data->rtk_ctx));
	k_mutex_unlock(&data->bus_lock);

#if defined(CONFIG_I3C_USE_IBI)
	/*
	 * Re-enable Hot Join on the bus after enumeration, Done outside bus_lock as it issues a
	 * CCC. Best-effort.
	 */
	if (ret == 0) {
		struct i3c_ccc_events hj_evt = {
			.events = I3C_CCC_EVT_HJ,
		};

		(void)i3c_ccc_do_events_all_set(dev, true, &hj_evt);
	}
#endif /* CONFIG_I3C_USE_IBI */

	return ret;
}

static int i3c_realtek_recover_bus(const struct device *dev)
{
	struct i3c_realtek_data *data = dev->data;
	unsigned int key;
	int ret;

	/* Hold the bus lock against concurrent transfers, and briefly lock IRQs
	 * because the recovery resets transfer state the ISR also touches.
	 */
	k_mutex_lock(&data->bus_lock, K_FOREVER);
	key = irq_lock();
	ret = i3c_realtek_err_to_errno(rtk_i3c_ctrl_recover(&data->rtk_ctx));
	irq_unlock(key);
	k_mutex_unlock(&data->bus_lock);

	return ret;
}

static int i3c_realtek_do_ccc(const struct device *dev, struct i3c_ccc_payload *payload)
{
	struct i3c_realtek_data *data = dev->data;
	rtk_i3c_ccc ccc = {0};
	rtk_i3c_msg ccc_msg = {0};
	rtk_i3c_tagt tagts[I3C_REALTEK_MAX_DEVS];
	rtk_i3c_msg msgs[I3C_REALTEK_MAX_DEVS];
	int ret;

	if (payload == NULL || payload->targets.num_targets > I3C_REALTEK_MAX_DEVS) {
		return -EINVAL;
	}

	ccc.id = payload->ccc.id;
	if (payload->ccc.data_len != 0U) {
		ccc_msg.data = payload->ccc.data;
		ccc_msg.len = payload->ccc.data_len;
		ccc_msg.flags = RTK_I3C_WRITE;
		ccc.msg = &ccc_msg;
	}

	ccc.num_tagts = payload->targets.num_targets;
	if (ccc.num_tagts != 0U) {
		ccc.tagt = tagts;
		for (size_t i = 0; i < ccc.num_tagts; i++) {
			msgs[i].data = payload->targets.payloads[i].data;
			msgs[i].len = payload->targets.payloads[i].data_len;
			msgs[i].count = 0U;
			msgs[i].flags =
				payload->targets.payloads[i].rnw ? RTK_I3C_READ : RTK_I3C_WRITE;
			tagts[i].addr = payload->targets.payloads[i].addr;
			tagts[i].msg = &msgs[i];
		}
	}

	k_mutex_lock(&data->bus_lock, K_FOREVER);
	ret = i3c_realtek_err_to_errno(
		rtk_i3c_do_ccc(&data->rtk_ctx, &ccc, RTK_I3C_SDR_MODE, false));
	k_mutex_unlock(&data->bus_lock);

	payload->ccc.num_xfer = ccc_msg.count;
	for (size_t i = 0; i < ccc.num_tagts; i++) {
		payload->targets.payloads[i].num_xfer = msgs[i].count;
	}

	return ret;
}

static int i3c_realtek_i3c_xfers(const struct device *dev, struct i3c_device_desc *target,
				 struct i3c_msg *msgs, uint8_t num_msgs)
{
	struct i3c_realtek_data *data = dev->data;
	int ret = 0;

	if (target == NULL || msgs == NULL) {
		return -EINVAL;
	}

	k_mutex_lock(&data->bus_lock, K_FOREVER);

	for (uint8_t i = 0; i < num_msgs; i++) {
		rtk_i3c_msg msg = {
			.data = msgs[i].buf,
			.len = msgs[i].len,
			.count = 0U,
			.flags = (msgs[i].flags & I3C_MSG_READ) ? RTK_I3C_READ : RTK_I3C_WRITE,
		};
		rtk_i3c_tagt tagt = {
			.addr = target->dynamic_addr,
			.msg = &msg,
		};
		rtk_i3c_mode mode = RTK_I3C_SDR_MODE;
		bool restart = ((msgs[i].flags & I3C_MSG_STOP) == 0U);

		if (msgs[i].buf == NULL || target->dynamic_addr == 0U) {
			ret = -EINVAL;
			break;
		}

		if ((msgs[i].flags & I3C_MSG_HDR) != 0U) {
			if (msgs[i].hdr_mode != I3C_MSG_HDR_DDR) {
				ret = -ENOTSUP;
				break;
			}
			mode = RTK_I3C_HDR_DDR;
		}

		ret = i3c_realtek_err_to_errno(
			rtk_i3c_ctrl_xfer(&data->rtk_ctx, &tagt, mode, restart));
		msgs[i].num_xfer = msg.count;
		if (ret != 0) {
			break;
		}
	}

	k_mutex_unlock(&data->bus_lock);

	return ret;
}

static struct i3c_device_desc *i3c_realtek_device_find(const struct device *dev,
						       const struct i3c_device_id *id)
{
	const struct i3c_realtek_config *config = dev->config;

	return i3c_dev_list_find(&config->common.dev_list, id);
}

#ifdef CONFIG_I3C_USE_IBI
/*
 * Issue one IBI/HJ/CR attempt and wait for its outcome. Returns 0 when accepted,
 * -EAGAIN when rejected (NACKed) or arbitration was lost (retryable), -ETIMEDOUT
 * when nothing responded, or a negative errno from the core. Caller holds bus_lock.
 */
static int i3c_realtek_ibi_raise_once(struct i3c_realtek_data *data, rtk_i3c_ibi_type ibi_type,
				      rtk_i3c_msg *msg_ptr)
{
	unsigned int key;
	int ret;

	k_sem_reset(&data->ibi_sem);
	/* Atomic vs the completion ISR that arms RX (see i3c_realtek_arm_rx). Only
	 * the state-mutating ibi_write is guarded; the wait below must not run with
	 * interrupts locked.
	 */
	key = irq_lock();
	ret = i3c_realtek_err_to_errno(rtk_i3c_ibi_write(&data->rtk_ctx, ibi_type, msg_ptr));
	irq_unlock(key);
	if (ret != 0) {
		return ret;
	}

	/* Wait for the outcome: RTK_I3C_EVENT_IBI_WRITE_COMPLETE (accepted, ibi_status
	 * 0) or RTK_I3C_EVENT_ARBITRATE_FAIL (rejected/lost arbitration, ibi_status
	 * -EAGAIN). A timeout means neither fired, e.g. no active controller.
	 */
	if (k_sem_take(&data->ibi_sem, I3C_REALTEK_IBI_TIMEOUT) != 0) {
		return -ETIMEDOUT;
	}

	return data->ibi_status;
}

static int i3c_realtek_ibi_raise(const struct device *dev, struct i3c_ibi *request)
{
	struct i3c_realtek_data *data = dev->data;
	rtk_i3c_msg msg = {0};
	rtk_i3c_msg *msg_ptr = NULL;
	rtk_i3c_ibi_type ibi_type;
	int ret;

	if (request == NULL) {
		return -EINVAL;
	}

	switch (request->ibi_type) {
	case I3C_IBI_TARGET_INTR:
		ibi_type = RTK_I3C_IBI_INTR;
		break;
	case I3C_IBI_CONTROLLER_ROLE_REQUEST:
		ibi_type = RTK_I3C_IBI_CTRL_REQ;
		break;
	case I3C_IBI_HOTJOIN:
		ibi_type = RTK_I3C_IBI_HOT_JOIN;
		break;
	default:
		return -EINVAL;
	}

	/* INTR and controller-role-request need an assigned dynamic address;
	 * hot-join is raised precisely because there is none yet.
	 */
	if (ibi_type != RTK_I3C_IBI_HOT_JOIN && data->rtk_cfg.tagt_info.dyn_addr == 0U) {
		return -EINVAL;
	}

	if (request->payload_len != 0U) {
		msg.data = request->payload;
		msg.len = request->payload_len;
		msg.flags = RTK_I3C_WRITE;
		msg_ptr = &msg;
	}

	/* Hot Join uses a more persistent retry policy than a regular IBI. */
	bool is_hj = (ibi_type == RTK_I3C_IBI_HOT_JOIN);
	int retry_max = is_hj ? I3C_REALTEK_HJ_RETRY_MAX : I3C_REALTEK_IBI_RETRY_MAX;
	k_timeout_t backoff = is_hj ? I3C_REALTEK_HJ_RETRY_BACKOFF : I3C_REALTEK_IBI_RETRY_BACKOFF;

	k_mutex_lock(&data->bus_lock, K_FOREVER);

	/* For hot join, an accepted request is signalled by DAA completion rather
	 * than IBI_WRITE_COMPLETE (see the ADDRESS_ASSIGNMENT_COMPLETE handler).
	 */
	data->hj_pending = is_hj;
	if (is_hj) {
		/* Remember we still want to join; used to auto-retry HJ if the
		 * controller re-enables Hot-Join (ENEC) without assigning an address.
		 */
		data->hj_wanted = true;
	}

	for (int attempt = 0;; attempt++) {
		ret = i3c_realtek_ibi_raise_once(data, ibi_type, msg_ptr);
		/* Retry only on transient rejection / lost arbitration, not on a
		 * timeout (no active controller) or a parameter/state error.
		 */
		if (ret != -EAGAIN || attempt >= retry_max) {
			break;
		}
		k_sleep(backoff);
	}

	data->hj_pending = false;

	k_mutex_unlock(&data->bus_lock);

	return ret;
}

/*
 * Deferred Hot-Join retry. Per the I3C spec a target that received DISEC(DISHJ)
 * and still needs a dynamic address may re-send its Hot-Join once ENEC(ENHJ)
 * re-enables it. The re-enable is detected in the CCC callback (ISR context),
 * which cannot call i3c_ibi_raise() directly (it blocks / takes the bus lock),
 * so the actual re-raise happens here on the system workqueue.
 */
static void i3c_realtek_hj_retry_work(struct k_work *work)
{
	struct k_work_delayable *dwork = k_work_delayable_from_work(work);
	struct i3c_realtek_data *data = CONTAINER_OF(dwork, struct i3c_realtek_data, hj_retry_work);
	struct i3c_ibi req = {
		.ibi_type = I3C_IBI_HOTJOIN,
	};

	/* Skip if an address was assigned between the ENEC and this work running. */
	if (data->rtk_cfg.tagt_info.dyn_addr != 0U || !data->hj_wanted) {
		return;
	}

	(void)i3c_realtek_ibi_raise(data->dev, &req);
}

/*
 * Controller side: enable receiving IBI from a target. Arms the local IBI RX
 * buffer, then tells the target to enable IBI generation via ENEC.
 */
static int i3c_realtek_ibi_enable(const struct device *dev, struct i3c_device_desc *target)
{
	struct i3c_realtek_data *data = dev->data;
	struct i3c_ccc_events events;
	int ret;

	if (target == NULL || !i3c_device_is_ibi_capable(target)) {
		return -EINVAL;
	}
	if (target->dynamic_addr == 0U) {
		return -EINVAL;
	}

	/* Program the HW per-target "IBI carries a payload" bit for this address.
	 * The ENTDAA path (handle_daa_phase / rtk_i3c_bus_handle_daa) does this
	 * during address arbitration, but a target that got its address via
	 * SETDASA (e.g. a static-addr device) never goes through that phase, so
	 * the bit is left clear and a payload-carrying IBI (BCR bit2=1) stalls
	 * after the address ACK — the core never clocks the MDB in. Set it here
	 * (idempotent) so IBI payload works regardless of how the address was
	 * assigned.
	 */
	rtk_i3c_set_ibi_mdb(&data->rtk_ctx, target->dynamic_addr,
			    (target->bcr & I3C_BCR_IBI_PAYLOAD) != 0);

	/* Sync this target's BCR into the bus-layer target table. The IBI receive
	 * ISR (rtk_i3c_bus_get_bcr_by_addr) reads BCR bit2 from this table to decide
	 * whether to assemble the IBI payload (incl. the MDB). The ENTDAA path fills
	 * char_info there; a SETDASA target does not, so BCR stays 0 and a
	 * payload-carrying IBI is silently dropped (payload_len 0). Fill it here.
	 */
	for (uint8_t i = 0; i < I3C_REALTEK_MAX_DEVS; i++) {
		if (data->tagt_table[i].info.char_info.pid == target->pid) {
			data->tagt_table[i].info.char_info.bcr = target->bcr;
			data->tagt_table[i].info.dyn_addr = target->dynamic_addr;
			data->tagt_table[i].active = true;
			break;
		}
	}

	/* Arm the RX buffer before enabling IBI at the target so the payload of an
	 * IBI that arrives immediately after ENEC is captured. arm_ibi_rx locks
	 * interrupts internally; the ENEC transfer below must not be under bus_lock
	 * here because i3c_ccc_do_events_set() takes bus_lock itself.
	 */
	i3c_realtek_arm_ibi_rx(data);

	events.events = I3C_CCC_EVT_INTR;
	ret = i3c_ccc_do_events_set(target, true, &events);
	if (ret != 0) {
		LOG_ERR("%s: ENEC(INTR) for 0x%02x failed (%d)", dev->name, target->dynamic_addr,
			ret);
	}

	return ret;
}

/* Controller side: disable receiving IBI from a target via DISEC. */
static int i3c_realtek_ibi_disable(const struct device *dev, struct i3c_device_desc *target)
{
	struct i3c_ccc_events events;
	int ret;

	if (target == NULL || target->dynamic_addr == 0U) {
		return -EINVAL;
	}

	events.events = I3C_CCC_EVT_INTR;
	ret = i3c_ccc_do_events_set(target, false, &events);
	if (ret != 0) {
		LOG_ERR("%s: DISEC(INTR) for 0x%02x failed (%d)", dev->name, target->dynamic_addr,
			ret);
	}

	return ret;
}
#endif /* CONFIG_I3C_USE_IBI */

static int i3c_realtek_target_register(const struct device *dev, struct i3c_target_config *cfg)
{
	struct i3c_realtek_data *data = dev->data;

	if (cfg == NULL) {
		return -EINVAL;
	}

	data->target_config = cfg;
	return 0;
}

static int i3c_realtek_target_unregister(const struct device *dev, struct i3c_target_config *cfg)
{
	struct i3c_realtek_data *data = dev->data;

	if (data->target_config != cfg) {
		return -EINVAL;
	}

	data->target_config = NULL;
	return 0;
}

static int i3c_realtek_target_tx_write(const struct device *dev, uint8_t *buf, uint16_t len,
				       uint8_t hdr_mode)
{
	struct i3c_realtek_data *data = dev->data;
	rtk_i3c_msg msg = {
		.data = buf,
		.len = len,
		.count = 0U,
		.flags = RTK_I3C_WRITE,
	};
	int ret;
	unsigned int key;

	if (buf == NULL || len == 0U) {
		return -EINVAL;
	}

	/* Target TX preload is SDR only; HDR read-back is not supported. */
	if (hdr_mode != 0U) {
		return -ENOTSUP;
	}

	k_mutex_lock(&data->bus_lock, K_FOREVER);
	/* Atomic vs the completion ISR that arms RX (see i3c_realtek_arm_rx). */
	key = irq_lock();
	ret = i3c_realtek_err_to_errno(rtk_i3c_tagt_xfer(&data->rtk_ctx, &msg));
	irq_unlock(key);
	k_mutex_unlock(&data->bus_lock);

	return ret == 0 ? (int)msg.count : ret;
}

static int i3c_realtek_init(const struct device *dev)
{
	const struct i3c_realtek_config *config = dev->config;
	struct i3c_realtek_data *data = dev->data;
	int ret;

	k_mutex_init(&data->bus_lock);
	k_sem_init(&data->ccc_end, 0, 1);
	k_sem_init(&data->xfer_end, 0, 1);
	k_sem_init(&data->ibi_sem, 0, 1);
	k_work_init_delayable(&data->hj_retry_work, i3c_realtek_hj_retry_work);
	data->dev = dev;

	i3c_realtek_devices[config->instance_id] = dev;

	rtk_i3c_get_config(NULL, &data->rtk_cfg);
	/* Seed the HJ-enable snapshot from the configured default so the first
	 * genuine DISEC->ENEC transition (not the boot state) triggers a retry.
	 */
	data->hj_enabled_prev = data->rtk_cfg.common_cfg.ibi.enable_hj;
	data->rtk_cfg.common_cfg.instance_id = config->instance_id;
	data->rtk_cfg.common_cfg.i3c_freq_hz = config->i3c_freq_hz;
	data->rtk_cfg.common_cfg.role = config->role;
	data->rtk_cfg.common_cfg.callback = i3c_realtek_hal_callback;
	data->rtk_cfg.common_cfg.ctx = dev;
	data->rtk_cfg.tagt_table = data->tagt_table;
	data->rtk_cfg.addr_slot = data->addr_slots;

	ret = pinctrl_apply_state(config->pincfg, PINCTRL_STATE_DEFAULT);
	if (ret != 0) {
		return ret;
	}

	if (!device_is_ready(config->clock_dev)) {
		return -ENODEV;
	}

	ret = clock_control_on(config->clock_dev, (clock_control_subsys_t)&config->sccon_cfg);
	if (ret != 0) {
		return ret;
	}

	/*
	 * The RTS5918 SCCON clock driver currently enables the PLL source and
	 * I3C IP clock. Keep reset handling local to the I3C driver to avoid
	 * resetting an already-initialized paired controller/target instance
	 * while the other I3C instances are still probing.
	 */
	config->irq_config_func(dev);

	if (config->role == RTK_I3C_CTRL_PRIM) {
		/*
		 * Defer Hot-Join acceptance until after bus init:
		 * a HJ during the initial RSTDAA/ENTDAA would interfere. Configure
		 * with HJ disabled, then re-enable it once the bus is up.
		 */
		bool want_hj = data->rtk_cfg.common_cfg.ibi.enable_hj;

		data->rtk_cfg.common_cfg.ibi.enable_hj = false;

		data->common.ctrl_config.scl.i3c = data->common.ctrl_config.scl.i3c
							   ? data->common.ctrl_config.scl.i3c
							   : RTK_I3C_I3C_PP_BAUD_HZ;
		data->common.ctrl_config.scl.i2c = data->common.ctrl_config.scl.i2c
							   ? data->common.ctrl_config.scl.i2c
							   : RTK_I3C_I2C_BAUD_HZ;

		ret = i3c_configure(dev, I3C_CONFIG_CONTROLLER, &data->common.ctrl_config);
		if (ret != 0) {
			return ret;
		}

		ret = i3c_realtek_address_slots_init(dev);
		if (ret != 0) {
			return ret;
		}

		/*
		 * Bring up the bus
		 * RSTACT/RSTDAA, DISEC, SETDASA/
		 * ENTDAA, then per device fetch BCR/DCR/MRL/MWL/CAPS via
		 * i3c_device_basic_info_get(), and finally ENEC(HJ). Primary
		 * controller only. Not fatal on failure (e.g. an empty bus at boot):
		 * any device that did get an address still works.
		 */
		ret = i3c_bus_init(dev, &config->common.dev_list);
		if (ret != 0) {
			LOG_WRN("%s: i3c_bus_init failed (%d); continuing", dev->name, ret);
			ret = 0;
		}

		/* Bus is up: now accept Hot Joins (deferred above). */
		if (want_hj) {
			data->rtk_cfg.common_cfg.ibi.enable_hj = true;
			rtk_i3c_set_hj_accept(&data->rtk_ctx, true);
		}
	} else if (config->role == RTK_I3C_TAGT) {
		struct i3c_config_target target_cfg = {
			.enabled = true,
			.static_addr = config->static_addr,
			.pid = config->pid,
			.bcr = config->bcr,
			.dcr = config->dcr,
			.max_read_len = config->max_read_len,
			.max_write_len = config->max_write_len,
		};

		ret = i3c_configure(dev, I3C_CONFIG_TARGET, &target_cfg);
		if (ret != 0) {
			return ret;
		}
	}

	return 0;
}

static const struct i3c_driver_api i3c_realtek_api = {
	.i2c_api.configure = i3c_realtek_i2c_configure,
	.i2c_api.transfer = i3c_realtek_i2c_transfer,

	.configure = i3c_realtek_configure,
	.config_get = i3c_realtek_config_get,
	.attach_i3c_device = i3c_realtek_attach_i3c_device,
	.reattach_i3c_device = i3c_realtek_reattach_i3c_device,
	.detach_i3c_device = i3c_realtek_detach_i3c_device,
	.attach_i2c_device = i3c_realtek_attach_i2c_device,
	.detach_i2c_device = i3c_realtek_detach_i2c_device,
	.do_daa = i3c_realtek_do_daa,
	.recover_bus = i3c_realtek_recover_bus,
	.do_ccc = i3c_realtek_do_ccc,
	.i3c_xfers = i3c_realtek_i3c_xfers,
	.i3c_device_find = i3c_realtek_device_find,
#ifdef CONFIG_I3C_USE_IBI
	.ibi_enable = i3c_realtek_ibi_enable,
	.ibi_disable = i3c_realtek_ibi_disable,
	.ibi_raise = i3c_realtek_ibi_raise,
#endif
	.target_register = i3c_realtek_target_register,
	.target_unregister = i3c_realtek_target_unregister,
	.target_tx_write = i3c_realtek_target_tx_write,
};

#define I3C_REALTEK_ROLE(n)                                                                        \
	COND_CODE_1(DT_INST_PROP(n, target_mode), (RTK_I3C_TAGT), (RTK_I3C_CTRL_PRIM))

/*
 * Materialise a struct device for each driverless RTK I3C target child node so
 * the i3c_device_desc's .dev = DEVICE_DT_GET(child) resolves at link time. These
 * placeholder targets have no child-side driver, so no init is needed; the
 * device model accepts a NULL init function.
 *
 * Restrict this to realtek,rts5918-i3c-target children: a child that binds its
 * own driver (e.g. zephyr,mctp-i3c-endpoint) already defines its device, so
 * defining one here too would be a duplicate device-object definition.
 */
#define I3C_REALTEK_TARGET_DEVICE_DEFINE(node_id)                                                  \
	IF_ENABLED(DT_NODE_HAS_COMPAT(node_id, realtek_rts5918_i3c_target),                        \
		   (I3C_DEVICE_DT_DEFINE(node_id, NULL, NULL, NULL, NULL, POST_KERNEL,             \
					 CONFIG_I3C_CONTROLLER_INIT_PRIORITY, NULL);))

#define I3C_REALTEK_INIT(n)                                                                        \
	static void i3c_realtek_irq_config_##n(const struct device *dev);                          \
	PINCTRL_DT_INST_DEFINE(n);                                                                 \
	DT_INST_FOREACH_CHILD_STATUS_OKAY(n, I3C_REALTEK_TARGET_DEVICE_DEFINE)                     \
	static struct i3c_device_desc i3c_realtek_i3c_dev_list_##n[] =                             \
		I3C_DEVICE_ARRAY_DT_INST(n);                                                       \
	static struct i3c_i2c_device_desc i3c_realtek_i2c_dev_list_##n[] =                         \
		I3C_I2C_DEVICE_ARRAY_DT_INST(n);                                                   \
	static struct i3c_realtek_data i3c_realtek_data_##n = {                                    \
		.common.ctrl_config.scl.i3c =                                                      \
			DT_INST_PROP_OR(n, i3c_scl_hz, RTK_I3C_I3C_PP_BAUD_HZ),                    \
		.common.ctrl_config.scl.i2c = DT_INST_PROP_OR(n, i2c_scl_hz, RTK_I3C_I2C_BAUD_HZ), \
	};                                                                                         \
	static const struct i3c_realtek_config i3c_realtek_config_##n = {                          \
		.base = DT_INST_REG_ADDR(n),                                                       \
		.pincfg = PINCTRL_DT_INST_DEV_CONFIG_GET(n),                                       \
		.clock_dev = DEVICE_DT_GET(DT_INST_CLOCKS_CTLR(n)),                                \
		.sccon_cfg =                                                                       \
			{                                                                          \
				.clk_grp = DT_INST_PHA(n, clocks, clk_grp),                        \
				.clk_idx = DT_INST_PHA(n, clocks, clk_idx),                        \
			},                                                                         \
		.instance_id = n,                                                                  \
		.i3c_freq_hz = DT_INST_PROP_OR(n, clock_frequency, RTK_I3C_FREQ_HZ),               \
		.i3c_od_scl_hz = DT_INST_PROP_OR(n, i3c_od_scl_hz, RTK_I3C_I3C_OD_BAUD_HZ),        \
		.role = I3C_REALTEK_ROLE(n),                                                       \
		.static_addr = DT_INST_PROP_OR(n, static_addr, 0),                                 \
		.dynamic_addr = DT_INST_PROP_OR(n, primary_controller_da, 0),                      \
		.pid = ((uint64_t)DT_INST_PROP_OR(n, pid_msb, 0) << 32) |                          \
		       DT_INST_PROP_OR(n, pid_lsb, 0),                                             \
		.bcr = DT_INST_PROP_OR(n, bcr, 0),                                                 \
		.dcr = DT_INST_PROP_OR(n, dcr, 0),                                                 \
		.max_read_len = DT_INST_PROP_OR(n, max_read_len, RTK_I3C_FIFO_DEPTH),              \
		.max_write_len = DT_INST_PROP_OR(n, max_write_len, RTK_I3C_FIFO_DEPTH),            \
		.irq_config_func = i3c_realtek_irq_config_##n,                                     \
		.common.dev_list.i3c = i3c_realtek_i3c_dev_list_##n,                               \
		.common.dev_list.num_i3c = ARRAY_SIZE(i3c_realtek_i3c_dev_list_##n),               \
		.common.dev_list.i2c = i3c_realtek_i2c_dev_list_##n,                               \
		.common.dev_list.num_i2c = ARRAY_SIZE(i3c_realtek_i2c_dev_list_##n),               \
	};                                                                                         \
	DEVICE_DT_INST_DEFINE(n, i3c_realtek_init, NULL, &i3c_realtek_data_##n,                    \
			      &i3c_realtek_config_##n, POST_KERNEL,                                \
			      CONFIG_I3C_CONTROLLER_INIT_PRIORITY, &i3c_realtek_api);              \
	static void i3c_realtek_irq_config_##n(const struct device *dev)                           \
	{                                                                                          \
		IRQ_CONNECT(DT_INST_IRQN(n), DT_INST_IRQ(n, priority), i3c_realtek_isr,            \
			    DEVICE_DT_INST_GET(n), 0);                                             \
		irq_enable(DT_INST_IRQN(n));                                                       \
	}

DT_INST_FOREACH_STATUS_OKAY(I3C_REALTEK_INIT)
