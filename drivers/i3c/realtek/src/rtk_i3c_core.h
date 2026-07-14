/*
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * Copyright (c) 2025, Realtek Semiconductor Corp. All rights reserved.
 */

#ifndef RTK_I3C_CORE_H_
#define RTK_I3C_CORE_H_

#include "rtk_i3c_internal.h"
#include "rtk_core.h"
#include "rtk_i3c.h"
#include "rtk_logging.h"
#include "rtk_i3c_config.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#define I3C_ISR_DONE_MASK  (1 << 0)
#define I3C_ISR_RXNE_MASK  (1 << 1)
#define I3C_ISR_RXIBI_MASK (1 << 8)
#define I3C_ISR_HJ_MASK    (1 << 9)
#define I3C_ISR_CR_MASK    (1 << 10)
#define I3C_ISR_RXNAK_MASK (1 << 7)
#define I3C_ISR_DAF_MASK   (1 << 15)

#define I3C_ISR_IS_HJorCR(isr_bits)                                                                \
	(((isr_bits) == (I3C_ISR_HJ_MASK | I3C_ISR_DONE_MASK)) ||                                  \
	 ((isr_bits) == (I3C_ISR_CR_MASK | I3C_ISR_DONE_MASK)))

#define IOCR_BUSY_TO_LOW  16
#define IOCR_BUSY_TO_HIGH 16

#define TSSr0_TDS_LOW       22
#define TSSr0_TDS_HIGH      23
#define TSSr0_TCAS_LOW      24
#define TSSr0_TCAS_HIGH     31
#define THPR_TNCRL_LOW      0
#define THPR_TNCRL_HIGH     15
#define THPR_TCRHPOVL_LOW   16
#define THPR_TCRHPOVL_HIGH  31
#define TSTMR_TSTA_TO_LOW   0
#define TSTMR_TSTA_TO_HIGH  31
#define THPTMR_TSDA_TO_LOW  0
#define THPTMR_TSDA_TO_HIGH 31

#define TBCR_TBUF_LOW       0
#define TBCR_TBUF_HIGH      11
#define TINIT_TAVAL_LOW     0
#define TINIT_TAVAL_HIGH    19
#define TINIT_THIGH_OD_LOW  20
#define TINIT_THIGH_OD_HIGH 31
#define TSSr0_TSU_STA_LOW   0
#define TSSr0_TSU_STA_HIGH  10
#define TSSr0_THD_STA_LOW   11
#define TSSr0_THD_STA_HIGH  21
#define TSSr1_TSU_STO_LOW   0
#define TSSr1_TSU_STO_HIGH  11

#define TDLYR_CDLY_LOW  0
#define TDLYR_CDLY_HIGH 7
#define TDLYR_PDLY_LOW  8
#define TDLYR_PDLY_HIGH 15
#define TDLYR_ODY_LOW   16
#define TDLYR_ODY_HIGH  29
#define TDLYR_RDGE_LOW  30
#define TDLYR_RDGE_HIGH 30

#define TDLYR2_PRE_DLY_LOW   0
#define TDLYR2_PRE_DLY_HIGH  7
#define TDLYR2_DDR_PDLY_LOW  8
#define TDLYR2_DDR_PDLY_HIGH 14
#define TDLYR2_DDR_RDGE_LOW  15
#define TDLYR2_DDR_RDGE_HIGH 15
#define TDLYR2_ADLY_LOW      16
#define TDLYR2_ADLY_HIGH     29
#define TDLYR2_RHDY_LOW      31
#define TDLYR2_RHDY_HIGH     31

#define NSEC_PER_SEC 1000000000U

#define ROUND_UP(x, y) (((x) + (y) - 1) / (y))

/**
 * @brief I3C core register structure
 */
typedef struct rtk_i3c_core {
	/* Protocol Control Registers0（Base addr: 0x000） */
	union {
		volatile uint32_t ccr; /**< Configuration Control Register @ 0x00 */
		struct {
			volatile uint32_t role: 1; /**< Indicate the role of I3C */
			volatile uint32_t
				pcr: 1; /**< Indicate the ctroller is primary or secondary */
			volatile uint32_t sctrl: 1;   /**< Enable I3C peripheral as a secondary
							 controller-capable I3C device. */
			volatile uint32_t ixc_dis: 1; /**< Disable I2C/I3C (IXC) function. */
			uint32_t _reserved0: 5;
			volatile uint32_t intr_dis: 1; /**< Disable global interrupts. */
			uint32_t _reserved1: 3;
			volatile uint32_t dma_tx: 1; /**< Enable DMAC TX hardware handshaking. */
			volatile uint32_t dma_rx: 1; /**< Enable DMAC RX hardware handshaking. */
			volatile uint32_t dma: 1;    /**< Enable DMAC hardware handshaking mode. */
			uint32_t _reserved2: 16;
		} ccr_b;
	};
	union {
		volatile uint32_t ipcr; /**< I3C Protocol Control Register @ 0x04 */
		struct {
			volatile uint32_t
				cstall: 1; /**< Enable Role TAGT to stretch SCL in I2C BUS. */
			volatile uint32_t i2c: 1;      /**< Indicate the protocol is I2C. */
			volatile uint32_t hdex_dis: 1; /**< Disable Role CTRL to send HDR EXIT
							  Pattern after receiving NACKed after it
							  sended broadcase write (7E/W). */
			volatile uint32_t ft_od: 1;    /**< Enable Role CTRL to force Timing =
							  Open-Drain (OD) mode. */
			volatile uint32_t hder_pp: 1;  /**< Enable Role CTRL to send header address
							  with  Push-Pull (PP) mode. */
			volatile uint32_t
				nak_dis: 1; /**< Disable RX NACK detection for Role CTRL. */
			volatile uint32_t
				ce_dis: 1; /**< Disable SDR error detction for Role CTRL. */
			volatile uint32_t
				te_dis: 1; /**< Disable SDR error detector for Role TAGT. */
			uint32_t _reserved0: 2;
			volatile uint32_t sa_dis: 1;  /**< Disable Static address for Role
							 TAGT (I3CC.Target). */
			volatile uint32_t ccc_pec: 1; /**< Direct CCC cmd must includes pec byte. */
			uint32_t _reserved1: 4;
			volatile uint32_t rtry_cnt: 2; /**< Read Retry Count for Role CTRL. */
			uint32_t _reserved2: 2;
			volatile uint32_t intel_ext: 4; /**< Enable INTEL spec extend functions. */
			volatile uint32_t intel: 1;     /**< Enable INTEL.spec support. */
			volatile uint32_t par_dis: 1;   /**< Disable Parity Check when
							   JESD.spec is enabled. */
			volatile uint32_t blen: 1;      /**< Enable Burst Length (BL) function when
							   INTEL.Spec      and PEC are enabled. */
			volatile uint32_t bclr: 1;      /**< Enable Bus Clear schemes when
							   JESD.spec is enabled. */
			volatile uint32_t brst: 1;      /**< Enable Bus Reset schemes when
							   JESD.spec is enabled. */
			volatile uint32_t pec: 1;       /**< Enable Packet Error Code (PEC)
							   checking when JESD.spec is enabled. */
			volatile uint32_t jesd: 1;      /**< Enable JESD (JEDEC-1B) spec support. */
			uint32_t _reserved3: 1;
		} ipcr_b;
	};
	union {
		volatile uint32_t iocr; /**< I3C IO Control Register @ 0x08 */
		struct {
			volatile uint32_t sec_ing: 1; /**< Indicated that I3C Secondary Controller
							 Handoff Procedure is going. */
			volatile uint32_t sec_err: 1; /**< Indicated that it occured an error in I3C
							 Secondary Controller Handoff Procedure. */
			uint32_t _reserved0: 1;
			volatile uint32_t scl_od: 1; /**< Enable Role CTRL to force SCL
							output in Open-Drain mode. */
			uint32_t _reserved1: 8;
			volatile uint32_t sda_dly: 4; /**< Set SDA delay count after SCL negedge for
							 Role CTRL to send TX data. */
			volatile uint32_t busy_to: 1; /**< Enable I3CC BUS timeout function. */
			uint32_t _reserved2: 11;
			volatile uint32_t cm_valid: 1; /**< Indicate Communication Mode is going. */
			volatile uint32_t cm_mode: 1;  /**< Enable Communication Mode for
							  Role CTRL/TAGT. */
			volatile uint32_t clpbk: 1;    /**< Enable SCL loop back calibration
							  function for Role CTRL. */
			volatile uint32_t dlpbk: 1;    /**< Enable SDA loop back calibration
							  function for Role CTRL. */
		} iocr_b;
	};
	/* IO Control Registers 1 (Base addr: 0x00C) */
	union {
		volatile uint32_t iocr1; /**< I3C IO Control Register 1 @ 0x0C */
		struct {
			volatile uint32_t eplds: 16; /**< Extended Payload Size. */
			volatile uint32_t tdfltr: 6; /**< Indicate the 50ns Spike Filter in
							I2C TAGT device. */
			volatile uint32_t
				dfltr: 1; /**< Enable SDA spike filter in I2C TAGT device. */
			volatile uint32_t
				cfltr: 1; /**< Enable SCL spike filter for I2C TATG device. */
			uint32_t _reserved0: 4;
			volatile uint32_t
				crc_epush: 1; /**< Enable CRC byte push to RXDA FIFO function
						 in I3C private write transfer. */
			volatile uint32_t
				crc_evalc: 1; /**< Enable CRC byte value translated function in
						 I3C private write transfer. */
			uint32_t _reserved1: 1;
			volatile uint32_t ndf_ext: 1; /**< Enable data frame length extended in I3C
							 private read or write transfers. */
		} iocr1_b;
	};
	/* Control Status Register (Base addr: 0x010) */
	union {
		volatile uint32_t cstr; /**< Control Status Register @ 0x10 */
		struct {
			volatile uint32_t start: 1; /**< I2C/I3C transaction status */
			volatile uint32_t cdpin: 1; /**< Enable Role CTRL to start CDPin mode. */
			volatile uint32_t
				astart: 1; /**< Control structure start async mode0 flow. */
			uint32_t _reserved0: 25;
			volatile uint32_t rflush: 1; /**< Flush RXDA FIFO. */
			volatile uint32_t tflush: 1; /**< Flush TXDA FIFO. */
			volatile uint32_t bflush: 1; /**< Flush BYFM FIFO. */
			volatile uint32_t
				flush: 1; /**< Flush BYFM/TXDA/RXDA and recovery ixc status. */
		} cstr_b;
	};
	/* Clock Baud Rate Register (Base addr: 0x014) */
	union {
		volatile uint32_t cbdr; /**< Clock Baud Rate Register @ 0x14 */
		struct {
			volatile uint32_t bdr_od: 16; /**< Baud rate for Open-Drain mode
							 (OD) in I2C/I3C for Role CTRL. */
			volatile uint32_t bdr_pp: 8;  /**< Baud rate for Push-Pull mode (PP)
							 in I3C BUS for Role CTRL. */
			volatile uint32_t bdr_fst: 8; /**< Baud rate for the first header address in
							 I3C BUS for Role CTRL. */
		} cbdr_b;
	};
	/* Status Registers (Base addr: 0x018) */
	union {
		volatile uint32_t csr; /**< Control Status Register @ 0x18 */
		struct {
			volatile uint32_t busy: 1;   /**< I3C busy flag. */
			volatile uint32_t bbusyn: 1; /**< Indicate the current BUS status. */
			volatile uint32_t
				abusy: 1; /**< Indicate the current status for Async mode0. */
			volatile uint32_t
				amode: 1; /**< Indicate that RXI356_I3CC is in Async mode0. */
			volatile uint32_t rxf: 1;  /**< Indicate that RXDA FIFO is full. */
			volatile uint32_t rxne: 1; /**< Indicate that RXDA FIFO is not empty. */
			volatile uint32_t rxdp: 1; /**< Indicate that RX DATA Drop happens. */
			uint32_t _reserved0: 1;
			volatile uint32_t txe: 1; /**< Indicate that TXDA FIFO is empty. */
			volatile uint32_t txf: 1; /**< Indicate that TXDA FIFO is full. */
			uint32_t _reserved1: 2;
			volatile uint32_t rxnak: 1; /**< Indicate that RXI356_I3CC has
						       received an NACK. */
			volatile uint32_t bce: 1;   /**< Indicate that BYFM CMD FIFO is empty. */
			volatile uint32_t bcf: 1;   /**< Indicate that BYFM CMD FIFO is full. */
			volatile uint32_t
				ccc: 1; /**< Indicate that Role TAGT has received a CCC cmd. */
			volatile uint32_t
				ddr: 1; /**< Indicate that RXI356_I3CC is in HDR-DDR mode. */
			uint32_t _reserved2: 10;
			volatile uint32_t rts_ptn: 1; /**< Indicate that Role TAGT has received an
							 Target Reset Pattern. */
			volatile uint32_t rstrb: 4;   /**< Indicate the strob value for the
							 current APB read transfer. */
		} csr_b;
	};
	/* Interrupt Status Register (Base addr: 0x01C) */
	union {
		volatile uint32_t isr; /**< Interrupt Status Register @ 0x1C */
		struct {
			volatile uint32_t done: 1; /**< Indicate the current I3C/I2C (IXC)
						      transaction status. */
			volatile uint32_t rxne: 1; /**< Indicate that RXDA FIFO status. */
			volatile uint32_t txf: 1;  /**< Indicate that TXDA FIFO status. */
			volatile uint32_t rxdp: 1; /**< Indicate the RX Data Drop status
						      for Role TAGT. */
			volatile uint32_t
				ecom: 1; /**< Indicate the transaction status for Role CTRL. */
			volatile uint32_t byfme: 1; /**< BYFM error status for Role CTRL. */
			volatile uint32_t pare: 1;  /**< Indicate the PAR-bit checking status. */
			volatile uint32_t rxnak: 1; /**< Indicate that RXI356_I3CC has
						       received an address/data NACK. */
			volatile uint32_t
				rxibi: 1; /**< Indicate IBI request status for Role CTRL. */
			volatile uint32_t rxhj: 1;  /**< Indicate Hot-Join request status
						       for Role CTRL. */
			volatile uint32_t rxsec: 1; /**< Indicate the Role request status
						       after master. */
			volatile uint32_t wetm: 1;  /**< Indicate write transcation status. */
			volatile uint32_t retm: 1;  /**< Indicate read transcation status. */
			volatile uint32_t pece: 1;  /**< Indicate PEC CRC Error status. */
			volatile uint32_t rdng: 1;  /**< Indicate the current apb read status. */
			volatile uint32_t daf: 1;   /**< Indicate SDA arbitration status. */
			volatile uint32_t brst: 1;  /**< Indicate Bus Reset status for Role TAGT. */
			volatile uint32_t bclr: 1;  /**< Indicate Bus Clear status for Role TAGT. */
			volatile uint32_t stall_to: 1; /**< Indicate stall timeout event status. */
			volatile uint32_t ddre: 1;     /**< Indicate DDR preamble, parity, CRC
							  errors status. */
			volatile uint32_t ce0: 1;      /**< CE0 interrupt enabled. */
			uint32_t _reserved0: 1;
			volatile uint32_t ce2: 1; /**< CE2 interrupt enabled. */
			volatile uint32_t ce3: 1; /**< CE3 interrupt enabled. */
			volatile uint32_t te0: 1; /**< TE0 interrupt enabled. */
			volatile uint32_t te1: 1; /**< TE1 interrupt enabled. */
			volatile uint32_t te2: 1; /**< TE2 interrupt enabled. */
			volatile uint32_t te3: 1; /**< TE3 interrupt enabled. */
			volatile uint32_t te4: 1; /**< TE4 interrupt enabled. */
			volatile uint32_t te5: 1; /**< TE5 interrupt enabled. */
			uint32_t _reserved3: 2;
		} isr_b;
	};
	/* Raw Interrupt Status Register (Base addr: 0x020) */
	union {
		volatile uint32_t risr; /**< Raw Interrupt Status Register @ 0x20 */
		struct {
			volatile uint32_t done: 1; /**< Indicate the current I3C/I2C (IXC)
						      transaction status. */
			volatile uint32_t rxne: 1; /**< Indicate that RXDA FIFO status. */
			volatile uint32_t txf: 1;  /**< Indicate that TXDA FIFO status. */
			volatile uint32_t rxdp: 1; /**< Indicate the RX Data Drop status
						      for Role TAGT. */
			volatile uint32_t
				ecom: 1; /**< Indicate the transaction status for Role CTRL. */
			volatile uint32_t byfme: 1; /**< BYFM error status for Role CTRL. */
			volatile uint32_t pare: 1;  /**< Indicate the PAR-bit checking status. */
			volatile uint32_t rxnak: 1; /**< Indicate that RXI356_I3CC has
						       received an NACK. */
			volatile uint32_t
				rxibi: 1; /**< Indicate IBI request status for Role CTRL. */
			volatile uint32_t rxhj: 1;  /**< Indicate Hot-Join request status
						       for Role CTRL. */
			volatile uint32_t rxsec: 1; /**< Indicate the Role request status. */
			volatile uint32_t wetm: 1;  /**< Indicate write transcation status. */
			volatile uint32_t retm: 1;  /**< Indicate read transcation status. */
			volatile uint32_t pece: 1;  /**< Indicate PEC CRC Error status. */
			volatile uint32_t rdng: 1;  /**< Indicate the current apb read status. */
			volatile uint32_t dafr: 1;  /**< Indicate SDA arbitration status. */
			volatile uint32_t brst: 1;  /**< Indicate Bus Reset status for Role TAGT. */
			volatile uint32_t bclr: 1;  /**< Indicate Bus Clear status for Role TAGT. */
			volatile uint32_t stall_to: 1; /**< Indicate stall timeout event status. */
			volatile uint32_t ddre: 1;     /**< Indicate DDR preamble, parity, CRC
							  error status. */
			volatile uint32_t ce0: 1;      /**< CE0 interrupt enabled. */
			uint32_t _reserved0: 1;
			volatile uint32_t ce2: 1; /**< CE2 interrupt enabled. */
			volatile uint32_t ce3: 1; /**< CE3 interrupt enabled. */
			volatile uint32_t te0: 1; /**< TE0 interrupt enabled. */
			volatile uint32_t te1: 1; /**< TE1 interrupt enabled. */
			volatile uint32_t te2: 1; /**< TE2 interrupt enabled. */
			volatile uint32_t te3: 1; /**< TE3 interrupt enabled. */
			volatile uint32_t te4: 1; /**< TE4 interrupt enabled. */
			volatile uint32_t te5: 1; /**< TE5 interrupt enabled. */
			uint32_t _reserved3: 2;
		} risr_b;
	};
	/* Interrupt Mask Register (Base addr: 0x024) */
	union {
		volatile uint32_t imr; /**< Interrupt Mask Register @ 0x24 */
		struct {
			volatile uint32_t done: 1;     /**< DONER interrupt enabled. */
			volatile uint32_t rxne: 1;     /**< RXNER interrupt enabled. */
			volatile uint32_t txf: 1;      /**< TXFR interrupt enabled. */
			volatile uint32_t rxdp: 1;     /**< RXDPR interrupt enabled. */
			volatile uint32_t ecom: 1;     /**< ECOMR interrupt enabled. */
			volatile uint32_t byfme: 1;    /**< BYFMER interrupt enabled. */
			volatile uint32_t pare: 1;     /**< PARER interrupt enabled. */
			volatile uint32_t rxnak: 1;    /**< RXNAKR interrupt enabled. */
			volatile uint32_t rxibi: 1;    /**< RXIBIR interrupt enabled. */
			volatile uint32_t rxhj: 1;     /**< RXHJIR interrupt enabled. */
			volatile uint32_t rxsec: 1;    /**< RXSECR interrupt enabled. */
			volatile uint32_t wetm: 1;     /**< WETMR interrupt enabled. */
			volatile uint32_t retm: 1;     /**< RETMR interrupt enabled. */
			volatile uint32_t pece: 1;     /**< PECER interrupt enabled. */
			volatile uint32_t rdng: 1;     /**< RDNGR interrupt enabled. */
			volatile uint32_t daf: 1;      /**< DAFR interrupt enabled. */
			volatile uint32_t brst: 1;     /**< BRSTR interrupt enabled. */
			volatile uint32_t bclr: 1;     /**< BCLRR interrupt enabled. */
			volatile uint32_t stall_to: 1; /**< STALL_TO interrupt enabled. */
			volatile uint32_t ddre: 1;     /**< DDRE interrupt enabled. */
			volatile uint32_t ce0: 1;      /**< CE0 interrupt enabled. */
			uint32_t _reserved0: 1;
			volatile uint32_t ce2: 1; /**< CE2 interrupt enabled. */
			volatile uint32_t ce3: 1; /**< CE3 interrupt enabled. */
			volatile uint32_t te0: 1; /**< TE0 interrupt enabled. */
			volatile uint32_t te1: 1; /**< TE1 interrupt enabled. */
			volatile uint32_t te2: 1; /**< TE2 interrupt enabled. */
			volatile uint32_t te3: 1; /**< TE3 interrupt enabled. */
			volatile uint32_t te4: 1; /**< TE4 interrupt enabled. */
			volatile uint32_t te5: 1; /**< TE5 interrupt enabled. */
			uint32_t _reserved3: 2;
		} imr_b;
	};
	/* Interrupt Clear Register (Base addr: 0x028) */
	union {
		volatile uint32_t icr; /**< Interrupt Clear Register @ 0x28 */
		struct {
			volatile uint32_t done: 1; /**< Write 1 to clear DONE interrupt. */
			uint32_t _reserved0: 2;
			volatile uint32_t rxdp: 1;     /**< Write 1 to clear RXDPR interrupt. */
			volatile uint32_t ecom: 1;     /**< Write 1 to clear ECOMR interrupt. */
			volatile uint32_t byfme: 1;    /**< Write 1 to clear BYFMER interrupt. */
			volatile uint32_t pare: 1;     /**< Write 1 to clear PARER interrupt. */
			volatile uint32_t rxnak: 1;    /**< Write 1 to clear RXNAKR interrupt. */
			volatile uint32_t rxibi: 1;    /**< Write 1 to clear RXIBIR interrupt. */
			volatile uint32_t rxhj: 1;     /**< Write 1 to clear RXHJIR interrupt. */
			volatile uint32_t rxsec: 1;    /**< Write 1 to clear RXSECR interrupt. */
			volatile uint32_t wetm: 1;     /**< Write 1 to clear WETMR interrupt. */
			volatile uint32_t retm: 1;     /**< Write 1 to clear RETMR interrupt. */
			volatile uint32_t pece: 1;     /**< Write 1 to clear PECER interrupt. */
			volatile uint32_t rdng: 1;     /**< Write 1 to clear RDNGR interrupt. */
			volatile uint32_t daf: 1;      /**< Write 1 to clear DAFR interrupt. */
			volatile uint32_t brst: 1;     /**< Write 1 to clear BRSTR interrupt. */
			volatile uint32_t bclr: 1;     /**< Write 1 to clear BCLRR interrupt. */
			volatile uint32_t stall_to: 1; /**< Write 1 to clear STALL_TO interrupt. */
			volatile uint32_t ddre: 1;     /**< Write 1 to clear DDRER interrupt. */
			volatile uint32_t ce0: 1;      /**< Write 1 to clear CE0 interrupt. */
			uint32_t _reserved1: 1;
			volatile uint32_t ce2: 1; /**< Write 1 to clear CE2 interrupt. */
			volatile uint32_t ce3: 1; /**< Write 1 to clear CE3 interrupt. */
			volatile uint32_t te0: 1; /**< Write 1 to clear TE0 interrupt. */
			volatile uint32_t te1: 1; /**< Write 1 to clear TE1 interrupt. */
			volatile uint32_t te2: 1; /**< Write 1 to clear TE2 interrupt. */
			volatile uint32_t te3: 1; /**< Write 1 to clear TE3 interrupt. */
			volatile uint32_t te4: 1; /**< Write 1 to clear TE4 interrupt. */
			volatile uint32_t te5: 1; /**< Write 1 to clear TE5 interrupt. */
			uint32_t _reserved2: 2;
		} icr_b;
	};
	/* Transaction Information Register (Base addr: 0x02C) */
	union {
		volatile uint32_t trinf; /**< Transaction Information Register @ 0x2C */
		struct {
			volatile uint32_t dar: 7;      /**< Indicate the address of IBI/HJ/SEC
							  reqs for Role CTRL. */
			volatile uint32_t rda: 1;      /**< Indicate the Read/Write type of
							  IBI/HJ/SEC reqs for Role CTRL. */
			volatile uint32_t txns_num: 6; /**< Indicate the transfer/cmd
							  number for Role CTRL. */
			uint32_t _reserved0: 1;
			volatile uint32_t ccc: 1; /**< Indicate that Role TAGT has received
						     an CCC CMD. */
			volatile uint32_t
				rxinf: 16; /**< Indicate the current transaction status. */
		} trinf_b;
	};
	/* Bus Timing Register (Base addr: 0x030) */
	union {
		volatile uint32_t tbcr; /**< Bus Timing Register @ 0x30 */
		struct {
			volatile uint32_t
				tbuf: 12;   /**< Bus Free Timing (tBUF) count for Mixed Bus. */
			uint32_t tidle: 20; /**< Bus IDLE Timing (tIDLE) count */
		} tbcr_b;
	};
	/* Initial Timing Register (Base addr: 0x034) */
	union {
		volatile uint32_t tinit; /**< Initial Timing Register @ 0x34 */
		struct {
			volatile uint32_t taval: 20;    /**< Bus Available Timing (tAVAL) count. */
			volatile uint32_t thigh_od: 12; /**< SCL High count for OD timing
							   in I2C/I3C BUS for Role CTRL. */
		} tinit_b;
	};
	/* Timing START Register 0 (Base addr: 0x038) */
	union {
		volatile uint32_t tssr0; /**< Timing START Register 0 @ 0x38 */
		struct {
			volatile uint32_t tsu_sta: 11; /**< tSU_STA timing count in I2C BUS
							  for Role CTRL. */
			volatile uint32_t thd_sta: 11; /**< tHD_STA timing count in I2C BUS
							  & during First Broadcast Address
							  in I3C BUS for Role CTRL. */
			volatile uint32_t tds: 2;      /**< tDS timing count for Role CTRL. */
			volatile uint32_t tcas: 8;     /**< tCAS timing count in only I3C BUS
							  for Role CTRL. */
		} tssr0_b;
	};
	/* Timing STOP Register 1 (Base addr: 0x03C) */
	union {
		volatile uint32_t tssr1; /**< Timing STOP Register 1 @ 0x3C */
		struct {
			volatile uint32_t tsu_sto: 12; /**< tSU_STO timing count in I2C BUS
							  for Role CTRL. */
			uint32_t _reserved0: 20;
		} tssr1_b;
	};
	/* Start Timeout Timing Register (Base addr: 0x040) */
	union {
		volatile uint32_t tstmr; /**< Start Timeout Timing Register @ 0x40 */
		struct {
			volatile uint32_t tsta_to: 32; /**< Timeout count for pulling SCL=Low when
							  SDA is Low at SEC HP flow. */
		} tstmr_b;
	};
	/* Timeout Timing Register (Base addr: 0x044) */
	union {
		volatile uint32_t tdr; /**< Timeout Timing Register @ 0x44 */
		struct {
			uint32_t _reserved0: 16;
			volatile uint32_t tstall_to: 16; /**< Timeout count for SCL stall.
							    Timeout = TSTALL_TO*32. */
		} tdr_b;
	};
	/* Control Role Handoff Process Timing Register (Base addr: 0x048) */
	union {
		volatile uint32_t thpr; /**< Control Role Handoff Process Timing Register @ 0x48 */
		struct {
			volatile uint32_t tcrhpovlp: 16; /**< tCRHPOverlap timing count. After this
							    time delay, the Former active controller
							    should release SCL to High-Z. */
			volatile uint32_t tncrl: 16; /**< tNEWCRLock timing count. During this time
							interval, the New-controller shall not drive
							SDA Low. */
		} thpr_b;
	};
	/* Secondary Handoff Process Timeout Register (Base addr: 0x04C) */
	union {
		volatile uint32_t thptmr; /**< Secondary Handoff Process Timeout Register @ 0x4C */
		struct {
			volatile uint32_t tsda_to: 16; /**< Timeout count for pulling
							  SDA=Low when SEC HP. */
			uint32_t _reserved0: 16;
		} thptmr_b;
	};
	/* Delay Setting Register (Base addr: 0x050) */
	union {
		volatile uint32_t tdlyr; /**< Delay Setting Register @ 0x50 */
		struct {
			volatile uint32_t cdly: 8; /**< SCL Received delay count. */
			volatile uint32_t
				pdly: 8; /**< Indicate the delay count for RX SDA sampling
					    point in address header/data phase with PP mode. */
			volatile uint32_t
				ody: 14; /**< Indicate the delay count for RX SDA sampling
					    point in address header/data phase with OD mode. */
			volatile uint32_t rdge_en: 1; /**< Enable selected sampling point
							 (edge) for RX SDR data capture */
			volatile uint32_t rdge: 1;    /**< Indicate sampling point (edge) for Role
							 CTRL/TAGT    to capture RX SDR data */
		} tdlyr_b;
	};
	/* Delay Setting Register 2 (Base addr: 0x054) */
	union {
		volatile uint32_t tdlyr2; /**< Delay Setting Register 2 @ 0x54 */
		struct {
			volatile uint32_t
				pre_dly: 8; /**< The timing for SCL keep in low before preamble
					       phase in DDR mode (min: BDR_PP). */
			volatile uint32_t
				ddr_pdly: 7; /**< SDA Received delay count for DDR mode. */
			volatile uint32_t ddr_rdge: 1; /**< Indicate capture point in DDR mode. */
			volatile uint32_t adly: 14;    /**< SDA Received delay count for 9th of IBI
							  header/ack/data phase with OD mode. */
			uint32_t _reserved0: 1;
			volatile uint32_t rhdy: 1; /**< Indicate RX SDR data sampling point
						      before or after SCL high edge with
						      Delay count (ODLY/PDLY). */
		} tdlyr2_b;
	};
	/* Bus Reset Timing Information Register (Base addr: 0x058) */
	union {
		volatile uint32_t trst; /**< Bus Reset Timing Information Register @ 0x58 */
		struct {
			volatile uint32_t brst: 20; /**< BUS reset timing count. Timeout =
						       TSTALL_TO*64. */
			uint32_t _reserved0: 12;
		} trst_b;
	};
	/* Early Termination Information Register (Base addr: 0x5C) */
	union {
		volatile uint32_t etm; /**< ETM Register @ 0x5C */
		struct {
			volatile uint32_t cetm_min_len: 8; /**< Minimum length of transaction not to
							      be early terminated. */
			volatile uint32_t cetm_max_len: 8; /**< Maximum length of transaction to be
							      early terminated. */
			volatile uint32_t cetm_en: 1;  /**< Enable CTRL early terminate feature. */
			volatile uint32_t intr_etm: 1; /**< Previous transaction has been
							  early terminated. */
			volatile uint32_t intr_disec: 1; /**< BUS arbitration won by TAGT not known
							    to be Monitoring Device. */
			volatile uint32_t disec_addr: 7; /**< Dynamic address of TAGT which
							    won BUS arbitration. */
			volatile uint32_t
				metm_regin_cnt: 3; /**< Delay cycle of the timing point for
						      monitoring device intervention. */
			uint32_t _reserved0: 3;
		} etm_b;
	};
	uint32_t _reserved0; /**< Reserved space @ 0x60 */

	/* CGATE (Base addr: 0x64) */
	union {
		volatile uint32_t cgate; /**< CGATE Register @ 0x64 */
		struct {
			volatile uint32_t clk_free: 1; /**< Clock gate enable for
							  always-needed signals. */
			volatile uint32_t clk_cr: 1;   /**< Clock gate enable of cr_reg module. */
			volatile uint32_t
				clk_wrap: 1; /**< Clock gate enable of apb_wrapper module. */
			volatile uint32_t
				clk_ixc: 1; /**< Clock gate enable of ixc_controller module. */
			volatile uint32_t
				clk_bfifo: 1; /**< Clock gate enable of byfm_fifo module. */
			volatile uint32_t
				clk_rfifo: 1; /**< Clock gate enable of rxda_fifo module. */
			volatile uint32_t
				clk_tfifo: 1; /**< Clock gate enable of txda_fifo module. */
			volatile uint32_t clk_tic0: 1; /**< Clock gate enable of tic module. */
			volatile uint32_t clk_tic1: 1; /**< Clock gate enable of tic module. */
			volatile uint32_t clk_tphy: 1; /**< Clock gate enable of tx_phy module. */
			volatile uint32_t clk_rphy: 1; /**< Clock gate enable of rx_phy module. */
			uint32_t _reserved0: 21;
		} cgate_b;
	};
	uint32_t _reserved1[2]; /**< Reserved space @ 0x68-0x6C */

	/* Transmit FIFO Threshold/Level Register (Base addr: 0x70) */
	union {
		volatile uint32_t txfl; /**< TXFL Register @ 0x70 */
		struct {
			volatile uint32_t txfl: 16;  /**< Transmit FIFO level. */
			volatile uint32_t txftl: 16; /**< Transmit FIFO threshold level. */
		} txfl_b;
	};

	/* Receive FIFO Threshold/Level Register (Base addr: 0x74) */
	union {
		volatile uint32_t rxfl; /**< RXFL Register @ 0x74 */
		struct {
			volatile uint32_t rxfl: 16;  /**< Receive FIFO level. */
			volatile uint32_t rxftl: 16; /**< Receive FIFO threshold level. */
		} rxfl_b;
	};

	/* DBG Index Register (Base addr: 0x78) */
	union {
		volatile uint32_t dbgi; /**< DBGI Register @ 0x78 */
		struct {
			volatile uint32_t gsel: 2;     /**< Debug access selection. */
			volatile uint32_t gidx: 8;     /**< Debug index of debug FIFO. */
			volatile uint32_t midx: 2;     /**< Index of MDB list groups. */
			volatile uint32_t metm_ndf: 1; /**< Index of early termination NDF. */
			uint32_t _reserved0: 5;
			volatile uint32_t test_mem: 1; /**< Data register enable. */
			volatile uint32_t test: 1;     /**< Test mode enable. */
			volatile uint32_t dpidx: 4;    /**< Debug Port Signal index. */
			uint32_t _reserved1: 2;
			volatile uint32_t ixc_tx: 1;   /**< IXC Send/Receive SDA Data. */
			volatile uint32_t rx_p_ph: 1;  /**< Debug information RX P_PH. */
			volatile uint32_t dbg_s_ph: 1; /**< Debug information S_PH. */
			volatile uint32_t dbg_h_ph: 1; /**< Debug information H_PH. */
			volatile uint32_t dbg_d_ph: 1; /**< Debug information D_PH. */
			volatile uint32_t dbg_p_ph: 1; /**< Debug information P_PH. */
		} dbgi_b;
	};
	/* DBG Data Register (Base addr: 0x7C) */
	union {
		volatile uint32_t dbgr; /**< DBGR Register @ 0x7C */
		struct {
			volatile uint32_t gdr: 32; /**< The debug data value. */
		} dbgr_b;
	};

	/* CCC Control Registers (Base addr: 0x080) */
	/* In-Band Request Capability Register (Base addr: 0x80) */
	union {
		volatile uint32_t ec; /**< EC Register @ 0x80 */
		struct {
			volatile uint32_t ibi: 1; /**< Enable Target Interrupt Requests handler */
			volatile uint32_t cr: 1;  /**< Enable Controller Role requests handler */
			uint32_t _reserved0: 1;
			volatile uint32_t hj: 1; /**< Enable Hot-Join event handler */
			uint32_t _reserved1: 12;
			volatile uint32_t treq_dis: 1; /**< Disable target IBI/HJ */
			uint32_t _reserved2: 6;
			volatile uint32_t mpid: 1; /**< Indicate I3C.PID[31:0] value is
						      used from CR_PIDH and CR_PIDL */
			volatile uint32_t ipls: 8; /**< IBI payload size for
						      CCC.SETMRL/GETMRL if BCR[2] == 1 */
		} ec_b;
	};

	/* Activity State Register (Base addr: 0x84) */
	union {
		volatile uint32_t as; /**< AS Register @ 0x84 */
		struct {
			volatile uint32_t as: 2; /**< Activity status for Role TAGT */
			uint32_t _reserved0: 30;
		} as_b;
	};

	/* Max Write/Read Length Register (Base addr: 0x88) */
	union {
		volatile uint32_t mxl; /**< MXL Register @ 0x88 */
		struct {
			volatile uint32_t mwl: 16; /**< Max write length */
			volatile uint32_t mrl: 16; /**< Max read length */
		} mxl_b;
	};

	/* Provisioned ID Low Register (Base addr: 0x8C) */
	union {
		volatile uint32_t pidl; /**< PIDL Register @ 0x8C */
		struct {
			volatile uint32_t dcr: 8;    /**< Device Characteristics Register */
			volatile uint32_t bcr: 8;    /**< Bus Characteristics Register */
			volatile uint32_t pid_l: 16; /**< Low 16-bit of Target Device
							48-bit Provisioned ID */
		} pidl_b;
	};

	/* Provisioned ID High Register (Base addr: 0x90) */
	union {
		volatile uint32_t pidh; /**< PIDH Register @ 0x90 */
		struct {
			volatile uint32_t pid_h: 32; /**< High 32-bit of Target Device
							48-bit Provisioned ID */
		} pidh_b;
	};

	/* Dynamic Address Status Register (Base addr: 0x94) */
	union {
		volatile uint32_t dsa; /**< DSA Register @ 0x94 */
		struct {
			volatile uint32_t dyn_addr: 7;  /**< Dynamic address */
			volatile uint32_t da1r_dis: 1;  /**< Disable Read/GETCCC ACK for DA = 0x1 */
			volatile uint32_t stc_addr: 10; /**< Static address */
			uint32_t _reserved0: 12;
			volatile uint32_t daa_ing: 1; /**< I3C.dynamic address assignment
							 flow is going */
			volatile uint32_t
				daa_done: 1; /**< I3C.dynamic address assignment is done */
		} dsa_b;
	};

	/* CCC Status Register (Base addr: 0x98) */
	union {
		volatile uint32_t cccst; /**< CCCST Register @ 0x98 */
		struct {
			volatile uint32_t status: 16; /**< Current status */
			uint32_t _reserved0: 16;
		} cccst_b;
	};

	/* Max Data Speed Register (Base addr: 0x9C) */
	union {
		volatile uint32_t mxds; /**< MXDS Register @ 0x9C */
		struct {
			volatile uint32_t mxr: 8; /**< Max read byte */
			volatile uint32_t mxw: 8; /**< Max write byte */
			uint32_t _reserved0: 16;
		} mxds_b;
	};

	/* Bus Information Register (Base addr: 0xA0) */
	union {
		volatile uint32_t busc; /**< BUSC Register @ 0xA0 */
		struct {
			volatile uint32_t buscon: 8; /**< BUS context value */
			uint32_t _reserved0: 16;
			volatile uint32_t hid: 8; /**< HID value for CCC.SETHID */
		} busc_b;
	};

	/* Group Address Register (Base addr: 0xA4) */
	union {
		volatile uint32_t gpa; /**< GPA Register @ 0xA4 */
		struct {
			volatile uint32_t gpa0: 7;   /**< Group address 0 */
			volatile uint32_t gpa0_a: 1; /**< Group address 0 assigned */
			volatile uint32_t gpa1: 7;   /**< Group address 1 */
			volatile uint32_t gpa1_a: 1; /**< Group address 1 assigned */
			volatile uint32_t gpa2: 7;   /**< Group address 2 */
			volatile uint32_t gpa2_a: 1; /**< Group address 2 assigned */
			uint32_t _reserved0: 8;
		} gpa_b;
	};

	/* CASYN Register (Base addr: 0xA8) */
	union {
		volatile uint32_t casyn; /**< CASYN Register @ 0xA8 */
		struct {
			volatile uint32_t
				c_ref: 16; /**< Controller counter from IBI start to the first
					      SCL positive-going edge after the IBI ACK */
			volatile uint32_t c_c2: 8; /**< Controller counter from the first SCL
						      positive-going edge after the IBI ACK to the
						      first SCL positive-going edge after the T-Bit
						      of the IBI's MDB */
			uint32_t _reserved0: 6;
			volatile uint32_t intr_asyn: 1; /**< Indicate ASYN IBI finished and inform
							   user to read CASYN and TASYN */
			volatile uint32_t asyn_fail: 1; /**< Indicate C_REF/C_C2 counter overflow */
		} casyn_b;
	};

	/* TASYN Register (Base addr: 0xAC) */
	union {
		volatile uint32_t tasyn; /**< TASYN Register @ 0xAC */
		struct {
			volatile uint32_t t_c1: 16; /**< Target counter from IBI start to the first
						       SCL positive-going edge after the IBI ACK */
			volatile uint32_t t_c2: 8;  /**< Target counter from the first SCL
						       positive-going edge after the IBI ACK
						       to the first SCL positive-going edge
						       after the T-Bit of the IBI's MDB */
			uint32_t _reserved0: 8;
		} tasyn_b;
	};

	/* DCTRL Register (Base addr: 0xB0) */
	union {
		volatile uint32_t dctrl; /**< DCTRL Register @ 0xB0 */
		struct {
			volatile uint32_t devctrl0: 8; /**< Byte 0 Data payload for CCC.DEVCTRL */
			volatile uint32_t devctrl1: 8; /**< Byte 1 Data payload for CCC.DEVCTRL */
			volatile uint32_t devctrl2: 8; /**< Byte 2 Data payload for CCC.DEVCTRL */
			volatile uint32_t devctrl3: 8; /**< Byte 3 Data payload for CCC.DEVCTRL */
		} dctrl_b;
	};

	/* DCAP Register (Base addr: 0xB4) */
	union {
		volatile uint32_t dcap; /**< DCAP Register @ 0xB4 */
		struct {
			volatile uint32_t devcap: 16; /**< Device capability in JESD.SPEC */
			uint32_t _reserved0: 16;
		} dcap_b;
	};

	/* XTIME Register (Base addr: 0xB8) */
	union {
		volatile uint32_t xtime; /**< XTIME Register @ 0xB8 */
		struct {
			volatile uint32_t xtime0: 8; /**< Inaccuracy Byte */
			volatile uint32_t xtime1: 8; /**< Frequency Byte */
			volatile uint32_t xtime2: 8; /**< State Byte */
			volatile uint32_t xtime3: 8; /**< Supported Modes Byte */
		} xtime_b;
	};

	/* ENDXFER Register (Base addr: 0xBC) */
	union {
		volatile uint32_t endxfer; /**< ENDXFER Register @ 0xBC */
		struct {
			uint32_t _reserved0: 4;
			volatile uint32_t wr_nack: 1; /**< Enable ACK/NACK Capability for
							 WRITE Command */
			volatile uint32_t
				wr_etm: 1; /**< Enable HDR WRITE Early Termination Request */
			volatile uint32_t etm_crc: 2; /**< CRC Word Indicator */
			uint32_t _reserved1: 8;
			volatile uint32_t metm_cap: 8; /**< Indicate Monitoring Device
							  Early Termination Capability */
			uint32_t _reserved2: 8;
		} endxfer_b;
	};

	/* MXL_MIN Register (Base addr: 0xC0) */
	union {
		volatile uint32_t mxl_min; /**< MXL_MIN Register @ 0xC0 */
		struct {
			volatile uint32_t mwl_min: 16; /**< MWL defined minimum value */
			volatile uint32_t mrl_min: 16; /**< MRL defined minimum value */
		} mxl_min_b;
	};

	/* MXDS_RT Register (Base addr: 0xC4) */
	union {
		volatile uint32_t mxds_rt; /**< MXDS_RT Register @ 0xC4 */
		struct {
			volatile uint32_t mxds_rt: 24; /**< 3-byte MaxRdTurn time in us to tell
							  controller how long to wait before reading
							  the data it requested */
			uint32_t _reserved0: 7;
			volatile uint32_t mxds_rten: 1; /**< 1'b1: IP will provide 3-byte RdTurn for
							   CCC.GETMXDS, 1'b0: No MaxRdTurn Bytes */
		} mxds_rt_b;
	};
	/* RSTACT Register (Base addr: 0xC8) */
	union {
		volatile uint32_t rstact; /**< RSTACT Register @ 0xC8 */
		struct {
			volatile uint32_t rstact1: 8; /**< Return time in millisecond for CCC.RSTACT
							 direct get with def_byte = 0x81 */
			volatile uint32_t rstact2: 8; /**< Return time in second for CCC.RSTACT
							 direct get with def_byte = 0x82 */
			uint32_t _reserved1: 8;
			volatile uint32_t rstact3: 8; /**< Reserved */
			uint32_t _reserved0: 8;
			volatile uint32_t rstact0: 8; /**< For Direct SET: Indicate the previous
							 received defining byte (contain 0x0-0x4).
							 For Direct GET: Return byte for CCC.RSTACT
							 direct get with def_byte = 0x0-0x3, 0x80 */
		} rstact_b;
	};
	uint32_t _reserved2[4]; /**< Reserved space @ 0xCC-0xDC */
	/* MEM_DR Register (Base addr: 0xE0) */
	union {
		volatile uint32_t mem_dr; /**< MEM_DR Register @ 0xE0 */
		struct {
			volatile uint32_t mem_dr: 32; /**< Mem-type read/write register */
		} mem_dr_b;
	};
	uint32_t _reserved3[2]; /**< Reserved space @ 0xE4-0xE8 */
	/* RW_CMD Register (Base addr: 0xEC) */
	union {
		volatile uint32_t rw_cmd; /**< RW_CMD Register @ 0xEC */
		struct {
			volatile uint32_t rw_cmd: 8; /**< Vendor - Direct CCCs read/write
							access to CRs */
			uint32_t _reserved0: 24;     /**< Reserved */
		} rw_cmd_b;
	};
	uint32_t _reserved4[3]; /**< Reserved space @ 0xF0-0xF8 */
	/* MDB Register (Base addr: 0xFC) */
	union {
		volatile uint32_t mdb; /**< MDB Register @ 0xFC */
		struct {
			volatile uint32_t mdb: 32; /**< Indicate the value of target device's MDB */
		} mdb_b;
	};

	/* FIFO Control Registers (Base addr: 0x100) */
	volatile uint32_t cfr[16];  /**< Control BYFM Register @ 0x100-0x13C */
	volatile uint8_t tbdfr[64]; /**< Byte-access TXDA register @ 0x140-0x17C */
	volatile uint8_t rbdfr[64]; /**< Byte-access RXDA register @ 0x180-0x1BC */
	uint32_t _reserved5[12];    /**< Reserved space @ 0x1C0-0x1EC */
	/* CAPS Register (Base addr: 0x1F0) */
	union {
		volatile uint32_t caps; /**< CAPS Register @ 0x1F0 */
		struct {
			volatile uint32_t getcap1: 8; /**< GETCAP1 Byte - HDR Mode support */
			volatile uint32_t getcap2: 8; /**< GETCAP2 Byte - HDR-DDR capabilities */
			volatile uint32_t getcap3: 8; /**< GETCAP3 Byte - Various capabilities */
			volatile uint32_t getcap4: 8; /**< GETCAP4 Byte - Reserved */
		} caps_b;
	};

	/* MSG Register (Base addr: 0x1F4) */
	union {
		volatile uint32_t msg; /**< MSG Register @ 0x1F4 */
		struct {
			volatile uint32_t msg: 32; /**< Reserved */
		} msg_b;
	};

	/* GIT Register (Base addr: 0x1F8) */
	union {
		volatile const uint32_t git; /**< GIT Register @ 0x1F8 */
		struct {
			volatile const uint32_t tag_num: 32; /**< Tag number/commit number */
		} git_b;
	};

	/* IDR Register (Base addr: 0x1FC) */
	union {
		volatile const uint32_t idr; /**< IDR Register @ 0x1FC */
		struct {
			volatile const uint32_t ip_num: 16;       /**< IP number (0x0356) */
			volatile const uint32_t release_date: 16; /**< Release date */
		} idr_b;
	};

	/* Protocol Control Registers1 (Base addr: 0x200) */
	uint32_t _reserved6[7]; /**< Reserved space @ 0x200-0x218 */
	/* ISR1 Register (Base addr: 0x21C) */
	union {
		volatile const uint32_t isr1; /**< ISR1 Register @ 0x21C */
		struct {
			uint32_t _reserved0: 4;
			volatile const uint32_t asyn: 1; /**< ASYN bit - IBI async_mode 0 status */
			uint32_t _reserved1: 3;
			volatile const uint32_t etm: 1; /**< ETM bit - Early termination status */
			uint32_t _reserved2: 22;
			volatile const uint32_t cmse: 1; /**< CMSE bit - Data size error status */
		} isr1_b;
	};

	/* RISR1 Register (Base addr: 0x220) */
	union {
		volatile const uint32_t risr1; /**< RISR1 Register @ 0x220 */
		struct {
			uint32_t _reserved0: 4;
			volatile const uint32_t
				asyn: 1; /**< ASYN bit - Raw IBI async_mode 0 status */
			uint32_t _reserved1: 3;
			volatile const uint32_t
				etm: 1; /**< ETM bit - Raw early termination status */
			uint32_t _reserved2: 22;
			volatile const uint32_t
				cmse: 1; /**< CMSE bit - Raw data size error status */
		} risr1_b;
	};

	/* IMR1 Register (Base addr: 0x224) */
	union {
		volatile uint32_t imr1; /**< IMR1 Register @ 0x224 */
		struct {
			uint32_t _reserved0: 4;
			volatile uint32_t asyn: 1; /**< ASYN bit - Interrupt mask */
			uint32_t _reserved1: 3;
			volatile const uint32_t etm: 1; /**< ETM bit - Interrupt mask */
			uint32_t _reserved2: 22;
			volatile uint32_t cmse: 1; /**< CMSE bit - Interrupt mask */
		} imr1_b;
	};

	/* ICR1 Register (Base addr: 0x228) */
	union {
		volatile uint32_t icr1; /**< ICR1 Register @ 0x228 */
		struct {
			uint32_t _reserved0: 4;
			volatile uint32_t asyn: 1; /**< ASYN bit - Clear ASYNR interrupt */
			uint32_t _reserved1: 3;
			volatile uint32_t etm: 1; /**< ETM bit - Clear ETMR interrupt */
			uint32_t _reserved2: 22;
			volatile uint32_t cmse: 1; /**< CMSE bit - Clear CMSER interrupt */
		} icr1_b;
	};
	uint32_t _reserved7[5]; /**< Reserved space @ 0x22C-0x23C */

	/* Extended Protocol Control Registers (Base addr: 0x240) */
	volatile uint32_t twdfr[16]; /**< Word-access TXDA register @ 0x240-0x27C */
	volatile uint32_t rwdfr[16]; /**< Word-access RXDA register @ 0x280-0x2BC */
} rtk_i3c_core;

/**********************************************************************************************************************
 * Register Lelvel Functions
 **********************************************************************************************************************/
static inline void rtk_i3c_core_set_role(rtk_i3c_core *core, uint32_t role)
{
	rtk_core_write32_mask(&core->ccr, 0, 2, role);
}

static inline void rtk_i3c_core_set_intr(rtk_i3c_core *core, bool enable)
{
	rtk_core_write32_mask(&core->ccr, 9, 9, !enable);
}

static inline void rtk_i3c_core_set_global_enable(rtk_i3c_core *core, bool enable)
{
	rtk_core_write32_mask(&core->ccr, 3, 3, !enable);
}

static inline void rtk_i3c_core_set_force_od(rtk_i3c_core *core, bool enable)
{
	rtk_core_write32_mask(&core->ipcr_b, 3, 3, enable);
}

static inline void rtk_i3c_core_set_header_pp(rtk_i3c_core *core, bool enable)
{
	rtk_core_write32_mask(&core->ipcr, 4, 4, (uint32_t)enable);
}

static inline void rtk_i3c_core_set_debug(rtk_i3c_core *core, bool enable)
{
	/* hdex_dis  – bit 2 of ipcr */
	rtk_core_write32_mask(&core->ipcr, 2, 2, (uint32_t)enable);
	/* nak_dis   – bit 5 of ipcr */
	rtk_core_write32_mask(&core->ipcr, 5, 5, (uint32_t)enable);
	/* ce_dis    – bit 6 of ipcr */
	rtk_core_write32_mask(&core->ipcr, 6, 6, (uint32_t)enable);
	/* te_dis    – bit 7 of ipcr */
	rtk_core_write32_mask(&core->ipcr, 7, 7, (uint32_t)enable);
	/* scl_od    – bit 3 of iocr */
	rtk_core_write32_mask(&core->iocr, 3, 3, (uint32_t)enable);
	/* sda_dly   – bits 4‑7 of iocr, set to 1 */
	rtk_core_write32_mask(&core->iocr, 4, 7, (uint32_t)1);
}

static inline void rtk_i3c_core_set_dma(rtk_i3c_core *core, uint32_t dma)
{
	rtk_core_write32_mask(&core->ccr, 13, 15, dma);
}

/**
 * Set the I2C compatitable mode.
 */
static inline void rtk_i3c_core_set_i2c(rtk_i3c_core *core, bool enable)
{
	rtk_core_write32_mask(&core->ipcr, 1, 1, enable);
}

/* OD SCL high-time duty (%). THIGH_OD = duty * (BDR_OD * 2) / 100. */
#ifndef RTK_I3C_OD_DUTY_PCT
#define RTK_I3C_OD_DUTY_PCT 50U
#endif

/*
 * Program the controller baud dividers and the timing that is derived from them
 * (OD high period, sampling delays, edge alignment). Mirrors the IP-recommended
 * master clock setup (i3c_dev_ctl_set_clk):
 *   - BDR_PP / BDR_OD  = Round_up(sysclk / (2 * clk))
 *   - BDR_FST          = Round_up(tHIGH_INIT / tSYSCLK / 4), tHIGH_INIT = 1300 ns
 *   - THIGH_OD         = duty% of the full OD period (BDR_OD * 2 cycles)
 *   - PDLY / ODLY / ADLY sample at the MIDPOINT of the SCL logic state (bdr/2),
 *     with RHDY set (sample SDA after the SCL rising edge) and RDGE cleared.
 * These delay values scale with the divider (tens of cycles); the earlier fixed
 * few-ns values mis-placed the OD sample point and dropped bytes on OD reads
 * (e.g. the ENTDAA characteristics read).
 *
 * Called from ctrl_init after set_timing(), so these fields intentionally
 * override the generic values set there.
 */
static inline void rtk_i3c_core_set_baud_rate(rtk_i3c_core *core, const rtk_i3c_bitrate_cfg *cfg,
					      uint32_t i3c_freq_hz)
{
	uint32_t bdr_pp = ROUND_UP(i3c_freq_hz / 2, cfg->i3c_pp_baud_hz);
	uint32_t bdr_od = ROUND_UP(i3c_freq_hz / 2, cfg->i3c_od_baud_hz);
	uint32_t bdr_fst = (uint32_t)(((uint64_t)13U * i3c_freq_hz + 39999999ULL) / 40000000ULL);
	uint32_t thigh_od, pdly, odly;

	if (bdr_pp < 5U) {
		bdr_pp = 5U;
	}
	if (bdr_fst == 0U) {
		bdr_fst = 1U;
	}

	thigh_od = (RTK_I3C_OD_DUTY_PCT * (bdr_od * 2U)) / 100U;
	if (thigh_od < 5U) {
		thigh_od = 5U;
	}

	/* Sample at the midpoint of the SCL logic state. */
	pdly = bdr_pp / 2U;
	odly = (bdr_od > 1U) ? (bdr_od / 2U) : 1U;

	/* CBDR: BDR_OD[15:0], BDR_PP[23:16], BDR_FST[31:24]. */
	rtk_core_write32_mask(&core->cbdr, 0, 15, bdr_od);
	rtk_core_write32_mask(&core->cbdr, 16, 23, bdr_pp);
	rtk_core_write32_mask(&core->cbdr, 24, 31, bdr_fst);

	/* OD high period. */
	rtk_core_write32_mask(&core->tinit, TINIT_THIGH_OD_LOW, TINIT_THIGH_OD_HIGH, thigh_od);

	/* tCAS. */
	rtk_core_write32_mask(&core->tssr0, TSSr0_TCAS_LOW, TSSr0_TCAS_HIGH, 0x9FU);

	/* TDLYR: PDLY + ODLY, clear RDGE (rely on RHDY below). */
	rtk_core_write32_mask(&core->tdlyr, TDLYR_PDLY_LOW, TDLYR_PDLY_HIGH, pdly);
	rtk_core_write32_mask(&core->tdlyr, TDLYR_ODY_LOW, TDLYR_ODY_HIGH, odly);
	rtk_core_write32_mask(&core->tdlyr, TDLYR_RDGE_LOW, TDLYR_RDGE_HIGH, 0U);

	/* TDLYR2: ADLY = ODLY, set RHDY (sample SDA after the SCL rising edge). */
	rtk_core_write32_mask(&core->tdlyr2, TDLYR2_ADLY_LOW, TDLYR2_ADLY_HIGH, odly);
	rtk_core_write32_mask(&core->tdlyr2, TDLYR2_RHDY_LOW, TDLYR2_RHDY_HIGH, 1U);
}

/**
 * Set In-Band Request capability
 */
static inline void rtk_i3c_core_set_ibi_cap(rtk_i3c_core *core, const rtk_i3c_ibi_cfg *cfg)
{
	/* ibi – bit 0 of EC */
	rtk_core_write32_mask(&core->ec, 0, 0, cfg->enable_ibi);
	/* cr  – bit 1 of EC */
	rtk_core_write32_mask(&core->ec, 1, 1, cfg->enable_cr);
	/* hj  – bit 3 of EC */
	rtk_core_write32_mask(&core->ec, 3, 3, cfg->enable_hj);
	/* ibi_payload_size [31:24]: max IBI payload the controller accepts. Match
	 * the verified IP setup (0xFF) instead of the 0x10 reset default.
	 */
	rtk_core_write32_mask(&core->ec, 24, 31, 0xFF);
}

/**
 * Enable/disable Hot-Join acceptance (EC bit 3) independently of the other IBI
 * capability bits. Used to defer HJ acceptance until after bus initialization.
 */
static inline void rtk_i3c_core_set_hj_accept(rtk_i3c_core *core, bool enable)
{
	rtk_core_write32_mask(&core->ec, 3, 3, (uint32_t)enable);
}

/**
 * Get In-Band Request capability
 */
static inline void rtk_i3c_core_get_ibi_cap(rtk_i3c_core *core, rtk_i3c_ibi_cfg *cfg)
{
	/* ibi – bit 0 of EC */
	cfg->enable_ibi = rtk_core_read32_mask(&core->ec, 0, 0);
	/* cr  – bit 1 of EC */
	cfg->enable_cr = rtk_core_read32_mask(&core->ec, 1, 1);
	/* hj  – bit 3 of EC */
	cfg->enable_hj = rtk_core_read32_mask(&core->ec, 3, 3);
}

/**
 * Set bus timing regsters
 */
static inline void rtk_i3c_core_set_timing(rtk_i3c_core *core, const rtk_i3c_timing_cfg *cfg,
					   uint32_t i3c_freq_hz)
{
	uint32_t high_ns, su_sta_ns, hd_sta_ns;
	uint32_t hd_dat_od_ns, hd_dat_ns;
	uint32_t ds_ns, cas_ns, su_sto_ns;
	uint32_t sta_to_ns, sda_to_ns;
	uint32_t crhpoverlap_ns, newcrlock_ns;

#ifdef RTK_I3C_LEGACY_I2C
	/* Not used in I2C */
	cas_ns = 0;
	ds_ns = 0;
	crhpoverlap_ns = 0;
	newcrlock_ns = 0;
	sta_to_ns = 0;
	sda_to_ns = 0;
	hd_dat_ns = 0;
	/* OD SCL high / start-stop setup-hold timing follows the runtime OD baud
	 * (from devicetree) instead of a compile-time bucket, so changing the OD
	 * rate in the DT keeps the OD duty/timing correct.
	 */
	if (cfg->i3c_od_baud_hz <= 100000) {
		high_ns = 4000 + 1000;
		su_sta_ns = 4700;
		hd_sta_ns = 4000;
		su_sto_ns = 4000;
		hd_dat_od_ns = 300 + 0;
	} else if (cfg->i3c_od_baud_hz <= 400000) {
		high_ns = 600 + 300;
		su_sta_ns = 600 + 20;
		hd_sta_ns = 600 + 20;
		su_sto_ns = 600 + 20;
		hd_dat_od_ns = 20 + 6;
	} else if (cfg->i3c_od_baud_hz <= 1000000) {
		high_ns = 260 + 120;
		su_sta_ns = 260 + 160;
		hd_sta_ns = 260 + 160;
		su_sto_ns = 260 + 0;
		hd_dat_od_ns = 0 + 0;
	} else {
		high_ns = 60 + 40;
		su_sta_ns = 260 + 160;
		hd_sta_ns = 260 + 160;
		su_sto_ns = 160 + 20;
		hd_dat_od_ns = 20 + 0;
	}
#else  /* RTK_I3C_LEGACY_I2C */
	cas_ns = 39;
	ds_ns = 0;
	su_sta_ns = 600 + 20;
	hd_sta_ns = 600 + 20;
	high_ns = 36 + 4;
	su_sto_ns = 20 + 20;
	crhpoverlap_ns = 200 + 40;
	newcrlock_ns = 1000;
	sta_to_ns = 100000;
	sda_to_ns = 100000;
	hd_dat_od_ns = 20 + 6;
	hd_dat_ns = 6 + 0;
#endif /* RTK_I3C_LEGACY_I2C */

	/* iocr.busy_to */
	rtk_core_write32_mask(&core->iocr, IOCR_BUSY_TO_LOW, IOCR_BUSY_TO_HIGH,
			      cfg->enable_timeout_detection);

	/* iocr.sda_dly [15:12] = 1: SDA sample/output delay, per the verified IP
	 * setup (IOCR = 0x00001000). Left at reset (0) the SDA edge is not delayed.
	 */
	rtk_core_write32_mask(&core->iocr, 12, 15, 1);

	/* tssr0.tds */
	rtk_core_write32_mask(&core->tssr0, TSSr0_TDS_LOW, TSSr0_TDS_HIGH,
			      ROUND_UP(ds_ns, NSEC_PER_SEC / i3c_freq_hz));

	/* tssr0.tcas */
	rtk_core_write32_mask(&core->tssr0, TSSr0_TCAS_LOW, TSSr0_TCAS_HIGH,
			      ROUND_UP(cas_ns, 2 * NSEC_PER_SEC / i3c_freq_hz) * 2);

	/* thpr.tncrl */
	rtk_core_write32_mask(&core->thpr, THPR_TNCRL_LOW, THPR_TNCRL_HIGH,
			      ROUND_UP(newcrlock_ns, NSEC_PER_SEC / i3c_freq_hz));

	/* thpr.tcrhpovlp */
	rtk_core_write32_mask(&core->thpr, THPR_TCRHPOVL_LOW, THPR_TCRHPOVL_HIGH,
			      ROUND_UP(crhpoverlap_ns, NSEC_PER_SEC / i3c_freq_hz));

	/* tstmr.tsta_to */
	rtk_core_write32_mask(&core->tstmr, TSTMR_TSTA_TO_LOW, TSTMR_TSTA_TO_HIGH,
			      ROUND_UP(sta_to_ns, NSEC_PER_SEC / i3c_freq_hz));

	/* thptmr.tsda_to */
	rtk_core_write32_mask(&core->thptmr, THPTMR_TSDA_TO_LOW, THPTMR_TSDA_TO_HIGH,
			      ROUND_UP(sda_to_ns, NSEC_PER_SEC / i3c_freq_hz));

	/* tbcr.tbuf */
	rtk_core_write32_mask(&core->tbcr, TBCR_TBUF_LOW, TBCR_TBUF_HIGH,
			      ROUND_UP(cfg->bus_free_ns, NSEC_PER_SEC / i3c_freq_hz));

	/* tinit.taval */
	rtk_core_write32_mask(&core->tinit, TINIT_TAVAL_LOW, TINIT_TAVAL_HIGH,
			      ROUND_UP(cfg->bus_available_ns, NSEC_PER_SEC / i3c_freq_hz));

	/* tinit.thigh_od */
	rtk_core_write32_mask(&core->tinit, TINIT_THIGH_OD_LOW, TINIT_THIGH_OD_HIGH,
			      ROUND_UP(high_ns, NSEC_PER_SEC / i3c_freq_hz));

	/* tssr0.tsu_sta */
	rtk_core_write32_mask(&core->tssr0, TSSr0_TSU_STA_LOW, TSSr0_TSU_STA_HIGH,
			      ROUND_UP(su_sta_ns, NSEC_PER_SEC / i3c_freq_hz));

	/* tssr0.thd_sta */
	rtk_core_write32_mask(&core->tssr0, TSSr0_THD_STA_LOW, TSSr0_THD_STA_HIGH,
			      ROUND_UP(hd_sta_ns, NSEC_PER_SEC / i3c_freq_hz));

	/* tssr1.tsu_sto */
	rtk_core_write32_mask(&core->tssr1, TSSr1_TSU_STO_LOW, TSSr1_TSU_STO_HIGH,
			      ROUND_UP(su_sto_ns, NSEC_PER_SEC / i3c_freq_hz));

	/* ------------------- delay settings ----------------------------------- */
#ifdef CONFIG_RTK_I3C_CTRL
	/* tdlyr.pdly = ROUND_UP(hd_dat_ns, …) */
	rtk_core_write32_mask(&core->tdlyr, TDLYR_PDLY_LOW, TDLYR_PDLY_HIGH,
			      ROUND_UP(hd_dat_ns, NSEC_PER_SEC / i3c_freq_hz));

	/* tdlyr.ody = ROUND_UP(hd_dat_od_ns, …) */
	rtk_core_write32_mask(&core->tdlyr, TDLYR_ODY_LOW, TDLYR_ODY_HIGH,
			      ROUND_UP(hd_dat_od_ns, NSEC_PER_SEC / i3c_freq_hz));
#endif /* CONFIG_RTK_I3C_CTRL */
}

static inline void rtk_i3c_core_set_addr(rtk_i3c_core *core, uint8_t dyn_addr, uint16_t stc_addr)
{
	/* dyn_addr – bits 0‑6 of DSA */
	rtk_core_write32_mask(&core->dsa, 0, 6, dyn_addr);
	/* stc_addr – bits 8‑17 of DSA (10‑bit static address) */
	rtk_core_write32_mask(&core->dsa, 8, 17, stc_addr);
}

static inline void rtk_i3c_core_start_xfer(rtk_i3c_core *core)
{
	rtk_core_write32_mask(&core->cstr, 0, 0, 1);
}

static inline int rtk_i3c_core_wait_byfm_fifo(rtk_i3c_core *core)
{
	return rtk_core_wait32(&core->csr, 14, 14, 0);
}

static inline uint32_t rtk_i3c_core_get_dsa(rtk_i3c_core *core)
{
	return rtk_core_read32(&core->dsa);
}

static inline bool rtk_i3c_core_get_hdr_ddr(rtk_i3c_core *core)
{
	/* daa_done is bit 31 of the DSA register */
	return rtk_core_read32_mask(&core->csr, 16, 16);
}

static inline void rtk_i3c_core_write_byfm(rtk_i3c_core *core, uint32_t byfm)
{
	LOG_REG("header=%" PRIu32 ", rdwr=%" PRIu32 ", stop=%" PRIu32 ", ftod=%" PRIu32
		", ack=%" PRIu32 ", tx9bit=%" PRIu32 ", rx9bit=%" PRIu32 ", hdr=%" PRIu32
		", seq=%" PRIu32 ", bcnt=%" PRIu32 ", ndf=%" PRIu32 ", ptxc=%" PRIu32
		", prxc=%" PRIu32 ", end=%" PRIu32 "\n",
		BIT_FIELD_GET((byfm), 0, 0),   /* header */
		BIT_FIELD_GET((byfm), 1, 1),   /* rdwr   */
		BIT_FIELD_GET((byfm), 2, 2),   /* stop   */
		BIT_FIELD_GET((byfm), 3, 3),   /* ftod   */
		BIT_FIELD_GET((byfm), 4, 4),   /* ack    */
		BIT_FIELD_GET((byfm), 5, 5),   /* tx9bit */
		BIT_FIELD_GET((byfm), 6, 6),   /* rx9bit */
		BIT_FIELD_GET((byfm), 7, 11),  /* hdr    */
		BIT_FIELD_GET((byfm), 12, 15), /* seq    */
		BIT_FIELD_GET((byfm), 16, 19), /* bcnt   */
		BIT_FIELD_GET((byfm), 20, 26), /* ndf    */
		BIT_FIELD_GET((byfm), 29, 29), /* ptxc   */
		BIT_FIELD_GET((byfm), 30, 30), /* prxc   */
		BIT_FIELD_GET((byfm), 31, 31)  /* end    */
	);
	rtk_core_write32(core->cfr, byfm);
}

static inline int rtk_i3c_core_wait_xfer_done(rtk_i3c_core *core)
{
	return rtk_core_wait32(&core->cstr, 0, 0, 0);
}

static inline int rtk_i3c_core_wait_tx_fifo(rtk_i3c_core *core)
{
	return rtk_core_wait32(&core->csr, 8, 8, 1);
}

static inline uint32_t rtk_i3c_core_get_txfl(rtk_i3c_core *core)
{
	/* avaliable value: 1, 4, 8, 16, 32 */
	return rtk_core_read32_mask(&core->txfl, 0, 15);
}

#ifdef CONFIG_RTK_I3C_DMA

static inline void rtk_i3c_core_dma_set_txftl(rtk_i3c_core *core, uint16_t threshold)
{
	/* avaliable value: 1, 4, 8, 16, 32 */
	rtk_core_write32_mask(&core->txfl, 16, 31, threshold);
	LOG_REG("%" PRIu16 "\n", threshold);
}

static inline void rtk_i3c_core_dma_set_rxftl(rtk_i3c_core *core, uint16_t threshold)
{
	/* avaliable value: 1, 4, 8, 16, 32 */
	rtk_core_write32_mask(&core->rxfl, 16, 31, threshold);
	LOG_REG("%" PRIu16 "\n", threshold);
}
#endif /* CONFIG_RTK_I3C_DMA */

static inline int rtk_i3c_core_wait_rx_fifo(rtk_i3c_core *core)
{
	return rtk_core_wait32(&core->csr, 5, 5, 1);
}

static inline void rtk_i3c_core_write_fifo_byte(rtk_i3c_core *core, uint8_t data)
{
	rtk_core_write8(core->tbdfr, data);
	LOG_REG("0x%02" PRIx8 "\n\n", data);
}

static inline void rtk_i3c_core_write_fifo_word(rtk_i3c_core *core, uint32_t data)
{
	rtk_core_write32(core->twdfr, data);
	LOG_REG("0x%08" PRIx32 "\n\n", data);
}

static inline void rtk_i3c_core_read_fifo_byte(rtk_i3c_core *core, uint8_t *data)
{
	*data = rtk_core_read8(core->rbdfr);
	LOG_REG("0x%02" PRIx8 "\n\n", *data);
}

static inline void rtk_i3c_core_read_fifo_word(rtk_i3c_core *core, uint32_t *data)
{
	*data = rtk_core_read32(core->rwdfr);
	LOG_REG("0x%08" PRIx32 "\n\n", *data);
}

static inline uint16_t rtk_i3c_core_get_rx_fifo_len(rtk_i3c_core *core)
{
	return rtk_core_read32_mask(&core->rxfl, 0, 15);
}

static inline int rtk_i3c_core_wait_rx_fifo_len(rtk_i3c_core *core, uint16_t len)
{
	return rtk_core_wait32(&core->rxfl, 0, 15, len);
}

/* Flush all FIFOs (cstr.flush = 1) */
static inline void rtk_i3c_core_flush_all(rtk_i3c_core *core)
{
	/* flush – bit 31 of CSTR */
	rtk_core_write32_mask(&core->cstr, 31, 31, 1);
}

/* Flush BYFM FIFO only (cstr.bflush = 1) */
static inline void rtk_i3c_core_flush_byfm(rtk_i3c_core *core)
{
	/* bflush – bit 30 of CSTR */
	rtk_core_write32_mask(&core->cstr, 30, 30, 1);
}

/* Flush TXDA FIFO only (cstr.tflush = 1) */
static inline void rtk_i3c_core_flush_tx(rtk_i3c_core *core)
{
	/* tflush – bit 29 of CSTR */
	rtk_core_write32_mask(&core->cstr, 29, 29, 1);
}

/* Flush RXDA FIFO only (cstr.rflush = 1) */
static inline void rtk_i3c_core_flush_rx(rtk_i3c_core *core)
{
	/* rflush – bit 28 of CSTR */
	rtk_core_write32_mask(&core->cstr, 28, 28, 1);
}

static inline uint32_t rtk_i3c_core_get_rxne(rtk_i3c_core *core)
{
	return rtk_core_read32_mask(&core->csr, 5, 5);
}

static inline uint32_t rtk_i3c_core_get_rxnak(rtk_i3c_core *core)
{
	return rtk_core_read32_mask(&core->csr, 12, 12);
}

static inline uint32_t rtk_i3c_core_get_rxnak_isr_mask(rtk_i3c_core *core)
{
	return rtk_core_read32_mask(&core->imr, 7, 7);
}

static inline uint32_t rtk_i3c_core_get_isr(rtk_i3c_core *core)
{
	return rtk_core_read32(&core->isr);
}

static inline void rtk_i3c_core_set_rxnak_isr(rtk_i3c_core *core, bool enable)
{
	rtk_core_write32_mask(&core->imr, 7, 7, enable);
}

static inline void rtk_i3c_core_set_ibi_isr(rtk_i3c_core *core, bool enable)
{
	rtk_core_write32_mask(&core->imr, 8, 8, enable);
}

static inline bool rtk_i3c_core_set_rxne_isr(rtk_i3c_core *core, bool enable)
{
	bool previous = rtk_core_read32_mask(&core->imr, 1, 1);
	rtk_core_write32_mask(&core->imr, 1, 1, enable);
	return previous;
}

static inline void rtk_i3c_core_clear_done_isr(rtk_i3c_core *core)
{
	rtk_core_write32_mask(&core->icr, 0, 0, 1);
}

static inline void rtk_i3c_core_clear_isr(rtk_i3c_core *core, uint32_t bit_mask)
{
	rtk_core_write32(&core->icr, bit_mask);
}

static inline uint32_t rtk_i3c_core_get_trinf(rtk_i3c_core *core)
{
	/* contains rx_info and ccc */
	return rtk_core_read32(&core->trinf);
}

static inline void rtk_i3c_core_set_mdb(rtk_i3c_core *core, uint8_t dyn_addr, bool ibi_payload)
{
	uint8_t addr_index = BIT_FIELD_GET(dyn_addr, 5, 6);
	uint8_t addr_offset = BIT_FIELD_GET(dyn_addr, 0, 4);
	rtk_core_write32_mask(&core->dbgi, 0, 1, 0);
	rtk_core_write32_mask(&core->dbgi, 10, 11, addr_index);
	rtk_core_write32_mask(&core->mdb, addr_offset, addr_offset, ibi_payload);
}

/**
 * Set command response info for target
 */
static inline void rtk_i3c_core_tagt_set_resp_info(rtk_i3c_core *core,
						   const rtk_i3c_tagt_resp_info *cfg)
{
	rtk_core_write32_mask(&core->mxl, 16, 31, cfg->max_read_len);
	rtk_core_write32_mask(&core->mxl, 0, 15, cfg->max_write_len);
	rtk_core_write32_mask(&core->ec, 24, 31, cfg->ibi_payload_len);
	rtk_core_write32_mask(&core->mxds, 0, 7, cfg->max_read_rate);
	rtk_core_write32_mask(&core->mxds, 8, 15, cfg->max_write_rate);
	rtk_core_write32_mask(&core->mxds_rt, 30, 31, cfg->read_turnaround_time_enable);
	rtk_core_write32_mask(&core->mxds_rt, 0, 23, cfg->read_turnaround_time);
	rtk_core_write32_mask(&core->as, 0, 1, cfg->activity_state);
}

/**
 * Set characteristics info for target
 */
static inline void rtk_i3c_core_tagt_set_char_info(rtk_i3c_core *core,
						   const rtk_i3c_tagt_char_info *cfg)
{
	/*
	 * EC.MPID selects CR_PIDH/CR_PIDL as the source for PID[31:0].
	 * Keep this enabled so the full 48-bit PID provided by devicetree is
	 * returned during ENTDAA/GETPID instead of only the fixed high 16 bits.
	 */
	rtk_core_write32_mask(&core->ec, 23, 23, 1);
	rtk_core_write32(&core->pidh, (uint32_t)(cfg->pid >> 16));
	rtk_core_write32_mask(&core->pidl, 16, 31, BIT_FIELD_GET(cfg->pid, 0, 15));
	rtk_core_write32_mask(&core->pidl, 0, 7, cfg->dcr);
	rtk_core_write32_mask(&core->pidl, 8, 15, cfg->bcr);
}

#endif
