/*
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * Copyright (c) 2025, Realtek Semiconductor Corp. All rights reserved.
 */

#ifndef RTK_CORE_H_
#define RTK_CORE_H_
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include "rtk_logging.h"

#define BF_MASK(low, high) (((1U << (high - low + 1)) - 1) << (low))

#define BIT_FIELD_SET(reg, low, high, value)                                                       \
	{                                                                                          \
		(reg) = (((reg) & ~BF_MASK((low), (high))) | ((value) << (low)));                  \
	}

#define BIT_FIELD_GET(reg, low, high) (((reg) & BF_MASK((low), (high))) >> (low))

static inline void rtk_core_write8(volatile void *addr, uint8_t value)
{
	*((volatile uint8_t *)addr) = value;
	LOG_COSIM("config_wbyte (32'h%08" PRIx32 ", 32'h%02" PRIx8 ");\n", (uint32_t)addr, value);
}

static inline void rtk_core_write32(volatile void *addr, uint32_t value)
{
	*((volatile uint32_t *)addr) = value;
	LOG_COSIM("config_write (32'h%08" PRIx32 ", 32'h%08" PRIx32 ");\n", (uint32_t)addr, value);
}

static inline void rtk_core_write32_mask(volatile void *addr, uint8_t bit_low, uint8_t bit_high,
					 uint32_t value)
{
	BIT_FIELD_SET(*(volatile uint32_t *)addr, bit_low, bit_high, value);
	LOG_COSIM("config_set (32'h%08" PRIx32 ", 32'h%08" PRIx32 ", 32'h%08" PRIx32 ");\n",
		  (uint32_t)addr, value << bit_low, (uint32_t)BF_MASK(bit_low, bit_high));
}

static inline uint8_t rtk_core_read8(volatile void *addr)
{
	LOG_COSIM("config_read (32'h%08" PRIx32 ");\n", (uint32_t)addr);
	return *(volatile uint8_t *)addr;
}

static inline uint32_t rtk_core_read32(volatile void *addr)
{
	LOG_COSIM("config_read (32'h%08" PRIx32 ");\n", (uint32_t)addr);
	return *(volatile uint32_t *)addr;
}

static inline uint32_t rtk_core_read32_mask(volatile void *addr, uint8_t bit_low, uint8_t bit_high)
{
	LOG_COSIM("config_read (32'h%08" PRIx32 ");\n", (uint32_t)addr);
	return BIT_FIELD_GET((*(volatile uint32_t *)addr), bit_low, bit_high);
}

static inline void rtk_core_wait32(volatile void *addr, uint8_t bit_low, uint8_t bit_high,
				   uint32_t expected)
{
	while (BIT_FIELD_GET(*(volatile uint32_t *)addr, bit_low, bit_high) != expected)
		;
	LOG_COSIM("config_wait (32'h%08" PRIx32 ", %d, %d , 32'h%08" PRIx32 ");\n", (uint32_t)addr,
		  bit_low, bit_high - bit_low + 1, expected);
}

#endif
