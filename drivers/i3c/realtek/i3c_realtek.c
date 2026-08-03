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

#define I3C_REALTEK_ADDR_SLOT_BITS  32U
#define I3C_REALTEK_ADDR_SLOT_WORDS ROUND_UP(RTK_I3C_MAX_DYN_ADDR + 1U, I3C_REALTEK_ADDR_SLOT_BITS)
#define I3C_REALTEK_MAX_DEVS        RTK_I3C_MAX_TAGT_COUNT

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
	uint32_t num_xfer;
	rtk_i3c_ctx rtk_ctx;
	rtk_i3c_cfg rtk_cfg;
	rtk_i3c_bus_tagt_item tagt_table[I3C_REALTEK_MAX_DEVS];
	uint32_t addr_slots[I3C_REALTEK_ADDR_SLOT_WORDS];
	struct i3c_target_config *target_config;
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

static int i3c_realtek_attach_i3c_device(const struct device *dev, struct i3c_device_desc *target,
					 uint8_t addr)
{
	struct i3c_realtek_data *data = dev->data;
	const char *name = target->dev ? target->dev->name : "i3c";

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
		}
		break;
	case RTK_I3C_EVENT_READ_COMPLETE:
	case RTK_I3C_EVENT_WRITE_COMPLETE:
		data->num_xfer = args->count;
		k_sem_give(&data->xfer_end);
		if (data->target_config != NULL && data->target_config->callbacks != NULL) {
			target_cb = data->target_config->callbacks;
			if (target_cb->stop_cb != NULL) {
				target_cb->stop_cb(data->target_config);
			}
		}
		break;
	case RTK_I3C_EVENT_COMMAND_COMPLETE:
		data->num_xfer = args->count;
		k_sem_give(&data->ccc_end);
		break;
	case RTK_I3C_EVENT_IBI_WRITE_COMPLETE:
	case RTK_I3C_EVENT_IBI_READ_COMPLETE:
		data->num_xfer = args->count;
		k_sem_give(&data->xfer_end);
		break;
	default:
		break;
	}
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
		data->rtk_cfg.bitrate_cfg.i2c_baud_hz =
			ctrl_cfg->scl.i2c ? ctrl_cfg->scl.i2c : RTK_I3C_I2C_BAUD_HZ;
		ret = i3c_realtek_err_to_errno(rtk_i3c_ctrl_init(&data->rtk_ctx, &data->rtk_cfg));
		break;
	case I3C_CONFIG_TARGET:
		target_cfg = bus_config;
		if (!target_cfg->enable) {
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
		data->rtk_cfg.tagt_info.resp_info.hdr_mode = target_cfg->supported_hdr;
		ret = i3c_realtek_err_to_errno(rtk_i3c_tagt_init(&data->rtk_ctx, &data->rtk_cfg));
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

		target_cfg->enable = true;
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

static int i3c_realtek_ibi_raise(const struct device *dev, struct i3c_ibi *request)
{
	struct i3c_realtek_data *data = dev->data;
	rtk_i3c_msg msg = {0};
	rtk_i3c_msg *msg_ptr = NULL;
	rtk_i3c_ibi_type ibi_type = RTK_I3C_IBI_INTR;
	int ret;

	if (request != NULL) {
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

		if (request->payload_len != 0U) {
			msg.data = request->payload;
			msg.len = request->payload_len;
			msg.flags = RTK_I3C_WRITE;
			msg_ptr = &msg;
		}
	}

	k_mutex_lock(&data->bus_lock, K_FOREVER);
	ret = i3c_realtek_err_to_errno(rtk_i3c_ibi_write(&data->rtk_ctx, ibi_type, msg_ptr));
	k_mutex_unlock(&data->bus_lock);

	return ret;
}

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

static int i3c_realtek_target_tx_write(const struct device *dev, uint8_t *buf, uint16_t len)
{
	struct i3c_realtek_data *data = dev->data;
	rtk_i3c_msg msg = {
		.data = buf,
		.len = len,
		.count = 0U,
		.flags = RTK_I3C_WRITE,
	};
	int ret;

	if (buf == NULL || len == 0U) {
		return -EINVAL;
	}

	k_mutex_lock(&data->bus_lock, K_FOREVER);
	ret = i3c_realtek_err_to_errno(rtk_i3c_tagt_xfer(&data->rtk_ctx, &msg));
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

	i3c_realtek_devices[config->instance_id] = dev;

	rtk_i3c_get_config(NULL, &data->rtk_cfg);
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
		ret = i3c_realtek_address_slots_init(dev);
		if (ret != 0) {
			return ret;
		}

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
	} else if (config->role == RTK_I3C_TAGT) {
		struct i3c_config_target target_cfg = {
			.enable = true,
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
	.configure = i3c_realtek_configure,
	.config_get = i3c_realtek_config_get,
	.attach_i3c_device = i3c_realtek_attach_i3c_device,
	.reattach_i3c_device = i3c_realtek_reattach_i3c_device,
	.detach_i3c_device = i3c_realtek_detach_i3c_device,
	.do_daa = i3c_realtek_do_daa,
	.do_ccc = i3c_realtek_do_ccc,
	.i3c_xfers = i3c_realtek_i3c_xfers,
	.i3c_device_find = i3c_realtek_device_find,
	.ibi_raise = i3c_realtek_ibi_raise,
	.target_register = i3c_realtek_target_register,
	.target_unregister = i3c_realtek_target_unregister,
	.target_tx_write = i3c_realtek_target_tx_write,
};

#define I3C_REALTEK_ROLE(n)                                                                        \
	COND_CODE_1(DT_INST_PROP(n, target_mode), (RTK_I3C_TAGT), (RTK_I3C_CTRL_PRIM))

static int i3c_realtek_target_device_init(const struct device *dev)
{
	ARG_UNUSED(dev);
	return 0;
}

#define I3C_REALTEK_TARGET_DEVICE_DEFINE(node_id)                                                  \
	I3C_DEVICE_DT_DEFINE(node_id, i3c_realtek_target_device_init, NULL, NULL, NULL,            \
			     POST_KERNEL, CONFIG_I3C_CONTROLLER_INIT_PRIORITY, NULL);

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
