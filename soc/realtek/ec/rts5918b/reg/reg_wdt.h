/*
 * Copyright (c) 2024 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _REALTEK_RTS5918_REG_WDT_H
#define _REALTEK_RTS5918_REG_WDT_H

/**
  * @brief WDT Controller (WDT)
  */

typedef struct {
	volatile uint32_t CTRL;
	const volatile uint32_t STS;
	volatile uint32_t CNT;
	volatile uint32_t DIV;
	volatile uint32_t INTEN;
} WDT_Type;

/* CTRL */
#define WDT_CTRL_EN_Pos                   (0UL)
#define WDT_CTRL_EN_Msk                   (0x1UL)
#define WDT_CTRL_RSTEN_Pos                (1UL)
#define WDT_CTRL_RSTEN_Msk                (0x2UL)
#define WDT_CTRL_RELOAD_Pos               (2UL)
#define WDT_CTRL_RELOAD_Msk               (0x4UL)
#define WDT_CTRL_CLRRSTFLAG_Pos           (3UL)
#define WDT_CTRL_CLRRSTFLAG_Msk           (0x8UL)
/* STS */
#define WDT_STS_RSTFLAG_Pos               (0UL)
#define WDT_STS_RSTFLAG_Msk               (0x1UL)
/* INTEN */
#define WDT_INTEN_WDTINTEN_Pos            (0UL)
#define WDT_INTEN_WDTINTEN_Msk            (0x1UL)

#endif /* _REALTEK_RTS5918_REG_WDT_H */
