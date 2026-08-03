/*
 * Copyright (c) 2021-2024, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stddef.h>
#include "tfm_hal_device_header.h"
#include "target_cfg.h"
#include "tfm_hal_platform.h"
#include "tfm_plat_defs.h"
#include "uart_stdout.h"

extern const struct memory_region_limits memory_regions;

/*
 * Realtek LALU HW-crypto bring-up for the SPE.
 *
 * SPE's mbedcrypto is built with CONFIG_ENABLE_LALU_SHA2, so TF-M's crypto
 * partition (tfm_crypto_core_library_init -> psa_crypto_init) runs SHA-256 on
 * the LALU engine. SPE links its OWN copy of libmbedcrypto.a, so its internal
 * sha2_core pointer is a separate, still-NULL instance - BL2 initialising its
 * own copy does not help here. Without this, the SPE hangs right after
 * "[INF][Crypto] Provision entropy seed... complete" (the first SHA in
 * tfm_crypto_core_library_init dereferences a NULL sha2_core / an un-owned,
 * mutex-gated SHA2 register file).
 *
 * The clocks/resets are global SoC state already established by BL2, but we
 * re-run the full sequence defensively in case system_reset_cfg() perturbed
 * them. Register values / offsets mirror boot_hal_bl2.c and the legacy
 * rtkcrypto_init(); they are RTS5918 absolute physical bases.
 */
#define RTK_SHA2_REG_BASE       (0x40040000ul)
#define RTK_SHA2DMA_REG_BASE    (0x40041000ul)
#define RTK_HMAC_MUTEX          (RTK_SHA2_REG_BASE + 0x00000D00ul)
#define RTK_PKE_MUTEX           (0x40080000ul + 0x00000034ul)  /* PKE engine mutex */
#define RTK_SYSTEM_REG_BASE     (0x40100000ul)
#define RTK_SYSTEM_SYSCLKSEL    (RTK_SYSTEM_REG_BASE + 0x00000004ul)
#define RTK_SYSTEM_IPCLK0       (RTK_SYSTEM_REG_BASE + 0x00000008ul)
#define RTK_SYSTEM_IPCLK4       (RTK_SYSTEM_REG_BASE + 0x0000001Cul)
#define RTK_SYSTEM_CLKGATING0   (RTK_SYSTEM_REG_BASE + 0x0000004Cul)
#define RTK_SYSTEM_CLKGATING4   (RTK_SYSTEM_REG_BASE + 0x00000060ul)
#define RTK_SYSTEM_IPRST0       (RTK_SYSTEM_REG_BASE + 0x00000064ul)
#define RTK_SYSTEM_IPRST4       (RTK_SYSTEM_REG_BASE + 0x00000078ul)
#define RTK_SYSTEM_LC           (RTK_SYSTEM_REG_BASE + 0x0000020Cul)
#define RTK_LALU_BIT            (0x1ul << 8)    /* bank 4: LALU */
#define RTK_DMA_OTP_BITS        (0x3ul << 28)   /* bank 0: DMA(28) + OTP(29) */

/* Provided by SPE's libmbedcrypto.a (library/lalu/lalu_sha2.c). */
extern int lalu_sha2_hw_init(void *sha2_base_address, void *dmac_base_address);

static void rtk_lalu_bringup(void)
{
    /* Clock source / PLL. */
    *(volatile uint32_t *)(RTK_SYSTEM_LC) |= 0x60000000ul;
    *(volatile uint32_t *)(RTK_SYSTEM_REG_BASE) = 0xfffffffful;
    *(volatile uint32_t *)(RTK_SYSTEM_SYSCLKSEL) &= ~(0x3ul << 4);

    /* IP clocks on (LALU bank 4; DMA+OTP bank 0). */
    *(volatile uint32_t *)(RTK_SYSTEM_IPCLK4) |= RTK_LALU_BIT;
    *(volatile uint32_t *)(RTK_SYSTEM_IPCLK0) |= RTK_DMA_OTP_BITS;
    __DSB();
    __ISB();

    /* Gate -> reset pulse (assert bit=0 -> deassert bit=1) -> ungate. */
    *(volatile uint32_t *)(RTK_SYSTEM_CLKGATING4) |= RTK_LALU_BIT;
    *(volatile uint32_t *)(RTK_SYSTEM_CLKGATING0) |= RTK_DMA_OTP_BITS;
    __DSB();
    __ISB();
    *(volatile uint32_t *)(RTK_SYSTEM_IPRST4) &= ~RTK_LALU_BIT;
    *(volatile uint32_t *)(RTK_SYSTEM_IPRST0) &= ~RTK_DMA_OTP_BITS;
    __DSB();
    __ISB();
    *(volatile uint32_t *)(RTK_SYSTEM_IPRST4) |= RTK_LALU_BIT;
    *(volatile uint32_t *)(RTK_SYSTEM_IPRST0) |= RTK_DMA_OTP_BITS;
    __DSB();
    __ISB();
    *(volatile uint32_t *)(RTK_SYSTEM_CLKGATING4) &= ~RTK_LALU_BIT;
    *(volatile uint32_t *)(RTK_SYSTEM_CLKGATING0) &= ~RTK_DMA_OTP_BITS;
    __DSB();
    __ISB();

    /* Latch SHA2 + DMAC bases into this binary's sha2_core.
     * NOTE: AES/GCM stays in software (LALU GCM HW path bus-stalls at runtime),
     * so lalu_set_hw_init() is intentionally NOT called here. */
    lalu_sha2_hw_init((void *)RTK_SHA2_REG_BASE, (void *)RTK_SHA2DMA_REG_BASE);

    /* Acquire the HMAC/SHA mutex before touching the SHA engine, and the PKE
     * mutex before any LALU PKE (ECDSA) register access. Each is acquired by a
     * single read and held for the session (nothing releases it). The PKE
     * scalar-mul path (lalu_ecp_mul_simplified) writes PKE registers directly
     * and never takes the mutex itself, so acquire it here. */
    (void)*(volatile uint32_t *)(RTK_HMAC_MUTEX);
    (void)*(volatile uint32_t *)(RTK_PKE_MUTEX);
}

FIH_RET_TYPE(enum tfm_hal_status_t) tfm_hal_platform_init(void)
{
    enum tfm_plat_err_t plat_err = TFM_PLAT_ERR_SYSTEM_ERR;
    fih_int fih_rc = FIH_FAILURE;

    plat_err = enable_fault_handlers();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    plat_err = system_reset_cfg();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    FIH_CALL(init_debug, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_PLAT_ERR_SUCCESS))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    __enable_irq();
    stdio_init();

    plat_err = nvic_interrupt_target_state_cfg();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    plat_err = nvic_interrupt_enable();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

#if defined(TEST_S_FPU) || defined(TEST_NS_FPU)
    /* Set IRQn in secure mode */
    NVIC_ClearTargetState(TFM_FPU_S_TEST_IRQ);

    /* Enable FPU secure test interrupt */
    NVIC_EnableIRQ(TFM_FPU_S_TEST_IRQ);
#endif

#if defined(TEST_NS_FPU)
    /* Set IRQn in non-secure mode */
    NVIC_SetTargetState(TFM_FPU_NS_TEST_IRQ);
#if (TFM_ISOLATION_LEVEL >= 2)
    /* On isolation level 2, FPU test ARoT service runs in unprivileged mode.
     * Set SCB.CCR.USERSETMPEND as 1 to enable FPU test service to access STIR
     * register.
     */
    SCB->CCR |= SCB_CCR_USERSETMPEND_Msk;
#endif
#endif

    /* Bring up the LALU SHA-2 engine for this binary before crypto init. */
    rtk_lalu_bringup();

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}

uint32_t tfm_hal_get_ns_VTOR(void)
{
    return memory_regions.non_secure_code_start;
}

uint32_t tfm_hal_get_ns_MSP(void)
{
    return *((uint32_t *)memory_regions.non_secure_code_start);
}

uint32_t tfm_hal_get_ns_entry_point(void)
{
    return *((uint32_t *)(memory_regions.non_secure_code_start + 4));
}
