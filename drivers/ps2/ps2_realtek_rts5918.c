/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT realtek_rts5918_ps2

#include <zephyr/device.h>
#include <zephyr/kernel.h>

#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/clock_control_rts5918.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/drivers/ps2.h>

#include "reg/reg_ps2.h"

LOG_MODULE_REGISTER(ps2_realtek_rts5918, CONFIG_PS2_LOG_LEVEL);

/* in 50us units */
#define PS2_RETRY_COUNT 10000

#define IS_BIT_SET(reg, bit)        (((reg >> bit) & (0x1)) != 0)
/*
 * The max duration of a PS/2 clock is about 100 micro-seconds.
 * A PS/2 transaction needs 11 clock cycles. It will take about 1.1 ms for a
 * complete transaction.
 */
#define PS2_TRANSACTION_TIMEOUT K_MSEC(2)

#ifdef CONFIG_PS2_REALTEK_FW_INHIBIT
// #define GPIO_PS2_CLK *((uint32_t*)0x40230110)
#define GPIO_PS2_CLK *((uint32_t*)(0x40230000 + 4*CONFIG_PS2_REALTEK_CLK_PIN))
#define GPIO_OUTPUT_L 0x10803
#endif

typedef void (*ps2_rts5918_irq_init_fn_ptr)(void);
struct ps2_rts5918_config {
    volatile struct ps2_regs * const regs;
    const struct device *clk_dev;
    uint32_t clk_grp;
    uint32_t clk_idx;
	const struct pinctrl_dev_config *pcfg;
    uint16_t irq_num;
    ps2_rts5918_irq_init_fn_ptr irq_init_fn_ptr;
};

struct ps2_rts5918_data {
	ps2_callback_t callback_isr;
    /* The mutex of the PS/2 controller */
	struct k_sem lock;
    /* The semaphore to synchronize the Tx transaction */
	struct k_sem tx_sync_sem;
};

static uint32_t temp_cnt = 0;

static int ps2_rts5918_configure(const struct device *dev,
            ps2_callback_t callback_isr)
{
	const struct ps2_rts5918_config *const config = dev->config;
    struct ps2_rts5918_data *const data = dev->data;
    volatile struct ps2_regs * const regs = config->regs;

    //LOG_INF("-- ps2_rts5918_configure --");

    if (!callback_isr) {
		return -EINVAL;
	}
    k_sem_take(&data->lock, K_FOREVER);

	data->callback_isr = callback_isr;

	/* Reset */
    regs->CTRL &= ~(1U << PS2_CTRL_RST_Pos);
	regs->CTRL |= (1U << PS2_CTRL_RST_Pos);
	/* Clear status bits */
    regs->STS |= 0xFF;
	/* Enable PS2 */
    regs->CTRL |= (1U << PS2_CTRL_EN_Pos);

    k_sem_give(&data->lock);

    return 0;
}

static int ps2_npcx_ctrl_bus_busy(const struct device *dev)
{
	const struct ps2_rts5918_config *const config = dev->config;
    volatile struct ps2_regs * const regs = config->regs;

	return IS_BIT_SET(regs->CTRL, PS2_CTRL_READY_Pos) ?
		       0 : -EBUSY;
}

static int ps2_rts5918_write(const struct device *dev, uint8_t value)
{
	const struct ps2_rts5918_config *const config = dev->config;
    struct ps2_rts5918_data *const data = dev->data;
    volatile struct ps2_regs * const regs = config->regs;
    int i = 0;


    if(k_sem_take(&data->lock, K_MSEC(10))) {
		return -EACCES;
	}

	// LOG_INF("ps2_rts5918_write: value=0x%02x", value);
    while (ps2_npcx_ctrl_bus_busy(dev) && (i < PS2_RETRY_COUNT)) {
		k_busy_wait(5);
		i++;
	}

    if (unlikely(i == PS2_RETRY_COUNT)) {
		LOG_ERR("PS2 write attempt timed out, CTRL=0x%08x", regs->CTRL);
		goto timeout_invalid;
	}

    regs->CTRL |= (1U << PS2_CTRL_MDSEL_Pos);
	/* clear staus */
	regs->STS |= 0xff;
	/* Enable Start Of Transaction interrupt */
	regs->INTEN |= BIT(PS2_INTEN_STRINTEN_Pos);
    /* set Tx data */
    regs->TXDAT = value & PS2_TXDAT_DATA_Msk;
	temp_cnt = 1;
    /* manual trigger for Tx mode */
    regs->CTRL |= (1U << PS2_CTRL_TXSTR_Pos);
	k_busy_wait(15);
	regs->CTRL &= ~BIT(PS2_CTRL_MDSEL_Pos);
	// LOG_INF("ps2_rts5913_write [0x%02x]", value);
    // while((regs->STS & (1U << PS2_STS_TDS_Pos)) == 0); // wait until transfer done
    if (k_sem_take(&data->tx_sync_sem, PS2_TRANSACTION_TIMEOUT) != 0) {
        /* Change the PS/2 module to receive mode */
		regs->CTRL &= ~BIT(PS2_CTRL_MDSEL_Pos);
		// LOG_ERR("PS2 write data timeout");
		goto timeout_invalid;
    }
	/* Clear status bits */
    // regs->STS |= 0xFF;

    k_sem_give(&data->lock);
	// LOG_INF("ps2_rts5918_write: success[0x%02x]", value);
    return 0;
timeout_invalid:
	k_sem_give(&data->lock);
	return -ETIMEDOUT;
}

static int ps2_rts5918_inhibit_interface(const struct device *dev)
{
	const struct ps2_rts5918_config * const config = dev->config;
	struct ps2_rts5918_data * const data = dev->data;
	volatile struct ps2_regs * const regs = config->regs;

	if (k_sem_take(&data->lock, K_MSEC(10)) != 0) {
		return -EACCES;
	}
#ifdef CONFIG_PS2_REALTEK_FW_INHIBIT
	/* set clock low to prevent device send next byte */
	GPIO_PS2_CLK = GPIO_OUTPUT_L;
#endif
    /* Reset all control */
	regs->CTRL &= ~(1U << PS2_CTRL_EN_Pos);
    /* Clear status bits */
	regs->STS |= 0xFF;

	NVIC_ClearPendingIRQ(config->irq_num);

	k_sem_give(&data->lock);

	return 0;
}

static int ps2_rts5918_enable_interface(const struct device *dev)
{
	const struct ps2_rts5918_config * const config = dev->config;
	struct ps2_rts5918_data * const data = dev->data;
	volatile struct ps2_regs * const regs = config->regs;

    if (k_sem_take(&data->lock, K_MSEC(10)) != 0) {
		return -EACCES;
	}

	/* set to default pin configure */
	pinctrl_apply_state(config->pcfg, PINCTRL_STATE_DEFAULT);
	/* Clear status bits */
    regs->STS |= 0xFF;
	/* Enable PS2 */
    regs->CTRL |= (1U << PS2_CTRL_EN_Pos);

	k_sem_give(&data->lock);

	return 0;
}

static int ps2_rts5918_is_rx_error(const struct device *dev)
{
    const struct ps2_rts5918_config * const config = dev->config;
	volatile struct ps2_regs * const regs = config->regs;
	uint8_t status;

	status = regs->STS & ( BIT(PS2_STS_RXTO_Pos) |      /* PS2 Receive Timeout */
                           BIT(PS2_STS_STPERR_Pos) |    /* Stop Bit Error */
                           BIT(PS2_STS_PRTERR_Pos) );   /* Parity Bit Error */

	if (status) {
		if (status & BIT(PS2_STS_RXTO_Pos))
			LOG_ERR("PS2 Receive Timeout");
		if (status & BIT(PS2_STS_STPERR_Pos))
			LOG_ERR("Stop Bit Error");
        if (status & BIT(PS2_STS_PRTERR_Pos))
			LOG_ERR("Parity Bit Error");
		return -EIO;
	}
    return 0;
}

static int ps2_rts5918_is_status_rx_error(const uint32_t STS)
{
	uint8_t status;
	status = STS & ( BIT(PS2_STS_RXTO_Pos) |      /* PS2 Receive Timeout */
						BIT(PS2_STS_STPERR_Pos) |    /* Stop Bit Error */
						BIT(PS2_STS_PRTERR_Pos) );   /* Parity Bit Error */

	if (status) {
		if (status & BIT(PS2_STS_RXTO_Pos))
			LOG_ERR("PS2 Receive Timeout");
		if (status & BIT(PS2_STS_STPERR_Pos))
			LOG_ERR("Stop Bit Error");
        if (status & BIT(PS2_STS_PRTERR_Pos))
			LOG_ERR("Parity Bit Error");
		return -EIO;
	}
    return 0;
}

static void ps2_rts5918_isr(const struct device *dev)
{
    const struct ps2_rts5918_config * const config = dev->config;
	struct ps2_rts5918_data * const data = dev->data;
	volatile struct ps2_regs * const regs = config->regs;
	uint32_t ps2_sts;
	int key;
    /* PS/2 Start of Transaction */
	if (IS_BIT_SET(regs->STS, PS2_STS_STRSTS_Pos) &&
	    IS_BIT_SET(regs->INTEN, PS2_INTEN_STRINTEN_Pos)) {
		/*
		 * Once set, SOT is not cleared until the shift mechanism
		 * is reset. Therefore, SOTIE should be cleared on the
		 * first occurrence of an SOT interrupt.
		 */
		regs->INTEN &= ~BIT(PS2_INTEN_STRINTEN_Pos);

		/* PS/2 End of Transaction */
    } else if (IS_BIT_SET(regs->STS, PS2_STS_TDS_Pos)) {
        regs->INTEN &= ~BIT(PS2_INTEN_TDSINTEN_Pos);
		key = irq_lock();
		/* backup Status register */
		ps2_sts = regs->STS;
		/* clear all status pending bit */
		regs->STS |= 0xff;
		/* Clear NVIC IRQ */
		NVIC_ClearPendingIRQ(DT_INST_IRQN(0));
		/* Enable STR and TDS Interrupt */
		regs->INTEN |= (BIT(PS2_INTEN_STRINTEN_Pos) | BIT(PS2_INTEN_TDSINTEN_Pos));
		irq_unlock(key);

        /* Tx is done */
		if (temp_cnt == 1) {
		// if (IS_BIT_SET(regs->CTRL, PS2_CTRL_MDSEL_Pos)) {
			/* Change the PS/2 module to receive mode */
			// regs->CTRL &= ~BIT(PS2_CTRL_MDSEL_Pos);
			temp_cnt = 0;
			k_sem_give(&data->tx_sync_sem);
		} else {
			// if (ps2_rts5918_is_rx_error(dev) == 0) {
			if (ps2_rts5918_is_status_rx_error(ps2_sts) == 0) {
				ps2_callback_t callback;
				uint8_t data_in = regs->RXDAT;
				// LOG_INF("Recv:0x%02x", data_in);
				callback = data->callback_isr;
				if (callback != NULL) {
#ifdef CONFIG_PS2_REALTEK_FW_INHIBIT
					/* set clock low to prevent device send next byte */
					GPIO_PS2_CLK = GPIO_OUTPUT_L;
#endif
					callback(dev, data_in);
				}
			}else{
				LOG_ERR("ps2 rx error");
			}
		}
		// regs->STS |= 0xff;
        /*
		 * Re-enable the Start Of Transaction interrupt when
		 * the shift mechanism is reset
		 */
        // regs->INTEN |= (BIT(PS2_INTEN_STRINTEN_Pos) | BIT(PS2_INTEN_TDSINTEN_Pos));
        LOG_DBG("EOT");
    }
	//NVIC_ClearPendingIRQ(DT_INST_IRQN(0));
}

static const struct ps2_driver_api ps2_rts5918_driver_api = {
	.config = ps2_rts5918_configure,
	.read = NULL,
	.write = ps2_rts5918_write,
	.disable_callback = ps2_rts5918_inhibit_interface,
	.enable_callback = ps2_rts5918_enable_interface,
};

static int ps2_rts5918_init(const struct device *dev)
{
	const struct ps2_rts5918_config *const config = dev->config;
    struct ps2_rts5918_data * const data = dev->data;
    volatile struct ps2_regs * const regs = config->regs;

	if (!device_is_ready(config->clk_dev)) {
		LOG_ERR("PS2 clock not ready");
		return -ENODEV;
	}

    struct rts5918_sccon_subsys sccon;
    sccon.clk_grp = config->clk_grp;
    sccon.clk_idx = config->clk_idx;

	int ret = clock_control_on(config->clk_dev, (clock_control_subsys_t)&sccon);

	if (ret != 0) {
		LOG_ERR("PS2 clock control on failed");
		return ret;
	}

	ret = pinctrl_apply_state(config->pcfg, PINCTRL_STATE_DEFAULT);

	if (ret < 0) {
		LOG_ERR("PS2 pinctrl setup failed (%d)", ret);
	}

    /*
	 * PS/2 interrupt enable register
	 * [0] - : STRINTEN   = 1: Start Condition Interrupt Enable
	 * [1] - : TDSINTEN   = 1: Transaction Done Interrupt Enable
	 * [2] - : STSINTEN   = 1: Status Interrupt Enable
	 */
	regs->INTEN = 0
			| BIT(PS2_INTEN_STRINTEN_Pos)
			| BIT(PS2_INTEN_TDSINTEN_Pos)
		    // | BIT(PS2_INTEN_STSINTEN_Pos)
			;

    k_sem_init(&data->lock, 1, 1);
    k_sem_init(&data->tx_sync_sem, 0, 1);
    config->irq_init_fn_ptr();
	NVIC_ClearPendingIRQ(DT_INST_IRQN(0));

	return ret;
}

#define REALTEK_PS2_DEVICE(inst)                                           \
                                                                           \
    PINCTRL_DT_INST_DEFINE(inst);                                          \
                                                                           \
    static void ps2_rts5918_irq_init_##inst(void)			               \
	{								                                       \
		IRQ_CONNECT(DT_INST_IRQN(inst), DT_INST_IRQ(inst, priority),	   \
			    ps2_rts5918_isr, DEVICE_DT_INST_GET(inst), 0);	           \
		irq_enable(DT_INST_IRQN(inst));									   \
	}								                                       \
								                                           \
    static const struct ps2_rts5918_config ps2_rts5918_config_##inst = {   \
        .pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(inst),                      \
        .clk_dev = DEVICE_DT_GET(DT_INST_CLOCKS_CTLR(0)),                  \
        .clk_grp = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), ps2, clk_grp),   \
        .clk_idx = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), ps2, clk_idx),   \
        .regs = (struct ps2_regs * const)(DT_INST_REG_ADDR(inst)),         \
        .irq_num = DT_INST_IRQN(inst),                                     \
        .irq_init_fn_ptr = ps2_rts5918_irq_init_##inst,		               \
    };                                                                     \
                                                                           \
    static struct ps2_rts5918_data ps2_rts5918_data_##inst; 			   \
    DEVICE_DT_INST_DEFINE(inst, ps2_rts5918_init, NULL,                    \
                  &ps2_rts5918_data_##inst,                                \
                  &ps2_rts5918_config_##inst, POST_KERNEL,                 \
                  CONFIG_PS2_INIT_PRIORITY,                                \
                  &ps2_rts5918_driver_api);

DT_INST_FOREACH_STATUS_OKAY(REALTEK_PS2_DEVICE)