/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT realtek_rts5918_espi

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/espi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/drivers/flash/rts5918_flash_api.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/clock_control_rts5918.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(espi, CONFIG_ESPI_LOG_LEVEL);

#include "espi_utils.h"
#include "reg/reg_acpi.h"
#include "reg/reg_emi.h"
#include "reg/reg_espi.h"
#include "reg/reg_kbc.h"
#include "reg/reg_port80.h"
#include "reg/reg_mbox.h"

BUILD_ASSERT(DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) == 1, "support only one espi compatible node");

struct espi_rts5918_config {
	volatile struct espi_reg *const espi_reg;
	uint32_t espislv_clk_grp;
	uint32_t espislv_clk_idx;
#ifdef CONFIG_ESPI_PERIPHERAL_8042_KBC
	volatile struct kbc_reg *const kbc_reg;
	uint32_t kbc_clk_grp;
	uint32_t kbc_clk_idx;
#endif
#ifdef CONFIG_ESPI_PERIPHERAL_HOST_IO
	volatile struct acpi_reg *const acpi_reg;
	uint32_t acpi_clk_grp;
	uint32_t acpi_clk_idx;
#endif
#ifdef CONFIG_ESPI_PERIPHERAL_EC_HOST_CMD
	volatile struct acpi_reg *const promt0_reg;
	uint32_t promt0_clk_grp;
	uint32_t promt0_clk_idx;
	volatile struct emi_reg *const emi0_reg;
	uint32_t emi0_clk_grp;
	uint32_t emi0_clk_idx;

	volatile struct acpi_reg *const promt1_reg;
	uint32_t promt1_clk_grp;
	uint32_t promt1_clk_idx;

	volatile struct acpi_reg *const promt2_reg;
	uint32_t promt2_clk_grp;
	uint32_t promt2_clk_idx;

	volatile struct acpi_reg *const promt3_reg;
	uint32_t promt3_clk_grp;
	uint32_t promt3_clk_idx;
#endif
#ifdef CONFIG_ESPI_PERIPHERAL_ACPI_SHM_REGION
	volatile struct emi_reg *const emi1_reg;
	uint32_t emi1_clk_grp;
	uint32_t emi1_clk_idx;
#endif
#ifdef CONFIG_ESPI_PERIPHERAL_DEBUG_PORT_80
	volatile struct port80_reg *const port80_reg;
	uint32_t port80_clk_grp;
	uint32_t port80_clk_idx;
#endif
#ifdef CONFIG_ESPI_MBOX
	volatile struct mbox_reg *const mbox_reg;
#endif
	const struct device *clk_dev;
	const struct pinctrl_dev_config *pcfg;
};

struct espi_rts5918_data {
	sys_slist_t callbacks;
	uint32_t config_data;
#ifdef CONFIG_ESPI_PERIPHERAL_8042_KBC
	int kbc_int_en;
	int kbc_pre_irq1;
	int ps2_pre_irq12;
#endif
#ifdef CONFIG_ESPI_OOB_CHANNEL
	struct k_sem oob_rx_lock;
	struct k_sem oob_tx_lock;
	uint8_t *oob_tx_ptr;
	uint8_t *oob_rx_ptr;
	bool oob_tx_busy;
#endif
#ifdef CONFIG_ESPI_FLASH_CHANNEL
	struct k_sem flash_lock;
	uint8_t *maf_ptr;
#endif
};

/*
 * =========================================================================
 * ESPI Peripheral KBC
 * =========================================================================
 */

#ifdef CONFIG_ESPI_PERIPHERAL_8042_KBC

static int espi_send_vw_event(uint8_t index, uint8_t data, const struct device *dev);

static void kbc_ibf_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	struct espi_event evt = {
		ESPI_BUS_PERIPHERAL_NOTIFICATION,
		ESPI_PERIPHERAL_8042_KBC,
		ESPI_PERIPHERAL_NODATA,
	};
	volatile struct kbc_reg *const kbc_reg = espi_config->kbc_reg;
	struct espi_evt_data_kbc *kbc_evt = (struct espi_evt_data_kbc *)&evt.evt_data;

	/*
	 * Indicates if the host sent a command or data.
	 * 0 = data
	 * 1 = Command.
	 */
	kbc_evt->type = kbc_reg->STS & KBC_STS_CMDSEL ? 1 : 0;

	/* The data in KBC Input Buffer */
	//kbc_evt->data = kbc_reg->IB;

	/* KBC Input Buffer Full event */
	kbc_evt->evt = HOST_KBC_EVT_IBF;
	espi_send_callbacks(&espi_data->callbacks, dev, evt);
}

static void kbc_obe_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	struct espi_event evt = {
		ESPI_BUS_PERIPHERAL_NOTIFICATION,
		ESPI_PERIPHERAL_8042_KBC,
		ESPI_PERIPHERAL_NODATA,
	};
	volatile struct kbc_reg *const kbc_reg = espi_config->kbc_reg;
	struct espi_evt_data_kbc *kbc_evt = (struct espi_evt_data_kbc *)&evt.evt_data;

	if (espi_data->kbc_pre_irq1 == 1 && !espi_send_vw_event(0x0, 0x01, dev)) {
		espi_data->kbc_pre_irq1 = 0;
	}

	if (espi_data->ps2_pre_irq12 == 1 && !espi_send_vw_event(0x0, 0x0C, dev)) {
		espi_data->ps2_pre_irq12 = 0;
	}

	if (kbc_reg->STS & KBC_STS_OBF) {
		kbc_reg->STS |= KBC_STS_CLR_OBF_FULL;
		kbc_reg->STS |= KBC_STS_CLR_OBF_FULL_STS;
	}
	kbc_reg->STS |= ACPI_STS_CLR_OBF_FULL_STS;
	/* Notify application that host already read out data. */
	kbc_evt->type = 0;
	kbc_evt->data = 0;
	kbc_evt->evt = HOST_KBC_EVT_OBE;
	espi_send_callbacks(&espi_data->callbacks, dev, evt);
}

static int espi_kbc_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *const espi_data = dev->data;
	struct rts5918_sccon_subsys sccon;
	volatile struct kbc_reg *const kbc_reg = espi_config->kbc_reg;
	int rc;

	if (!device_is_ready(espi_config->clk_dev)) {
		LOG_ERR("KBC clock not ready");
		return -ENODEV;
	}

	espi_data->kbc_int_en = 0;
	espi_data->kbc_pre_irq1 = 0;
	espi_data->ps2_pre_irq12 = 0;

	sccon.clk_grp = espi_config->kbc_clk_grp;
	sccon.clk_idx = espi_config->kbc_clk_idx;

	rc = clock_control_on(espi_config->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc != 0) {
		LOG_ERR("KBC clock control on failed");
		return rc;
	}

	kbc_reg->VWCTRL1 = (0x01 << KBC_VWCTRL1_IRQNUM_Pos) | KBC_VWCTRL1_ACTEN;
	kbc_reg->INTEN = KBC_INTEN_IBFINTEN | KBC_INTEN_OBFINTEN;

	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), kbc_ibf, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), kbc_obe, irq));

	/* IBF */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), kbc_ibf, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), kbc_ibf, priority), kbc_ibf_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), kbc_ibf, irq));

	/* OBE */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), kbc_obe, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), kbc_obe, priority), kbc_obe_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), kbc_obe, irq));

	return 0;
}

static int lpc_request_read_8042(const struct device *dev, enum lpc_peripheral_opcode op,
				 uint32_t *data)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct kbc_reg *const kbc_reg = espi_config->kbc_reg;

	switch (op) {
	case E8042_OBF_HAS_CHAR:
		*data = (kbc_reg->STS & KBC_STS_OBF) ? 1 : 0;
		break;
	case E8042_IBF_HAS_CHAR:
		*data = (kbc_reg->STS & KBC_STS_IBF) ? 1 : 0;
		break;
	case E8042_READ_KB_STS:
		*data = kbc_reg->STS;
		break;
	case E8042_READ_KB_DATA:
		*data = kbc_reg->IB;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int espi_send_vw_event(uint8_t index, uint8_t data, const struct device *dev);

static void espi_send_vw_event_with_kbdata(uint8_t index, uint8_t data, uint32_t kbc_data,
					   const struct device *dev);

static uint8_t kbc_write(uint8_t data, const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *const espi_data = dev->data;
	volatile struct kbc_reg *const kbc_reg = espi_config->kbc_reg;
	uint32_t exData = (uint32_t)data;

	if ((espi_data->kbc_pre_irq1 == 1)&&(espi_data->kbc_int_en)) {
		/* Gen IRQ1-Level High to VW ch */
		espi_send_vw_event(0x0, 0x01, dev);
	}

	if (espi_data->kbc_int_en) {

		espi_data->kbc_pre_irq1 = 1;
		/* Gen IRQ1-Level High to VW ch */
		espi_send_vw_event_with_kbdata(0x0, 0x81, exData, dev);
	} else {
		kbc_reg->OB = exData;
	}

	return 0;
}

static uint8_t ps2_write(uint8_t data, const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *const espi_data = dev->data;
	volatile struct kbc_reg *const kbc_reg = espi_config->kbc_reg;
	uint32_t exData = (uint32_t)data;

	if ((espi_data->ps2_pre_irq12 == 1)&&(espi_data->kbc_int_en)) {
		/* Gen IRQ12-Level Low to VW ch */
		espi_send_vw_event(0x0, 0x0C, dev);
	}

	if (espi_data->kbc_int_en) {
		/* set flag first to avoid interrupt before flag set */
		espi_data->ps2_pre_irq12 = 1;
		/* Gen IRQ12-Level High to VW ch */
		espi_send_vw_event_with_kbdata(0x0, 0x8C, exData, dev);
	} else {
		kbc_reg->OB = exData;
	}

	return 0;
}

static int lpc_request_write_8042(const struct device *dev, enum lpc_peripheral_opcode op,
				  uint32_t *data)
{
	struct espi_rts5918_data *const espi_data = dev->data;
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct kbc_reg *const kbc_reg = espi_config->kbc_reg;

	switch (op) {
	case E8042_WRITE_KB_CHAR:
		kbc_write(*data & 0xff, dev);
		break;
	case E8042_WRITE_MB_CHAR:
		ps2_write(*data & 0xff, dev);
		break;
	case E8042_RESUME_IRQ:
		espi_data->kbc_int_en = 1;
		break;
	case E8042_RESUME_OBE_IRQ:
		kbc_reg->INTEN |= KBC_INTEN_OBFINTEN;
		break;
	case E8042_PAUSE_IRQ:
		espi_data->kbc_int_en = 0;
		break;
	case E8042_PAUSE_OBE_IRQ:
		kbc_reg->INTEN &= ~KBC_INTEN_OBFINTEN;
		break;
	case E8042_CLEAR_OBF:
		kbc_reg->STS |= KBC_STS_CLR_OBF_FULL;
		kbc_reg->STS |= KBC_STS_CLR_OBF_FULL_STS;
		break;
	case E8042_CLEAR_IBF:
		kbc_reg->STS |= KBC_STS_CLR_IBF_FULL;
		break;
	case E8042_LDN_ACT_EN:
		kbc_reg->VWCTRL1 |= KBC_VWCTRL1_ACTEN;
		break;
	case E8042_LDN_ACT_DIS:
		kbc_reg->VWCTRL1 &= ~KBC_VWCTRL1_ACTEN;
		break;
	case E8042_SET_FLAG:
		/* FW shouldn't modify these flags directly */
		// *data &= ~(KBC_STS_OBF | KBC_STS_IBF | KBC_STS_STS2);
		*data &= ~(KBC_STS_OBF | KBC_STS_IBF | KBC_STS_CLR_OBF_FULL | KBC_STS_CLR_IBF_FULL);
		kbc_reg->STS |= *data & 0xff;
		break;
	case E8042_CLEAR_FLAG:
		/* FW shouldn't modify these flags directly */
		// *data |= KBC_STS_OBF | KBC_STS_IBF | KBC_STS_STS2;
		*data |= KBC_STS_OBF | KBC_STS_IBF | KBC_STS_CLR_OBF_FULL | KBC_STS_CLR_IBF_FULL;
		kbc_reg->STS &= ~(*data & 0xff);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

#else /* CONFIG_ESPI_PERIPHERAL_8042_KBC */

static int lpc_request_read_8042(const struct device *dev, enum lpc_peripheral_opcode op,
				 uint32_t *data)
{
	return -ENOTSUP;
}

static int lpc_request_write_8042(const struct device *dev, enum lpc_peripheral_opcode op,
				  uint32_t *data)
{
	return -ENOTSUP;
}

#endif /* CONFIG_ESPI_PERIPHERAL_8042_KBC */

/*
 * =========================================================================
 * ESPI Peripheral Shared Memory Region
 * =========================================================================
 */

#ifdef CONFIG_ESPI_PERIPHERAL_ACPI_SHM_REGION
#define ESPI_RTK_PERIPHERAL_ACPI_SHD_MEM_SIZE 256

static uint8_t acpi_shd_mem_sram[ESPI_RTK_PERIPHERAL_ACPI_SHD_MEM_SIZE] __aligned(256);

static void espi_setup_acpi_shm(const struct espi_rts5918_config *const espi_config)
{
	volatile struct emi_reg *const emi1_reg = espi_config->emi1_reg;

	emi1_reg->SAR = (uint32_t)&acpi_shd_mem_sram[0];
}

#endif /* CONFIG_ESPI_PERIPHERAL_ACPI_SHM_REGION */

/*
 * =========================================================================
 * ESPI Peripheral Host IO (ACPI)
 * =========================================================================
 */

#ifdef CONFIG_ESPI_PERIPHERAL_HOST_IO

/*
 * Phase 5.9 (Bison): the upper-layer Lenovo ecfw shim
 * (mdl_espi_hub.c::task_acpi_handler) dispatches based on
 * `espi_evt_data_acpi::type` matching `enum espihub_acpi_evt_data`:
 *   ESPIHUB_ACPI_IBF_DATA = 0
 *   ESPIHUB_ACPI_IBF_CMD  = 1
 *   ESPIHUB_ACPI_OBE      = 2
 *
 * The original driver only set type for IBF (relying on CMDSEL == 1)
 * and left it at 0 for OBE, which was misclassified as an IBF data
 * event. Both ISRs now set the correct type so OBE actually flows
 * through to IntEc0Obf() in HwAcpicom.c.
 */
#define ESPIHUB_ACPI_IBF_DATA  0
#define ESPIHUB_ACPI_IBF_CMD   1
#define ESPIHUB_ACPI_OBE       2

static void acpi_ibf_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	struct espi_event evt = {ESPI_BUS_PERIPHERAL_NOTIFICATION, ESPI_PERIPHERAL_HOST_IO,
				 ESPI_PERIPHERAL_NODATA};
	struct espi_evt_data_acpi *acpi_evt = (struct espi_evt_data_acpi *)&evt.evt_data;
	volatile struct acpi_reg *const acpi_reg = espi_config->acpi_reg;

	/* Host put data on input buffer of ACPI EC0 channel */
	if (acpi_reg->STS & ACPI_STS_IBF) {
		/*
		 * Disable the line until HwAcpicom::IntEc0Ibf re-arms it.
		 * The data byte is left in IDR — IntEc0Ibf re-reads via the
		 * drv_host_acpi helpers and clears IBF as a side effect.
		 */
		NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_ibf, irq)));

		acpi_evt->type = (acpi_reg->STS & ACPI_STS_CMDSEL)
				 ? ESPIHUB_ACPI_IBF_CMD
				 : ESPIHUB_ACPI_IBF_DATA;
		acpi_evt->data = 0;
		acpi_evt->evt  = HOST_ACPI_EVT_IBF;
		espi_send_callbacks(&espi_data->callbacks, dev, evt);
	}
}

static void acpi_obe_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = (struct espi_rts5918_data *)dev->data;
	struct espi_event evt = {ESPI_BUS_PERIPHERAL_NOTIFICATION, ESPI_PERIPHERAL_HOST_IO,
				 ESPI_PERIPHERAL_NODATA};
	struct espi_evt_data_acpi *acpi_evt = (struct espi_evt_data_acpi *)&evt.evt_data;
	volatile struct acpi_reg *const acpi_reg = espi_config->acpi_reg;

	if (acpi_reg->STS & ACPI_STS_OBF) {
		acpi_reg->STS |= ACPI_STS_CLR_OBF_FULL;
	}

	acpi_reg->STS |= ACPI_STS_CLR_OBF_FULL_STS;
	/* Notify application that host already read out data. */
	acpi_evt->type = ESPIHUB_ACPI_OBE;
	acpi_evt->data = 0;
	acpi_evt->evt  = HOST_ACPI_EVT_OBE;
	espi_send_callbacks(&espi_data->callbacks, dev, evt);
}

static int espi_acpi_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct rts5918_sccon_subsys sccon;
	volatile struct acpi_reg *const acpi_reg = espi_config->acpi_reg;
	int rc;

	if (!device_is_ready(espi_config->clk_dev)) {
		LOG_ERR("ACPI clock not ready");
		return -ENODEV;
	}

	sccon.clk_grp = espi_config->acpi_clk_grp;
	sccon.clk_idx = espi_config->acpi_clk_idx;
	rc = clock_control_on(espi_config->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc != 0) {
		LOG_ERR("ACPI clock control on failed");
		return rc;
	}

	acpi_reg->VWCTRL1 = (0x00UL << ACPI_VWCTRL1_IRQNUM_Pos) | ACPI_VWCTRL1_ACTEN;
	acpi_reg->INTEN = ACPI_INTEN_IBFINTEN  | ACPI_INTEN_OBFINTEN;

	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_ibf, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_obe, irq));

	/* IBF */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_ibf, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_ibf, priority), acpi_ibf_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_ibf, irq));

	/* OBE */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_obe, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_obe, priority), acpi_obe_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_obe, irq));

	return 0;
}

static int espi_emi_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct rts5918_sccon_subsys sccon;
	int rc;

	if (!device_is_ready(espi_config->clk_dev)) {
		LOG_ERR("ACPI clock not ready");
		return -ENODEV;
	}

	sccon.clk_grp = espi_config->emi0_clk_grp;
	sccon.clk_idx = espi_config->emi0_clk_idx;
	rc = clock_control_on(espi_config->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc != 0) {
		LOG_ERR("ACPI clock control on failed");
		return rc;
	}

	sccon.clk_grp = espi_config->emi1_clk_grp;
	sccon.clk_idx = espi_config->emi1_clk_idx;
	rc = clock_control_on(espi_config->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc != 0) {
		LOG_ERR("ACPI clock control on failed");
		return rc;
	}



	return 0;
}

static int lpc_request_read_acpi(const struct espi_rts5918_config *const espi_config,
				 enum lpc_peripheral_opcode op, uint32_t *data)
{
	volatile struct acpi_reg *const acpi_reg = espi_config->acpi_reg;

	switch (op) {
	case EACPI_OBF_HAS_CHAR:
		*data = acpi_reg->STS & ACPI_STS_OBF ? 1 : 0;
		break;
	case EACPI_IBF_HAS_CHAR:
		*data = acpi_reg->STS & ACPI_STS_IBF ? 1 : 0;
		break;
	case EACPI_READ_STS:
		*data = acpi_reg->STS;
		break;
#ifdef CONFIG_ESPI_PERIPHERAL_ACPI_SHM_REGION
	case EACPI_GET_SHARED_MEMORY:
		*data = (uint32_t)acpi_shd_mem_sram;
		break;
#endif
	default:
		return -EINVAL;
	}

	return 0;
}

static int lpc_request_write_acpi(const struct espi_rts5918_config *const espi_config,
				  enum lpc_peripheral_opcode op, uint32_t *data)
{
	volatile struct acpi_reg *const acpi_reg = espi_config->acpi_reg;

	switch (op) {
	case EACPI_WRITE_CHAR:
		acpi_reg->OB = *data & 0xff;
		break;
	case EACPI_WRITE_STS:
		acpi_reg->STS = *data & 0xff;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

#else /* CONFIG_ESPI_PERIPHERAL_HOST_IO */

static int lpc_request_read_acpi(const struct espi_rts5918_config *const espi_config,
				 enum lpc_peripheral_opcode op, uint32_t *data)
{
	return -ENOTSUP;
}

static int lpc_request_write_acpi(const struct espi_rts5918_config *const espi_config,
				  enum lpc_peripheral_opcode op, uint32_t *data)
{
	return -ENOTSUP;
}

#endif /* CONFIG_ESPI_PERIPHERAL_HOST_IO */

/*
 * =========================================================================
 * ESPI Peripheral EC Host Command (Promt0)
 * =========================================================================
 */

#ifdef CONFIG_ESPI_PERIPHERAL_EC_HOST_CMD

#define ESPI_RTK_PERIPHERAL_HOST_CMD_PARAM_SIZE 256
static uint8_t ec_host_cmd_sram[ESPI_RTK_PERIPHERAL_HOST_CMD_PARAM_SIZE] __aligned(256);

/*
 * Phase 5.8/5.9/5.10/5.11 (Bison): the upper-layer Lenovo ecfw shim
 * (ecfw-zephyr/middle/espi/mdl_espi_hub.c) consumes promt-port events
 * as ESPI_PERIPHERAL_HOST_IO_PVT and overlays evt_data with
 * struct espi_evt_data_pvt_io { type:8; data:8; ... }, where `type` is
 * one of `enum espihub_pvt_io_evt_data` defined in
 * include/middle/mdl_espi_hub.h. We can't include that header from
 * this driver, so the numeric constants are duplicated below with the
 * symbolic name in the comment.
 *
 * IMPORTANT: keep these in sync with mdl_espi_hub.h::espihub_pvt_io_evt_data.
 *
 * Promt port → PVT IO type mapping for the Bison board (matches the
 * sister project's boards/lenovo/lbc2_arl_7088/realtek layout):
 *   promt0 → BATT  (HwSmapiCom)
 *   promt2 → 4CCCMD (HwEcApi)
 *   promt3 → NVS   (HwEsNvs)
 */
#define ESPIHUB_PVT_IO_BATT_CMD_IBF       0   /* host wrote SMAPI command  */
#define ESPIHUB_PVT_IO_BATT_DATA_IBF      1   /* host wrote SMAPI data     */
#define ESPIHUB_PVT_IO_BATT_OBE           2   /* host read SMAPI output    */
#define ESPIHUB_PVT_IO_NVS_CMD_IBF        3   /* host wrote NVS command    */
#define ESPIHUB_PVT_IO_NVS_DATA_IBF       4   /* host wrote NVS data       */
#define ESPIHUB_PVT_IO_NVS_OBE            5   /* host read NVS output      */
#define ESPIHUB_PVT_IO_4CCCMD_CMD_IBF     6   /* host wrote 4cccmd command */
#define ESPIHUB_PVT_IO_4CCCMD_DATA_IBF    7   /* host wrote 4cccmd data    */
#define ESPIHUB_PVT_IO_4CCCMD_OBE         8   /* host read 4cccmd output   */

struct rts5918_pvt_io_evt {
	uint32_t type     : 8;
	uint32_t data     : 8;
	uint32_t reserved : 16;
};

static void promt0_ibf_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct acpi_reg *const promt0_reg = espi_config->promt0_reg;
	struct espi_rts5918_data *data = dev->data;
	struct espi_event evt = {
		.evt_type    = ESPI_BUS_PERIPHERAL_NOTIFICATION,
		.evt_details = ESPI_PERIPHERAL_HOST_IO_PVT,
		.evt_data    = ESPI_PERIPHERAL_NODATA,
	};
	struct rts5918_pvt_io_evt *pvt =
		(struct rts5918_pvt_io_evt *)&evt.evt_data;

	if (promt0_reg->STS & ACPI_STS_IBF) {
		//printk("promt0_ibf_isr\r\n");
		bool is_cmd = (promt0_reg->STS & ACPI_STS_CMDSEL) ? true : false;

		/*
		 * Leave the data byte in IB; reading it clears IBF and would
		 * let the host overwrite it before the upper layer (IntEc1Ibf
		 * / SmapiIntInputBufferFull) latches it. The app reads IDR3
		 * itself.
		 */
		pvt->data = 0;
		pvt->type = is_cmd ? ESPIHUB_PVT_IO_BATT_CMD_IBF
				   : ESPIHUB_PVT_IO_BATT_DATA_IBF;

		NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt0_ibf, irq)));
		espi_send_callbacks(&data->callbacks, dev, evt);
	}
}

static void promt0_obe_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = (struct espi_rts5918_data *)dev->data;
	volatile struct acpi_reg *const promt0_reg = espi_config->promt0_reg;
	struct espi_event evt = {
		.evt_type    = ESPI_BUS_PERIPHERAL_NOTIFICATION,
		.evt_details = ESPI_PERIPHERAL_HOST_IO_PVT,
		.evt_data    = ESPI_PERIPHERAL_NODATA,
	};
	struct rts5918_pvt_io_evt *pvt =
		(struct rts5918_pvt_io_evt *)&evt.evt_data;

	if (promt0_reg->STS & ACPI_STS_OBF) {
		promt0_reg->STS |= ACPI_STS_CLR_OBF_FULL;
	}
	promt0_reg->STS |= ACPI_STS_CLR_OBF_FULL_STS;

	pvt->type = ESPIHUB_PVT_IO_BATT_OBE;
	pvt->data = 0;
	espi_send_callbacks(&espi_data->callbacks, dev, evt);
}

static int espi_promt0_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct rts5918_sccon_subsys sccon;
	volatile struct acpi_reg *const promt0_reg = espi_config->promt0_reg;
	int rc;

	if (!device_is_ready(espi_config->clk_dev)) {
		LOG_ERR("Promt0 clock not ready");
		return -ENODEV;
	}

	sccon.clk_grp = espi_config->promt0_clk_grp;
	sccon.clk_idx = espi_config->promt0_clk_idx;

	rc = clock_control_on(espi_config->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc != 0) {
		LOG_ERR("Promt0 clock control on failed");
		return rc;
	}

	promt0_reg->STS = 0;

	if (promt0_reg->STS & ACPI_STS_IBF) {
		(void)promt0_reg->IB;  /* discard pending byte */
	}
	if (promt0_reg->STS & ACPI_STS_IBF) {
		promt0_reg->STS |= ACPI_STS_CLR_IBF_FULL;
	}

	promt0_reg->PTADDR =
		CONFIG_ESPI_PERIPHERAL_HOST_CMD_DATA_PORT_NUM | (0x04 << ACPI_PTADDR_OFFSET_Pos);
	promt0_reg->VWCTRL1 = ACPI_VWCTRL1_ACTEN;
	promt0_reg->INTEN = ACPI_INTEN_IBFINTEN;

	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt0_ibf, irq));
NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt0_obe, irq));

	/* IBF */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt0_ibf, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), promt0_ibf, priority), promt0_ibf_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt0_ibf, irq));

	/* OBE */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt0_obe, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), promt0_obe, priority), promt0_obe_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt0_obe, irq));

	return 0;
}

static void promt1_ibf_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct acpi_reg *const promt1_reg = espi_config->promt1_reg;
	struct espi_rts5918_data *data = (struct espi_rts5918_data *)dev->data;
	struct espi_event evt = {.evt_type = ESPI_BUS_PERIPHERAL_NOTIFICATION,
				 .evt_details = ESPI_PERIPHERAL_INDEX_1 << 16 | ESPI_PERIPHERAL_EC_HOST_CMD,
				 .evt_data = ESPI_PERIPHERAL_NODATA};

	struct espi_evt_data_acpi *promt1_evt = (struct espi_evt_data_acpi *)&evt.evt_data;
	if (promt1_reg->STS & ACPI_STS_IBF) {
		/*
		* Indicates if the host sent a command or data.
		* 0 = data
		* 1 = Command.
		*/
		NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt1_ibf, irq)));
		promt1_evt->type = promt1_reg->STS & ACPI_STS_CMDSEL ? 1 : 0;

		/* The data in PROMT1 Input Buffer */
		promt1_evt->data = promt1_reg->IB;
		/* PROMT1 Input Buffer Full event */
		promt1_evt->evt = HOST_ACPI_EVT_IBF;
		espi_send_callbacks(&data->callbacks, dev, evt);
		NVIC_EnableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt1_ibf, irq)));
	}
}

static void promt1_obe_isr(const struct device *dev)
{
#ifdef CONFIG_ESPI_PERIPHERAL_PM1_OBE_EVT_DATA
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = (struct espi_rts5918_data *)dev->data;
	struct espi_event evt = {
		ESPI_BUS_PERIPHERAL_NOTIFICATION,
		ESPI_PERIPHERAL_INDEX_1 << 16 | ESPI_PERIPHERAL_EC_HOST_CMD,
		ESPI_PERIPHERAL_NODATA,
	};
	volatile struct acpi_reg *const promt1_reg = espi_config->promt1_reg;
	struct espi_evt_data_acpi *promt1_evt = (struct espi_evt_data_acpi *)&evt.evt_data;

	if (promt1_reg->STS & ACPI_STS_OBF) {
		promt1_reg->STS |= ACPI_STS_CLR_OBF_FULL;
	}
	promt1_reg->STS |= ACPI_STS_CLR_OBF_FULL_STS;
	/* Notify application that host already read out data. */
	promt1_evt->type = 0;
	promt1_evt->data = 0;
	/* PROMT1 Input Buffer Empty event */
	promt1_evt->evt = HOST_ACPI_EVT_OBE;
	espi_send_callbacks(&espi_data->callbacks, dev, evt);
#endif
}

static int espi_promt1_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct rts5918_sccon_subsys sccon;
	volatile struct acpi_reg *const promt1_reg = espi_config->promt1_reg;
	int rc;

	if (!device_is_ready(espi_config->clk_dev)) {
		LOG_ERR("Promt1 clock not ready");
		return -ENODEV;
	}

	sccon.clk_grp = espi_config->promt1_clk_grp;
	sccon.clk_idx = espi_config->promt1_clk_idx;

	rc = clock_control_on(espi_config->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc != 0) {
		LOG_ERR("Promt1 clock control on failed");
		return rc;
	}

	promt1_reg->STS = 0;

	if (promt1_reg->STS & ACPI_STS_IBF) {
		(void)promt1_reg->IB;  /* discard pending byte */
	}

	if (promt1_reg->STS & ACPI_STS_IBF) {
		promt1_reg->STS |= ACPI_STS_CLR_IBF_FULL;
	}

	promt1_reg->PTADDR =
		CONFIG_ESPI_PERIPHERAL_HOST_CMD_PM1_PARAM_PORT_NUM | (0x04 << ACPI_PTADDR_OFFSET_Pos);
	promt1_reg->VWCTRL1 = ACPI_VWCTRL1_ACTEN;
	promt1_reg->INTEN = ACPI_INTEN_IBFINTEN;

	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt1_ibf, irq));
NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt1_obe, irq));

	/* IBF */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt1_ibf, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), promt1_ibf, priority), promt1_ibf_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt1_ibf, irq));

	/* OBE */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt1_obe, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), promt1_obe, priority), promt1_obe_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt1_obe, irq));

	return 0;
}

/* Phase 5.12 (Bison): promt2 carries the 4cccmd / EcApi host port. */

static void promt2_ibf_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct acpi_reg *const promt2_reg = espi_config->promt2_reg;
	struct espi_rts5918_data *data = (struct espi_rts5918_data *)dev->data;
	struct espi_event evt = {
		.evt_type    = ESPI_BUS_PERIPHERAL_NOTIFICATION,
		.evt_details = ESPI_PERIPHERAL_HOST_IO_PVT,
		.evt_data    = ESPI_PERIPHERAL_NODATA,
	};
	struct rts5918_pvt_io_evt *pvt =
		(struct rts5918_pvt_io_evt *)&evt.evt_data;

	if (promt2_reg->STS & ACPI_STS_IBF) {
		bool is_cmd = (promt2_reg->STS & ACPI_STS_CMDSEL) ? true : false;

		/*
		 * Leave the data byte in IB; reading it clears IBF and would
		 * let the host overwrite it before the upper layer (IntEc3Ibf
		 * via Full_Interrupt_Handler) latches it. The app reads
		 * IDR_EC_API itself.
		 */
		pvt->data = 0;
		pvt->type = is_cmd ? ESPIHUB_PVT_IO_4CCCMD_CMD_IBF
				   : ESPIHUB_PVT_IO_4CCCMD_DATA_IBF;

		NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt2_ibf, irq)));
		espi_send_callbacks(&data->callbacks, dev, evt);
	}
}

static void promt2_obe_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = (struct espi_rts5918_data *)dev->data;
	volatile struct acpi_reg *const promt2_reg = espi_config->promt2_reg;
	struct espi_event evt = {
		.evt_type    = ESPI_BUS_PERIPHERAL_NOTIFICATION,
		.evt_details = ESPI_PERIPHERAL_HOST_IO_PVT,
		.evt_data    = ESPI_PERIPHERAL_NODATA,
	};
	struct rts5918_pvt_io_evt *pvt =
		(struct rts5918_pvt_io_evt *)&evt.evt_data;

	if (promt2_reg->STS & ACPI_STS_OBF) {
		promt2_reg->STS |= ACPI_STS_CLR_OBF_FULL;
	}
	promt2_reg->STS |= ACPI_STS_CLR_OBF_FULL_STS;

	pvt->type = ESPIHUB_PVT_IO_4CCCMD_OBE;
	pvt->data = 0;
	espi_send_callbacks(&espi_data->callbacks, dev, evt);
}

static int espi_promt2_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct rts5918_sccon_subsys sccon;
	volatile struct acpi_reg *const promt2_reg = espi_config->promt2_reg;
	int rc;

	if (!device_is_ready(espi_config->clk_dev)) {
		LOG_ERR("promt2 clock not ready");
		return -ENODEV;
	}

	sccon.clk_grp = espi_config->promt2_clk_grp;
	sccon.clk_idx = espi_config->promt2_clk_idx;

	rc = clock_control_on(espi_config->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc != 0) {
		LOG_ERR("promt2 clock control on failed");
		return rc;
	}

	promt2_reg->STS = 0;

	if (promt2_reg->STS & ACPI_STS_IBF) {
		(void)promt2_reg->IB;  /* discard pending byte */
	}

	if (promt2_reg->STS & ACPI_STS_IBF) {
		promt2_reg->STS |= ACPI_STS_CLR_IBF_FULL;
	}

	promt2_reg->PTADDR =
		CONFIG_ESPI_PERIPHERAL_HOST_CMD_PM2_PARAM_PORT_NUM | (0x04 << ACPI_PTADDR_OFFSET_Pos);
	promt2_reg->VWCTRL1 = ACPI_VWCTRL1_ACTEN;
	promt2_reg->INTEN = ACPI_INTEN_IBFINTEN;
	promt2_reg->INTEN |= ACPI_INTEN_OBFINTEN;

	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt2_ibf, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt2_obe, irq));

	/* IBF */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt2_ibf, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), promt2_ibf, priority), promt2_ibf_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt2_ibf, irq));

	/* OBE */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt2_obe, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), promt2_obe, priority), promt2_obe_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt2_obe, irq));

	return 0;
}

static void promt3_ibf_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct acpi_reg *const promt3_reg = espi_config->promt3_reg;
	struct espi_rts5918_data *data = (struct espi_rts5918_data *)dev->data;
	struct espi_event evt = {
		.evt_type    = ESPI_BUS_PERIPHERAL_NOTIFICATION,
		.evt_details = ESPI_PERIPHERAL_HOST_IO_PVT,
		.evt_data    = ESPI_PERIPHERAL_NODATA,
	};
	struct rts5918_pvt_io_evt *pvt =
		(struct rts5918_pvt_io_evt *)&evt.evt_data;

	if (promt3_reg->STS & ACPI_STS_IBF) {
		bool is_cmd = (promt3_reg->STS & ACPI_STS_CMDSEL) ? true : false;

		/*
		 * Leave the data byte in IB; reading it clears IBF and would
		 * let the host overwrite it before the upper layer (IntEc2Ibf)
		 * latches it. The app reads IDR_ES_NVS itself.
		 */
		pvt->data = 0;
		pvt->type = is_cmd ? ESPIHUB_PVT_IO_NVS_CMD_IBF
				   : ESPIHUB_PVT_IO_NVS_DATA_IBF;

		/* Disable the line until the upper layer processes the byte
		 * and re-arms it; matches the sister project's pattern in
		 * boards/lenovo/lbc2_arl_7088/realtek/HwEsNvs.c.
		 */
		NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt3_ibf, irq)));

		espi_send_callbacks(&data->callbacks, dev, evt);
	}
}

static void promt3_obe_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = (struct espi_rts5918_data *)dev->data;
	volatile struct acpi_reg *const promt3_reg = espi_config->promt3_reg;
	struct espi_event evt = {
		.evt_type    = ESPI_BUS_PERIPHERAL_NOTIFICATION,
		.evt_details = ESPI_PERIPHERAL_HOST_IO_PVT,
		.evt_data    = ESPI_PERIPHERAL_NODATA,
	};
	struct rts5918_pvt_io_evt *pvt =
		(struct rts5918_pvt_io_evt *)&evt.evt_data;

	if (promt3_reg->STS & ACPI_STS_OBF) {
		promt3_reg->STS |= ACPI_STS_CLR_OBF_FULL;
	}
	promt3_reg->STS |= ACPI_STS_CLR_OBF_FULL_STS;

	pvt->type = ESPIHUB_PVT_IO_NVS_OBE;
	pvt->data = 0;
	espi_send_callbacks(&espi_data->callbacks, dev, evt);
}

static int espi_promt3_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct rts5918_sccon_subsys sccon;
	volatile struct acpi_reg *const promt3_reg = espi_config->promt3_reg;
	int rc;

	if (!device_is_ready(espi_config->clk_dev)) {
		LOG_ERR("promt3 clock not ready");
		return -ENODEV;
	}

	sccon.clk_grp = espi_config->promt3_clk_grp;
	sccon.clk_idx = espi_config->promt3_clk_idx;

	rc = clock_control_on(espi_config->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc != 0) {
		LOG_ERR("promt3 clock control on failed");
		return rc;
	}

	promt3_reg->STS = 0;

	if (promt3_reg->STS & ACPI_STS_IBF) {
		(void)promt3_reg->IB;  /* discard pending byte */
	}

	if (promt3_reg->STS & ACPI_STS_IBF) {
		promt3_reg->STS |= ACPI_STS_CLR_IBF_FULL;
	}

	promt3_reg->PTADDR =
		CONFIG_ESPI_PERIPHERAL_HOST_CMD_PM3_PARAM_PORT_NUM | (0x04 << ACPI_PTADDR_OFFSET_Pos);
	promt3_reg->VWCTRL1 = ACPI_VWCTRL1_ACTEN;
	promt3_reg->INTEN = ACPI_INTEN_IBFINTEN;

	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt3_ibf, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt3_obe, irq));

	/* IBF */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt3_ibf, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), promt3_ibf, priority), promt3_ibf_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt3_ibf, irq));

	/* OBE */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt3_obe, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), promt3_obe, priority), promt3_obe_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), promt3_obe, irq));

	return 0;
}
#endif /* CONFIG_ESPI_PERIPHERAL_EC_HOST_CMD */

/*
 * =========================================================================
 * ESPI Peripheral Channel Read/Write API
 * =========================================================================
 */

#ifdef CONFIG_ESPI_PERIPHERAL_CHANNEL

uint8_t g_mailbox_index = 0xFF;
uint32_t g_mailbox_buffer[256];

#define BASE_ADDR_MBI ((uint32_t)(g_mailbox_buffer))

#define MBX_HOST_TO_EC_MAILBOX_REGISTER (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x00)))
#define MBX_EC_TO_HOST_MAILBOX_REGISTER (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x04)))

#define MBX_MAILBOX_REGISTER_0  (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x10))) //MBI_GPD0
#define MBX_MAILBOX_REGISTER_1  (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x14))) //MBI_GPD1
#define MBX_MAILBOX_REGISTER_2  (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x18))) //MBI_GPD2
#define MBX_MAILBOX_REGISTER_3  (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x1C))) //MBI_GPD3
#define MBX_MAILBOX_REGISTER_4  (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x20))) //MBI_GPD4
#define MBX_MAILBOX_REGISTER_5  (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x24))) //MBI_GPD5
#define MBX_MAILBOX_REGISTER_6  (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x28))) //MBI_GPD6
#define MBX_MAILBOX_REGISTER_7  (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x2C))) //MBI_GPD7
#define MBX_MAILBOX_REGISTER_8  (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x30))) //MBI_GPD8
#define MBX_MAILBOX_REGISTER_9  (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x34))) //MBI_GPD9
#define MBX_MAILBOX_REGISTER_10 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x38))) //MBI_GPD10
#define MBX_MAILBOX_REGISTER_11 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x3C))) //MBI_GPD11
#define MBX_MAILBOX_REGISTER_12 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x40))) //MBI_GPD12
#define MBX_MAILBOX_REGISTER_13 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x44))) //MBI_GPD13
#define MBX_MAILBOX_REGISTER_14 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x48))) //MBI_GPD14
#define MBX_MAILBOX_REGISTER_15 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x4C))) //MBI_GPD15
#define MBX_MAILBOX_REGISTER_16 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x50))) //MBI_GPD16
#define MBX_MAILBOX_REGISTER_17 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x54))) //MBI_GPD17
#define MBX_MAILBOX_REGISTER_18 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x58))) //MBI_GPD18
#define MBX_MAILBOX_REGISTER_19 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x5C))) //MBI_GPD19
#define MBX_MAILBOX_REGISTER_20 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x60))) //MBI_GPD20
#define MBX_MAILBOX_REGISTER_21 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x64))) //MBI_GPD21
#define MBX_MAILBOX_REGISTER_22 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x68))) //MBI_GPD22
#define MBX_MAILBOX_REGISTER_23 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x6C))) //MBI_GPD23
#define MBX_MAILBOX_REGISTER_24 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x70))) //MBI_GPD24
#define MBX_MAILBOX_REGISTER_25 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x74))) //MBI_GPD25
#define MBX_MAILBOX_REGISTER_26 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x78))) //MBI_GPD26
#define MBX_MAILBOX_REGISTER_27 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x7C))) //MBI_GPD27
#define MBX_MAILBOX_REGISTER_28 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x80))) //MBI_GPD28
#define MBX_MAILBOX_REGISTER_29 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x84))) //MBI_GPD29
#define MBX_MAILBOX_REGISTER_30 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x88))) //MBI_GPD30
#define MBX_MAILBOX_REGISTER_31 (*((volatile unsigned char *)(BASE_ADDR_MBI + 0x8C))) //MBI_GPD31

static void espi_periph_ch_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *data = dev->data;

	struct espi_event evt = {.evt_type = ESPI_BUS_EVENT_CHANNEL_READY,
				 .evt_details = ESPI_CHANNEL_PERIPHERAL,
				 .evt_data = 0};

	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;

	uint32_t status = espi_reg->EPSTS;
	uint32_t config = espi_reg->EPCFG;

	if (status & ESPI_EPSTS_CLRSTS) {
		evt.evt_data = config & ESPI_EPCFG_CHEN ? 1 : 0;
		espi_send_callbacks(&data->callbacks, dev, evt);

		espi_reg->EPSTS = ESPI_EPSTS_CLRSTS;
	}
#ifdef CONFIG_ESPI_RTS5918_IOSHORT_RW
	else if(status & ESPI_EPSTS_WR_HOLE) {
		evt.evt_type = ESPI_BUS_PERIPHERAL_NOTIFICATION;
		evt.evt_details = ESPI_PERIPHERAL_EC_HOST_CMD;
		evt.evt_data = ESPI_PERIPHERAL_NODATA;

		struct espi_evt_data_acpi *ioshort_evt = (struct espi_evt_data_acpi *)&evt.evt_data;
		/*
		* Indicates if the host sent a command or data.
		* 0 = data
		* 1 = Command.
		*/
		const uint16_t io_address = espi_reg->IOSHORTRDADDR & ESPI_IOSHORTRDADDR_ADDR;
		ioshort_evt->type = io_address == 0x1604? 1 : 0;

		// Handle Bios debug port ch1 (86h)
		if(io_address == 0x86)
		{
			evt.evt_details = ESPI_PERIPHERAL_DEBUG_PORT80;
			evt.evt_details |= (ESPI_PERIPHERAL_INDEX_1 << 16);
		}
		// Handle mailbox index (1610h)
		else if(io_address == 0x1610) {
			evt.evt_details |= (ESPI_PERIPHERAL_INDEX_5 << 16);
		}
		// Handle mailbox data (1611h)
		else if(io_address == 0x1611) {
			evt.evt_details |= (ESPI_PERIPHERAL_INDEX_6 << 16);
		}

		/* The data in IOSHORT Input Buffer */
		ioshort_evt->data = (espi_reg->IOSHORTRDDATA & 0xff);

    	// LOG_ERR("addr 0x%X value 0x%X", io_address, ioshort_evt->data);
		if(io_address == 0x86) {
			if(espi_reg->IOSHORTSTS & ESPI_IOSHORTSTS_TYPE) {
				espi_send_callbacks(&data->callbacks, dev, evt);
			}
		}
		else if (io_address == 0x1610 /* index */) {
			if(espi_reg->IOSHORTSTS & ESPI_IOSHORTSTS_TYPE) {
				/* Host Write */
				g_mailbox_index = ioshort_evt->data;
				// LOG_ERR("g_mailbox_index %x",g_mailbox_index);
			}
			// There is no case of "Host Read"
		}
		else if (io_address == 0x1611 /* data */) {
			if(espi_reg->IOSHORTSTS & ESPI_IOSHORTSTS_TYPE) {
				/* Host Write */
				// LOG_ERR("g_mailbox_index %x data %x",g_mailbox_index, ioshort_evt->data);
				// g_mailbox_buffer[g_mailbox_index] = ioshort_evt->data & 0xFF;
				switch(g_mailbox_index)
				{
					case 0x0:
						MBX_HOST_TO_EC_MAILBOX_REGISTER = ioshort_evt->data;
						break;
					case 0x1:
						if(ioshort_evt->data == 0xFF)
							MBX_EC_TO_HOST_MAILBOX_REGISTER = 0;
						break;
					default:
						if(g_mailbox_index >= 0x10) {
							g_mailbox_buffer[4 + (g_mailbox_index - 0x10)] = ioshort_evt->data;
						}
						break;
				}
				// MBX_MAILBOX_REGISTER_0 = ioshort_evt->data;
				if(MBX_HOST_TO_EC_MAILBOX_REGISTER != 0)
					IntMbx();
			} else {
				/* Host Read */
				uint32_t value = 0;
				switch(g_mailbox_index)
				{
					case 0x0:
						value = MBX_HOST_TO_EC_MAILBOX_REGISTER;
						break;
					default:
						if(g_mailbox_index >= 0x10) {
							value = g_mailbox_buffer[4 + (g_mailbox_index - 0x10)];
						}
						break;
				}
				// LOG_ERR("g_mailbox_index %x %x",g_mailbox_index, value);
				espi_reg->IOSHORTRDDATA = value << 8;
			}
		}
		int key = irq_lock();
		espi_reg->EPSTS = ESPI_EPSTS_WR_HOLE;
		espi_reg->IOSHORTSTS |= ESPI_IOSHORTSTS_RSPSTART | ESPI_IOSHORTSTS_ACCEPT;
		irq_unlock(key);
	}
#endif
	else {
		espi_reg->EPSTS = 0xF;
	}
}

static int lpc_request_read_custom(const struct espi_rts5918_config *const espi_config,
				   enum lpc_peripheral_opcode op, uint32_t *data)
{
#ifdef CONFIG_ESPI_PERIPHERAL_CUSTOM_OPCODE
volatile struct acpi_reg *const promt0_reg = espi_config->promt0_reg;
	volatile struct acpi_reg *const promt1_reg = espi_config->promt1_reg;
	volatile struct acpi_reg *const promt2_reg = espi_config->promt2_reg;
	volatile struct acpi_reg *const promt3_reg = espi_config->promt3_reg;

	switch (op) {
	case ECUSTOM_HOST_CMD_GET_PARAM_MEMORY:
		*data = (uint32_t)ec_host_cmd_sram;
		break;
	case ECUSTOM_HOST_CMD_GET_PARAM_MEMORY_SIZE:
		*data = ESPI_RTK_PERIPHERAL_HOST_CMD_PARAM_SIZE;
		break;
case ECUSTOM_OBF_HAS_CHAR:
		*data = (promt0_reg->STS & ACPI_STS_OBF) ? 1 : 0;
		break;
	case ECUSTOM_IBF_HAS_CHAR:
		*data = (promt0_reg->STS & ACPI_STS_IBF) ? 1 : 0;
		break;
	case ECUSTOM_IBF_CHAR:
		*data = promt0_reg->IB;
		break;
	case ECUSTOM_READ_HOST_STS:
		*data = promt0_reg->STS;
		break;
	case ECUSTOM_WRITE_HOST_STS:
		promt0_reg->STS = *data;
		break;
	case ECUSTOM_HOST_CMD_PM1_OBF_HAS_CHAR:
		*data = (promt1_reg->STS & ACPI_STS_OBF) ? 1 : 0;
		break;
	case ECUSTOM_HOST_CMD_PM1_IBF_HAS_CHAR:
		*data = (promt1_reg->STS & ACPI_STS_IBF) ? 1 : 0;
		break;
	case ECUSTOM_IBF_PM1_CHAR:
		*data = promt1_reg->IB;
		break;
	case ECUSTOM_READ_HOST_PM1_STS:
		*data = promt1_reg->STS;
		break;
	case ECUSTOM_HOST_CMD_PM2_OBF_HAS_CHAR:
		*data = (promt2_reg->STS & ACPI_STS_OBF) ? 1 : 0;
		break;
	case ECUSTOM_HOST_CMD_PM2_IBF_HAS_CHAR:
		*data = (promt2_reg->STS & ACPI_STS_IBF) ? 1 : 0;
		break;
	case ECUSTOM_IBF_PM2_CHAR:
		*data = promt2_reg->IB;
		break;
	case ECUSTOM_READ_HOST_PM2_STS:
		*data = promt2_reg->STS;
		break;
	case ECUSTOM_HOST_CMD_PM3_OBF_HAS_CHAR:
		*data = (promt3_reg->STS & ACPI_STS_OBF) ? 1 : 0;
		break;
	case ECUSTOM_HOST_CMD_PM3_IBF_HAS_CHAR:
		*data = (promt3_reg->STS & ACPI_STS_IBF) ? 1 : 0;
		break;
	case ECUSTOM_IBF_PM3_CHAR:
		*data = promt3_reg->IB;
		break;
	case ECUSTOM_READ_HOST_PM3_STS:
		*data = promt3_reg->STS;
		break;
	default:
		return -EINVAL;
	}

	return 0;
#else
	return -ENOTSUP;
#endif
}

static int espi_rts5918_read_lpc_request(const struct device *dev, enum lpc_peripheral_opcode op,
					 uint32_t *data)
{
	const struct espi_rts5918_config *const espi_config = dev->config;

	if (op >= E8042_START_OPCODE && op <= E8042_MAX_OPCODE) {
		return lpc_request_read_8042(dev, op, data);
	} else if (op >= EACPI_START_OPCODE && op <= EACPI_MAX_OPCODE) {
		return lpc_request_read_acpi(espi_config, op, data);
	} else if (op >= ECUSTOM_START_OPCODE && op <= ECUSTOM_MAX_OPCODE) {
		return lpc_request_read_custom(espi_config, op, data);
	} else {
		return -ENOTSUP;
	}
}

static int lpc_request_write_custom(const struct espi_rts5918_config *const espi_config,
				    enum lpc_peripheral_opcode op, uint32_t *data)
{
#ifdef CONFIG_ESPI_PERIPHERAL_CUSTOM_OPCODE
	volatile struct acpi_reg *const promt0_reg = espi_config->promt0_reg;
volatile struct acpi_reg *const promt1_reg = espi_config->promt1_reg;
	volatile struct acpi_reg *const promt2_reg = espi_config->promt2_reg;
	volatile struct acpi_reg *const promt3_reg = espi_config->promt3_reg;

	switch (op) {
	case ECUSTOM_HOST_SUBS_INTERRUPT_EN:
		if (*data == 0) {
			NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt0_ibf, irq)));
NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt1_ibf, irq)));
			NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt2_ibf, irq)));
			NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt3_ibf, irq)));
			NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_obe, irq)));
			NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_ibf, irq)));
			NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), port80, irq)));
			NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), kbc_ibf, irq)));
			NVIC_DisableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), kbc_obe, irq)));

		} else {
			NVIC_EnableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt0_ibf, irq)));
NVIC_EnableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt1_ibf, irq)));
			NVIC_EnableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt2_ibf, irq)));
			NVIC_EnableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), promt3_ibf, irq)));
			NVIC_EnableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_obe, irq)));
			NVIC_EnableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), acpi_ibf, irq)));
			NVIC_EnableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), port80, irq)));
			NVIC_EnableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), kbc_ibf, irq)));
			NVIC_EnableIRQ((DT_IRQ_BY_NAME(DT_DRV_INST(0), kbc_obe, irq)));
		}
		break;
	case ECUSTOM_HOST_CMD_SEND_RESULT:
		promt0_reg->STS &= ~ACPI_STS_STS0;
		promt0_reg->OB = *data & 0xff;
		break;
case ECUSTOM_HOST_CMD_SEND_WRITE:
		promt0_reg->OB = *data & 0xff;
		break;
	case ECUSTOM_HOST_CMD_SET_FLAG:
		promt0_reg->STS = (promt0_reg->STS | *data) & 0xff;
		break;
	case ECUSTOM_HOST_CMD_CLEAR_FLAG:
		promt0_reg->STS = (promt0_reg->STS & (~(*data & 0xff))) & 0xff;
		break;
	case ECUSTOM_RESUME_IBF_IRQ:
		promt0_reg->INTEN |= ACPI_INTEN_IBFINTEN;
		break;
	case ECUSTOM_PAUSE_IBF_IRQ:
		promt0_reg->INTEN &= ~ACPI_INTEN_IBFINTEN;
		break;
	case ECUSTOM_RESUME_OBE_IRQ:
		promt0_reg->INTEN |= ACPI_INTEN_OBFINTEN;
		break;
	case ECUSTOM_PAUSE_OBE_IRQ:
		promt0_reg->INTEN &= ~ACPI_INTEN_OBFINTEN;
		break;
		/* Promt 1 */
	case ECUSTOM_HOST_CMD_PM1_WRITE_CHAR:
		promt1_reg->OB = *data & 0xff;
		break;
	case ECUSTOM_HOST_CMD_PM1_SET_FLAG:
		promt1_reg->STS = (promt1_reg->STS | *data) & 0xff;
		break;
	case ECUSTOM_HOST_CMD_PM1_CLEAR_FLAG:
		promt1_reg->STS = (promt1_reg->STS & (~(*data & 0xff))) & 0xff;
		break;
	case ECUSTOM_HOST_CMD_PM1_RESUME_IBF_IRQ:
		promt1_reg->INTEN |= ACPI_INTEN_IBFINTEN;
		break;
	case ECUSTOM_HOST_CMD_PM1_PAUSE_IBF_IRQ:
		promt1_reg->INTEN &= ~ACPI_INTEN_IBFINTEN;
		break;
	case ECUSTOM_HOST_CMD_PM1_RESUME_OBE_IRQ:
		promt1_reg->INTEN |= ACPI_INTEN_OBFINTEN;
		break;
	case ECUSTOM_HOST_CMD_PM1_PAUSE_OBE_IRQ:
		promt1_reg->INTEN &= ~ACPI_INTEN_OBFINTEN;
		break;
		/* Promt 2 */
	case ECUSTOM_HOST_CMD_PM2_WRITE_CHAR:
		promt2_reg->OB = *data & 0xff;
		break;
	case ECUSTOM_HOST_CMD_PM2_SET_FLAG:
		promt2_reg->STS = (promt2_reg->STS | *data) & 0xff;
		break;
	case ECUSTOM_HOST_CMD_PM2_CLEAR_FLAG:
		promt2_reg->STS = (promt2_reg->STS & (~(*data & 0xff))) & 0xff;
		break;
	case ECUSTOM_HOST_CMD_PM2_RESUME_IBF_IRQ:
		promt2_reg->INTEN |= ACPI_INTEN_IBFINTEN;
		break;
	case ECUSTOM_HOST_CMD_PM2_PAUSE_IBF_IRQ:
		promt2_reg->INTEN &= ~ACPI_INTEN_IBFINTEN;
		break;
	case ECUSTOM_HOST_CMD_PM2_RESUME_OBE_IRQ:
		promt2_reg->INTEN |= ACPI_INTEN_OBFINTEN;
		break;
	case ECUSTOM_HOST_CMD_PM2_PAUSE_OBE_IRQ:
		promt2_reg->INTEN &= ~ACPI_INTEN_OBFINTEN;
		break;
		/* Promt 3 */
	case ECUSTOM_HOST_CMD_PM3_WRITE_CHAR:
		promt3_reg->OB = *data & 0xff;
		break;
	case ECUSTOM_HOST_CMD_PM3_RESUME_IBF_IRQ:
		promt3_reg->INTEN |= ACPI_INTEN_IBFINTEN;
		break;
	case ECUSTOM_HOST_CMD_PM3_PAUSE_IBF_IRQ:
		promt3_reg->INTEN &= ~ACPI_INTEN_IBFINTEN;
		break;
	case ECUSTOM_HOST_CMD_PM3_SET_FLAG:
		promt3_reg->STS = (promt3_reg->STS | *data) & 0xff;
		break;
	case ECUSTOM_HOST_CMD_PM3_CLEAR_FLAG:
		promt3_reg->STS = (promt3_reg->STS & (~(*data & 0xff))) & 0xff;
		break;
	case ECUSTOM_HOST_CMD_PM3_RESUME_OBE_IRQ:
		promt3_reg->INTEN |= ACPI_INTEN_OBFINTEN;
		break;
	case ECUSTOM_HOST_CMD_PM3_PAUSE_OBE_IRQ:
		promt3_reg->INTEN &= ~ACPI_INTEN_OBFINTEN;
		break;
	default:
		return -EINVAL;
	}

	return 0;
#else
	return -ENOTSUP;
#endif
}

static int espi_rts5918_write_lpc_request(const struct device *dev, enum lpc_peripheral_opcode op,
					  uint32_t *data)
{
	const struct espi_rts5918_config *const espi_config = dev->config;

	if (op >= E8042_START_OPCODE && op <= E8042_MAX_OPCODE) {
		return lpc_request_write_8042(dev, op, data);
	} else if (op >= EACPI_START_OPCODE && op <= EACPI_MAX_OPCODE) {
		return lpc_request_write_acpi(espi_config, op, data);
	} else if (op >= ECUSTOM_START_OPCODE && op <= ECUSTOM_MAX_OPCODE) {
		return lpc_request_write_custom(espi_config, op, data);
	} else {
		return -ENOTSUP;
	}
}

static void espi_periph_ch_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;

	espi_reg->EPINTEN = ESPI_EPINTEN_CFGCHGEN | ESPI_EPINTEN_MEMWREN | ESPI_EPINTEN_MEMRDEN;
#ifdef CONFIG_ESPI_RTS5918_IOSHORT_RW
	espi_reg->EPINTEN |= ESPI_EPINTEN_WR_HOLE_EN;
#endif

	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), periph_ch, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), periph_ch, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), periph_ch, priority), espi_periph_ch_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), periph_ch, irq));
}

#endif /* CONFIG_ESPI_PERIPHERAL_CHANNEL */

/*
 * =========================================================================
 * ESPI Peripheral Debug Port 80
 * =========================================================================
 */

#ifdef CONFIG_ESPI_PERIPHERAL_DEBUG_PORT_80

static void espi_port80_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	struct espi_event evt = {ESPI_BUS_PERIPHERAL_NOTIFICATION,
				 ESPI_PERIPHERAL_INDEX_0 << 16 | ESPI_PERIPHERAL_DEBUG_PORT80,
				 ESPI_PERIPHERAL_NODATA};
	volatile struct port80_reg *const port80_reg = espi_config->port80_reg;
	uint8_t port80_write_ptr = (port80_reg->STS & PORT80_STS_WRPTR_MSK) >> PORT80_STS_WRPTR_POS;
	uint8_t port80_read_ptr = (port80_reg->STS & PORT80_STS_RDPTR_MSK) >> PORT80_STS_RDPTR_POS;

	while(port80_read_ptr != port80_write_ptr)
	{
		evt.evt_data = port80_reg->DATA;
		espi_send_callbacks(&espi_data->callbacks, dev, evt);
		port80_read_ptr = (port80_reg->STS & PORT80_STS_RDPTR_MSK) >> PORT80_STS_RDPTR_POS;
	}
}

static int espi_peri_ch_port80_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct rts5918_sccon_subsys sccon;
	volatile struct port80_reg *const port80_reg = espi_config->port80_reg;
	int rc;

	if (!device_is_ready(espi_config->clk_dev)) {
		return -ENODEV;
	}

	sccon.clk_grp = espi_config->port80_clk_grp;
	sccon.clk_idx = espi_config->port80_clk_idx;

	rc = clock_control_on(espi_config->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc != 0) {
		return rc;
	}

	port80_reg->ADDR = 0x80UL;
	port80_reg->CFG = PORT80_CFG_CLRFLG | PORT80_CFG_THREEN;
	port80_reg->INTEN = PORT80_INTEN_THREINTEN;

	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), port80, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), port80, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), port80, priority), espi_port80_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), port80, irq));

	return 0;
}

#endif /* CONFIG_ESPI_PERIPHERAL_DEBUG_PORT_80 */

/*
 * =========================================================================
 * ESPI VWIRE channel
 * =========================================================================
 */

#ifdef CONFIG_ESPI_VWIRE_CHANNEL

#define VW_CH_IDX2  0x02UL
#define VW_CH_IDX3  0x03UL
#define VW_CH_IDX4  0x04UL
#define VW_CH_IDX5  0x05UL
#define VW_CH_IDX6  0x06UL
#define VW_CH_IDX7  0x07UL
#define VW_CH_IDX40 0x40UL
#define VW_CH_IDX41 0x41UL
#define VW_CH_IDX42 0x42UL
#define VW_CH_IDX43 0x43UL
#define VW_CH_IDX44 0x44UL
#define VW_CH_IDX47 0x47UL
#define VW_CH_IDX4A 0x4AUL
#define VW_CH_IDX51 0x51UL
#define VW_CH_IDX61 0x61UL

struct espi_vw_channel {
	uint8_t vw_index;
	uint8_t level_mask;
	uint8_t valid_mask;
	uint8_t active_high;
};

struct espi_vw_signal_t {
	enum espi_vwire_signal signal;
	void (*vw_signal_callback)(const struct device *dev);
};

#define VW_CH(signal, index, level, valid, activeInHigh)                                                         \
	[signal] = {.vw_index = index, .level_mask = level, .valid_mask = valid, .active_high = activeInHigh}

static const struct espi_vw_channel vw_channel_list[] = {
	VW_CH(ESPI_VWIRE_SIGNAL_SLP_S3, VW_CH_IDX2, BIT(0), BIT(4), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_SLP_S4, VW_CH_IDX2, BIT(1), BIT(5), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_SLP_S5, VW_CH_IDX2, BIT(2), BIT(6), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_OOB_RST_WARN, VW_CH_IDX3, BIT(2), BIT(6), 1),
	VW_CH(ESPI_VWIRE_SIGNAL_PLTRST, VW_CH_IDX3, BIT(1), BIT(5), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_SUS_STAT, VW_CH_IDX3, BIT(0), BIT(4), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_NMIOUT, VW_CH_IDX7, BIT(2), BIT(6), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_SMIOUT, VW_CH_IDX7, BIT(1), BIT(5), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_HOST_RST_WARN, VW_CH_IDX7, BIT(0), BIT(4), 1),
	VW_CH(ESPI_VWIRE_SIGNAL_SLP_A, VW_CH_IDX41, BIT(3), BIT(7), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_SUS_PWRDN_ACK, VW_CH_IDX41, BIT(1), BIT(5), 1),
	VW_CH(ESPI_VWIRE_SIGNAL_SUS_WARN, VW_CH_IDX41, BIT(0), BIT(4), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_SLP_WLAN, VW_CH_IDX42, BIT(1), BIT(5), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_SLP_LAN, VW_CH_IDX42, BIT(0), BIT(4), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_HOST_C10, VW_CH_IDX47, BIT(0), BIT(4), 1),
	VW_CH(ESPI_VWIRE_SIGNAL_DNX_WARN, VW_CH_IDX4A, BIT(1), BIT(5), 1),
	VW_CH(ESPI_VWIRE_SIGNAL_PME, VW_CH_IDX4, BIT(3), BIT(7), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_WAKE, VW_CH_IDX4, BIT(2), BIT(6), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_OOB_RST_ACK, VW_CH_IDX4, BIT(0), BIT(4), 1),
	VW_CH(ESPI_VWIRE_SIGNAL_TARGET_BOOT_STS, VW_CH_IDX5, BIT(3), BIT(7),1 ),
	VW_CH(ESPI_VWIRE_SIGNAL_ERR_NON_FATAL, VW_CH_IDX5, BIT(2), BIT(6), 1),
	VW_CH(ESPI_VWIRE_SIGNAL_ERR_FATAL, VW_CH_IDX5, BIT(1), BIT(5), 1),
	VW_CH(ESPI_VWIRE_SIGNAL_TARGET_BOOT_DONE, VW_CH_IDX5, BIT(0), BIT(4), 1),
	VW_CH(ESPI_VWIRE_SIGNAL_HOST_RST_ACK, VW_CH_IDX6, BIT(3), BIT(7), 1),
	VW_CH(ESPI_VWIRE_SIGNAL_RST_CPU_INIT, VW_CH_IDX6, BIT(2), BIT(6), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_SMI, VW_CH_IDX6, BIT(1), BIT(5), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_SCI, VW_CH_IDX6, BIT(0), BIT(4), 0),
	VW_CH(ESPI_VWIRE_SIGNAL_DNX_ACK, VW_CH_IDX40, BIT(1), BIT(5), 1),
	VW_CH(ESPI_VWIRE_SIGNAL_SUS_ACK, VW_CH_IDX40, BIT(0), BIT(4), 0),
};

struct espi_vw_ch_cached {
	uint8_t idx2;
	uint8_t idx3;
	uint8_t idx7;
	uint8_t idx41;
	uint8_t idx42;
	uint8_t idx43;
	uint8_t idx44;
	uint8_t idx47;
	uint8_t idx4a;
	uint8_t idx51;
	uint8_t idx61;
};

struct espi_vw_tx_cached {
	uint8_t idx4;
	uint8_t idx5;
	uint8_t idx6;
	uint8_t idx40;
};

static struct espi_vw_ch_cached espi_vw_ch_cached_data;
static struct espi_vw_tx_cached espi_vw_tx_cached_data;

static int espi_rts5918_send_vwire(const struct device *dev, enum espi_vwire_signal signal,
				   uint8_t level);
static int espi_rts5918_receive_vwire(const struct device *dev, enum espi_vwire_signal signal,
				      uint8_t *level);
static int vw_signal_set_valid(const struct device *dev, enum espi_vwire_signal signal,
			       uint8_t valid);
static void notify_system_state(const struct device *dev, enum espi_vwire_signal signal);
static void notify_host_warning(const struct device *dev, enum espi_vwire_signal signal);
static void vw_slp3_handler(const struct device *dev);
static void vw_slp4_handler(const struct device *dev);
static void vw_slp5_handler(const struct device *dev);
static void vw_sus_stat_handler(const struct device *dev);
static void vw_pltrst_handler(const struct device *dev);
static void vw_oob_rst_warn_handler(const struct device *dev);
static void vw_host_rst_warn_handler(const struct device *dev);
static void vw_smiout_handler(const struct device *dev);
static void vw_nmiout_handler(const struct device *dev);
static void vw_sus_warn_handler(const struct device *dev);
static void vw_sus_pwrdn_ack_handler(const struct device *dev);
static void vw_sus_slp_a_handler(const struct device *dev);
static void vw_slp_lan_handler(const struct device *dev);
static void vw_slp_wlan_handler(const struct device *dev);
static void vw_host_c10_handler(const struct device *dev);
static void espi_vw_ch_setup(const struct device *dev);
static void espi_vw_ch_isr(const struct device *dev);
#ifdef CONFIG_ESPI_AUTOMATIC_BOOT_DONE_ACKNOWLEDGE
static void send_target_bootdone(const struct device *dev);
#endif

static void espi_vw_ch_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *data = dev->data;

	struct espi_event evt = {.evt_type = ESPI_BUS_EVENT_CHANNEL_READY,
				 .evt_details = ESPI_CHANNEL_VWIRE,
				 .evt_data = 0};

	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;

	uint32_t config = espi_reg->EVCFG;

	evt.evt_data = (config & ESPI_EVCFG_CHEN) ? 1 : 0;
	espi_send_callbacks(&data->callbacks, dev, evt);

	// if (config & ESPI_EVCFG_CHEN) {
		// 	vw_signal_set_valid(dev, ESPI_VWIRE_SIGNAL_TARGET_BOOT_STS, 1);
		// 	vw_signal_set_valid(dev, ESPI_VWIRE_SIGNAL_TARGET_BOOT_DONE, 1);

// #ifdef CONFIG_ESPI_AUTOMATIC_BOOT_DONE_ACKNOWLEDGE
		// 		send_target_bootdone(dev);
// #endif
	// }
	espi_reg->EVSTS = ESPI_EVSTS_RXIDXCHG;
}

static const struct espi_vw_signal_t vw_idx2_signals[] = {
	{ESPI_VWIRE_SIGNAL_SLP_S3, vw_slp3_handler},
	{ESPI_VWIRE_SIGNAL_SLP_S4, vw_slp4_handler},
	{ESPI_VWIRE_SIGNAL_SLP_S5, vw_slp5_handler},
};

static void espi_vw_idx2_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint8_t cur_idx_data = espi_reg->EVIDX2;
	uint8_t updated_bit = cur_idx_data ^ espi_vw_ch_cached_data.idx2;

	if (espi_reg->EVSTS & ESPI_EVSTS_IDX2CHG) {
		espi_vw_ch_cached_data.idx2 = cur_idx_data;

		for (int i = 0; i < ARRAY_SIZE(vw_idx2_signals); i++) {
			enum espi_vwire_signal vw_signal = vw_idx2_signals[i].signal;

			if (updated_bit & vw_channel_list[vw_signal].level_mask &&
			    vw_idx2_signals[i].vw_signal_callback != NULL) {
				vw_idx2_signals[i].vw_signal_callback(dev);
			}
		}
		if (espi_vw_ch_cached_data.idx2 == espi_reg->EVIDX2) {
			espi_reg->EVSTS = ESPI_EVSTS_IDX2CHG;
		}
	}
}

static const struct espi_vw_signal_t vw_idx3_signals[] = {
	{ESPI_VWIRE_SIGNAL_SUS_STAT, vw_sus_stat_handler},
	{ESPI_VWIRE_SIGNAL_PLTRST, vw_pltrst_handler},
	{ESPI_VWIRE_SIGNAL_OOB_RST_WARN, vw_oob_rst_warn_handler},
};

static void espi_vw_idx3_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint8_t cur_idx_data = espi_reg->EVIDX3;
	uint8_t updated_bit = cur_idx_data ^ espi_vw_ch_cached_data.idx3;

	if (espi_reg->EVSTS & ESPI_EVSTS_IDX3CHG) {
		espi_vw_ch_cached_data.idx3 = cur_idx_data;

		for (int i = 0; i < ARRAY_SIZE(vw_idx3_signals); i++) {
			enum espi_vwire_signal vw_signal = vw_idx3_signals[i].signal;

			if (updated_bit & vw_channel_list[vw_signal].level_mask &&
			    vw_idx3_signals[i].vw_signal_callback != NULL) {
				vw_idx3_signals[i].vw_signal_callback(dev);
			}
		}
		if (espi_vw_ch_cached_data.idx3 == espi_reg->EVIDX3) {
			espi_reg->EVSTS = ESPI_EVSTS_IDX3CHG;
		}
	}
}

static const struct espi_vw_signal_t vw_idx7_signals[] = {
	{ESPI_VWIRE_SIGNAL_HOST_RST_WARN, vw_host_rst_warn_handler},
	{ESPI_VWIRE_SIGNAL_SMIOUT, vw_smiout_handler},
	{ESPI_VWIRE_SIGNAL_NMIOUT, vw_nmiout_handler},
};

static void espi_vw_idx7_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint8_t cur_idx_data = espi_reg->EVIDX7;
	uint8_t updated_bit = cur_idx_data ^ espi_vw_ch_cached_data.idx7;

	if (espi_reg->EVSTS & ESPI_EVSTS_IDX7CHG) {
		espi_vw_ch_cached_data.idx7 = cur_idx_data;

		for (int i = 0; i < ARRAY_SIZE(vw_idx7_signals); i++) {
			enum espi_vwire_signal vw_signal = vw_idx7_signals[i].signal;

			if (updated_bit & vw_channel_list[vw_signal].level_mask &&
			    vw_idx7_signals[i].vw_signal_callback != NULL) {
				vw_idx7_signals[i].vw_signal_callback(dev);
			}
		}
		if (espi_vw_ch_cached_data.idx7 == espi_reg->EVIDX7) {
			espi_reg->EVSTS = ESPI_EVSTS_IDX7CHG;
		}
	}
}

static const struct espi_vw_signal_t vw_idx41_signals[] = {
	{ESPI_VWIRE_SIGNAL_SUS_WARN, vw_sus_warn_handler},
	{ESPI_VWIRE_SIGNAL_SUS_PWRDN_ACK, vw_sus_pwrdn_ack_handler},
	{ESPI_VWIRE_SIGNAL_SLP_A, vw_sus_slp_a_handler},
};

static void espi_vw_idx41_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint8_t cur_idx_data = espi_reg->EVIDX41;
	uint8_t updated_bit = cur_idx_data ^ espi_vw_ch_cached_data.idx41;

	if (espi_reg->EVSTS & ESPI_EVSTS_IDX41CHG) {
		espi_vw_ch_cached_data.idx41 = cur_idx_data;
		for (int i = 0; i < ARRAY_SIZE(vw_idx41_signals); i++) {
			enum espi_vwire_signal vw_signal = vw_idx41_signals[i].signal;

			if (updated_bit & vw_channel_list[vw_signal].level_mask &&
			    vw_idx41_signals[i].vw_signal_callback != NULL) {
				vw_idx41_signals[i].vw_signal_callback(dev);
			}
		}
		if (espi_vw_ch_cached_data.idx41 == espi_reg->EVIDX41) {
			espi_reg->EVSTS = ESPI_EVSTS_IDX41CHG;
		}
	}
}

static const struct espi_vw_signal_t vw_idx42_signals[] = {
	{ESPI_VWIRE_SIGNAL_SLP_LAN, vw_slp_lan_handler},
	{ESPI_VWIRE_SIGNAL_SLP_WLAN, vw_slp_wlan_handler},
};

static void espi_vw_idx42_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint8_t cur_idx_data = espi_reg->EVIDX42;
	uint8_t updated_bit = cur_idx_data ^ espi_vw_ch_cached_data.idx42;

	if (espi_reg->EVSTS & ESPI_EVSTS_IDX42CHG) {
		espi_vw_ch_cached_data.idx42 = cur_idx_data;

		for (int i = 0; i < ARRAY_SIZE(vw_idx42_signals); i++) {
			enum espi_vwire_signal vw_signal = vw_idx42_signals[i].signal;

			if (updated_bit & vw_channel_list[vw_signal].level_mask &&
			    vw_idx42_signals[i].vw_signal_callback != NULL) {
				vw_idx42_signals[i].vw_signal_callback(dev);
			}
		}
		if (espi_vw_ch_cached_data.idx42 == espi_reg->EVIDX42) {
			espi_reg->EVSTS = ESPI_EVSTS_IDX42CHG;
		}
	}
}

static const struct espi_vw_signal_t vw_idx43_signals[] = {};

static void espi_vw_idx43_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint8_t cur_idx_data = espi_reg->EVIDX43;
	uint8_t updated_bit = cur_idx_data ^ espi_vw_ch_cached_data.idx43;

	if (espi_reg->EVSTS & ESPI_EVSTS_IDX43CHG) {
		espi_vw_ch_cached_data.idx43 = cur_idx_data;

		for (int i = 0; i < ARRAY_SIZE(vw_idx43_signals); i++) {
			enum espi_vwire_signal vw_signal = vw_idx43_signals[i].signal;

			if (updated_bit & vw_channel_list[vw_signal].level_mask &&
			    vw_idx43_signals[i].vw_signal_callback != NULL) {
				vw_idx43_signals[i].vw_signal_callback(dev);
			}
		}
		if (espi_vw_ch_cached_data.idx43 == espi_reg->EVIDX43) {
			espi_reg->EVSTS = ESPI_EVSTS_IDX43CHG;
		}
	}
}

static const struct espi_vw_signal_t vw_idx44_signals[] = {};

static void espi_vw_idx44_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint8_t cur_idx_data = espi_reg->EVIDX44;
	uint8_t updated_bit = cur_idx_data ^ espi_vw_ch_cached_data.idx44;

	if (espi_reg->EVSTS & ESPI_EVSTS_IDX44CHG) {
		espi_vw_ch_cached_data.idx44 = cur_idx_data;

		for (int i = 0; i < ARRAY_SIZE(vw_idx44_signals); i++) {
			enum espi_vwire_signal vw_signal = vw_idx44_signals[i].signal;

			if (updated_bit & vw_channel_list[vw_signal].level_mask &&
			    vw_idx44_signals[i].vw_signal_callback != NULL) {
				vw_idx44_signals[i].vw_signal_callback(dev);
			}
		}
		if (espi_vw_ch_cached_data.idx44 == espi_reg->EVIDX44) {
			espi_reg->EVSTS = ESPI_EVSTS_IDX44CHG;
		}
	}
}

static const struct espi_vw_signal_t vw_idx47_signals[] = {
	{ESPI_VWIRE_SIGNAL_HOST_C10, vw_host_c10_handler},
};

static void espi_vw_idx47_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint8_t cur_idx_data = espi_reg->EVIDX47;
	uint8_t updated_bit = cur_idx_data ^ espi_vw_ch_cached_data.idx47;

	if (espi_reg->EVSTS & ESPI_EVSTS_IDX47CHG) {
		espi_vw_ch_cached_data.idx47 = cur_idx_data;

		for (int i = 0; i < ARRAY_SIZE(vw_idx47_signals); i++) {
			enum espi_vwire_signal vw_signal = vw_idx47_signals[i].signal;

			if (updated_bit & vw_channel_list[vw_signal].level_mask &&
			    vw_idx47_signals[i].vw_signal_callback != NULL) {
				vw_idx47_signals[i].vw_signal_callback(dev);
			}
		}
		if (espi_vw_ch_cached_data.idx47 == espi_reg->EVIDX47) {
			espi_reg->EVSTS = ESPI_EVSTS_IDX47CHG;
		}
	}
}

static void espi_vw_idx4a_isr(const struct device *dev)
{
	ARG_UNUSED(dev);
}

static void espi_vw_idx51_isr(const struct device *dev)
{
	ARG_UNUSED(dev);
}

static void espi_vw_idx61_isr(const struct device *dev)
{
	ARG_UNUSED(dev);
}

static int vw_signal_set_valid(const struct device *dev, enum espi_vwire_signal signal,
			       uint8_t valid)
{
	uint8_t vw_index = vw_channel_list[signal].vw_index;
	uint8_t valid_mask = vw_channel_list[signal].valid_mask;

	if (signal > ARRAY_SIZE(vw_channel_list)) {
		return -EIO;
	}

	switch (vw_index) {
	case VW_CH_IDX4:
		if (valid) {
			espi_vw_tx_cached_data.idx4 |= valid_mask;
		} else {
			espi_vw_tx_cached_data.idx4 &= ~valid_mask;
		}
		break;
	case VW_CH_IDX5:
		if (valid) {
			espi_vw_tx_cached_data.idx5 |= valid_mask;
		} else {
			espi_vw_tx_cached_data.idx5 &= ~valid_mask;
		}
		break;
	case VW_CH_IDX6:
		if (valid) {
			espi_vw_tx_cached_data.idx6 |= valid_mask;
		} else {
			espi_vw_tx_cached_data.idx6 &= ~valid_mask;
		}
		break;
	case VW_CH_IDX40:
		if (valid) {
			espi_vw_tx_cached_data.idx40 |= valid_mask;
		} else {
			espi_vw_tx_cached_data.idx40 &= ~valid_mask;
		}
		break;
	default:
		return -EIO;
	}

	return 0;
}

static void espi_flash_chg_isr(const struct device *dev);

static void vw_ch_isr_wa_cb(struct k_work *work)
{
	espi_flash_chg_isr(DEVICE_DT_GET(DT_DRV_INST(0)));
}
static K_WORK_DELAYABLE_DEFINE(vw_ch_isr_wa, vw_ch_isr_wa_cb);

#ifdef CONFIG_ESPI_AUTOMATIC_BOOT_DONE_ACKNOWLEDGE
static void send_target_bootdone(const struct device *dev)
{
	int ret;
	uint8_t boot_done;

	ret = espi_rts5918_receive_vwire(dev, ESPI_VWIRE_SIGNAL_TARGET_BOOT_DONE, &boot_done);
	if (!ret && !boot_done) {
		espi_rts5918_send_vwire(dev, ESPI_VWIRE_SIGNAL_TARGET_BOOT_STS, 1);
		espi_rts5918_send_vwire(dev, ESPI_VWIRE_SIGNAL_TARGET_BOOT_DONE, 1);
		k_work_cancel_delayable(&vw_ch_isr_wa);
	}
}
#endif

static void notify_system_state(const struct device *dev, enum espi_vwire_signal signal)
{
	struct espi_rts5918_data *data = dev->data;
	struct espi_event evt = {ESPI_BUS_EVENT_VWIRE_RECEIVED, 0, 0};

	uint8_t status = 0;

	espi_rts5918_receive_vwire(dev, signal, &status);

	evt.evt_details = signal;
	evt.evt_data = status;
	espi_send_callbacks(&data->callbacks, dev, evt);
}

static void notify_host_warning(const struct device *dev, enum espi_vwire_signal signal)
{
	uint8_t status = 0;

	espi_rts5918_receive_vwire(dev, signal, &status);
	//k_busy_wait(200);

	switch (signal) {
	case ESPI_VWIRE_SIGNAL_SUS_WARN:
		vw_signal_set_valid(dev, ESPI_VWIRE_SIGNAL_SUS_ACK, 1);
		espi_rts5918_send_vwire(dev, ESPI_VWIRE_SIGNAL_SUS_ACK, status);
		break;
	case ESPI_VWIRE_SIGNAL_OOB_RST_WARN:
		espi_rts5918_send_vwire(dev, ESPI_VWIRE_SIGNAL_OOB_RST_ACK, status);
		break;
	case ESPI_VWIRE_SIGNAL_HOST_RST_WARN:
		espi_rts5918_send_vwire(dev, ESPI_VWIRE_SIGNAL_HOST_RST_ACK, status);
		break;
	default:
		break;
	}
}

static void vw_slp3_handler(const struct device *dev)
{
	notify_system_state(dev, ESPI_VWIRE_SIGNAL_SLP_S3);
}

static void vw_slp4_handler(const struct device *dev)
{
	notify_system_state(dev, ESPI_VWIRE_SIGNAL_SLP_S4);
}

static void vw_slp5_handler(const struct device *dev)
{
	notify_system_state(dev, ESPI_VWIRE_SIGNAL_SLP_S5);
}

static void vw_sus_stat_handler(const struct device *dev)
{
	notify_system_state(dev, ESPI_VWIRE_SIGNAL_SUS_STAT);
}

static void vw_pltrst_handler(const struct device *dev)
{
	struct espi_rts5918_data *data = dev->data;

	struct espi_event evt = {ESPI_BUS_EVENT_VWIRE_RECEIVED, ESPI_VWIRE_SIGNAL_PLTRST, 0};
	uint8_t status = 0;

	espi_rts5918_receive_vwire(dev, ESPI_VWIRE_SIGNAL_PLTRST, &status);

	if (status) {
		vw_signal_set_valid(dev, ESPI_VWIRE_SIGNAL_SMI, 1);
		vw_signal_set_valid(dev, ESPI_VWIRE_SIGNAL_SCI, 1);
		vw_signal_set_valid(dev, ESPI_VWIRE_SIGNAL_HOST_RST_ACK, 1);
		vw_signal_set_valid(dev, ESPI_VWIRE_SIGNAL_RST_CPU_INIT, 1);

		espi_rts5918_send_vwire(dev, ESPI_VWIRE_SIGNAL_SMI, 1);
		espi_rts5918_send_vwire(dev, ESPI_VWIRE_SIGNAL_SCI, 1);
		espi_rts5918_send_vwire(dev, ESPI_VWIRE_SIGNAL_HOST_RST_ACK, 0);
		espi_rts5918_send_vwire(dev, ESPI_VWIRE_SIGNAL_RST_CPU_INIT, 1);
	}

	evt.evt_data = status;
	espi_send_callbacks(&data->callbacks, dev, evt);
}

static void vw_oob_rst_warn_handler(const struct device *dev)
{
	if (!IS_ENABLED(CONFIG_ESPI_AUTOMATIC_WARNING_ACKNOWLEDGE)) {
		notify_system_state(dev, ESPI_VWIRE_SIGNAL_OOB_RST_WARN);
	} else {
		notify_host_warning(dev, ESPI_VWIRE_SIGNAL_OOB_RST_WARN);
	}
}

static void vw_host_rst_warn_handler(const struct device *dev)
{
	if (!IS_ENABLED(CONFIG_ESPI_AUTOMATIC_WARNING_ACKNOWLEDGE)) {
		notify_system_state(dev, ESPI_VWIRE_SIGNAL_HOST_RST_WARN);
	} else {
		notify_host_warning(dev, ESPI_VWIRE_SIGNAL_HOST_RST_WARN);
	}
}

static void vw_smiout_handler(const struct device *dev)
{
	notify_system_state(dev, ESPI_VWIRE_SIGNAL_SMIOUT);
}

static void vw_nmiout_handler(const struct device *dev)
{
	notify_system_state(dev, ESPI_VWIRE_SIGNAL_NMIOUT);
}

static void vw_sus_warn_handler(const struct device *dev)
{
	if (!IS_ENABLED(CONFIG_ESPI_AUTOMATIC_WARNING_ACKNOWLEDGE)) {
		notify_system_state(dev, ESPI_VWIRE_SIGNAL_SUS_WARN);
	} else {
		notify_host_warning(dev, ESPI_VWIRE_SIGNAL_SUS_WARN);
	}
}

static void vw_sus_pwrdn_ack_handler(const struct device *dev)
{
	notify_system_state(dev, ESPI_VWIRE_SIGNAL_SUS_PWRDN_ACK);
}

static void vw_sus_slp_a_handler(const struct device *dev)
{
	notify_system_state(dev, ESPI_VWIRE_SIGNAL_SLP_A);
}

static void vw_slp_lan_handler(const struct device *dev)
{
	notify_system_state(dev, ESPI_VWIRE_SIGNAL_SLP_LAN);
}

static void vw_slp_wlan_handler(const struct device *dev)
{
	notify_system_state(dev, ESPI_VWIRE_SIGNAL_SLP_WLAN);
}

static void vw_host_c10_handler(const struct device *dev)
{
	notify_system_state(dev, ESPI_VWIRE_SIGNAL_HOST_C10);
}

#define VW_TIMEOUT_US 1024

static int espi_rts5918_send_vwire(const struct device *dev, enum espi_vwire_signal signal,
				   uint8_t level)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;

	uint8_t vw_idx = vw_channel_list[signal].vw_index;
	uint8_t lev_msk = vw_channel_list[signal].level_mask;
	uint32_t rd_cnt = VW_TIMEOUT_US;
	uint32_t tx_data = 0;

	if (signal > ARRAY_SIZE(vw_channel_list)) {
		return -EIO;
	}

	switch (vw_idx) {
	case VW_CH_IDX4:
		tx_data = espi_vw_tx_cached_data.idx4;
		break;
	case VW_CH_IDX5:
		tx_data = espi_vw_tx_cached_data.idx5;
		break;
	case VW_CH_IDX6:
		tx_data = espi_vw_tx_cached_data.idx6;
		break;
	case VW_CH_IDX40:
		tx_data = espi_vw_tx_cached_data.idx40;
		break;
	default:
		return -EIO;
	}

	tx_data |= (vw_idx << 8);

	if (level) {
		tx_data |= lev_msk;
	} else {
		tx_data &= ~lev_msk;
	}

	if (espi_reg->EVSTS & ESPI_EVSTS_TXFULL) {
		return -EIO;
	}

	espi_reg->EVTXDAT = tx_data;
	k_busy_wait(10);
	while (!(espi_reg->EVSTS & ESPI_EVSTS_TXEMPTY) && rd_cnt--) {
		// k_busy_wait(10);
	}

	switch (vw_idx) {
	case VW_CH_IDX4:
		espi_vw_tx_cached_data.idx4 = tx_data;
		break;
	case VW_CH_IDX5:
		espi_vw_tx_cached_data.idx5 = tx_data;
		break;
	case VW_CH_IDX6:
		espi_vw_tx_cached_data.idx6 = tx_data;
		break;
	case VW_CH_IDX40:
		espi_vw_tx_cached_data.idx40 = tx_data;
		break;
	default:
		return -EIO;
	}

	return 0;
}

static int espi_rts5918_receive_vwire(const struct device *dev, enum espi_vwire_signal signal,
				      uint8_t *level)
{
	ARG_UNUSED(dev);
	uint8_t vw_idx, lev_msk, valid_msk;
	uint8_t vw_data;
uint8_t active_h;

	vw_idx = vw_channel_list[signal].vw_index;
	lev_msk = vw_channel_list[signal].level_mask;
	valid_msk = vw_channel_list[signal].valid_mask;
active_h = vw_channel_list[signal].active_high;

	if (signal > ARRAY_SIZE(vw_channel_list)) {
		return -EIO;
	}

	switch (vw_idx) {
	case VW_CH_IDX2:
		vw_data = espi_vw_ch_cached_data.idx2;
		break;
	case VW_CH_IDX3:
		// if (espi_vw_ch_cached_data.idx3 != espi_reg->EVIDX3) {
			// 	espi_vw_ch_cached_data.idx3 = espi_reg->EVIDX3;
		// }
		vw_data = espi_vw_ch_cached_data.idx3;
		break;
	case VW_CH_IDX4:
		vw_data = espi_vw_tx_cached_data.idx4;
		break;
	case VW_CH_IDX5:
		vw_data = espi_vw_tx_cached_data.idx5;
		break;
	case VW_CH_IDX6:
		vw_data = espi_vw_tx_cached_data.idx6;
		break;
	case VW_CH_IDX7:
		// if (espi_vw_ch_cached_data.idx7 != espi_reg->EVIDX7) {
			// 	espi_vw_ch_cached_data.idx7 = espi_reg->EVIDX7;
		// }
		vw_data = espi_vw_ch_cached_data.idx7;
		break;
	case VW_CH_IDX40:
		vw_data = espi_vw_tx_cached_data.idx40;
		break;
	case VW_CH_IDX41:
		// if (espi_vw_ch_cached_data.idx41 != espi_reg->EVIDX41) {
			// 	espi_vw_ch_cached_data.idx41 = espi_reg->EVIDX41;
		// }
		vw_data = espi_vw_ch_cached_data.idx41;
		break;
	case VW_CH_IDX42:
		// if (espi_vw_ch_cached_data.idx42 != espi_reg->EVIDX42) {
			// 	espi_vw_ch_cached_data.idx42 = espi_reg->EVIDX42;
		// }
		vw_data = espi_vw_ch_cached_data.idx42;
		break;
	case VW_CH_IDX43:
		// if (espi_vw_ch_cached_data.idx43 != espi_reg->EVIDX43) {
			// 	espi_vw_ch_cached_data.idx43 = espi_reg->EVIDX43;
		// }
		vw_data = espi_vw_ch_cached_data.idx43;
		break;
	case VW_CH_IDX44:
		// if (espi_vw_ch_cached_data.idx44 != espi_reg->EVIDX44) {
			// 	espi_vw_ch_cached_data.idx44 = espi_reg->EVIDX44;
		// }
		vw_data = espi_vw_ch_cached_data.idx44;
		break;
	case VW_CH_IDX47:
		// if (espi_vw_ch_cached_data.idx47 != espi_reg->EVIDX47) {
			// 	espi_vw_ch_cached_data.idx47 = espi_reg->EVIDX47;
		// }
		vw_data = espi_vw_ch_cached_data.idx47;
		break;
	case VW_CH_IDX4A:
		// if (espi_vw_ch_cached_data.idx4a != espi_reg->EVIDX4A) {
			// 	espi_vw_ch_cached_data.idx4a = espi_reg->EVIDX4A;
		// }
		vw_data = espi_vw_ch_cached_data.idx4a;
		break;
	case VW_CH_IDX51:
		// if (espi_vw_ch_cached_data.idx51 != espi_reg->EVIDX51) {
			// 	espi_vw_ch_cached_data.idx51 = espi_reg->EVIDX51;
		// }
		vw_data = espi_vw_ch_cached_data.idx51;
		break;
	case VW_CH_IDX61:
		// if (espi_vw_ch_cached_data.idx61 != espi_reg->EVIDX61) {
			// 	espi_vw_ch_cached_data.idx61 = espi_reg->EVIDX61;
		// }
		vw_data = espi_vw_ch_cached_data.idx61;
		break;
	default:
		return -EIO;
	}

			//if (vw_data & valid_msk) {
			*level = !!(vw_data & lev_msk);
		//}else{
		//*level = active_h?1:0;
	//}
	// else {
			// 	/* Not valid */
			// 	LOG_ERR("Mask NOT valid");
	// 	*level = 0;
		// }

	return 0;
}

static void espi_vw_ch_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;

	espi_reg->EVSTS |= ESPI_EVSTS_RXIDXCLR;

	espi_vw_ch_cached_data.idx2 = espi_reg->EVIDX2;
	espi_vw_ch_cached_data.idx3 = espi_reg->EVIDX3;
	espi_vw_ch_cached_data.idx7 = espi_reg->EVIDX7;
	espi_vw_ch_cached_data.idx41 = espi_reg->EVIDX41;
	espi_vw_ch_cached_data.idx42 = espi_reg->EVIDX42;
	espi_vw_ch_cached_data.idx43 = espi_reg->EVIDX43;
	espi_vw_ch_cached_data.idx44 = espi_reg->EVIDX44;
	espi_vw_ch_cached_data.idx47 = espi_reg->EVIDX47;
	espi_vw_ch_cached_data.idx4a = espi_reg->EVIDX4A;
	espi_vw_ch_cached_data.idx51 = espi_reg->EVIDX51;
	espi_vw_ch_cached_data.idx61 = espi_reg->EVIDX61;

	espi_vw_tx_cached_data.idx4 = 0;
	espi_vw_tx_cached_data.idx5 = 0;
	espi_vw_tx_cached_data.idx6 = 0;
	espi_vw_tx_cached_data.idx40 = 0;

	espi_reg->EVRXINTEN = (ESPI_EVRXINTEN_CFGCHGEN | ESPI_EVRXINTEN_RXCHGEN | ESPI_EVRXINTEN_RXVWEN);

	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_ch, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx2, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx3, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx7, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx41, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx42, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx43, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx44, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx47, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx4a, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx51, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx61, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_ch, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_ch, priority), espi_vw_ch_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_ch, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx2, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx2, priority), espi_vw_idx2_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx2, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx3, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx3, priority), espi_vw_idx3_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx3, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx7, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx7, priority), espi_vw_idx7_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx7, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx41, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx41, priority), espi_vw_idx41_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx41, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx42, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx42, priority), espi_vw_idx42_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx42, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx43, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx43, priority), espi_vw_idx43_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx43, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx44, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx44, priority), espi_vw_idx44_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx44, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx47, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx47, priority), espi_vw_idx47_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx47, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx4a, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx4a, priority), espi_vw_idx4a_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx4a, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx51, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx51, priority), espi_vw_idx51_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx51, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx61, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx61, priority), espi_vw_idx61_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), vw_idx61, irq));
}

#ifdef CONFIG_ESPI_PERIPHERAL_8042_KBC

#define ESPI_VW_EVENT_IDLE_TIMEOUT_US     1024UL
#define ESPI_VW_EVENT_COMPLETE_TIMEOUT_US 10000UL

static int espi_send_vw_event(uint8_t index, uint8_t data, const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	uint16_t retry_counter = ESPI_VW_EVENT_IDLE_TIMEOUT_US;
	uint32_t i;
	uint32_t timeout_cnt = 0;

	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;

	if ((espi_reg->EVCFG & ESPI_EVCFG_CHEN) == 0 || (espi_reg->EVCFG & ESPI_EVCFG_CHRDY) == 0) {
		return -EBUSY;
	}

	/* Wait for TX FIFO to not be full before writing, with timeout */
	while (espi_reg->EVSTS & ESPI_EVSTS_TXFULL) {
		timeout_cnt++;
		if (timeout_cnt >= ESPI_VW_EVENT_COMPLETE_TIMEOUT_US) {
			return -EBUSY;
		}
	}

	__disable_irq();
	i = 0x0000FFFF & ((uint32_t)index << 8 | (uint32_t)data);
	espi_reg->EVTXDAT = i;
	k_busy_wait(10);
	/* Wait for TX FIFO to not be full after writing, with shorter timeout */
	while (!(espi_reg->EVSTS & ESPI_EVSTS_TXEMPTY) && --retry_counter) {
		// k_busy_wait(10);
	}

	espi_reg->EVSTS |= ESPI_EVSTS_TXDONE;
	__enable_irq();

	return 0;
}

static void espi_send_vw_event_with_kbdata(uint8_t index, uint8_t data, uint32_t kbc_data,
					   const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	volatile struct kbc_reg *const kbc_reg = espi_config->kbc_reg;
	uint32_t i;
	uint32_t timeout_cnt = 0;

	if ((espi_reg->EVCFG & ESPI_EVCFG_CHEN) == 0 || (espi_reg->EVCFG & ESPI_EVCFG_CHRDY) == 0) {
		return;
	}

	/* Wait for TX FIFO to not be full before writing, with timeout */
	while (espi_reg->EVSTS & ESPI_EVSTS_TXFULL) {
		timeout_cnt++;
		if (timeout_cnt >= ESPI_VW_EVENT_COMPLETE_TIMEOUT_US) {
			return;
		}
	}

	__disable_irq();
	kbc_reg->OB = (uint32_t)kbc_data & 0xff;
	i = 0x0000FFFF & ((uint32_t)index << 8 | (uint32_t)data);
	espi_reg->EVTXDAT = i;
	k_busy_wait(10);
	/* Wait for TX FIFO to not be full after writing, in IRQ disabled state */
	timeout_cnt = 0;
	while (!(espi_reg->EVSTS & ESPI_EVSTS_TXEMPTY)) {
		timeout_cnt++;
		if (timeout_cnt >= ESPI_VW_EVENT_COMPLETE_TIMEOUT_US) {
			// espi_data->kbc_pre_irq1 = 1;
			__enable_irq();
			return;
		}
	}

	//espi_data->kbc_pre_irq1 = 1;
	espi_reg->EVSTS |= ESPI_EVSTS_TXDONE;
	__enable_irq();
}

#endif /* CONFIG_ESPI_PERIPHERAL_8042_KBC */

#endif /* CONFIG_ESPI_VWIRE_CHANNEL */

/*
 * =========================================================================
 * ESPI OOB channel
 * =========================================================================
 */

#ifdef CONFIG_ESPI_OOB_CHANNEL

#define MAX_OOB_TIMEOUT 200UL /* ms */
#define OOB_BUFFER_SIZE 256UL

static int espi_rts5918_send_oob(const struct device *dev, struct espi_oob_packet *pckt)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	int ret;

	if (!(espi_reg->EOCFG & ESPI_EOCFG_CHRDY)) {
		LOG_ERR("%s: OOB channel isn't ready", __func__);
		return -EIO;
	}

	if (espi_data->oob_tx_busy) {
		LOG_ERR("%s: OOB channel is busy", __func__);
		return -EIO;
	}

	if (pckt->len > OOB_BUFFER_SIZE) {
		LOG_ERR("%s: OOB Tx have no insufficient space", __func__);
		return -EINVAL;
	}

	for (int i = 0; i < pckt->len; i++) {
		espi_data->oob_tx_ptr[i] = pckt->buf[i];
	}

	espi_reg->EOTXLEN = pckt->len - 1;
	espi_reg->EOTXCTRL = ESPI_EOTXCTRL_TXSTR;

	espi_data->oob_tx_busy = true;

	/* Wait until ISR or timeout */
	ret = k_sem_take(&espi_data->oob_tx_lock, K_MSEC(MAX_OOB_TIMEOUT));
	espi_data->oob_tx_busy = false;
	if (ret == -EAGAIN) {
		return -ETIMEDOUT;
	} else {
		return 0;
	}
}

static int espi_rts5918_receive_oob(const struct device *dev, struct espi_oob_packet *pckt)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint32_t rx_len;

	if (!(espi_reg->EOCFG & ESPI_EOCFG_CHRDY)) {
		LOG_ERR("%s: OOB channel isn't ready", __func__);
		return -EIO;
	}

	if (espi_reg->EOSTS & ESPI_EOSTS_RXPND) {
		LOG_ERR("OOB Receive Pending");
		return -EIO;
	}

#ifndef CONFIG_ESPI_OOB_CHANNEL_RX_ASYNC
	/* Wait until ISR or timeout */
	int ret = k_sem_take(&espi_data->oob_rx_lock, K_MSEC(MAX_OOB_TIMEOUT));

	if (ret == -EAGAIN) {
		LOG_ERR("OOB Rx Timeout");
		return -ETIMEDOUT;
	}
#endif

	/* Check if buffer passed to driver can fit the received buffer */
	rx_len = espi_reg->EORXLEN;

	if (rx_len > pckt->len) {
		LOG_ERR("space rcvd %d vs %d", rx_len, pckt->len);
		return -EIO;
	}

	pckt->len = rx_len;

	for (int i = 0; i < rx_len; i++) {
		pckt->buf[i] = espi_data->oob_rx_ptr[i];
	}

	return 0;
}

static void espi_oob_tx_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint32_t status = espi_reg->EOSTS;

	if (status & ESPI_EOSTS_TXDONE) {
		k_sem_give(&espi_data->oob_tx_lock);
		espi_reg->EOSTS = ESPI_EOSTS_TXDONE;
	}
}

static void espi_oob_rx_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint32_t status = espi_reg->EOSTS;

#ifdef CONFIG_ESPI_OOB_CHANNEL_RX_ASYNC
	struct espi_event evt = {
		.evt_type = ESPI_BUS_EVENT_OOB_RECEIVED, .evt_details = 0, .evt_data = 0};
#endif

	if (status & ESPI_EOSTS_RXDONE) {
#ifndef CONFIG_ESPI_OOB_CHANNEL_RX_ASYNC
		k_sem_give(&espi_data->oob_rx_lock);
#else
		k_busy_wait(250);
		evt.evt_details = espi_reg->EORXLEN;
		espi_send_callbacks(&espi_data->callbacks, dev, evt);
#endif
		espi_reg->EOSTS = ESPI_EOSTS_RXDONE;
	}
}

static void espi_oob_chg_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;

	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;

	struct espi_event evt = {.evt_type = ESPI_BUS_EVENT_CHANNEL_READY,
				 .evt_details = ESPI_CHANNEL_OOB,
				 .evt_data = 0};

	uint32_t status = espi_reg->EOSTS;
	uint32_t config = espi_reg->EOCFG;

	if (status & ESPI_EOSTS_CFGENCHG) {
		evt.evt_data = config & ESPI_EVCFG_CHEN ? 1 : 0;
		espi_send_callbacks(&espi_data->callbacks, dev, evt);

		if (config & ESPI_EVCFG_CHEN) {
			vw_signal_set_valid(dev, ESPI_VWIRE_SIGNAL_OOB_RST_ACK, 1);
		}

		espi_reg->EOSTS = ESPI_EOSTS_CFGENCHG;
	}
}

/*
 * RTS5918 OOB DMA can only access addresses >= 0x20080000. Placing the
 * buffers in .noinit guarantees they land in the upper half of SRAM
 * (.noinit follows .bss, which already extends past 0x20080000 in this
 * build). A runtime check in espi_oob_ch_setup() verifies the constraint
 * to catch regressions if .bss ever shrinks.
 */
#define RTS5918_OOB_BUF_MIN_ADDR 0x20080000UL

static uint8_t oob_tx_buffer[OOB_BUFFER_SIZE] __noinit __aligned(4);
static uint8_t oob_rx_buffer[OOB_BUFFER_SIZE] __noinit __aligned(4);

static int espi_oob_ch_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	espi_data->oob_tx_busy = false;

	/* HW constraint: OOB DMA SAR must point to address >= 0x20080000 */
	if ((uintptr_t)oob_tx_buffer < RTS5918_OOB_BUF_MIN_ADDR ||
	    (uintptr_t)oob_rx_buffer < RTS5918_OOB_BUF_MIN_ADDR) {
		LOG_ERR("OOB buf addr violates >= 0x%08lx (tx=%p rx=%p)",
			RTS5918_OOB_BUF_MIN_ADDR,
			(void *)oob_tx_buffer, (void *)oob_rx_buffer);
		return -EINVAL;
	}

	espi_data->oob_tx_ptr = oob_tx_buffer;
	if (espi_data->oob_tx_ptr == NULL) {
		LOG_ERR("Failed to allocate OOB Tx buffer");
		return -ENOMEM;
	}

	espi_data->oob_rx_ptr = oob_rx_buffer;
	if (espi_data->oob_tx_ptr == NULL) {
		LOG_ERR("Failed to allocate OOB Rx buffer");
		return -ENOMEM;
	}

	espi_reg->EOTXBUF = (uint32_t)espi_data->oob_tx_ptr;
	espi_reg->EORXBUF = (uint32_t)espi_data->oob_rx_ptr;

	espi_reg->EOTXINTEN = ESPI_EOTXINTEN_TXEN;
	espi_reg->EORXINTEN = (ESPI_EORXINTEN_RXEN | ESPI_EORXINTEN_CHENCHG);

	k_sem_init(&espi_data->oob_tx_lock, 0, 1);
#ifndef CONFIG_ESPI_OOB_CHANNEL_RX_ASYNC
	k_sem_init(&espi_data->oob_rx_lock, 0, 1);
#endif

	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), oob_tx, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), oob_rx, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), oob_chg, irq));

	/* Tx */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), oob_tx, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), oob_tx, priority), espi_oob_tx_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), oob_tx, irq));

	/* Rx */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), oob_rx, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), oob_rx, priority), espi_oob_rx_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), oob_rx, irq));

	/* Chg */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), oob_chg, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), oob_chg, priority), espi_oob_chg_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), oob_chg, irq));

	return 0;
}

#endif /* CONFIG_ESPI_OOB_CHANNEL */

/*
 * =========================================================================
 * ESPI flash channel
 * =========================================================================
 */

#ifdef CONFIG_ESPI_FLASH_CHANNEL
struct data_item_t {
    uint32_t addr;
	const struct device *dev;
};
#define MSGQ_MAX_MSGS 10
#define MSG_SIZE sizeof(struct data_item_t)
K_MSGQ_DEFINE(taf_rx_msgq, MSG_SIZE, MSGQ_MAX_MSGS, 4);
static struct data_item_t taf_rx_data;

static void espi_saf_erase_cmd_isr(const struct device *dev)
{
	const struct device *spic_dev = DEVICE_DT_GET(DT_NODELABEL(flash_controller));
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	static uint32_t last_addr = 0;
	int ret = 0;

	if(espi_reg->EFSTS & ESPI_EFSTS_SAFERS) {
		const uint32_t safs_flash_addr = espi_reg->ESRXADR;
		int key = irq_lock();
		// Make sure sector erase is not swapped-out
		irq_disable(40);
		taf_rx_data.dev = dev;
		taf_rx_data.addr = safs_flash_addr;

		ret = k_msgq_put(&taf_rx_msgq, &taf_rx_data, K_NO_WAIT);
		if (ret != 0) {
			LOG_ERR("TAF buffer full!");
		}
		irq_unlock(key);
	}
	/* clear pending interrupt */
	NVIC_ClearPendingIRQ(40);

 	return;
}

#define MAX_FLASH_TIMEOUT 1000UL
#define MAF_BUFFER_SIZE   512UL

enum {
	MAF_TR_READ = 0ul,
	MAF_TR_WRITE = 1ul,
	MAF_TR_ERASE = 2ul,
};

static int espi_rts5918_flash_read(const struct device *dev, struct espi_flash_packet *pckt)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	int ret;
	uint32_t ctrl;

	if (!(espi_reg->EFCONF & ESPI_EFCONF_CHEN)) {
		LOG_ERR("Flash channel is disabled");
		return -EIO;
	}

	if (pckt->len > MAF_BUFFER_SIZE) {
		LOG_ERR("Invalid size request");
		return -EINVAL;
	}

	if (espi_reg->EMCTRL & ESPI_EMCTRL_START) {
		LOG_ERR("Channel still busy");
		return -EBUSY;
	}

	ctrl = (MAF_TR_READ << ESPI_EMCTRL_MDSEL_Pos) | ESPI_EMCTRL_START;

	espi_reg->EMADR = pckt->flash_addr;
	espi_reg->EMTRLEN = pckt->len;
	espi_reg->EMCTRL = ctrl;

	/* Wait until ISR or timeout */
	ret = k_sem_take(&espi_data->flash_lock, K_MSEC(MAX_FLASH_TIMEOUT));
	if (ret == -EAGAIN) {
		LOG_ERR("%s timeout", __func__);
		return -ETIMEDOUT;
	}

	for (int i = 0; i < pckt->len; i++) {
		pckt->buf[i] = espi_data->maf_ptr[i];
	}

	return 0;
}

static int espi_rts5918_flash_write(const struct device *dev, struct espi_flash_packet *pckt)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	int ret;
	uint32_t ctrl;

	if (!(espi_reg->EFCONF & ESPI_EFCONF_CHEN)) {
		LOG_ERR("Flash channel is disabled");
		return -EIO;
	}

	if (pckt->len > MAF_BUFFER_SIZE) {
		LOG_ERR("Packet length is too big");
		return -EINVAL;
	}

	if (espi_reg->EMCTRL & ESPI_EMCTRL_START) {
		LOG_ERR("Channel still busy");
		return -EBUSY;
	}

	for (int i = 0; i < pckt->len; i++) {
		espi_data->maf_ptr[i] = pckt->buf[i];
	}

	ctrl = (MAF_TR_WRITE << ESPI_EMCTRL_MDSEL_Pos) | ESPI_EMCTRL_START;

	espi_reg->EMADR = pckt->flash_addr;
	espi_reg->EMTRLEN = pckt->len;
	espi_reg->EMCTRL = ctrl;

	/* Wait until ISR or timeout */
	ret = k_sem_take(&espi_data->flash_lock, K_MSEC(MAX_FLASH_TIMEOUT));
	if (ret == -EAGAIN) {
		LOG_ERR("%s timeout", __func__);
		return -ETIMEDOUT;
	}

	return 0;
}

static int espi_rts5918_flash_erase(const struct device *dev, struct espi_flash_packet *pckt)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	int ret;
	uint32_t ctrl;

	if (!(espi_reg->EFCONF & ESPI_EFCONF_CHEN)) {
		LOG_ERR("Flash channel is disabled");
		return -EIO;
	}

	if (espi_reg->EMCTRL & ESPI_EMCTRL_START) {
		LOG_ERR("Channel still busy");
		return -EBUSY;
	}

	ctrl = (MAF_TR_ERASE << ESPI_EMCTRL_MDSEL_Pos) | ESPI_EMCTRL_START;

	espi_reg->EMADR = pckt->flash_addr;
	espi_reg->EMTRLEN = pckt->len;
	espi_reg->EMCTRL = ctrl;

	/* Wait until ISR or timeout */
	ret = k_sem_take(&espi_data->flash_lock, K_MSEC(MAX_FLASH_TIMEOUT));
	if (ret == -EAGAIN) {
		LOG_ERR("%s timeout", __func__);
		return -ETIMEDOUT;
	}

	return 0;
}

static void espi_maf_tr_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint32_t status = espi_reg->EFSTS;

	if (status & ESPI_EFSTS_MAFTXDN) {
		k_sem_give(&espi_data->flash_lock);
		espi_reg->EFSTS = ESPI_EFSTS_MAFTXDN;
	}
}

static void espi_flash_chg_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;

	struct espi_event evt = {.evt_type = ESPI_BUS_EVENT_CHANNEL_READY,
				 .evt_details = ESPI_CHANNEL_FLASH,
				 .evt_data = 0};

	uint32_t config = espi_reg->EFCONF;

	if (1) {
		evt.evt_data = (config & ESPI_EFCONF_CHEN) ? 1 : 0;
if(1) {
			espi_reg->EFCONF |= ESPI_EFCONF_CHEN;
			vw_signal_set_valid(dev, ESPI_VWIRE_SIGNAL_TARGET_BOOT_STS, 1);
			vw_signal_set_valid(dev, ESPI_VWIRE_SIGNAL_TARGET_BOOT_DONE, 1);
			vw_signal_set_valid(dev, ESPI_VWIRE_SIGNAL_SUS_ACK, 1);
#ifdef CONFIG_ESPI_AUTOMATIC_BOOT_DONE_ACKNOWLEDGE
			send_target_bootdone(dev);
#endif
		}
		espi_send_callbacks(&espi_data->callbacks, dev, evt);

		espi_reg->EFSTS = ESPI_EFSTS_CHENCHG;
	}
}

static uint8_t flash_channel_buffer[MAF_BUFFER_SIZE] __aligned(4);

static int espi_flash_ch_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = dev->data;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;

	espi_data->maf_ptr = flash_channel_buffer;
	if (espi_data->maf_ptr == NULL) {
		LOG_ERR("Failed to allocate MAF buffer");
		return -ENOMEM;
	}

	espi_reg->EMBUF = (uint32_t)espi_data->maf_ptr;
	espi_reg->EMINTEN = ESPI_EMINTEN_CHENCHG | ESPI_EMINTEN_TRDONEEN;

	espi_reg->ESINTEN |= ESPI_ESINTEN_ERASEEN;
	espi_reg->ESBUF = 0x60000000;
	IRQ_CONNECT(40, 0, espi_saf_erase_cmd_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(40);

	k_sem_init(&espi_data->flash_lock, 0, 1);

	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), maf_tr, irq));
	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), flash_chg, irq));

	/* MAF Tr */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), maf_tr, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), maf_tr, priority), espi_maf_tr_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), maf_tr, irq));

	/* Chg */
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), flash_chg, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), flash_chg, priority), espi_flash_chg_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), flash_chg, irq));

	return 0;
}

#endif /* CONFIG_ESPI_FLASH_CHANNEL */

#ifdef CONFIG_ESPI_MBOX

static void espi_mbox_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = (struct espi_rts5918_data *)dev->data;
	volatile struct mbox_reg *const mbox_reg = espi_config->mbox_reg;
	struct espi_event evt = {ESPI_BUS_PERIPHERAL_NOTIFICATION, ESPI_PERIPHERAL_MBOX,
		ESPI_PERIPHERAL_NODATA};

	LOG_ERR("mbox trig!!!!!!!!");

	espi_send_callbacks(&espi_data->callbacks, dev, evt);

	mbox_reg->INTSTS = MBX_INTSTS_CLR_Msk;		// W1C

}

#define SYSTEM_MMAP (0x40100000ul)
#define SYSTEM_VINOUT_MONITOR (*(volatile uint32_t *)(SYSTEM_MMAP + 0x110ul))
#define SYSTEM_VINOUT_MONITOR_VIVOLOG ((SYSTEM_VINOUT_MONITOR >> 27) & 0x3)
#define SYSTEM_LDO2_WRITE (*(volatile uint32_t *)(SYSTEM_MMAP + 0x114ul))
#define SYSTEM_VINOUT_CTRL (*(volatile uint32_t *)(SYSTEM_MMAP + 0x10Cul))

static void init_bsram ()
{
	/* Check first-time system booting */
	if(SYSTEM_VINOUT_MONITOR_VIVOLOG == 0)
	{
		/* Set bsram all zero */
		memset((void *)0x20200000, 0, 256);

		/* Enable VIVOLOG */
		SYSTEM_LDO2_WRITE = 0x2;
		SYSTEM_VINOUT_MONITOR |= (1 << 27);
		SYSTEM_LDO2_WRITE = 0;
	}

	SYSTEM_VINOUT_CTRL |= (0x1 << 31);
}

static int espi_mabox_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *espi_data = (struct espi_rts5918_data *)dev->data;
	volatile struct mbox_reg *const mbox_reg = espi_config->mbox_reg;

	LOG_ERR("MBOX start!!!");
	mbox_reg->INTCTRL = MBX_INTCTRL_INTEN_Msk;
	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), mbx, irq),
	DT_IRQ_BY_NAME(DT_DRV_INST(0), mbx, priority), espi_mbox_isr,
	DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), mbx, irq));
	LOG_ERR("MBOX end!!!");


	init_bsram();

	return 0;
}
#endif

/*
 * =========================================================================
 * ESPI common function and API
 * =========================================================================
 */

#define RTS5918_ESPI_MAX_FREQ_20 20
#define RTS5918_ESPI_MAX_FREQ_25 25
#define RTS5918_ESPI_MAX_FREQ_33 33
#define RTS5918_ESPI_MAX_FREQ_50 50
#define RTS5918_ESPI_MAX_FREQ_66 66

static int espi_rts5918_configure(const struct device *dev, struct espi_cfg *cfg)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *data = dev->data;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;

	uint32_t gen_conf = 0;
	uint8_t io_mode = 0;

	/* Maximum Frequency Supported */
	switch (cfg->max_freq) {
	case RTS5918_ESPI_MAX_FREQ_20:
		gen_conf |= 0UL << ESPI_ESPICFG_MXFREQSUP_Pos;
		break;
	case RTS5918_ESPI_MAX_FREQ_25:
		gen_conf |= 1UL << ESPI_ESPICFG_MXFREQSUP_Pos;
		break;
	case RTS5918_ESPI_MAX_FREQ_33:
		gen_conf |= 2UL << ESPI_ESPICFG_MXFREQSUP_Pos;
		break;
	case RTS5918_ESPI_MAX_FREQ_50:
		gen_conf |= 3UL << ESPI_ESPICFG_MXFREQSUP_Pos;
		break;
	case RTS5918_ESPI_MAX_FREQ_66:
		gen_conf |= 4UL << ESPI_ESPICFG_MXFREQSUP_Pos;
		break;
	default:
		return -EINVAL;
	}

	/* I/O Mode Supported */
	io_mode = cfg->io_caps >> 1;

	if (io_mode > 3) {
		return -EINVAL;
	}
	gen_conf |= io_mode << ESPI_ESPICFG_IOSUP_Pos;

	/* Channel Supported */
	if (cfg->channel_caps & ESPI_CHANNEL_PERIPHERAL) {
		gen_conf |= BIT(0) << ESPI_ESPICFG_CHSUP_Pos;
	}

	if (cfg->channel_caps & ESPI_CHANNEL_VWIRE) {
		gen_conf |= BIT(1) << ESPI_ESPICFG_CHSUP_Pos;
	}

	if (cfg->channel_caps & ESPI_CHANNEL_OOB) {
		gen_conf |= BIT(2) << ESPI_ESPICFG_CHSUP_Pos;
	}

	if (cfg->channel_caps & ESPI_CHANNEL_FLASH) {
		gen_conf |= BIT(3) << ESPI_ESPICFG_CHSUP_Pos;
	}

	espi_reg->ESPICFG = gen_conf;
	data->config_data = espi_reg->ESPICFG;

	return 0;
}

static bool espi_rts5918_channel_ready(const struct device *dev, enum espi_channel ch)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;

	switch (ch) {
	case ESPI_CHANNEL_PERIPHERAL:
		return espi_reg->EPCFG & ESPI_EPCFG_CHEN ? true : false;
	case ESPI_CHANNEL_VWIRE:
		return espi_reg->EVCFG & ESPI_EVCFG_CHEN ? true : false;
	case ESPI_CHANNEL_OOB:
		return espi_reg->EOCFG & ESPI_EOCFG_CHEN ? true : false;
	case ESPI_CHANNEL_FLASH:
		return espi_reg->EFCONF & ESPI_EFCONF_CHEN ? true : false;
	default:
		return false;
	}
}

static int espi_rts5918_manage_callback(const struct device *dev, struct espi_callback *callback,
					bool set)
{
	struct espi_rts5918_data *data = dev->data;

	return espi_manage_callback(&data->callbacks, callback, set);
}

//static DEVICE_API(espi, espi_rts5918_driver_api) = {
static const struct espi_driver_api espi_rts5918_driver_api = {
	.config = espi_rts5918_configure,
	.get_channel_status = espi_rts5918_channel_ready,
	.manage_callback = espi_rts5918_manage_callback,
#ifdef CONFIG_ESPI_PERIPHERAL_CHANNEL
	.read_lpc_request = espi_rts5918_read_lpc_request,
	.write_lpc_request = espi_rts5918_write_lpc_request,
#endif
#ifdef CONFIG_ESPI_VWIRE_CHANNEL
	.send_vwire = espi_rts5918_send_vwire,
	.receive_vwire = espi_rts5918_receive_vwire,
#endif
#ifdef CONFIG_ESPI_OOB_CHANNEL
	.send_oob = espi_rts5918_send_oob,
	.receive_oob = espi_rts5918_receive_oob,
#endif
#ifdef CONFIG_ESPI_FLASH_CHANNEL
	.flash_read = espi_rts5918_flash_read,
	.flash_write = espi_rts5918_flash_write,
	.flash_erase = espi_rts5918_flash_erase,
#endif
};

static void espi_vw_ch_setup(const struct device *dev);

#define VW_RESET_DELAY 150UL

static void espi_rst_isr(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct espi_rts5918_data *data = dev->data;

	struct espi_event evt = {.evt_type = ESPI_BUS_RESET, .evt_details = 0, .evt_data = 0};

	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
	uint32_t status = espi_reg->ERSTCFG;

	espi_reg->ERSTCFG |= ESPI_ERSTCFG_RSTSTS;
	//espi_reg->ERSTCFG ^= ESPI_ERSTCFG_RSTPOL;

	if (status & ESPI_ERSTCFG_RSTSTS) {
		if (status & ESPI_ERSTCFG_RSTPOL) {
			/* rst pin high go low trigger interrupt */
			evt.evt_data = 1;
		} else {
			/* rst pin low go high trigger interrupt */
			evt.evt_data = 0;
#ifdef CONFIG_ESPI_VWIRE_CHANNEL
			//espi_vw_ch_setup(dev);
			//espi_reg->ESPICFG = data->config_data;
			//if (espi_reg->EVCFG & ESPI_EVCFG_CHEN) {
				k_timeout_t delay = K_MSEC(VW_RESET_DELAY);

				k_work_schedule(&vw_ch_isr_wa, delay);
			//}
#endif

#ifdef CONFIG_ESPI_VWIRE_CHANNEL
			espi_vw_ch_setup(dev);
			#endif
			espi_reg->EOCFG &= ~ESPI_EOCFG_CHEN;
			espi_reg->EOCFG &= ~ESPI_EOCFG_CHRDY;
			espi_reg->EFCONF &= ~ESPI_EFCONF_CHEN;
			espi_reg->EFCONF &= ~ESPI_EFCONF_CHRDY;
			espi_reg->EVCFG &= ~ESPI_EVCFG_CHEN;
			espi_reg->EVCFG &= ~ESPI_EVCFG_CHRDY;

			if(espi_reg->ELCTRL) {
				/* Reset LTR */
				espi_reg->ELCTRL = 0;
				espi_reg->ELMSG = 0;
				espi_reg->ELCTRL = 1;
			}
espi_reg->ESPICFG = data->config_data;
		}
		espi_send_callbacks(&data->callbacks, dev, evt);
	}
}

static void espi_bus_reset_setup(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	volatile struct espi_reg *const espi_reg = espi_config->espi_reg;

	espi_reg->ERSTCFG = ESPI_ERSTCFG_RSTINTEN;
	espi_reg->ERSTCFG = ESPI_ERSTCFG_RSTMONEN;

	// if (espi_reg->ERSTCFG & ESPI_ERSTCFG_RSTSTS) {
	// 	/* high to low */
	// 	espi_reg->ERSTCFG =
	// 		ESPI_ERSTCFG_RSTMONEN | ESPI_ERSTCFG_RSTPOL | ESPI_ERSTCFG_RSTINTEN;
	// } else {
		/* low to high */
		espi_reg->ERSTCFG = ESPI_ERSTCFG_RSTMONEN | ESPI_ERSTCFG_RSTINTEN;
	//}

	NVIC_ClearPendingIRQ(DT_IRQ_BY_NAME(DT_DRV_INST(0), bus_rst, irq));

	IRQ_CONNECT(DT_IRQ_BY_NAME(DT_DRV_INST(0), bus_rst, irq),
		    DT_IRQ_BY_NAME(DT_DRV_INST(0), bus_rst, priority), espi_rst_isr,
		    DEVICE_DT_GET(DT_DRV_INST(0)), 0);
	irq_enable(DT_IRQ_BY_NAME(DT_DRV_INST(0), bus_rst, irq));
}

static int espi_rts5918_init(const struct device *dev)
{
	const struct espi_rts5918_config *const espi_config = dev->config;
	struct rts5918_sccon_subsys sccon;

	int rc;

	/* Setup eSPI pins */
	rc = pinctrl_apply_state(espi_config->pcfg, PINCTRL_STATE_DEFAULT);
	if (rc < 0) {
		LOG_ERR("eSPI pinctrl setup failed (%d)", rc);
		return rc;
	}

	if (!device_is_ready(espi_config->clk_dev)) {
		LOG_ERR("eSPI clock not ready");
		return -ENODEV;
	}

	/* Enable eSPI clock */
	sccon.clk_grp = espi_config->espislv_clk_grp;
	sccon.clk_idx = espi_config->espislv_clk_idx;
	rc = clock_control_on(espi_config->clk_dev, (clock_control_subsys_t)&sccon);
	if (rc != 0) {
		LOG_ERR("eSPI clock control on failed");
		goto exit;
	}

	/* Setup eSPI bus reset */
	espi_bus_reset_setup(dev);

#ifdef CONFIG_ESPI_PERIPHERAL_8042_KBC
	/* Setup KBC */
	rc = espi_kbc_setup(dev);
	if (rc != 0) {
		LOG_ERR("eSPI KBC setup failed");
		goto exit;
	}
#endif

#ifdef CONFIG_ESPI_PERIPHERAL_ACPI_SHM_REGION
	espi_setup_acpi_shm(espi_config);
#endif

#ifdef CONFIG_ESPI_PERIPHERAL_HOST_IO
	/* Setup ACPI */
	rc = espi_acpi_setup(dev);
	if (rc != 0) {
		LOG_ERR("eSPI ACPI setup failed");
		goto exit;
	}
#endif

	/* Setup EMI */
	rc = espi_emi_setup(dev);
	if (rc != 0) {
		LOG_ERR("eSPI EMI setup failed");
		goto exit;
	}

#ifdef CONFIG_ESPI_PERIPHERAL_EC_HOST_CMD
	rc = espi_promt0_setup(dev);
	if (rc != 0) {
		LOG_ERR("eSPI Promt0 setup failed");
		goto exit;
	}

	//rc = espi_promt1_setup(dev);
	// if (rc != 0) {
	// 	LOG_ERR("eSPI Promt1 setup failed");
	// 	goto exit;
	// }
	rc = espi_promt2_setup(dev);
	if (rc != 0) {
		LOG_ERR("eSPI Promt2 setup failed");
		goto exit;
	}
	rc = espi_promt3_setup(dev);
	if (rc != 0) {
		LOG_ERR("eSPI Promt3 setup failed");
		goto exit;
	}
#endif

#ifdef CONFIG_ESPI_PERIPHERAL_DEBUG_PORT_80
	/* Setup Port80 */
	rc = espi_peri_ch_port80_setup(dev);
	if (rc != 0) {
		LOG_ERR("eSPI Port80 setup failed");
		goto exit;
	}
#endif

#ifdef CONFIG_ESPI_PERIPHERAL_CHANNEL
	/* Setup eSPI peripheral channel */
	espi_periph_ch_setup(dev);
#endif

#ifdef CONFIG_ESPI_VWIRE_CHANNEL
	/* Setup eSPI virtual-wire channel */
	espi_vw_ch_setup(dev);
#endif

#ifdef CONFIG_ESPI_OOB_CHANNEL
	/* Setup eSPI OOB channel */
	rc = espi_oob_ch_setup(dev);
	if (rc != 0) {
		LOG_ERR("eSPI OOB channel setup failed");
		goto exit;
	}
#endif

#ifdef CONFIG_ESPI_FLASH_CHANNEL
	/* Setup eSPI flash channel */
	rc = espi_flash_ch_setup(dev);
	if (rc != 0) {
		LOG_ERR("eSPI flash channel setup failed");
		goto exit;
	}
#endif

#ifdef CONFIG_ESPI_MBOX
	/* Setup eSPI mbox */
	rc = espi_mabox_setup(dev);
	if (rc != 0) {
		LOG_ERR("eSPI mbox setup failed");
		goto exit;
	}
#endif

exit:
	return rc;
}

void taf_rx_thread(void)
{
	static uint32_t safs_flash_addr1;
	int ret;
	const struct device *spic_dev = DEVICE_DT_GET(DT_NODELABEL(flash_controller));
	while(1)
	{
		LOG_INF("get taf_msg queue!!!!");
        ret = k_msgq_get(&taf_rx_msgq, &taf_rx_data, K_FOREVER);
		if(ret == 0) {
			LOG_INF("start run taf_rx_thread thread\n\r");
			const struct espi_rts5918_config *const espi_config = taf_rx_data.dev->config;
			volatile struct espi_reg *const espi_reg = espi_config->espi_reg;
			safs_flash_addr1 = taf_rx_data.addr;
			flash_rts5918_saf_erase_sector_handler(spic_dev, safs_flash_addr1 & (~0xFFF));
			espi_reg->EFSTS = (ESPI_EFSTS_SAFERS);

			// Wait for host read flash np free status
			volatile int cnt = 10000;
			while (cnt--) {
				if((espi_reg->EFSTS & ESPI_EFSTS_SAFERS) == 0){
					break;
				}
			}
			if(cnt == 0)
			{
				LOG_ERR("Clear erase status Timeout!!!");
			}

			unsigned int key = irq_lock();
			NVIC_ClearPendingIRQ(40);
			irq_enable(40);
			irq_unlock(key);
		}
	}
}

K_THREAD_DEFINE(taf_rx_tid, 1024, taf_rx_thread, NULL, NULL, NULL,
                0, 0, 0);

PINCTRL_DT_INST_DEFINE(0);

static struct espi_rts5918_data espi_rts5918_data_0;

static const struct espi_rts5918_config espi_rts5918_config = {
	.espi_reg = (volatile struct espi_reg *const)DT_INST_REG_ADDR_BY_NAME(0, espi_target),
	.espislv_clk_grp = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), espi_target, clk_grp),
	.espislv_clk_idx = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), espi_target, clk_idx),
#ifdef CONFIG_ESPI_PERIPHERAL_8042_KBC
	.kbc_reg = (volatile struct kbc_reg *const)DT_INST_REG_ADDR_BY_NAME(0, kbc),
	.kbc_clk_grp = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), kbc, clk_grp),
	.kbc_clk_idx = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), kbc, clk_idx),
#endif
#ifdef CONFIG_ESPI_PERIPHERAL_HOST_IO
	.acpi_reg = (volatile struct acpi_reg *const)DT_INST_REG_ADDR_BY_NAME(0, acpi),
	.acpi_clk_grp = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), acpi, clk_grp),
	.acpi_clk_idx = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), acpi, clk_idx),
#endif
#ifdef CONFIG_ESPI_PERIPHERAL_EC_HOST_CMD
	.promt0_reg = (volatile struct acpi_reg *const)DT_INST_REG_ADDR_BY_NAME(0, promt0),
	.promt0_clk_grp = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), promt0, clk_grp),
	.promt0_clk_idx = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), promt0, clk_idx),

	.emi0_reg = (volatile struct emi_reg *const)DT_INST_REG_ADDR_BY_NAME(0, emi0),
	.emi0_clk_grp = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), emi0, clk_grp),
	.emi0_clk_idx = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), emi0, clk_idx),

	.promt1_reg = (volatile struct acpi_reg *const)DT_INST_REG_ADDR_BY_NAME(0, promt1),
	.promt1_clk_grp = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), promt1, clk_grp),
	.promt1_clk_idx = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), promt1, clk_idx),

	.promt2_reg = (volatile struct acpi_reg *const)DT_INST_REG_ADDR_BY_NAME(0, promt2),
	.promt2_clk_grp = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), promt2, clk_grp),
	.promt2_clk_idx = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), promt2, clk_idx),

	.promt3_reg = (volatile struct acpi_reg *const)DT_INST_REG_ADDR_BY_NAME(0, promt3),
	.promt3_clk_grp = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), promt3, clk_grp),
	.promt3_clk_idx = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), promt3, clk_idx),
#endif
#ifdef CONFIG_ESPI_PERIPHERAL_ACPI_SHM_REGION
	.emi1_reg = (volatile struct emi_reg *const)DT_INST_REG_ADDR_BY_NAME(0, emi1),
	.emi1_clk_grp = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), emi1, clk_grp),
	.emi1_clk_idx = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), emi1, clk_idx),
#endif
#ifdef CONFIG_ESPI_PERIPHERAL_DEBUG_PORT_80
	.port80_reg = (volatile struct port80_reg *const)DT_INST_REG_ADDR_BY_NAME(0, port80),
	.port80_clk_grp = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), port80, clk_grp),
	.port80_clk_idx = DT_CLOCKS_CELL_BY_NAME(DT_DRV_INST(0), port80, clk_idx),
#endif
#ifdef CONFIG_ESPI_MBOX
	.mbox_reg = (volatile struct portmbox_reg *const)DT_INST_REG_ADDR_BY_NAME(0, mbx),

#endif
	.clk_dev = DEVICE_DT_GET(DT_INST_CLOCKS_CTLR(0)),
	.pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(0),
};

DEVICE_DT_INST_DEFINE(0, &espi_rts5918_init, NULL, &espi_rts5918_data_0, &espi_rts5918_config,
		      PRE_KERNEL_2, CONFIG_ESPI_INIT_PRIORITY, &espi_rts5918_driver_api);
