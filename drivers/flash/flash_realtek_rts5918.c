/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT     realtek_rts5918_flash_controller
#define SOC_NV_FLASH_NODE DT_INST(0, soc_nv_flash)

#define PINCTRL_STATE_EXTALT	PINCTRL_STATE_PRIV_START

#define FLASH_PAGE_SZ      256
#define FLASH_WRITE_BLK_SZ DT_PROP(SOC_NV_FLASH_NODE, write_block_size)
#define FLASH_ERASE_BLK_SZ DT_PROP(SOC_NV_FLASH_NODE, erase_block_size)

#define LOG_LEVEL CONFIG_FLASH_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(flash_rts5918);

#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#ifdef CONFIG_FLASH_EX_OP_ENABLED
#include <zephyr/drivers/flash/rts5918_flash_api_ex.h>
#endif
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/pinctrl.h>
#include <soc.h>
#include <string.h>

#include "spi_nor.h"
#include "reg/reg_spic.h"

#define FLASH_CMD_RDSFDP         0x5A /* Read SFDP */
#define FLASH_CMD_EX4B           0xE9 /* Exit 4-byte mode */
#define FLASH_CMD_EXTNADDR_WREAR 0xC5 /* Write extended address register */
#define FLASH_CMD_EXTNADDR_RDEAR 0xC8 /* Read extended address register */

/* W25Q-style Security Register vendor commands */
#define FLASH_CMD_RDSECREG       0x48 /* Read Security Registers  (24-bit addr + 8 dummy) */
#define FLASH_CMD_PPSECREG       0x42 /* Program Security Registers (24-bit addr, up to 256B) */
#define FLASH_CMD_ERSECREG       0x44 /* Erase Security Registers (24-bit addr, erases a 256B page) */

#define SEC_REG_PAGE_BYTES       256
#define SEC_REG_READ_DUMMY       8

#define MODE(x)    (((x) << 6) & SPIC_CTRL0_SCPH)
#define TMOD(x)    (((x) << SPIC_CTRL0_TMOD_Pos) & SPIC_CTRL0_TMOD_Msk)
#define CMD_CH(x)  (((x) << SPIC_CTRL0_CMDCH_Pos) & SPIC_CTRL0_CMDCH_Msk)
#define ADDR_CH(x) (((x) << SPIC_CTRL0_ADDRCH_Pos) & SPIC_CTRL0_ADDRCH_Msk)
#define DATA_CH(x) (((x) << SPIC_CTRL0_DATACH_Pos) & SPIC_CTRL0_DATACH_Msk)

#define USER_CMD_LENGTH(x)  (((x) << SPIC_USERLENGTH_CMDLEN_Pos) & SPIC_USERLENGTH_CMDLEN_Msk)
#define USER_ADDR_LENGTH(x) (((x) << SPIC_USERLENGTH_ADDRLEN_Pos) & SPIC_USERLENGTH_ADDRLEN_Msk)
#define USER_RD_DUMMY_LENGTH(x)                                                                    \
	(((x) << SPIC_USERLENGTH_RDDUMMYLEN_Pos) & SPIC_USERLENGTH_RDDUMMYLEN_Msk)

#define TX_NDF(x) (((x) << SPIC_TXNDF_NUM_Pos) & SPIC_TXNDF_NUM_Msk)
#define RX_NDF(x) (((x) << SPIC_RXNDF_NUM_Pos) & SPIC_RXNDF_NUM_Msk)

#define TIMEOUT_SPICEN  10UL
#define TIMEOUT_SPIBUSY 10000UL
#define TIMEOUT_SAFLIMITCNT 100UL

enum {
	THREEBYTEERASE = 0,
	FOURBYTEERASE = 1,
};

enum {
	COMMAND_READ = 0,
	COMMAND_WRITE = 1,
};

enum spic_freq {
	SPIC_FREQ_SYS_CLK_DIV2 = 1,
	SPIC_FREQ_SYS_CLK_DIV4,
	SPIC_FREQ_SYS_CLK_DIV8,
	SPIC_FREQ_SYS_CLK_DIV16,
};

enum spic_bus_width {
	SPIC_CFG_BUS_SINGLE,
	SPIC_CFG_BUS_DUAL,
	SPIC_CFG_BUS_QUAD,
};

enum spic_address_size {
	SPIC_CFG_ADDR_SIZE_8,
	SPIC_CFG_ADDR_SIZE_16,
	SPIC_CFG_ADDR_SIZE_24,
	SPIC_CFG_ADDR_SIZE_32,
};

struct qspi_cmd {
	struct {
		enum spic_bus_width bus_width; /* Bus width for the instruction */
		uint8_t value;                 /* Instruction value */
		uint8_t disabled; /* Instruction phase skipped if disabled is set to true */
	} instruction;
	struct {
		enum spic_bus_width bus_width; /* Bus width for the address */
		enum spic_address_size size;   /* Address size */
		uint32_t value;                /* Address value */
		uint8_t disabled; /* Address phase skipped if disabled is set to true */
	} address;
	struct {
		enum spic_bus_width bus_width; /* Bus width for alternative */
		uint8_t size;                  /* Alternative size */
		uint32_t value;                /* Alternative value */
		uint8_t disabled; /* Alternative phase skipped if disabled is set to true */
	} alt;
	uint8_t dummy_count; /* Dummy cycles count */
	struct {
		enum spic_bus_width bus_width; /* Bus width for data */
	} data;
};

struct flash_rts5918_dev_config {
	volatile struct reg_spic_reg *regs;
	struct flash_parameters flash_rts5918_parameters;
	uint32_t enter_4ba;
	const struct pinctrl_dev_config *pcfg;
	uint8_t qspi_mode; /* 0=1-1-1, 1=1-1-4, 2=1-4-4 */
};

struct flash_rts5918_dev_data {
	struct k_sem sem;
	struct qspi_cmd command_default;
	/*
	 * Chip-select index driven onto SER by spic_cs_active(). Default
	 * 0 matches single-chip Nuvoton-era topology; Bison uses CS1 on
	 * SPIC0 via FLASH_RTS5918_EX_OP_SELECT_CS before each op.
	 */
	uint8_t cs;
};

static const uint8_t user_addr_len[] = {
	[SPIC_CFG_ADDR_SIZE_8] = 1,
	[SPIC_CFG_ADDR_SIZE_16] = 2,
	[SPIC_CFG_ADDR_SIZE_24] = 3,
	[SPIC_CFG_ADDR_SIZE_32] = 4,
};

static int config_command(struct qspi_cmd *command, uint8_t cmd, uint32_t addr,
			  enum spic_address_size addr_size, uint8_t dummy_count)
{
	int ret = 0;

	switch (cmd) {
	case SPI_NOR_CMD_WREN:
	case SPI_NOR_CMD_WRDI:
	case SPI_NOR_CMD_WRSR:
	case SPI_NOR_CMD_WRSR2:
	case SPI_NOR_CMD_RDID:
	case SPI_NOR_CMD_RDSR:
	case SPI_NOR_CMD_RDSR2:
	case SPI_NOR_CMD_CE:
	case SPI_NOR_CMD_4BA:
	case FLASH_CMD_EX4B:
	case FLASH_CMD_EXTNADDR_WREAR:
	case FLASH_CMD_EXTNADDR_RDEAR:
	case SPI_NOR_CMD_RESET_EN:
	case SPI_NOR_CMD_RESET_MEM:
		command->address.disabled = 1;
		command->data.bus_width = SPIC_CFG_BUS_SINGLE;
		break;
	case SPI_NOR_CMD_READ:
	case SPI_NOR_CMD_READ_FAST:
	case SPI_NOR_CMD_SE:
	case SPI_NOR_CMD_BE:
case SPI_NOR_CMD_SE_4B:
	case FLASH_CMD_RDSFDP:
	case SPI_NOR_CMD_PP:
	case FLASH_CMD_RDSECREG:
	case FLASH_CMD_PPSECREG:
	case FLASH_CMD_ERSECREG:
		command->address.disabled = 0;
		command->address.bus_width = SPIC_CFG_BUS_SINGLE;
		command->data.bus_width = SPIC_CFG_BUS_SINGLE;
		break;
	case SPI_NOR_CMD_DREAD:
		command->address.disabled = 0;
		command->address.bus_width = SPIC_CFG_BUS_SINGLE;
		command->data.bus_width = SPIC_CFG_BUS_DUAL;
		break;
	case SPI_NOR_CMD_QREAD:
		command->address.disabled = 0;
		command->address.bus_width = SPIC_CFG_BUS_SINGLE;
		command->data.bus_width = SPIC_CFG_BUS_QUAD;
		break;
	case SPI_NOR_CMD_2READ:
		command->address.disabled = 0;
		command->address.bus_width = SPIC_CFG_BUS_DUAL;
		command->data.bus_width = SPIC_CFG_BUS_DUAL;
		break;
	case SPI_NOR_CMD_4READ:
	case SPI_NOR_CMD_PP_1_4_4:
		command->address.disabled = 0;
		command->address.bus_width = SPIC_CFG_BUS_QUAD;
		command->data.bus_width = SPIC_CFG_BUS_QUAD;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	command->instruction.value = cmd;
	command->address.size = addr_size;
	command->address.value = addr;
	command->dummy_count = dummy_count;

	return ret;
}

static int spic_wait_finish(const struct device *dev)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;
	int count = TIMEOUT_SPICEN;

	while ((spic_reg->SSIENR & SPIC_SSIENR_SPICEN) && count) {
		--count;
	}
	if (!count) {
		return -ETIMEDOUT;
	}
	return 0;
}

static inline void spic_flush_fifo(const struct device *dev)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;

	spic_reg->FLUSH = SPIC_FLUSH_ALL;
}

static inline void spic_cs_active(const struct device *dev)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;
	struct flash_rts5918_dev_data *data = dev->data;

	spic_reg->SER = BIT(data->cs);
}

static inline void spic_cs_deactivate(const struct device *dev)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;

	spic_reg->SER = 0UL;
}



static inline void spic_usermode(const struct device *dev)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;

	spic_reg->CTRL0 |= SPIC_CTRL0_USERMD;


}

static inline void spic_automode(const struct device *dev)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;

	spic_reg->CTRL0 &= ~SPIC_CTRL0_USERMD;


}

uint32_t SPIC_AutoMode_EnterEngineer(const struct device *dev)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;

	spic_usermode(dev);

    spic_reg->VALIDCMD = 0x000007E0;    // Enter Engineer mode
    spic_reg->VALIDCMD  = 0x00000600;   // Enter Engineer mode
    spic_reg->VALIDCMD  = 0x00000000;   // enable auto write enable command and read status command

    if (!(spic_reg->VALIDCMD & 0x80000000ul)) {
        return 1;
    }

    return 0;
}

static void spic_prepare_command(const struct device *dev, const struct qspi_cmd *command,
				 uint32_t tx_size, uint32_t rx_size, uint8_t write)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;
	uint8_t addr_len = user_addr_len[command->address.size];

	spic_flush_fifo(dev);

	/* set SSIENR: deactivate to program this transfer */
	spic_reg->SSIENR = 0UL;

	/* set CTRLR0: TX mode and channel */
	spic_reg->CTRL0 &= ~(TMOD(3) | CMD_CH(3) | ADDR_CH(3) | DATA_CH(3));
	spic_reg->CTRL0 |= TMOD(write == 0x01 ? 0x00UL : 0x03UL) |
			   ADDR_CH(command->address.bus_width) | DATA_CH(command->data.bus_width);

	/* set USER_LENGTH */
	spic_reg->USERLENGTH = USER_CMD_LENGTH(1) |
			       USER_ADDR_LENGTH(command->address.disabled ? 0 : addr_len) |
			       USER_RD_DUMMY_LENGTH(command->dummy_count * spic_reg->BAUDR * 2);

	/* Write command */
	if (!command->instruction.disabled) {
		spic_reg->DR.BYTE = command->instruction.value;
	}

	/* Write address */
	if (!command->address.disabled) {
		for (int i = 0; i < addr_len; i++) {
			spic_reg->DR.BYTE =
				(uint8_t)(command->address.value >> (8 * (addr_len - i - 1)));
		}
	}

	/* Set TX_NDF: frame number of Tx data */
	spic_reg->TXNDF = TX_NDF(tx_size);

	/* Set RX_NDF: frame number of receiving data. */
	spic_reg->RXNDF = RX_NDF(rx_size);
}

static void spic_transmit_data(const struct device *dev, const void *data, uint32_t *length)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;

	uint32_t len = *length;

	/* set SSIENR to start the transfer */
	spic_reg->SSIENR = SPIC_SSIENR_SPICEN;

	/* write the remaining data into fifo */
	for (int i = 0; i < len;) {
		if (spic_reg->SR & SPIC_SR_TFNF) {
			spic_reg->DR.BYTE = ((const uint8_t *)data)[i];
			i++;
		}
	}
}

static void spic_receive_data(const struct device *dev, void *data, uint32_t *length)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;

	uint32_t i, cnt, rx_num, fifo, len;
	uint8_t *rx_data = data;

	len = *length;
	rx_data = data;

	/* set SSIENR to start the transfer */
	spic_reg->SSIENR = SPIC_SSIENR_SPICEN;

	rx_num = 0;
	while (rx_num < len) {
		cnt = spic_reg->RXFLR;

		for (i = 0; i < (cnt / 4); i++) {
			fifo = spic_reg->DR.WORD;
			memcpy((void *)(rx_data + rx_num), (void *)&fifo, 4);
			rx_num += 4;
		}

		if (rx_num < len) {
			uint32_t remaining = (len - rx_num < cnt % 4) ? len - rx_num : cnt % 4;

			for (i = 0; i < remaining; i++) {
				*(uint8_t *)(rx_data + rx_num) = spic_reg->DR.BYTE;
				rx_num += 1;
			}
		}
	}
}

static int spic_write(const struct device *dev, const struct qspi_cmd *command, const void *data,
		      uint32_t *length)
{
	int ret;

	//spic_usermode(dev);

	spic_prepare_command(dev, command, *length, 0, COMMAND_WRITE);
	spic_cs_active(dev);

	spic_transmit_data(dev, data, length);
	ret = spic_wait_finish(dev);

	spic_cs_deactivate(dev);
	//spic_automode(dev);

	return ret;
}

static int spic_read(const struct device *dev, const struct qspi_cmd *command, void *data,
		     size_t *length)
{
	int ret;

	//spic_usermode(dev);
	spic_prepare_command(dev, command, 0, *length, COMMAND_READ);
	spic_cs_active(dev);

	spic_receive_data(dev, data, length);
	ret = spic_wait_finish(dev);

	spic_cs_deactivate(dev);
	//spic_automode(dev);

	return ret;
}

static int flash_read_sr(const struct device *dev, uint8_t *val);


static int flash_write_enable(const struct device *dev)
{
	struct flash_rts5918_dev_data *data = dev->data;
	struct qspi_cmd *command = &data->command_default;
	uint32_t len = 0;
	uint8_t sr = 0;
	int ret;
	int timeout = TIMEOUT_SPIBUSY;

	// do {
	// 	ret = flash_read_sr(dev, &sr);
	// 	// if (ret < 0) {
	// 	// 	return ret;
	// 	// }
	// 	if (!(sr & SPI_NOR_WIP_BIT)) {
	// 		break;
	// 	}
	// 	timeout--;
	// } while (timeout > 0);
	// if(timeout==0) {
	// 	LOG_ERR("write_enable: wait WIP clear fail");
	// }

	config_command(command, SPI_NOR_CMD_WREN, 0, 0, 0);
	ret = spic_write(dev, command, 0, &len);
	if (ret < 0) {
		goto exit;
	}

	timeout = TIMEOUT_SPIBUSY;
	do {
		flash_read_sr(dev, &sr);
// 		ret = flash_read_sr(dev, &sr);
// if (ret < 0) {
// 			return ret;
// 		}
		if (!(sr & SPI_NOR_WIP_BIT)) {
			return 0;
		}
		timeout--;
	} while (timeout > 0);

exit:
	return ret;

// ret = spic_write(dev, command, 0, &len);
// 	if (ret < 0) {
// 		goto exit;
// 	}

// 	/* If it's a sector erase loop, it requires approximately 3000 cycles,
// 	 * while a program page requires about 40 cycles.
// 	 */
// 	timeout = TIMEOUT_SPIBUSY;
// 	do {
// 		ret = flash_read_sr(dev, &sr);
// 		// if (ret < 0) {
// 		// 	return ret;
// 		// }
// 		if (sr & SPI_NOR_WEL_BIT) {
// 			return 0;
// 		}
// 		timeout--;
// 	} while (timeout > 0);

// exit:
// 	return ret;
}

static int flash_write_disable(const struct device *dev)
{
	struct flash_rts5918_dev_data *data = dev->data;
	struct qspi_cmd *command = &data->command_default;
	uint32_t len = 0;
	uint8_t sr = 0;
	int ret;
	int timeout = TIMEOUT_SPIBUSY;

	config_command(command, SPI_NOR_CMD_WRDI, 0, 0, 0);

	ret = spic_write(dev, command, NULL, &len);
	if (ret < 0) {
		goto exit;
	}

	timeout = TIMEOUT_SPIBUSY;
	do {
		flash_read_sr(dev, &sr);
// 		ret = flash_read_sr(dev, &sr);
// if (ret < 0) {
// 			return ret;
// 		}
		if (!(sr & SPI_NOR_WIP_BIT)) {
			return 0;
		}
		timeout--;
	} while (timeout > 0);

exit:
	return ret;
}

static int flash_read_sr(const struct device *dev, uint8_t *val)
{
	struct flash_rts5918_dev_data *data = dev->data;
	struct qspi_cmd *command = &data->command_default;
	int status;
	uint32_t len = 1;
	uint8_t sr;

	config_command(command, SPI_NOR_CMD_RDSR, 0, 0, 0);
	status = spic_read(dev, command, &sr, &len);
	if (status) {
		return status;
	}
	*val = sr;

	return 0;
}

#if 1//def CONFIG_FLASH_EX_OP_ENABLED
static int flash_read_sr2(const struct device *dev, uint8_t *val)
{
	struct flash_rts5918_dev_data *data = dev->data;
	struct qspi_cmd *command = &data->command_default;
	int status;
	uint32_t len = 1;
	uint8_t sr;

	config_command(command, SPI_NOR_CMD_RDSR2, 0, 0, 0);
	status = spic_read(dev, command, &sr, &len);
	if (status) {
		return status;
	}
	*val = sr;

	return 0;
}
#endif

#ifdef CONFIG_FLASH_EX_OP_ENABLED
static int flash_set_wp(const struct device *dev, uint8_t *val)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;

	if (!val) {
		return -EINVAL;
	}

	if (*val) {
		spic_reg->CTRLR2 |= SPIC_CTRLR2_WPN_SET;
	}

	return 0;
}

static int flash_get_wp(const struct device *dev, uint8_t *val)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;

	*val = (uint8_t)(spic_reg->CTRLR2 & SPIC_CTRLR2_WPN_SET);

	return 0;
}
#endif

static int flash_wait_till_ready(const struct device *dev)
{
//int ret;
	int timeout = TIMEOUT_SPIBUSY;
	uint8_t sr = 0;

	/* If it's a sector erase loop, it requires approximately 3000 cycles,
	 * while a program page requires about 40 cycles.
	 */
	do {
		flash_read_sr(dev, &sr);
// 		ret = flash_read_sr(dev, &sr);
// if (ret < 0) {
// 			return ret;
// 		}
		if (!(sr & SPI_NOR_WIP_BIT)) {
			return 0;
		}
		timeout--;
	} while (timeout > 0);

	LOG_ERR("Flash wait timed out");
	return -ETIMEDOUT;
}

static int saf_flash_wait_till_ready(const struct device *dev)
{
//int ret;
	int timeout = TIMEOUT_SAFLIMITCNT;
	uint8_t sr = 0;

	/* If it's a sector erase loop, it requires approximately 3000 cycles,
	 * while a program page requires about 40 cycles.
	 */
	do {
		flash_read_sr(dev, &sr);
// 		ret = flash_read_sr(dev, &sr);
// if (ret < 0) {
// 			return ret;
// 		}
		if (!(sr & SPI_NOR_WIP_BIT)) {
			return 0;
		}
		timeout--;
		k_msleep(10);
	} while (timeout > 0);

	LOG_ERR("Flash erase wait timed out");
	return -ETIMEDOUT;
}

static int flash_enter_4byte(const struct device *dev)
{
	struct flash_rts5918_dev_data *data = dev->data;
	struct qspi_cmd *command = &data->command_default;
	uint32_t len = 0,ret;
	spic_usermode(dev);
	config_command(command, SPI_NOR_CMD_4BA, 0, 0, 0);
	ret = spic_write(dev, command, NULL, &len);
	if (ret < 0) {
		goto exit;
	}

	ret = flash_wait_till_ready(dev);
exit:
	// flash_write_disable(dev);
	// LOG_ERR("WRITE 4 BYTE MODE SCUESS");
	spic_automode(dev);
	return ret;
}

static int flash_exit_4byte(const struct device *dev)
{
	struct flash_rts5918_dev_data *data = dev->data;
	struct qspi_cmd *command = &data->command_default;
	uint32_t len = 0,ret;
	spic_usermode(dev);
	config_command(command, SPI_NOR_CMD_EXIT_4BA, 0, 0, 0);
	ret = spic_write(dev, command, NULL, &len);
	if (ret < 0) {
		goto exit;
	}

	ret = flash_wait_till_ready(dev);
exit:
	// flash_write_disable(dev);
	// LOG_ERR("WRITE 4 BYTE MODE SCUESS");
	spic_automode(dev);
	return ret;
}

#if 1//def CONFIG_FLASH_EX_OP_ENABLED
static int flash_write_status_reg(const struct device *dev, uint8_t *val, uint8_t cnt)
{
	struct flash_rts5918_dev_data *data = dev->data;
	struct qspi_cmd *command = &data->command_default;
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;	
	int ret;
	uint32_t len = cnt;
	spic_usermode(dev);
	ret = flash_write_enable(dev);
	if (ret < 0) {
		if ((uintptr_t)spic_reg == 0x40000000) {
			if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
				spic_automode(dev);
			}
		} else {
			spic_automode(dev);
		}
		return ret;
	}

	config_command(command, SPI_NOR_CMD_WRSR, 0, 0, 0);
	ret = spic_write(dev, command, val, &len);
	if (ret < 0) {
		goto exit;
	}

	ret = flash_wait_till_ready(dev);
exit:
	flash_write_disable(dev);
	if ((uintptr_t)spic_reg == 0x40000000) {
		if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
			spic_automode(dev);
		}
	} else {
		spic_automode(dev);
	}
	return ret;
}

static int flash_write_status_reg2(const struct device *dev, uint8_t *val, uint8_t cnt)
{
	struct flash_rts5918_dev_data *data = dev->data;
	struct qspi_cmd *command = &data->command_default;
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;		
	int ret;
	uint32_t len = cnt;
	spic_usermode(dev);
	ret = flash_write_enable(dev);
	if (ret < 0) {
		if ((uintptr_t)spic_reg == 0x40000000) {
			if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
				spic_automode(dev);
			}
		} else {
			spic_automode(dev);
		}
		return ret;
	}

	config_command(command, SPI_NOR_CMD_WRSR2, 0, 0, 0);
	ret = spic_write(dev, command, val, &len);
	if (ret < 0) {
		goto exit;
	}

	ret = flash_wait_till_ready(dev);
exit:
	flash_write_disable(dev);
	if ((uintptr_t)spic_reg == 0x40000000) {
		if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
			spic_automode(dev);
		}
	} else {
		spic_automode(dev);
	}
	return ret;
}
#endif

static int flash_erase_sector(const struct device *dev, uint32_t address, uint8_t type)
{
	struct flash_rts5918_dev_data *data = dev->data;
	struct qspi_cmd *command = &data->command_default;
	// enum spic_address_size addr_size = SPIC_CFG_ADDR_SIZE_24;
enum spic_address_size addr_size = SPIC_CFG_ADDR_SIZE_32;
const struct flash_rts5918_dev_config *config = dev->config;
volatile struct reg_spic_reg *spic_reg = config->regs;
	int ret;
	uint32_t len = 0;
	spic_usermode(dev);
	if (type == THREEBYTEERASE) {
		config_command(command, SPI_NOR_CMD_EXIT_4BA, 0, 0, 0);
		ret = spic_write(dev, command, NULL, &len);

		ret = flash_wait_till_ready(dev);
		if (ret != 0) {
			printk("exit 4byte addr: 4BA failed %d!", ret);
			//return ret;
		}				
	}	
	ret = flash_write_enable(dev);
	if (ret < 0) {
		if ((uintptr_t)spic_reg == 0x40000000) {
			if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
				spic_automode(dev);
			}
		} else {
			spic_automode(dev);
		}
		return ret;
	}
	if (type == FOURBYTEERASE) {
		config_command(command, SPI_NOR_CMD_SE_4B, address, SPIC_CFG_ADDR_SIZE_32, 0);
	} else {
		config_command(command, SPI_NOR_CMD_SE, address, SPIC_CFG_ADDR_SIZE_24, 0);
	}
	ret = spic_write(dev, command, NULL, &len);
	if (ret < 0) {
		goto err_exit;
	}
	ret = flash_wait_till_ready(dev);

err_exit:
	flash_write_disable(dev);
	if (type == THREEBYTEERASE && ((uintptr_t)spic_reg == 0x40000000)) {
		config_command(command, SPI_NOR_CMD_4BA, 0, 0, 0);
		ret = spic_write(dev, command, NULL, &len);

		ret = flash_wait_till_ready(dev);
		if (ret != 0) {
			printk("Enable 4byte addr: 4BA failed %d!", ret);
			//return ret;
		}				
	}		

	if ((uintptr_t)spic_reg == 0x40000000) {
		if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
			spic_automode(dev);
		}
	} else {
		spic_automode(dev);
	}
	// flash_write_disable(dev);
	return ret;
}

static int saf_flash_erase_sector(const struct device *dev, uint32_t address, uint8_t type)
{
	struct flash_rts5918_dev_data *data = dev->data;
	struct qspi_cmd *command = &data->command_default;
	// enum spic_address_size addr_size = SPIC_CFG_ADDR_SIZE_24;
enum spic_address_size addr_size = SPIC_CFG_ADDR_SIZE_32;
const struct flash_rts5918_dev_config *config = dev->config;
volatile struct reg_spic_reg *spic_reg = config->regs;
	int ret;
	uint32_t len = 0;
	spic_usermode(dev);
	if (type == THREEBYTEERASE) {
		config_command(command, SPI_NOR_CMD_EXIT_4BA, 0, 0, 0);
		ret = spic_write(dev, command, NULL, &len);

		ret = flash_wait_till_ready(dev);
		if (ret != 0) {
			printk("exit 4byte addr: 4BA failed %d!", ret);
			//return ret;
		}				
	}	
	ret = flash_write_enable(dev);
	if (ret < 0) {
		if ((uintptr_t)spic_reg == 0x40000000) {
			if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
				spic_automode(dev);
			}
		} else {
			spic_automode(dev);
		}
		return ret;
	}
	if (type == FOURBYTEERASE) {
		config_command(command, SPI_NOR_CMD_SE_4B, address, SPIC_CFG_ADDR_SIZE_32, 0);
	} else {
		config_command(command, SPI_NOR_CMD_SE, address, SPIC_CFG_ADDR_SIZE_24, 0);
	}
	ret = spic_write(dev, command, NULL, &len);
	if (ret < 0) {
		goto err_exit;
	}
	
	k_msleep(30);
	ret = saf_flash_wait_till_ready(dev);
err_exit:
	flash_write_disable(dev);
	if (type == THREEBYTEERASE && ((uintptr_t)spic_reg == 0x40000000)) {
		config_command(command, SPI_NOR_CMD_4BA, 0, 0, 0);
		ret = spic_write(dev, command, NULL, &len);

		ret = flash_wait_till_ready(dev);
		if (ret != 0) {
			printk("Enable 4byte addr: 4BA failed %d!", ret);
			//return ret;
		}				
	}		

	if ((uintptr_t)spic_reg == 0x40000000) {
		if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
			spic_automode(dev);
		}
	} else {
		spic_automode(dev);
	}
	// flash_write_disable(dev);
	return ret;
}

/*
 * W25Q Security Register helpers.
 *
 * Addresses are 24-bit on Winbond regardless of whether the main array
 * is in 3-byte or 4-byte mode, so these always use SPIC_CFG_ADDR_SIZE_24.
 * Read uses cmd 0x48 with 8 dummy cycles; program/erase are WREN-gated
 * and poll WIP after the op.
 *
 * Restoring auto-mode at the end mirrors flash_erase_sector /
 * flash_program_page: SPIC0 only re-enters auto-mode when the platform
 * register at 0x402301e4 bit 8 is still asserted (matches the "external
 * flash ready for memory-mapped access" latch checked elsewhere).
 */

static inline void spic_restore_automode(const struct device *dev)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;

	if ((uintptr_t)spic_reg == 0x40000000) {
		if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
			spic_automode(dev);
		}
	} else {
		spic_automode(dev);
	}
}

static int flash_read_sec_reg(const struct device *dev, uint32_t address,
			      void *data, uint32_t length)
{
	struct flash_rts5918_dev_data *dev_data = dev->data;
	struct qspi_cmd *command = &dev_data->command_default;
	int ret;
	uint32_t len = length;

	if (data == NULL || length == 0 || length > SEC_REG_PAGE_BYTES) {
		return -EINVAL;
	}

	spic_usermode(dev);
	config_command(command, FLASH_CMD_RDSECREG, address,
		       SPIC_CFG_ADDR_SIZE_24, SEC_REG_READ_DUMMY);
	ret = spic_read(dev, command, data, (size_t *)&len);
	spic_restore_automode(dev);
	return ret;
}

static int flash_program_sec_reg(const struct device *dev, uint32_t address,
				 const void *data, uint32_t length)
{
	struct flash_rts5918_dev_data *dev_data = dev->data;
	struct qspi_cmd *command = &dev_data->command_default;
	int ret;
	uint32_t len = length;

	if (data == NULL || length == 0 || length > SEC_REG_PAGE_BYTES) {
		return -EINVAL;
	}

	spic_usermode(dev);
	ret = flash_write_enable(dev);
	if (ret < 0) {
		goto out;
	}

	config_command(command, FLASH_CMD_PPSECREG, address,
		       SPIC_CFG_ADDR_SIZE_24, 0);
	ret = spic_write(dev, command, data, &len);
	if (ret < 0) {
		goto out_disable;
	}
	ret = flash_wait_till_ready(dev);

out_disable:
	flash_write_disable(dev);
out:
	spic_restore_automode(dev);
	return ret;
}

static int flash_erase_sec_reg(const struct device *dev, uint32_t address)
{
	struct flash_rts5918_dev_data *dev_data = dev->data;
	struct qspi_cmd *command = &dev_data->command_default;
	int ret;
	uint32_t len = 0;

	spic_usermode(dev);
	ret = flash_write_enable(dev);
	if (ret < 0) {
		goto out;
	}

	config_command(command, FLASH_CMD_ERSECREG, address,
		       SPIC_CFG_ADDR_SIZE_24, 0);
	ret = spic_write(dev, command, NULL, &len);
	if (ret < 0) {
		goto out_disable;
	}
	ret = flash_wait_till_ready(dev);

out_disable:
	flash_write_disable(dev);
out:
	spic_restore_automode(dev);
	return ret;
}

struct qspi_cmd_set {
	uint8_t read;
	uint8_t program;
	uint8_t dummy;
};
/* qspi_mode 對應的指令 */
static const struct qspi_cmd_set qspi_cmd_table[] = {
	/* mode 0: 1-1-1 */
	[0] = {
		.read = SPI_NOR_CMD_READ,
		.program = SPI_NOR_CMD_PP,
		.dummy = 0,
	},
	/* mode 1: 1-1-4 */
	[1] = {
		.read = SPI_NOR_CMD_QREAD,
		.program = SPI_NOR_CMD_PP_1_1_4,
		.dummy = 8,
	},
	/* mode 2: 1-4-4 */
	[2] = {
		.read = SPI_NOR_CMD_4READ,
		.program = SPI_NOR_CMD_PP_1_4_4,
		.dummy = 8,
	},
};

static inline const struct qspi_cmd_set *
flash_get_qspi_cmd(const struct device *dev)
{
	const struct flash_rts5918_dev_config *cfg = dev->config;

	//if (cfg->qspi_mode >= ARRAY_SIZE(qspi_cmd_table)) {
		return &qspi_cmd_table[0]; /* fallback 1-1-1 */
	//}

	//return &qspi_cmd_table[cfg->qspi_mode];
}

static int flash_program_page(const struct device *dev, uint32_t address, const uint8_t *data,
			      uint32_t size)
{
	struct flash_rts5918_dev_data *dev_data = dev->data;
	struct qspi_cmd *command = &dev_data->command_default;
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;		
	enum spic_address_size addr_size = SPIC_CFG_ADDR_SIZE_24;
	int ret = 0;
	uint32_t offset = 0, chunk = 0, page_size = FLASH_PAGE_SZ;
	spic_usermode(dev);
	while (size > 0) {
		ret = flash_write_enable(dev);
		if (ret < 0) {
			if ((uintptr_t)spic_reg == 0x40000000) {
				if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
					spic_automode(dev);
				}
			} else {
				spic_automode(dev);
			}
			return ret;
		}

		offset = address % page_size;
		chunk = (offset + size < page_size) ? size : (page_size - offset);

		// config_command(command, SPI_NOR_CMD_PP, address, addr_size, 0);
		const struct qspi_cmd_set *cmd;

		cmd = flash_get_qspi_cmd(dev);

		config_command(command,
				cmd->program,
				address,
				addr_size,
				0);

		ret = spic_write(dev, command, data, (size_t *)&chunk);
		if (ret < 0) {
			goto err_exit;
		}

		data += chunk;
		address += chunk;
		size -= chunk;

		flash_wait_till_ready(dev);
	}

err_exit:
	flash_write_disable(dev);
	if ((uintptr_t)spic_reg == 0x40000000) {
		if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
			spic_automode(dev);
		}
	} else {
		spic_automode(dev);
	}
	return ret;
}

// static int flash_normal_read(const struct device *dev, uint8_t rdcmd, uint32_t address,
// 			     uint8_t *data, uint32_t size)
static int flash_normal_read(const struct device *dev,
			     uint8_t rdcmd,
			     uint8_t dummy,
			     uint32_t address,
			     uint8_t *data,
			     uint32_t size)
{
	struct flash_rts5918_dev_data *dev_data = dev->data;
	struct qspi_cmd *command = &dev_data->command_default;
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;		
	enum spic_address_size addr_size = SPIC_CFG_ADDR_SIZE_24;
	int ret;

	uint32_t src_addr = address;
	uint8_t *dst_idx = data;

	uint32_t remind_size = size;
	uint32_t block_size = 0x8000UL;
	// uint8_t dummy_count = (rdcmd == SPI_NOR_CMD_READ) ? 0 : 8;

	// config_command(command, rdcmd, src_addr, addr_size, dummy_count);
	config_command(command, rdcmd, src_addr, addr_size, dummy);
	spic_usermode(dev);
	while (remind_size > 0) {
		command->address.value = src_addr;

		if (remind_size >= block_size) {
			ret = spic_read(dev, command, dst_idx, (size_t *)&block_size);
			src_addr += block_size;
			remind_size -= block_size;
			dst_idx += block_size;
		} else {
			ret = spic_read(dev, command, dst_idx, (size_t *)&remind_size);
			dst_idx += remind_size;
			remind_size = 0;
		}

		if (ret < 0) {
			if ((uintptr_t)spic_reg == 0x40000000) {
				if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
					spic_automode(dev);
				}
			} else {
				spic_automode(dev);
			}
			return ret;
		}
	}
	if ((uintptr_t)spic_reg == 0x40000000) {
		if ((*(volatile uint32_t *)(0x402301e4) & (0x1 << 8)) == (0x1 << 8)) {
			spic_automode(dev);
		}
	} else {
		spic_automode(dev);
	}
	return 0;
}

static int check_boundary(off_t offset, size_t len)
{
	// if (offset < 0) {
		// 	return -EINVAL;
	// }

	// if (offset >= (DT_REG_ADDR(SOC_NV_FLASH_NODE) + DT_REG_SIZE(SOC_NV_FLASH_NODE))) {
		// 	return -EINVAL;
	// }

	// if (len > ((DT_REG_ADDR(SOC_NV_FLASH_NODE) + DT_REG_SIZE(SOC_NV_FLASH_NODE))-offset)) {
		// 	return -EINVAL;
	// }

	return 0;
}

static int flash_rts5918_erase(const struct device *dev, off_t offset, size_t len)
{
	struct flash_rts5918_dev_data *data = dev->data;
	int ret = -EINVAL;

	if (len == 0) {
		return 0;
	}
	//return 0;
	// if ((offset % FLASH_ERASE_BLK_SZ) != 0) {
		// 	return -EINVAL;
	// }

	// if ((len % FLASH_ERASE_BLK_SZ) != 0) {
		// 	return -EINVAL;
	// }

	// ret = check_boundary(offset, len);
	// if (ret < 0) {
		// 	return ret;
	// }


	// k_sem_take(&data->sem, K_FOREVER);

	// for (; len > 0; len -= FLASH_ERASE_BLK_SZ) {
		//flash_rts5918_saf_erase_sector_handler(dev, offset);
	if (offset > 0xFFFFFF) {
		ret = flash_erase_sector(dev, offset, FOURBYTEERASE);
	} else {
		ret = flash_erase_sector(dev, offset, THREEBYTEERASE);
	}
		// if (ret < 0) {
			// 	LOG_ERR("erase @0x%08lx fail", offset);
		// }
		// offset += FLASH_ERASE_BLK_SZ;
	// }


	// k_sem_give(&data->sem);

	return ret;
}

static int flash_rts5918_write(const struct device *dev, off_t offset, const void *data, size_t len)
{
	struct flash_rts5918_dev_data *dev_data = dev->data;
	int ret;
	unsigned int key;

	if (len == 0) {
		return 0;
	}

	ret = check_boundary(offset, len);
	if (ret < 0) {
		return ret;
	}

	k_sem_take(&dev_data->sem, K_FOREVER);
	key = irq_lock();
	ret = flash_program_page(dev, offset, data, len);
	irq_unlock(key);
	k_sem_give(&dev_data->sem);

	return ret;
}

static int flash_rts5918_read(const struct device *dev, off_t offset, void *data, size_t len)
{
	struct flash_rts5918_dev_data *dev_data = dev->data;
	int ret;

	if (len == 0) {
		return 0;
	}

	ret = check_boundary(offset, len);
	if (ret < 0) {
		return ret;
	}

	// k_sem_take(&dev_data->sem, K_FOREVER);
	// ret = flash_normal_read(dev, SPI_NOR_CMD_READ, offset, data, len);
	const struct qspi_cmd_set *cmd;

	cmd = flash_get_qspi_cmd(dev);

	ret = flash_normal_read(dev,
			cmd->read,
			cmd->dummy,
			offset,
			data,
			len);
	// k_sem_give(&dev_data->sem);

	return ret;
}

static const struct flash_parameters *flash_rts5918_get_parameters(const struct device *dev)
{
	const struct flash_rts5918_dev_config *config = dev->config;

	return &config->flash_rts5918_parameters;
}

#if defined(CONFIG_FLASH_PAGE_LAYOUT)
static const struct flash_pages_layout dev_layout = {
	.pages_count =
		DT_REG_SIZE(SOC_NV_FLASH_NODE) / DT_PROP(SOC_NV_FLASH_NODE, erase_block_size),
	.pages_size = DT_PROP(SOC_NV_FLASH_NODE, erase_block_size),
};

static void flash_rts5918_pages_layout(const struct device *dev,
				       const struct flash_pages_layout **layout,
				       size_t *layout_size)
{
	*layout = &dev_layout;
	*layout_size = 1;
}
#endif /* CONFIG_FLASH_PAGE_LAYOUT */

#ifdef CONFIG_FLASH_EX_OP_ENABLED
static int flash_rts5918_ex_op(const struct device *dev, uint16_t opcode, const uintptr_t in,
			       void *out)
{
	// struct flash_rts5918_dev_data *dev_data = dev->data;
	int ret = -EINVAL;

	// k_sem_take(&dev_data->sem, K_FOREVER);

	switch (opcode) {
	case FLASH_RTS5918_EX_OP_WR_ENABLE:
		ret = flash_write_enable(dev);
		break;
	case FLASH_RTS5918_EX_OP_WR_DISABLE:
		ret = flash_write_disable(dev);
		break;
	case FLASH_RTS5918_EX_OP_WR_SR:
		ret = flash_write_status_reg(dev, (uint8_t *)out, 1);
		break;
	case FLASH_RTS5918_EX_OP_WR_SR2:
		ret = flash_write_status_reg2(dev, (uint8_t *)out, 1);
		break;
	case FLASH_RTS5918_EX_OP_RD_SR:
		ret = flash_read_sr(dev, (uint8_t *)in);
		break;
	case FLASH_RTS5918_EX_OP_RD_SR2:
		ret = flash_read_sr2(dev, (uint8_t *)in);
		break;
	case FLASH_RTS5918_EX_OP_SET_WP:
		ret = flash_set_wp(dev, (uint8_t *)out);
		break;
	case FLASH_RTS5918_EX_OP_GET_WP:
		ret = flash_get_wp(dev, (uint8_t *)in);
		break;
	case FLASH_RTS5918_EX_OP_4BYTE_MODE:
		ret = flash_enter_4byte(dev);
		break;
	case FLASH_RTS5918_EX_OP_3BYTE_MODE:
		ret = flash_exit_4byte(dev);
		break;
	case FLASH_RTS5918_EX_OP_SELECT_CS: {
		struct flash_rts5918_dev_data *dev_data = dev->data;
		uint8_t cs = (uint8_t)(in & 0xFFU);

		if (cs > 1U) {
			ret = -EINVAL;
			break;
		}
		dev_data->cs = cs;
		ret = 0;
		break;
	}
	case FLASH_RTS5918_EX_OP_SEC_REG_READ: {
		const struct flash_rts5918_sec_reg_op *op =
			(const struct flash_rts5918_sec_reg_op *)in;

		if (op == NULL) {
			ret = -EINVAL;
			break;
		}
		ret = flash_read_sec_reg(dev, op->address, op->buf, op->length);
		break;
	}
	case FLASH_RTS5918_EX_OP_SEC_REG_WRITE: {
		const struct flash_rts5918_sec_reg_op *op =
			(const struct flash_rts5918_sec_reg_op *)in;

		if (op == NULL) {
			ret = -EINVAL;
			break;
		}
		ret = flash_program_sec_reg(dev, op->address, op->buf,
					    op->length);
		break;
	}
	case FLASH_RTS5918_EX_OP_SEC_REG_ERASE: {
		const struct flash_rts5918_sec_reg_op *op =
			(const struct flash_rts5918_sec_reg_op *)in;

		if (op == NULL) {
			ret = -EINVAL;
			break;
		}
		ret = flash_erase_sec_reg(dev, op->address);
		break;
	}
	}

	// k_sem_give(&dev_data->sem);

	return ret;
}
#endif
/*
static DEVICE_API(flash, flash_rts5918_api) = {
	.erase = flash_rts5918_erase,
	.write = flash_rts5918_write,
	.read = flash_rts5918_read,
	.get_parameters = flash_rts5918_get_parameters,
#ifdef CONFIG_FLASH_PAGE_LAYOUT
	.page_layout = flash_rts5918_pages_layout,
#endif
#ifdef CONFIG_FLASH_EX_OP_ENABLED
	.ex_op = flash_rts5918_ex_op,
#endif
};
*/
static const struct flash_driver_api flash_rts5918_api = {
	.erase = flash_rts5918_erase,
	.write = flash_rts5918_write,
	.read = flash_rts5918_read,
	.get_parameters = flash_rts5918_get_parameters,
#ifdef CONFIG_FLASH_PAGE_LAYOUT
	.page_layout = flash_rts5918_pages_layout,
#endif
#ifdef CONFIG_FLASH_EX_OP_ENABLED
	.ex_op = flash_rts5918_ex_op,
#endif
};

static int flash_set_qspi_mode(const struct device *dev)
{
	struct flash_rts5918_dev_data *data = dev->data;
	volatile struct reg_spic_reg *regs = ((struct flash_rts5918_dev_config*)dev->config)->regs;
	struct qspi_cmd *cmd = &data->command_default;
	uint8_t mode = 0;

	switch (mode) {
	case 0: /* 1-1-1 */
		cmd->instruction.bus_width = SPIC_CFG_BUS_SINGLE;
		cmd->address.bus_width = SPIC_CFG_BUS_SINGLE;
		cmd->data.bus_width = SPIC_CFG_BUS_SINGLE;
		break;
	case 1: /* 1-1-4 */
		cmd->instruction.bus_width = SPIC_CFG_BUS_SINGLE;
		cmd->address.bus_width = SPIC_CFG_BUS_SINGLE;
		cmd->data.bus_width = SPIC_CFG_BUS_QUAD;
		regs->CTRL0 |= (0x2 << 18);
		/* enable QE bit */
	{
		uint8_t sr2;
		flash_read_sr2(dev, &sr2);
		if (!(sr2 & BIT(1))) { /* QE bit = 1 */
			sr2 |= BIT(1);
			flash_write_status_reg2(dev, &sr2, 1);
		}
	}
		break;
	case 2: /* 1-4-4 */
		cmd->instruction.bus_width = SPIC_CFG_BUS_SINGLE;
		cmd->address.bus_width = SPIC_CFG_BUS_QUAD;
		cmd->data.bus_width = SPIC_CFG_BUS_QUAD;
		regs->CTRL0 |= (0x2 << 18) | (0x2 << 16);
		/* enable QE bit */
	{
		uint8_t sr2;
		flash_read_sr2(dev, &sr2);
		if (!(sr2 & BIT(1))) {
			sr2 |= BIT(1);
			flash_write_status_reg2(dev, &sr2, 1);
		}
	}
		break;
	default:
		return -EINVAL;
	}

	{
		uint8_t sr2_2;
		flash_read_sr2(dev, &sr2_2);
		if (!(sr2_2 & BIT(1))) {
			LOG_ERR("Failed to set QE bit in SR2");
			return -EIO;
		}
	}
	return 0;
}

static int flash_rts5918_init(const struct device *dev)
{
	const struct flash_rts5918_dev_config *config = dev->config;
	volatile struct reg_spic_reg *spic_reg = config->regs;
	struct flash_rts5918_dev_data *data = dev->data;
	uint32_t ret = 0;
	int rc;

	/* Setup SPIC pins.
	 *
	 * An instance with no pinctrl-0 (e.g. the internal-eflash SPIC1
	 * controller on Bison, whose SER lines are not brought out) will
	 * make pinctrl_apply_state return -ENOENT. That's expected: the
	 * internal flash has no pads to mux, so we log-and-continue
	 * instead of failing init.
	 */
	rc = pinctrl_apply_state(config->pcfg, PINCTRL_STATE_DEFAULT);
	if (rc == -ENOENT) {
		LOG_WRN("SPIC @%p: no pinctrl-0 (internal eflash?)", spic_reg);
	} else if (rc < 0) {
		LOG_ERR("SPIC pinctrl setup failed (%d)", rc);
		return rc;
	}


	spic_reg->SSIENR = 0UL;
	spic_reg->IMR = 0UL;

	spic_reg->CTRL0 = ((spic_reg->CTRL0 & SPIC_CTRL0_CK_MTIMES_Msk) | CMD_CH(0) | DATA_CH(0) |
			   ADDR_CH(0) | MODE(0) | ((spic_reg->CTRL0 & SPIC_CTRL0_SIPOL_Msk)));

	spic_reg->BAUDR = 1UL;
	spic_reg->FBAUD = 1UL;

	k_sem_init(&data->sem, 1, 1);

	if (!(spic_reg->VALIDCMD & 0x80000000ul)) {
		ret = SPIC_AutoMode_EnterEngineer(dev);
		if(ret){
			LOG_ERR("Enter engineer mode failed %d!", ret);
			return ret;
		}
    }

	if (config->enter_4ba != 0) {

	}

	bool wr_en = (config->enter_4ba & 0x02) != 0;
	if (wr_en) {

	}
		// if (wr_en) {
		// 	ret = flash_write_enable(dev);
		// 	LOG_ERR("Enable 4byte addr: WREN!");
		// 	if (ret != 0) {
		// 		LOG_ERR("Enable 4byte addr: WREN failed %d!", ret);
		// 		return ret;
		// 	}
		// }

		/* SPIC for external flash (BIOS) */
		if ((uintptr_t)spic_reg == 0x40000000) {
			ret = flash_enter_4byte(dev);
			printk("flash_enter_4byte!\r\n");
			if (ret != 0) {
				LOG_ERR("Enable 4byte addr: 4BA failed %d!", ret);
				return ret;
			}

			/* Configure SPIC in Quad Read/Program mode */
			spic_reg->VALIDCMD |= SPIC_AUTO_VALIDCMD_RD_QUAD_O | SPIC_AUTO_VALIDCMD_WR_QUAD_I | SPIC_AUTO_VALIDCMD_DUM_EN;

			/* Register Fast Read Quad Output with 4-Byte Address (6Ch) as quad read command */
			spic_reg->RQD &= ~SPIC_READ_QUAD_DATA_RD_QUAD_O_CMD_Msk;
			spic_reg->RQD = (SPI_NOR_CMD_QREAD_4B << SPIC_READ_QUAD_DATA_RD_QUAD_O_CMD_Pos);

			/* Register Quad Input Page Program with 4-Byte Address (34h) as quad page program command */
			spic_reg->WQD &= ~SPIC_READ_QUAD_DATA_WR_QUAD_I_CMD_Msk;
			spic_reg->WQD = (SPI_NOR_CMD_PP_1_1_4_4B << SPIC_READ_QUAD_DATA_WR_QUAD_I_CMD_Pos);

			/* AUTOLENGTH.AUTO_ADDR_LENGTH set to 4 to support 4-byte mode */
			spic_reg->AUTOLENGTH &= ~SPIC_AUTOLENGTH_ADDRLEN_Msk;
			spic_reg->AUTOLENGTH |= (0x4 << SPIC_AUTOLENGTH_ADDRLEN_Pos);

			/* AUTOLENGTH.RD_DUMMY_LENGTH set to 8 cycle to satisfy quad-read requirement */
			spic_reg->AUTOLENGTH &= ~SPIC_AUTOLENGTH_RDDUMMYLEN_Msk;
			spic_reg->AUTOLENGTH |= ((16 * spic_reg->BAUDR) << SPIC_AUTOLENGTH_RDDUMMYLEN_Pos);
			
		}
	//}
	/* according DTS setting mode to enable QE */
	spic_usermode(dev);
	flash_set_qspi_mode(dev);

	spic_automode(dev);

	return 0;
}


void flash_rts5918_saf_write_enable_hardcode(volatile struct reg_spic_reg *spic_reg)
{
  spic_reg->SSIENR = 0;
  spic_reg->CTRL0 = 0x80000000;
  spic_reg->TXNDF = 0;
  spic_reg->RXNDF = 0;
  spic_reg->USERLENGTH = 0x1000;
  spic_reg->DR.BYTE = 0x6;
  spic_reg->SSIENR = 0x3;
  while(spic_reg->SSIENR & 0x1);

  spic_reg->TXFTLR = 0;
  spic_reg->RXFTLR = 0;
  spic_reg->FLUSH = 1;
  spic_reg->SSIENR = 0;
  spic_reg->CTRL0 = 0x0;
}

uint8_t flash_rts5918_saf_read_status_hardcode(volatile struct reg_spic_reg *spic_reg)
{
  uint8_t status;

  spic_reg->SSIENR = 0;
  spic_reg->CTRL0 = 0x80000300;
  spic_reg->TXNDF = 0;
  spic_reg->RXNDF = 1;
  spic_reg->USERLENGTH = 0x1000;
  spic_reg->DR.BYTE = 0x5;
  spic_reg->SSIENR = 0x1;

  spic_reg->IMR |= 0x8;
  spic_reg->RXFTLR = 2;
  spic_reg->SSIENR = 0x1;
  status = spic_reg->DR.BYTE;
  while(spic_reg->SSIENR & 0x1);

  spic_reg->TXFTLR = 0;
  spic_reg->RXFTLR = 0;
  spic_reg->FLUSH = 1;
  spic_reg->SSIENR = 0;
  spic_reg->CTRL0 = 0x0;

  return status;
}

void flash_rts5918_saf_erase_sector_hardcode(volatile struct reg_spic_reg *spic_reg, uint32_t address)
{
	spic_reg->SSIENR = 0;
	spic_reg->CTRL0 = 0x80000000;
	spic_reg->TXNDF = 0;
	spic_reg->RXNDF = 0;
	spic_reg->USERLENGTH = 0x1000;
	spic_reg->DR.BYTE = 33;
	spic_reg->USERLENGTH |= 0x40000;
  	spic_reg->DR.BYTE = (address >> 24) & 0xFF;
  	spic_reg->DR.BYTE = (address >> 16) & 0xFF;
  	spic_reg->DR.BYTE = (address >> 8) & 0xFF;
  	spic_reg->DR.BYTE = address & 0xFF;
	spic_reg->SSIENR = 0x3;
  	while(spic_reg->SSIENR & 0x1);

	spic_reg->TXFTLR = 0;
  	spic_reg->RXFTLR = 0;
  	spic_reg->FLUSH = 1;
  	spic_reg->SSIENR = 0;
  	spic_reg->CTRL0 = 0x0;
}

void flash_rts5918_saf_erase_sector_handler(const struct device *dev, const uint32_t address)
{
	const struct flash_rts5918_dev_config *config = dev->config;
    volatile struct reg_spic_reg *spic_reg = config->regs;
	//LOG_INF("erase handler");
	//LOG_INF("reg base: %x", (uint32_t)spic_reg);
	saf_flash_erase_sector(dev, address, FOURBYTEERASE);
	return ;
	uint32_t mmap_addr = 0x60000000 + address;
	uint8_t status;
	// const struct flash_rts5918_dev_config *config = dev->config;
	// const struct flash_rts5918_dev_config *config = dev->config;
    // volatile struct reg_spic_reg *spic_reg = config->regs;


	// LOG_HEXDUMP_INF(mmap_addr, 32, "read flash");
	 LOG_INF("pre wr_en_1");
	// LOG_INF("pre wr_en_2");
	flash_rts5918_saf_write_enable_hardcode(spic_reg);
	 LOG_INF("wr_en");
	do {
		status = flash_rts5918_saf_read_status_hardcode(spic_reg);
		// LOG_INF("rd_sts %02X", status);
	} while((status & 0x2) == 0);

	// LOG_HEXDUMP_INF(mmap_addr, 32, "read flash");

	flash_rts5918_saf_erase_sector_hardcode(spic_reg, address);
	 LOG_INF("erase 0x%08X", address);

	do {
		status = flash_rts5918_saf_read_status_hardcode(spic_reg);
		 LOG_INF("rd_sts %02X", status);
	} while(status & 0x1);

	// LOG_HEXDUMP_INF(mmap_addr, 32, "read flash");
}

#define RTS5918_FLASH_PINCTRL_DEF(inst) PINCTRL_DT_INST_DEFINE(inst)


#define RTS5918_FLASH_CONFIG(inst)                                             \
	static const struct flash_rts5918_dev_config flash_rts5918_config_##inst = {    \
		.regs = (volatile struct reg_spic_reg *)DT_INST_REG_ADDR(inst),         \
		.flash_rts5918_parameters = {                                        \
				.write_block_size = FLASH_WRITE_BLK_SZ,                      \
				.erase_value = 0xff,                                         \
			},                                                               \
		.enter_4ba = DT_INST_PROP_OR(inst, enter_4byte_addr, 0),                \
		.pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(inst),                           \
	};

#define RTS5918_FLASH_DATA(inst)                                                                     \
	static struct flash_rts5918_dev_data flash_rts5918_data_##inst = {                           \
		.command_default = {                                                                 \
			.instruction = {                                                             \
					.bus_width = SPIC_CFG_BUS_SINGLE,                            \
					.disabled = 0,                                               \
				},                                                                   \
			.address = {                                                                 \
					.bus_width = SPIC_CFG_BUS_SINGLE,                            \
					.size = SPIC_CFG_ADDR_SIZE_24,                               \
					.disabled = 0,                                               \
				},                                                                   \
			.alt = {                                                                     \
					.size = 0,                                                   \
					.disabled = 1,                                               \
				},                                                                   \
			.dummy_count = 0,                                                            \
			.data = {                                                                    \
					.bus_width = SPIC_CFG_BUS_SINGLE,                            \
				},                                                                   \
		},                                                                                   \
		.cs = 0,                                                                             \
	};

#define RTS5918_FLASH_DEVICE_INIT(index)                                                             \
	RTS5918_FLASH_PINCTRL_DEF(index);                                                            \
	RTS5918_FLASH_CONFIG(index);                                                                 \
	RTS5918_FLASH_DATA(index);                                                                   \
	DEVICE_DT_INST_DEFINE(index, &flash_rts5918_init, NULL, &flash_rts5918_data_##index,         \
			      &flash_rts5918_config_##index,                                         \
			      PRE_KERNEL_1, CONFIG_FLASH_INIT_PRIORITY,                              \
			      &flash_rts5918_api);

DT_INST_FOREACH_STATUS_OKAY(RTS5918_FLASH_DEVICE_INIT)
