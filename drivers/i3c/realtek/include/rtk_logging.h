/*
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * Copyright (c) 2025, Realtek Semiconductor Corp. All rights reserved.
 */

#ifndef _RTK_LOGGING_H
#define _RTK_LOGGING_H

#include "rtk_i3c_config.h"
#include <stdio.h>

#define RTK_LOG_LEVEL_OFF     0
#define RTK_LOG_LEVEL_ERROR   1
#define RTK_LOG_LEVEL_WARNING 2
#define RTK_LOG_LEVEL_INFO    3
#define RTK_LOG_LEVEL_DEBUG   4
#define RTK_LOG_LEVEL_REG     5

#ifndef CONFIG_RTS5918_I3C_CORE_LOG_LEVEL
#define CONFIG_RTS5918_I3C_CORE_LOG_LEVEL RTK_LOG_LEVEL_OFF
#endif

#ifdef CONFIG_RTS5918_I3C_COSIM
#define LOG_PREFIX "//"
#define LOG_COSIM(...)                                                                             \
	do {                                                                                       \
		printf(__VA_ARGS__);                                                               \
	} while (0)
#else
#define LOG_PREFIX ""
#define LOG_COSIM(...)
#endif

#if CONFIG_RTS5918_I3C_CORE_LOG_LEVEL >= RTK_LOG_LEVEL_REG
#define LOG_REG(fmt, ...)                                                                          \
	do {                                                                                       \
		printf(LOG_PREFIX "[REG] [%s]: " fmt, __FUNCTION__, ##__VA_ARGS__);                \
	} while (0)
#else
#define LOG_REG(fmt, ...)
#endif

#if CONFIG_RTS5918_I3C_CORE_LOG_LEVEL >= RTK_LOG_LEVEL_DEBUG
#define LOG_DBG(fmt, ...)                                                                          \
	do {                                                                                       \
		printf(LOG_PREFIX "[DBG] [%s]: " fmt, __FUNCTION__, ##__VA_ARGS__);                \
	} while (0)

#define LOG_HEX(data, len)                                                                         \
	do {                                                                                       \
		for (size_t i = 0; i < (size_t)(len); i++) {                                       \
			printf("%s", i % 16 ? " " : "\n" LOG_PREFIX);                              \
			printf("%02x", ((uint8_t *)(data))[i]);                                    \
		}                                                                                  \
		printf("\n");                                                                      \
	} while (0)
#define ASSERT(condition)                                                                          \
	do {                                                                                       \
		if (!(condition)) {                                                                \
			printf("ASSERTION FAILED: %s at %s:%d\n", #condition, __FILE__, __LINE__); \
			for (;;)                                                                   \
				;                                                                  \
		}                                                                                  \
	} while (0)
#else
#define LOG_DBG(fmt, ...)
#define LOG_HEX(data, len)
#define ASSERT(condition)
#endif

#if CONFIG_RTS5918_I3C_CORE_LOG_LEVEL >= RTK_LOG_LEVEL_INFO
#define LOG_INF(fmt, ...)                                                                          \
	do {                                                                                       \
		printf(LOG_PREFIX "[INF] [%s]: " fmt, __FUNCTION__, ##__VA_ARGS__);                \
	} while (0)
#else
#define LOG_INF(fmt, ...)
#endif

#if CONFIG_RTS5918_I3C_CORE_LOG_LEVEL >= RTK_LOG_LEVEL_WARNING
#define LOG_WRN(fmt, ...)                                                                          \
	do {                                                                                       \
		printf(LOG_PREFIX "[WRN] [%s]: " fmt, __FUNCTION__, ##__VA_ARGS__);                \
	} while (0)
#else
#define LOG_WRN(fmt, ...)
#endif

#if CONFIG_RTS5918_I3C_CORE_LOG_LEVEL >= RTK_LOG_LEVEL_ERROR
#define LOG_ERR(fmt, ...)                                                                          \
	do {                                                                                       \
		printf(LOG_PREFIX "[ERR] [%s]: " fmt, __FUNCTION__, ##__VA_ARGS__);                \
	} while (0)
#else
#define LOG_ERR(fmt, ...)
#endif

#endif
