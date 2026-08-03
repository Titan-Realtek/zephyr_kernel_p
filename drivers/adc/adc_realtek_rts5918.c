/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT realtek_rts5918_adc

#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/clock_control_rts5918.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/sys/util.h>

#include "reg/reg_adc.h"

#define ADC_CONTEXT_USES_KERNEL_TIMER
#define ADC_CONTEXT_WAIT_FOR_COMPLETION_TIMEOUT K_MSEC(200)
#include "adc_context.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(adc_rts5918, CONFIG_ADC_LOG_LEVEL);

#define rts5918_ADC_MAX_CHAN        12
#define rts5918_ADC_POLLING_TIME_MS 1
#define rts5918_ADC_ENABLE_TIMEOUT  200000

struct adc_rts5918_config {
	volatile struct adc_regs *regs;
	const struct pinctrl_dev_config *pcfg;
#ifdef CONFIG_CLOCK_CONTROL
	const struct device *clk_dev;
	struct rts5918_sccon_subsys sccon_cfg;
#endif
};

struct adc_rts5918_data {
	struct adc_context ctx;
	const struct device *adc_dev;
	volatile uint16_t *buffer;
	volatile uint16_t *repeat_buffer;
	uint32_t channels;
};

static void adc_context_start_sampling(struct adc_context *ctx)
{
	struct adc_rts5918_data *data = CONTAINER_OF(ctx, struct adc_rts5918_data, ctx);
	const struct device *adc_dev = data->adc_dev;
	const struct adc_rts5918_config *const cfg = adc_dev->config;
	volatile struct adc_regs *regs = cfg->regs;

	data->repeat_buffer = data->buffer;

	LOG_DBG("starting sampling");
	/* set conversion mode to Single Mode */
	regs->CTRL_b.cfg_sar_cont = 0;
	/* Regular channels trigger enable */
	regs->REGULAR_b.cfg_sar_rswstart = 0;
    regs->REGULAR_b.cfg_sar_rexttrig = 1;
	/* Regular channels software trigger and reset by hardware */
    regs->REGULAR_b.cfg_sar_rswstart = 1;

	k_busy_wait(1000);
}

static void adc_context_update_buffer_pointer(struct adc_context *ctx, bool repeat_sampling)
{
	struct adc_rts5918_data *data = CONTAINER_OF(ctx, struct adc_rts5918_data, ctx);

	if (repeat_sampling) {
		data->buffer = data->repeat_buffer;
	}
}

static int adc_rts5918_channel_setup(const struct device *dev,
				     const struct adc_channel_cfg *channel_cfg)
{
	const struct adc_rts5918_config *const cfg = dev->config;
	volatile struct adc_regs *regs = cfg->regs;

	if (channel_cfg->acquisition_time != ADC_ACQ_TIME_DEFAULT) {
		LOG_ERR("Conversion time not supported!");
		return -EINVAL;
	}

	if (channel_cfg->channel_id >= rts5918_ADC_MAX_CHAN) {
		LOG_ERR("Channel %d not supported!", channel_cfg->channel_id);
		return -EINVAL;
	}

	if (channel_cfg->gain != ADC_GAIN_1) {
		LOG_ERR("ADC gain not supported!");
		return -EINVAL;
	}

	uint8_t channel_id = channel_cfg->channel_id;

	/* enable channel interrupt */
	// regs->INTEN |= (0x01ul << channel_id);
	/* enable channel LPF */
	regs->LPFCTRL |= (0x01ul << channel_id);

	return 0;
}

static bool adc_rts5918_validate_buffer_size(const struct adc_sequence *sequence)
{
	int chan_count = 0;
	size_t buff_need;
	uint32_t chan_mask;

	for (chan_mask = 0x80; chan_mask != 0; chan_mask >>= 1) {
		if (chan_mask & sequence->channels) {
			chan_count++;
		}
	}

	buff_need = chan_count * sizeof(uint16_t);

	if (sequence->options) {
		buff_need *= 1 + sequence->options->extra_samplings;
	}

	if (buff_need > sequence->buffer_size) {
		return false;
	}

	return true;
}

static int adc_rts5918_enable(const struct device *dev)
{
	const struct adc_rts5918_config *const cfg = dev->config;
	volatile struct adc_regs *regs = cfg->regs;
	int64_t st = k_uptime_get();

	LOG_DBG("adc enable");
	/* ADC Enable */
	regs->CTRL_b.cfg_sar_on = 1;
	return 0;
}

static int adc_rts5918_start_read(const struct device *dev, const struct adc_sequence *sequence)
{
	struct adc_rts5918_data *const data = dev->data;

	if (sequence->channels & ~BIT_MASK(rts5918_ADC_MAX_CHAN)) {
		LOG_ERR("Incorrect channels, bitmask 0x%x", sequence->channels);
		return -EINVAL;
	}

	if (sequence->channels == 0UL) {
		LOG_ERR("No channel selected");
		return -EINVAL;
	}

	if (!adc_rts5918_validate_buffer_size(sequence)) {
		LOG_ERR("Incorrect buffer size");
		return -ENOMEM;
	}

	data->channels = sequence->channels;
	data->buffer = sequence->buffer;

	if (adc_rts5918_enable(dev) < 0) {
		return -EIO;
	}

	adc_context_start_read(&data->ctx, sequence);

	return adc_context_wait_for_completion(&data->ctx);
}

static int adc_rts5918_read(const struct device *dev, const struct adc_sequence *sequence)
{
	struct adc_rts5918_data *const data = dev->data;
	const struct adc_rts5918_config *const cfg = dev->config;
	volatile struct adc_regs *regs = cfg->regs;	
	int error;

	adc_context_lock(&data->ctx, false, NULL);
	error = adc_rts5918_start_read(dev, sequence);
	if (error != 0) {
		regs->INTEN1 = 0x1FFFFF;
		NVIC_ClearPendingIRQ(DT_INST_IRQN(0));
		/* ADC Disable */
		regs->CTRL_b.cfg_sar_on = 0;
		/* Regular channels trigger disable */
    	regs->REGULAR_b.cfg_sar_rexttrig = 0;
		/* Reset ADC controller */
    	regs->REGULAR_b.cfg_sar_rswstart = 0;		
	}
	adc_context_release(&data->ctx, error);

	return error;
}

#define LPF_DATA_MASK GENMASK(11, 0)
static void rts5918_adc_get_sample(const struct device *dev)
{
	const struct adc_rts5918_config *const cfg = dev->config;
	volatile struct adc_regs *regs = cfg->regs;
	struct adc_rts5918_data *const data = dev->data;
	uint32_t idx;
	uint32_t channels = data->channels;
	uint32_t bit;
	volatile uint32_t *LPFDATA;
	uint32_t lpf_idx;
	bool	 lpf_up_down;

	/*
	 * Using the enabled channel bit set, from
	 * lowest channel number to highest, find out
	 * which channel is enabled and copy the ADC
	 * values from hardware registers to the data
	 * buffer.
	 */
	bit = find_lsb_set(channels);

	while (bit != 0) {
		idx = bit - 1;
		lpf_idx = idx / 2;
		lpf_up_down = idx % 2;
		LPFDATA = (&regs->LPFDATA0) + lpf_idx;
		if(lpf_up_down)
			*data->buffer = (uint16_t)((*LPFDATA >> 16) & LPF_DATA_MASK);
		else
			*data->buffer = (uint16_t)(*LPFDATA & LPF_DATA_MASK);
		data->buffer++;

		channels &= ~BIT(idx);
		bit = find_lsb_set(channels);
	}
}

static void adc_rts5918_single_isr(const struct device *dev)
{
	const struct adc_rts5918_config *const cfg = dev->config;
	volatile struct adc_regs *regs = cfg->regs;
	struct adc_rts5918_data *const data = dev->data;
	uint32_t inten1;

	if (regs->CTRL_b.cfg_sar_cont == 0) {

		inten1 = regs->INTEN1;
		regs->INTEN1 &= regs->INTEN1;
		if((inten1 & BIT(17)) == 0)
			return;
		rts5918_adc_get_sample(dev);

		/* ADC Disable */
		regs->CTRL_b.cfg_sar_on = 0;
		/* Regular channels trigger disable */
    	regs->REGULAR_b.cfg_sar_rexttrig = 0;
		/* Reset ADC controller */
    	regs->REGULAR_b.cfg_sar_rswstart = 0;

		adc_context_on_sampling_done(&data->ctx, dev);
	}
}

static int adc_rts5918_init(const struct device *dev)
{
	const struct adc_rts5918_config *const cfg = dev->config;
	struct adc_rts5918_data *const data = dev->data;
	volatile struct adc_regs *regs = cfg->regs;

	int ret;

	data->adc_dev = dev;

	ret = pinctrl_apply_state(cfg->pcfg, PINCTRL_STATE_DEFAULT);
	if (ret != 0) {
		LOG_ERR("rts5918 ADC pinctrl setup failed (%d)", ret);
		return ret;
	}

#ifdef CONFIG_CLOCK_CONTROL
	if (!device_is_ready(cfg->clk_dev)) {
		LOG_ERR("clock \"%s\" device not ready", cfg->clk_dev->name);
		return -ENODEV;
	}

	ret = clock_control_on(cfg->clk_dev, (clock_control_subsys_t)&cfg->sccon_cfg);
	if (ret != 0) {
		LOG_ERR("clock power on fail");
		return ret;
	}
#endif
	/* set default RSQ value */
	regs->RSQ0 = 0x0A418820;
    regs->RSQ1 = 0x16A4A0E6;
    regs->RSQ2 = 0x0007B9AC;

	/* set Force end of conversion interrupt enable */
	regs->INTEN_b.cfg_sar_eoc = true;
	regs->INTEN_b.cfg_sar_normal_eoc = true;
	regs->INTEN_b.cfg_sar_normal_eocie = true;
	regs->INTEN_b.cfg_sar_eocie = true;

	regs->CALICTRL0_b.cfg_sar_cal_bypass = false;
	/* set Regular channel sequence length */
	regs->REGULAR_b.cfg_sar_rl = 0xf;

	/* disable ADC controller */
    regs->CTRL_b.cfg_sar_on = 0;

	/* Reset ADC controller */
    regs->REGULAR_b.cfg_sar_rswstart = 0;
    // regs->REGULAR_b.cfg_sar_rswstart = 1;

	IRQ_CONNECT(DT_INST_IRQN(0), DT_INST_IRQ(0, priority), adc_rts5918_single_isr,
		    DEVICE_DT_INST_GET(0), 0);
	irq_enable(DT_INST_IRQN(0));

	adc_context_unlock_unconditionally(&data->ctx);

	return 0;
}

#define DEV_CONFIG_CLK_DEV_INIT(n)                                                                 \
	.clk_dev = DEVICE_DT_GET(DT_INST_CLOCKS_CTLR(n)),                                          \
	.sccon_cfg = {                                                                             \
		.clk_grp = DT_INST_CLOCKS_CELL(n, clk_grp),                                        \
		.clk_idx = DT_INST_CLOCKS_CELL(n, clk_idx),                                        \
	}

#define ADC_rts5918_INIT(n)                                                                        \
	PINCTRL_DT_INST_DEFINE(n);                                                                 \
                                                                                                   \
	struct adc_driver_api adc_rts5918_api_##n = {                                            \
		.channel_setup = adc_rts5918_channel_setup,                                        \
		.read = adc_rts5918_read,                                                          \
		.ref_internal = DT_INST_PROP(n, vref_mv),                                          \
	};                                                                                         \
                                                                                                   \
	static struct adc_rts5918_config adc_rts5918_dev_cfg_##n = {                               \
		.regs = (struct adc_regs *)(DT_INST_REG_ADDR(n)),                                  \
		.pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(n),                                         \
		DEV_CONFIG_CLK_DEV_INIT(n)};                                                       \
                                                                                                   \
	static struct adc_rts5918_data adc_rts5918_dev_data_##n = {                                \
		ADC_CONTEXT_INIT_TIMER(adc_rts5918_dev_data_##n, ctx),                             \
		ADC_CONTEXT_INIT_LOCK(adc_rts5918_dev_data_##n, ctx),                              \
		ADC_CONTEXT_INIT_SYNC(adc_rts5918_dev_data_##n, ctx),                              \
	};                                                                                         \
                                                                                                   \
	DEVICE_DT_INST_DEFINE(n, adc_rts5918_init, NULL, &adc_rts5918_dev_data_##n,                \
			      &adc_rts5918_dev_cfg_##n, PRE_KERNEL_1, CONFIG_ADC_INIT_PRIORITY,    \
			      &adc_rts5918_api_##n);

DT_INST_FOREACH_STATUS_OKAY(ADC_rts5918_INIT)
