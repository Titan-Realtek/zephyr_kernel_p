/*
 * Copyright (c) 2023 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
#ifndef _REALTEK_RTS5918_REG_MBOX_H
#define _REALTEK_RTS5918_REG_MBOX_H

#define MBX_BASE                    0x400B1600UL
#define MBX                         ((MBX_Type*)               MBX_BASE)

/* =========================================================================================================================== */
/* ================                                            MBX                                            ================ */
/* =========================================================================================================================== */

struct mbox_reg {
  const volatile uint32_t STS;
  volatile uint32_t DATA[48];
  volatile uint32_t INTCTRL;
  volatile uint32_t IRQNUM;
  volatile uint32_t INTSTS;
};

/**
  * @brief Mailbox Controller (MBX)
  */

  typedef struct {                                /*!< (@ 0x400B1600) MBX Structure                                              */
  
    union {
      volatile uint32_t STS;                         /*!< (@ 0x00000000) MAILBOX STATUS REGISTER                                    */
      
      struct {
        volatile uint32_t STS        : 8;            /*!< [7..0] Status                                                             */
                 uint32_t            : 24;
      } STS_b;
    } ;
    volatile uint32_t  DATA[48];                     /*!< (@ 0x00000004) MAILBOX DATA[0..47] REGISTER                               */
    
    union {
      volatile uint32_t INTCTRL;                     /*!< (@ 0x000000C4) MAILBOX INTERRUPT CONTROL REGISTER                         */
      
      struct {
        volatile uint32_t IRQEN      : 1;            /*!< [0..0] Auto IRQ Enable                                                    */
        volatile uint32_t TGLV       : 1;            /*!< [1..1] Trigger Level Select                                               */
        volatile uint32_t INTEN      : 1;            /*!< [2..2] Interrupt Enable                                                   */
                 uint32_t            : 29;
      } INTCTRL_b;
    } ;
    
    union {
      volatile uint32_t IRQNUM;                      /*!< (@ 0x000000C8) MAILBOX IRQ NUMBER REGISTER                                */
      
      struct {
        volatile uint32_t NUM        : 8;            /*!< [7..0] IRQ Number                                                         */
                 uint32_t            : 24;
      } IRQNUM_b;
    } ;
    
    union {
      volatile uint32_t INTSTS;                      /*!< (@ 0x000000CC) MAILBOX INTERRUPT STATUS REGISTER                          */
      
      struct {
        volatile uint32_t STS        : 1;            /*!< [0..0] Indicate wheather the interrupt is occurred                        */
        volatile uint32_t CLR        : 1;            /*!< [1..1] Write 1 to clear interrupt status                                  */
                 uint32_t            : 30;
      } INTSTS_b;
    } ;
  } MBX_Type;                                     /*!< Size = 208 (0xd0)                                                         */

/* =========================================================================================================================== */
/* ================                                            MBX                                            ================ */
/* =========================================================================================================================== */

/* ==========================================================  STS  ========================================================== */
#define MBX_STS_STS_Pos                   (0UL)                     /*!< STS (Bit 0)                                           */
#define MBX_STS_STS_Msk                   (0xffUL)                  /*!< STS (Bitfield-Mask: 0xff)                             */
/* =========================================================  DATA  ========================================================== */
/* ========================================================  INTCTRL  ======================================================== */
#define MBX_INTCTRL_IRQEN_Pos             (0UL)                     /*!< IRQEN (Bit 0)                                         */
#define MBX_INTCTRL_IRQEN_Msk             (0x1UL)                   /*!< IRQEN (Bitfield-Mask: 0x01)                           */
#define MBX_INTCTRL_TGLV_Pos              (1UL)                     /*!< TGLV (Bit 1)                                          */
#define MBX_INTCTRL_TGLV_Msk              (0x2UL)                   /*!< TGLV (Bitfield-Mask: 0x01)                            */
#define MBX_INTCTRL_INTEN_Pos             (2UL)                     /*!< INTEN (Bit 2)                                         */
#define MBX_INTCTRL_INTEN_Msk             (0x4UL)                   /*!< INTEN (Bitfield-Mask: 0x01)                           */
/* ========================================================  IRQNUM  ========================================================= */
#define MBX_IRQNUM_NUM_Pos                (0UL)                     /*!< NUM (Bit 0)                                           */
#define MBX_IRQNUM_NUM_Msk                (0xffUL)                  /*!< NUM (Bitfield-Mask: 0xff)                             */
/* ========================================================  INTSTS  ========================================================= */
#define MBX_INTSTS_STS_Pos                (0UL)                     /*!< STS (Bit 0)                                           */
#define MBX_INTSTS_STS_Msk                (0x1UL)                   /*!< STS (Bitfield-Mask: 0x01)                             */
#define MBX_INTSTS_CLR_Pos                (1UL)                     /*!< CLR (Bit 1)                                           */
#define MBX_INTSTS_CLR_Msk                (0x2UL)                   /*!< CLR (Bitfield-Mask: 0x01)                             */


#endif /* _REALTEK_RTS5918_REG_MBOX_H */
