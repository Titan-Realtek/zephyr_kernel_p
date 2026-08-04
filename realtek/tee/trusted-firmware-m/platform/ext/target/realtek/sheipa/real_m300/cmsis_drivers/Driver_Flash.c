/*
 * Copyright (c) 2013-2022 ARM Limited. All rights reserved.
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * RTS5918 SPIC (SPI flash controller) CMSIS flash driver for TF-M BL2.
 *
 * This is a straight port of the Zephyr driver
 * drivers/flash/flash_realtek_rts5918.c: the same user-mode transfer
 * path (config_command -> spic_prepare_command -> spic_transmit_data /
 * spic_receive_data -> spic_write / spic_read) and the same mid-level
 * ops (flash_write_enable / flash_read_sr / flash_wait_till_ready /
 * flash_program_page / flash_erase_sector / flash_normal_read) are kept
 * as close to the kernel as possible.
 *
 * The Zephyr-specific plumbing is stripped for the bare-metal BL2 build:
 *   - the `const struct device *dev` handle becomes the single global
 *     SPIC register pointer plus the module-level `spic_cs`;
 *   - k_sem / irq_lock / LOG / pinctrl / DT / ex_op are dropped;
 *   - auto/memory-map mode is not used by BL2, so spic_automode() is a
 *     documented no-op (the real switch would touch platform registers
 *     0x402301e4 / 0x402301e8 -- see the deferred pinmux/automode note).
 *
 * The register block mirrors soc/realtek/ec/rts5918/reg/reg_spic.h,
 * redefined self-contained here because that header depends on Zephyr's
 * BIT()/GENMASK() helpers which are unavailable in the TF-M build.
 *
 * The outer ARM_DRIVER_FLASH (Driver_FLASH0) interface is preserved so
 * MCUboot reaches ARM_Flash_ReadData() through the flash_map layer.
 */

#include <string.h>
#include <stdint.h>
#include "Driver_Flash.h"
#include "platform_retarget.h"
#include "RTE_Device.h"

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  ((void)arg)
#endif

/* Driver version */
#define ARM_FLASH_DRV_VERSION      ARM_DRIVER_VERSION_MAJOR_MINOR(1, 1)
#define ARM_FLASH_DRV_ERASE_VALUE  0xFF

/* ------------------------------------------------------------------ */
/*                     SPIC controller definition                     */
/* ------------------------------------------------------------------ */

/* SPIC0 register base (RTS5918 external boot flash controller). There is
 * no 0x5xxx secure alias on this SoC; the secure world accesses the
 * peripheral at the same physical address as the non-secure world.
 */
#define SPIC0_BASE          (0x40000000UL)

/* Chip-select line carrying the boot flash (driven onto SER). The Zephyr
 * driver defaults data->cs to 0 and only switches to CS1 at runtime via
 * FLASH_RTS5918_EX_OP_SELECT_CS (which also re-muxes the pads). BL2 has
 * no pinctrl, so it relies on the boot ROM leaving the pads muxed to the
 * CS it used to load BL2. Keep this in sync with the boot flash wiring;
 * see the deferred pinmux note if BL2 fails to read.
 */
#define SPIC_BOOT_CS        (0U)

#define FLASH_PAGE_SZ       (256U)  /* Page-program granularity */

/*
 * Physical base offset of the EC firmware region inside the (larger,
 * host-shared) BIOS SPI NOR. TF-M / MCUboot / boot_hal_bl2 all address flash
 * with region-relative offsets: FLASH_AREA_*_OFFSET in flash_layout.h start at
 * 0 (BL2@0, S@0xB000, NS@0x38000) and merge_flash_bin.sh builds a 0-based
 * flash_full.bin. The whole image is programmed at this byte offset in the
 * shared NOR, so every SPI transfer this driver issues is translated
 * offset -> physical here (the single choke point). is_range_valid() still
 * bounds the relative offset against the 4 MB region geometry, so the base is
 * added only at the SPI layer, after validation. Must stay sector-aligned
 * (0x1000). 0 = image sits at the start of the flash.
 *
 * 0x10FC000 (~16.98 MB) requires 32-bit addressing; FLASH_USE_4BYTE_ADDR=1
 * (below) already selects the 4-byte-address opcodes, so no extra change.
 */
#ifndef FLASH_REGION_BASE_OFFSET
#define FLASH_REGION_BASE_OFFSET (0x10FC000U)
#endif

/* SPI NOR command set (subset of Zephyr's spi_nor.h used here). */
#define SPI_NOR_CMD_WREN     (0x06U) /* Write Enable                 */
#define SPI_NOR_CMD_WRDI     (0x04U) /* Write Disable                */
#define SPI_NOR_CMD_RDSR     (0x05U) /* Read Status Register         */
#define SPI_NOR_CMD_READ     (0x03U) /* Read Data (3-byte address)   */
#define SPI_NOR_CMD_PP       (0x02U) /* Page Program (3-byte)        */
#define SPI_NOR_CMD_SE       (0x20U) /* 4 KB Sector Erase (3-byte)   */
#define SPI_NOR_CMD_CE       (0xC7U) /* Chip Erase                   */
#define SPI_NOR_CMD_4BA      (0xB7U) /* Enter 4-Byte Address Mode    */
#define SPI_NOR_CMD_EXIT_4BA (0xE9U) /* Exit 4-Byte Address Mode     */
#define SPI_NOR_CMD_READ_4B  (0x13U) /* Read Data (4-byte address)   */
#define SPI_NOR_CMD_PP_4B    (0x12U) /* Page Program (4-byte)        */
#define SPI_NOR_CMD_SE_4B    (0x21U) /* 4 KB Sector Erase (4-byte)   */

#define SPI_NOR_WIP_BIT     (0x01U) /* SR bit0: Write In Progress */
#define SPI_NOR_WEL_BIT     (0x02U) /* SR bit1: Write Enable Latch */

/*
 * Address mode. BL2 defaults to 4-byte addressing: the boot flash is put
 * into 4-Byte Address Mode (0xB7) at init and the dedicated 4-byte-address
 * opcodes (0x13 read / 0x12 program / 0x21 erase) are used, which carry a
 * 32-bit address regardless of the flash's mode latch. Define
 * FLASH_USE_4BYTE_ADDR to 0 to fall back to the 3-byte path (0x03/0x02/0x20
 * with a 24-bit address), which is enough for flashes up to 16 MB.
 */
#ifndef FLASH_USE_4BYTE_ADDR
#define FLASH_USE_4BYTE_ADDR (1)
#endif

#if (FLASH_USE_4BYTE_ADDR)
#define FLASH_ADDR_SIZE      SPIC_CFG_ADDR_SIZE_32
#define FLASH_CMD_READ       SPI_NOR_CMD_READ_4B
#define FLASH_CMD_PP         SPI_NOR_CMD_PP_4B
#define FLASH_CMD_SE         SPI_NOR_CMD_SE_4B
#else
#define FLASH_ADDR_SIZE      SPIC_CFG_ADDR_SIZE_24
#define FLASH_CMD_READ       SPI_NOR_CMD_READ
#define FLASH_CMD_PP         SPI_NOR_CMD_PP
#define FLASH_CMD_SE         SPI_NOR_CMD_SE
#endif

/* ------------------------------------------------------------------ */
/*                     SPIC0 boot-flash pin mux                       */
/* ------------------------------------------------------------------ */

/*
 * The boot ROM already muxes the SPIC0 pads (CS0 / CLK / IO0-3) to load
 * BL2, so this pinmux pass is DISABLED by default. Set FLASH_APPLY_PINMUX
 * to 1 only if you need BL2 to (re)assert the flash pads itself -- and
 * first confirm the GPIO/function assignments below match your board's
 * routing, because writing the wrong GCRs will break the boot flash.
 *
 * The values here reproduce what the Zephyr pinctrl driver
 * (drivers/pinctrl/pinctrl_realtek_rts5918.c) writes for a normal-function
 * pin: GCR[gpio] = function-select bits | pad-config bits. GCR is the
 * per-pin control word in the pin controller at 0x40230000 (GCR[n] at
 * offset n*4). Function encoding (bits 8-10) and config bits mirror
 * dt-bindings/pinctrl/realtek-rts5918-pinctrl.h:
 *   FUNC1=BIT(8) FUNC2=BIT(9) FUNC4=BIT(10);
 *   input-enable=BIT(1), input-schmitt-enable=BIT(15).
 *
 * Defaults below = the coherent "a" pin group from rts5918-pinctrl.dtsi
 * (spic01_io0a..io3a + clk0a) with CS0 on gpio000. Adjust per board.
 */
#ifndef FLASH_APPLY_PINMUX
#define FLASH_APPLY_PINMUX  (1)
#endif

#if (FLASH_APPLY_PINMUX)
#define REG32(a)                (*(volatile uint32_t *)(a))
/* Pin controller (GPIO) register base -- pin-controller@40230000. */
#define PINCTRL_GCR_BASE    (0x40230000UL)
#define RTS5918_SYS_BASE        (0x40100000UL)
#define RTS5918_SPIC_PINSEL      (RTS5918_SYS_BASE + 0x140UL)

#define RTS5918_SPIC_IO3B   (1UL << 6)
#define RTS5918_SPIC_IO2B   (1UL << 4)
#define RTS5918_SPIC_IO1B   (1UL << 2)
#define RTS5918_SPIC_IO0B   (1UL << 0)
#define RTS5918_SYSCLK_LOWCLK   (1UL << 13)
/* Function-select bits (MFCTRL, GCR bits 10:8). */
#define PIN_FUNC1           (1UL << 8)  /* BIT(8)  */
#define PIN_FUNC2           (1UL << 9)  /* BIT(9)  */
#define PIN_FUNC4           (1UL << 10) /* BIT(10) */

/* Pad-config bits applied to every flash pad (input path + schmitt). */
#define PIN_CFG_FLASH       ((1UL << 1) | (1UL << 15))

/* Boot-flash pad assignment: { gpio number, function-select | config }.
 * Keep in sync with the board's actual SPIC0 routing.
 */
static const struct {
    uint8_t  gpio;
    uint32_t gcr;
} flash_pinmux[] = {
    {122U, PIN_FUNC1 | PIN_CFG_FLASH }, /* spic01_cs0a_gpio000  (CS0)  */
    {125U, PIN_FUNC1 | PIN_CFG_FLASH }, /* spic01_clk0a_gpio026 (CLK)  */
    {123U, PIN_FUNC1 | PIN_CFG_FLASH }, /* spic01_io0a_gpio032  (IO0)  */
    {124U, PIN_FUNC1 | PIN_CFG_FLASH }, /* spic01_io1a_gpio031  (IO1)  */
    {119U, PIN_FUNC1 | PIN_CFG_FLASH }, /* spic01_io2a_gpio034  (IO2)  */
    {120U, PIN_FUNC1 | PIN_CFG_FLASH }, /* spic01_io3a_gpio035  (IO3)  */
};

#endif /* FLASH_APPLY_PINMUX */

/* SPIC register block. Field offsets validated against reg_spic.h:
 * DR sits at 0x60 and RFS at 0xE0.
 */
struct reg_spic_reg {
    volatile uint32_t CTRL0;         /* 0x00 */
    volatile uint32_t RXNDF;         /* 0x04 */
    volatile uint32_t SSIENR;        /* 0x08 */
    volatile uint32_t RESERVED0;     /* 0x0C */
    volatile uint32_t SER;           /* 0x10 */
    volatile uint32_t BAUDR;         /* 0x14 */
    volatile uint32_t TXFTLR;        /* 0x18 */
    volatile uint32_t RXFTLR;        /* 0x1C */
    volatile uint32_t TXFLR;         /* 0x20 */
    volatile uint32_t RXFLR;         /* 0x24 */
    volatile uint32_t SR;            /* 0x28 */
    volatile uint32_t IMR;           /* 0x2C */
    volatile uint32_t ISR;           /* 0x30 */
    volatile uint32_t RISR;          /* 0x34 */
    volatile uint32_t TXOICR;        /* 0x38 */
    volatile uint32_t RXOICR;        /* 0x3C */
    volatile uint32_t RXUICR;        /* 0x40 */
    volatile uint32_t MSTICR;        /* 0x44 */
    volatile uint32_t ICR;           /* 0x48 */
    volatile uint32_t RESERVED1[5];  /* 0x4C..0x5F */
    union {
        volatile uint8_t  BYTE;
        volatile uint16_t HALF;
        volatile uint32_t WORD;
    } DR;                            /* 0x60 */
    volatile uint32_t RESERVED2[31]; /* 0x64..0xDF */
    volatile uint32_t RFS;           /* 0xE0 */
    volatile uint32_t RDD;           /* 0xE4 */
    volatile uint32_t RDAD;          /* 0xE8 */
    volatile uint32_t RQD;           /* 0xEC */
    volatile uint32_t RQAD;          /* 0xF0 */
    volatile uint32_t WS;            /* 0xF4 */
    volatile uint32_t WDD;           /* 0xF8 */
    volatile uint32_t WDAD;          /* 0xFC */
    volatile uint32_t WQD;           /* 0x100 */
    volatile uint32_t WQAD;          /* 0x104 */
    volatile uint32_t WE;            /* 0x108 */
    volatile uint32_t RS;            /* 0x10C */
    volatile uint32_t CTRLR2;        /* 0x110 */
    volatile uint32_t FBAUD;         /* 0x114 */
    volatile uint32_t USERLENGTH;    /* 0x118 */
    volatile uint32_t AUTOLENGTH;    /* 0x11C */
    volatile uint32_t VALIDCMD;      /* 0x120 */
    volatile uint32_t FLASH_SIZE;    /* 0x124 */
    volatile uint32_t FLUSH;         /* 0x128 */
    volatile uint32_t DUM_BYTE;      /* 0x12C */
    volatile uint32_t TXNDF;         /* 0x130 */
};

/* Single controller in BL2; `spic_reg` replaces the kernel's dev->config. */
static volatile struct reg_spic_reg *const spic_reg =
    (volatile struct reg_spic_reg *)SPIC0_BASE;

/* CTRL0 fields (reg_spic.h) */
#define SPIC_CTRL0_SIPOL_Msk           (0x0000001FUL) /* GENMASK(4, 0)   */
#define SPIC_CTRL0_SCPH                (1UL << 6)
#define SPIC_CTRL0_TMOD_Pos            (8UL)
#define SPIC_CTRL0_TMOD_Msk            (0x00000300UL) /* GENMASK(9, 8)   */
#define SPIC_CTRL0_ADDRCH_Pos          (16UL)
#define SPIC_CTRL0_ADDRCH_Msk          (0x00030000UL) /* GENMASK(17, 16) */
#define SPIC_CTRL0_DATACH_Pos          (18UL)
#define SPIC_CTRL0_DATACH_Msk          (0x000C0000UL) /* GENMASK(19, 18) */
#define SPIC_CTRL0_CMDCH_Pos           (20UL)
#define SPIC_CTRL0_CMDCH_Msk           (0x00300000UL) /* GENMASK(21, 20) */
#define SPIC_CTRL0_CK_MTIMES_Msk       (0x0F800000UL) /* GENMASK(27, 23) */
#define SPIC_CTRL0_USERMD              (1UL << 31)

/* SSIENR / SR / FLUSH */
#define SPIC_SSIENR_SPICEN             (1UL << 0)
#define SPIC_SR_TFNF                   (1UL << 1)
#define SPIC_SR_RFNE                   (1UL << 3)
#define SPIC_FLUSH_ALL                 (1UL << 0)

/* USERLENGTH: RDDUMMY[11:0], CMDLEN[13:12], ADDRLEN[19:16] */
#define SPIC_USERLENGTH_RDDUMMYLEN_Pos (0UL)
#define SPIC_USERLENGTH_RDDUMMYLEN_Msk (0x00000FFFUL)
#define SPIC_USERLENGTH_CMDLEN_Pos     (12UL)
#define SPIC_USERLENGTH_CMDLEN_Msk     (0x00003000UL)
#define SPIC_USERLENGTH_ADDRLEN_Pos    (16UL)
#define SPIC_USERLENGTH_ADDRLEN_Msk    (0x000F0000UL)

/* TX/RX NDF: 24-bit frame counts */
#define SPIC_TXNDF_NUM_Pos             (0UL)
#define SPIC_TXNDF_NUM_Msk             (0x00FFFFFFUL)
#define SPIC_RXNDF_NUM_Pos             (0UL)
#define SPIC_RXNDF_NUM_Msk             (0x00FFFFFFUL)

/* Field-packing helpers (mirror flash_realtek_rts5918.c). */
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

/* Bounded spin counts (BL2 is single-threaded, no RTOS timers). The kernel
 * uses 10 / 10000; SPICEN clears almost immediately once the FIFO is
 * drained, and WIP polling issues one RDSR transfer per iteration.
 */
#define TIMEOUT_SPICEN  10UL
#define TIMEOUT_SPIBUSY 10000UL

enum {
    COMMAND_READ = 0,
    COMMAND_WRITE = 1,
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

static const uint8_t user_addr_len[] = {
    [SPIC_CFG_ADDR_SIZE_8] = 1,
    [SPIC_CFG_ADDR_SIZE_16] = 2,
    [SPIC_CFG_ADDR_SIZE_24] = 3,
    [SPIC_CFG_ADDR_SIZE_32] = 4,
};

/* Reusable command scratch + active chip-select (replace dev->data). */
static struct qspi_cmd command_default = {
    .instruction = {.bus_width = SPIC_CFG_BUS_SINGLE, .disabled = 0},
    .address     = {.bus_width = SPIC_CFG_BUS_SINGLE,
                    .size = SPIC_CFG_ADDR_SIZE_24, .disabled = 0},
    .alt         = {.size = 0, .disabled = 1},
    .dummy_count = 0,
    .data        = {.bus_width = SPIC_CFG_BUS_SINGLE},
};
static uint8_t spic_cs = SPIC_BOOT_CS;

/* ------------------------------------------------------------------ */
/*                     SPIC user-mode transfer path                   */
/* ------------------------------------------------------------------ */

static int config_command(struct qspi_cmd *command, uint8_t cmd, uint32_t addr,
                          enum spic_address_size addr_size, uint8_t dummy_count)
{
    int ret = 0;

    switch (cmd) {
    case SPI_NOR_CMD_WREN:
    case SPI_NOR_CMD_WRDI:
    case SPI_NOR_CMD_RDSR:
    case SPI_NOR_CMD_CE:
    case SPI_NOR_CMD_4BA:
    case SPI_NOR_CMD_EXIT_4BA:
        command->address.disabled = 1;
        command->data.bus_width = SPIC_CFG_BUS_SINGLE;
        break;
    case SPI_NOR_CMD_READ:
    case SPI_NOR_CMD_SE:
    case SPI_NOR_CMD_PP:
    case SPI_NOR_CMD_READ_4B:
    case SPI_NOR_CMD_SE_4B:
    case SPI_NOR_CMD_PP_4B:
        command->address.disabled = 0;
        command->address.bus_width = SPIC_CFG_BUS_SINGLE;
        command->data.bus_width = SPIC_CFG_BUS_SINGLE;
        break;
    default:
        ret = -1;
        break;
    }

    command->instruction.value = cmd;
    command->address.size = addr_size;
    command->address.value = addr;
    command->dummy_count = dummy_count;

    return ret;
}

static int spic_wait_finish(void)
{
    int count = TIMEOUT_SPICEN;

    while ((spic_reg->SSIENR & SPIC_SSIENR_SPICEN) && count) {
        --count;
    }
    if (!count) {
        return ARM_DRIVER_ERROR_TIMEOUT;
    }
    return 0;
}

static inline void spic_flush_fifo(void)
{
    spic_reg->FLUSH = SPIC_FLUSH_ALL;
}

static inline void spic_cs_active(void)
{
    spic_reg->SER = 1UL;
}

static inline void spic_cs_deactivate(void)
{
    spic_reg->SER = 0UL;
}

static inline void spic_usermode(void)
{
    spic_reg->CTRL0 |= SPIC_CTRL0_USERMD;
}

/* Auto/memory-map mode switch needs platform registers 0x402301e4 /
 * 0x402301e8 (the deferred pinmux/automode work). BL2 never uses the
 * memory-mapped window, so leaving the controller in user mode is fine
 * and this mirrors the kernel call sites without touching those regs.
 */
static inline void spic_automode(void)
{
    /* intentionally left as a no-op for BL2 */
}

#if (FLASH_APPLY_PINMUX)
/* Assert the boot-flash pin functions by writing each pad's GCR word in
 * the pin controller, mirroring pinctrl_configure_pins()'s normal-pin
 * path. Only compiled/called when FLASH_APPLY_PINMUX is enabled; by
 * default the boot ROM's pad muxing is left untouched.
 */
static void spic_pinmux_config(void)
{
    volatile uint32_t *const gcr = (volatile uint32_t *)PINCTRL_GCR_BASE;
    uint32_t i;

    for (i = 0; i < (sizeof(flash_pinmux) / sizeof(flash_pinmux[0])); i++) {
        gcr[flash_pinmux[i].gpio] = flash_pinmux[i].gcr;
    }
}
#endif /* FLASH_APPLY_PINMUX */

static void spic_prepare_command(const struct qspi_cmd *command,
                                 uint32_t tx_size, uint32_t rx_size, uint8_t write)
{
    uint8_t addr_len = user_addr_len[command->address.size];

    spic_flush_fifo();

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

static void spic_transmit_data(const void *data, uint32_t *length)
{
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

static void spic_receive_data(void *data, uint32_t *length)
{
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

static int spic_write(const struct qspi_cmd *command, const void *data, uint32_t *length)
{
    int ret;

    spic_prepare_command(command, *length, 0, COMMAND_WRITE);
    spic_cs_active();

    spic_transmit_data(data, length);
    ret = spic_wait_finish();

    spic_cs_deactivate();

    return ret;
}

static int spic_read(const struct qspi_cmd *command, void *data, uint32_t *length)
{
    int ret;

    spic_prepare_command(command, 0, *length, COMMAND_READ);
    spic_cs_active();

    spic_receive_data(data, length);
    ret = spic_wait_finish();

    spic_cs_deactivate();

    return ret;
}

/* ------------------------------------------------------------------ */
/*                        SPI NOR mid-level ops                       */
/* ------------------------------------------------------------------ */

static int flash_read_sr(uint8_t *val)
{
    struct qspi_cmd *command = &command_default;
    int status;
    uint32_t len = 1;
    uint8_t sr;

    config_command(command, SPI_NOR_CMD_RDSR, 0, 0, 0);
    status = spic_read(command, &sr, &len);
    if (status) {
        return status;
    }
    *val = sr;

    return 0;
}

static int flash_write_enable(void)
{
    struct qspi_cmd *command = &command_default;
    uint32_t len = 0;
    uint8_t sr = 0;
    int ret;
    int timeout;

    config_command(command, SPI_NOR_CMD_WREN, 0, 0, 0);
    ret = spic_write(command, 0, &len);
    if (ret < 0) {
        return ret;
    }

    timeout = TIMEOUT_SPIBUSY;
    do {
        flash_read_sr(&sr);
        if (!(sr & SPI_NOR_WIP_BIT)) {
            return 0;
        }
        timeout--;
    } while (timeout > 0);

    return ret;
}

static int flash_write_disable(void)
{
    struct qspi_cmd *command = &command_default;
    uint32_t len = 0;
    uint8_t sr = 0;
    int ret;
    int timeout;

    config_command(command, SPI_NOR_CMD_WRDI, 0, 0, 0);
    ret = spic_write(command, NULL, &len);
    if (ret < 0) {
        return ret;
    }

    timeout = TIMEOUT_SPIBUSY;
    do {
        flash_read_sr(&sr);
        if (!(sr & SPI_NOR_WIP_BIT)) {
            return 0;
        }
        timeout--;
    } while (timeout > 0);

    return ret;
}

static int flash_wait_till_ready(void)
{
    int timeout = TIMEOUT_SPIBUSY;
    uint8_t sr = 0;

    /* A sector erase needs ~3000 iterations; a program page ~40. */
    do {
        flash_read_sr(&sr);
        if (!(sr & SPI_NOR_WIP_BIT)) {
            return 0;
        }
        timeout--;
    } while (timeout > 0);

    return ARM_DRIVER_ERROR_TIMEOUT;
}

static int flash_enter_4byte(void)
{
    struct qspi_cmd *command = &command_default;
    uint32_t len = 0;
    int ret;

    spic_usermode();
    config_command(command, SPI_NOR_CMD_4BA, 0, 0, 0);
    ret = spic_write(command, NULL, &len);
    if (ret < 0) {
        goto exit;
    }

    ret = flash_wait_till_ready();
exit:
    spic_automode();
    return ret;
}

static int flash_exit_4byte(void)
{
    struct qspi_cmd *command = &command_default;
    uint32_t len = 0;
    int ret;

    spic_usermode();
    config_command(command, SPI_NOR_CMD_EXIT_4BA, 0, 0, 0);
    ret = spic_write(command, NULL, &len);
    if (ret < 0) {
        goto exit;
    }

    ret = flash_wait_till_ready();
exit:
    spic_automode();
    return ret;
}

static int flash_program_page(uint32_t address, const uint8_t *data, uint32_t size)
{
    struct qspi_cmd *command = &command_default;
    enum spic_address_size addr_size = FLASH_ADDR_SIZE;
    int ret = 0;
    uint32_t offset = 0, chunk = 0, page_size = FLASH_PAGE_SZ;

    /* Translate region-relative offset -> physical SPI address. */
    address += FLASH_REGION_BASE_OFFSET;

    spic_usermode();
    while (size > 0) {
        ret = flash_write_enable();
        if (ret < 0) {
            spic_automode();
            return ret;
        }

        offset = address % page_size;
        chunk = (offset + size < page_size) ? size : (page_size - offset);

        config_command(command, FLASH_CMD_PP, address, addr_size, 0);

        ret = spic_write(command, data, &chunk);
        if (ret < 0) {
            goto err_exit;
        }

        data += chunk;
        address += chunk;
        size -= chunk;

        flash_wait_till_ready();
    }

err_exit:
    flash_write_disable();
    spic_automode();
    return ret;
}

static int flash_erase_sector(uint32_t address)
{
    struct qspi_cmd *command = &command_default;
    int ret;
    uint32_t len = 0;

    /* Translate region-relative offset -> physical SPI address. */
    address += FLASH_REGION_BASE_OFFSET;

    spic_usermode();
    ret = flash_write_enable();
    if (ret < 0) {
        spic_automode();
        return ret;
    }

    config_command(command, FLASH_CMD_SE, address, FLASH_ADDR_SIZE, 0);
    ret = spic_write(command, NULL, &len);
    if (ret < 0) {
        goto err_exit;
    }
    ret = flash_wait_till_ready();

err_exit:
    flash_write_disable();
    spic_automode();
    return ret;
}

static int flash_chip_erase(void)
{
    struct qspi_cmd *command = &command_default;
    int ret;
    uint32_t len = 0;

    spic_usermode();
    ret = flash_write_enable();
    if (ret < 0) {
        spic_automode();
        return ret;
    }

    config_command(command, SPI_NOR_CMD_CE, 0, 0, 0);
    ret = spic_write(command, NULL, &len);
    if (ret < 0) {
        goto err_exit;
    }
    ret = flash_wait_till_ready();

err_exit:
    flash_write_disable();
    spic_automode();
    return ret;
}

static int flash_normal_read(uint8_t rdcmd, uint8_t dummy, uint32_t address,
                             uint8_t *data, uint32_t size)
{
    struct qspi_cmd *command = &command_default;
    enum spic_address_size addr_size = FLASH_ADDR_SIZE;
    int ret;

    /* Translate region-relative offset -> physical SPI address. */
    uint32_t src_addr = address + FLASH_REGION_BASE_OFFSET;
    uint8_t *dst_idx = data;

    uint32_t remind_size = size;
    uint32_t block_size = 0x8000UL;

    config_command(command, rdcmd, src_addr, addr_size, dummy);
    spic_usermode();
    while (remind_size > 0) {
        command->address.value = src_addr;

        if (remind_size >= block_size) {
            ret = spic_read(command, dst_idx, &block_size);
            src_addr += block_size;
            remind_size -= block_size;
            dst_idx += block_size;
        } else {
            ret = spic_read(command, dst_idx, &remind_size);
            dst_idx += remind_size;
            remind_size = 0;
        }

        if (ret < 0) {
            spic_automode();
            return ret;
        }
    }
    spic_automode();
    return 0;
}

/* ------------------------------------------------------------------ */
/*                         CMSIS flash device                         */
/* ------------------------------------------------------------------ */

enum {
    DATA_WIDTH_8BIT   = 0u,
    DATA_WIDTH_16BIT,
    DATA_WIDTH_32BIT,
    DATA_WIDTH_ENUM_SIZE
};

static const uint32_t data_width_byte[DATA_WIDTH_ENUM_SIZE] = {
    sizeof(uint8_t),
    sizeof(uint16_t),
    sizeof(uint32_t),
};

struct arm_flash_dev_t {
    const uint32_t memory_base;   /*!< FLASH memory-mapped base (unused for I/O) */
    ARM_FLASH_INFO *data;         /*!< FLASH geometry */
};

/* Flash Status */
static ARM_FLASH_STATUS FlashStatus = {0, 0, 0};

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_FLASH_API_VERSION,
    ARM_FLASH_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES DriverCapabilities = {
    0, /* event_ready */
    0, /* data_width = 0:8-bit, 1:16-bit, 2:32-bit */
    1  /* erase_chip */
};

static int32_t is_range_valid(struct arm_flash_dev_t *flash_dev,
                              uint32_t offset)
{
    uint32_t flash_limit = (flash_dev->data->sector_count *
                            flash_dev->data->sector_size) - 1;

    return (offset > flash_limit) ? -1 : 0;
}

static int32_t is_write_aligned(struct arm_flash_dev_t *flash_dev,
                                uint32_t param)
{
    return ((param % flash_dev->data->program_unit) != 0) ? -1 : 0;
}

static int32_t is_sector_aligned(struct arm_flash_dev_t *flash_dev,
                                 uint32_t offset)
{
    return ((offset % flash_dev->data->sector_size) != 0) ? -1 : 0;
}

#if (RTE_FLASH0)
static ARM_FLASH_INFO ARM_FLASH0_DEV_DATA = {
    .sector_info  = NULL,                  /* Uniform sector layout */
    .sector_count = FLASH0_SIZE / FLASH0_SECTOR_SIZE,
    .sector_size  = FLASH0_SECTOR_SIZE,
    .page_size    = FLASH0_PAGE_SIZE,
    .program_unit = FLASH0_PROGRAM_UNIT,
    .erased_value = ARM_FLASH_DRV_ERASE_VALUE};

static struct arm_flash_dev_t ARM_FLASH0_DEV = {
#if (__DOMAIN_NS == 1)
    .memory_base = FLASH0_BASE_NS,
#else
    .memory_base = FLASH0_BASE_S,
#endif /* __DOMAIN_NS == 1 */
    .data        = &(ARM_FLASH0_DEV_DATA)};

struct arm_flash_dev_t *FLASH0_DEV = &ARM_FLASH0_DEV;

/*
 * Functions
 */

static ARM_DRIVER_VERSION ARM_Flash_GetVersion(void)
{
    return DriverVersion;
}

static ARM_FLASH_CAPABILITIES ARM_Flash_GetCapabilities(void)
{
    return DriverCapabilities;
}

static int32_t ARM_Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
    ARG_UNUSED(cb_event);

    if (FLASH0_PROGRAM_UNIT % data_width_byte[DriverCapabilities.data_width] ||
        DriverCapabilities.data_width >= DATA_WIDTH_ENUM_SIZE) {
        return ARM_DRIVER_ERROR;
    }

#if (FLASH_APPLY_PINMUX)
    /* Optionally (re)assert the SPIC0 flash pads. Disabled by default --
     * the boot ROM already muxed them to load BL2. See flash_pinmux[]. */
    spic_pinmux_config();
    REG32(RTS5918_SPIC_PINSEL)  |= RTS5918_SPIC_IO3B;
    REG32(RTS5918_SPIC_PINSEL)  |= RTS5918_SPIC_IO2B;
    REG32(RTS5918_SPIC_PINSEL)  |= RTS5918_SPIC_IO1B;
    REG32(RTS5918_SPIC_PINSEL)  |= RTS5918_SPIC_IO0B;    
#endif

    /* Bring the SPIC into a known 1-1-1 user-mode state. Boot ROM has
     * already powered the controller and muxed the flash pads (it loaded
     * BL2 over this bus), so we only reset the transfer engine, force the
     * command/address/data channels to single-bit, and select the bus.
     * This mirrors flash_rts5918_init() minus the pinctrl / k_sem /
     * engineer-mode / 4-byte / QE-mode setup that BL2 does not need.
     */
    spic_reg->SSIENR = 0UL;
    spic_reg->IMR    = 0UL;

    spic_reg->CTRL0 = ((spic_reg->CTRL0 & SPIC_CTRL0_CK_MTIMES_Msk) |
                       CMD_CH(0) | DATA_CH(0) | ADDR_CH(0) | MODE(0) |
                       (spic_reg->CTRL0 & SPIC_CTRL0_SIPOL_Msk));

    spic_reg->BAUDR = 1UL;
    spic_reg->FBAUD = 1UL;

    spic_usermode();

#if (FLASH_USE_4BYTE_ADDR)
    /* Put the boot flash into 4-Byte Address Mode so that the reads / erases /
     * programs issued below use 32-bit addresses. Mirrors the kernel driver,
     * which enters 4-byte mode on the external SPIC0 flash during init. */
    if (flash_enter_4byte() < 0) {
        return ARM_DRIVER_ERROR;
    }
#endif

    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_Uninitialize(void)
{
    /* Leave the SPIC quiescent; the next stage re-initialises as needed. */
    spic_cs_deactivate();
    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_PowerControl(ARM_POWER_STATE state)
{
    switch (state) {
    case ARM_POWER_FULL:
        /* Nothing to be done */
        return ARM_DRIVER_OK;
        break;

    case ARM_POWER_OFF:
    case ARM_POWER_LOW:
    default:
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
}

static int32_t ARM_Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
    int ret;

    /* CMSIS ARM_FLASH_ReadData API requires the `addr` data type size
     * aligned. Data type size is specified by data_width (8-bit here).
     */
    if (addr % data_width_byte[DriverCapabilities.data_width] != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check flash memory boundaries */
    if (is_range_valid(FLASH0_DEV, addr + cnt) != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if (cnt == 0) {
        return 0;
    }

    /* Single-bit-bus read, no dummy. Opcode and address width follow the
     * configured address mode (4-byte 0x13 / 3-byte 0x03). The `addr` is a
     * flash byte offset that maps 1:1 to what MCUboot asks for.
     */
    ret = flash_normal_read(FLASH_CMD_READ, 0, addr, (uint8_t *)data, cnt);
    if (ret < 0) {
        return ret;
    }

    /* Conversion between bytes and data items */
    cnt /= data_width_byte[DriverCapabilities.data_width];
    return cnt;
}

static int32_t ARM_Flash_ProgramData(uint32_t addr, const void *data,
                                     uint32_t cnt)
{
    int32_t rc = 0;
    int ret;

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check flash memory boundaries and alignment with minimal write size */
    rc  = is_range_valid(FLASH0_DEV, addr + cnt);
    rc |= is_write_aligned(FLASH0_DEV, addr);
    rc |= is_write_aligned(FLASH0_DEV, cnt);
    if (rc != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if (cnt == 0) {
        return 0;
    }

    ret = flash_program_page(addr, (const uint8_t *)data, cnt);
    if (ret < 0) {
        return ret;
    }

    /* Conversion between bytes and data items */
    cnt /= data_width_byte[DriverCapabilities.data_width];
    return cnt;
}

static int32_t ARM_Flash_EraseSector(uint32_t addr)
{
    int32_t rc;

    rc  = is_range_valid(FLASH0_DEV, addr);
    rc |= is_sector_aligned(FLASH0_DEV, addr);
    if (rc != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    return (flash_erase_sector(addr) < 0) ? ARM_DRIVER_ERROR : ARM_DRIVER_OK;
}

static int32_t ARM_Flash_EraseChip(void)
{
    if (DriverCapabilities.erase_chip != 1) {
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }

    return (flash_chip_erase() < 0) ? ARM_DRIVER_ERROR : ARM_DRIVER_OK;
}

static ARM_FLASH_STATUS ARM_Flash_GetStatus(void)
{
    return FlashStatus;
}

static ARM_FLASH_INFO * ARM_Flash_GetInfo(void)
{
    return FLASH0_DEV->data;
}

ARM_DRIVER_FLASH Driver_FLASH0 = {
    ARM_Flash_GetVersion,
    ARM_Flash_GetCapabilities,
    ARM_Flash_Initialize,
    ARM_Flash_Uninitialize,
    ARM_Flash_PowerControl,
    ARM_Flash_ReadData,
    ARM_Flash_ProgramData,
    ARM_Flash_EraseSector,
    ARM_Flash_EraseChip,
    ARM_Flash_GetStatus,
    ARM_Flash_GetInfo
};
#endif /* RTE_FLASH0 */
