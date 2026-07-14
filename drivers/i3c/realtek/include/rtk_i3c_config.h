/**
 * @file rtk_i3c_config.h
 * @brief I3C driver configuration macros
 *
 * This file contains all the configuration macros for the Realtek I3C driver.
 * Users can override these defaults by defining the macros before including
 * this header file.
 */
#ifndef RTK_I3C_CONFIG_H_
#define RTK_I3C_CONFIG_H_

/** FIFO_DEPTH = 2^FIFO_DEPTH_LOG  */
#define RTK_I3C_FIFO_DEPTH     64
/** Frequency in Hz for I3C communication (PLL125M source, IPDIV5 = /1 -> 125 MHz IP clock). */
#define RTK_I3C_FREQ_HZ        125000000U
/** Max target number to manage . */
#define RTK_I3C_MAX_TAGT_COUNT 8
/** Max dynamic address to be assigned . */
#define RTK_I3C_MAX_DYN_ADDR   0x7f

// #define RTK_I3C_LEGACY_I2C

/**
 * @brief Enable I3C User PID input signal
 */
// #define RTK_I3C_ENABLE_USER_PID

/**
 * @brief Enable Hot Join (HJ) functionality
 */
#define RTK_I3C_ENABLE_HJ

/**
 * @brief Enable In-Band Interrupt (IBI) functionality
 */
#define RTK_I3C_ENABLE_IBI

/**
 * @brief Enable Controller Role Request (CR) functionality
 */
#define RTK_I3C_ENABLE_CR

/**
 * @brief I2C baud rate configuration
 * @value Baud rate in Hz (0 to use default 400kHz)
 */
#define RTK_I3C_I2C_BAUD_HZ 400000U

/**
 * @brief I3C push-pull baud rate configuration
 * @value Baud rate in Hz (0 to use default 12.5MHz)
 */
#define RTK_I3C_I3C_PP_BAUD_HZ 12500000U

/**
 * @brief I3C open-drain baud rate configuration
 * @value Baud rate in Hz (0 to use default 2.5MHz)
 */
#define RTK_I3C_I3C_OD_BAUD_HZ 2500000U

#ifndef CONFIG_RTK_I3C_CORE_LOG_LEVEL
#define CONFIG_RTK_I3C_CORE_LOG_LEVEL 0
#endif
#endif
