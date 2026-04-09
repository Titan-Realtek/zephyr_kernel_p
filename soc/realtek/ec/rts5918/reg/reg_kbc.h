/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_SOC_REALTEK_RTS5918_REG_KBC_H
#define ZEPHYR_SOC_REALTEK_RTS5918_REG_KBC_H

struct kbc_reg {
	uint32_t STS;
	uint32_t IB;
	uint32_t OB;
	uint32_t PTADDR;
	uint32_t INTEN;
	uint32_t VWCTRL0;
	uint32_t VWCTRL1;
};

/* STS */
#define KBC_STS_OBF    BIT(0)
#define KBC_STS_IBF    BIT(1)
#define KBC_STS_STS0   BIT(2)
#define KBC_STS_CMDSEL BIT(3)
#define KBC_STS_BURST  BIT(4)
#define KBC_STS_STS2   BIT(5)
#define KBC_STS_STS3   BIT(6)
#define KBC_STS_STS4   BIT(7)
#define KBC_STS_CLR_OBF_FULL   BIT(8)
#define KBC_STS_CLR_IBF_FULL   BIT(9)
#define KBC_STS_OBF_FULL_STS   BIT(10)
#define KBC_STS_CLR_OBF_FULL_STS   BIT(11)

/* IB */
#define KBC_IB_IBDAT_Pos (0UL)
#define KBC_IB_IBDAT_Msk GENMASK(7, 0)

/* OB */
#define KBC_OB_OBDAT_Pos (0UL)
#define KBC_OB_OBDAT_Msk GENMASK(7, 0)

/* PTADDR */
#define KBC_PTADDR_DATADDR_Pos (0UL)
#define KBC_PTADDR_DATADDR_Msk GENMASK(15, 0)
#define KBC_PTADDR_CMDOFS_Pos  (16UL)
#define KBC_PTADDR_CMDOFS_Msk  GENMASK(18, 16)

/* INTEN */
#define KBC_INTEN_OBFINTEN_Pos (0UL)
#define KBC_INTEN_OBFINTEN     BIT(0)
#define KBC_INTEN_IBFINTEN_Pos (1UL)
#define KBC_INTEN_IBFINTEN     BIT(1)

/* VWCTRL0 */
#define KBC_VWCTRL0_IRQEN_Pos (0UL)
#define KBC_VWCTRL0_IRQEN     BIT(0)
#define KBC_VWCTRL0_TGLV_Pos  (1UL)
#define KBC_VWCTRL0_TGLV      BIT(1)

/* VWCTRL1 */
#define KBC_VWCTRL1_IRQNUM_Pos (0UL)
#define KBC_VWCTRL1_IRQNUM_Msk GENMASK(7, 0)
#define KBC_VWCTRL1_ACTEN_Pos  (8UL)
#define KBC_VWCTRL1_ACTEN      BIT(8)

#endif /* ZEPHYR_SOC_REALTEK_RTS5918_REG_KBC_H */
