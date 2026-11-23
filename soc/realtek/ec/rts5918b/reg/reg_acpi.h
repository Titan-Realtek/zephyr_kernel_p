/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_SOC_REALTEK_RTS5918_REG_ACPI_H
#define ZEPHYR_SOC_REALTEK_RTS5918_REG_ACPI_H

struct acpi_reg {
	uint32_t STS;
	uint32_t IB;
	uint32_t OB;
	uint32_t PTADDR;
	uint32_t INTEN;
	uint32_t VWCTRL0;
	uint32_t VWCTRL1;
};

/* STS */
#define ACPI_STS_OBF    BIT(0)
#define ACPI_STS_IBF    BIT(1)
#define ACPI_STS_STS0   BIT(2)
#define ACPI_STS_CMDSEL BIT(3)
#define ACPI_STS_BURST  BIT(4)
#define ACPI_STS_STS2   BIT(5)
#define ACPI_STS_STS3   BIT(6)
#define ACPI_STS_STS4   BIT(7)
#define ACPI_STS_CLR_OBF_FULL   BIT(8)
#define ACPI_STS_CLR_IBF_FULL   BIT(9)
#define ACPI_STS_OBF_FULL_STS   BIT(10)
#define ACPI_STS_CLR_OBF_FULL_STS   BIT(11)
/* IB */
#define ACPI_IB_IBDAT_Pos (0UL)
#define ACPI_IB_IBDAT_Msk GENMASK(7, 0)

/* OB */
#define ACPI_OB_OBDAT_Pos (0UL)
#define ACPI_OB_OBDAT_Msk GENMASK(7, 0)

/* PTADDR */
#define ACPI_PTADDR_ADDR_Pos   (0UL)
#define ACPI_PTADDR_ADDR_Msk   GENMASK(15, 0)
#define ACPI_PTADDR_OFFSET_Pos (16UL)
#define ACPI_PTADDR_OFFSET_Msk GENMASK(18, 16)

/* INTEN */
#define ACPI_INTEN_OBFINTEN BIT(0)
#define ACPI_INTEN_IBFINTEN BIT(1)

/* VWCTRL0 */
#define ACPI_VWCTRL0_IRQEN BIT(0)
#define ACPI_VWCTRL0_TGLV  BIT(1)

/* VWCTRL1 */
#define ACPI_VWCTRL1_IRQNUM_Pos (0UL)
#define ACPI_VWCTRL1_IRQNUM_Msk GENMASK(7, 0)
#define ACPI_VWCTRL1_ACTEN      BIT(8)

#endif /* ZEPHYR_SOC_REALTEK_RTS5918_REG_ACPI_H */
