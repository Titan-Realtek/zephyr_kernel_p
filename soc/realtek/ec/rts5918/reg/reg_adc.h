/*
 * Copyright (c) 2025 Realtek, SIBG-SD7
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_SOC_REALTEK_RTS5918_REG_ADC_H
#define ZEPHYR_SOC_REALTEK_RTS5918_REG_ADC_H

struct adc_regs{                                /*!< (@ 0x40251900) ADC Structure                                              */
  union {
    uint32_t CTRL;                        /*!< (@ 0x00000000) CTRL                                                       */

    struct {
      uint32_t cfg_sar_on : 1;            /*!< [0..0] nan                                                                */
      uint32_t cfg_sar_ldo_on : 1;        /*!< [1..1] nan                                                                */
      uint32_t cfg_sar_dalign : 1;        /*!< [2..2] nan                                                                */
      uint32_t cfg_sar_cont : 1;          /*!< [3..3] nan                                                                */
      uint32_t cfg_sar_discnum : 3;       /*!< [6..4] nan                                                                */
      uint32_t cfg_fake_data_rdy : 1;     /*!< [7..7] nan                                                                */
      uint32_t cfg_fake_data_rdy_sel : 1; /*!< [8..8] nan                                                                */
      uint32_t cfg_sar_acc_conut : 3;     /*!< [11..9] nan                                                               */
      uint32_t sel_adc_testout_ch : 5;    /*!< [16..12] nan                                                              */
      uint32_t sel_adc_ch_mode : 1;       /*!< [17..17] nan                                                              */
      uint32_t fast_comparator : 1;       /*!< [18..18] nan                                                              */
      uint32_t cfg_sar_buf_on : 1;        /*!< [19..19] nan                                                              */
      uint32_t cfg_adc_pre1 : 3;          /*!< [22..20] nan                                                              */
            uint32_t            : 9;
    } CTRL_b;
  } ;

  union {
    uint32_t REGULAR;                     /*!< (@ 0x00000004) REGULAR                                                    */

    struct {
      uint32_t cfg_sar_rl : 4;            /*!< [3..0] nan                                                                */
      uint32_t cfg_sar_rexttrig : 1;      /*!< [4..4] nan                                                                */
      uint32_t cfg_sar_rextsel : 3;       /*!< [7..5] nan                                                                */
      uint32_t cfg_sar_rdiscen : 1;       /*!< [8..8] nan                                                                */
      uint32_t cfg_sar_rswstart : 1;      /*!< [9..9] nan                                                                */
            uint32_t            : 6;
      uint32_t cfg_fake_dout : 15;        /*!< [30..16] nan                                                              */
      uint32_t cfg_fake_dout_sel : 1;     /*!< [31..31] nan                                                              */
    } REGULAR_b;
  } ;

  union {
    uint32_t SAMPLE0;                     /*!< (@ 0x00000008) SAMPLE0                                                    */

    struct {
      uint32_t cfg_sar_smpt0 : 3;         /*!< [2..0] nan                                                                */
      uint32_t cfg_sar_smpt1 : 3;         /*!< [5..3] nan                                                                */
      uint32_t cfg_sar_smpt2 : 3;         /*!< [8..6] nan                                                                */
      uint32_t cfg_sar_smpt3 : 3;         /*!< [11..9] nan                                                               */
      uint32_t cfg_sar_smpt4 : 3;         /*!< [14..12] nan                                                              */
      uint32_t cfg_sar_smpt5 : 3;         /*!< [17..15] nan                                                              */
      uint32_t cfg_sar_smpt6 : 3;         /*!< [20..18] nan                                                              */
      uint32_t cfg_sar_smpt7 : 3;         /*!< [23..21] nan                                                              */
      uint32_t cfg_sar_smpt8 : 3;         /*!< [26..24] nan                                                              */
      uint32_t cfg_sar_smpt9 : 3;         /*!< [29..27] nan                                                              */
            uint32_t            : 2;
    } SAMPLE0_b;
  } ;

  union {
    uint32_t SAMPLE1;                     /*!< (@ 0x0000000C) SAMPLE1                                                    */

    struct {
      uint32_t cfg_sar_smpt10 : 3;        /*!< [2..0] nan                                                                */
      uint32_t cfg_sar_smpt11 : 3;        /*!< [5..3] nan                                                                */
            uint32_t            : 26;
    } SAMPLE1_b;
  } ;

  union {
    uint32_t RSQ0;                        /*!< (@ 0x00000010) SEQ0                                                       */

    struct {
      uint32_t cfg_sar_rsq1 : 5;          /*!< [4..0] nan                                                                */
      uint32_t cfg_sar_rsq2 : 5;          /*!< [9..5] nan                                                                */
      uint32_t cfg_sar_rsq3 : 5;          /*!< [14..10] nan                                                              */
      uint32_t cfg_sar_rsq4 : 5;          /*!< [19..15] nan                                                              */
      uint32_t cfg_sar_rsq5 : 5;          /*!< [24..20] nan                                                              */
      uint32_t cfg_sar_rsq6 : 5;          /*!< [29..25] nan                                                              */
            uint32_t            : 2;
    } RSQ0_b;
  } ;

  union {
    uint32_t RSQ1;                        /*!< (@ 0x00000014) SEQ1                                                       */

    struct {
      uint32_t cfg_sar_rsq7 : 5;          /*!< [4..0] nan                                                                */
      uint32_t cfg_sar_rsq8 : 5;          /*!< [9..5] nan                                                                */
      uint32_t cfg_sar_rsq9 : 5;          /*!< [14..10] nan                                                              */
      uint32_t cfg_sar_rsq10 : 5;         /*!< [19..15] nan                                                              */
      uint32_t cfg_sar_rsq11 : 5;         /*!< [24..20] nan                                                              */
      uint32_t cfg_sar_rsq12 : 5;         /*!< [29..25] nan                                                              */
            uint32_t            : 2;
    } RSQ1_b;
  } ;

  union {
    uint32_t RSQ2;                        /*!< (@ 0x00000018) SEQ2                                                       */

    struct {
            uint32_t            : 10;
      uint32_t cfg_sar_rsq15 : 5;         /*!< [14..10] nan                                                              */
      uint32_t cfg_sar_rsq16 : 5;         /*!< [19..15] nan                                                              */
            uint32_t            : 12;
    } RSQ2_b;
  } ;

  union {
    uint32_t INTEN;                       /*!< (@ 0x0000001C) INTEN                                                      */

    struct {
      uint32_t cfg_sar_ch_en_ch0 : 1;     /*!< [0..0] nan                                                                */
      uint32_t cfg_sar_ch_en_ch1 : 1;     /*!< [1..1] nan                                                                */
      uint32_t cfg_sar_ch_en_ch2 : 1;     /*!< [2..2] nan                                                                */
      uint32_t cfg_sar_ch_en_ch3 : 1;     /*!< [3..3] nan                                                                */
      uint32_t cfg_sar_ch_en_ch4 : 1;     /*!< [4..4] nan                                                                */
      uint32_t cfg_sar_ch_en_ch5 : 1;     /*!< [5..5] nan                                                                */
      uint32_t cfg_sar_ch_en_ch6 : 1;     /*!< [6..6] nan                                                                */
      uint32_t cfg_sar_ch_en_ch7 : 1;     /*!< [7..7] nan                                                                */
      uint32_t cfg_sar_ch_en_ch8 : 1;     /*!< [8..8] nan                                                                */
      uint32_t cfg_sar_ch_en_ch9 : 1;     /*!< [9..9] nan                                                                */
      uint32_t cfg_sar_ch_en_ch10 : 1;    /*!< [10..10] nan                                                              */
      uint32_t cfg_sar_ch_en_ch11 : 1;    /*!< [11..11] nan                                                              */
      uint32_t cfg_sar_ch_en_ch12 : 1;    /*!< [12..12] nan                                                              */
      uint32_t cfg_sar_ch_en_ch13 : 1;    /*!< [13..13] nan                                                              */
      uint32_t cfg_sar_ch_en_ch14 : 1;    /*!< [14..14] nan                                                              */
      uint32_t cfg_sar_ch_en_ch15 : 1;    /*!< [15..15] nan                                                              */
      uint32_t cfg_sar_rdr_updaie : 1;    /*!< [16..16] nan                                                              */
      uint32_t cfg_sar_eocie : 1;         /*!< [17..17] nan                                                              */
      uint32_t cfg_sar_normal_eocie : 1;  /*!< [18..18] nan                                                              */
      uint32_t cfg_sar_group_eocie : 1;   /*!< [19..19] nan                                                              */
      uint32_t cfg_sar_continue_eocie : 1;/*!< [20..20] nan                                                              */
      uint32_t cfg_sar_rdr_upda : 1;      /*!< [21..21] nan                                                              */
      uint32_t cfg_sar_eoc : 1;           /*!< [22..22] nan                                                              */
      uint32_t cfg_sar_normal_eoc : 1;    /*!< [23..23] nan                                                              */
      uint32_t cfg_sar_group_eoc : 1;     /*!< [24..24] nan                                                              */
      uint32_t cfg_sar_continue_eoc : 1;  /*!< [25..25] nan                                                              */
            uint32_t            : 6;
    } INTEN_b;
  } ;

  union {
    uint32_t INTEN1;                      /*!< (@ 0x00000020) INTEN1                                                     */

    struct {
		uint32_t cfg_sar_ch0_done : 1;      /*!< [0..0] nan                                                                */
		uint32_t cfg_sar_ch1_done : 1;      /*!< [1..1] nan                                                                */
		uint32_t cfg_sar_ch2_done : 1;      /*!< [2..2] nan                                                                */
		uint32_t cfg_sar_ch3_done : 1;      /*!< [3..3] nan                                                                */
		uint32_t cfg_sar_ch4_done : 1;      /*!< [4..4] nan                                                                */
		uint32_t cfg_sar_ch5_done : 1;      /*!< [5..5] nan                                                                */
		uint32_t cfg_sar_ch6_done : 1;      /*!< [6..6] nan                                                                */
		uint32_t cfg_sar_ch7_done : 1;      /*!< [7..7] nan                                                                */
		uint32_t cfg_sar_ch8_done : 1;      /*!< [8..8] nan                                                                */
		uint32_t cfg_sar_ch9_done : 1;      /*!< [9..9] nan                                                                */
		uint32_t cfg_sar_ch10_done : 1;     /*!< [10..10] nan                                                              */
		uint32_t cfg_sar_ch11_done : 1;     /*!< [11..11] nan                                                              */
		uint32_t cfg_sar_ch12_done : 1;     /*!< [12..12] nan                                                              */
		uint32_t cfg_sar_ch13_done : 1;     /*!< [13..13] nan                                                              */
		uint32_t cfg_sar_ch14_done : 1;     /*!< [14..14] nan                                                              */
		uint32_t cfg_sar_ch15_done : 1;     /*!< [15..15] nan                                                              */
		uint32_t cfg_sar_rdr_upda_done : 1; /*!< [16..16] nan                                                              */
		uint32_t cfg_sar_eoc_done : 1;      /*!< [17..17] nan                                                              */
		uint32_t cfg_sar_normal_eoc_done : 1;/*!< [18..18] nan                                                             */
		uint32_t cfg_sar_group_eoc_done : 1;/*!< [19..19] nan                                                              */
		uint32_t cfg_sar_continue_eoc_done : 1;/*!< [20..20] nan                                                           */
        uint32_t            : 11;
    } INTEN1_b;
  } ;

  union {
    uint32_t DEBUG;                       /*!< (@ 0x00000024) DEBUG                                                      */

    struct {
      uint32_t cfg_sar_rdr_di : 16;       /*!< [15..0] nan                                                               */
      uint32_t cfg_sar_rdr_upda_di : 1;   /*!< [16..16] nan                                                              */
      uint32_t cfg_sar_rbusy_di : 1;      /*!< [17..17] nan                                                              */
      uint32_t cfg_sar_eoc_di : 1;        /*!< [18..18] nan                                                              */
      uint32_t            : 13;
    } DEBUG_b;
  } ;

  union {
    uint32_t CALICTRL0;                   /*!< (@ 0x00000028) CALICTRL0                                                  */

    struct {
		uint32_t cfg_sar_bypass_sel : 1;    /*!< [0..0] nan                                                                */
		uint32_t fake_data_sel : 1;         /*!< [1..1] nan                                                                */
		uint32_t cfg_sar_cal_bypass : 1;    /*!< [2..2] nan                                                                */
		uint32_t            : 1;
		uint32_t coef_a     : 19;           /*!< [22..4] nan                                                               */
		uint32_t            : 9;
    } CALICTRL0_b;
  } ;

  union {
    uint32_t CALICTRL1;                   /*!< (@ 0x0000002C) CALICTRL1                                                  */

    struct {
      	uint32_t coef_b     : 27;           /*!< [26..0] nan                                                               */
        uint32_t            : 5;
    } CALICTRL1_b;
  } ;

  union {
    uint32_t CALIDATA0;                   /*!< (@ 0x00000030) CALIDATA0                                                  */

    struct {
    	uint32_t cfg_sar_adc_out_ch0 : 12;  /*!< [11..0] nan                                                               */
        uint32_t            : 4;
      	uint32_t cfg_sar_adc_out_ch1 : 12;  /*!< [27..16] nan                                                              */
    	uint32_t            : 4;
    } CALIDATA0_b;
  } ;

  union {
    uint32_t CALIDATA1;                   /*!< (@ 0x00000034) CALIDATA1                                                  */

    struct {
    	uint32_t cfg_sar_adc_out_ch2 : 12;  /*!< [11..0] nan                                                               */
        uint32_t            : 4;
    	uint32_t cfg_sar_adc_out_ch3 : 12;  /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } CALIDATA1_b;
  } ;

  union {
    uint32_t CALIDATA2;                   /*!< (@ 0x00000038) CALIDATA2                                                  */

    struct {
    	uint32_t cfg_sar_adc_out_ch4 : 12;  /*!< [11..0] nan                                                               */
        uint32_t            : 4;
    	uint32_t cfg_sar_adc_out_ch5 : 12;  /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } CALIDATA2_b;
  } ;

  union {
    uint32_t CALIDATA3;                   /*!< (@ 0x0000003C) CALIDATA3                                                  */

    struct {
      	uint32_t cfg_sar_adc_out_ch6 : 12;  /*!< [11..0] nan                                                               */
        uint32_t            : 4;
      	uint32_t cfg_sar_adc_out_ch7 : 12;  /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } CALIDATA3_b;
  } ;

  union {
    uint32_t CALIDATA4;                   /*!< (@ 0x00000040) CALIDATA4                                                  */

    struct {
      	uint32_t cfg_sar_adc_out_ch8 : 12;  /*!< [11..0] nan                                                               */
        uint32_t            : 4;
      	uint32_t cfg_sar_adc_out_ch9 : 12;  /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } CALIDATA4_b;
  } ;

  union {
    uint32_t CALIDATA5;                   /*!< (@ 0x00000044) CALIDATA5                                                  */

    struct {
      	uint32_t cfg_sar_adc_out_ch10 : 12; /*!< [11..0] nan                                                               */
        uint32_t            : 4;
      	uint32_t cfg_sar_adc_out_ch11 : 12; /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } CALIDATA5_b;
  } ;

  union {
    uint32_t CALIDATA6;                   /*!< (@ 0x00000048) CALIDATA6                                                  */

    struct {
      uint32_t cfg_sar_adc_out_ch12 : 12; /*!< [11..0] nan                                                               */
            uint32_t            : 4;
      uint32_t cfg_sar_adc_out_ch13 : 12; /*!< [27..16] nan                                                              */
            uint32_t            : 4;
    } CALIDATA6_b;
  } ;

  union {
    uint32_t CALIDATA7;                   /*!< (@ 0x0000004C) CALIDATA7                                                  */

    struct {
      uint32_t cfg_sar_adc_out_ch14 : 12; /*!< [11..0] nan                                                               */
            uint32_t            : 4;
      uint32_t cfg_sar_adc_out_ch15 : 12; /*!< [27..16] nan                                                              */
            uint32_t            : 4;
    } CALIDATA7_b;
  } ;

  union {
    uint32_t LPFCTRL;                     /*!< (@ 0x00000050) LPFCTRL                                                    */

    struct {
      uint32_t cfg_sar_ch_lpf_bypass : 16;/*!< [15..0] nan                                                               */
            uint32_t            : 16;
    } LPFCTRL_b;
  } ;

  union {
    uint32_t LPFDATA0;                    /*!< (@ 0x00000054) LPFDATA0                                                   */

    struct {
      	uint32_t sar_adc_data_ch0 : 12;     /*!< [11..0] nan                                                               */
        uint32_t            : 4;
      	uint32_t sar_adc_data_ch1 : 12;     /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } LPFDATA0_b;
  } ;

  union {
    uint32_t LPFDATA1;                    /*!< (@ 0x00000058) LPFDATA1                                                   */

    struct {
    	uint32_t sar_adc_data_ch2 : 12;     /*!< [11..0] nan                                                               */
        uint32_t            : 4;
      	uint32_t sar_adc_data_ch3 : 12;     /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } LPFDATA1_b;
  } ;

  union {
    uint32_t LPFDATA2;                    /*!< (@ 0x0000005C) LPFDATA2                                                   */

    struct {
      	uint32_t sar_adc_data_ch4 : 12;     /*!< [11..0] nan                                                               */
        uint32_t            : 4;
      	uint32_t sar_adc_data_ch5 : 12;     /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } LPFDATA2_b;
  } ;

  union {
    uint32_t LPFDATA3;                    /*!< (@ 0x00000060) LPFDATA3                                                   */

    struct {
      	uint32_t sar_adc_data_ch6 : 12;     /*!< [11..0] nan                                                               */
        uint32_t            : 4;
      	uint32_t sar_adc_data_ch7 : 12;     /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } LPFDATA3_b;
  } ;

  union {
    uint32_t LPFDATA4;                    /*!< (@ 0x00000064) LPFDATA4                                                   */

    struct {
      	uint32_t sar_adc_data_ch8 : 12;     /*!< [11..0] nan                                                               */
        uint32_t            : 4;
      	uint32_t sar_adc_data_ch9 : 12;     /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } LPFDATA4_b;
  } ;

  union {
    uint32_t LPFDATA5;                    /*!< (@ 0x00000068) LPFDATA5                                                   */

    struct {
      	uint32_t sar_adc_data_ch10 : 12;    /*!< [11..0] nan                                                               */
        uint32_t            : 4;
      	uint32_t sar_adc_data_ch11 : 12;    /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } LPFDATA5_b;
  } ;

  union {
    uint32_t LPFDATA6;                    /*!< (@ 0x0000006C) LPFDATA6                                                   */

    struct {
      	uint32_t sar_adc_data_ch12 : 12;    /*!< [11..0] nan                                                               */
        uint32_t            : 4;
      	uint32_t sar_adc_data_ch13 : 12;    /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } LPFDATA6_b;
  } ;

  union {
    uint32_t LPFDATA7;                    /*!< (@ 0x00000070) LPFDATA7                                                   */

    struct {
      	uint32_t sar_adc_data_ch14 : 12;    /*!< [11..0] nan                                                               */
        uint32_t            : 4;
      	uint32_t sar_adc_data_ch15 : 12;    /*!< [27..16] nan                                                              */
        uint32_t            : 4;
    } LPFDATA7_b;
  } ;

  union {
    uint32_t EDUNCYDATA0;                 /*!< (@ 0x00000074) EDUNCYDATA0                                                */

    struct {
      	uint32_t sar_adc_data_reduncy_ch0 : 15;/*!< [14..0] nan                                                            */
        uint32_t            : 1;
      	uint32_t sar_adc_data_reduncy_ch1 : 15;/*!< [30..16] nan                                                           */
        uint32_t            : 1;
    } EDUNCYDATA0_b;
  } ;

  union {
    uint32_t EDUNCYDATA1;                 /*!< (@ 0x00000078) EDUNCYDATA1                                                */

    struct {
      	uint32_t sar_adc_data_reduncy_ch2 : 15;/*!< [14..0] nan                                                            */
        uint32_t            : 1;
      	uint32_t sar_adc_data_reduncy_ch3 : 15;/*!< [30..16] nan                                                           */
        uint32_t            : 1;
    } EDUNCYDATA1_b;
  } ;

  union {
    uint32_t EDUNCYDATA2;                 /*!< (@ 0x0000007C) EDUNCYDATA2                                                */

    struct {
      	uint32_t sar_adc_data_reduncy_ch4 : 15;/*!< [14..0] nan                                                            */
        uint32_t            : 1;
      	uint32_t sar_adc_data_reduncy_ch5 : 15;/*!< [30..16] nan                                                           */
        uint32_t            : 1;
    } EDUNCYDATA2_b;
  } ;

  union {
    uint32_t EDUNCYDATA3;                 /*!< (@ 0x00000080) EDUNCYDATA3                                                */

    struct {
      	uint32_t sar_adc_data_reduncy_ch6 : 15;/*!< [14..0] nan                                                            */
        uint32_t            : 1;
      	uint32_t sar_adc_data_reduncy_ch7 : 15;/*!< [30..16] nan                                                           */
        uint32_t            : 1;
    } EDUNCYDATA3_b;
  } ;

  union {
    uint32_t EDUNCYDATA4;                 /*!< (@ 0x00000084) EDUNCYDATA4                                                */

    struct {
      	uint32_t sar_adc_data_reduncy_ch8 : 15;/*!< [14..0] nan                                                            */
        uint32_t            : 1;
      	uint32_t sar_adc_data_reduncy_ch9 : 15;/*!< [30..16] nan                                                           */
        uint32_t            : 1;
    } EDUNCYDATA4_b;
  } ;

  union {
    uint32_t EDUNCYDATA5;                 /*!< (@ 0x00000088) EDUNCYDATA5                                                */

    struct {
      	uint32_t sar_adc_data_reduncy_ch10 : 15;/*!< [14..0] nan                                                           */
        uint32_t            : 1;
      	uint32_t sar_adc_data_reduncy_ch11 : 15;/*!< [30..16] nan                                                          */
        uint32_t            : 1;
    } EDUNCYDATA5_b;
  } ;

  union {
    uint32_t EDUNCYDATA6;                 /*!< (@ 0x0000008C) EDUNCYDATA6                                                */

    struct {
      	uint32_t sar_adc_data_reduncy_ch12 : 15;/*!< [14..0] nan                                                           */
        uint32_t            : 1;
      	uint32_t sar_adc_data_reduncy_ch13 : 15;/*!< [30..16] nan                                                          */
        uint32_t            : 1;
    } EDUNCYDATA6_b;
  } ;

  union {
    uint32_t EDUNCYDATA7;                 /*!< (@ 0x00000090) EDUNCYDATA7                                                */

    struct {
      	uint32_t sar_adc_data_reduncy_ch14 : 15;/*!< [14..0] nan                                                           */
        uint32_t            : 1;
      	uint32_t sar_adc_data_reduncy_ch15 : 15;/*!< [30..16] nan                                                          */
        uint32_t            : 1;
    } EDUNCYDATA7_b;
  } ;

  union {
    uint32_t COMP0;                       /*!< (@ 0x00000094) COMP0                                                      */

    struct {
		uint32_t acpr0_enable : 1;          /*!< [0..0] nan                                                                */
		uint32_t acpr0_debounce_sel : 2;    /*!< [2..1] nan                                                                */
		uint32_t acpr0_active_output_polarity : 1;/*!< [3..3] nan                                                          */
		uint32_t acpr0_threshold : 12;      /*!< [15..4] nan                                                               */
		uint32_t acpr0_ch_sel : 3;          /*!< [18..16] nan                                                              */
		uint32_t acpr0_interrupt_enable : 1;/*!< [19..19] nan                                                              */
        uint32_t            : 12;
    } COMP0_b;
  } ;

  union {
    uint32_t COMP1;                       /*!< (@ 0x00000098) COMP1                                                      */

    struct {
		uint32_t acpr1_enable : 1;          /*!< [0..0] nan                                                                */
		uint32_t acpr1_debounce_sel : 2;    /*!< [2..1] nan                                                                */
		uint32_t acpr1_active_output_polarity : 1;/*!< [3..3] nan                                                          */
		uint32_t acpr1_threshold : 12;      /*!< [15..4] nan                                                               */
		uint32_t acpr1_ch_sel : 3;          /*!< [18..16] nan                                                              */
		uint32_t acpr1_interrupt_enable : 1;/*!< [19..19] nan                                                              */
        uint32_t            : 12;
    } COMP1_b;
  } ;

  union {
    uint32_t COMPSTS;                     /*!< (@ 0x0000009C) COMPSTS                                                    */

    struct {
      	uint32_t acpr0_active_output_status : 1;/*!< [0..0] nan                                                            */
    	uint32_t acpr1_active_output_status : 1;/*!< [1..1] nan                                                            */
        uint32_t            : 30;
    } COMPSTS_b;
  } ;

  union {
    uint32_t COMPINT;                     /*!< (@ 0x000000A0) COMPINT                                                    */

    struct {
		uint32_t acpr0_interrupt : 1;       /*!< [0..0] nan                                                                */
		uint32_t acpr1_interrupt : 1;       /*!< [1..1] nan                                                                */
		uint32_t acpr0_over_threshold : 1;  /*!< [2..2] nan                                                                */
		uint32_t acpr1_over_threshold : 1;  /*!< [3..3] nan                                                                */
    	uint32_t            : 28;
    } COMPINT_b;
  } ;

  union {
    uint32_t DUMMY;                       /*!< (@ 0x000000A4) DUMMY                                                      */

    struct {
		uint32_t dummy2     : 16;           /*!< [15..0] nan                                                               */
		uint32_t dummy1     : 16;           /*!< [31..16] nan                                                              */
    } DUMMY_b;
  } ;
};                                     /*!< Size = 168 (0xa8)                                                         */

#endif /* ZEPHYR_SOC_REALTEK_RTS5918_REG_ADC_H */
