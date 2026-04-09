/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2025 Realtek Semiconductor Corporation, SIBG-SD7
 * Author: Chia-Yang Lin <cylin0708@realtek.com>
 */

#define DT_DRV_COMPAT realtek_rts5918_rtc

#include <soc.h>
#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/kernel.h>
#include <zephyr/spinlock.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/clock_control_rts5918.h>
#include "rtc_utils.h"

#include "reg/reg_system.h"
#include "reg/reg_rtc.h"

LOG_MODULE_REGISTER(rtc_rts5918, CONFIG_RTC_LOG_LEVEL);

#define RTS5918_RTC_TIME_MASK                                                                      \
	(RTC_ALARM_TIME_MASK_SECOND | RTC_ALARM_TIME_MASK_MINUTE | RTC_ALARM_TIME_MASK_HOUR |      \
	 RTC_ALARM_TIME_MASK_WEEKDAY | RTC_ALARM_TIME_MASK_MONTHDAY | RTC_ALARM_TIME_MASK_MONTH |  \
	 RTC_ALARM_TIME_MASK_YEAR)

#define RTS5918_RTC_DIVCTL_NORMAL_OPERATION BIT(1)

#define RTS5918_RTC_DAYWEEK_OFFSET 1
#define RTS5918_RTC_MONTH_OFFSET   1
#define RTS5918_RTC_YEAR_OFFSET    100

typedef void (*rtc_rts5918_irq_init_fn_ptr)(void);
struct rtc_rts5918_config {
	RTC_Type *regs;
	uint32_t rtc_base;
	uint32_t rtc_clk_grp;
	uint32_t rtc_clk_idx;
	const struct device *clk_dev;
	uint16_t irq_num;
	rtc_rts5918_irq_init_fn_ptr irq_init_fn_ptr;
};

struct rtc_rts5918_data {
#ifdef CONFIG_RTC_ALARM
	rtc_alarm_callback alarm_callback;
	void *alarm_user_data;
#endif /* CONFIG_RTC_ALARM */
#ifdef CONFIG_RTC_UPDATE
	rtc_update_callback update_callback;
	void *update_user_data;
#endif /* CONFIG_RTC_UPDATE */
	struct k_spinlock lock;
	struct k_sem cr_sec_evt_sem;
	struct k_sem cr_upd_ack_sem;
};

static void rtc_rts5918_reset_rtc_time(const struct device *dev)
{
	const struct rtc_rts5918_config *const dev_cfg = dev->config;
	RTC_Type *rtc_regs = dev_cfg->regs;

	rtc_regs->CTRL1 |= RTC_CTRL1_SETMODE_Msk;
	rtc_regs->CTRL0 &= ~RTC_CTRL0_DIVCTL_Msk;
	rtc_regs->CTRL0 |= (RTS5918_RTC_DIVCTL_NORMAL_OPERATION << RTC_CTRL0_DIVCTL_Pos);
	rtc_regs->CTRL1 |= RTC_CTRL1_DATEMODE_Msk;
	rtc_regs->CTRL1 |= RTC_CTRL1_HRMODE_Msk;
	rtc_regs->SEC = 0;
	rtc_regs->MIN = 0;
	rtc_regs->HR &= ~(RTC_HR_AMPM_Msk | RTC_HR_VAL_Msk);
	rtc_regs->DAYWEEK = BIT(0);
	rtc_regs->DAYMONTH = BIT(0);
	rtc_regs->MONTH = BIT(0);
	rtc_regs->YEAR = 0;
	rtc_regs->WEEK &= ~RTC_WEEK_NUM_Msk;
}

static int rtc_rts5918_set_time(const struct device *dev, const struct rtc_time *timeptr)
{
	const struct rtc_rts5918_config *const dev_cfg = dev->config;
	RTC_Type *rtc_regs = dev_cfg->regs;

	if (!rtc_utils_validate_rtc_time(timeptr, RTS5918_RTC_TIME_MASK)) {
		rtc_rts5918_reset_rtc_time(dev);
		k_msleep(1);
		return -EINVAL;
	}

	rtc_regs->CTRL1 |= RTC_CTRL1_SETMODE_Msk;
	rtc_regs->SEC = timeptr->tm_sec;
	rtc_regs->MIN = timeptr->tm_min;
	rtc_regs->HR = timeptr->tm_hour;
	rtc_regs->DAYWEEK = timeptr->tm_wday + RTS5918_RTC_DAYWEEK_OFFSET;
	rtc_regs->DAYMONTH = timeptr->tm_mday;
	rtc_regs->MONTH = timeptr->tm_mon + RTS5918_RTC_MONTH_OFFSET;
	rtc_regs->YEAR = timeptr->tm_year % RTS5918_RTC_YEAR_OFFSET;
	/* Need to delay in order to update register after setting RTC time */
	k_msleep(1);
	rtc_regs->CTRL1 &= ~RTC_CTRL1_SETMODE_Msk;

	return 0;
}

static int rtc_rts5918_get_time(const struct device *dev, struct rtc_time *timeptr)
{
	const struct rtc_rts5918_config *const dev_cfg = dev->config;
	RTC_Type *rtc_regs = dev_cfg->regs;

	timeptr->tm_sec = rtc_regs->SEC;
	timeptr->tm_min = rtc_regs->MIN;
	timeptr->tm_hour = rtc_regs->HR;
	timeptr->tm_wday = rtc_regs->DAYWEEK - RTS5918_RTC_DAYWEEK_OFFSET;
	timeptr->tm_mday = rtc_regs->DAYMONTH;
	timeptr->tm_mon = rtc_regs->MONTH - RTS5918_RTC_MONTH_OFFSET;
	timeptr->tm_year = rtc_regs->YEAR + RTS5918_RTC_YEAR_OFFSET;

	/* No support for daylight saving time flag and nanoseconds in rts5918 RTC */
	timeptr->tm_isdst = -1;
	timeptr->tm_nsec = 0;

	return 0;
}

static void rtc_rts5918_isr(const struct device *dev)
{
	struct rtc_rts5918_data *data = dev->data;
	const struct rtc_rts5918_config *const dev_cfg = dev->config;
	RTC_Type *rtc_regs = dev_cfg->regs;
	uint32_t sr = rtc_regs->CTRL2;

#ifdef CONFIG_RTC_ALARM
	if (sr & RTC_CTRL2_AIF_Msk) {
		// regs->RTC_SCCR = RTC_SCCR_ALRCLR;
		rtc_regs->CTRL2 = RTC_CTRL2_AIF_Msk;
		if (data->alarm_callback != NULL) {
			data->alarm_callback(dev, 0, data->alarm_user_data);
		}
	}
#endif /* CONFIG_RTC_ALARM */
}

#ifdef CONFIG_RTC_ALARM
static uint16_t rtc_rts5918_alarm_get_supported_mask(void)
{
	return (RTC_ALARM_TIME_MASK_SECOND
	      | RTC_ALARM_TIME_MASK_MINUTE
	      | RTC_ALARM_TIME_MASK_HOUR
	      | RTC_ALARM_TIME_MASK_MONTHDAY
	      | RTC_ALARM_TIME_MASK_WEEKDAY);
}

static int rtc_rts5918_alarm_get_supported_fields(const struct device *dev, uint16_t id,
					      uint16_t *mask)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(id);

	*mask = rtc_rts5918_alarm_get_supported_mask();
	return 0;
}

static int rtc_rts5918_alarm_set_time(const struct device *dev, uint16_t id, uint16_t mask,
				  const struct rtc_time *timeptr)
{
	struct rtc_rts5918_data *data = dev->data;
	const struct rtc_rts5918_config *config = dev->config;
	RTC_Type *regs = config->regs;

	uint32_t mask_supported;

	mask_supported = rtc_rts5918_alarm_get_supported_mask();

	if ((id != 0)) {
		LOG_ERR("error id");
		return -EINVAL;
	}

	if ((mask > 0) && (timeptr == NULL)) {
		LOG_ERR("timertr is NULL");
		return -EINVAL;
	}

	if (mask & ~mask_supported) {
		LOG_ERR("mask not support, mask=0x%08x, mask_support=0x%08x", mask, mask_supported);
		return -EINVAL;
	}

	if (rtc_utils_validate_rtc_time(timeptr, mask) == false) {
		LOG_ERR("rtc time not validated");
		return -EINVAL;
	}

	k_spinlock_key_t key = k_spin_lock(&data->lock);

	irq_disable(config->irq_num);
	// rtc_regs->CTRL1 |= RTC_CTRL1_SETMODE_Msk;
	/* Set RTC alarm time */
	if (mask & RTC_ALARM_TIME_MASK_SECOND) {
		regs->SECALARM = timeptr->tm_sec;
	}else{
		regs->SECALARM = regs->SEC;
	}
	if (mask & RTC_ALARM_TIME_MASK_MINUTE) {
		regs->MINALARM = timeptr->tm_min;
	}else{
		regs->MINALARM = regs->MIN;
	}
	if (mask & RTC_ALARM_TIME_MASK_HOUR) {
		regs->HRALARM = timeptr->tm_hour;
	}else{
		regs->HRALARM = regs->HR;
	}
	if (mask & RTC_ALARM_TIME_MASK_WEEKDAY) {
		regs->DAYWEEKALARM = timeptr->tm_wday + RTS5918_RTC_DAYWEEK_OFFSET;
	}else{
		regs->DAYWEEKALARM = regs->DAYWEEK;
	}
	if (mask & RTC_ALARM_TIME_MASK_MONTHDAY) {
		regs->DAYMONTHALARM = timeptr->tm_mday;
	}else{
		regs->DAYMONTHALARM = regs->DAYMONTH;
	}
	/* Need to delay in order to update register after setting RTC time */
	k_msleep(1);
	// rtc_regs->CTRL1 &= ~RTC_CTRL1_SETMODE_Msk;

	/* Clear alarm pending status */
	regs->CTRL2 |= RTC_CTRL2_AIF_Msk;

	irq_enable(config->irq_num);
	k_spin_unlock(&data->lock, key);
	return 0;
}

static int rtc_rts5918_alarm_get_time(const struct device *dev, uint16_t id, uint16_t *mask,
				  struct rtc_time *timeptr)
{
	struct rtc_rts5918_data *data = dev->data;
	const struct rtc_rts5918_config *config = dev->config;
	RTC_Type *regs = config->regs;

	if ((id != 0) || (mask == NULL) || (timeptr == NULL)) {
		return -EINVAL;
	}

	k_spinlock_key_t key = k_spin_lock(&data->lock);

	timeptr->tm_sec = regs->SECALARM;
	timeptr->tm_min = regs->MINALARM;
	timeptr->tm_hour = regs->HRALARM;
	timeptr->tm_wday = regs->DAYWEEKALARM;
	timeptr->tm_mday = regs->DAYMONTHALARM;

	k_spin_unlock(&data->lock, key);

	LOG_DBG("get time:  mday = %d, wday = %d, hour = %d, "
		"min = %d, sec = %d",
		timeptr->tm_mday, timeptr->tm_wday,
		timeptr->tm_hour, timeptr->tm_min, timeptr->tm_sec);

	return 0;
}

static int rtc_rts5918_alarm_is_pending(const struct device *dev, uint16_t id)
{
	struct rtc_rts5918_data *data = dev->data;
	const struct rtc_rts5918_config *config = dev->config;
	RTC_Type *regs = config->regs;

	if (id != 0) {
		return -EINVAL;
	}

	k_spinlock_key_t key = k_spin_lock(&data->lock);

	if ((regs->CTRL2 & RTC_CTRL2_AIF_Msk) == 0) {
		k_spin_unlock(&data->lock, key);

		return 0;
	}

	k_spin_unlock(&data->lock, key);
	return 1;
}

static int rtc_rts5918_alarm_set_callback(const struct device *dev, uint16_t id,
				      rtc_alarm_callback callback, void *user_data)
{
	struct rtc_rts5918_data *data = dev->data;
	const struct rtc_rts5918_config *config = dev->config;
	RTC_Type *regs = config->regs;

	if (id != 0) {
		LOG_ERR("alarm set callback, wrong id=%d", id);
		return -EINVAL;
	}

	k_spinlock_key_t key = k_spin_lock(&data->lock);

	irq_disable(config->irq_num);
	data->alarm_callback = callback;
	data->alarm_user_data = user_data;

	if (data->alarm_callback) {
		regs->CTRL1 |= RTC_CTRL1_AIE_Msk;
	} else {
		regs->CTRL1 &= RTC_CTRL1_AIE_Msk;
	}

	irq_enable(config->irq_num);
	k_spin_unlock(&data->lock, key);
	return 0;
}
#endif /* CONFIG_RTC_ALARM */

// static DEVICE_API(rtc, rtc_rts5918_driver_api) = {
static const struct rtc_driver_api rtc_rts5918_driver_api = {
	.set_time = rtc_rts5918_set_time,
	.get_time = rtc_rts5918_get_time,
#ifdef CONFIG_RTC_ALARM
	.alarm_get_supported_fields = rtc_rts5918_alarm_get_supported_fields,
	.alarm_set_time = rtc_rts5918_alarm_set_time,
	.alarm_get_time = rtc_rts5918_alarm_get_time,
	.alarm_is_pending = rtc_rts5918_alarm_is_pending,
	.alarm_set_callback = rtc_rts5918_alarm_set_callback,
#endif /* CONFIG_RTC_ALARM */
};

static int rtc_rts5918_init(const struct device *dev)
{
	const struct rtc_rts5918_config *const rtc_config = dev->config;
	struct rts5918_sccon_subsys sccon;

	int rc;

	if (!device_is_ready(rtc_config->clk_dev)) {
		LOG_ERR("RTC device not ready");
		return -ENODEV;
	}

	sccon.clk_grp = rtc_config->rtc_clk_grp;
	sccon.clk_idx = rtc_config->rtc_clk_idx;
	rc = clock_control_on(rtc_config->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc < 0) {
		LOG_ERR("Failed to turn on RTC clock (%d)", rc);
		return rc;
	}

	rtc_rts5918_reset_rtc_time(dev);
	rtc_config->irq_init_fn_ptr();

	return rc;
}

#define RTC_RTS5918_CONFIG(inst)                                                                   \
	static struct rtc_rts5918_config rtc_rts5918_config_##inst = {                             \
		.regs = (RTC_Type *)(DT_INST_REG_ADDR(inst)),                                      \
		.rtc_base = DT_INST_REG_ADDR(inst),                                                \
		.rtc_clk_grp = DT_INST_CLOCKS_CELL_BY_NAME(inst, rtc, clk_grp),                    \
		.rtc_clk_idx = DT_INST_CLOCKS_CELL_BY_NAME(inst, rtc, clk_idx),                    \
		.clk_dev = DEVICE_DT_GET(DT_INST_CLOCKS_CTLR(inst)),                               \
		.irq_num = DT_INST_IRQN(inst),                                                      \
		.irq_init_fn_ptr = rtc_rts5918_irq_init_##inst,		\
	};

#define RTC_RTS5918_DEVICE_INIT(index)                                                             \
	static void rtc_rts5918_irq_init_##index(void)				\
	{								\
		IRQ_CONNECT(DT_INST_IRQN(index), DT_INST_IRQ(index, priority),	\
			    rtc_rts5918_isr, DEVICE_DT_INST_GET(index), 0);	\
	}								\
									\
	RTC_RTS5918_CONFIG(index)                                                                  \
	static struct rtc_rts5918_data rtc_rts5918_data_##index;			\
	DEVICE_DT_INST_DEFINE(index, &rtc_rts5918_init, NULL, &rtc_rts5918_data_##index, &rtc_rts5918_config_##index,   \
			      POST_KERNEL, CONFIG_RTC_INIT_PRIORITY, &rtc_rts5918_driver_api);

DT_INST_FOREACH_STATUS_OKAY(RTC_RTS5918_DEVICE_INIT)
