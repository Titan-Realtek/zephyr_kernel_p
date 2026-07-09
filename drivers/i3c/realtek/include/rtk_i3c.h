/*
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * Copyright (c) 2025, Realtek Semiconductor Corp. All rights reserved.
 */

#ifndef RTK_I3C_H_
#define RTK_I3C_H_

#include "rtk_i3c_config.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** Check if the CCC is a broadcast address */
#define IS_CCC_BRCT(ccc)        (ccc <= 0x7f)
/** Target Characteristics info size (including PID). */
#define I3C_TAGT_CHAR_INFO_SIZE (0x08)

/**
 * @brief Driver version macros
 *
 * These macros define the major, minor, and patch versions of the RTK I3C
 * driver.
 */
#define RTK_I3C_VERSION_MAJOR 1
#define RTK_I3C_VERSION_MINOR 6
#define RTK_I3C_VERSION_PATCH 0

/* ==============================================================
 * BCR (Bus Characteristics Register) bit definitions
 * Reference: MIPI I3C Specification Table 5
 * ============================================================== */

/**< BCR[1]: IBI Request Capable (0=Not Capable, 1=Capable)   */
#define I3C_BCR_IBI_CAP (1 << 1)

/**< BCR[2]: IBI Payload (0=No data, 1=MDB follows) */
#define I3C_BCR_IBI_PAYLOAD (1 << 2)

/**< BCR[6]: Controller role (with BCR[7]=0 -> 2'b01=Controller-capable) */
#define I3C_BCR_CR_CAP (1 << 6)

/* ==============================================================
 * ENEC/DISEC event bits
 * ============================================================== */
#define I3C_EVENT_IBI (1 << 0) /**< IBI event */
#define I3C_EVENT_CR  (1 << 1) /**< Controller Role Request event */
#define I3C_EVENT_HJ  (1 << 3) /**< Hot Join event */

/**
 * @brief Message flags for transfers.
 *
 * Used to specify the direction and type of transfer.
 */
typedef enum rtk_i3c_msg_flag {
	RTK_I3C_WRITE = 0,
	RTK_I3C_READ = 1,
	RTK_I3C_DMA = 0x4, /**< Use DMA for transfer */
} rtk_i3c_msg_flag;

/**
 * @brief I3C target type enumeration
 *
 * Specifies the type of target device for I3C transfers.
 */
typedef enum rtk_i3c_tagt_type {
	RTK_I3C_TAGT_I3C,
	RTK_I3C_TAGT_I2C,
} rtk_i3c_tagt_type;

/**
 * @brief I3C message structure definition
 *
 * Defines a message structure used for I3C transfers.
 */
typedef struct rtk_i3c_msg {
	uint8_t *data; /**< Pointer to data buffer */
	size_t count;  /**< Number of transferred bytes */
	size_t len;    /**< Total length of the data buffer */
	uint8_t flags; /**< Transfer flags */
} rtk_i3c_msg;

/**
 * @brief I3C target structure definition
 *
 * Represents an I3C target device in the transfer.
 */
typedef struct rtk_i3c_tagt {
	/** @brief Target address */
	uint16_t addr;

	rtk_i3c_msg *msg; /**< Pointer to message structure */
} rtk_i3c_tagt;

/**
 * @brief CCC structure definition
 *
 * Represents a Common Command Code (CCC) message.
 */
typedef struct rtk_i3c_ccc {
	/**
	 * @brief The CCC ID (@c I3C_CCC_*).
	 */
	uint8_t id;

	rtk_i3c_msg *msg; /**< Pointer to ccc message structure */

	uint8_t num_tagts;  /**< Number of targets */
	rtk_i3c_tagt *tagt; /**< Pointer to target array */
} rtk_i3c_ccc;

/** Common Command Codes defined by MIPI I3C Specification v1.1. */
typedef enum rtk_i3c_cmd {
	I3C_CCC_BRCT_ENEC = (0x00),      /**< Enable Target initiated events. */
	I3C_CCC_BRCT_DISEC = (0x01),     /**< Disable Target initiated events. */
	I3C_CCC_BRCT_ENTAS0 = (0x02),    /**< Enter Activity State 0. */
	I3C_CCC_BRCT_ENTAS1 = (0x03),    /**< Enter Activity State 1. */
	I3C_CCC_BRCT_ENTAS2 = (0x04),    /**< Enter Activity State 2. */
	I3C_CCC_BRCT_ENTAS3 = (0x05),    /**< Enter Activity State 3. */
	I3C_CCC_BRCT_RSTDAA = (0x06),    /**< Reset Dynamic Address Assignment. */
	I3C_CCC_BRCT_ENTDAA = (0x07),    /**< Enter Dynamic Address Assignment. */
	I3C_CCC_BRCT_DEFSVLS = (0x08),   /**< Define List of Slaves. */
	I3C_CCC_BRCT_SETMWL = (0x09),    /**< Set Max Write Length. */
	I3C_CCC_BRCT_SETMRL = (0x0A),    /**< Set Max Read Length. */
	I3C_CCC_BRCT_ENTTM = (0x0B),     /**< Enter Test Mode. */
	I3C_CCC_BRCT_SETBUSCON = (0x0C), /**< Set BUS Context. */
	I3C_CCC_BRCT_ENDXFER = (0x12),   /**< Data Transfer Ending Procedure Control. */
	I3C_CCC_BRCT_ENTHDR0 = (0x20),   /**< Enter HDR Mode 0. */
	I3C_CCC_BRCT_ENTHDR1 = (0x21),   /**< Enter HDR Mode 1. */
	I3C_CCC_BRCT_ENTHDR2 = (0x22),   /**< Enter HDR Mode 2. */
	I3C_CCC_BRCT_ENTHDR3 = (0x23),   /**< Enter HDR Mode 3. */
	I3C_CCC_BRCT_ENTHDR4 = (0x24),   /**< Enter HDR Mode 4 (Reserved for future definition). */
	I3C_CCC_BRCT_ENTHDR5 = (0x25),   /**< Enter HDR Mode 5 (Reserved for future definition). */
	I3C_CCC_BRCT_ENTHDR6 = (0x26),   /**< Enter HDR Mode 6 (Reserved for future definition). */
	I3C_CCC_BRCT_ENTHDR7 = (0x27),   /**< Enter HDR Mode 7 (Reserved for future definition). */
	I3C_CCC_BRCT_SETXTIME = (0x28),  /**< Set Exchange Timing Info. */
	I3C_CCC_BRCT_SETAASA = (0x29),   /**< Set All Addresses to Static Address. */
	I3C_CCC_BRCT_RSTACT = (0x2A),    /**< Target Reset Action. */
	I3C_CCC_BRCT_DEFGRPA = (0x2B),   /**< Define List of Group Address. */
	I3C_CCC_BRCT_RSTGRPA = (0x2C),   /**< Reset Group Address. */
	I3C_CCC_BRCT_MLANE = (0x2D),     /**< Multi-Lane Data Transfer Control. */

	I3C_CCC_DRCT_ENEC = (0x80),      /**< Enable Target initiated events. */
	I3C_CCC_DRCT_DISEC = (0x81),     /**< Disable Target initiated events. */
	I3C_CCC_DRCT_ENTAS0 = (0x82),    /**< Enter Activity State 0. */
	I3C_CCC_DRCT_ENTAS1 = (0x83),    /**< Enter Activity State 1. */
	I3C_CCC_DRCT_ENTAS2 = (0x84),    /**< Enter Activity State 2. */
	I3C_CCC_DRCT_ENTAS3 = (0x85),    /**< Enter Activity State 3. */
	I3C_CCC_DRCT_RSTDAA = (0x86),    /**< Reset Dynamic Address Assignment (DEPRECATED v1.0). */
	I3C_CCC_DRCT_SETDASA = (0x87),   /**< Set Dynamic Address from Static Address. */
	I3C_CCC_DRCT_SETNEWDA = (0x88),  /**< Set New Dynamic Address. */
	I3C_CCC_DRCT_SETMWL = (0x89),    /**< Set Max Write Length. */
	I3C_CCC_DRCT_SETMRL = (0x8A),    /**< Set Max Read Length. */
	I3C_CCC_DRCT_GETMWL = (0x8B),    /**< Get Max Write Length. */
	I3C_CCC_DRCT_GETMRL = (0x8C),    /**< Get Max Read Length. */
	I3C_CCC_DRCT_GETPID = (0x8D),    /**< Get Provisional ID. */
	I3C_CCC_DRCT_GETBCR = (0x8E),    /**< Get Bus Characteristic Register. */
	I3C_CCC_DRCT_GETDCR = (0x8F),    /**< Get Device Characteristic Register. */
	I3C_CCC_DRCT_GETSTATUS = (0x90), /**< Get Device Status. */
	I3C_CCC_DRCT_GETACCMST = (0x91), /**< Get Accept Mastership. */
	I3C_CCC_DRCT_ENDXFER = (0x92),   /**< Data Transfer Ending Procedure Control. */
	I3C_CCC_DRCT_SETBRGTGT = (0x93), /**< Set Bridge Targets. */
	I3C_CCC_DRCT_GETMXDS = (0x94),   /**< Get Max Data Speed. */
	I3C_CCC_DRCT_GETHDRCAP = (0x95), /**< Get HDR Capability. */
	I3C_CCC_DRCT_SETROUTE = (0x96),  /**< Set Route. */
	I3C_CCC_DRCT_D2DXFER = (0x97),   /**< Device to Device(s) Tunneling Control. */
	I3C_CCC_DRCT_SETXTIME = (0x98),  /**< Set Exchange Timing Information. */
	I3C_CCC_DRCT_GETXTIME = (0x99),  /**< Get Exchange Timing Information. */
	I3C_CCC_DRCT_RSTACT = (0x9A),    /**< Reset Target Action. */
	I3C_CCC_DRCT_SETGRPA = (0x9B),   /**< Set Group Address. */
	I3C_CCC_DRCT_RSTGRPA = (0x9C),   /**< Reset Group Address. */
	I3C_CCC_DRCT_MLANE = (0x9D),     /**< Multi-Lane Data Transfer Control. */
} rtk_i3c_cmd;

/**
 * @brief I3C receive buffer structure definition
 *
 * Structure for managing receive buffers in I3C operations.
 */
typedef struct rtk_i3c_rx_buffer {
	rtk_i3c_msg msg;

	/* Must be the last member */
	bool buffer_requested; /**< Flag for new buffer request when read buffer is
				  full. */
} rtk_i3c_rx_buffer;

/**
 * @brief I3C write buffer structure definition
 *
 * Structure for managing write buffers in I3C operations.
 */
typedef struct rtk_i3c_tx_buffer {
	rtk_i3c_msg msg;
} rtk_i3c_tx_buffer;

/**
 * @brief I3C HDR mode enumeration
 *
 * Enumerates the different modes supported by I3C.
 */
typedef enum rtk_i3c_mode {
	RTK_I3C_SDR_MODE = 0,
	RTK_I3C_HDR_DDR = 0x1,
	RTK_I3C_I2C = 0x10,
	RTK_I3C_I2C_10BIT = 0x11,
	RTK_I3C_MODE_MAX,
} rtk_i3c_mode;

/**
 * @brief I3C event types
 *
 * Defines various events that can occur during I3C operations.
 */
typedef enum rtk_i3c_event {
	/** @brief Events that only occur in Controller mode. */
	RTK_I3C_EVENT_NONE, /**< No event or the event is cleared by application */

	/**
	 * @brief A Target device has finished writing its PID, BCR, and DCR. This
	 * information is provided in
	 * @ref i3c_callback_args_t::p_Target_info.
	 */
	RTK_I3C_EVENT_ADDRESS_ASSIGNMENT_PHASE,
	RTK_I3C_EVENT_IBI_READ_COMPLETE, /**< An IBI has successfully been read. */

	/**
	 * @brief There is no more space in the IBI read buffer. The application may
	 * provide another buffer by calling
	 */
	RTK_I3C_EVENT_IBI_READ_BUFFER_FULL,

	/**
	 * @brief There is no more space in the read buffer. The application may
	 * provide another buffer by calling
	 */
	RTK_I3C_EVENT_READ_BUFFER_FULL,
	RTK_I3C_EVENT_IBI_WRITE_COMPLETE,        /**< A IBI was written successfully. */
	RTK_I3C_EVENT_HDR_EXIT_PATTERN_DETECTED, /**< The HDR exit pattern was
						    detected on the bus. */

	/** @brief Events that are common to Controller and Target mode. */

	RTK_I3C_EVENT_ADDRESS_ASSIGNMENT_COMPLETE, /**< Dynamic Address Assignment
						      has completed. */
	RTK_I3C_EVENT_COMMAND_COMPLETE,            /**< A command was completed. */
	RTK_I3C_EVENT_WRITE_COMPLETE,              /**< A write transfer has completed. */
	RTK_I3C_EVENT_READ_COMPLETE,               /**< A read transfer has completed. */
	RTK_I3C_EVENT_TIMEOUT_DETECTED,            /**< SCL is stuck at the logic high or logic
						      low level during a transfer. */
	RTK_I3C_EVENT_ARBITRATE_FAIL,              /**< SDA arbitration fail occurred. */
	RTK_I3C_EVENT_INTERNAL_ERROR,              /**< An internal error occurred. */
} rtk_i3c_event;

/** @brief The type of In-Band Interrupt. */
typedef enum rtk_i3c_ibi_type {
	RTK_I3C_IBI_INTR,     /**< Application specific In-Band Interrupt for
				      notifying the controller when an event occurs. */
	RTK_I3C_IBI_HOT_JOIN, /**< Request the controller to perform the
				      Dynamic Address Assignment process. */
	RTK_I3C_IBI_CTRL_REQ  /**< Request the controller to give up control of
				      the bus. */
} rtk_i3c_ibi_type;

/** @brief I3C target characteristics information. */
typedef struct rtk_i3c_tagt_char_info {
	uint64_t pid: 48; /**< Device Provisioned ID register  */
	uint8_t bcr;      /**< Bus characteristics register information. */
	uint8_t dcr;      /**< Device characteristics register information. */
} rtk_i3c_tagt_char_info;

/** @brief Maximum Sustained Data Rate for non-CCC messages sent by Controller
 * to Target. */
typedef enum rtk_i3c_data_rate {
	RTK_I3C_DATA_RATE_MAX = 0,  /**< There is no data rate limit. */
	RTK_I3C_DATA_RATE_8MHZ = 1, /**< The max sustained data rate is 8 Mhz. */
	RTK_I3C_DATA_RATE_6MHZ = 2, /**< The max sustained data rate is 6 Mhz. */
	RTK_I3C_DATA_RATE_4MHZ = 3, /**< The max sustained data rate is 4 Mhz. */
	RTK_I3C_DATA_RATE_2MHZ = 4, /**< The max sustained data rate is 2 Mhz. */
} rtk_i3c_data_rate;

/** @brief Supported activity states for ENTASn Command (See ENTASn in the MIPI
 * I3C Specification v1.0). */
typedef enum rtk_i3c_activity_state {
	I3C_ACTIVITY_STATE_ENTAS0 = 0U, /**< Activity Interval (1 microsecond). */
	I3C_ACTIVITY_STATE_ENTAS1 = 1U, /**< Activity Interval (100 microseconds). */
	I3C_ACTIVITY_STATE_ENTAS2 = 2U, /**< Activity Interval (2 milliseconds). */
	I3C_ACTIVITY_STATE_ENTAS3 = 3U, /**< Activity Interval (50 milliseconds). */
} rtk_i3c_activity_state;

/** @brief I3C target command response information. */
typedef struct rtk_i3c_tagt_resp_info {
	uint16_t max_read_len;                 /**< Maximum read length for CCC.SETMWL/GETMWL */
	uint16_t max_write_len;                /**< Maximum write length for CCC.SETMRL/GETMRL */
	uint8_t ibi_payload_len;               /**< IBI payload size for CCC.SETMRL/GETMRL */
	rtk_i3c_data_rate max_write_rate;      /**< Max Write Data Rate. */
	rtk_i3c_data_rate max_read_rate;       /**< Max Read Data Rate. */
	rtk_i3c_activity_state activity_state; /**< Activity states for ENTASn Command */
	bool read_turnaround_time_enable;      /**< Enable transmission of the of the Max
						  Read Max Read Turnaround Time. */
	uint32_t read_turnaround_time;         /**< Max Read Turnaround Time. */
	uint8_t hdr_mode; /**< Support hdr mode, could be OR logic in i3c_hdr_mode. */
} rtk_i3c_tagt_resp_info;

/** @brief I3C target characteristics information. */
typedef struct rtk_i3c_tagt_info {
	rtk_i3c_tagt_char_info char_info; /**< Target characteristics information*/
	rtk_i3c_tagt_resp_info resp_info; /**< I3C target command response information. */
	uint8_t dyn_addr;                 /**< dynamic address */
	uint16_t stc_addr;                /**< static address */
	bool is_i2c;                      /**< i2c target */
} rtk_i3c_tagt_info;

/** @brief Arguments that are passed to the user callback when an event occurs.
 */
typedef struct rtk_i3c_callback_args {
	rtk_i3c_event event;       /**< The type of event that has occurred. */
	uint32_t count;            /**< Number of bytes transferred. */
	uint32_t len;              /**< Number of bytes expected. */
	uint8_t dyn_addr;          /**< The dynamic address that was assigned to the Target
				      during ENTDAA. */
	rtk_i3c_ibi_type ibi_type; /**< The type of IBI that has been received. */
	uint8_t ibi_addr;          /**< The address of the device that sent the IBI. */
	uint8_t ccc_id;            /**< The command code of the received command. */
	rtk_i3c_tagt_char_info *tagt_char_info; /**< Target characteristics
						   information during DAA phase */
	const void *ctx;                        /**< APP provided context */
} rtk_i3c_callback_args;

/**
 * @brief IBI configuration structure
 *
 * Configuration for In-Band Interrupt handling.
 */
typedef struct rtk_i3c_ibi_cfg {
	bool enable_hj: 1;  /**< Enable hot join. */
	bool enable_ibi: 1; /**< Enable in-band interrupt. */
	bool enable_cr: 1;  /**< Enable controller role requests. */
} rtk_i3c_ibi_cfg;

/**
 * @brief Bitrate configuration structure
 *
 * Configuration for I3C and I2C bitrates.
 */
typedef struct rtk_i3c_bitrate_cfg {
	uint32_t i2c_baud_hz;     /**< Desired i2c baud rate settings. */
	uint32_t i3c_pp_baud_hz;  /**< Desired push pull baud rate settings. */
	uint32_t i3c_od_baud_hz;  /**< Desired open drain baud rate settings. */
	uint32_t i3c_fst_baud_hz; /**< Desired first header address baud rate
				     settings. */
} rtk_i3c_bitrate_cfg;

/**
 * @brief I3C role enumeration
 *
 * Defines possible roles for an I3C device.
 */
typedef enum rtk_i3c_role {
	RTK_I3C_TAGT = 0x1,      /**< Target role CCR.bit1 = 0, CCR.bit0 = 1 */
	RTK_I3C_CTRL_PRIM = 0x2, /**< Primary Controller role CCR.bit1 = 1, CCR.bit0 = 0 */
	RTK_I3C_CTRL_I2C = 0x3,  /**< I2C Controller Only.*/
	RTK_I3C_CTRL_SEC = 0x4,  /**< Secondary Controller role CCR.bit1 = 0,
				    CCR.bit0 = 0, CCR.bit2 = 1 */
} rtk_i3c_role;

/**
 * @brief I3C DMA configuration
 *
 * Enumerates DMA-related options for I3C.
 */
typedef enum rtk_i3c_dma {
	RTK_I3C_DMA_DIS = 0x0, /**< Disable DMAC hardware handshaking mode. CCR.bit15 = 0 */
	RTK_I3C_DMA_TX_EN =
		0x1, /**< Enable RTK_I3C as DMAC destination peripheral). CCR.bit13 = 1 */
	RTK_I3C_DMA_RX_EN = 0x2, /**< Enable RTK_I3C as DMAC source peripheral). CCR.bit14 = 1 */
	RTK_I3C_DMA_EN = 0x4,    /**< Enable DMAC hardware handshaking mode. CCR.bit15 = 1 */
} rtk_i3c_dma;

/**
 * @brief I3C timing configuration structure
 *
 * Configuration for timing parameters of I3C bus operations.
 */
typedef struct rtk_i3c_timing_cfg {
	uint32_t bus_free_ns;
	uint32_t bus_available_ns;
	uint32_t bus_idle_ns;
	uint32_t i3c_od_baud_hz;       /**< OD SCL rate, selects OD high/setup timing. */
	bool enable_timeout_detection; /**< Enable I3CC BUS timeout function. */
} rtk_i3c_timing_cfg;

/**
 * @brief I3C configuration structure
 *
 * Common configuration structure for both Controller and Target roles.
 */
typedef struct rtk_i3c_common_cfg {
	uint8_t instance_id;       /**< Instance ID for the I3C peripherals . */
	uint32_t i3c_freq_hz;      /**< Frequency of the I3C peripheral in Hz. */
	rtk_i3c_role role;         /**< Device role. */
	rtk_i3c_ibi_cfg ibi;       /**< IBI control configuration. */
	rtk_i3c_timing_cfg timing; /**< Bus condition detection timing */
	void (*callback)(rtk_i3c_callback_args *const args); /**< Pointer to the user callback. */
	const void *ctx;                                     /**< APP provided context */
} rtk_i3c_common_cfg;

/**
 * @brief Controller configuration structure
 *
 * Additional configuration for Controller role devices.
 */
typedef struct rtk_i3c_cfg {
	rtk_i3c_common_cfg common_cfg;   /**< Common configuration structure. */
	rtk_i3c_bitrate_cfg bitrate_cfg; /**< Controller only Bitrate configuration. */
	void *tagt_table;
	void *addr_slot;
	rtk_i3c_tagt_info tagt_info; /**< Target only information. */
} rtk_i3c_cfg;

/**
 * @brief I3C context structure
 *
 * Context structure for I3C driver instances.
 */
typedef struct rtk_i3c_ctx {
	void *core;              /**< Base address for the I3C peripheral registers. */
	volatile uint32_t state; /**< Frequency of the I3C peripheral in Hz. */
	uint8_t ccc_id;          /**< Current Common Code */
	rtk_i3c_rx_buffer rx_buffer;
	rtk_i3c_tx_buffer tx_buffer;
	rtk_i3c_rx_buffer ibi_buffer;

#if defined(CONFIG_RTK_I3C_CTRL) || defined(__DOXYGEN__)
	rtk_i3c_tagt_char_info tagt_char_info; /**< Current Target characteristics
						  information during DAA phase */
#endif
	rtk_i3c_cfg *cfg;
} rtk_i3c_ctx;

/**
 * @brief Get controller or target configuration.
 *        If current config is NULL, then returen default config.
 * @param ctx Pointer to I3C context
 * @param config Pointer to target configuration
 * @return Status code
 */
void rtk_i3c_get_config(rtk_i3c_ctx *ctx, rtk_i3c_cfg *config);

#if defined(CONFIG_RTK_I3C_CTRL) || defined(__DOXYGEN__)
/**
 * @brief Initialize I3C controller
 * @param ctx Pointer to I3C context
 * @param config Pointer to controller configuration.
 *               Set to NULL to use default config.
 * @return Status code
 */
int rtk_i3c_ctrl_init(rtk_i3c_ctx *ctx, rtk_i3c_cfg *config);

/**
 * @brief Perform Dynamic Address Assignment
 * @param ctx Pointer to I3C context
 * @return Status code
 */
int rtk_i3c_do_daa(rtk_i3c_ctx *ctx);

/**
 * @brief Send CCC command
 * @param ctx Pointer to I3C context
 * @param payload Pointer to CCC payload
 * @param i3c_mode Indicate this is a SDR or specific HDR mode transfer
 * @param restart Indicate there will be another transfer followed
 * @return Status code
 */
int rtk_i3c_do_ccc(rtk_i3c_ctx *ctx, const rtk_i3c_ccc *ccc, uint8_t i3c_mode, bool restart);

/**
 * @brief Perform controller transfer
 * @param ctx Pointer to I3C context
 * @param tagt Pointer to target structure
 * @param i3c_mode Indicate this is a SDR or specific HDR mode transfer
 * @param restart Indicate there will be another transfer followed
 * @return Status code
 */
int rtk_i3c_ctrl_xfer(rtk_i3c_ctx *ctx, const rtk_i3c_tagt *tagt, uint8_t i3c_mode, bool restart);

/**
 * @brief Read IBI data. Only for controller
 * @param ctx Pointer to I3C context
 * @param msg Pointer to message structure
 * @return Status code
 */
int rtk_i3c_ibi_read(rtk_i3c_ctx *ctx, rtk_i3c_msg *msg);

/**
 * @brief Deinitialize I3C controller
 * @param ctx Pointer to I3C context
 * @return Status code
 */
int rtk_i3c_ctrl_deinit(rtk_i3c_ctx *ctx);

#endif /* defined(CONFIG_RTK_I3C_CTRL) || ... */

#if defined(CONFIG_RTK_I3C_TAGT) || defined(__DOXYGEN__)
/**
 * @brief Initialize I3C target
 * @param ctx Pointer to I3C context
 * @param config Pointer to target configuration
 * @return Status code
 */
int rtk_i3c_tagt_init(rtk_i3c_ctx *ctx, rtk_i3c_cfg *config);

/**
 * @brief Perform target transfer
 * @param ctx Pointer to I3C context
 * @param msg Pointer to message structure
 * @return Status code
 */
int rtk_i3c_tagt_xfer(rtk_i3c_ctx *ctx, rtk_i3c_msg *msg);

/**
 * @brief Write IBI data. Only for target.
 * @param ctx Pointer to I3C context
 * @param ibi_type The type of IBI (RTK_I3C_IBI_INTR, RTK_I3C_IBI_HOT_JOIN, RTK_I3C_IBI_CTRL_REQ)
 * @param msg Pointer to message structure
 * @return Status code
 */
int rtk_i3c_ibi_write(rtk_i3c_ctx *ctx, rtk_i3c_ibi_type ibi_type, rtk_i3c_msg *msg);

/**
 * @brief Deinitialize I3C target
 * @param ctx Pointer to I3C context
 * @return Status code
 */
int rtk_i3c_tagt_deinit(rtk_i3c_ctx *ctx);
#endif

/**
 * @brief Status code definitions
 *
 * Status codes used throughout the I3C driver for indicating success or failure
 * of operations.
 */
#define SEVERITY_SUCCESS 0x0
#define SEVERITY_INFO    0x1
#define SEVERITY_WARNING 0x4
#define SEVERITY_ERROR   0x8

#define SOURCE_RTK_I3C 0x05

#define STATUS_CONSTRUCT(severity, source, code) (((severity) << 28) | ((source) << 16) | (code))

#define RTK_I3C_INVAL_PARAM STATUS_CONSTRUCT(SEVERITY_ERROR, SOURCE_RTK_I3C, 0x0001)

#define RTK_I3C_DMA_ERROR STATUS_CONSTRUCT(SEVERITY_ERROR, SOURCE_RTK_I3C, 0x0002)

#define RTK_I3C_NOT_SUPPORTED STATUS_CONSTRUCT(SEVERITY_ERROR, SOURCE_RTK_I3C, 0x0003)

#define RTK_I3C_XFER_TERMINATION STATUS_CONSTRUCT(SEVERITY_ERROR, SOURCE_RTK_I3C, 0x0004)

#define RTK_I3C_READ_BUFFER_FULL STATUS_CONSTRUCT(SEVERITY_ERROR, SOURCE_RTK_I3C, 0x0005)

#define RTK_I3C_BUSY STATUS_CONSTRUCT(SEVERITY_ERROR, SOURCE_RTK_I3C, 0x0006)

#define RTK_I3C_ADDR_OCCUPIED STATUS_CONSTRUCT(SEVERITY_ERROR, SOURCE_RTK_I3C, 0x0007)

#define RTK_I3C_TAGT_NOT_FOUND STATUS_CONSTRUCT(SEVERITY_ERROR, SOURCE_RTK_I3C, 0x0008)

#define RTK_I3C_TXFIFO_WILL_FULL STATUS_CONSTRUCT(SEVERITY_ERROR, SOURCE_RTK_I3C, 0x0009)

#define RTK_I3C_NOT_ENABLED STATUS_CONSTRUCT(SEVERITY_ERROR, SOURCE_RTK_I3C, 0x000a)

#define RTK_I3C_TIMEOUT STATUS_CONSTRUCT(SEVERITY_ERROR, SOURCE_RTK_I3C, 0x000b)

#endif /* RTK_I3C_H_ */
