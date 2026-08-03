/*
 * Copyright (c) 2017-2024, Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tfm_hal_device_header.h"
#include "fih.h"
#include "target_cfg.h"
#include "Driver_MPC.h"
#include "platform_retarget_dev.h"
#include "region_defs.h"
#include "tfm_plat_defs.h"
#include "region.h"

#define ENABLE_PPC_MPC_SUPPORT 0

#ifdef PSA_API_TEST_IPC
#define PSA_FF_TEST_SECURE_UART2
#endif

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

/* The section names come from the scatter file */
REGION_DECLARE(Load$$LR$$, LR_NS_PARTITION, $$Base);
REGION_DECLARE(Image$$, ER_VENEER, $$Base);
REGION_DECLARE(Image$$, VENEER_ALIGN, $$Limit);

#ifdef BL2
REGION_DECLARE(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base);
#endif /* BL2 */

const struct memory_region_limits memory_regions = {
    .non_secure_code_start =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base) +
        BL2_HEADER_SIZE,

    .non_secure_partition_base =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base),

    .non_secure_partition_limit =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base) +
        NS_PARTITION_SIZE - 1,

    .veneer_base = (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base),
    .veneer_limit = (uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit),

#ifdef BL2
    .secondary_partition_base =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base),

    .secondary_partition_limit =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base) +
        SECONDARY_PARTITION_SIZE - 1,
#endif /* BL2 */
};

/* Import MPC driver */
extern ARM_DRIVER_MPC Driver_SRAM1_MPC, Driver_SRAM2_MPC;

/* Define Peripherals NS address range for the platform */
#define PERIPHERALS_BASE_NS_START (0x40000000)
/* NS peripheral window spans 0x40000000..0x50080000; RLAR limit is the last
 * inclusive address (0x50080000 - 1), 32-byte aligned. */
#define PERIPHERALS_BASE_NS_END   (0x5007FFFF)

/* Memory-mapped SPI-flash-controller apertures the NS EC FW accesses directly
 * (HwFlashInterface.c: PVT_FLASH_ADDRESS=0x60000000 = SPIC1 window,
 * MCM_FLASH_ADDRESS=0x80000000 = SPIC2 window). The whole 512 MB controller
 * aperture must be NS, not just the eflash device size. Without these SAU NS
 * regions an NS read (e.g. eRPMC init) hits SAU-default Secure and faults
 * AUVIOL (SFSR=0x48, SFAR=0x8000xxxx). See failed27. */
/* SPIC1 (PVT, 0x60000000..0x7FFFFFFF) and SPIC2 (MCM, 0x80000000..0x9FFFFFFF)
 * are contiguous, so cover both flash-controller apertures with ONE SAU region
 * to conserve SAU slots (only 8 total). */
#define SPIC_BASE_NS    (0x60000000)
#define SPIC_LIMIT_NS   (0x9FFFFFFF)

/* Battery-backed RAM (bb-ram@20200000, 0x100 bytes) — the NS EC FW reads/writes
 * it directly (bbram_rts5918_read); it sits above SRAM at 0x20200000, outside
 * the 0x40000000 peripheral window, so it needs its own SAU NS region or NS
 * hits SAU-default Secure -> AUVIOL (SFAR=0x202000xx). See failed28. */
#define BBRAM_BASE_NS   (0x20200000)
#define BBRAM_LIMIT_NS  (0x202000FF)

/* HW-crypto engine grid that MUST remain Secure even though it sits inside the
 * 0x40000000 NS peripheral window: SHA2 (0x40040000), SHA2DMA (0x40041000),
 * SHA3 (0x40042000), AES (0x40043000). The SPE's mbedcrypto (CONFIG_ENABLE_
 * LALU_SHA2) drives these registers directly; if SAU marks them NS the secure
 * accesses become Non-Secure and the crypto IP's global control registers
 * (DMA_EN/ch_en) silently reject the NS writes, so the DMA never enables and
 * the first SHA hangs (SPE stall after "Provision entropy seed... complete").
 * NS FW reaches crypto via PSA IPC, not these registers, so it does not need
 * them NS. Carve this range out of the NS peripheral region below. */
#define CRYPTO_ENGINE_BASE_S    (0x40040000)
/* Upper bound extended from 0x40043FFF to 0x40046FFF so the LALU key manager
 * (KEYGMGR_BASE = 0x40046000, used by LALU GCM/AES setkey) is also SAU-Secure;
 * otherwise the SPE's NS access to it is rejected -> BusFault in
 * load_key_to_engine(). Covers SHA2/SHA2DMA/SHA3/AES + key manager. */
#define CRYPTO_ENGINE_LIMIT_S   (0x40046FFF)

/* LALU PKE engine block, carved SAU-Secure for the SPE only (SPE-side ECDSA
 * scalar-mul via lalu_ecp_mul_simplified touches these; NS reaches ECDSA via
 * PSA IPC). Covers PKE regs (0x40080000, incl. pke_mutex @+0x34), MMEM
 * (0x40090000), TMEM (0x400A0000) and IMEM (0x400B0000). BL2 does not use LALU
 * PKE and cannot spare the extra SAU region, so the split is #ifndef BL2. */
#define PKE_ENGINE_BASE_S       (0x40080000)
#define PKE_ENGINE_LIMIT_S      (0x400BFFFF)

/* Enable system reset request for CPU 0 */
#define ENABLE_CPU0_SYSTEM_RESET_REQUEST (1U << 4U)

/* To write into AIRCR register, 0x5FA value must be write to the VECTKEY field,
 * otherwise the processor ignores the write.
 */
#define SCB_AIRCR_WRITE_MASK ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos))

/* Debug configuration flags */
#define SPNIDEN_SEL_STATUS (0x01u << 7)
#define SPNIDEN_STATUS     (0x01u << 6)
#define SPIDEN_SEL_STATUS  (0x01u << 5)
#define SPIDEN_STATUS      (0x01u << 4)
#define NIDEN_SEL_STATUS   (0x01u << 3)
#define NIDEN_STATUS       (0x01u << 2)
#define DBGEN_SEL_STATUS   (0x01u << 1)
#define DBGEN_STATUS       (0x01u << 0)

#define All_SEL_STATUS (SPNIDEN_SEL_STATUS | SPIDEN_SEL_STATUS | \
                        NIDEN_SEL_STATUS | DBGEN_SEL_STATUS)

struct platform_data_t tfm_peripheral_std_uart = {
        UART0_BASE_NS,
        UART0_BASE_NS + 0xFFF,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};

struct platform_data_t tfm_peripheral_uart1 = {
        UART1_BASE_S,
        UART1_BASE_S + 0xFFF,
        PPC_SP_APB_PPC_EXP1,
        CMSDK_UART1_APB_PPC_POS
};

struct platform_data_t tfm_peripheral_timer0 = {
        CMSDK_TIMER0_BASE_S,
        CMSDK_TIMER1_BASE_S - 1,
        PPC_SP_APB_PPC0,
        CMSDK_TIMER0_APB_PPC_POS
};

#ifdef PSA_API_TEST_IPC

/* Below data structure are only used for PSA FF tests, and this pattern is
 * definitely not to be followed for real life use cases, as it can break
 * security.
 */

struct platform_data_t
    tfm_peripheral_FF_TEST_UART_REGION = {
        UART2_BASE_S,
        UART2_BASE_S + 0xFFF,
        PPC_SP_APB_PPC_EXP2,
        CMSDK_UART2_APB_PPC_POS
};

struct platform_data_t
    tfm_peripheral_FF_TEST_WATCHDOG_REGION = {
        APB_WATCHDOG_BASE_S,
        APB_WATCHDOG_BASE_S + 0xFFF,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};

#define FF_TEST_NVMEM_REGION_START            0x102FFC00
#define FF_TEST_NVMEM_REGION_END              0x102FFFFF
#define FF_TEST_SERVER_PARTITION_MMIO_START   0x3801FC00
#define FF_TEST_SERVER_PARTITION_MMIO_END     0x3801FCFF
#define FF_TEST_DRIVER_PARTITION_MMIO_START   0x3801FE00
#define FF_TEST_DRIVER_PARTITION_MMIO_END     0x3801FEFF

struct platform_data_t
    tfm_peripheral_FF_TEST_NVMEM_REGION = {
        FF_TEST_NVMEM_REGION_START,
        FF_TEST_NVMEM_REGION_END,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};

struct platform_data_t
    tfm_peripheral_FF_TEST_SERVER_PARTITION_MMIO = {
        FF_TEST_SERVER_PARTITION_MMIO_START,
        FF_TEST_SERVER_PARTITION_MMIO_END,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};

struct platform_data_t
    tfm_peripheral_FF_TEST_DRIVER_PARTITION_MMIO = {
        FF_TEST_DRIVER_PARTITION_MMIO_START,
        FF_TEST_DRIVER_PARTITION_MMIO_END,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};
#endif

enum tfm_plat_err_t enable_fault_handlers(void)
{
    /* Explicitly set secure fault priority to the highest */
    NVIC_SetPriority(SecureFault_IRQn, 0);

    /* Enables BUS, MEM, USG and Secure faults */
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk
                  | SCB_SHCSR_BUSFAULTENA_Msk
                  | SCB_SHCSR_MEMFAULTENA_Msk
                  | SCB_SHCSR_SECUREFAULTENA_Msk;
    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t system_reset_cfg(void)
{
    return TFM_PLAT_ERR_SUCCESS;
}

FIH_RET_TYPE(enum tfm_plat_err_t) init_debug(void)
{
    FIH_RET(fih_int_encode(TFM_PLAT_ERR_SUCCESS));
}

/*----------------- NVIC interrupt target state to NS configuration ----------*/
enum tfm_plat_err_t nvic_interrupt_target_state_cfg(void)
{
    /* Target every interrupt to NS; unimplemented interrupts will be WI */
    for (uint8_t i=0; i<sizeof(NVIC->ITNS)/sizeof(NVIC->ITNS[0]); i++) {
        NVIC->ITNS[i] = 0xFFFFFFFF;
    }

#if ENABLE_PPC_MPC_SUPPORT
    /* Make sure that MPC and PPC are targeted to S state */
    NVIC_ClearTargetState(MPC_IRQn);
    NVIC_ClearTargetState(PPC_IRQn);
#endif

#ifdef SECURE_UART1
    /* UART1 is a secure peripheral, so its IRQs have to target S state */
    NVIC_ClearTargetState(UARTRX1_IRQn);
    NVIC_ClearTargetState(UARTTX1_IRQn);
    NVIC_ClearTargetState(UART1_IRQn);
#endif

    return TFM_PLAT_ERR_SUCCESS;
}

/*----------------- NVIC interrupt enabling for S peripherals ----------------*/
enum tfm_plat_err_t nvic_interrupt_enable(void)
{
#if ENABLE_PPC_MPC_SUPPORT
    struct spctrl_def* spctrl = CMSDK_SPCTRL;
    int32_t ret = ARM_DRIVER_OK;

    /* MPC interrupt enabling */
    ret = Driver_SRAM1_MPC.EnableInterrupt();
    if (ret != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    ret = Driver_SRAM2_MPC.EnableInterrupt();
    if (ret != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    NVIC_EnableIRQ(MPC_IRQn);

    /* PPC interrupt enabling */
    /* Clear pending PPC interrupts */
    /* In the PPC configuration function, we have used the Non-Secure
     * Privilege Control Block to grant unprivilged NS access to some
     * peripherals used by NS. That triggers a PPC0 exception as that
     * register is meant for NS privileged access only. Clear it here
     */
    spctrl->secppcintclr = CMSDK_APB_PPC0_INT_POS_MASK;

    /* Enable PPC interrupts for APB PPC */
    spctrl->secppcinten |= CMSDK_APB_PPC0_INT_POS_MASK |
                           CMSDK_APB_PPC1_INT_POS_MASK |
                           CMSDK_APB_PPCEXP0_INT_POS_MASK |
                           CMSDK_APB_PPCEXP1_INT_POS_MASK |
                           CMSDK_APB_PPCEXP2_INT_POS_MASK |
                           CMSDK_APB_PPCEXP3_INT_POS_MASK;

    NVIC_EnableIRQ(PPC_IRQn);
#endif /* ENABLE_PPC_MPC_SUPPORT */

#ifdef PSA_FF_TEST_SECURE_UART2
    NVIC_EnableIRQ(FF_TEST_UART_IRQ);
#endif

    return TFM_PLAT_ERR_SUCCESS;
}

/*------------------- SAU/IDAU configuration functions -----------------------*/
#if defined(PSA_API_TEST_NS) && !defined(PSA_API_TEST_IPC)
#define DEV_APIS_TEST_NVMEM_REGION_START (NS_DATA_LIMIT + 1)
#define DEV_APIS_TEST_NVMEM_REGION_LIMIT \
    (DEV_APIS_TEST_NVMEM_REGION_START + DEV_APIS_TEST_NVMEM_REGION_SIZE - 1)
#endif

struct sau_cfg_t {
    uint32_t RBAR;
    uint32_t RLAR;
    bool nsc;
};

const struct sau_cfg_t sau_cfg[] = {
    {
        ((uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base)),
        ((uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base) +
        NS_PARTITION_SIZE - 1),
        false,
    },
    {
        NS_DATA_START,
        NS_DATA_LIMIT,
        false,
    },
    {
        (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base),
        (uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit) - 1,
        true,
    },
    /* NS peripheral window, split around the Secure HW-crypto engine grid
     * (see CRYPTO_ENGINE_BASE_S). Lower half: 0x40000000..0x4003FFFF NS.
     * NOTE: this replaces the upstream PSA_FF_TEST_SECURE_UART2 split that used
     * to carve UART2 (0x40202000) Secure. That carve was inert here (secure/NS
     * console both use UART0 = Driver_USART0; UART2-secure only serves the PSA
     * FF test suite, which this build does not run) so it was dropped in favour
     * of the crypto carve, keeping the SPE within the 8-region SAU limit. */
    {
        PERIPHERALS_BASE_NS_START,
        (CRYPTO_ENGINE_BASE_S - 1),
        false,
    },
    /* Crypto engine 0x40040000..0x40046FFF is left uncovered => SAU-default
     * Secure (includes the key manager at 0x40046000). */
#ifdef BL2
    /* BL2: single upper NS window (no LALU PKE use; keep within SAU region
     * budget). 0x40047000..0x5007FFFF NS. */
    {
        (CRYPTO_ENGINE_LIMIT_S + 1),
        PERIPHERALS_BASE_NS_END,
        false,
    },
#else
    /* SPE: split the upper NS window around the LALU PKE block so
     * 0x40080000..0x400BFFFF stays SAU-default Secure. */
    {
        (CRYPTO_ENGINE_LIMIT_S + 1),    /* 0x40047000 */
        (PKE_ENGINE_BASE_S - 1),        /* 0x4007FFFF */
        false,
    },
    {
        (PKE_ENGINE_LIMIT_S + 1),       /* 0x400C0000 */
        PERIPHERALS_BASE_NS_END,
        false,
    },
#endif
    {
        SPIC_BASE_NS,
        SPIC_LIMIT_NS,
        false,
    },
    {
        BBRAM_BASE_NS,
        BBRAM_LIMIT_NS,
        false,
    },
#ifdef BL2
    {
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base),
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base) +
        SECONDARY_PARTITION_SIZE - 1,
        false,
    },
#endif
#if defined(PSA_API_TEST_NS) && !defined(PSA_API_TEST_IPC)
    {
        DEV_APIS_TEST_NVMEM_REGION_START,
        DEV_APIS_TEST_NVMEM_REGION_LIMIT,
        false,
    },
#endif
};

#define NR_SAU_INIT_STEP                 3

FIH_RET_TYPE(int32_t) sau_and_idau_cfg(void)
{
    uint32_t i;

    /* Ensure all memory accesses are completed */
    __DMB();

    /* Enables SAU */
    TZ_SAU_Enable();

    for (i = 0; i < ARRAY_SIZE(sau_cfg); i++) {
        SAU->RNR = i;
        SAU->RBAR = sau_cfg[i].RBAR & SAU_RBAR_BADDR_Msk;
        SAU->RLAR = (sau_cfg[i].RLAR & SAU_RLAR_LADDR_Msk) |
                    (sau_cfg[i].nsc ? SAU_RLAR_NSC_Msk : 0U) |
                    SAU_RLAR_ENABLE_Msk;
    }

    /* Ensure the write is completed and flush pipeline */
    __DSB();
    __ISB();

    FIH_RET(fih_int_encode(ARM_DRIVER_OK));
}

#ifdef TFM_FIH_PROFILE_ON
fih_int fih_verify_sau_and_idau_cfg(void)
{
    struct spctrl_def *spctrl = CMSDK_SPCTRL;
    uint32_t i;

    /* Check SAU is enabled */
    if ((SAU->CTRL & (SAU_CTRL_ENABLE_Msk)) != (SAU_CTRL_ENABLE_Msk)) {
        FIH_RET(fih_int_encode(ARM_DRIVER_ERROR));
    }

    for (i = 0; i < ARRAY_SIZE(sau_cfg); i++) {
        SAU->RNR = i;
        if (SAU->RBAR != (sau_cfg[i].RBAR & SAU_RBAR_BADDR_Msk)) {
            FIH_RET(fih_int_encode(ARM_DRIVER_ERROR));
        }
        if (SAU->RLAR != ((sau_cfg[i].RLAR & SAU_RLAR_LADDR_Msk) |
                          (sau_cfg[i].nsc ? SAU_RLAR_NSC_Msk : 0U) |
                          SAU_RLAR_ENABLE_Msk)) {
            FIH_RET(fih_int_encode(ARM_DRIVER_ERROR));
        }
    }

    FIH_RET(fih_int_encode(ARM_DRIVER_OK));
}
#endif /* TFM_FIH_PROFILE_ON */

/*------------------- Memory configuration functions -------------------------*/
#ifdef BL2
#define NR_MPC_INIT_STEP                 7
#else
#define NR_MPC_INIT_STEP                 6
#endif

FIH_RET_TYPE(int32_t) mpc_init_cfg(void)
{
#if ENABLE_PPC_MPC_SUPPORT
    int32_t ret = ARM_DRIVER_OK;

    ret = Driver_SRAM1_MPC.Initialize();
    if (ret != ARM_DRIVER_OK) {
        FIH_RET(fih_int_encode(ret));
    }

    ret = Driver_SRAM1_MPC.ConfigRegion(
                                      memory_regions.non_secure_partition_base,
                                      memory_regions.non_secure_partition_limit,
                                      ARM_MPC_ATTR_NONSECURE);
    if (ret != ARM_DRIVER_OK) {
        FIH_RET(fih_int_encode(ret));
    }

#ifdef BL2
    /* Secondary image region */
    ret = Driver_SRAM1_MPC.ConfigRegion(memory_regions.secondary_partition_base,
                                  memory_regions.secondary_partition_limit,
                                  ARM_MPC_ATTR_NONSECURE);
    if (ret != ARM_DRIVER_OK) {
        FIH_RET(fih_int_encode(ret));
    }
#endif /* BL2 */

    ret = Driver_SRAM2_MPC.Initialize();
    if (ret != ARM_DRIVER_OK) {
        FIH_RET(fih_int_encode(ret));
    }

    ret = Driver_SRAM2_MPC.ConfigRegion(NS_DATA_START, NS_DATA_LIMIT,
                                        ARM_MPC_ATTR_NONSECURE);
#if defined(PSA_API_TEST_NS) && !defined(PSA_API_TEST_IPC)
    ret = Driver_SRAM2_MPC.ConfigRegion(DEV_APIS_TEST_NVMEM_REGION_START,
                                        DEV_APIS_TEST_NVMEM_REGION_LIMIT,
                                        ARM_MPC_ATTR_NONSECURE);
#endif
    if (ret != ARM_DRIVER_OK) {
        FIH_RET(fih_int_encode(ret));
    }

    /* Lock down the MPC configuration */
    ret = Driver_SRAM1_MPC.LockDown();
    if (ret != ARM_DRIVER_OK) {
        FIH_RET(fih_int_encode(ret));
    }

    ret = Driver_SRAM2_MPC.LockDown();
    if (ret != ARM_DRIVER_OK) {
        FIH_RET(fih_int_encode(ret));
    }

    /* Add barriers to assure the MPC configuration is done before continue
     * the execution.
     */
    __DSB();
    __ISB();
#endif /* ENABLE_PPC_MPC_SUPPORT */

    FIH_RET(fih_int_encode(ARM_DRIVER_OK));
}

#ifdef TFM_FIH_PROFILE_ON
fih_int fih_verify_mpc_cfg(void)
{
#if ENABLE_PPC_MPC_SUPPORT
    ARM_MPC_SEC_ATTR attr;

    Driver_SRAM1_MPC.GetRegionConfig(memory_regions.non_secure_partition_base,
                                     memory_regions.non_secure_partition_limit,
                                     &attr);
    if (attr != ARM_MPC_ATTR_NONSECURE) {
        FIH_RET(fih_int_encode(ARM_DRIVER_ERROR));
    }

#ifdef BL2
    Driver_SRAM1_MPC.GetRegionConfig(memory_regions.secondary_partition_base,
                                     memory_regions.secondary_partition_limit,
                                     &attr);
    if (attr != ARM_MPC_ATTR_NONSECURE) {
        FIH_RET(fih_int_encode(ARM_DRIVER_ERROR));
    }
#endif /* BL2 */

    Driver_SRAM2_MPC.GetRegionConfig(NS_DATA_START, NS_DATA_LIMIT, &attr);
    if (attr != ARM_MPC_ATTR_NONSECURE) {
        FIH_RET(fih_int_encode(ARM_DRIVER_ERROR));
    }

#if defined(PSA_API_TEST_NS) && !defined(PSA_API_TEST_IPC)
    Driver_SRAM2_MPC.GetRegionConfig(DEV_APIS_TEST_NVMEM_REGION_START,
                                     DEV_APIS_TEST_NVMEM_REGION_LIMIT,
                                     &attr);
    if (attr != ARM_MPC_ATTR_NONSECURE) {
        FIH_RET(fih_int_encode(ARM_DRIVER_ERROR));
    }
#endif /* PSA_API_TEST_NS && !PSA_API_TEST_IPC */
#endif /* ENABLE_PPC_MPC_SUPPORT */

    FIH_RET(fih_int_encode(ARM_DRIVER_OK));
}
#endif /* TFM_FIH_PROFILE_ON */

/*---------------------- PPC configuration functions -------------------------*/
#define NR_PPC_INIT_STEP                 4

FIH_RET_TYPE(int32_t) ppc_init_cfg(void)
{
#if ENABLE_PPC_MPC_SUPPORT
    struct spctrl_def* spctrl = CMSDK_SPCTRL;
    struct nspctrl_def* nspctrl = CMSDK_NSPCTRL;

    /* Grant non-secure access to peripherals in the PPC0
     * (timer0 and 1, dualtimer, watchdog, mhu 0 and 1)
     */
     spctrl->apbnsppc0 |= (1U << CMSDK_TIMER0_APB_PPC_POS) |
                          (1U << CMSDK_TIMER1_APB_PPC_POS) |
                          (1U << CMSDK_DTIMER_APB_PPC_POS) |
                          (1U << CMSDK_MHU0_APB_PPC_POS) |
                          (1U << CMSDK_MHU1_APB_PPC_POS);

    /* Grant non-secure access for APB peripherals on EXP1 */
    spctrl->apbnsppcexp1 |= (1U << CMSDK_SPI0_APB_PPC_POS) |
                            (1U << CMSDK_SPI1_APB_PPC_POS) |
                            (1U << CMSDK_SPI2_APB_PPC_POS) |
                            (1U << CMSDK_SPI3_APB_PPC_POS) |
                            (1U << CMSDK_SPI4_APB_PPC_POS) |
                            (1U << CMSDK_UART0_APB_PPC_POS) |
#ifdef SECURE_UART1
    /* To statically configure a peripheral as secure, skip PPC NS peripheral
     * configuration for the given device.
     */
#else
                            (1U << CMSDK_UART1_APB_PPC_POS) |
#endif

#ifndef PSA_FF_TEST_SECURE_UART2
                            (1U << CMSDK_UART2_APB_PPC_POS) |
#endif
                            (1U << CMSDK_UART3_APB_PPC_POS) |
                            (1U << CMSDK_UART4_APB_PPC_POS) |
                            (1U << CMSDK_I2C0_APB_PPC_POS) |
                            (1U << CMSDK_I2C1_APB_PPC_POS) |
                            (1U << CMSDK_I2C2_APB_PPC_POS) |
                            (1U << CMSDK_I2C3_APB_PPC_POS);
    /* Grant non-secure access for APB peripherals on EXP2 */
    spctrl->apbnsppcexp2 |= (1U << CMSDK_FPGA_SCC_PPC_POS) |
                            (1U << CMSDK_FPGA_AUDIO_PPC_POS) |
                            (1U << CMSDK_FPGA_IO_PPC_POS);

    /* Grant non-secure access to all peripherals on AHB EXP:
     * Make sure that all possible peripherals are enabled by default
     */
    spctrl->ahbnsppcexp0 |= (1U << CMSDK_VGA_PPC_POS) |
                            (1U << CMSDK_GPIO0_PPC_POS) |
                            (1U << CMSDK_GPIO1_PPC_POS) |
                            (1U << CMSDK_GPIO2_PPC_POS) |
                            (1U << CMSDK_GPIO3_PPC_POS) |
                            (1U << MPS2_ETHERNET_PPC_POS);

    spctrl->ahbnsppcexp1 |= (1U << CMSDK_DMA0_PPC_POS) |
                            (1U << CMSDK_DMA1_PPC_POS) |
                            (1U << CMSDK_DMA2_PPC_POS) |
                            (1U << CMSDK_DMA3_PPC_POS);

    /* in NS, grant un-privileged for UART0 */
    nspctrl->apbnspppcexp1 |= (1U << CMSDK_UART0_APB_PPC_POS);

    /* in NS, grant un-privileged access for LEDs */
    nspctrl->apbnspppcexp2 |= (1U << CMSDK_FPGA_SCC_PPC_POS) |
                              (1U << CMSDK_FPGA_IO_PPC_POS);

    /* Configure the response to a security violation as a
     * bus error instead of RAZ/WI
     */
    spctrl->secrespcfg |= 1U;
#endif /* ENABLE_PPC_MPC_SUPPORT */

    FIH_RET(fih_int_encode(ARM_DRIVER_OK));
}

#ifdef TFM_FIH_PROFILE_ON
fih_int fih_verify_ppc_cfg(void)
{
#if ENABLE_PPC_MPC_SUPPORT
    struct spctrl_def* spctrl = CMSDK_SPCTRL;
    struct nspctrl_def* nspctrl = CMSDK_NSPCTRL;

    /* Check non-secure access to peripherals in the PPC0
     * (timer0 and 1, dualtimer, watchdog)
     */
    if ((!(spctrl->apbnsppc0 & (1U << CMSDK_TIMER0_APB_PPC_POS))) ||
        (!(spctrl->apbnsppc0 & (1U << CMSDK_TIMER1_APB_PPC_POS))) ||
        (!(spctrl->apbnsppc0 & (1U << CMSDK_DTIMER_APB_PPC_POS)))) {
        FIH_RET(fih_int_encode(ARM_DRIVER_ERROR));
    }

    /* Check non-secure access for APB peripherals on EXP1 */
    if ((!(spctrl->apbnsppcexp1 & (1U << CMSDK_SPI0_APB_PPC_POS))) ||
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_SPI1_APB_PPC_POS))) ||
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_SPI2_APB_PPC_POS))) ||
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_SPI3_APB_PPC_POS))) ||
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_SPI4_APB_PPC_POS))) ||
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_UART0_APB_PPC_POS))) ||
#ifdef SECURE_UART1
    /* Peripheral is statically configured as secure, skip check on PPC NS
     * peripheral configuration for the given device.
     */
#else
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_UART1_APB_PPC_POS))) ||

#endif

#ifndef PSA_FF_TEST_SECURE_UART2
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_UART2_APB_PPC_POS))) ||
#endif
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_UART3_APB_PPC_POS))) ||
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_UART4_APB_PPC_POS))) ||
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_I2C0_APB_PPC_POS))) ||
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_I2C1_APB_PPC_POS))) ||
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_I2C2_APB_PPC_POS))) ||
        (!(spctrl->apbnsppcexp1 & (1U << CMSDK_I2C3_APB_PPC_POS)))) {
        FIH_RET(fih_int_encode(ARM_DRIVER_ERROR));
    }

    /* In NS, check un-privileged for UART0 */
    if (!(nspctrl->apbnspppcexp1 & (1U << CMSDK_UART0_APB_PPC_POS))) {
        FIH_RET(fih_int_encode(ARM_DRIVER_ERROR));
    }

    /* In NS, check un-privileged access for LEDs */
    if ((!(nspctrl->apbnspppcexp2 & (1U << CMSDK_FPGA_SCC_PPC_POS))) ||
        (!(nspctrl->apbnspppcexp2 & (1U << CMSDK_FPGA_IO_PPC_POS)))) {
        FIH_RET(fih_int_encode(ARM_DRIVER_ERROR));
    }

    /* Check whether the response to a security violation is a
     * bus error instead of RAZ/WI
     */
    if (!(spctrl->secrespcfg & 1U)) {
        FIH_RET(fih_int_encode(ARM_DRIVER_ERROR));
    }
#endif /* ENABLE_PPC_MPC_SUPPORT */

    FIH_RET(fih_int_encode(ARM_DRIVER_OK));
}
#endif /* TFM_FIH_PROFILE_ON */

void ppc_configure_to_non_secure(enum ppc_bank_e bank, uint16_t pos)
{
#if ENABLE_PPC_MPC_SUPPORT
    /* Setting NS flag for peripheral to enable NS access */
    struct spctrl_def* spctrl = CMSDK_SPCTRL;
    ((uint32_t*)&(spctrl->ahbnsppc0))[bank] |= (1U << pos);
#endif
}

FIH_RET_TYPE(int32_t) ppc_configure_to_secure(enum ppc_bank_e bank, uint16_t pos)
{
#if ENABLE_PPC_MPC_SUPPORT
    /* Clear NS flag for peripheral to prevent NS access */
    struct spctrl_def* spctrl = CMSDK_SPCTRL;
    ((uint32_t*)&(spctrl->ahbnsppc0))[bank] &= ~(1U << pos);
#endif

    FIH_RET(fih_int_encode(ARM_DRIVER_OK));
}

FIH_RET_TYPE(int32_t) ppc_en_secure_unpriv(enum ppc_bank_e bank, uint16_t pos)
{
#if ENABLE_PPC_MPC_SUPPORT
    struct spctrl_def* spctrl = CMSDK_SPCTRL;
    ((uint32_t*)&(spctrl->ahbspppc0))[bank] |= (1U << pos);
#endif

    FIH_RET(fih_int_encode(ARM_DRIVER_OK));
}

FIH_RET_TYPE(int32_t) ppc_clr_secure_unpriv(enum ppc_bank_e bank, uint16_t pos)
{
#if ENABLE_PPC_MPC_SUPPORT
    struct spctrl_def* spctrl = CMSDK_SPCTRL;
    ((uint32_t*)&(spctrl->ahbspppc0))[bank] &= ~(1U << pos);
#endif

    FIH_RET(fih_int_encode(ARM_DRIVER_OK));
}

void ppc_clear_irq(void)
{
#if ENABLE_PPC_MPC_SUPPORT
    struct spctrl_def* spctrl = CMSDK_SPCTRL;
    /* Clear APB PPC EXP2 IRQ */
    spctrl->secppcintclr = CMSDK_APB_PPCEXP2_INT_POS_MASK;
#endif
}
