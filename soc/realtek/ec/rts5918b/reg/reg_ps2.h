/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _REALTEK_RTS5918_REG_PS2_H
#define _REALTEK_RTS5918_REG_PS2_H

struct ps2_regs {
    uint32_t CTRL;
    uint32_t TXDAT;
    uint32_t RXDAT;
    uint32_t STS;
    uint32_t INTEN;
};

#define PS2_CTRL_EN_Pos             (0U)
#define PS2_CTRL_MDSEL_Pos          (1U)
#define PS2_CTRL_TXSTR_Pos          (2U)
#define PS2_CTRL_READY_Pos          (3U)
#define PS2_CTRL_RST_Pos            (4U)

#define PS2_TXDAT_DATA_Pos          (0U)
#define PS2_TXDAT_DATA_Msk          GENMASK(7, 0)

#define PS2_RXDAT_DATA_Pos          (0U)
#define PS2_RXDAT_DATA_Msk          GENMASK(7, 0)

#define PS2_STS_STRSTS_Pos          (0U)
#define PS2_STS_TDS_Pos             (1U)
#define PS2_STS_PRTERR_Pos          (2U)
#define PS2_STS_STPERR_Pos          (3U)
#define PS2_STS_BUSY_Pos            (4U)
#define PS2_STS_TXSTRTO_Pos         (5U)
#define PS2_STS_TXTO_Pos            (6U)
#define PS2_STS_RXTO_Pos            (7U)

#define PS2_INTEN_STRINTEN_Pos      (0U)
#define PS2_INTEN_TDSINTEN_Pos      (1U)
#define PS2_INTEN_STSINTEN_Pos      (2U)

#endif /* _REALTEK_RTS5918_REG_PS2_H */