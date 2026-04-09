
#ifndef __IM                                    /*!< Fallback for older CMSIS versions                                         */
  #define __IM   __I
#endif
#ifndef __OM                                    /*!< Fallback for older CMSIS versions                                         */
  #define __OM   __O
#endif
#ifndef __IOM                                   /*!< Fallback for older CMSIS versions                                         */
  #define __IOM  __IO
#endif


/* =========================================================================================================================== */
/* ================                                          SHA2DMA                                          ================ */
/* =========================================================================================================================== */


/**
  * @brief Lalu SHA2 DMA Controller (SHA2DMA)
  */

typedef struct {                                /*!< (@ 0x40041000) SHA2DMA Structure                                          */
  __IOM uint32_t  SAR;                          /*!< (@ 0x00000000) SOURCE ADDRESS REGISTER                                    */
  __IM  uint32_t  RESERVED;
  __IOM uint32_t  DAR;                          /*!< (@ 0x00000008) DESTINATION ADDRESS REGISTER                               */
  __IM  uint32_t  RESERVED1[3];
  
  union {
    __IOM uint32_t CTRLL;                       /*!< (@ 0x00000018) CONTROL REGISTER LOW                                       */
    
    struct {
      __IOM uint32_t INTEN      : 1;            /*!< [0..0] Interrupt Enable Bit                                               */
      __IOM uint32_t DSTTRWIDTH : 3;            /*!< [3..1] Destination Transfer Width                                         */
      __IOM uint32_t SRCTRWIDTH : 3;            /*!< [6..4] Source Transfer Width                                              */
      __IOM uint32_t DINC       : 2;            /*!< [8..7] Destination Address Increment                                      */
      __IOM uint32_t SINC       : 2;            /*!< [10..9] Source Address Increment                                          */
      __IOM uint32_t DESTMSIZE  : 3;            /*!< [13..11] Destination Burst Transaction Length                             */
      __IOM uint32_t SRCMSIZE   : 3;            /*!< [16..14] Source Burst Transaction Length                                  */
            uint32_t            : 15;
    } CTRLL_b;
  } ;
  
  union {
    __IOM uint32_t CTRLH;                       /*!< (@ 0x0000001C) CONTROL REGISTER HIGH                                      */
    
    struct {
      __IOM uint32_t BLOCKTS    : 12;           /*!< [11..0] Block Transfer Size                                               */
            uint32_t            : 20;
    } CTRLH_b;
  } ;
  __IM  uint32_t  RESERVED2[9];
  
  union {
    __IOM uint32_t CFG;                         /*!< (@ 0x00000044) CONFIGURATION REGISTER                                     */
    
    struct {
            uint32_t            : 7;
      __IOM uint32_t SRCPER     : 4;            /*!< [10..7] Assigns a Hardware Handshaking Interface to The Source
                                                     of Channel                                                                */
      __IOM uint32_t DSTPER     : 4;            /*!< [14..11] Assigns a Hardware Handshaking Interface to The Destination
                                                     of Channel                                                                */
      __IOM uint32_t EXTSRCPER1 : 1;            /*!< [15..15] The extened 4th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER1 : 1;            /*!< [16..16] The extened 4th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER2 : 1;            /*!< [17..17] The extened 5th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER2 : 1;            /*!< [18..18] The extened 5th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER3 : 1;            /*!< [19..19] The extened 6th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER3 : 1;            /*!< [20..20] The extened 6th bit of DSTPER                                    */
            uint32_t            : 11;
    } CFG_b;
  } ;
  __IM  uint32_t  RESERVED3[178];
  __IOM uint32_t  MSKTFR;                       /*!< (@ 0x00000310) MASK FOR INTTFR INTERRUPT REGISTER                         */
  __IM  uint32_t  RESERVED4[9];
  __IOM uint32_t  CLRTFR;                       /*!< (@ 0x00000338) CLEAR INTERRUPT STATUS FOR INTTFR INTERRUPT REGISTER       */
  __IM  uint32_t  RESERVED5[9];
  
  union {
    __IM  uint32_t INTSTS;                      /*!< (@ 0x00000360) COMBINED INTERRUPT SATAUS REGISTER                         */
    
    struct {
      __IM  uint32_t TFR        : 1;            /*!< [0..0] OR of The Contents of StatusTfr Register                           */
      __IM  uint32_t BLOCK      : 1;            /*!< [1..1] OR of The Contents of StatusBlock Register                         */
      __IM  uint32_t SRCT       : 1;            /*!< [2..2] OR of The Contents of StatusSrcTran Register                       */
      __IM  uint32_t DSTT       : 1;            /*!< [3..3] OR of The Contents of StatusDst Register                           */
      __IM  uint32_t ERR        : 1;            /*!< [4..4] OR of The Contents of StatusErr Register                           */
            uint32_t            : 27;
    } INTSTS_b;
  } ;
  __IM  uint32_t  RESERVED6[13];
  
  union {
    __IOM uint32_t CONF;                        /*!< (@ 0x00000398) CONFIGURATION REGISTER                                     */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable DMA Controller                                              */
            uint32_t            : 31;
    } CONF_b;
  } ;
  __IM  uint32_t  RESERVED7;
  
  union {
    __IOM uint32_t CHEN;                        /*!< (@ 0x000003A0) CHANNEL ENABLE REGISTER                                    */
    
    struct {
      __IOM uint32_t CHEN       : 1;            /*!< [0..0] Enables DMA Channel                                                */
            uint32_t            : 7;
      __IOM uint32_t CHENWE     : 1;            /*!< [8..8] Enables Channel Write Enable                                       */
            uint32_t            : 23;
    } CHEN_b;
  } ;
} SHA2DMA_Type;                                 /*!< Size = 932 (0x3a4)                                                        */



/* =========================================================================================================================== */
/* ================                                           SHA3                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief Lalu SHA3 Engine (SHA3)
  */

typedef struct {                                /*!< (@ 0x40042000) SHA3 Structure                                             */
  __IOM uint32_t  DINADDR;                      /*!< (@ 0x00000000) INPUT DATA ADDRESS REGISTER                                */
  __IOM uint32_t  DINLEN;                       /*!< (@ 0x00000004) DATA LENGTH REGISTER                                       */
  
  union {
    __IOM uint32_t SPEC;                        /*!< (@ 0x00000008) SPECIFICATION REGISTER                                     */
    
    struct {
      __IOM uint32_t PREFIX     : 1;            /*!< [0..0] SHA3 Or SHAKE Selection                                            */
            uint32_t            : 7;
      __IOM uint32_t SUFFIX     : 3;            /*!< [10..8] Suffix Of SHA3 Specification                                      */
            uint32_t            : 21;
    } SPEC_b;
  } ;
  __IOM uint32_t  INTDIS;                       /*!< (@ 0x0000000C) INTERRUPT DISABLE REGISTER                                 */
  __IOM uint32_t  LPMDDIS;                      /*!< (@ 0x00000010) LOW POWER MODE DISABLE REGISTER                            */
  
  union {
    __IOM uint32_t ENABLE;                      /*!< (@ 0x00000014) ENABLE REGISTER                                            */
    
    struct {
      __IOM uint32_t EN         : 2;            /*!< [1..0] Enable SHA3 Engine To Execute Command                              */
            uint32_t            : 30;
    } ENABLE_b;
  } ;
  __IOM uint32_t  CLR;                          /*!< (@ 0x00000018) CLEAR HASH VALUE AND INPUT BUFFER REGISTER                 */
  __IM  uint32_t  RESERVED;
  __IOM uint32_t  SQUEEZE;                      /*!< (@ 0x00000020) SQUEEZE REGISTER                                           */
  __IM  uint32_t  RESERVED1[2];
  
  union {
    __IOM uint32_t ERRSTS;                      /*!< (@ 0x0000002C) ERROR STATUS REGISTER                                      */
    
    struct {
      __IOM uint32_t PRECMD     : 2;            /*!< [1..0] Latest Command                                                     */
            uint32_t            : 6;
      __IOM uint32_t ERR        : 3;            /*!< [10..8] Error Status                                                      */
            uint32_t            : 21;
    } ERRSTS_b;
  } ;
  __IOM uint32_t  ADDRERR;                      /*!< (@ 0x00000030) READ ERROR ADDRESS REGISTER                                */
  
  union {
    __IOM uint32_t HASH[50];                    /*!< (@ 0x00000034) HASH VALUE REGISTER                                        */
    
    struct {
      __IOM uint32_t BYTE0      : 8;            /*!< [7..0] Hash value, byte0                                                  */
      __IOM uint32_t BYTE1      : 8;            /*!< [15..8] Hash value, byte1                                                 */
      __IOM uint32_t BYTE2      : 8;            /*!< [23..16] Hash value, byte2                                                */
      __IOM uint32_t BYTE3      : 8;            /*!< [31..24] Hash value, byte3                                                */
    } HASH_b[50];
  } ;
} SHA3_Type;                                    /*!< Size = 252 (0xfc)                                                         */



/* =========================================================================================================================== */
/* ================                                            AES                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief Lalu AES Engine (AES)
  */

typedef struct {                                /*!< (@ 0x40043000) AES Structure                                              */
  __IOM uint32_t  DATAIN3;                      /*!< (@ 0x00000000) DATA IN REGISTER #3                                        */
  __IOM uint32_t  DATAIN2;                      /*!< (@ 0x00000004) DATA IN REGISTER #2                                        */
  __IOM uint32_t  DATAIN1;                      /*!< (@ 0x00000008) DATA IN REGISTER #1                                        */
  __IOM uint32_t  DATAIN0;                      /*!< (@ 0x0000000C) DATA IN REGISTER #0                                        */
  __IOM uint32_t  DATAOUT3;                     /*!< (@ 0x00000010) DATA OUT REGISTER #3                                       */
  __IOM uint32_t  DATAOUT2;                     /*!< (@ 0x00000014) DATA OUT REGISTER #2                                       */
  __IOM uint32_t  DATAOUT1;                     /*!< (@ 0x00000018) DATA OUT REGISTER #1                                       */
  __IOM uint32_t  DATAOUT0;                     /*!< (@ 0x0000001C) DATA OUT REGISTER #0                                       */
  __IOM uint32_t  KEYDEAOUT7;                   /*!< (@ 0x00000020) DECRYPTION KEY REGISTER #7                                 */
  __IOM uint32_t  KEYDEAOUT6;                   /*!< (@ 0x00000024) DECRYPTION KEY REGISTER #6                                 */
  __IOM uint32_t  KEYDEAOUT5;                   /*!< (@ 0x00000028) DECRYPTION KEY REGISTER #5                                 */
  __IOM uint32_t  KEYDEAOUT4;                   /*!< (@ 0x0000002C) DECRYPTION KEY REGISTER #4                                 */
  __IOM uint32_t  KEYDEAOUT3;                   /*!< (@ 0x00000030) DECRYPTION KEY REGISTER #3                                 */
  __IOM uint32_t  KEYDEAOUT2;                   /*!< (@ 0x00000034) DECRYPTION KEY REGISTER #2                                 */
  __IOM uint32_t  KEYDEAOUT1;                   /*!< (@ 0x00000038) DECRYPTION KEY REGISTER #1                                 */
  __IOM uint32_t  KEYDEAOUT0;                   /*!< (@ 0x0000003C) DECRYPTION KEY REGISTER #0                                 */
  __IOM uint32_t  KEY7;                         /*!< (@ 0x00000040) KEY REGISTER #7                                            */
  __IOM uint32_t  KEY6;                         /*!< (@ 0x00000044) KEY REGISTER #6                                            */
  __IOM uint32_t  KEY5;                         /*!< (@ 0x00000048) KEY REGISTER #5                                            */
  __IOM uint32_t  KEY4;                         /*!< (@ 0x0000004C) KEY REGISTER #4                                            */
  __IOM uint32_t  KEY3;                         /*!< (@ 0x00000050) KEY REGISTER #3                                            */
  __IOM uint32_t  KEY2;                         /*!< (@ 0x00000054) KEY REGISTER #2                                            */
  __IOM uint32_t  KEY1;                         /*!< (@ 0x00000058) KEY REGISTER #1                                            */
  __IOM uint32_t  KEY0;                         /*!< (@ 0x0000005C) KEY REGISTER #0                                            */
  __IOM uint32_t  IV3;                          /*!< (@ 0x00000060) IV REGISTER #3                                             */
  __IOM uint32_t  IV2;                          /*!< (@ 0x00000064) IV REGISTER #2                                             */
  __IOM uint32_t  IV1;                          /*!< (@ 0x00000068) IV REGISTER #1                                             */
  __IOM uint32_t  IV0;                          /*!< (@ 0x0000006C) IV REGISTER #0                                             */
  
  union {
    __IOM uint32_t CONF;                        /*!< (@ 0x00000070) CONFIGURATION REGISTER                                     */
    
    struct {
      __IOM uint32_t MODE       : 1;            /*!< [0..0] Engnie Mode Selection                                              */
      __IOM uint32_t CIPHERMD   : 4;            /*!< [4..1] Cipher Mode Selection                                              */
      __IOM uint32_t KEYSIZE    : 2;            /*!< [6..5] Key Size Selection                                                 */
      __IOM uint32_t DECRYPT    : 1;            /*!< [7..7] Decrypt/Encrypt Selection                                          */
      __IOM uint32_t FIRSTBLK   : 1;            /*!< [8..8] First Block                                                        */
      __IOM uint32_t DEKEYGEN   : 1;            /*!< [9..9] Generate A Key When Decrypting                                     */
            uint32_t            : 1;
      __IOM uint32_t SCANMSKEN  : 1;            /*!< [11..11] SCA Mask Enable                                                  */
            uint32_t            : 4;
      __IOM uint32_t GCMLAST    : 1;            /*!< [16..16] Indicated The Last Block                                         */
      __IOM uint32_t CMACLAST   : 1;            /*!< [17..17] Indicated The Last Block                                         */
            uint32_t            : 6;
      __IOM uint32_t SOFTRST    : 1;            /*!< [24..24] Software Reset                                                   */
            uint32_t            : 7;
    } CONF_b;
  } ;
  __IOM uint32_t  LEN;                          /*!< (@ 0x00000074) INPUT LENGTH REGISTER                                      */
  __IOM uint32_t  GMACLEN1;                     /*!< (@ 0x00000078) AAD LENGTH REGISTER #1                                     */
  __IOM uint32_t  GMACLEN0;                     /*!< (@ 0x0000007C) AAD LENGTH REGISTER #0                                     */
  __IOM uint32_t  TAG3;                         /*!< (@ 0x00000080) TAG REGISTER #3                                            */
  __IOM uint32_t  TAG2;                         /*!< (@ 0x00000084) TAG REGISTER #2                                            */
  __IOM uint32_t  TAG1;                         /*!< (@ 0x00000088) TAG REGISTER #1                                            */
  __IOM uint32_t  TAG0;                         /*!< (@ 0x0000008C) TAG REGISTER #0                                            */
  __IOM uint32_t  GHASHKEY3;                    /*!< (@ 0x00000090) GHASH KEY REGISTER #3                                      */
  __IOM uint32_t  GHASHKEY2;                    /*!< (@ 0x00000094) GHASH KEY REGISTER #2                                      */
  __IOM uint32_t  GHASHKEY1;                    /*!< (@ 0x00000098) GHASH KEY REGISTER #1                                      */
  __IOM uint32_t  GHASHKEY0;                    /*!< (@ 0x0000009C) GHASH KEY REGISTER #0                                      */
  __IM  uint32_t  RESERVED[912];
  
  union {
    __IOM uint32_t INTSTS;                      /*!< (@ 0x00000EE0) INTERRUPT STATUS REGISTER                                  */
    
    struct {
      __IOM uint32_t CH0INTSTS  : 1;            /*!< [0..0] Channel Interrupt Status                                           */
      __IOM uint32_t CH1INTSTS  : 1;            /*!< [1..1] Channel Interrupt Status                                           */
      __IOM uint32_t CH2INTSTS  : 1;            /*!< [2..2] Channel Interrupt Status                                           */
      __IOM uint32_t CH3INTSTS  : 1;            /*!< [3..3] Channel Interrupt Status                                           */
      __IOM uint32_t CH4INTSTS  : 1;            /*!< [4..4] Channel Interrupt Status                                           */
      __IOM uint32_t CH5INTSTS  : 1;            /*!< [5..5] Channel Interrupt Status                                           */
      __IOM uint32_t CH6INTSTS  : 1;            /*!< [6..6] Channel Interrupt Status                                           */
      __IOM uint32_t CH7INTSTS  : 1;            /*!< [7..7] Channel Interrupt Status                                           */
      __IOM uint32_t CH8INTSTS  : 1;            /*!< [8..8] Channel Interrupt Status                                           */
      __IOM uint32_t CH9INTSTS  : 1;            /*!< [9..9] Channel Interrupt Status                                           */
      __IOM uint32_t CH10INTSTS : 1;            /*!< [10..10] Channel Interrupt Status                                         */
      __IOM uint32_t CH11INTSTS : 1;            /*!< [11..11] Channel Interrupt Status                                         */
      __IOM uint32_t CH12INTSTS : 1;            /*!< [12..12] Channel Interrupt Status                                         */
      __IOM uint32_t CH13INTSTS : 1;            /*!< [13..13] Channel Interrupt Status                                         */
      __IOM uint32_t CH14INTSTS : 1;            /*!< [14..14] Channel Interrupt Status                                         */
      __IOM uint32_t CH15INTSTS : 1;            /*!< [15..15] Channel Interrupt Status                                         */
            uint32_t            : 16;
    } INTSTS_b;
  } ;
  __IM  uint32_t  RESERVED1[3];
  
  union {
    __IM  uint32_t INTRAW;                      /*!< (@ 0x00000EF0) RAW INTERRUPT STATUS REGISTER                              */
    
    struct {
      __IM  uint32_t SLVDONE    : 1;            /*!< [0..0] Slave Mode Completes AES                                           */
      __IM  uint32_t ERRBLKSIZE : 1;            /*!< [1..1] Input size is a multiple of 128                                    */
      __IM  uint32_t ERRDECIPHER : 1;           /*!< [2..2] User Set a Non-Existent Cipher Mode                                */
      __IM  uint32_t ERRORENDE  : 1;            /*!< [3..3] User Uses Operation When Closing                                   */
            uint32_t            : 28;
    } INTRAW_b;
  } ;
  
  union {
    __IOM uint32_t INTMSK;                      /*!< (@ 0x00000EF4) ENABLE INTERRUPT REGISTER                                  */
    
    struct {
      __IOM uint32_t SLVDONE    : 1;            /*!< [0..0] Enable SLVDONE Interrupt                                           */
      __IOM uint32_t ERRBLKSIZE : 1;            /*!< [1..1] Enable ERRBLKSIZE Interrupt                                        */
      __IOM uint32_t ERRDECIPHER : 1;           /*!< [2..2] Enable ERRDECIPHER Interrupt                                       */
      __IOM uint32_t ERRORENDE  : 1;            /*!< [3..3] Enable ERRORENDE Interrupt                                         */
            uint32_t            : 28;
    } INTMSK_b;
  } ;
  
  union {
    __IOM uint32_t INTMSKSTS;                   /*!< (@ 0x00000EF8) MASKED INTERRUPT STATUS REGISTER                           */
    
    struct {
      __IOM uint32_t SLVDONE    : 1;            /*!< [0..0] Slave Mode Completes AES                                           */
      __IOM uint32_t ERRBLKSIZE : 1;            /*!< [1..1] Input size is a multiple of 128                                    */
      __IOM uint32_t ERRDECIPHER : 1;           /*!< [2..2] User Set a Non-Existent Cipher Mode                                */
      __IOM uint32_t ERRORENDE  : 1;            /*!< [3..3] User Uses Operation When Closing                                   */
            uint32_t            : 28;
    } INTMSKSTS_b;
  } ;
  
  union {
    __IOM uint32_t INTCLR;                      /*!< (@ 0x00000EFC) CLEAR INTERRUPT REGISTER                                   */
    
    struct {
      __IOM uint32_t SLVDONE    : 1;            /*!< [0..0] Clear SLVDONE Interrupt                                            */
      __IOM uint32_t ERRBLKSIZE : 1;            /*!< [1..1] Clear ERRBLKSIZE Interrupt                                         */
      __IOM uint32_t ERRDECIPHER : 1;           /*!< [2..2] Clear ERRDECIPHER Interrupt                                        */
      __IOM uint32_t ERRORENDE  : 1;            /*!< [3..3] Clear ERRORENDE Interrupt                                          */
            uint32_t            : 28;
    } INTCLR_b;
  } ;
} AES_Type;                                     /*!< Size = 3840 (0xf00)                                                       */



/* =========================================================================================================================== */
/* ================                                            DMA                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief DMA Controller (DMA)
  */

typedef struct {                                /*!< (@ 0x40002000) DMA Structure                                              */
  __IOM uint32_t  SAR0;                         /*!< (@ 0x00000000) SOURCE ADDRESS REGISTER                                    */
  __IM  uint32_t  RESERVED;
  __IOM uint32_t  DAR0;                         /*!< (@ 0x00000008) DESTINATION ADDRESS REGISTER                               */
  __IM  uint32_t  RESERVED1;
  
  union {
    __IOM uint32_t LLP0;                        /*!< (@ 0x00000010) LINKED LIST POINTER REGISTER                               */
    
    struct {
      __IOM uint32_t LMS        : 2;            /*!< [1..0] List Master Select                                                 */
      __IOM uint32_t LOC        : 30;           /*!< [31..2] Starting Address In Memory                                        */
    } LLP0_b;
  } ;
  __IM  uint32_t  RESERVED2;
  
  union {
    __IOM uint32_t CTRL0L;                      /*!< (@ 0x00000018) CONTROL REGISTER LOW                                       */
    
    struct {
      __IOM uint32_t INTEN      : 1;            /*!< [0..0] Interrupt Enable Bit                                               */
      __IOM uint32_t DSTTRWIDTH : 3;            /*!< [3..1] Destination Transfer Width                                         */
      __IOM uint32_t SRCTRWIDTH : 3;            /*!< [6..4] Source Transfer Width                                              */
      __IOM uint32_t DINC       : 2;            /*!< [8..7] Destination Address Increment                                      */
      __IOM uint32_t SINC       : 2;            /*!< [10..9] Source Address Increment                                          */
      __IOM uint32_t DESTMSIZE  : 3;            /*!< [13..11] Destination Burst Transaction Length                             */
      __IOM uint32_t SRCMSIZE   : 3;            /*!< [16..14] Source Burst Transaction Length                                  */
            uint32_t            : 3;
      __IOM uint32_t TTFC       : 3;            /*!< [22..20] Transfer Type and Flow Control                                   */
            uint32_t            : 9;
    } CTRL0L_b;
  } ;
  
  union {
    __IOM uint32_t CTRL0H;                      /*!< (@ 0x0000001C) CONTROL REGISTER HIGH                                      */
    
    struct {
      __IOM uint32_t BLOCKTS    : 32;           /*!< [31..0] Block Transfer Size                                               */
    } CTRL0H_b;
  } ;
  __IM  uint32_t  RESERVED3[8];
  
  union {
    __IOM uint32_t CFG0L;                       /*!< (@ 0x00000040) CONFIGURATION REGISTER LOW                                 */
    
    struct {
      __IM  uint32_t INACTIVE   : 1;            /*!< [0..0] Indicates if the channel is inactive                               */
      __IM  uint32_t SRCPCTLOVER : 1;           /*!< [1..1] DMA Source bus protocol finish, indicating the current
                                                     read command has finished                                                 */
      __IM  uint32_t DSTPCTLOVER : 1;           /*!< [2..2] DMA destination bus protocol finish, indicating the current
                                                     read command has finished                                                 */
            uint32_t            : 1;
      __IOM uint32_t CHPRIOR    : 4;            /*!< [7..4] Channel Priority                                                   */
      __IOM uint32_t CHSUSP     : 1;            /*!< [8..8] Channel Suspend                                                    */
      __IM  uint32_t FIFOEMPTY  : 1;            /*!< [9..9] Indicates if there is data left in the channel FIFO                */
      __IOM uint32_t HSSELDST   : 1;            /*!< [10..10] Destination Software or Hardware Handshaking Select              */
      __IOM uint32_t HSSELSRC   : 1;            /*!< [11..11] Source Software or Hardware Handshaking Select                   */
            uint32_t            : 6;
      __IOM uint32_t DSTHSPOL   : 1;            /*!< [18..18] Destination Handshaking Interface Polarity                       */
      __IOM uint32_t SRCHSPOL   : 1;            /*!< [19..19] Source Handshaking Interface Polarity                            */
            uint32_t            : 12;
    } CFG0L_b;
  } ;
  
  union {
    __IOM uint32_t CFG0H;                       /*!< (@ 0x00000044) CONFIGURATION REGISTER HIGH                                */
    
    struct {
            uint32_t            : 7;
      __IOM uint32_t SRCPER     : 4;            /*!< [10..7] Assigns a Hardware Handshaking Interface to The Source
                                                     of Channel                                                                */
      __IOM uint32_t DSTPER     : 4;            /*!< [14..11] Assigns a Hardware Handshaking Interface to The Destination
                                                     of Channel                                                                */
      __IOM uint32_t EXTSRCPER1 : 1;            /*!< [15..15] The extened 4th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER1 : 1;            /*!< [16..16] The extened 4th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER2 : 1;            /*!< [17..17] The extened 5th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER2 : 1;            /*!< [18..18] The extened 5th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER3 : 1;            /*!< [19..19] The extened 6th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER3 : 1;            /*!< [20..20] The extened 6th bit of DSTPER                                    */
            uint32_t            : 11;
    } CFG0H_b;
  } ;
  __IM  uint32_t  RESERVED4[4];
  __IOM uint32_t  SAR1;                         /*!< (@ 0x00000058) SOURCE ADDRESS REGISTER                                    */
  __IM  uint32_t  RESERVED5;
  __IOM uint32_t  DAR1;                         /*!< (@ 0x00000060) DESTINATION ADDRESS REGISTER                               */
  __IM  uint32_t  RESERVED6;
  
  union {
    __IOM uint32_t LLP1;                        /*!< (@ 0x00000068) LINKED LIST POINTER REGISTER                               */
    
    struct {
      __IOM uint32_t LMS        : 2;            /*!< [1..0] List Master Select                                                 */
      __IOM uint32_t LOC        : 30;           /*!< [31..2] Starting Address In Memory                                        */
    } LLP1_b;
  } ;
  __IM  uint32_t  RESERVED7;
  
  union {
    __IOM uint32_t CTRL1L;                      /*!< (@ 0x00000070) CONTROL REGISTER LOW                                       */
    
    struct {
      __IOM uint32_t INTEN      : 1;            /*!< [0..0] Interrupt Enable Bit                                               */
      __IOM uint32_t DSTTRWIDTH : 3;            /*!< [3..1] Destination Transfer Width                                         */
      __IOM uint32_t SRCTRWIDTH : 3;            /*!< [6..4] Source Transfer Width                                              */
      __IOM uint32_t DINC       : 2;            /*!< [8..7] Destination Address Increment                                      */
      __IOM uint32_t SINC       : 2;            /*!< [10..9] Source Address Increment                                          */
      __IOM uint32_t DESTMSIZE  : 3;            /*!< [13..11] Destination Burst Transaction Length                             */
      __IOM uint32_t SRCMSIZE   : 3;            /*!< [16..14] Source Burst Transaction Length                                  */
            uint32_t            : 3;
      __IOM uint32_t TTFC       : 3;            /*!< [22..20] Transfer Type and Flow Control                                   */
            uint32_t            : 9;
    } CTRL1L_b;
  } ;
  
  union {
    __IOM uint32_t CTRL1H;                      /*!< (@ 0x00000074) CONTROL REGISTER HIGH                                      */
    
    struct {
      __IOM uint32_t BLOCKTS    : 32;           /*!< [31..0] Block Transfer Size                                               */
    } CTRL1H_b;
  } ;
  __IM  uint32_t  RESERVED8[8];
  
  union {
    __IOM uint32_t CFG1L;                       /*!< (@ 0x00000098) CONFIGURATION REGISTER LOW                                 */
    
    struct {
      __IM  uint32_t INACTIVE   : 1;            /*!< [0..0] Indicates if the channel is inactive                               */
      __IM  uint32_t SRCPCTLOVER : 1;           /*!< [1..1] DMA Source bus protocol finish, indicating the current
                                                     read command has finished                                                 */
      __IM  uint32_t DSTPCTLOVER : 1;           /*!< [2..2] DMA destination bus protocol finish, indicating the current
                                                     read command has finished                                                 */
            uint32_t            : 1;
      __IOM uint32_t CHPRIOR    : 4;            /*!< [7..4] Channel Priority                                                   */
      __IOM uint32_t CHSUSP     : 1;            /*!< [8..8] Channel Suspend                                                    */
      __IM  uint32_t FIFOEMPTY  : 1;            /*!< [9..9] Indicates if there is data left in the channel FIFO                */
      __IOM uint32_t HSSELDST   : 1;            /*!< [10..10] Destination Software or Hardware Handshaking Select              */
      __IOM uint32_t HSSELSRC   : 1;            /*!< [11..11] Source Software or Hardware Handshaking Select                   */
            uint32_t            : 6;
      __IOM uint32_t DSTHSPOL   : 1;            /*!< [18..18] Destination Handshaking Interface Polarity                       */
      __IOM uint32_t SRCHSPOL   : 1;            /*!< [19..19] Source Handshaking Interface Polarity                            */
            uint32_t            : 12;
    } CFG1L_b;
  } ;
  
  union {
    __IOM uint32_t CFG1H;                       /*!< (@ 0x0000009C) CONFIGURATION REGISTER HIGH                                */
    
    struct {
            uint32_t            : 7;
      __IOM uint32_t SRCPER     : 4;            /*!< [10..7] Assigns a Hardware Handshaking Interface to The Source
                                                     of Channel                                                                */
      __IOM uint32_t DSTPER     : 4;            /*!< [14..11] Assigns a Hardware Handshaking Interface to The Destination
                                                     of Channel                                                                */
      __IOM uint32_t EXTSRCPER1 : 1;            /*!< [15..15] The extened 4th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER1 : 1;            /*!< [16..16] The extened 4th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER2 : 1;            /*!< [17..17] The extened 5th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER2 : 1;            /*!< [18..18] The extened 5th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER3 : 1;            /*!< [19..19] The extened 6th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER3 : 1;            /*!< [20..20] The extened 6th bit of DSTPER                                    */
            uint32_t            : 11;
    } CFG1H_b;
  } ;
  __IM  uint32_t  RESERVED9[4];
  __IOM uint32_t  SAR2;                         /*!< (@ 0x000000B0) SOURCE ADDRESS REGISTER                                    */
  __IM  uint32_t  RESERVED10;
  __IOM uint32_t  DAR2;                         /*!< (@ 0x000000B8) DESTINATION ADDRESS REGISTER                               */
  __IM  uint32_t  RESERVED11;
  
  union {
    __IOM uint32_t LLP2;                        /*!< (@ 0x000000C0) LINKED LIST POINTER REGISTER                               */
    
    struct {
      __IOM uint32_t LMS        : 2;            /*!< [1..0] List Master Select                                                 */
      __IOM uint32_t LOC        : 30;           /*!< [31..2] Starting Address In Memory                                        */
    } LLP2_b;
  } ;
  __IM  uint32_t  RESERVED12;
  
  union {
    __IOM uint32_t CTRL2L;                      /*!< (@ 0x000000C8) CONTROL REGISTER LOW                                       */
    
    struct {
      __IOM uint32_t INTEN      : 1;            /*!< [0..0] Interrupt Enable Bit                                               */
      __IOM uint32_t DSTTRWIDTH : 3;            /*!< [3..1] Destination Transfer Width                                         */
      __IOM uint32_t SRCTRWIDTH : 3;            /*!< [6..4] Source Transfer Width                                              */
      __IOM uint32_t DINC       : 2;            /*!< [8..7] Destination Address Increment                                      */
      __IOM uint32_t SINC       : 2;            /*!< [10..9] Source Address Increment                                          */
      __IOM uint32_t DESTMSIZE  : 3;            /*!< [13..11] Destination Burst Transaction Length                             */
      __IOM uint32_t SRCMSIZE   : 3;            /*!< [16..14] Source Burst Transaction Length                                  */
            uint32_t            : 3;
      __IOM uint32_t TTFC       : 3;            /*!< [22..20] Transfer Type and Flow Control                                   */
            uint32_t            : 9;
    } CTRL2L_b;
  } ;
  
  union {
    __IOM uint32_t CTRL2H;                      /*!< (@ 0x000000CC) CONTROL REGISTER HIGH                                      */
    
    struct {
      __IOM uint32_t BLOCKTS    : 32;           /*!< [31..0] Block Transfer Size                                               */
    } CTRL2H_b;
  } ;
  __IM  uint32_t  RESERVED13[8];
  
  union {
    __IOM uint32_t CFG2L;                       /*!< (@ 0x000000F0) CONFIGURATION REGISTER LOW                                 */
    
    struct {
      __IM  uint32_t INACTIVE   : 1;            /*!< [0..0] Indicates if the channel is inactive                               */
      __IM  uint32_t SRCPCTLOVER : 1;           /*!< [1..1] DMA Source bus protocol finish, indicating the current
                                                     read command has finished                                                 */
      __IM  uint32_t DSTPCTLOVER : 1;           /*!< [2..2] DMA destination bus protocol finish, indicating the current
                                                     read command has finished                                                 */
            uint32_t            : 1;
      __IOM uint32_t CHPRIOR    : 4;            /*!< [7..4] Channel Priority                                                   */
      __IOM uint32_t CHSUSP     : 1;            /*!< [8..8] Channel Suspend                                                    */
      __IM  uint32_t FIFOEMPTY  : 1;            /*!< [9..9] Indicates if there is data left in the channel FIFO                */
      __IOM uint32_t HSSELDST   : 1;            /*!< [10..10] Destination Software or Hardware Handshaking Select              */
      __IOM uint32_t HSSELSRC   : 1;            /*!< [11..11] Source Software or Hardware Handshaking Select                   */
            uint32_t            : 6;
      __IOM uint32_t DSTHSPOL   : 1;            /*!< [18..18] Destination Handshaking Interface Polarity                       */
      __IOM uint32_t SRCHSPOL   : 1;            /*!< [19..19] Source Handshaking Interface Polarity                            */
            uint32_t            : 12;
    } CFG2L_b;
  } ;
  
  union {
    __IOM uint32_t CFG2H;                       /*!< (@ 0x000000F4) CONFIGURATION REGISTER HIGH                                */
    
    struct {
            uint32_t            : 7;
      __IOM uint32_t SRCPER     : 4;            /*!< [10..7] Assigns a Hardware Handshaking Interface to The Source
                                                     of Channel                                                                */
      __IOM uint32_t DSTPER     : 4;            /*!< [14..11] Assigns a Hardware Handshaking Interface to The Destination
                                                     of Channel                                                                */
      __IOM uint32_t EXTSRCPER1 : 1;            /*!< [15..15] The extened 4th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER1 : 1;            /*!< [16..16] The extened 4th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER2 : 1;            /*!< [17..17] The extened 5th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER2 : 1;            /*!< [18..18] The extened 5th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER3 : 1;            /*!< [19..19] The extened 6th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER3 : 1;            /*!< [20..20] The extened 6th bit of DSTPER                                    */
            uint32_t            : 11;
    } CFG2H_b;
  } ;
  __IM  uint32_t  RESERVED14[4];
  __IOM uint32_t  SAR3;                         /*!< (@ 0x00000108) SOURCE ADDRESS REGISTER                                    */
  __IM  uint32_t  RESERVED15;
  __IOM uint32_t  DAR3;                         /*!< (@ 0x00000110) DESTINATION ADDRESS REGISTER                               */
  __IM  uint32_t  RESERVED16;
  
  union {
    __IOM uint32_t LLP3;                        /*!< (@ 0x00000118) LINKED LIST POINTER REGISTER                               */
    
    struct {
      __IOM uint32_t LMS        : 2;            /*!< [1..0] List Master Select                                                 */
      __IOM uint32_t LOC        : 30;           /*!< [31..2] Starting Address In Memory                                        */
    } LLP3_b;
  } ;
  __IM  uint32_t  RESERVED17;
  
  union {
    __IOM uint32_t CTRL3L;                      /*!< (@ 0x00000120) CONTROL REGISTER LOW                                       */
    
    struct {
      __IOM uint32_t INTEN      : 1;            /*!< [0..0] Interrupt Enable Bit                                               */
      __IOM uint32_t DSTTRWIDTH : 3;            /*!< [3..1] Destination Transfer Width                                         */
      __IOM uint32_t SRCTRWIDTH : 3;            /*!< [6..4] Source Transfer Width                                              */
      __IOM uint32_t DINC       : 2;            /*!< [8..7] Destination Address Increment                                      */
      __IOM uint32_t SINC       : 2;            /*!< [10..9] Source Address Increment                                          */
      __IOM uint32_t DESTMSIZE  : 3;            /*!< [13..11] Destination Burst Transaction Length                             */
      __IOM uint32_t SRCMSIZE   : 3;            /*!< [16..14] Source Burst Transaction Length                                  */
            uint32_t            : 3;
      __IOM uint32_t TTFC       : 3;            /*!< [22..20] Transfer Type and Flow Control                                   */
            uint32_t            : 9;
    } CTRL3L_b;
  } ;
  
  union {
    __IOM uint32_t CTRL3H;                      /*!< (@ 0x00000124) CONTROL REGISTER HIGH                                      */
    
    struct {
      __IOM uint32_t BLOCKTS    : 32;           /*!< [31..0] Block Transfer Size                                               */
    } CTRL3H_b;
  } ;
  __IM  uint32_t  RESERVED18[8];
  
  union {
    __IOM uint32_t CFG3L;                       /*!< (@ 0x00000148) CONFIGURATION REGISTER LOW                                 */
    
    struct {
      __IM  uint32_t INACTIVE   : 1;            /*!< [0..0] Indicates if the channel is inactive                               */
      __IM  uint32_t SRCPCTLOVER : 1;           /*!< [1..1] DMA Source bus protocol finish, indicating the current
                                                     read command has finished                                                 */
      __IM  uint32_t DSTPCTLOVER : 1;           /*!< [2..2] DMA destination bus protocol finish, indicating the current
                                                     read command has finished                                                 */
            uint32_t            : 1;
      __IOM uint32_t CHPRIOR    : 4;            /*!< [7..4] Channel Priority                                                   */
      __IOM uint32_t CHSUSP     : 1;            /*!< [8..8] Channel Suspend                                                    */
      __IM  uint32_t FIFOEMPTY  : 1;            /*!< [9..9] Indicates if there is data left in the channel FIFO                */
      __IOM uint32_t HSSELDST   : 1;            /*!< [10..10] Destination Software or Hardware Handshaking Select              */
      __IOM uint32_t HSSELSRC   : 1;            /*!< [11..11] Source Software or Hardware Handshaking Select                   */
            uint32_t            : 6;
      __IOM uint32_t DSTHSPOL   : 1;            /*!< [18..18] Destination Handshaking Interface Polarity                       */
      __IOM uint32_t SRCHSPOL   : 1;            /*!< [19..19] Source Handshaking Interface Polarity                            */
            uint32_t            : 12;
    } CFG3L_b;
  } ;
  
  union {
    __IOM uint32_t CFG3H;                       /*!< (@ 0x0000014C) CONFIGURATION REGISTER HIGH                                */
    
    struct {
            uint32_t            : 7;
      __IOM uint32_t SRCPER     : 4;            /*!< [10..7] Assigns a Hardware Handshaking Interface to The Source
                                                     of Channel                                                                */
      __IOM uint32_t DSTPER     : 4;            /*!< [14..11] Assigns a Hardware Handshaking Interface to The Destination
                                                     of Channel                                                                */
      __IOM uint32_t EXTSRCPER1 : 1;            /*!< [15..15] The extened 4th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER1 : 1;            /*!< [16..16] The extened 4th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER2 : 1;            /*!< [17..17] The extened 5th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER2 : 1;            /*!< [18..18] The extened 5th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER3 : 1;            /*!< [19..19] The extened 6th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER3 : 1;            /*!< [20..20] The extened 6th bit of DSTPER                                    */
            uint32_t            : 11;
    } CFG3H_b;
  } ;
  __IM  uint32_t  RESERVED19[4];
  __IOM uint32_t  SAR4;                         /*!< (@ 0x00000160) SOURCE ADDRESS REGISTER                                    */
  __IM  uint32_t  RESERVED20;
  __IOM uint32_t  DAR4;                         /*!< (@ 0x00000168) DESTINATION ADDRESS REGISTER                               */
  __IM  uint32_t  RESERVED21;
  
  union {
    __IOM uint32_t LLP4;                        /*!< (@ 0x00000170) LINKED LIST POINTER REGISTER                               */
    
    struct {
      __IOM uint32_t LMS        : 2;            /*!< [1..0] List Master Select                                                 */
      __IOM uint32_t LOC        : 30;           /*!< [31..2] Starting Address In Memory                                        */
    } LLP4_b;
  } ;
  __IM  uint32_t  RESERVED22;
  
  union {
    __IOM uint32_t CTRL4L;                      /*!< (@ 0x00000178) CONTROL REGISTER LOW                                       */
    
    struct {
      __IOM uint32_t INTEN      : 1;            /*!< [0..0] Interrupt Enable Bit                                               */
      __IOM uint32_t DSTTRWIDTH : 3;            /*!< [3..1] Destination Transfer Width                                         */
      __IOM uint32_t SRCTRWIDTH : 3;            /*!< [6..4] Source Transfer Width                                              */
      __IOM uint32_t DINC       : 2;            /*!< [8..7] Destination Address Increment                                      */
      __IOM uint32_t SINC       : 2;            /*!< [10..9] Source Address Increment                                          */
      __IOM uint32_t DESTMSIZE  : 3;            /*!< [13..11] Destination Burst Transaction Length                             */
      __IOM uint32_t SRCMSIZE   : 3;            /*!< [16..14] Source Burst Transaction Length                                  */
            uint32_t            : 3;
      __IOM uint32_t TTFC       : 3;            /*!< [22..20] Transfer Type and Flow Control                                   */
            uint32_t            : 9;
    } CTRL4L_b;
  } ;
  
  union {
    __IOM uint32_t CTRL4H;                      /*!< (@ 0x0000017C) CONTROL REGISTER HIGH                                      */
    
    struct {
      __IOM uint32_t BLOCKTS    : 32;           /*!< [31..0] Block Transfer Size                                               */
    } CTRL4H_b;
  } ;
  __IM  uint32_t  RESERVED23[8];
  
  union {
    __IOM uint32_t CFG4L;                       /*!< (@ 0x000001A0) CONFIGURATION REGISTER LOW                                 */
    
    struct {
      __IM  uint32_t INACTIVE   : 1;            /*!< [0..0] Indicates if the channel is inactive                               */
      __IM  uint32_t SRCPCTLOVER : 1;           /*!< [1..1] DMA Source bus protocol finish, indicating the current
                                                     read command has finished                                                 */
      __IM  uint32_t DSTPCTLOVER : 1;           /*!< [2..2] DMA destination bus protocol finish, indicating the current
                                                     read command has finished                                                 */
            uint32_t            : 1;
      __IOM uint32_t CHPRIOR    : 4;            /*!< [7..4] Channel Priority                                                   */
      __IOM uint32_t CHSUSP     : 1;            /*!< [8..8] Channel Suspend                                                    */
      __IM  uint32_t FIFOEMPTY  : 1;            /*!< [9..9] Indicates if there is data left in the channel FIFO                */
      __IOM uint32_t HSSELDST   : 1;            /*!< [10..10] Destination Software or Hardware Handshaking Select              */
      __IOM uint32_t HSSELSRC   : 1;            /*!< [11..11] Source Software or Hardware Handshaking Select                   */
            uint32_t            : 6;
      __IOM uint32_t DSTHSPOL   : 1;            /*!< [18..18] Destination Handshaking Interface Polarity                       */
      __IOM uint32_t SRCHSPOL   : 1;            /*!< [19..19] Source Handshaking Interface Polarity                            */
            uint32_t            : 12;
    } CFG4L_b;
  } ;
  
  union {
    __IOM uint32_t CFG4H;                       /*!< (@ 0x000001A4) CONFIGURATION REGISTER HIGH                                */
    
    struct {
            uint32_t            : 7;
      __IOM uint32_t SRCPER     : 4;            /*!< [10..7] Assigns a Hardware Handshaking Interface to The Source
                                                     of Channel                                                                */
      __IOM uint32_t DSTPER     : 4;            /*!< [14..11] Assigns a Hardware Handshaking Interface to The Destination
                                                     of Channel                                                                */
      __IOM uint32_t EXTSRCPER1 : 1;            /*!< [15..15] The extened 4th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER1 : 1;            /*!< [16..16] The extened 4th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER2 : 1;            /*!< [17..17] The extened 5th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER2 : 1;            /*!< [18..18] The extened 5th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER3 : 1;            /*!< [19..19] The extened 6th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER3 : 1;            /*!< [20..20] The extened 6th bit of DSTPER                                    */
            uint32_t            : 11;
    } CFG4H_b;
  } ;
  __IM  uint32_t  RESERVED24[4];
  __IOM uint32_t  SAR5;                         /*!< (@ 0x000001B8) SOURCE ADDRESS REGISTER                                    */
  __IM  uint32_t  RESERVED25;
  __IOM uint32_t  DAR5;                         /*!< (@ 0x000001C0) DESTINATION ADDRESS REGISTER                               */
  __IM  uint32_t  RESERVED26;
  
  union {
    __IOM uint32_t LLP5;                        /*!< (@ 0x000001C8) LINKED LIST POINTER REGISTER                               */
    
    struct {
      __IOM uint32_t LMS        : 2;            /*!< [1..0] List Master Select                                                 */
      __IOM uint32_t LOC        : 30;           /*!< [31..2] Starting Address In Memory                                        */
    } LLP5_b;
  } ;
  __IM  uint32_t  RESERVED27;
  
  union {
    __IOM uint32_t CTRL5L;                      /*!< (@ 0x000001D0) CONTROL REGISTER LOW                                       */
    
    struct {
      __IOM uint32_t INTEN      : 1;            /*!< [0..0] Interrupt Enable Bit                                               */
      __IOM uint32_t DSTTRWIDTH : 3;            /*!< [3..1] Destination Transfer Width                                         */
      __IOM uint32_t SRCTRWIDTH : 3;            /*!< [6..4] Source Transfer Width                                              */
      __IOM uint32_t DINC       : 2;            /*!< [8..7] Destination Address Increment                                      */
      __IOM uint32_t SINC       : 2;            /*!< [10..9] Source Address Increment                                          */
      __IOM uint32_t DESTMSIZE  : 3;            /*!< [13..11] Destination Burst Transaction Length                             */
      __IOM uint32_t SRCMSIZE   : 3;            /*!< [16..14] Source Burst Transaction Length                                  */
            uint32_t            : 3;
      __IOM uint32_t TTFC       : 3;            /*!< [22..20] Transfer Type and Flow Control                                   */
            uint32_t            : 9;
    } CTRL5L_b;
  } ;
  
  union {
    __IOM uint32_t CTRL5H;                      /*!< (@ 0x000001D4) CONTROL REGISTER HIGH                                      */
    
    struct {
      __IOM uint32_t BLOCKTS    : 32;           /*!< [31..0] Block Transfer Size                                               */
    } CTRL5H_b;
  } ;
  __IM  uint32_t  RESERVED28[8];
  
  union {
    __IOM uint32_t CFG5L;                       /*!< (@ 0x000001F8) CONFIGURATION REGISTER LOW                                 */
    
    struct {
      __IM  uint32_t INACTIVE   : 1;            /*!< [0..0] Indicates if the channel is inactive                               */
      __IM  uint32_t SRCPCTLOVER : 1;           /*!< [1..1] DMA Source bus protocol finish, indicating the current
                                                     read command has finished                                                 */
      __IM  uint32_t DSTPCTLOVER : 1;           /*!< [2..2] DMA destination bus protocol finish, indicating the current
                                                     read command has finished                                                 */
            uint32_t            : 1;
      __IOM uint32_t CHPRIOR    : 4;            /*!< [7..4] Channel Priority                                                   */
      __IOM uint32_t CHSUSP     : 1;            /*!< [8..8] Channel Suspend                                                    */
      __IM  uint32_t FIFOEMPTY  : 1;            /*!< [9..9] Indicates if there is data left in the channel FIFO                */
      __IOM uint32_t HSSELDST   : 1;            /*!< [10..10] Destination Software or Hardware Handshaking Select              */
      __IOM uint32_t HSSELSRC   : 1;            /*!< [11..11] Source Software or Hardware Handshaking Select                   */
            uint32_t            : 6;
      __IOM uint32_t DSTHSPOL   : 1;            /*!< [18..18] Destination Handshaking Interface Polarity                       */
      __IOM uint32_t SRCHSPOL   : 1;            /*!< [19..19] Source Handshaking Interface Polarity                            */
            uint32_t            : 12;
    } CFG5L_b;
  } ;
  
  union {
    __IOM uint32_t CFG5H;                       /*!< (@ 0x000001FC) CONFIGURATION REGISTER HIGH                                */
    
    struct {
            uint32_t            : 7;
      __IOM uint32_t SRCPER     : 4;            /*!< [10..7] Assigns a Hardware Handshaking Interface to The Source
                                                     of Channel                                                                */
      __IOM uint32_t DSTPER     : 4;            /*!< [14..11] Assigns a Hardware Handshaking Interface to The Destination
                                                     of Channel                                                                */
      __IOM uint32_t EXTSRCPER1 : 1;            /*!< [15..15] The extened 4th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER1 : 1;            /*!< [16..16] The extened 4th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER2 : 1;            /*!< [17..17] The extened 5th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER2 : 1;            /*!< [18..18] The extened 5th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER3 : 1;            /*!< [19..19] The extened 6th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER3 : 1;            /*!< [20..20] The extened 6th bit of DSTPER                                    */
            uint32_t            : 11;
    } CFG5H_b;
  } ;
  __IM  uint32_t  RESERVED29[4];
  __IOM uint32_t  SAR6;                         /*!< (@ 0x00000210) SOURCE ADDRESS REGISTER                                    */
  __IM  uint32_t  RESERVED30;
  __IOM uint32_t  DAR6;                         /*!< (@ 0x00000218) DESTINATION ADDRESS REGISTER                               */
  __IM  uint32_t  RESERVED31;
  
  union {
    __IOM uint32_t LLP6;                        /*!< (@ 0x00000220) LINKED LIST POINTER REGISTER                               */
    
    struct {
      __IOM uint32_t LMS        : 2;            /*!< [1..0] List Master Select                                                 */
      __IOM uint32_t LOC        : 30;           /*!< [31..2] Starting Address In Memory                                        */
    } LLP6_b;
  } ;
  __IM  uint32_t  RESERVED32;
  
  union {
    __IOM uint32_t CTRL6L;                      /*!< (@ 0x00000228) CONTROL REGISTER LOW                                       */
    
    struct {
      __IOM uint32_t INTEN      : 1;            /*!< [0..0] Interrupt Enable Bit                                               */
      __IOM uint32_t DSTTRWIDTH : 3;            /*!< [3..1] Destination Transfer Width                                         */
      __IOM uint32_t SRCTRWIDTH : 3;            /*!< [6..4] Source Transfer Width                                              */
      __IOM uint32_t DINC       : 2;            /*!< [8..7] Destination Address Increment                                      */
      __IOM uint32_t SINC       : 2;            /*!< [10..9] Source Address Increment                                          */
      __IOM uint32_t DESTMSIZE  : 3;            /*!< [13..11] Destination Burst Transaction Length                             */
      __IOM uint32_t SRCMSIZE   : 3;            /*!< [16..14] Source Burst Transaction Length                                  */
            uint32_t            : 3;
      __IOM uint32_t TTFC       : 3;            /*!< [22..20] Transfer Type and Flow Control                                   */
            uint32_t            : 9;
    } CTRL6L_b;
  } ;
  
  union {
    __IOM uint32_t CTRL6H;                      /*!< (@ 0x0000022C) CONTROL REGISTER HIGH                                      */
    
    struct {
      __IOM uint32_t BLOCKTS    : 32;           /*!< [31..0] Block Transfer Size                                               */
    } CTRL6H_b;
  } ;
  __IM  uint32_t  RESERVED33[8];
  
  union {
    __IOM uint32_t CFG6L;                       /*!< (@ 0x00000250) CONFIGURATION REGISTER LOW                                 */
    
    struct {
      __IM  uint32_t INACTIVE   : 1;            /*!< [0..0] Indicates if the channel is inactive                               */
      __IM  uint32_t SRCPCTLOVER : 1;           /*!< [1..1] DMA Source bus protocol finish, indicating the current
                                                     read command has finished                                                 */
      __IM  uint32_t DSTPCTLOVER : 1;           /*!< [2..2] DMA destination bus protocol finish, indicating the current
                                                     read command has finished                                                 */
            uint32_t            : 1;
      __IOM uint32_t CHPRIOR    : 4;            /*!< [7..4] Channel Priority                                                   */
      __IOM uint32_t CHSUSP     : 1;            /*!< [8..8] Channel Suspend                                                    */
      __IM  uint32_t FIFOEMPTY  : 1;            /*!< [9..9] Indicates if there is data left in the channel FIFO                */
      __IOM uint32_t HSSELDST   : 1;            /*!< [10..10] Destination Software or Hardware Handshaking Select              */
      __IOM uint32_t HSSELSRC   : 1;            /*!< [11..11] Source Software or Hardware Handshaking Select                   */
            uint32_t            : 6;
      __IOM uint32_t DSTHSPOL   : 1;            /*!< [18..18] Destination Handshaking Interface Polarity                       */
      __IOM uint32_t SRCHSPOL   : 1;            /*!< [19..19] Source Handshaking Interface Polarity                            */
            uint32_t            : 12;
    } CFG6L_b;
  } ;
  
  union {
    __IOM uint32_t CFG6H;                       /*!< (@ 0x00000254) CONFIGURATION REGISTER HIGH                                */
    
    struct {
            uint32_t            : 7;
      __IOM uint32_t SRCPER     : 4;            /*!< [10..7] Assigns a Hardware Handshaking Interface to The Source
                                                     of Channel                                                                */
      __IOM uint32_t DSTPER     : 4;            /*!< [14..11] Assigns a Hardware Handshaking Interface to The Destination
                                                     of Channel                                                                */
      __IOM uint32_t EXTSRCPER1 : 1;            /*!< [15..15] The extened 4th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER1 : 1;            /*!< [16..16] The extened 4th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER2 : 1;            /*!< [17..17] The extened 5th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER2 : 1;            /*!< [18..18] The extened 5th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER3 : 1;            /*!< [19..19] The extened 6th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER3 : 1;            /*!< [20..20] The extened 6th bit of DSTPER                                    */
            uint32_t            : 11;
    } CFG6H_b;
  } ;
  __IM  uint32_t  RESERVED34[4];
  __IOM uint32_t  SAR7;                         /*!< (@ 0x00000268) SOURCE ADDRESS REGISTER                                    */
  __IM  uint32_t  RESERVED35;
  __IOM uint32_t  DAR7;                         /*!< (@ 0x00000270) DESTINATION ADDRESS REGISTER                               */
  __IM  uint32_t  RESERVED36;
  
  union {
    __IOM uint32_t LLP7;                        /*!< (@ 0x00000278) LINKED LIST POINTER REGISTER                               */
    
    struct {
      __IOM uint32_t LMS        : 2;            /*!< [1..0] List Master Select                                                 */
      __IOM uint32_t LOC        : 30;           /*!< [31..2] Starting Address In Memory                                        */
    } LLP7_b;
  } ;
  __IM  uint32_t  RESERVED37;
  
  union {
    __IOM uint32_t CTRL7L;                      /*!< (@ 0x00000280) CONTROL REGISTER LOW                                       */
    
    struct {
      __IOM uint32_t INTEN      : 1;            /*!< [0..0] Interrupt Enable Bit                                               */
      __IOM uint32_t DSTTRWIDTH : 3;            /*!< [3..1] Destination Transfer Width                                         */
      __IOM uint32_t SRCTRWIDTH : 3;            /*!< [6..4] Source Transfer Width                                              */
      __IOM uint32_t DINC       : 2;            /*!< [8..7] Destination Address Increment                                      */
      __IOM uint32_t SINC       : 2;            /*!< [10..9] Source Address Increment                                          */
      __IOM uint32_t DESTMSIZE  : 3;            /*!< [13..11] Destination Burst Transaction Length                             */
      __IOM uint32_t SRCMSIZE   : 3;            /*!< [16..14] Source Burst Transaction Length                                  */
            uint32_t            : 3;
      __IOM uint32_t TTFC       : 3;            /*!< [22..20] Transfer Type and Flow Control                                   */
            uint32_t            : 9;
    } CTRL7L_b;
  } ;
  
  union {
    __IOM uint32_t CTRL7H;                      /*!< (@ 0x00000284) CONTROL REGISTER HIGH                                      */
    
    struct {
      __IOM uint32_t BLOCKTS    : 32;           /*!< [31..0] Block Transfer Size                                               */
    } CTRL7H_b;
  } ;
  __IM  uint32_t  RESERVED38[8];
  
  union {
    __IOM uint32_t CFG7L;                       /*!< (@ 0x000002A8) CONFIGURATION REGISTER LOW                                 */
    
    struct {
      __IM  uint32_t INACTIVE   : 1;            /*!< [0..0] Indicates if the channel is inactive                               */
      __IM  uint32_t SRCPCTLOVER : 1;           /*!< [1..1] DMA Source bus protocol finish, indicating the current
                                                     read command has finished                                                 */
      __IM  uint32_t DSTPCTLOVER : 1;           /*!< [2..2] DMA destination bus protocol finish, indicating the current
                                                     read command has finished                                                 */
            uint32_t            : 1;
      __IOM uint32_t CHPRIOR    : 4;            /*!< [7..4] Channel Priority                                                   */
      __IOM uint32_t CHSUSP     : 1;            /*!< [8..8] Channel Suspend                                                    */
      __IM  uint32_t FIFOEMPTY  : 1;            /*!< [9..9] Indicates if there is data left in the channel FIFO                */
      __IOM uint32_t HSSELDST   : 1;            /*!< [10..10] Destination Software or Hardware Handshaking Select              */
      __IOM uint32_t HSSELSRC   : 1;            /*!< [11..11] Source Software or Hardware Handshaking Select                   */
            uint32_t            : 6;
      __IOM uint32_t DSTHSPOL   : 1;            /*!< [18..18] Destination Handshaking Interface Polarity                       */
      __IOM uint32_t SRCHSPOL   : 1;            /*!< [19..19] Source Handshaking Interface Polarity                            */
            uint32_t            : 12;
    } CFG7L_b;
  } ;
  
  union {
    __IOM uint32_t CFG7H;                       /*!< (@ 0x000002AC) CONFIGURATION REGISTER HIGH                                */
    
    struct {
            uint32_t            : 7;
      __IOM uint32_t SRCPER     : 4;            /*!< [10..7] Assigns a Hardware Handshaking Interface to The Source
                                                     of Channel                                                                */
      __IOM uint32_t DSTPER     : 4;            /*!< [14..11] Assigns a Hardware Handshaking Interface to The Destination
                                                     of Channel                                                                */
      __IOM uint32_t EXTSRCPER1 : 1;            /*!< [15..15] The extened 4th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER1 : 1;            /*!< [16..16] The extened 4th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER2 : 1;            /*!< [17..17] The extened 5th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER2 : 1;            /*!< [18..18] The extened 5th bit of DSTPER                                    */
      __IOM uint32_t EXTSRCPER3 : 1;            /*!< [19..19] The extened 6th bit of SRCPER                                    */
      __IOM uint32_t EXTDSTPER3 : 1;            /*!< [20..20] The extened 6th bit of DSTPER                                    */
            uint32_t            : 11;
    } CFG7H_b;
  } ;
  __IM  uint32_t  RESERVED39[4];
  __IOM uint32_t  RAWTFR;                       /*!< (@ 0x000002C0) RAW STATUS FOR INTTFR INTERRUPT REGISTER                   */
  __IM  uint32_t  RESERVED40;
  __IOM uint32_t  RAWBLOCK;                     /*!< (@ 0x000002C8) RAW STATUS FOR INTBLOCK REGISTER                           */
  __IM  uint32_t  RESERVED41;
  __IOM uint32_t  RAWSRCTRAN;                   /*!< (@ 0x000002D0) RAW STATUS FOR INTSRCTRAN INTERRUPT REGISTER               */
  __IM  uint32_t  RESERVED42;
  __IOM uint32_t  RAWDSTTRAN;                   /*!< (@ 0x000002D8) RAW STATUS FOR INTDSTTRAN INTERRUPT REGISTER               */
  __IM  uint32_t  RESERVED43;
  __IOM uint32_t  RAWERR;                       /*!< (@ 0x000002E0) RAW STATUS FOR INTERR INTERRUPT REGISTER                   */
  __IM  uint32_t  RESERVED44;
  __IOM uint32_t  STSTFR;                       /*!< (@ 0x000002E8) INTERRUPT STATUS FOR INTTFR INTERRUPT REGISTER             */
  __IM  uint32_t  RESERVED45;
  __IOM uint32_t  STSBLOCK;                     /*!< (@ 0x000002F0) INTERRUPT STATUS FOR INTBLOCK REGISTER                     */
  __IM  uint32_t  RESERVED46;
  __IOM uint32_t  STSSRCTRAN;                   /*!< (@ 0x000002F8) INTERRUPT STATUS FOR INTSRCTRAN INTERRUPT REGISTER         */
  __IM  uint32_t  RESERVED47;
  __IOM uint32_t  STSDSTTRAN;                   /*!< (@ 0x00000300) INTERRUPT STATUS FOR INTDSTTRAN INTERRUPT REGISTER         */
  __IM  uint32_t  RESERVED48;
  __IOM uint32_t  STSERR;                       /*!< (@ 0x00000308) INTERRUPT STATUS FOR INTERR INTERRUPT REGISTER             */
  __IM  uint32_t  RESERVED49;
  
  union {
    __IOM uint32_t MSKTFR;                      /*!< (@ 0x00000310) MASK FOR INTTFR INTERRUPT REGISTER                         */
    
    struct {
      __IOM uint32_t INTMSK     : 8;            /*!< [7..0] Interrupt Mask                                                     */
      __IOM uint32_t INTMASKWE  : 8;            /*!< [15..8] Interrupt Mask Write Enable                                       */
            uint32_t            : 16;
    } MSKTFR_b;
  } ;
  __IM  uint32_t  RESERVED50;
  
  union {
    __IOM uint32_t MSKBLOCK;                    /*!< (@ 0x00000318) MASK FOR INTBLOCK REGISTER                                 */
    
    struct {
      __IOM uint32_t INTMSK     : 8;            /*!< [7..0] Interrupt Mask                                                     */
      __IOM uint32_t INTMASKWE  : 8;            /*!< [15..8] Interrupt Mask Write Enable                                       */
            uint32_t            : 16;
    } MSKBLOCK_b;
  } ;
  __IM  uint32_t  RESERVED51;
  
  union {
    __IOM uint32_t MSKSRCTRAN;                  /*!< (@ 0x00000320) MASK FOR INTSRCTRAN INTERRUPT REGISTER                     */
    
    struct {
      __IOM uint32_t INTMSK     : 8;            /*!< [7..0] Interrupt Mask                                                     */
      __IOM uint32_t INTMASKWE  : 8;            /*!< [15..8] Interrupt Mask Write Enable                                       */
            uint32_t            : 16;
    } MSKSRCTRAN_b;
  } ;
  __IM  uint32_t  RESERVED52;
  
  union {
    __IOM uint32_t MSKDSTTRAN;                  /*!< (@ 0x00000328) MASK FOR INTDSTTRAN INTERRUPT REGISTER                     */
    
    struct {
      __IOM uint32_t INTMSK     : 8;            /*!< [7..0] Interrupt Mask                                                     */
      __IOM uint32_t INTMASKWE  : 8;            /*!< [15..8] Interrupt Mask Write Enable                                       */
            uint32_t            : 16;
    } MSKDSTTRAN_b;
  } ;
  __IM  uint32_t  RESERVED53;
  
  union {
    __IOM uint32_t MSKERR;                      /*!< (@ 0x00000330) MASK FOR INTERR INTERRUPT REGISTER                         */
    
    struct {
      __IOM uint32_t INTMSK     : 8;            /*!< [7..0] Interrupt Mask                                                     */
      __IOM uint32_t INTMASKWE  : 8;            /*!< [15..8] Interrupt Mask Write Enable                                       */
            uint32_t            : 16;
    } MSKERR_b;
  } ;
  __IM  uint32_t  RESERVED54;
  __IOM uint32_t  CLRTFR;                       /*!< (@ 0x00000338) CLEAR INTERRUPT STATUS FOR INTTFR INTERRUPT REGISTER       */
  __IM  uint32_t  RESERVED55;
  __IOM uint32_t  CLRBLOCK;                     /*!< (@ 0x00000340) CLEAR INTERRUPT STATUS FOR INTBLOCK REGISTER               */
  __IM  uint32_t  RESERVED56;
  __IOM uint32_t  CLRSRCTRAN;                   /*!< (@ 0x00000348) CLEAR INTERRUPT STATUS FOR INTSRCTRAN INTERRUPT
                                                                    REGISTER                                                   */
  __IM  uint32_t  RESERVED57;
  __IOM uint32_t  CLRDSTTRAN;                   /*!< (@ 0x00000350) CLEAR INTERRUPT STATUS FOR INTDSTTRAN INTERRUPT
                                                                    REGISTER                                                   */
  __IM  uint32_t  RESERVED58;
  __IOM uint32_t  CLRERR;                       /*!< (@ 0x00000358) CLEAR INTERRUPT STATUS FOR INTERR INTERRUPT REGISTER       */
  __IM  uint32_t  RESERVED59;
  
  union {
    __IM  uint32_t INTSTS;                      /*!< (@ 0x00000360) COMBINED INTERRUPT SATAUS REGISTER                         */
    
    struct {
      __IM  uint32_t TFR        : 1;            /*!< [0..0] OR of The Contents of StatusTfr Register                           */
      __IM  uint32_t BLOCK      : 1;            /*!< [1..1] OR of The Contents of StatusBlock Register                         */
      __IM  uint32_t SRCT       : 1;            /*!< [2..2] OR of The Contents of StatusSrcTran Register                       */
      __IM  uint32_t DSTT       : 1;            /*!< [3..3] OR of The Contents of StatusDst Register                           */
      __IM  uint32_t ERR        : 1;            /*!< [4..4] OR of The Contents of StatusErr Register                           */
            uint32_t            : 27;
    } INTSTS_b;
  } ;
  __IM  uint32_t  RESERVED60[13];
  
  union {
    __IOM uint32_t CONF;                        /*!< (@ 0x00000398) CONFIGURATION REGISTER                                     */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable DMA Controller                                              */
            uint32_t            : 31;
    } CONF_b;
  } ;
  __IM  uint32_t  RESERVED61;
  
  union {
    __IOM uint32_t CHEN;                        /*!< (@ 0x000003A0) CHANNEL ENABLE REGISTER                                    */
    
    struct {
      __IOM uint32_t CH0EN      : 1;            /*!< [0..0] Enables DMA Channel                                                */
      __IOM uint32_t CH1EN      : 1;            /*!< [1..1] Enables DMA Channel                                                */
      __IOM uint32_t CH2EN      : 1;            /*!< [2..2] Enables DMA Channel                                                */
      __IOM uint32_t CH3EN      : 1;            /*!< [3..3] Enables DMA Channel                                                */
      __IOM uint32_t CH4EN      : 1;            /*!< [4..4] Enables DMA Channel                                                */
      __IOM uint32_t CH5EN      : 1;            /*!< [5..5] Enables DMA Channel                                                */
      __IOM uint32_t CH6EN      : 1;            /*!< [6..6] Enables DMA Channel                                                */
      __IOM uint32_t CH7EN      : 1;            /*!< [7..7] Enables DMA Channel                                                */
            uint32_t            : 7;
      __IOM uint32_t CH0WREN    : 1;            /*!< [15..15] Enables DMA Channel Write Enable                                 */
      __IOM uint32_t CH1WREN    : 1;            /*!< [16..16] Enables DMA Channel Write Enable                                 */
      __IOM uint32_t CH2WREN    : 1;            /*!< [17..17] Enables DMA Channel Write Enable                                 */
      __IOM uint32_t CH3WREN    : 1;            /*!< [18..18] Enables DMA Channel Write Enable                                 */
      __IOM uint32_t CH4WREN    : 1;            /*!< [19..19] Enables DMA Channel Write Enable                                 */
      __IOM uint32_t CH5WREN    : 1;            /*!< [20..20] Enables DMA Channel Write Enable                                 */
      __IOM uint32_t CH6WREN    : 1;            /*!< [21..21] Enables DMA Channel Write Enable                                 */
      __IOM uint32_t CH7WREN    : 1;            /*!< [22..22] Enables DMA Channel Write Enable                                 */
            uint32_t            : 9;
    } CHEN_b;
  } ;
} DMA_Type;                                     /*!< Size = 932 (0x3a4)                                                        */



/* =========================================================================================================================== */
/* ================                                            WDT                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief WDT Controller (WDT)
  */

typedef struct {                                /*!< (@ 0x40253F00) WDT Structure                                              */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000000) CONTROL REGISTER                                           */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable WDT                                                         */
      __IOM uint32_t RSTEN      : 1;            /*!< [1..1] Enable WDT to Reset System                                         */
      __IOM uint32_t RELOAD     : 1;            /*!< [2..2] Reload WDT Counter                                                 */
      __IOM uint32_t CLRRSTFLAG : 1;            /*!< [3..3] Clear WDT reset flag                                               */
            uint32_t            : 28;
    } CTRL_b;
  } ;
  
  union {
    __IM  uint32_t STS;                         /*!< (@ 0x00000004) STATUS REGISTER                                            */
    
    struct {
      __IM  uint32_t RSTFLAG    : 1;            /*!< [0..0] Reset Flag                                                         */
            uint32_t            : 31;
    } STS_b;
  } ;
  __IOM uint32_t  CNT;                          /*!< (@ 0x00000008) INITIAL COUNTER REGISTER                                   */
  __IOM uint32_t  DIV;                          /*!< (@ 0x0000000C) DIVIDER REGISTER                                           */
  
  union {
    __IOM uint32_t INTEN;                       /*!< (@ 0x00000010) INTERRUPT ENABLE REGISTER                                  */
    
    struct {
      __IOM uint32_t WDTINTEN   : 1;            /*!< [0..0] Enable WDT Interrupt                                               */
            uint32_t            : 31;
    } INTEN_b;
  } ;
} WDT_Type;                                     /*!< Size = 20 (0x14)                                                          */



/* =========================================================================================================================== */
/* ================                                          PWRBTN0                                          ================ */
/* =========================================================================================================================== */


/**
  * @brief Power Button Over WDT Controller 0 (PWRBTN0)
  */

typedef struct {                                /*!< (@ 0x40253D00) PWRBTN0 Structure                                          */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000000) CONTROL REGISTER                                           */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable WDT                                                         */
      __IOM uint32_t RSTEN      : 1;            /*!< [1..1] Enable WDT to Reset System                                         */
      __IOM uint32_t RELOAD     : 1;            /*!< [2..2] Reload WDT Counter                                                 */
      __IOM uint32_t CLRRSTFLAG : 1;            /*!< [3..3] Clear WDT reset flag                                               */
      __IOM uint32_t PWRBTNTRIG : 1;            /*!< [4..4] Pwrbtn trigger status                                              */
            uint32_t            : 27;
    } CTRL_b;
  } ;
  
  union {
    __IM  uint32_t STS;                         /*!< (@ 0x00000004) STATUS REGISTER                                            */
    
    struct {
      __IM  uint32_t RSTFLAG    : 1;            /*!< [0..0] Reset Flag                                                         */
            uint32_t            : 31;
    } STS_b;
  } ;
  
  union {
    __IOM uint32_t CNT;                         /*!< (@ 0x00000008) INITIAL COUNTER REGISTER                                   */
    
    struct {
      __IOM uint32_t CNT        : 8;            /*!< [7..0] Counter value                                                      */
            uint32_t            : 24;
    } CNT_b;
  } ;
  
  union {
    __IOM uint32_t DIV;                         /*!< (@ 0x0000000C) DIVIDER REGISTER                                           */
    
    struct {
      __IOM uint32_t DIV        : 16;           /*!< [15..0] Divider value                                                     */
            uint32_t            : 16;
    } DIV_b;
  } ;
  
  union {
    __IOM uint32_t INTEN;                       /*!< (@ 0x00000010) INTERRUPT ENABLE REGISTER                                  */
    
    struct {
      __IOM uint32_t WDTINTEN   : 1;            /*!< [0..0] Enable WDT Interrupt                                               */
            uint32_t            : 31;
    } INTEN_b;
  } ;
  
  union {
    __IOM uint32_t CONF;                        /*!< (@ 0x00000014) PWRBTN CONTROL REGISTER                                    */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable Power Button Function                                       */
      __IOM uint32_t TRIGMD     : 1;            /*!< [1..1] Trigger Level Selection                                            */
      __IOM uint32_t RSTOUTPOL  : 1;            /*!< [2..2] Reset Output Polarity                                              */
            uint32_t            : 13;
      __IOM uint32_t RSTPULSEWIDTH : 16;        /*!< [31..16] Reset Output Pulse Width                                         */
    } CONF_b;
  } ;
} PWRBTN_Type;                                  /*!< Size = 24 (0x18)                                                          */



/* =========================================================================================================================== */
/* ================                                            RTC                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief RTC Controller (RTC)
  */

typedef struct {                                /*!< (@ 0x40253C00) RTC Structure                                              */
  __IOM uint8_t   SEC;                          /*!< (@ 0x00000000) SECOND REGISTER                                            */
  __IOM uint8_t   SECALARM;                     /*!< (@ 0x00000001) SECOND ALARM REGISTER                                      */
  __IOM uint8_t   MIN;                          /*!< (@ 0x00000002) MINUTES REGISTER                                           */
  __IOM uint8_t   MINALARM;                     /*!< (@ 0x00000003) MINUTES ALARM REGISTER                                     */
  
  union {
    __IOM uint8_t HR;                           /*!< (@ 0x00000004) HOUR REGISTER                                              */
    
    struct {
      __IOM uint8_t VAL         : 7;            /*!< [6..0] Value of Hour                                                      */
      __IOM uint8_t AMPM        : 1;            /*!< [7..7] AM or PM Mode                                                      */
    } HR_b;
  } ;
  
  union {
    __IOM uint8_t HRALARM;                      /*!< (@ 0x00000005) HOUR ALARM REGISTER                                        */
    
    struct {
      __IOM uint8_t VAL         : 7;            /*!< [6..0] Value of Hour Alarm                                                */
      __IOM uint8_t AMPM        : 1;            /*!< [7..7] AM or PM Mode of Hour Alarm                                        */
    } HRALARM_b;
  } ;
  __IOM uint8_t   DAYWEEK;                      /*!< (@ 0x00000006) DAY OF WEEK REGISTER                                       */
  __IOM uint8_t   DAYMONTH;                     /*!< (@ 0x00000007) DAY OF MONTH REGISTER                                      */
  __IOM uint8_t   MONTH;                        /*!< (@ 0x00000008) MONTH REGISTER                                             */
  __IOM uint8_t   YEAR;                         /*!< (@ 0x00000009) YEAR REGISTER                                              */
  
  union {
    __IOM uint8_t CTRL0;                        /*!< (@ 0x0000000A) CONTROL REGISTER #0                                        */
    
    struct {
      __IOM uint8_t RATESEL     : 4;            /*!< [3..0] Periodic Interrupt Rate Select                                     */
      __IOM uint8_t DIVCTL      : 3;            /*!< [6..4] Divider Chain Control                                              */
      __IOM uint8_t UIP         : 1;            /*!< [7..7] Update In Progress                                                 */
    } CTRL0_b;
  } ;
  
  union {
    __IOM uint8_t CTRL1;                        /*!< (@ 0x0000000B) CONTROL REGISTER #1                                        */
    
    struct {
      __IOM uint8_t DLS         : 1;            /*!< [0..0] Daylight Saving Enable                                             */
      __IOM uint8_t HRMODE      : 1;            /*!< [1..1] Hour Mode Selection                                                */
      __IOM uint8_t DATEMODE    : 1;            /*!< [2..2] Date Mode Selection                                                */
            uint8_t             : 1;
      __IOM uint8_t UIE         : 1;            /*!< [4..4] Update Ended Interrupt Enable                                      */
      __IOM uint8_t AIE         : 1;            /*!< [5..5] Alarm Interrupt Enable                                             */
      __IOM uint8_t PIE         : 1;            /*!< [6..6] Periodic Interrupt Enable                                          */
      __IOM uint8_t SETMODE     : 1;            /*!< [7..7] Set Enable                                                         */
    } CTRL1_b;
  } ;
  
  union {
    __IOM uint8_t CTRL2;                        /*!< (@ 0x0000000C) CONTROL REGISTER #2                                        */
    
    struct {
            uint8_t             : 4;
      __IOM uint8_t UIF         : 1;            /*!< [4..4] Update Interrupt Flag                                              */
      __IOM uint8_t AIF         : 1;            /*!< [5..5] Alarm Interrupt Flag                                               */
      __IOM uint8_t PIF         : 1;            /*!< [6..6] Periodic Interrupt Flag                                            */
      __IOM uint8_t IRQF        : 1;            /*!< [7..7] Interrupt Flag                                                     */
    } CTRL2_b;
  } ;
  __IOM uint8_t   DAYWEEKALARM;                 /*!< (@ 0x0000000D) DAY OF WEEK ALARM REGISTER                                 */
  __IOM uint8_t   DAYMONTHALARM;                /*!< (@ 0x0000000E) DAY OF MONTH ALARM REGISTER                                */
  __IM  uint8_t   RESERVED;
  __IM  uint32_t  RESERVED1[2];
  
  union {
    __IOM uint32_t DLSFW;                       /*!< (@ 0x00000018) DAYLIGHT SAVINGS FORWARD REGISTER                          */
    
    struct {
      __IOM uint32_t DSTMONTH   : 8;            /*!< [7..0] Daylight Saving Forward Month                                      */
      __IOM uint32_t DSTDAYWEEK : 3;            /*!< [10..8] Daylight Saving Forward Day of Week                               */
            uint32_t            : 5;
      __IOM uint32_t DSTWEEK    : 3;            /*!< [18..16] Daylight Saving Forward Week                                     */
            uint32_t            : 5;
      __IOM uint32_t DSTHR      : 7;            /*!< [30..24] Daylight Saving Forward Hour                                     */
      __IOM uint32_t DSTAMPMSEL : 1;            /*!< [31..31] Daylight Saving Forward AM/PM                                    */
    } DLSFW_b;
  } ;
  
  union {
    __IOM uint32_t DLSBW;                       /*!< (@ 0x0000001C) DAYLIGHT SAVINGS BACKWARD REGISTER                         */
    
    struct {
      __IOM uint32_t DSTMONTH   : 8;            /*!< [7..0] Daylight Saving Backward Month                                     */
      __IOM uint32_t DSTDAYWEEK : 3;            /*!< [10..8] Daylight Saving Backward Day of Week                              */
            uint32_t            : 5;
      __IOM uint32_t DSTWEEK    : 3;            /*!< [18..16] Daylight Saving Backward Week                                    */
            uint32_t            : 5;
      __IOM uint32_t DSTHR      : 7;            /*!< [30..24] Daylight Saving Backward Hour                                    */
      __IOM uint32_t DSTAMPMSEL : 1;            /*!< [31..31] Daylight Saving Backward AM/PM                                   */
    } DLSBW_b;
  } ;
  
  union {
    __IOM uint8_t WEEK;                         /*!< (@ 0x00000020) NUMBER OF WEEK REGISTER                                    */
    
    struct {
      __IOM uint8_t NUM         : 4;            /*!< [3..0] Number of Week                                                     */
            uint8_t             : 4;
    } WEEK_b;
  } ;
  __IM  uint8_t   RESERVED2;
  __IM  uint16_t  RESERVED3;
} RTC_Type;                                     /*!< Size = 36 (0x24)                                                          */



/* =========================================================================================================================== */
/* ================                                          SLWTMR0                                          ================ */
/* =========================================================================================================================== */


/**
  * @brief 32-bit SlowTimer Controller (SLWTMR0)
  */

typedef struct {                                /*!< (@ 0x40253700) SLWTMR0 Structure                                          */
  __IOM uint32_t  LDCNT;                        /*!< (@ 0x00000000) LOAD COUNTER REGISTER                                      */
  __IOM uint32_t  CNT;                          /*!< (@ 0x00000004) CURRENT COUNTER REGISTER                                   */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000008) CONTROL REGISTER                                           */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable Timer Contoller                                             */
      __IOM uint32_t MDSEL      : 1;            /*!< [1..1] Timer Operating Mode Selection                                     */
      __IOM uint32_t INTEN      : 1;            /*!< [2..2] Enable Timer Interrupt                                             */
      __IOM uint32_t STOP       : 1;            /*!< [3..3] Stop Timer                                                         */
            uint32_t            : 28;
    } CTRL_b;
  } ;
  
  union {
    __IOM uint32_t INTSTS;                      /*!< (@ 0x0000000C) INTERRUPT STATUS REGISTER                                  */
    
    struct {
      __IOM uint32_t STS        : 1;            /*!< [0..0] Timer Interrupt Status                                             */
            uint32_t            : 31;
    } INTSTS_b;
  } ;
} TIMER_Type;                                   /*!< Size = 16 (0x10)                                                          */



/* =========================================================================================================================== */
/* ================                                           I2C1                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief I2C Controller (I2C1)
  */

typedef struct {                                /*!< (@ 0x40250000) I2C1 Structure                                             */
  
  union {
    __IOM uint32_t CON;                         /*!< (@ 0x00000000) CONTROL REGISTER                                           */
    
    struct {
      __IOM uint32_t MATSER     : 1;            /*!< [0..0] Enable/Disable Master Mode                                         */
      __IOM uint32_t SPEED      : 2;            /*!< [2..1] Speed Mode                                                         */
      __IOM uint32_t SLV10BIT   : 1;            /*!< [3..3] Slave Responds To 7-bit Or 10-bit Addresses                        */
      __IOM uint32_t MST10BIT   : 1;            /*!< [4..4] Master Is 7-bit Or 10-bit Addressing Mode                          */
      __IOM uint32_t RESTREN    : 1;            /*!< [5..5] Enable/Disable Master Restart                                      */
      __IOM uint32_t SLVDISABLE : 1;            /*!< [6..6] Enable/Disable Slave Mode                                          */
      __IOM uint32_t STPDET     : 1;            /*!< [7..7] How Slave Issues Stop Detection Interrupt                          */
      __IOM uint32_t TXEMPTYCTRL : 1;           /*!< [8..8] Controls the Generation Of The Tx Empty Interrupt                  */
      __IOM uint32_t RXFULLHLDC : 1;            /*!< [9..9] Hold the Bus When The Rx FIFO Is Full Or Not                       */
      __IOM uint32_t STPMSTACTIVE : 1;          /*!< [10..10] How I2c Controller Issues The Stop Detection Interrupt           */
      __IOM uint32_t BUSCLREN   : 1;            /*!< [11..11] Enables the Bus Clear Feature                                    */
            uint32_t            : 20;
    } CON_b;
  } ;
  
  union {
    __IOM uint32_t TAR;                         /*!< (@ 0x00000004) TARGET ADDRESS REGISTER                                    */
    
    struct {
      __IOM uint32_t TAR        : 10;           /*!< [9..0] Target Address For Any Master Transaction                          */
      __IOM uint32_t GCORSTR    : 1;            /*!< [10..10] Indicates Whether A General Call Or Start Byte Command           */
      __IOM uint32_t SPECIAL    : 1;            /*!< [11..11] Indicates Whether Software Performs A Device-id Or
                                                     General Call Or Start Byte Command                                        */
            uint32_t            : 20;
    } TAR_b;
  } ;
  
  union {
    __IOM uint32_t SAR;                         /*!< (@ 0x00000008) SLAVE ADDRESS REGISTER                                     */
    
    struct {
      __IOM uint32_t SAR        : 10;           /*!< [9..0] Holds The Slave Address When The I2c Is Operating As
                                                     A Slave                                                                   */
            uint32_t            : 22;
    } SAR_b;
  } ;
  __IM  uint32_t  RESERVED;
  
  union {
    __IOM uint32_t DATACMD;                     /*!< (@ 0x00000010) DATA BUFFER AND COMMAND REGISTER                           */
    
    struct {
      __IOM uint32_t DATA       : 8;            /*!< [7..0] The Data To Be Transmitted Or Received On The I2C Bus              */
      __OM  uint32_t CMD        : 1;            /*!< [8..8] Controls Whether A Read Or A Write Is Performed                    */
      __OM  uint32_t STOP       : 1;            /*!< [9..9] Controls Whether A Stop Is Issued After The Byte Is Sent
                                                     Or Received                                                               */
      __OM  uint32_t RESTART    : 1;            /*!< [10..10] Controls Whether a RESTART Is Issued Before The Byte
                                                     Is Sent Or Received                                                       */
      __IM  uint32_t FIRSTDATA  : 1;            /*!< [11..11] Indicates The First Data Byte Received After The Address
                                                     Phase                                                                     */
            uint32_t            : 20;
    } DATACMD_b;
  } ;
  
  union {
    __IOM uint32_t SCLHCNT;                     /*!< (@ 0x00000014) STANDARD SPEED I2C CLOCK SCL HIGH COUNT REGISTER           */
    
    struct {
      __IOM uint32_t HCNT       : 16;           /*!< [15..0] Sets the SCL Clock High-period Count For Standard Speed           */
            uint32_t            : 16;
    } SCLHCNT_b;
  } ;
  
  union {
    __IOM uint32_t SCLLCNT;                     /*!< (@ 0x00000018) STANDARD SPEED I2C CLOCK SCL LOW COUNT REGISTER            */
    
    struct {
      __IOM uint32_t LCNT       : 16;           /*!< [15..0] Sets the SCL Clock Low-period Count For Standard Speed            */
            uint32_t            : 16;
    } SCLLCNT_b;
  } ;
  
  union {
    __IOM uint32_t FSSCLHCNT;                   /*!< (@ 0x0000001C) FAST MODE SCL HIGH COUNT REGISTER                          */
    
    struct {
      __IOM uint32_t HCNT       : 16;           /*!< [15..0] Sets the SCL Clock High-period Count For Fast Speed               */
            uint32_t            : 16;
    } FSSCLHCNT_b;
  } ;
  
  union {
    __IOM uint32_t FSSCLLCNT;                   /*!< (@ 0x00000020) FAST MODE SCL LOW COUNT REGISTER                           */
    
    struct {
      __IOM uint32_t LCNT       : 16;           /*!< [15..0] Sets the SCL Clock Low-period Count For Fast Speed                */
            uint32_t            : 16;
    } FSSCLLCNT_b;
  } ;
  __IM  uint32_t  RESERVED1[2];
  
  union {
    __IM  uint32_t INTSTAT;                     /*!< (@ 0x0000002C) I2C INTERRUPT STATUS REGISTER                              */
    
    struct {
      __IM  uint32_t RXUNDER    : 1;            /*!< [0..0] RX_UNDER Interrupt                                                 */
      __IM  uint32_t RXOVER     : 1;            /*!< [1..1] RX_OVER Interrupt                                                  */
      __IM  uint32_t RXFULL     : 1;            /*!< [2..2] RX_FULL Interrupt                                                  */
      __IM  uint32_t TXOVER     : 1;            /*!< [3..3] TX_OVER Interrupt                                                  */
      __IM  uint32_t TXEMPTY    : 1;            /*!< [4..4] TX_EMPTY Interrupt                                                 */
      __IM  uint32_t RDREQ      : 1;            /*!< [5..5] RD_REQ interrupt                                                   */
      __IM  uint32_t TXABRT     : 1;            /*!< [6..6] TX_ABRT Interrupt                                                  */
      __IM  uint32_t RXDONE     : 1;            /*!< [7..7] RX_DONE Interrupt                                                  */
      __IM  uint32_t ACTIVITY   : 1;            /*!< [8..8] ACTIVITY Interrupt                                                 */
      __IM  uint32_t STOPDET    : 1;            /*!< [9..9] STOP_DET Interrupt                                                 */
      __IM  uint32_t STARTDET   : 1;            /*!< [10..10] START_DET Interrupt                                              */
      __IM  uint32_t GENCALL    : 1;            /*!< [11..11] GEN_CALL Interrupt                                               */
      __IM  uint32_t RESTARTDET : 1;            /*!< [12..12] RESTART_DET Interrupt                                            */
      __IM  uint32_t MSTONHOLD  : 1;            /*!< [13..13] MASTER_ON_HOLD Interrupt                                         */
      __IM  uint32_t SCLSTUCKLOW : 1;           /*!< [14..14] SCL_STUCK_AT_LOW Interrupt                                       */
      __IM  uint32_t WRREQ      : 1;            /*!< [15..15] WR_REQ Interrupt                                                 */
            uint32_t            : 16;
    } INTSTAT_b;
  } ;
  
  union {
    __IOM uint32_t INTMASK;                     /*!< (@ 0x00000030) INTERRUPT MASK REGISTER                                    */
    
    struct {
      __IOM uint32_t RXUNDERMSK : 1;            /*!< [0..0] RX_UNDER Interrupt Mask                                            */
      __IOM uint32_t RXOVERMSK  : 1;            /*!< [1..1] RX_OVER Interrupt Mask                                             */
      __IOM uint32_t RXFULLMSK  : 1;            /*!< [2..2] RX_FULL Interrupt Mask                                             */
      __IOM uint32_t TXOVERMSK  : 1;            /*!< [3..3] TX_OVER Interrupt Mask                                             */
      __IOM uint32_t TXEMPTYMSK : 1;            /*!< [4..4] TX_EMPTY Interrupt Mask                                            */
      __IOM uint32_t RDREQMSK   : 1;            /*!< [5..5] RD_REQ Interrupt Mask                                              */
      __IOM uint32_t TXABRTMSK  : 1;            /*!< [6..6] TX_ABRT Interrupt Mask                                             */
      __IOM uint32_t RXDONEMSK  : 1;            /*!< [7..7] RX_DONE Interrupt Mask                                             */
      __IOM uint32_t ACTIVITYMSK : 1;           /*!< [8..8] ACTIVITY Interrupt Mask                                            */
      __IOM uint32_t STOPDETMSK : 1;            /*!< [9..9] STOP_DET Interrupt Mask                                            */
      __IOM uint32_t STARTDETMSK : 1;           /*!< [10..10] START_DET Interrupt Mask                                         */
      __IOM uint32_t GENCALLMSK : 1;            /*!< [11..11] GEN_CALL Interrupt Mask                                          */
      __IOM uint32_t RESTARTDETMSK : 1;         /*!< [12..12] RESTART_DET Interrupt Mask                                       */
      __IOM uint32_t MSTONHOLDMSK : 1;          /*!< [13..13] MASTER_ON_HOLD Interrupt Mask                                    */
      __IOM uint32_t SCLSTUCKLOWMSK : 1;        /*!< [14..14] SCL_STUCK_AT_LOW Interrupt Mask                                  */
      __IOM uint32_t WRREQMSK   : 1;            /*!< [15..15] WR_REQ Interrupt Mask                                            */
            uint32_t            : 16;
    } INTMASK_b;
  } ;
  
  union {
    __IOM uint32_t RAWINTSTAT;                  /*!< (@ 0x00000034) INTERRUPT STATUS REGISTER                                  */
    
    struct {
      __IM  uint32_t RXUNDERRAW : 1;            /*!< [0..0] RX_UNDER Interrupt Status                                          */
      __IM  uint32_t RXOVERRAW  : 1;            /*!< [1..1] RX_UNDER Interrupt Status                                          */
      __IM  uint32_t RXFULLRAW  : 1;            /*!< [2..2] RX_UNDER Interrupt Status                                          */
      __IM  uint32_t TXOVERRAW  : 1;            /*!< [3..3] TX_OVER Interrupt Status                                           */
      __IM  uint32_t TXEMPTYRAW : 1;            /*!< [4..4] TX_EMPTY Interrupt Status                                          */
      __IM  uint32_t RDREQRAW   : 1;            /*!< [5..5] RD_REQ Interrupt Status                                            */
      __IM  uint32_t TXABRTRAW  : 1;            /*!< [6..6] TX_ABRT Interrupt Status                                           */
      __IM  uint32_t RXDONERAW  : 1;            /*!< [7..7] RX_DONE Interrupt Status                                           */
      __IM  uint32_t ACTIVITYRAW : 1;           /*!< [8..8] I2C Controller Activity                                            */
      __IM  uint32_t STOPDETRAW : 1;            /*!< [9..9] STOP_DET Interrupt Status                                          */
      __IM  uint32_t STARTDETRAW : 1;           /*!< [10..10] START_DET Interrupt Status                                       */
      __IM  uint32_t GENCALLRAW : 1;            /*!< [11..11] GEN_CALL Interrupt Status                                        */
      __IM  uint32_t RESTARTDETRAW : 1;         /*!< [12..12] RESTART_DET Interrupt Status                                     */
      __IM  uint32_t MSTONHOLDRAW : 1;          /*!< [13..13] MASTER_ON_HOLD Interrupt Status                                  */
      __IM  uint32_t SCLSTUCKLOWRAW : 1;        /*!< [14..14] SCL_STUCK_AT_LOW Interrupt Status                                */
      __IM  uint32_t WRREQRAW   : 1;            /*!< [15..15] WR_REQ Interrupt Status                                          */
            uint32_t            : 16;
    } RAWINTSTAT_b;
  } ;
  
  union {
    __IOM uint32_t RXTL;                        /*!< (@ 0x00000038) I2C RECEIVE FIFO THRESHOLD REGISTER                        */
    
    struct {
      __IOM uint32_t THRES      : 8;            /*!< [7..0] Receive FIFO Threshold Level                                       */
            uint32_t            : 24;
    } RXTL_b;
  } ;
  
  union {
    __IOM uint32_t TXTL;                        /*!< (@ 0x0000003C) I2C TRANSMIT FIFO THRESHOLD REGISTER                       */
    
    struct {
      __IOM uint32_t THRES      : 8;            /*!< [7..0] Transmit FIFO Threshold Level                                      */
            uint32_t            : 24;
    } TXTL_b;
  } ;
  __IM  uint32_t  CLRINT;                       /*!< (@ 0x00000040) CLEAR COMBINED AND INDIVIDUAL INTERRUPT REGISTER           */
  __IM  uint32_t  CLRRXUNDER;                   /*!< (@ 0x00000044) Clear RX_UNDER Interrupt                                   */
  __IM  uint32_t  CLRRXOVER;                    /*!< (@ 0x00000048) Clear RX_OVER Interrupt Register                           */
  __IM  uint32_t  CLRTXOVER;                    /*!< (@ 0x0000004C) Clear TX_OVER Interrupt Register                           */
  __IM  uint32_t  CLRRDREQ;                     /*!< (@ 0x00000050) Clear RD_REQ Interrupt Register                            */
  __IM  uint32_t  CLRTXABRT;                    /*!< (@ 0x00000054) Clear TX_ABRT Interrupt Register                           */
  __IM  uint32_t  CLRRXDONE;                    /*!< (@ 0x00000058) Clear RX_DONE Interrupt Register                           */
  __IM  uint32_t  CLRACTIVITY;                  /*!< (@ 0x0000005C) Clear ACTIVITY Interrupt Register                          */
  __IM  uint32_t  CLRSTOPDET;                   /*!< (@ 0x00000060) Clear STOP_DET Interrupt Register                          */
  __IM  uint32_t  CLRSTARTDET;                  /*!< (@ 0x00000064) Clear START_DET Interrupt Register                         */
  __IM  uint32_t  CLRGENCALL;                   /*!< (@ 0x00000068) Clear GEN_CALL Interrupt Register                          */
  
  union {
    __IOM uint32_t ENABLE;                      /*!< (@ 0x0000006C) ENABLE REGISTER                                            */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Controls Whether The I2C Controller Is Enabled                     */
      __IOM uint32_t ABORT      : 1;            /*!< [1..1] Initiates The Transfer Abort                                       */
      __IOM uint32_t TXCMDBLOCK : 1;            /*!< [2..2] Blocks The Transmission Of Data On I2c Bus Or Not                  */
      __IOM uint32_t SDARECOVEN : 1;            /*!< [3..3] Control Knob To Initiate The SDA Recovery Mechanism                */
            uint32_t            : 28;
    } ENABLE_b;
  } ;
  
  union {
    __IM  uint32_t STATUS;                      /*!< (@ 0x00000070) I2C STATUS Register                                        */
    
    struct {
      __IM  uint32_t ACTIVITY   : 1;            /*!< [0..0] I2C Activity Status                                                */
      __IM  uint32_t TFNF       : 1;            /*!< [1..1] Transmit FIFO Not Full                                             */
      __IM  uint32_t TFE        : 1;            /*!< [2..2] Transmit FIFO Completely Empty                                     */
      __IM  uint32_t RFNE       : 1;            /*!< [3..3] Receive FIFO Not Empty                                             */
      __IM  uint32_t RFF        : 1;            /*!< [4..4] Receive FIFO Completely Full                                       */
      __IM  uint32_t MSTACTIVITY : 1;           /*!< [5..5] Master FSM Activity Status                                         */
      __IM  uint32_t SLVACTIVITY : 1;           /*!< [6..6] Slave FSM Activity Status                                          */
      __IM  uint32_t MSTHOLDTXEMPTY : 1;        /*!< [7..7] Indicates The Bus Hold When The Master Holds The Bus
                                                     Because Of The Tx FIFO Being Empty                                        */
      __IM  uint32_t MSTHOLDRXFULL : 1;         /*!< [8..8] INdicates The Bus Hold In Master Mode Due To Rx FIFO
                                                     Is Full And Additional Byte Has Been Received                             */
      __IM  uint32_t SLVHOLDTXEMPTY : 1;        /*!< [9..9] Indicates The Bus Hold In Slave Mode Due To Rx FIFO Is
                                                     Full And Additional Byte Has Been Received                                */
      __IM  uint32_t SLVHOLDRXFULL : 1;         /*!< [10..10] Indicates The Bus Hold In Slave Mode Due To Rx FIFO
                                                     Is Full And Additional Byte Has Been Received                             */
      __IM  uint32_t SDANOTRECOV : 1;           /*!< [11..11] Indicates That SDA Stuck At Low Is Not Recovered After
                                                     The Recovery Mechanism                                                    */
            uint32_t            : 20;
    } STATUS_b;
  } ;
  __IOM uint32_t  TXFLR;                        /*!< (@ 0x00000074) I2C TRANSMIT FIFO LEVEL REGISTER                           */
  __IOM uint32_t  RXFLR;                        /*!< (@ 0x00000078) I2C RECEIVE FIFO LEVEL REGISTER                            */
  
  union {
    __IOM uint32_t SDAHOLD;                     /*!< (@ 0x0000007C) I2C SDA HOLD TIME LENGTH REGISTER                          */
    
    struct {
      __IOM uint32_t SDATXHOLD  : 16;           /*!< [15..0] Sets The Required SDA Hold Time In Units Of clk Period            */
      __IOM uint32_t SDARXHOLD  : 8;            /*!< [23..16] Sets the required SDA hold time in units of I2C clk
                                                     period                                                                    */
            uint32_t            : 8;
    } SDAHOLD_b;
  } ;
  
  union {
    __IOM uint32_t TXABRTSRC;                   /*!< (@ 0x00000080) I2C TRANSMIT ABORT SOURCE REGISTER                         */
    
    struct {
      __IM  uint32_t ADDR7BNAK  : 1;            /*!< [0..0] Indicates That The Master Is In 7-bit Addressing Mode
                                                     And The Address Sent Was Not Acknowledged By Any Slave                    */
      __IM  uint32_t ADDR10BNAK1 : 1;           /*!< [1..1] Indicates That The Master Is In 10-bit Address Mode And
                                                     The First 10-bit Address Byte Was Not Acknowledged By Any
                                                     Slave                                                                     */
      __IM  uint32_t ADDR10BNACK2 : 1;          /*!< [2..2] Indicates That The Master Is In 10-bit Address Mode And
                                                     The Second 10-bit Address Byte Was Not Acknowledged By
                                                     Any Slave                                                                 */
      __IM  uint32_t TXDATANAK  : 1;            /*!< [3..3] Indicates The Master-mode Only Bit. When Themaster Receives
                                                     An Acknowledgement For The Address                                        */
      __IM  uint32_t GCALLNAK   : 1;            /*!< [4..4] Indicates That I2C Controller In Master Mode Has Sent
                                                     A General Call And No Slave On The Bus Acknowledged The
                                                     General Call                                                              */
      __IM  uint32_t GCALLREAD  : 1;            /*!< [5..5] Indicates That Dw_apb_i2c In The Master Mode Has Sent
                                                     A General Call But The User Programmed The Byte Following
                                                     The General Call To Be A Read From The Bus                                */
      __IM  uint32_t HSACKDET   : 1;            /*!< [6..6] Indicates That The Master Is In High Speed Mode And The
                                                     High Speed Master Code Was Acknowledged                                   */
      __IM  uint32_t SBYTEACKET : 1;            /*!< [7..7] Indicates That The Master Has Sent A START Byte And The
                                                     START Byte Was Acknowledged                                               */
      __IM  uint32_t HSNORSTRT  : 1;            /*!< [8..8] Indicates That The Restart Is Disabled                             */
      __IM  uint32_t SBYTENORSTRT : 1;          /*!< [9..9] Indicates That The Restart Is Disabled                             */
      __IM  uint32_t ADDR10BRDNORSTRT : 1;      /*!< [10..10] Indicates That The Restart Is Disabled                           */
      __IM  uint32_t MASTERIDS  : 1;            /*!< [11..11] Indicates That The User Tries To Initiate A Master
                                                     Operation With The Master Mode Disabled                                   */
      __IM  uint32_t ARBLOST    : 1;            /*!< [12..12] This Field Specifies That The Master Has Lost Arbitration        */
      __IM  uint32_t SLVFLUSHTXFIFO : 1;        /*!< [13..13] This Field Specifies That The Slave Has Received A
                                                     Read Command And Some Data Exists In The TX FIFO                          */
      __IM  uint32_t SLVARBLOST : 1;            /*!< [14..14] This Field Indicates That A Slave Has Lost The Bus
                                                     While Transmitting Data To A Remote Master                                */
      __IM  uint32_t SLVRDINTX  : 1;            /*!< [15..15] When The Processor Side Responds To A Slave Mode Request
                                                     For Data To Be Transmitted To A Remote Master And User
                                                     Writes A 1 In Cmd (bit 8) Of Ic_data_cmd Register                         */
      __IM  uint32_t USRABRT    : 1;            /*!< [16..16] This Is A Master-mode-only Bit. Master Has Detected
                                                     The Transfer Abort                                                        */
      __IM  uint32_t SDASTUCKLOW : 1;           /*!< [17..17] This Is A Master-mode-only Bit. Master Detects The
                                                     SDA Stuck At Low                                                          */
      __IM  uint32_t DEVNAK     : 1;            /*!< [18..18] Master Is Initiating The DEVICE_ID Transfer And The
                                                     Device Id Sent Was Not Acknowledged By Any Slave                          */
      __IM  uint32_t DEVSLVADDRNAK : 1;         /*!< [19..19] Master Is Initiating The DEVICE_ID Transfer And The
                                                     Slave Address Sent Was Not Acknowledged By Any Slave                      */
      __IM  uint32_t DEVWRITE   : 1;            /*!< [20..20] Master Is Initiating The DEVICE_ID Transfer And The
                                                     Tx-FIFO Consists Of Write Commands                                        */
            uint32_t            : 2;
      __IM  uint32_t TXFLUSHCNT : 9;            /*!< [31..23] Indicates The Number Of Tx FIFO Data Commands Which
                                                     Are Flushed Due To TX_ABRT Interrupt                                      */
    } TXABRTSRC_b;
  } ;
  
  union {
    __IOM uint32_t SLVDATANACKONLY;             /*!< (@ 0x00000084) GENERATE SLAVE DATA NACK REGISTER                          */
    
    struct {
      __IOM uint32_t NAK        : 1;            /*!< [0..0] Generate Nack. This Nack Generation Only Occurs When
                                                     I2C Controller Is A Slave-receiver                                        */
            uint32_t            : 31;
    } SLVDATANACKONLY_b;
  } ;
  __IM  uint32_t  RESERVED2[3];
  
  union {
    __IOM uint32_t SDASETUP;                    /*!< (@ 0x00000094) I2C SDA SETUP REGISTER                                     */
    
    struct {
      __IOM uint32_t SETUP      : 8;            /*!< [7..0] SDA Setup (tSU:DAT)                                                */
            uint32_t            : 24;
    } SDASETUP_b;
  } ;
  
  union {
    __IOM uint32_t ACKGENCALL;                  /*!< (@ 0x00000098) I2C ACK GENERAL CALL REGISTER                              */
    
    struct {
      __IOM uint32_t ACK        : 1;            /*!< [0..0] ACK General Call                                                   */
            uint32_t            : 31;
    } ACKGENCALL_b;
  } ;
  
  union {
    __IM  uint32_t ENSTS;                       /*!< (@ 0x0000009C) I2C ENABLE STATUS REGISTER                                 */
    
    struct {
      __IM  uint32_t ENABLE     : 1;            /*!< [0..0] IC Enable Status                                                   */
      __IM  uint32_t SLVDISBUSY : 1;            /*!< [1..1] Slave Disabled While Busy (Transmit, Receive)                      */
      __IM  uint32_t SLVRXDATALOST : 1;         /*!< [2..2] Slave Received Data Lost                                           */
            uint32_t            : 29;
    } ENSTS_b;
  } ;
  
  union {
    __IOM uint32_t SPKLEN;                      /*!< (@ 0x000000A0) I2C SS, FS OR FM+ RO UFM SPIKE SUPPRESSION LIMIT           */
    
    struct {
      __IOM uint32_t LEN        : 8;            /*!< [7..0] Sets the Duration, Measured In Clk Cycle                           */
            uint32_t            : 24;
    } SPKLEN_b;
  } ;
  __IM  uint32_t  RESERVED3;
  __IOM uint32_t  CLRRESTARTDET;                /*!< (@ 0x000000A8) CLEAR RESTART_DET INTERRUPT REGISTER                       */
  __IOM uint32_t  SCLLOWTO;                     /*!< (@ 0x000000AC) I2C SCL STUCK AT LOW TIMEOUT REGISTER                      */
  __IOM uint32_t  SDALOWTO;                     /*!< (@ 0x000000B0) I2C SDA STUCK AT LOW TIMEOUT REGISTER                      */
  __IOM uint32_t  CLRSCLSTUCKDET;               /*!< (@ 0x000000B4) CLEAR SCL STUCK AT LOW DETECT INTERRUPT REGISTER           */
} I2C_Type;                                     /*!< Size = 184 (0xb8)                                                         */



/* =========================================================================================================================== */
/* ================                                           I3C0                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief I3C Controller (I3C0)
  */

typedef struct {                                /*!< (@ 0x40003000) I3C0 Structure                                             */
  
  union {
    __IOM uint32_t CCR;                         /*!< (@ 0x00000000) CONFIGURATION CONTROL REGISTER                             */
    
    struct {
      __IOM uint32_t MODE       : 1;            /*!< [0..0] Mode Select                                                        */
      __IOM uint32_t PCR        : 1;            /*!< [1..1] Primary Control                                                    */
      __IOM uint32_t SCTRL      : 1;            /*!< [2..2] Secondary Control Mode                                             */
      __IOM uint32_t I3CDIS     : 1;            /*!< [3..3] I3C Disable                                                        */
            uint32_t            : 5;
      __IOM uint32_t INTRDIS    : 1;            /*!< [9..9] Interrupt Disable                                                  */
            uint32_t            : 22;
    } CCR_b;
  } ;
  
  union {
    __IOM uint32_t IPCR;                        /*!< (@ 0x00000004) IXC PROTOCOL CONTROL REGISTER                              */
    
    struct {
      __IM  uint32_t CSTALL     : 1;            /*!< [0..0] SCL Stall Enable                                                   */
      __IOM uint32_t I2CEN      : 1;            /*!< [1..1] I2C Bus Enable                                                     */
      __IOM uint32_t HDEXDIS    : 1;            /*!< [2..2] HDR Exit Pattern Disable                                           */
      __IOM uint32_t FTOD       : 1;            /*!< [3..3] Force Timing = OD Enable                                           */
      __IOM uint32_t HDERPP     : 1;            /*!< [4..4] Header Address turn to PP Timing Enable                            */
      __IOM uint32_t NAKDIS     : 1;            /*!< [5..5] RX NACK Disable                                                    */
      __IOM uint32_t CEDIS      : 1;            /*!< [6..6] CTRL Error Disable                                                 */
      __IOM uint32_t TEDIS      : 1;            /*!< [7..7] TAGT Error Disable                                                 */
            uint32_t            : 8;
      __IOM uint32_t RTRYCNT    : 2;            /*!< [17..16] Read Retry Count                                                 */
            uint32_t            : 2;
      __IOM uint32_t INTELGA    : 1;            /*!< [20..20] INTEL Spec. Extend GA Enable                                     */
      __IOM uint32_t INTELCCC   : 1;            /*!< [21..21] INTEL Spec. Extend CCC Enable                                    */
      __IOM uint32_t INTELAS    : 1;            /*!< [22..22] INTEL Spec. Extend AS Enable                                     */
      __IOM uint32_t INTELDDR   : 1;            /*!< [23..23] INTEL Spec. Extend DDR Enable                                    */
      __IOM uint32_t INTEL      : 1;            /*!< [24..24] INTEL Spec. Enable                                               */
      __IOM uint32_t PARDIS     : 1;            /*!< [25..25] Parity Disable                                                   */
      __IOM uint32_t BLEN       : 1;            /*!< [26..26] BL Enable                                                        */
      __IOM uint32_t BCLR       : 1;            /*!< [27..27] Bus Clear Enable                                                 */
      __IOM uint32_t BRST       : 1;            /*!< [28..28] Bus Reset Enable                                                 */
      __IOM uint32_t PEC        : 1;            /*!< [29..29] PEC CRC Enable                                                   */
      __IOM uint32_t JESD       : 1;            /*!< [30..30] JESD Spec. Enable                                                */
            uint32_t            : 1;
    } IPCR_b;
  } ;
  
  union {
    __IOM uint32_t IOCR;                        /*!< (@ 0x00000008) I/O CONFIGURATION CONTROL REGISTER                         */
    
    struct {
            uint32_t            : 3;
      __IOM uint32_t SCLOD      : 1;            /*!< [3..3] Force SCL in Open-Drain Status                                     */
            uint32_t            : 8;
      __IOM uint32_t SDADLY     : 4;            /*!< [15..12] SDA Deslay Count                                                 */
            uint32_t            : 11;
      __IOM uint32_t BUSYTO     : 1;            /*!< [27..27] Turn to Idle when SCL/SDA hold for tIDLE                         */
      __IOM uint32_t SECING     : 1;            /*!< [28..28] Indicated for I3C SEC HP Flow Running                            */
      __IOM uint32_t SECERR     : 1;            /*!< [29..29] Indicated for I3C SEC HP Flow Occured an Error                   */
      __IOM uint32_t CLPBK      : 1;            /*!< [30..30] SCL LPBK Enable                                                  */
      __IOM uint32_t DLPBK      : 1;            /*!< [31..31] SDA LPBK Enable                                                  */
    } IOCR_b;
  } ;
  __IM  uint32_t  RESERVED;
  
  union {
    __IOM uint32_t CSTR;                        /*!< (@ 0x00000010) CONTROL STRUCTURE REGISTER                                 */
    
    struct {
      __IOM uint32_t START      : 1;            /*!< [0..0] Control structure flow start                                       */
      __IOM uint32_t CDPIN      : 1;            /*!< [1..1] CD Pin start                                                       */
      __IOM uint32_t ASTART     : 1;            /*!< [2..2] Control structure async mode 0 flow start                          */
            uint32_t            : 25;
      __IOM uint32_t RFLUSH     : 1;            /*!< [28..28] Flush RXDA/RCMD FIFO                                             */
      __IOM uint32_t TFLUSH     : 1;            /*!< [29..29] Flush TXDA FIFO                                                  */
      __IOM uint32_t BFLUSH     : 1;            /*!< [30..30] Flush BYFM FIFO                                                  */
      __IOM uint32_t FLUSH      : 1;            /*!< [31..31] Flush BYFM/TXDA/RXDA and Recovery IXC Status                     */
    } CSTR_b;
  } ;
  
  union {
    __IOM uint32_t CBDR;                        /*!< (@ 0x00000014) SCL BAUD RATE REGISTER                                     */
    
    struct {
      __IOM uint32_t BDROD      : 16;           /*!< [15..0] Baud rate for OD                                                  */
      __IOM uint32_t BDRPP      : 8;            /*!< [23..16] Baud rate for PP                                                 */
      __IOM uint32_t BDRFST     : 8;            /*!< [31..24] Baud rate for the first header address                           */
    } CBDR_b;
  } ;
  
  union {
    __IOM uint32_t CSR;                         /*!< (@ 0x00000018) CURRENT STATUS REGISTER                                    */
    
    struct {
      __IM  uint32_t BUSY       : 1;            /*!< [0..0] BUSY                                                               */
      __IM  uint32_t BBUSYN     : 1;            /*!< [1..1] BUS IDLE                                                           */
      __IM  uint32_t ABUSY      : 1;            /*!< [2..2] ASYNC MODE 0 BUSY                                                  */
      __IOM uint32_t AMODE      : 1;            /*!< [3..3] IN ASYNC MODE 0                                                    */
      __IM  uint32_t RXF        : 1;            /*!< [4..4] RX DATA FULL                                                       */
      __IM  uint32_t RXNE       : 1;            /*!< [5..5] RX DATA NOT EMPTY                                                  */
      __IM  uint32_t RXDP       : 1;            /*!< [6..6] RX DATA DROP                                                       */
            uint32_t            : 1;
      __IM  uint32_t TXE        : 1;            /*!< [8..8] TXDA EMPTY                                                         */
      __IM  uint32_t TXF        : 1;            /*!< [9..9] TXDA FULL                                                          */
      __IM  uint32_t TXUN       : 1;            /*!< [10..10] TXDA UBDERFLOW                                                   */
            uint32_t            : 1;
      __IM  uint32_t RXNAK      : 1;            /*!< [12..12] RECIEVED NACK                                                    */
      __IM  uint32_t BCE        : 1;            /*!< [13..13] BYFM CMD FIFO EMPTY                                              */
      __IM  uint32_t BCF        : 1;            /*!< [14..14] BYFM CMD FIFO FULL                                               */
      __IM  uint32_t CCC        : 1;            /*!< [15..15] RECEIVE A CCC CMD IN TARGET MODE                                 */
      __IM  uint32_t DDR        : 1;            /*!< [16..16] HDR-DDR MODE                                                     */
            uint32_t            : 15;
    } CSR_b;
  } ;
  
  union {
    __IM  uint32_t ISR;                         /*!< (@ 0x0000001C) INTERRUPT STATUS REGISTER                                  */
    
    struct {
      __IM  uint32_t DONE       : 1;            /*!< [0..0] I3C/I2C Transation is Done                                         */
      __IM  uint32_t RXNE       : 1;            /*!< [1..1] RXDA Not Empty                                                     */
      __IM  uint32_t TXF        : 1;            /*!< [2..2] TXDA Full                                                          */
      __IM  uint32_t RXDP       : 1;            /*!< [3..3] RX Data Drop Occur                                                 */
      __IM  uint32_t ECOM       : 1;            /*!< [4..4] Early Complete Transaction                                         */
      __IM  uint32_t BYFME      : 1;            /*!< [5..5] BYFME Error                                                        */
      __IM  uint32_t PARE       : 1;            /*!< [6..6] PAR-bit Error                                                      */
      __IM  uint32_t RXNAK      : 1;            /*!< [7..7] Receieve NACK                                                      */
      __IM  uint32_t RXIBI      : 1;            /*!< [8..8] Receieve IBI REQ                                                   */
      __IM  uint32_t RXHJI      : 1;            /*!< [9..9] Receieve Hot Join REQ                                              */
      __IM  uint32_t RXSEC      : 1;            /*!< [10..10] Receieve Role REQ / Receieve CCC GETACCCR                        */
      __IM  uint32_t WETM       : 1;            /*!< [11..11] Write Early Terminate Occur                                      */
      __IM  uint32_t RETM       : 1;            /*!< [12..12] Read Early Terminate Occur                                       */
      __IM  uint32_t PECE       : 1;            /*!< [13..13] PEC CRC Error                                                    */
      __IM  uint32_t RDNG       : 1;            /*!< [14..14] Read RXDA word unaligned                                         */
      __IM  uint32_t DAF        : 1;            /*!< [15..15] SDA Arbitration Fail                                             */
      __IM  uint32_t BRST       : 1;            /*!< [16..16] Bus Reset                                                        */
      __IM  uint32_t BCLR       : 1;            /*!< [17..17] Bus Clear                                                        */
      __IM  uint32_t STALLTO    : 1;            /*!< [18..18] SCL Stall Timeout in CTRL Mode / BUS Buy Timeout in
                                                     TAGT Mode                                                                 */
      __IM  uint32_t DDRE       : 1;            /*!< [19..19] DDR Preamble / Parity / CRC Error                                */
      __IM  uint32_t CE0IS      : 1;            /*!< [20..20] SDR Controller Error Type 0                                      */
            uint32_t            : 1;
      __IM  uint32_t CE2IS      : 1;            /*!< [22..22] SDR Controller Error Type 2                                      */
      __IM  uint32_t CE3IS      : 1;            /*!< [23..23] SDR Controller Error Type 3                                      */
      __IM  uint32_t TE0IS      : 1;            /*!< [24..24] SDR Target Error Type 0                                          */
      __IM  uint32_t TE1IS      : 1;            /*!< [25..25] SDR Target Error Type 1                                          */
      __IM  uint32_t TE2IS      : 1;            /*!< [26..26] SDR Target Error Type 2                                          */
      __IM  uint32_t TE3IS      : 1;            /*!< [27..27] SDR Target Error Type 3                                          */
      __IM  uint32_t TE4IS      : 1;            /*!< [28..28] SDR Target Error Type 4                                          */
      __IM  uint32_t TE5IS      : 1;            /*!< [29..29] SDR target error type 5                                          */
            uint32_t            : 2;
    } ISR_b;
  } ;
  
  union {
    __IOM uint32_t RISR;                        /*!< (@ 0x00000020) INTERRUPT RAW STATUS REGISTER                              */
    
    struct {
      __IOM uint32_t DONE       : 1;            /*!< [0..0] I3C/I2C Transation is Done                                         */
      __IOM uint32_t RXNE       : 1;            /*!< [1..1] RXDA Not Empty                                                     */
      __IOM uint32_t TXF        : 1;            /*!< [2..2] TXDA Full                                                          */
      __IOM uint32_t RXDP       : 1;            /*!< [3..3] RX Data Drop Occur                                                 */
      __IOM uint32_t ECOM       : 1;            /*!< [4..4] Early Complete Transaction                                         */
      __IOM uint32_t BYFME      : 1;            /*!< [5..5] BYFME Error                                                        */
      __IOM uint32_t PARE       : 1;            /*!< [6..6] PAR-bit Error                                                      */
      __IOM uint32_t RXNAK      : 1;            /*!< [7..7] Receieve NACK                                                      */
      __IOM uint32_t RXIBI      : 1;            /*!< [8..8] Receieve IBI REQ                                                   */
      __IOM uint32_t RXHJI      : 1;            /*!< [9..9] Receieve Hot Join REQ                                              */
      __IOM uint32_t RXSEC      : 1;            /*!< [10..10] Receieve Role REQ / Receieve CCC GETACCCR                        */
      __IOM uint32_t WETM       : 1;            /*!< [11..11] Write Early Terminate Occur                                      */
      __IOM uint32_t RETM       : 1;            /*!< [12..12] Read Early Terminate Occur                                       */
      __IOM uint32_t PECE       : 1;            /*!< [13..13] PEC CRC Error                                                    */
      __IOM uint32_t RDNG       : 1;            /*!< [14..14] Read RXDA word unaligned                                         */
      __IOM uint32_t DAF        : 1;            /*!< [15..15] SDA Arbitration Fail                                             */
      __IOM uint32_t BRST       : 1;            /*!< [16..16] Bus Reset                                                        */
      __IOM uint32_t BCLR       : 1;            /*!< [17..17] Bus Clear                                                        */
      __IOM uint32_t STALLTO    : 1;            /*!< [18..18] SCL Stall Timeout in CTRL Mode / BUS Buy Timeout in
                                                     TAGT Mode                                                                 */
      __IOM uint32_t DDRE       : 1;            /*!< [19..19] DDR Preamble / Parity / CRC Error                                */
      __IOM uint32_t CE0IS      : 1;            /*!< [20..20] SDR Controller Error Type 0                                      */
            uint32_t            : 1;
      __IOM uint32_t CE2IS      : 1;            /*!< [22..22] SDR Controller Error Type 2                                      */
      __IOM uint32_t CE3IS      : 1;            /*!< [23..23] SDR Controller Error Type 3                                      */
      __IOM uint32_t TE0IS      : 1;            /*!< [24..24] SDR Target Error Type 0                                          */
      __IOM uint32_t TE1IS      : 1;            /*!< [25..25] SDR Target Error Type 1                                          */
      __IOM uint32_t TE2IS      : 1;            /*!< [26..26] SDR Target Error Type 2                                          */
      __IOM uint32_t TE3IS      : 1;            /*!< [27..27] SDR Target Error Type 3                                          */
      __IOM uint32_t TE4IS      : 1;            /*!< [28..28] SDR Target Error Type 4                                          */
      __IOM uint32_t TE5IS      : 1;            /*!< [29..29] SDR target error type 5                                          */
            uint32_t            : 2;
    } RISR_b;
  } ;
  
  union {
    __IOM uint32_t IER;                         /*!< (@ 0x00000024) INTERRUPT ENABLE CONTROL REGISTER                          */
    
    struct {
      __IOM uint32_t DONE       : 1;            /*!< [0..0] I3C/I2C Transation is Done                                         */
      __IOM uint32_t RXNE       : 1;            /*!< [1..1] RXDA Not Empty                                                     */
      __IOM uint32_t TXF        : 1;            /*!< [2..2] TXDA Full                                                          */
      __IOM uint32_t RXDP       : 1;            /*!< [3..3] RX Data Drop Occur                                                 */
      __IOM uint32_t ECOM       : 1;            /*!< [4..4] Early Complete Transaction                                         */
      __IOM uint32_t BYFME      : 1;            /*!< [5..5] BYFME Error                                                        */
      __IOM uint32_t PARE       : 1;            /*!< [6..6] PAR-bit Error                                                      */
      __IOM uint32_t RXNAK      : 1;            /*!< [7..7] Receieve NACK                                                      */
      __IOM uint32_t RXIBI      : 1;            /*!< [8..8] Receieve IBI REQ                                                   */
      __IOM uint32_t RXHJI      : 1;            /*!< [9..9] Receieve Hot Join REQ                                              */
      __IOM uint32_t RXSEC      : 1;            /*!< [10..10] Receieve Role REQ / Receieve CCC GETACCCR                        */
      __IOM uint32_t WETM       : 1;            /*!< [11..11] Write Early Terminate Occur                                      */
      __IOM uint32_t RETM       : 1;            /*!< [12..12] Read Early Terminate Occur                                       */
      __IOM uint32_t PECE       : 1;            /*!< [13..13] PEC CRC Error                                                    */
      __IOM uint32_t RDNG       : 1;            /*!< [14..14] Read RXDA word unaligned                                         */
      __IOM uint32_t DAF        : 1;            /*!< [15..15] SDA Arbitration Fail                                             */
      __IOM uint32_t BRST       : 1;            /*!< [16..16] Bus Reset                                                        */
      __IOM uint32_t BCLR       : 1;            /*!< [17..17] Bus Clear                                                        */
      __IOM uint32_t STALLTO    : 1;            /*!< [18..18] SCL Stall Timeout in CTRL Mode / BUS Buy Timeout in
                                                     TAGT Mode                                                                 */
      __IOM uint32_t DDRE       : 1;            /*!< [19..19] DDR Preamble / Parity / CRC Error                                */
      __IOM uint32_t CE0IS      : 1;            /*!< [20..20] SDR Controller Error Type 0                                      */
            uint32_t            : 1;
      __IOM uint32_t CE2IS      : 1;            /*!< [22..22] SDR Controller Error Type 2                                      */
      __IOM uint32_t CE3IS      : 1;            /*!< [23..23] SDR Controller Error Type 3                                      */
      __IOM uint32_t TE0IS      : 1;            /*!< [24..24] SDR Target Error Type 0                                          */
      __IOM uint32_t TE1IS      : 1;            /*!< [25..25] SDR Target Error Type 1                                          */
      __IOM uint32_t TE2IS      : 1;            /*!< [26..26] SDR Target Error Type 2                                          */
      __IOM uint32_t TE3IS      : 1;            /*!< [27..27] SDR Target Error Type 3                                          */
      __IOM uint32_t TE4IS      : 1;            /*!< [28..28] SDR Target Error Type 4                                          */
      __IOM uint32_t TE5IS      : 1;            /*!< [29..29] SDR target error type 5                                          */
            uint32_t            : 2;
    } IER_b;
  } ;
  
  union {
    __IOM uint32_t ICR;                         /*!< (@ 0x00000028) INTERRUPT CLEAR REGISTER                                   */
    
    struct {
      __IOM uint32_t DONE       : 1;            /*!< [0..0] I3C/I2C Transation is Done                                         */
      __IOM uint32_t RXNE       : 1;            /*!< [1..1] RXDA Not Empty                                                     */
      __IOM uint32_t TXF        : 1;            /*!< [2..2] TXDA Full                                                          */
      __IOM uint32_t RXDP       : 1;            /*!< [3..3] RX Data Drop Occur                                                 */
      __IOM uint32_t ECOM       : 1;            /*!< [4..4] Early Complete Transaction                                         */
      __IOM uint32_t BYFME      : 1;            /*!< [5..5] BYFME Error                                                        */
      __IOM uint32_t PARE       : 1;            /*!< [6..6] PAR-bit Error                                                      */
      __IOM uint32_t RXNAK      : 1;            /*!< [7..7] Receieve NACK                                                      */
      __IOM uint32_t RXIBI      : 1;            /*!< [8..8] Receieve IBI REQ                                                   */
      __IOM uint32_t RXHJI      : 1;            /*!< [9..9] Receieve Hot Join REQ                                              */
      __IOM uint32_t RXSEC      : 1;            /*!< [10..10] Receieve Role REQ / Receieve CCC GETACCCR                        */
      __IOM uint32_t WETM       : 1;            /*!< [11..11] Write Early Terminate Occur                                      */
      __IOM uint32_t RETM       : 1;            /*!< [12..12] Read Early Terminate Occur                                       */
      __IOM uint32_t PECE       : 1;            /*!< [13..13] PEC CRC Error                                                    */
      __IOM uint32_t RDNG       : 1;            /*!< [14..14] Read RXDA word unaligned                                         */
      __IOM uint32_t DAF        : 1;            /*!< [15..15] SDA Arbitration Fail                                             */
      __IOM uint32_t BRST       : 1;            /*!< [16..16] Bus Reset                                                        */
      __IOM uint32_t BCLR       : 1;            /*!< [17..17] Bus Clear                                                        */
      __IOM uint32_t STALLTO    : 1;            /*!< [18..18] SCL Stall Timeout in CTRL Mode / BUS Buy Timeout in
                                                     TAGT Mode                                                                 */
      __IOM uint32_t DDRE       : 1;            /*!< [19..19] DDR Preamble / Parity / CRC Error                                */
      __IOM uint32_t CE0IS      : 1;            /*!< [20..20] SDR Controller Error Type 0                                      */
            uint32_t            : 1;
      __IOM uint32_t CE2IS      : 1;            /*!< [22..22] SDR Controller Error Type 2                                      */
      __IOM uint32_t CE3IS      : 1;            /*!< [23..23] SDR Controller Error Type 3                                      */
      __IOM uint32_t TE0IS      : 1;            /*!< [24..24] SDR Target Error Type 0                                          */
      __IOM uint32_t TE1IS      : 1;            /*!< [25..25] SDR Target Error Type 1                                          */
      __IOM uint32_t TE2IS      : 1;            /*!< [26..26] SDR Target Error Type 2                                          */
      __IOM uint32_t TE3IS      : 1;            /*!< [27..27] SDR Target Error Type 3                                          */
      __IOM uint32_t TE4IS      : 1;            /*!< [28..28] SDR Target Error Type 4                                          */
      __IOM uint32_t TE5IS      : 1;            /*!< [29..29] SDR target error type 5                                          */
            uint32_t            : 2;
    } ICR_b;
  } ;
  
  union {
    __IOM uint32_t TRINF;                       /*!< (@ 0x0000002C) TRANSACTION INFORMATION REGISTER                           */
    
    struct {
      __IM  uint32_t DA         : 7;            /*!< [6..0] HDR-DDR CMD Code / IBI or HJ or SEC REQ's DA for CTRL              */
      __IM  uint32_t RDA        : 1;            /*!< [7..7] HDR-DDR REQ for TAGT / IBI or HJ or SEC REQ for CTRL               */
      __IM  uint32_t TXNSNUM    : 6;            /*!< [13..8] Transacation for ROLE CTRL                                        */
            uint32_t            : 1;
      __IM  uint32_t RXCCC      : 1;            /*!< [15..15] REVEIVE A CCC CMD for TAGT                                       */
      __IM  uint32_t CCCDEF     : 8;            /*!< [23..16] CCC.DEFINING for TAGT                                            */
      __IM  uint32_t RXINF      : 8;            /*!< [31..24] CCC_CMD for TAGT / IBI_MDB for CTRL                              */
    } TRINF_b;
  } ;
  
  union {
    __IOM uint32_t TBCR;                        /*!< (@ 0x00000030) BUS TIMING REGISTER                                        */
    
    struct {
      __IOM uint32_t TBUF       : 12;           /*!< [11..0] Bus Free Timing (tBUF) Count                                      */
      __IOM uint32_t TIDLE      : 20;           /*!< [31..12] Bus Idle Timing (tIDLE) Count                                    */
    } TBCR_b;
  } ;
  
  union {
    __IOM uint32_t TINIT;                       /*!< (@ 0x00000034) INITIAL TIMING REGISTER                                    */
    
    struct {
      __IOM uint32_t TAVAL      : 20;           /*!< [19..0] Bus Available Timing (tAVAL) Count                                */
      __IOM uint32_t THIGHOD    : 12;           /*!< [31..20] SCL High Count for OD Timing                                     */
    } TINIT_b;
  } ;
  
  union {
    __IOM uint32_t TSSR0;                       /*!< (@ 0x00000038) TIMING START REGISTER 0                                    */
    
    struct {
      __IOM uint32_t TSUSTA     : 11;           /*!< [10..0] tSU_STA Timing Count                                              */
      __IOM uint32_t THDSTA     : 11;           /*!< [21..11] tHD_STA Timing Count                                             */
      __IOM uint32_t TDS        : 2;            /*!< [23..22] tDS Timing Count                                                 */
      __IOM uint32_t TCAS       : 8;            /*!< [31..24] tCAS Timing Count                                                */
    } TSSR0_b;
  } ;
  
  union {
    __IOM uint32_t TSSR1;                       /*!< (@ 0x0000003C) TIMING STOP REGISTER 1                                     */
    
    struct {
      __IOM uint32_t TSUSTO     : 12;           /*!< [11..0] tSU_STO Timing Count                                              */
            uint32_t            : 20;
    } TSSR1_b;
  } ;
  __IM  uint32_t  RESERVED1[4];
  
  union {
    __IOM uint32_t TDLYR0;                      /*!< (@ 0x00000050) TIMING DELAY REGISTER 0                                    */
    
    struct {
      __IOM uint32_t CDLY       : 8;            /*!< [7..0] SCL Receieve Delay in PP mode                                      */
      __IOM uint32_t PDLY       : 8;            /*!< [15..8] SDR Receieve Delay in PP mode                                     */
      __IOM uint32_t ODLY       : 14;           /*!< [29..16] SDR Receieve Delay in OD mode                                    */
      __IOM uint32_t NINEDGE    : 1;            /*!< [30..30] SDR RXDA Capture Edge According to CR_RDGE                       */
      __IOM uint32_t RDGE       : 1;            /*!< [31..31] SDR RXDA Capture Edge when cr_9dge Set                           */
    } TDLYR0_b;
  } ;
  
  union {
    __IOM uint32_t TDLYR1;                      /*!< (@ 0x00000054) TIMING DELAY REGISTER 1                                    */
    
    struct {
      __IOM uint32_t PREDLY     : 8;            /*!< [7..0] SCL Low Delay Time before Preamble Phase in DDR Mode               */
      __IOM uint32_t DDRPDLY    : 7;            /*!< [14..8] SDR Receieve Delay in DDR mode                                    */
      __IOM uint32_t DDRRDGE    : 1;            /*!< [15..15] Capture Point in DDR mode                                        */
      __IOM uint32_t ADLY       : 14;           /*!< [29..16] SDR RX Delay for 9th Header/Data phase in OD mode                */
            uint32_t            : 1;
      __IOM uint32_t RHDY       : 1;            /*!< [31..31] SDR RXDA Capture High Edge Delay Enable                          */
    } TDLYR1_b;
  } ;
  
  union {
    __IOM uint32_t TRST;                        /*!< (@ 0x00000058) TIMING BUS RESET REGISTER                                  */
    
    struct {
      __IOM uint32_t BRST       : 20;           /*!< [19..0] BUS Reset Timing                                                  */
            uint32_t            : 12;
    } TRST_b;
  } ;
  __IM  uint32_t  RESERVED2[5];
  __IM  uint32_t  TXFL;                         /*!< (@ 0x00000070) TXDA FIFO Level                                            */
  __IM  uint32_t  RXFL;                         /*!< (@ 0x00000074) RXDA FIFO Level                                            */
  __IM  uint32_t  RESERVED3[2];
  
  union {
    __IOM uint32_t EC;                          /*!< (@ 0x00000080) I3C TARGET EVENT BYTE REGISTER                             */
    
    struct {
      __IOM uint32_t INT        : 1;            /*!< [0..0] INTERRUPT BIT                                                      */
      __IOM uint32_t CR         : 1;            /*!< [1..1] CONTROL REGISTER BIT                                               */
            uint32_t            : 1;
      __IOM uint32_t HJ         : 1;            /*!< [3..3] HOT JOIN                                                           */
            uint32_t            : 19;
      __IOM uint32_t MPID       : 1;            /*!< [23..23] I3C ID From CR_PIDH ADN CR_PIDL                                  */
      __IOM uint32_t IPLS       : 8;            /*!< [31..24] IBI PAYLOAD SIZE                                                 */
    } EC_b;
  } ;
  
  union {
    __IOM uint32_t AS;                          /*!< (@ 0x00000084) ACTIVITY STATE REGISTER                                    */
    
    struct {
            uint32_t            : 24;
      __IOM uint32_t AS         : 8;            /*!< [31..24] IBI PAYLOAD SIZE                                                 */
    } AS_b;
  } ;
  
  union {
    __IOM uint32_t MXL;                         /*!< (@ 0x00000088) MAX R/W LENGTH REGISTER                                    */
    
    struct {
      __IOM uint32_t MWL        : 16;           /*!< [15..0] Max WRITE Length for SETMWL/GETMWL                                */
      __IOM uint32_t MRL        : 16;           /*!< [31..16] Max Read Length for SETMRL/GETMRL                                */
    } MXL_b;
  } ;
  
  union {
    __IOM uint32_t PIDL;                        /*!< (@ 0x0000008C) PIDL REGISTER                                              */
    
    struct {
      __IOM uint32_t DCR        : 8;            /*!< [7..0] I3C.DCR                                                            */
      __IOM uint32_t BCR        : 8;            /*!< [15..8] I3C.BCR                                                           */
      __IOM uint32_t PIDI       : 16;           /*!< [31..16] I3C.PID[15:0]                                                    */
    } PIDL_b;
  } ;
  __IOM uint32_t  PIDH;                         /*!< (@ 0x00000090) PIDH REGISTER                                              */
  
  union {
    __IOM uint32_t DSA;                         /*!< (@ 0x00000094) DYNAMIC/STATIC ADDRESS REGISTER                            */
    
    struct {
      __IM  uint32_t DYNADDR    : 7;            /*!< [6..0] DYNAMIC ADDRESS                                                    */
      __IOM uint32_t DA1RDIS    : 1;            /*!< [7..7] SETDASA Format 2                                                   */
      __IOM uint32_t STCADDR    : 10;           /*!< [17..8] Static Address                                                    */
            uint32_t            : 12;
      __IM  uint32_t DAAING     : 1;            /*!< [30..30] I3C Dynamic Address Assign Flow is Running                       */
      __IM  uint32_t DAADONE    : 1;            /*!< [31..31] I3C Dynamic Address Assign Flow is Done                          */
    } DSA_b;
  } ;
  
  union {
    __IOM uint32_t CCCST;                       /*!< (@ 0x00000098) DEVICE STATUS (GETSTATUS) REGISTER                         */
    
    struct {
      __IOM uint32_t STATUS     : 16;           /*!< [15..0] CCC: GETSTATUS                                                    */
            uint32_t            : 16;
    } CCCST_b;
  } ;
  
  union {
    __IOM uint32_t MXDS;                        /*!< (@ 0x0000009C) MAX DATA SPEED REGISTER                                    */
    
    struct {
      __IOM uint32_t MXR        : 8;            /*!< [7..0] Reply Value by CCC: GETMXDSS                                       */
      __IOM uint32_t MXW        : 8;            /*!< [15..8] Transaction Value by CCC: GETMXDS                                 */
            uint32_t            : 16;
    } MXDS_b;
  } ;
  
  union {
    __IOM uint32_t BUSC;                        /*!< (@ 0x000000A0) BUS CONTEXT REGISTER                                       */
    
    struct {
      __IM  uint32_t BUSCON     : 8;            /*!< [7..0] Bus Context                                                        */
            uint32_t            : 16;
      __IM  uint32_t HID        : 8;            /*!< [31..24] CCC.SETHID                                                       */
    } BUSC_b;
  } ;
  
  union {
    __IOM uint32_t GPA;                         /*!< (@ 0x000000A4) GROUP ADDRESS REGISTER                                     */
    
    struct {
      __IM  uint32_t GPA0       : 7;            /*!< [6..0] Group Address 0 for TAGT Mode                                      */
      __IM  uint32_t GPA0A      : 1;            /*!< [7..7] Group Address 0 Assigned                                           */
      __IM  uint32_t GPA1       : 7;            /*!< [14..8] Group Address 1 for TAGT Mode                                     */
      __IM  uint32_t GPA1A      : 1;            /*!< [15..15] Group Address 1 Assigned                                         */
      __IM  uint32_t GPA2       : 7;            /*!< [22..16] Group Address 2 for TAGT Mode                                    */
      __IM  uint32_t GPA2A      : 1;            /*!< [23..23] Group Address 2 Assigned                                         */
            uint32_t            : 8;
    } GPA_b;
  } ;
  
  union {
    __IOM uint32_t CASYN;                       /*!< (@ 0x000000A8) CONTROLLER ASYN TIMING REGISTER                            */
    
    struct {
      __IM  uint32_t CREF       : 16;           /*!< [15..0] IBI to First SCL Positive after IBI_ACK                           */
      __IM  uint32_t CC2        : 8;            /*!< [23..16] First SCL Positive Edge after IBI_ACK to First SCL
                                                     Positive Edge after T-Bit of IBI's MDB                                    */
            uint32_t            : 6;
      __IM  uint32_t INTRASYN   : 1;            /*!< [30..30] ASYN IBI Finished                                                */
      __IM  uint32_t ASYNFAIL   : 1;            /*!< [31..31] C_REF/C2 Counter Overflow                                        */
    } CASYN_b;
  } ;
  
  union {
    __IOM uint32_t TASYN;                       /*!< (@ 0x000000AC) TARGET ASYN TIMING REGISTER                                */
    
    struct {
      __IM  uint32_t TC1        : 16;           /*!< [15..0] IBI to First SCL Positive after IBI_ACK                           */
      __IM  uint32_t TC2        : 8;            /*!< [23..16] First SCL Positive Edge after IBI_ACK to First SCL
                                                     Positive Edge after T-Bit of IBI's MDB                                    */
            uint32_t            : 8;
    } TASYN_b;
  } ;
  __IM  uint32_t  DCTRL;                        /*!< (@ 0x000000B0) DEVCTRL REGISTER                                           */
  
  union {
    __IOM uint32_t DCAP;                        /*!< (@ 0x000000B4) DEVCAP REGISTER                                            */
    
    struct {
      __IM  uint32_t DEVCAP     : 16;           /*!< [15..0] Data Payload for CCC.DEVCAP                                       */
            uint32_t            : 16;
    } DCAP_b;
  } ;
  
  union {
    __IOM uint32_t XTIME;                       /*!< (@ 0x000000B8) EXCAHNGE TIMING SUPPORT INFORMATION REGISTER               */
    
    struct {
      __IOM uint32_t XTIME0     : 8;            /*!< [7..0] Data Byte (Inaccuracy) for CCC.GETXTIME                            */
      __IOM uint32_t XTIME1     : 8;            /*!< [15..8] Data Byte (Frequency) for CCC.GETXTIME                            */
      __IOM uint32_t XTIME2     : 8;            /*!< [23..16] Data Byte (State) for CCC.GETXTIME                               */
      __IOM uint32_t XTIME3     : 8;            /*!< [31..24] Data Byte (Supported Modes) for CCC.GETXTIME                     */
    } XTIME_b;
  } ;
  
  union {
    __IOM uint32_t ENDXFER;                     /*!< (@ 0x000000BC) ENDXFER EARLY TERMINATION PARAMETERS REGISTER              */
    
    struct {
            uint32_t            : 4;
      __IOM uint32_t WRNACK     : 1;            /*!< [4..4] Enable ACK-NACK Capability for Write Command                       */
      __IOM uint32_t WRETM      : 1;            /*!< [5..5] Enable HDR Write Early Termination Request                         */
      __IOM uint32_t ETMCRC     : 2;            /*!< [7..6] Data Byte (Supported Modes) for CCC.GETXTIME                       */
            uint32_t            : 24;
    } ENDXFER_b;
  } ;
  
  union {
    __IOM uint32_t MXLMIN;                      /*!< (@ 0x000000C0) MRL/MWL DEFINED MINIMUM VALUE REGISTER                     */
    
    struct {
      __IOM uint32_t MWLMIN     : 16;           /*!< [15..0] MWL Defined Minimum Value                                         */
      __IOM uint32_t MRLMIN     : 16;           /*!< [31..16] MRL Defined Minimum Value                                        */
    } MXLMIN_b;
  } ;
  
  union {
    __IOM uint32_t MXDSRT;                      /*!< (@ 0x000000C4) MAX DATA READTURN REGISTER                                 */
    
    struct {
      __IOM uint32_t MXDSRT     : 24;           /*!< [23..0] 3-BYTES MAX READ TURN                                             */
            uint32_t            : 7;
      __IOM uint32_t MXDSRTEN   : 1;            /*!< [31..31] Max Data Read turn Enable                                        */
    } MXDSRT_b;
  } ;
  __IOM uint32_t  RSTACT;                       /*!< (@ 0x000000C8) RSTACT REGISTER                                            */
  __IM  uint32_t  RESERVED4[5];
  __IOM uint32_t  MEMDR;                        /*!< (@ 0x000000E0) DATA REGISTER 0                                            */
  __IM  uint32_t  RESERVED5[2];
  __IM  uint32_t  RWCMD;                        /*!< (@ 0x000000EC) VENDOR DIRECT CCC FOR ACCESS TO CONTROL REGISTERS          */
  __IM  uint32_t  RESERVED6[3];
  __IOM uint32_t  MDB;                          /*!< (@ 0x000000FC) MDB LIST REGISTERS                                         */
  __IOM uint32_t  CFR[16];                      /*!< (@ 0x00000100) CONTROL BYFM FIFO REGISTERS                                */
  __IOM uint32_t  TDFR;                         /*!< (@ 0x00000140) TXDA FIFO REGISTERS                                        */
  __IM  uint32_t  RESERVED7[31];
  __IM  uint32_t  RCFR;                         /*!< (@ 0x000001C0) RX COMMAND REGISTERS                                       */
} I3C_Type;                                     /*!< Size = 452 (0x1c4)                                                        */



/* =========================================================================================================================== */
/* ================                                           PWM1                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief PWM Controller (PWM1)
  */

typedef struct {                                /*!< (@ 0x40254500) PWM1 Structure                                             */
  __IOM uint32_t  DUTY;                         /*!< (@ 0x00000000) PWM DUTY REGISTER                                          */
  __IOM uint32_t  DIV;                          /*!< (@ 0x00000004) PWM DIVIDER REGISTER                                       */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000008) PWM CONTROL REGISTER                                       */
    
    struct {
            uint32_t            : 27;
      __IOM uint32_t OUTMODE    : 1;            /*!< [27..27] Output Mode                                                      */
      __IOM uint32_t CLKSRC     : 1;            /*!< [28..28] Clock Source Selection                                           */
      __IOM uint32_t INVT       : 1;            /*!< [29..29] Output Inverter Control                                          */
      __IOM uint32_t RST        : 1;            /*!< [30..30] Reset PWM Controller                                             */
      __IOM uint32_t EN         : 1;            /*!< [31..31] Enable PWM Controller                                            */
    } CTRL_b;
  } ;
} PWM_Type;                                     /*!< Size = 12 (0xc)                                                           */



/* =========================================================================================================================== */
/* ================                                           LED1                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief PWMLED Controller (LED1)
  */

typedef struct {                                /*!< (@ 0x40252D00) LED1 Structure                                             */
  
  union {
    __IOM uint32_t MODE;                        /*!< (@ 0x00000000) MODE REGISTER                                              */
    
    struct {
      __IOM uint32_t SEL        : 2;            /*!< [1..0] Mode Selection                                                     */
            uint32_t            : 30;
    } MODE_b;
  } ;
  
  union {
    __IOM uint32_t DL;                          /*!< (@ 0x00000004) DUTY LIMIT REGISTER                                        */
    
    struct {
      __IOM uint32_t MINV       : 8;            /*!< [7..0] Minimum Duty Cycle                                                 */
      __IOM uint32_t MAXV       : 8;            /*!< [15..8] Maximum Duty Cycle                                                */
            uint32_t            : 16;
    } DL_b;
  } ;
  
  union {
    __IOM uint32_t WT;                          /*!< (@ 0x00000008) WAIT TIME REGISTER                                         */
    
    struct {
      __IOM uint32_t LKV        : 12;           /*!< [11..0] Low wait-time                                                     */
            uint32_t            : 4;
      __IOM uint32_t HKV        : 12;           /*!< [27..16] High wait-time                                                   */
            uint32_t            : 4;
    } WT_b;
  } ;
  
  union {
    __IOM uint32_t SD;                          /*!< (@ 0x0000000C) STEP DUTY REGISTER                                         */
    
    struct {
      __IOM uint32_t STEP0DV    : 4;            /*!< [3..0] Step0 Duty Value                                                   */
      __IOM uint32_t STEP1DV    : 4;            /*!< [7..4] Step1 Duty Value                                                   */
      __IOM uint32_t STEP2DV    : 4;            /*!< [11..8] Step2 Duty Value                                                  */
      __IOM uint32_t STEP3DV    : 4;            /*!< [15..12] Step3 Duty Value                                                 */
      __IOM uint32_t STEP4DV    : 4;            /*!< [19..16] Step4 Duty Value                                                 */
      __IOM uint32_t STEP5DV    : 4;            /*!< [23..20] Step5 Duty Value                                                 */
      __IOM uint32_t STEP6DV    : 4;            /*!< [27..24] Step6 Duty Value                                                 */
      __IOM uint32_t STEP7DV    : 4;            /*!< [31..28] Step7 Duty Value                                                 */
    } SD_b;
  } ;
  
  union {
    __IOM uint32_t ST;                          /*!< (@ 0x00000010) STEP TIME REGISTER                                         */
    
    struct {
      __IOM uint32_t STEP0TV    : 4;            /*!< [3..0] Step0 Time Value                                                   */
      __IOM uint32_t STEP1TV    : 4;            /*!< [7..4] Step1 Time Value                                                   */
      __IOM uint32_t STEP2TV    : 4;            /*!< [11..8] Step2 Time Value                                                  */
      __IOM uint32_t STEP3TV    : 4;            /*!< [15..12] Step3 Time Value                                                 */
      __IOM uint32_t STEP4TV    : 4;            /*!< [19..16] Step4 Time Value                                                 */
      __IOM uint32_t STEP5TV    : 4;            /*!< [23..20] Step5 Time Value                                                 */
      __IOM uint32_t STEP6TV    : 4;            /*!< [27..24] Step6 Time Value                                                 */
      __IOM uint32_t STEP7TV    : 4;            /*!< [31..28] Step7 Time Value                                                 */
    } ST_b;
  } ;
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000014) CONTROL REGISTER                                           */
    
    struct {
            uint32_t            : 16;
      __IOM uint32_t CYCLE      : 8;            /*!< [23..16] The number of output cycle                                       */
            uint32_t            : 4;
      __IOM uint32_t OUTPUT     : 1;            /*!< [28..28] Control the output method                                        */
      __IOM uint32_t INVT       : 1;            /*!< [29..29] Output Inverter Control                                          */
      __IOM uint32_t RST        : 1;            /*!< [30..30] Reset PWM LED Controller                                         */
      __IOM uint32_t EN         : 1;            /*!< [31..31] Enable PWMLED Controller                                         */
    } CTRL_b;
  } ;
} PWMLED_Type;                                  /*!< Size = 24 (0x18)                                                          */



/* =========================================================================================================================== */
/* ================                                          TACHO1                                           ================ */
/* =========================================================================================================================== */


/**
  * @brief Tachometer Controller (TACHO1)
  */

typedef struct {                                /*!< (@ 0x40251F00) TACHO1 Structure                                           */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000000) CONTROL REGISTER                                           */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable Tachometer Controller                                       */
      __IOM uint32_t FILTEREN   : 1;            /*!< [1..1] Enable Filter of Tachometer                                        */
      __IOM uint32_t SELEDGE    : 2;            /*!< [3..2] Select Edges of Tachometer                                         */
      __IOM uint32_t READMODE   : 1;            /*!< [4..4] Select Reading Mode of Tachometer                                  */
            uint32_t            : 11;
      __IM  uint32_t CNT        : 16;           /*!< [31..16] Counter of Tachometer                                            */
    } CTRL_b;
  } ;
  
  union {
    __IOM uint32_t STS;                         /*!< (@ 0x00000004) STATUS REGISTER                                            */
    
    struct {
      __IOM uint32_t LIMIT      : 1;            /*!< [0..0] Limit status of Tachometer                                         */
      __IM  uint32_t PIN        : 1;            /*!< [1..1] Pin Status of Tachometer                                           */
      __IOM uint32_t CHG        : 1;            /*!< [2..2] Tachometer Input Changes Its Status                                */
      __IOM uint32_t CNTRDY     : 1;            /*!< [3..3] Status of Count Ready                                              */
      __IOM uint32_t FAIL       : 1;            /*!< [4..4] Tachometer Failed                                                  */
            uint32_t            : 27;
    } STS_b;
  } ;
  
  union {
    __IOM uint32_t LIMITH;                      /*!< (@ 0x00000008) HIGH-LIMIT REGISTER                                        */
    
    struct {
      __IOM uint32_t VAL        : 16;           /*!< [15..0] High limit value                                                  */
            uint32_t            : 16;
    } LIMITH_b;
  } ;
  
  union {
    __IOM uint32_t LIMITL;                      /*!< (@ 0x0000000C) LOW-LIMIT REGISTER                                         */
    
    struct {
      __IOM uint32_t VAL        : 16;           /*!< [15..0] Low limit value                                                   */
            uint32_t            : 16;
    } LIMITL_b;
  } ;
  
  union {
    __IOM uint32_t INTEN;                       /*!< (@ 0x00000010) INTERRUPT ENABLE REGISTER                                  */
    
    struct {
      __IOM uint32_t LIMITEN    : 1;            /*!< [0..0] Limit Interrupt Enable                                             */
      __IOM uint32_t CNTRDYEN   : 1;            /*!< [1..1] Ready to count Interrupt Enable                                    */
      __IOM uint32_t CHGEN      : 1;            /*!< [2..2] Input Change Interrupt Enable                                      */
      __IOM uint32_t FAILEN     : 1;            /*!< [3..3] Tacho Failed Interrupt Enable                                      */
            uint32_t            : 28;
    } INTEN_b;
  } ;
  __IOM uint32_t  THRESHOLD;                    /*!< (@ 0x00000014) TACHO THRESHOLD REGISTER                                   */
  __IOM uint32_t  TIMEOUT;                      /*!< (@ 0x00000018) TACHO TIMEOUT REGISTER                                     */
} TACH1_Type;                                   /*!< Size = 28 (0x1c)                                                          */



/* =========================================================================================================================== */
/* ================                                            ADC                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief 12-bit ADC Controller (ADC)
  */

typedef struct {                                /*!< (@ 0x40251900) ADC Structure                                              */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000000) CONTROL REGISTER                                           */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable ADC Controller                                              */
      __IOM uint32_t START      : 1;            /*!< [1..1] Start ADC Conversion                                               */
      __IOM uint32_t RST        : 1;            /*!< [2..2] Reset ADC Controller                                               */
      __IOM uint32_t MDSEL      : 1;            /*!< [3..3] Conversion Mode Selection                                          */
      __IOM uint32_t SGLDNINTEN : 1;            /*!< [4..4] Enable Single Done Interrupt                                       */
      __IOM uint32_t RPTDNINTEN : 1;            /*!< [5..5] Enable Repeat Done Interrupt                                       */
            uint32_t            : 26;
    } CTRL_b;
  } ;
  
  union {
    __IOM uint32_t CHCTRL;                      /*!< (@ 0x00000004) CHANNEL CONTROL REGISTER                                   */
    
    struct {
      __IOM uint32_t CH0EN      : 1;            /*!< [0..0] Enable ADC Channel                                                 */
      __IOM uint32_t CH1EN      : 1;            /*!< [1..1] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH2EN      : 1;            /*!< [2..2] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH3EN      : 1;            /*!< [3..3] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH4EN      : 1;            /*!< [4..4] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH5EN      : 1;            /*!< [5..5] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH6EN      : 1;            /*!< [6..6] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH7EN      : 1;            /*!< [7..7] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH8EN      : 1;            /*!< [8..8] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH9EN      : 1;            /*!< [9..9] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH10EN     : 1;            /*!< [10..10] Conversion of ADC Channel Complete                               */
      __IOM uint32_t CH11EN     : 1;            /*!< [11..11] Conversion of ADC Channel Complete                               */
      __IOM uint32_t LPF0BP     : 1;            /*!< [12..12] Bypass ADC Channel Low-Pass Filter                               */
      __IOM uint32_t LPF1BP     : 1;            /*!< [13..13] Bypass ADC Channel Low-Pass Filter                               */
      __IOM uint32_t LPF2BP     : 1;            /*!< [14..14] Bypass ADC Channel Low-Pass Filter                               */
      __IOM uint32_t LPF3BP     : 1;            /*!< [15..15] Bypass ADC Channel Low-Pass Filter                               */
      __IOM uint32_t LPF4BP     : 1;            /*!< [16..16] Bypass ADC Channel Low-Pass Filter                               */
      __IOM uint32_t LPF5BP     : 1;            /*!< [17..17] Bypass ADC Channel Low-Pass Filter                               */
      __IOM uint32_t LPF6BP     : 1;            /*!< [18..18] Bypass ADC Channel Low-Pass Filter                               */
      __IOM uint32_t LPF7BP     : 1;            /*!< [19..19] Bypass ADC Channel Low-Pass Filter                               */
      __IOM uint32_t LPF8BP     : 1;            /*!< [20..20] Bypass ADC Channel Low-Pass Filter                               */
      __IOM uint32_t LPF9BP     : 1;            /*!< [21..21] Bypass ADC Channel Low-Pass Filter                               */
      __IOM uint32_t LPF10BP    : 1;            /*!< [22..22] Bypass ADC Channel Low-Pass Filter                               */
      __IOM uint32_t LPF11BP    : 1;            /*!< [23..23] Bypass ADC Channel Low-Pass Filter                               */
      __IOM uint32_t CALBP      : 1;            /*!< [24..24] Bypass Linear Calibration                                        */
            uint32_t            : 7;
    } CHCTRL_b;
  } ;
  
  union {
    __IOM uint32_t STS;                         /*!< (@ 0x00000008) STATUS REGISTER                                            */
    
    struct {
      __IOM uint32_t CH0DN      : 1;            /*!< [0..0] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH1DN      : 1;            /*!< [1..1] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH2DN      : 1;            /*!< [2..2] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH3DN      : 1;            /*!< [3..3] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH4DN      : 1;            /*!< [4..4] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH5DN      : 1;            /*!< [5..5] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH6DN      : 1;            /*!< [6..6] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH7DN      : 1;            /*!< [7..7] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH8DN      : 1;            /*!< [8..8] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH9DN      : 1;            /*!< [9..9] Conversion of ADC Channel Complete                                 */
      __IOM uint32_t CH10DN     : 1;            /*!< [10..10] Conversion of ADC Channel Complete                               */
      __IOM uint32_t CH11DN     : 1;            /*!< [11..11] Conversion of ADC Channel Complete                               */
      __IOM uint32_t SGLDN      : 1;            /*!< [12..12] Single Conversion Completed                                      */
      __IOM uint32_t RPTDN      : 1;            /*!< [13..13] Repeat Conversion Completed                                      */
            uint32_t            : 2;
      __IM  uint32_t RDY        : 1;            /*!< [16..16] ADC Ready to Conversion                                          */
      __IM  uint32_t LPFSTB     : 1;            /*!< [17..17] Low-Pass Filter Stable                                           */
            uint32_t            : 14;
    } STS_b;
  } ;
  
  union {
    __IOM uint32_t CHDATA[12];                  /*!< (@ 0x0000000C) ADC channel Data                                           */
    
    struct {
      __IOM uint32_t RESULT     : 12;           /*!< [11..0] Conversion Result of Corresponding ADC Channel                    */
            uint32_t            : 20;
    } CHDATA_b[12];
  } ;
  
  union {
    __IOM uint32_t COEFFA;                      /*!< (@ 0x0000003C) ADC COEFFICIENT A                                          */
    
    struct {
      __IOM uint32_t COEFA      : 19;           /*!< [18..0] 19 Bit Coefficient A                                              */
            uint32_t            : 12;
      __IOM uint32_t COEFASEL   : 1;            /*!< [31..31] Enable FW Write Data to Coefficient A                            */
    } COEFFA_b;
  } ;
  
  union {
    __IOM uint32_t COEFFB;                      /*!< (@ 0x00000040) ADC COEFFICIENT B                                          */
    
    struct {
      __IOM uint32_t COEFB      : 27;           /*!< [26..0] 27 Bit Coefficient B                                              */
            uint32_t            : 4;
      __IOM uint32_t COEFASEL   : 1;            /*!< [31..31] Enable FW Write Data to Coefficient B                            */
    } COEFFB_b;
  } ;
  
  union {
    __IOM uint32_t ACPR0;                       /*!< (@ 0x00000044) ADC COMPARATOR 0 REGISTER                                  */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable ADC Comparator                                              */
      __IOM uint32_t DEBNCE     : 2;            /*!< [2..1] ADC Comparator Debounce                                            */
      __IOM uint32_t OUTPOR     : 1;            /*!< [3..3] ADC Comparator Output Polarity                                     */
      __IM  uint32_t OUTSTS     : 1;            /*!< [4..4] ADC Comparator Output Status                                       */
      __IOM uint32_t THRSHLD    : 12;           /*!< [16..5] ADC Comparator Threshold                                          */
      __IOM uint32_t ACPRCH     : 3;            /*!< [19..17] ADC Comparator Channel Select                                    */
      __IOM uint32_t INTEN      : 1;            /*!< [20..20] Enable ADC Comparator Interrupt                                  */
      __IOM uint32_t INTSTS     : 1;            /*!< [21..21] Enable ADC Comparator Interrupt Status                           */
            uint32_t            : 10;
    } ACPR0_b;
  } ;
  
  union {
    __IOM uint32_t ACPR1;                       /*!< (@ 0x00000048) ADC COMPARATOR 0 REGISTER                                  */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable ADC Comparator                                              */
      __IOM uint32_t DEBNCE     : 2;            /*!< [2..1] ADC Comparator Debounce                                            */
      __IOM uint32_t OUTPOR     : 1;            /*!< [3..3] ADC Comparator Output Polarity                                     */
      __IM  uint32_t OUTSTS     : 1;            /*!< [4..4] ADC Comparator Output Status                                       */
      __IOM uint32_t THRSHLD    : 12;           /*!< [16..5] ADC Comparator Threshold                                          */
      __IOM uint32_t ACPRCH     : 3;            /*!< [19..17] ADC Comparator Channel Select                                    */
      __IOM uint32_t INTEN      : 1;            /*!< [20..20] Enable ADC Comparator Interrupt                                  */
      __IOM uint32_t INTSTS     : 1;            /*!< [21..21] Enable ADC Comparator Interrupt Status                           */
            uint32_t            : 10;
    } ACPR1_b;
  } ;
  
  union {
    __IOM uint32_t ACPR2;                       /*!< (@ 0x0000004C) ADC COMPARATOR 0 REGISTER                                  */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable ADC Comparator                                              */
      __IOM uint32_t DEBNCE     : 2;            /*!< [2..1] ADC Comparator Debounce                                            */
      __IOM uint32_t OUTPOR     : 1;            /*!< [3..3] ADC Comparator Output Polarity                                     */
      __IM  uint32_t OUTSTS     : 1;            /*!< [4..4] ADC Comparator Output Status                                       */
      __IOM uint32_t THRSHLD    : 12;           /*!< [16..5] ADC Comparator Threshold                                          */
      __IOM uint32_t ACPRCH     : 3;            /*!< [19..17] ADC Comparator Channel Select                                    */
      __IOM uint32_t INTEN      : 1;            /*!< [20..20] Enable ADC Comparator Interrupt                                  */
      __IOM uint32_t INTSTS     : 1;            /*!< [21..21] Enable ADC Comparator Interrupt Status                           */
            uint32_t            : 10;
    } ACPR2_b;
  } ;
  __IM  uint32_t  RESERVED[12];
  
  union {
    __IOM uint32_t ACPR3;                       /*!< (@ 0x00000080) ADC COMPARATOR 0 REGISTER                                  */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable ADC Comparator                                              */
      __IOM uint32_t DEBNCE     : 2;            /*!< [2..1] ADC Comparator Debounce                                            */
      __IOM uint32_t OUTPOR     : 1;            /*!< [3..3] ADC Comparator Output Polarity                                     */
      __IM  uint32_t OUTSTS     : 1;            /*!< [4..4] ADC Comparator Output Status                                       */
      __IOM uint32_t THRSHLD    : 12;           /*!< [16..5] ADC Comparator Threshold                                          */
      __IOM uint32_t ACPRCH     : 3;            /*!< [19..17] ADC Comparator Channel Select                                    */
      __IOM uint32_t INTEN      : 1;            /*!< [20..20] Enable ADC Comparator Interrupt                                  */
      __IOM uint32_t INTSTS     : 1;            /*!< [21..21] Enable ADC Comparator Interrupt Status                           */
            uint32_t            : 10;
    } ACPR3_b;
  } ;
} ADC_Type;                                     /*!< Size = 132 (0x84)                                                         */



/* =========================================================================================================================== */
/* ================                                           PS2_1                                           ================ */
/* =========================================================================================================================== */


/**
  * @brief PS2 Controller (PS2_1)
  */

typedef struct {                                /*!< (@ 0x40254300) PS2_1 Structure                                            */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000000) CONTROL REGISTER                                           */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable PS/2 Controller                                             */
      __IOM uint32_t MDSEL      : 1;            /*!< [1..1] Accessing Mode Selection                                           */
      __IOM uint32_t TXSTR      : 1;            /*!< [2..2] Kick PS/2 Controller Start to Transmit                             */
      __IM  uint32_t RDY        : 1;            /*!< [3..3] Ready to Transmit or Not                                           */
      __IOM uint32_t ACTIVE     : 1;            /*!< [4..4] PS/2 Controller Operate Stage                                      */
            uint32_t            : 27;
    } CTRL_b;
  } ;
  
  union {
    __IOM uint32_t TX;                          /*!< (@ 0x00000004) TRANSMIT DATA REGISTER                                     */
    
    struct {
      __IOM uint32_t DATA       : 8;            /*!< [7..0] PS/2 Transmit Data                                                 */
            uint32_t            : 24;
    } TX_b;
  } ;
  
  union {
    __IOM uint32_t RX;                          /*!< (@ 0x00000008) RECEIVE DATA REGISTER                                      */
    
    struct {
      __IOM uint32_t DATA       : 8;            /*!< [7..0] PS/2 Receive Data Buffer                                           */
            uint32_t            : 24;
    } RX_b;
  } ;
  
  union {
    __IOM uint32_t STS;                         /*!< (@ 0x0000000C) STATUS REGISTER                                            */
    
    struct {
      __IOM uint32_t STRSTS     : 1;            /*!< [0..0] Strat-bit Detection Status                                         */
      __IOM uint32_t TDS        : 1;            /*!< [1..1] Transcation Done Status                                            */
      __IOM uint32_t PRTERR     : 1;            /*!< [2..2] Parity-bit Error Status                                            */
      __IOM uint32_t STPERR     : 1;            /*!< [3..3] Stop-bit Error Status                                              */
      __IM  uint32_t BUSY       : 1;            /*!< [4..4] Busy Status                                                        */
      __IOM uint32_t TXSTRTO    : 1;            /*!< [5..5] Transmit Start Timeout Status                                      */
      __IOM uint32_t TXTO       : 1;            /*!< [6..6] Transmit Timeout Status                                            */
      __IOM uint32_t RXTO       : 1;            /*!< [7..7] Receive Timeout Status                                             */
            uint32_t            : 24;
    } STS_b;
  } ;
  
  union {
    __IOM uint32_t INTEN;                       /*!< (@ 0x00000010) INTERRUPT ENABLE REGISTER                                  */
    
    struct {
      __IOM uint32_t STRINTEN   : 1;            /*!< [0..0] Enable Start-bit Detection Interrupt                               */
      __IOM uint32_t TDSINTEN   : 1;            /*!< [1..1] Enable Transmit Done Interrupt                                     */
      __IOM uint32_t STSINTEN   : 1;            /*!< [2..2] Enable Status Interrupt                                            */
            uint32_t            : 29;
    } INTEN_b;
  } ;
} PS2_Type;                                     /*!< Size = 20 (0x14)                                                          */



/* =========================================================================================================================== */
/* ================                                            KBM                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief Keyboard Matrix Controller (KBM)
  */

typedef struct {                                /*!< (@ 0x40254200) KBM Structure                                              */
  
  union {
    __IOM uint32_t SCANOUT;                     /*!< (@ 0x00000000) SCAN OUT CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t KSO0       : 1;            /*!< [0..0] KSO Output Level                                                   */
      __IOM uint32_t KSO1       : 1;            /*!< [1..1] KSO Output Level                                                   */
      __IOM uint32_t KSO2       : 1;            /*!< [2..2] KSO Output Level                                                   */
      __IOM uint32_t KSO3       : 1;            /*!< [3..3] KSO Output Level                                                   */
      __IOM uint32_t KSO4       : 1;            /*!< [4..4] KSO Output Level                                                   */
      __IOM uint32_t KSO5       : 1;            /*!< [5..5] KSO Output Level                                                   */
      __IOM uint32_t KSO6       : 1;            /*!< [6..6] KSO Output Level                                                   */
      __IOM uint32_t KSO7       : 1;            /*!< [7..7] KSO Output Level                                                   */
      __IOM uint32_t KSO8       : 1;            /*!< [8..8] KSO Output Level                                                   */
      __IOM uint32_t KSO9       : 1;            /*!< [9..9] KSO Output Level                                                   */
      __IOM uint32_t KSO10      : 1;            /*!< [10..10] KSO Output Level                                                 */
      __IOM uint32_t KSO11      : 1;            /*!< [11..11] KSO Output Level                                                 */
      __IOM uint32_t KSO12      : 1;            /*!< [12..12] KSO Output Level                                                 */
      __IOM uint32_t KSO13      : 1;            /*!< [13..13] KSO Output Level                                                 */
      __IOM uint32_t KSO14      : 1;            /*!< [14..14] KSO Output Level                                                 */
      __IOM uint32_t KSO15      : 1;            /*!< [15..15] KSO Output Level                                                 */
      __IOM uint32_t KSO16      : 1;            /*!< [16..16] KSO Output Level                                                 */
      __IOM uint32_t KSO17      : 1;            /*!< [17..17] KSO Output Level                                                 */
      __IOM uint32_t KSO18      : 1;            /*!< [18..18] KSO Output Level                                                 */
      __IOM uint32_t KSO19      : 1;            /*!< [19..19] KSO Output Level                                                 */
            uint32_t            : 12;
    } SCANOUT_b;
  } ;
  
  union {
    __IM  uint32_t SCANIN;                      /*!< (@ 0x00000004) SACN INPUT STATUS REGISTER                                 */
    
    struct {
      __IM  uint32_t KSI0       : 1;            /*!< [0..0] KSI Input Level                                                    */
      __IM  uint32_t KSI1       : 1;            /*!< [1..1] KSI Input Level                                                    */
      __IM  uint32_t KSI2       : 1;            /*!< [2..2] KSI Input Level                                                    */
      __IM  uint32_t KSI3       : 1;            /*!< [3..3] KSI Input Level                                                    */
      __IM  uint32_t KSI4       : 1;            /*!< [4..4] KSI Input Level                                                    */
      __IM  uint32_t KSI5       : 1;            /*!< [5..5] KSI Input Level                                                    */
      __IM  uint32_t KSI6       : 1;            /*!< [6..6] KSI Input Level                                                    */
      __IM  uint32_t KSI7       : 1;            /*!< [7..7] KSI Input Level                                                    */
      __IM  uint32_t KSI8       : 1;            /*!< [8..8] KSI Input Level                                                    */
      __IM  uint32_t KSI9       : 1;            /*!< [9..9] KSI Input Level                                                    */
            uint32_t            : 22;
    } SCANIN_b;
  } ;
  
  union {
    __IOM uint32_t INTEN;                       /*!< (@ 0x00000008) INTERRUPT ENABLE REGISTER                                  */
    
    struct {
      __IOM uint32_t KSI0       : 1;            /*!< [0..0] KSI Interrupt Enable                                               */
      __IOM uint32_t KSI1       : 1;            /*!< [1..1] KSI Interrupt Enable                                               */
      __IOM uint32_t KSI2       : 1;            /*!< [2..2] KSI Interrupt Enable                                               */
      __IOM uint32_t KSI3       : 1;            /*!< [3..3] KSI Interrupt Enable                                               */
      __IOM uint32_t KSI4       : 1;            /*!< [4..4] KSI Interrupt Enable                                               */
      __IOM uint32_t KSI5       : 1;            /*!< [5..5] KSI Interrupt Enable                                               */
      __IOM uint32_t KSI6       : 1;            /*!< [6..6] KSI Interrupt Enable                                               */
      __IOM uint32_t KSI7       : 1;            /*!< [7..7] KSI Interrupt Enable                                               */
      __IOM uint32_t KSI8       : 1;            /*!< [8..8] KSI Interrupt Enable                                               */
      __IOM uint32_t KSI9       : 1;            /*!< [9..9] KSI Interrupt Enable                                               */
            uint32_t            : 22;
    } INTEN_b;
  } ;
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x0000000C) CONTROL REGISTER                                           */
    
    struct {
      __IOM uint32_t KSOTYPE    : 1;            /*!< [0..0] Output Type of KSO pin                                             */
      __IOM uint32_t KSI8EN     : 1;            /*!< [1..1] Enable KSI8                                                        */
      __IOM uint32_t KSI9EN     : 1;            /*!< [2..2] Enable KSI9                                                        */
            uint32_t            : 13;
      __IOM uint32_t KSO18EN    : 1;            /*!< [16..16] Enable KSO18                                                     */
      __IOM uint32_t KSO19EN    : 1;            /*!< [17..17] Enable KSO19                                                     */
      __IOM uint32_t KSIINTSTS  : 1;            /*!< [18..18] KSI Interrupt Status                                             */
      __IOM uint32_t I2CDBGDEL  : 1;            /*!< [19..19] I2C Debug Select                                                 */
      __IOM uint32_t I2CDBGEN   : 1;            /*!< [20..20] I2C Debug Enable                                                 */
            uint32_t            : 9;
      __IOM uint32_t ESPIMONEN  : 1;            /*!< [30..30] ESPI Monitor Enable                                              */
      __IOM uint32_t KBMDBGEN   : 1;            /*!< [31..31] KBM Debug Enable                                                 */
    } CTRL_b;
  } ;
  
  union {
    __IM  uint32_t DBGSTS;                      /*!< (@ 0x00000010) DWBUG STATUS REGISTER                                      */
    
    struct {
      __IM  uint32_t DBGMODE    : 1;            /*!< [0..0] Debug Mode Status                                                  */
      __IM  uint32_t EXITMODE   : 1;            /*!< [1..1] Exit Debug Mode Status                                             */
      __IM  uint32_t STEPSTS    : 4;            /*!< [5..2] Entry Debug Status                                                 */
      __IM  uint32_t PWRSTS     : 1;            /*!< [6..6] KBM Power Status                                                   */
            uint32_t            : 1;
      __IM  uint32_t KSISTS     : 8;            /*!< [15..8] KSIs Status                                                       */
      __IM  uint32_t INFLS      : 1;            /*!< [16..16] Internal Flash Select                                            */
      __IM  uint32_t CS0FLS     : 1;            /*!< [17..17] CS0 Flash Select                                                 */
      __IM  uint32_t CS1FLS     : 1;            /*!< [18..18] CS1 Flash Select                                                 */
      __IM  uint32_t JTAGMODE   : 1;            /*!< [19..19] Jtag Mode Select                                                 */
      __IM  uint32_t ESPIMODE   : 1;            /*!< [20..20] eSPI Mode Select                                                 */
      __IM  uint32_t SWDMODE    : 1;            /*!< [21..21] SWD Mode Select                                                  */
            uint32_t            : 10;
    } DBGSTS_b;
  } ;
  
  union {
    __IOM uint32_t SCANCTRL;                    /*!< (@ 0x00000014) KB SCAN CONTROL REGISTER                                   */
    
    struct {
      __IOM uint32_t AUTOEN     : 1;            /*!< [0..0] KBS AUTO SCAN ENABLE                                               */
      __IOM uint32_t INTEN      : 1;            /*!< [1..1] KBS AUTO SCAN INTERRUPT ENABLE                                     */
      __IOM uint32_t KBSLOOP    : 3;            /*!< [4..2] KBS AUTO SCAN LOOP                                                 */
      __IOM uint32_t KBMSIZE    : 3;            /*!< [7..5] KBS MATRIX SIZE                                                    */
      __IOM uint32_t HDELAY     : 4;            /*!< [11..8] KSO HIGH DELAY                                                    */
      __IOM uint32_t SCANDELAY  : 4;            /*!< [15..12] KSI SCAN DELAY                                                   */
      __IOM uint32_t ROUNDDELAY : 4;            /*!< [19..16] SCAN ROUND DELAY                                                 */
            uint32_t            : 12;
    } SCANCTRL_b;
  } ;
  
  union {
    __IOM uint32_t KBSSTS;                      /*!< (@ 0x00000018) KBSCAN STATUS REGISTER                                     */
    
    struct {
      __IOM uint32_t KSO0KSI    : 1;            /*!< [0..0] KSO Sampling SKI status                                            */
      __IOM uint32_t KSO1KSI    : 1;            /*!< [1..1] KSO Sampling SKI status                                            */
      __IOM uint32_t KSO2KSI    : 1;            /*!< [2..2] KSO Sampling SKI status                                            */
      __IOM uint32_t KSO3KSI    : 1;            /*!< [3..3] KSO Sampling SKI status                                            */
      __IOM uint32_t KSO4KSI    : 1;            /*!< [4..4] KSO Sampling SKI status                                            */
      __IOM uint32_t KSO5KSI    : 1;            /*!< [5..5] KSO Sampling SKI status                                            */
      __IOM uint32_t KSO6KSI    : 1;            /*!< [6..6] KSO Sampling SKI status                                            */
      __IOM uint32_t KSO7KSI    : 1;            /*!< [7..7] KSO Sampling SKI status                                            */
      __IOM uint32_t KSO8KSI    : 1;            /*!< [8..8] KSO Sampling SKI status                                            */
      __IOM uint32_t KSO9KSI    : 1;            /*!< [9..9] KSO Sampling SKI status                                            */
      __IOM uint32_t KSO10KSI   : 1;            /*!< [10..10] KSO Sampling SKI status                                          */
      __IOM uint32_t KSO11KSI   : 1;            /*!< [11..11] KSO Sampling SKI status                                          */
      __IOM uint32_t KSO12KSI   : 1;            /*!< [12..12] KSO Sampling SKI status                                          */
      __IOM uint32_t KSO13KSI   : 1;            /*!< [13..13] KSO Sampling SKI status                                          */
      __IOM uint32_t KSO14KSI   : 1;            /*!< [14..14] KSO Sampling SKI status                                          */
      __IOM uint32_t KSO15KSI   : 1;            /*!< [15..15] KSO Sampling SKI status                                          */
      __IOM uint32_t KSO16KSI   : 1;            /*!< [16..16] KSO Sampling SKI status                                          */
      __IOM uint32_t KSO17KSI   : 1;            /*!< [17..17] KSO Sampling SKI status                                          */
      __IOM uint32_t KSO18KSI   : 1;            /*!< [18..18] KSO Sampling SKI status                                          */
      __IOM uint32_t KSO19KSI   : 1;            /*!< [19..19] KSO Sampling SKI status                                          */
            uint32_t            : 12;
    } KBSSTS_b;
  } ;
  
  union {
    __IM  uint32_t KSOGP0DAT;                   /*!< (@ 0x0000001C) KSO GROUP0 DATA REGISTER                                   */
    
    struct {
      __IM  uint32_t KSO0DATA   : 8;            /*!< [7..0] KSO0 data                                                          */
      __IM  uint32_t KSO1DATA   : 8;            /*!< [15..8] KSO1 data                                                         */
      __IM  uint32_t KSO2DATA   : 8;            /*!< [23..16] KSO2 data                                                        */
      __IM  uint32_t KSO3DATA   : 8;            /*!< [31..24] KSO3 data                                                        */
    } KSOGP0DAT_b;
  } ;
  
  union {
    __IM  uint32_t KSOGP1DAT;                   /*!< (@ 0x00000020) KSO GROUP1 DATA REGISTER                                   */
    
    struct {
      __IM  uint32_t KSO4DATA   : 8;            /*!< [7..0] KSO4 data                                                          */
      __IM  uint32_t KSO5DATA   : 8;            /*!< [15..8] KSO5 data                                                         */
      __IM  uint32_t KSO6DATA   : 8;            /*!< [23..16] KSO6 data                                                        */
      __IM  uint32_t KSO7DATA   : 8;            /*!< [31..24] KSO7 data                                                        */
    } KSOGP1DAT_b;
  } ;
  
  union {
    __IM  uint32_t KSOGP2DAT;                   /*!< (@ 0x00000024) KSO GROUP2 DATA REGISTER                                   */
    
    struct {
      __IM  uint32_t KSO8DATA   : 8;            /*!< [7..0] KSO8 data                                                          */
      __IM  uint32_t KSO9DATA   : 8;            /*!< [15..8] KSO9 data                                                         */
      __IM  uint32_t KSO10DATA  : 8;            /*!< [23..16] KSO10 data                                                       */
      __IM  uint32_t KSO11DATA  : 8;            /*!< [31..24] KSO11 data                                                       */
    } KSOGP2DAT_b;
  } ;
  
  union {
    __IM  uint32_t KSOGP3DAT;                   /*!< (@ 0x00000028) KSO GROUP3 DATA REGISTER                                   */
    
    struct {
      __IM  uint32_t KSO12DATA  : 8;            /*!< [7..0] KSO12 data                                                         */
      __IM  uint32_t KSO13DATA  : 8;            /*!< [15..8] KSO13 data                                                        */
      __IM  uint32_t KSO14DATA  : 8;            /*!< [23..16] KSO14 data                                                       */
      __IM  uint32_t KSO15DATA  : 8;            /*!< [31..24] KSO15 data                                                       */
    } KSOGP3DAT_b;
  } ;
  
  union {
    __IM  uint32_t KSOGP4DAT;                   /*!< (@ 0x0000002C) KSO GROUP4 DATA REGISTER                                   */
    
    struct {
      __IM  uint32_t KSO16DATA  : 8;            /*!< [7..0] KSO16 data                                                         */
      __IM  uint32_t KSO17DATA  : 8;            /*!< [15..8] KSO17 data                                                        */
      __IM  uint32_t KSO18DATA  : 8;            /*!< [23..16] KSO18 data                                                       */
      __IM  uint32_t KSO19DATA  : 8;            /*!< [31..24] KSO19 data                                                       */
    } KSOGP4DAT_b;
  } ;
} KBM_Type;                                     /*!< Size = 48 (0x30)                                                          */



/* =========================================================================================================================== */
/* ================                                           UART                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief UART Controller (UART)
  */

typedef struct {                                /*!< (@ 0x40251600) UART Structure                                             */
  
  union {
    union {
      __IM  uint32_t RBR;                       /*!< (@ 0x00000000) RECEIVE BUFFER REGISTER                                    */
      
      struct {
        __IM  uint32_t DATA     : 8;            /*!< [7..0] Receive Buffer                                                     */
              uint32_t          : 24;
      } RBR_b;
    } ;
    
    union {
      __IOM uint32_t THR;                       /*!< (@ 0x00000000) TRANSMIT HOLDING REGISTER                                  */
      
      struct {
        __IOM uint32_t DATA     : 8;            /*!< [7..0] Transmit Holding                                                   */
              uint32_t          : 24;
      } THR_b;
    } ;
    
    union {
      __IOM uint32_t DLL;                       /*!< (@ 0x00000000) DIVISOR LATCH LOW REGISTER                                 */
      
      struct {
        __IOM uint32_t DIVL     : 8;            /*!< [7..0] Divisor Latch Low Byte                                             */
              uint32_t          : 24;
      } DLL_b;
    } ;
  };
  
  union {
    union {
      __IOM uint32_t DLH;                       /*!< (@ 0x00000004) DIVISOR LATCH HIGH REGISTER                                */
      
      struct {
        __IOM uint32_t DIVH     : 8;            /*!< [7..0] Divisor Latch High Byte                                            */
              uint32_t          : 24;
      } DLH_b;
    } ;
    
    union {
      __IOM uint32_t IER;                       /*!< (@ 0x00000004) INTRRRUPT ENABLE REGISTER                                  */
      
      struct {
        __IOM uint32_t ERBFI    : 1;            /*!< [0..0] Enable Received Data Available Interrupt                           */
        __IOM uint32_t ETBEI    : 1;            /*!< [1..1] Enable Transmit Holding Register Empty Interrupt                   */
        __IOM uint32_t ELSI     : 1;            /*!< [2..2] Enable Receiver Line Status Interrupt                              */
              uint32_t          : 4;
        __IOM uint32_t PTIME    : 1;            /*!< [7..7] Programmable THRE Interrupt Mode Enable                            */
              uint32_t          : 24;
      } IER_b;
    } ;
  };
  
  union {
    union {
      __IM  uint32_t IIR;                       /*!< (@ 0x00000008) INTERRUPT IDENTIFICATION                                   */
      
      struct {
        __IM  uint32_t IID      : 4;            /*!< [3..0] Interrupt ID                                                       */
              uint32_t          : 2;
        __IM  uint32_t FIFOSE   : 2;            /*!< [7..6] FIFOs Enabled                                                      */
              uint32_t          : 24;
      } IIR_b;
    } ;
    
    union {
      __IOM uint32_t FCR;                       /*!< (@ 0x00000008) FIFO CONTROL REGISTER                                      */
      
      struct {
        __IOM uint32_t FIFOE    : 1;            /*!< [0..0] FIFO Enabled                                                       */
        __IOM uint32_t RFIFOR   : 1;            /*!< [1..1] Rx FIFO Reset                                                      */
        __IOM uint32_t XFIFOR   : 1;            /*!< [2..2] Tx FIFO Reset                                                      */
              uint32_t          : 1;
        __IOM uint32_t TXTRILEV : 2;            /*!< [5..4] TX Empty Trigger Level                                             */
        __IOM uint32_t RXTRILEV : 2;            /*!< [7..6] Rx Trigger Level                                                   */
              uint32_t          : 24;
      } FCR_b;
    } ;
  };
  
  union {
    __IOM uint32_t LCR;                         /*!< (@ 0x0000000C) LINE CONTROL REGISTER                                      */
    
    struct {
      __IOM uint32_t DLS        : 2;            /*!< [1..0] Data Length Select                                                 */
      __IOM uint32_t STOP       : 1;            /*!< [2..2] Number of Stop Bits                                                */
      __IOM uint32_t PEN        : 1;            /*!< [3..3] Parity Enable                                                      */
      __IOM uint32_t EPS        : 1;            /*!< [4..4] Even Parity Select                                                 */
      __IOM uint32_t STP        : 1;            /*!< [5..5] Stick Parity                                                       */
      __IOM uint32_t BC         : 1;            /*!< [6..6] Break Control Bit                                                  */
      __IOM uint32_t DLAB       : 1;            /*!< [7..7] Divisor Latch Access Bit                                           */
            uint32_t            : 24;
    } LCR_b;
  } ;
  
  union {
    __IOM uint32_t MCR;                         /*!< (@ 0x00000010) MODEM CONTROL REGISTER                                     */
    
    struct {
      __IOM uint32_t DTR        : 1;            /*!< [0..0] Data Terminal Ready                                                */
      __IOM uint32_t RTS        : 1;            /*!< [1..1] Request to Send                                                    */
      __IOM uint32_t OUT1       : 1;            /*!< [2..2] OUT1                                                               */
      __IOM uint32_t OUT2       : 1;            /*!< [3..3] OUT2                                                               */
      __IOM uint32_t LB         : 1;            /*!< [4..4] LoopBack                                                           */
      __IOM uint32_t AFCE       : 1;            /*!< [5..5] Auto Flow Control Enable                                           */
      __IOM uint32_t SIRE       : 1;            /*!< [6..6] SIR Mode Enable                                                    */
            uint32_t            : 25;
    } MCR_b;
  } ;
  
  union {
    __IM  uint32_t LSR;                         /*!< (@ 0x00000014) LINE STATUS REGISTER                                       */
    
    struct {
      __IM  uint32_t DR         : 1;            /*!< [0..0] Data Ready Bit                                                     */
      __IM  uint32_t OE         : 1;            /*!< [1..1] Overrun Error bit                                                  */
      __IM  uint32_t PE         : 1;            /*!< [2..2] Parity Error bit                                                   */
      __IM  uint32_t FE         : 1;            /*!< [3..3] Framing Error bit                                                  */
      __IM  uint32_t BI         : 1;            /*!< [4..4] Break Interrupt bit                                                */
      __IM  uint32_t THRE       : 1;            /*!< [5..5] Transmit Holding Register Empty bit                                */
      __IM  uint32_t TEMT       : 1;            /*!< [6..6] Transmitter Empty Bit                                              */
      __IM  uint32_t RFE        : 1;            /*!< [7..7] Receiver FIFO Error                                                */
            uint32_t            : 24;
    } LSR_b;
  } ;
  
  union {
    __IM  uint32_t MSR;                         /*!< (@ 0x00000018) MODEM STATUS REGISTER                                      */
    
    struct {
      __IM  uint32_t DCTS       : 1;            /*!< [0..0] Data Clear to Send                                                 */
      __IM  uint32_t DDSR       : 1;            /*!< [1..1] Delta Data Set Ready                                               */
      __IM  uint32_t TERI       : 1;            /*!< [2..2] Trailing Edge of Ring Indicator                                    */
      __IM  uint32_t DDCD       : 1;            /*!< [3..3] Delta Data Carrier Detect                                          */
      __IM  uint32_t CTS        : 1;            /*!< [4..4] Clear to Send                                                      */
      __IM  uint32_t DSR        : 1;            /*!< [5..5] Data Set Ready                                                     */
      __IM  uint32_t RI         : 1;            /*!< [6..6] Ring Indicator                                                     */
      __IM  uint32_t DCD        : 1;            /*!< [7..7] Data Carrier Detect                                                */
            uint32_t            : 24;
    } MSR_b;
  } ;
  __IM  uint32_t  RESERVED[24];
  
  union {
    __IM  uint32_t USR;                         /*!< (@ 0x0000007C) UART Status Register                                       */
    
    struct {
      __IM  uint32_t BUSY       : 1;            /*!< [0..0] UART Busy                                                          */
      __IM  uint32_t TFNF       : 1;            /*!< [1..1] Transmit FIFO Not Full                                             */
      __IM  uint32_t TFE        : 1;            /*!< [2..2] Transmit FIFO Empty                                                */
      __IM  uint32_t RFNE       : 1;            /*!< [3..3] Receive FIFO Not Empty                                             */
      __IM  uint32_t RFF        : 1;            /*!< [4..4] Receive FIFO Full                                                  */
            uint32_t            : 27;
    } USR_b;
  } ;
  __IM  uint32_t  TFL;                          /*!< (@ 0x00000080) UART TRANSMIT FIFO LEVEL                                   */
  __IM  uint32_t  RFL;                          /*!< (@ 0x00000084) UART RECEIVE FIFO LEVEL                                    */
  
  union {
    __OM  uint32_t SRR;                         /*!< (@ 0x00000088) UART SOFTWARE RESET REGISTER                               */
    
    struct {
      __OM  uint32_t UR         : 1;            /*!< [0..0] UART Reset                                                         */
      __OM  uint32_t RFR        : 1;            /*!< [1..1] RCVR FIFO Reset                                                    */
      __OM  uint32_t XFR        : 1;            /*!< [2..2] XMIT FIFO Reset                                                    */
            uint32_t            : 29;
    } SRR_b;
  } ;
} UART_Type;                                    /*!< Size = 140 (0x8c)                                                         */



/* =========================================================================================================================== */
/* ================                                           SPIC1                                           ================ */
/* =========================================================================================================================== */


/**
  * @brief SPIC Controller (SPIC1)
  */

typedef struct {                                /*!< (@ 0x40000000) SPIC1 Structure                                            */
  
  union {
    __IOM uint32_t CTRL0;                       /*!< (@ 0x00000000) CONTROL REGISTER #0                                        */
    
    struct {
            uint32_t            : 6;
      __IOM uint32_t SCPH       : 1;            /*!< [6..6] Serial Clock Phase                                                 */
      __IOM uint32_t SCPOL      : 1;            /*!< [7..7] Serial Clock Polarity                                              */
      __IOM uint32_t TMOD       : 2;            /*!< [9..8] Transfer Mode                                                      */
            uint32_t            : 6;
      __IOM uint32_t ADDRCH     : 2;            /*!< [17..16] Channel Number of Address Phase After Command Phase              */
      __IOM uint32_t DATACH     : 2;            /*!< [19..18] Channel Number of Data in Transmitting or Receiving
                                                     Data                                                                      */
      __IOM uint32_t CMDCH      : 2;            /*!< [21..20] Channel Number of Command Phase in Transmitting or
                                                     Receiving Data                                                            */
            uint32_t            : 9;
      __IOM uint32_t USERMD     : 1;            /*!< [31..31] User Mode Bit                                                    */
    } CTRL0_b;
  } ;
  
  union {
    __IOM uint32_t RXNDF;                       /*!< (@ 0x00000004) NUMBER OF RX DATA FRAME REGISTER                           */
    
    struct {
      __IOM uint32_t NUM        : 24;           /*!< [23..0] Number of Rx Data Frames                                          */
            uint32_t            : 8;
    } RXNDF_b;
  } ;
  
  union {
    __IOM uint32_t SSIENR;                      /*!< (@ 0x00000008) ENABLE REGISTER                                            */
    
    struct {
      __IOM uint32_t SPICEN     : 1;            /*!< [0..0] Set to Enable SPIC and Start User Mode Transaction                 */
      __IOM uint32_t ATCKCMD    : 1;            /*!< [1..1] Set to enable ATCK_CMD implementation                              */
            uint32_t            : 30;
    } SSIENR_b;
  } ;
  __IM  uint32_t  RESERVED;
  
  union {
    __IOM uint32_t SER;                         /*!< (@ 0x00000010) SELECT TARGET FLASH REGISTER                               */
    
    struct {
      __IOM uint32_t SEL        : 1;            /*!< [0..0] Select to SPI Flash                                                */
            uint32_t            : 31;
    } SER_b;
  } ;
  
  union {
    __IOM uint32_t BAUDR;                       /*!< (@ 0x00000014) BAUD RATE SELECT REGISTER                                  */
    
    struct {
      __IOM uint32_t SCKDV      : 12;           /*!< [11..0] Define SPI Clock Divider Value                                    */
            uint32_t            : 20;
    } BAUDR_b;
  } ;
  __IOM uint32_t  TXFTLR;                       /*!< (@ 0x00000018) TX FIFO THRESHOLD REGISTER                                 */
  __IOM uint32_t  RXFTLR;                       /*!< (@ 0x0000001C) RECEIVE FIFO THRESHOLD LEVEL                               */
  __IOM uint32_t  TXFLR;                        /*!< (@ 0x00000020) TRANSMIT FIFO LEVEL REGISTER                               */
  __IOM uint32_t  RXFLR;                        /*!< (@ 0x00000024) RECEIVE FIFO LEVEL REGISTER                                */
  
  union {
    __IOM uint32_t SR;                          /*!< (@ 0x00000028) STATUS REGISTER                                            */
    
    struct {
      __IM  uint32_t BUSY       : 1;            /*!< [0..0] SPIC busy flag                                                     */
      __IM  uint32_t TFNF       : 1;            /*!< [1..1] Transmit FIFO is not full                                          */
      __IM  uint32_t TFE        : 1;            /*!< [2..2] Transmit FIFO is empty                                             */
      __IM  uint32_t RFNE       : 1;            /*!< [3..3] Receive FIFO is not empty                                          */
      __IM  uint32_t RFF        : 1;            /*!< [4..4] Receive FIFO full                                                  */
      __IM  uint32_t TXE        : 1;            /*!< [5..5] Transmission error                                                 */
            uint32_t            : 26;
    } SR_b;
  } ;
  
  union {
    __IOM uint32_t IMR;                         /*!< (@ 0x0000002C) Interrupt Mask Register                                    */
    
    struct {
      __IOM uint32_t TXEIM      : 1;            /*!< [0..0] Transmit FIFO empty interrupt masked                               */
      __IOM uint32_t TXOIM      : 1;            /*!< [1..1] Transmit FIFO overflow interrupt mask                              */
      __IOM uint32_t RXUIM      : 1;            /*!< [2..2] Receive FIFO underflow interrupt masked                            */
      __IOM uint32_t RXOIM      : 1;            /*!< [3..3] Receive FIFO overflow interrupt masked                             */
      __IOM uint32_t RXFIM      : 1;            /*!< [4..4] Receive FIFO full interrupt masked                                 */
      __IOM uint32_t FSEIM      : 1;            /*!< [5..5] FIFO size error interrupt mask                                     */
            uint32_t            : 3;
      __IOM uint32_t USSIM      : 1;            /*!< [9..9] User-mode error interrupt mask                                     */
      __IOM uint32_t TFSIM      : 1;            /*!< [10..10] Transmit finish interrupt mask                                   */
            uint32_t            : 21;
    } IMR_b;
  } ;
  
  union {
    __IOM uint32_t ISR;                         /*!< (@ 0x00000030) INTERRUPT STATUS REGISTER                                  */
    
    struct {
      __IM  uint32_t TXEIS      : 1;            /*!< [0..0] Transmit FIFO empty interrupt status after masking                 */
      __IM  uint32_t TXOIS      : 1;            /*!< [1..1] Transmit FIFO overflow interrupt status after masking              */
      __IM  uint32_t RXUIS      : 1;            /*!< [2..2] Receive FIFO underflow interrupt status after masking              */
      __IM  uint32_t RXOIS      : 1;            /*!< [3..3] Receive FIFO overflow interrupt status after masking               */
      __IM  uint32_t RXFIS      : 1;            /*!< [4..4] Receive FIFO full interrupt status after masking                   */
      __IM  uint32_t FSEIS      : 1;            /*!< [5..5] FIFO size error interrupt status after masking                     */
            uint32_t            : 3;
      __IM  uint32_t USEIS      : 1;            /*!< [9..9] User mode error status after masking                               */
      __IM  uint32_t TFSIS      : 1;            /*!< [10..10] Transmit finish status after masking                             */
            uint32_t            : 21;
    } ISR_b;
  } ;
  
  union {
    __IOM uint32_t RISR;                        /*!< (@ 0x00000034) RAW INTERRUPT STATUS REGISTER                              */
    
    struct {
      __IM  uint32_t TXEIR      : 1;            /*!< [0..0] Transmit Fifo Empty Interrupt Status Before Masking                */
      __IM  uint32_t TXOIR      : 1;            /*!< [1..1] Transmit Fifo Overflow Interrupt Status Before Masking             */
      __IM  uint32_t RXUIR      : 1;            /*!< [2..2] Receive Fifo Underflow Interrupt Status Before Masking             */
      __IM  uint32_t RXOIR      : 1;            /*!< [3..3] Receive Fifo Overflow Interrupt Status Before Masking              */
      __IM  uint32_t RXFIR      : 1;            /*!< [4..4] Receive Fifo Full Interrupt Status Before Masking                  */
      __IM  uint32_t FSEIR      : 1;            /*!< [5..5] FIFO Size Error Interrupt Status Before Masking                    */
            uint32_t            : 3;
      __IM  uint32_t USEIR      : 1;            /*!< [9..9] User_mode Error Status Interrupt Status Before Masking             */
      __IM  uint32_t TFSIR      : 1;            /*!< [10..10] Transmit Finish Status Interrupt Status Before Masking           */
            uint32_t            : 21;
    } RISR_b;
  } ;
  __IOM uint32_t  TXOICR;                       /*!< (@ 0x00000038) TRANSMIT FIFO OVERFLOW INTERRUPT CLEAR REGISTER            */
  __IOM uint32_t  RXOICR;                       /*!< (@ 0x0000003C) RECEIVE FIFO OVERFLOW INTERRUPT CLEAR REGISTER             */
  __IOM uint32_t  RXUICR;                       /*!< (@ 0x00000040) RECEIVE FIFO UNDERFLOW INTERRUPT CLEAR REGISTER            */
  __IOM uint32_t  MSTICR;                       /*!< (@ 0x00000044) MASTER ERROR INTERRUPT CLEAR REGISTER                      */
  __IOM uint32_t  ICR;                          /*!< (@ 0x00000048) INTERRUPT CLEAR REGISTER                                   */
  __IM  uint32_t  RESERVED1[5];
  
  union {
    __IOM uint32_t DR;                          /*!< (@ 0x00000060) DATA REGISTER                                              */
    __IOM uint8_t DR_BYTE;                      /*!< (@ 0x00000060) DATA REGISTER IN BYTE                                      */
    __IOM uint16_t DR_HALF;                     /*!< (@ 0x00000060) DATA REGISTER IN HALF WORD                                 */
    __IOM uint32_t DR_WORD;                     /*!< (@ 0x00000060) DATA REGISTER IN WORD                                      */
  };
  __IM  uint32_t  RESERVED2[31];
  __IOM uint32_t  FRDCMD;                       /*!< (@ 0x000000E0) FAST READ DATA COMMAND REGISTER                            */
  __IM  uint32_t  RESERVED3[13];
  
  union {
    __IOM uint32_t USERLENGTH;                  /*!< (@ 0x00000118) DECIDES BYTE NUMBERS OF COMMAND, ADDRESS AND
                                                                    DATA PHASE TO TRANSMIT                                     */
    
    struct {
      __IOM uint32_t RDDUMMYLEN : 12;           /*!< [11..0] Indicate Delay Cycles For Receiving Data In User Mode             */
      __IOM uint32_t CMDLEN     : 2;            /*!< [13..12] Indicate Number Of Bytes In Command Phase In User Mode           */
            uint32_t            : 2;
      __IOM uint32_t ADDRLEN    : 4;            /*!< [19..16] Indicate Number Of Bytes In Address Phase In User Mode           */
            uint32_t            : 12;
    } USERLENGTH_b;
  } ;
  
  union {
    __IOM uint32_t AUTOLENGTH;                  /*!< (@ 0x0000011C) RX Delay Cycle and Address Size in Auto Mode               */
    
    struct {
      __IOM uint32_t DLYCYCLE   : 12;           /*!< [11..0] Indicate Delay Cycles For Receiving Data In Auto Mode             */
      __IOM uint32_t CYCLE      : 4;            /*!< [15..12] Indicate Clock Cycles In Auto Mode                               */
      __IOM uint32_t ADDRLEN    : 4;            /*!< [19..16] Indicate Number Of Bytes In Address Phase In Auto Mode           */
      __IOM uint32_t RDDUMMYLEN : 8;            /*!< [27..20] Indicate Number Of Dummy Bytes In Auto Mode after RX             */
      __IOM uint32_t CMDLEN     : 2;            /*!< [29..28] Indicate R/W CMD Bytes In Auto Mode                              */
            uint32_t            : 2;
    } AUTOLENGTH_b;
  } ;
  
  union {
    __IOM uint32_t VALIDCMD;                    /*!< (@ 0x00000120) Valid Command Register                                     */
    
    struct {
      __IOM uint32_t FRDSINGLE  : 1;            /*!< [0..0] Fast Read is a Valid Command to Execute                            */
      __IOM uint32_t RDDI       : 1;            /*!< [1..1] Dual Data Read is a Valid Command to Execute                       */
      __IOM uint32_t RDIO       : 1;            /*!< [2..2] Dual Address/Data Read is a Valid Command to Execute               */
      __IOM uint32_t RQO        : 1;            /*!< [3..3] Quad Data Read is a Valid Command to Execute                       */
      __IOM uint32_t RQIO       : 1;            /*!< [4..4] Quad Address/Data Read is a Valid Command to Execute               */
      __IOM uint32_t WD1        : 1;            /*!< [5..5] Dual Data Write is a Valid Command to Execute                      */
      __IOM uint32_t WD2        : 1;            /*!< [6..6] Dual Address/Data Write is a Valid Command to Execute              */
      __IOM uint32_t WRQ1       : 1;            /*!< [7..7] Quad Data Write is a Valid Command to Execute                      */
      __IOM uint32_t WRQ2       : 1;            /*!< [8..8] Quad Address/Data Write is a Valid Command to Execute              */
      __IOM uint32_t RMRDSR     : 1;            /*!< [9..9] Remove Read Status Register in Auto Write                          */
      __IOM uint32_t RMWEN      : 1;            /*!< [10..10] Remove Write Enable in Auto Write                                */
      __IOM uint32_t PRMEN      : 1;            /*!< [11..11] Enable SPI Performance Read Mode in Auto Write                   */
      __IOM uint32_t CTRL0CH    : 1;            /*!< [12..12] CMD_CH/DATA_CH/ADDR_CH and DDR_EN Field in Auto Mode             */
      __IOM uint32_t DUMEN      : 1;            /*!< [13..13] Push Dummy Byte after Pushing Address to Flash in Auto
                                                     Read                                                                      */
            uint32_t            : 2;
      __IOM uint32_t SEQRDDIS   : 1;            /*!< [16..16] Disable Sequential R/W in Auto Mode                              */
      __IOM uint32_t ADDPUIN    : 1;            /*!< [17..17] Pull in First Address to Negedge of spi_clk                      */
      __IOM uint32_t ADDSWAP    : 1;            /*!< [18..18] Enable Address Swap based on CR.FLASH_SIZE in Auto
                                                     Mode                                                                      */
            uint32_t            : 13;
    } VALIDCMD_b;
  } ;
  __IM  uint32_t  RESERVED4;
  
  union {
    __IOM uint32_t FLUSH;                       /*!< (@ 0x00000128) FLUSH FIFO REGISTER                                        */
    
    struct {
      __IOM uint32_t ALL        : 1;            /*!< [0..0] Clear All Data In All FIFO (Include TX_FIFO, RX_FIFO
                                                     and ST_FIFO)                                                              */
      __IOM uint32_t DRFIFO     : 1;            /*!< [1..1] Clear All Data in the TX FIFO and RX FIFO                          */
      __IOM uint32_t STFIFO     : 1;            /*!< [2..2] Clear All Data in the ST_FIFO                                      */
            uint32_t            : 29;
    } FLUSH_b;
  } ;
  __IM  uint32_t  RESERVED5;
  
  union {
    __IOM uint32_t TXNDF;                       /*!< (@ 0x00000130) A NUMBER OF DATA FRAMES OF TX DATA IN USER MODE            */
    
    struct {
      __IOM uint32_t NUM        : 24;           /*!< [23..0] Number of Rx Data Frames                                          */
            uint32_t            : 8;
    } TXNDF_b;
  } ;
  __IM  uint32_t  RESERVED6[9];
  
  union {
    __IOM uint32_t GATE;                        /*!< (@ 0x00000158) CLK GATING CONTROL REGISTER                                */
    
    struct {
            uint32_t            : 4;
      __IOM uint32_t AWGATE     : 1;            /*!< [4..4] Auto Write Clock Gating Enable                                     */
            uint32_t            : 27;
    } GATE_b;
  } ;
} SPIC_Type;                                    /*!< Size = 348 (0x15c)                                                        */



/* =========================================================================================================================== */
/* ================                                           SPIS                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief SPIS Controller (SPIS)
  */

typedef struct {                                /*!< (@ 0x40222400) SPIS Structure                                             */
  
  union {
    __IOM uint32_t SPISCFG;                     /*!< (@ 0x00000000) SPIS CONFIGURATION REGISTER                                */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] SPIS Enable Bit                                                    */
            uint32_t            : 1;
      __IOM uint32_t WEN        : 1;            /*!< [2..2] Write to Input Buffer Enable Bit                                   */
      __IOM uint32_t AUTIBF     : 1;            /*!< [3..3] Auto Input Buffer Full Indication Enable Bit                       */
      __IOM uint32_t AUTOBE     : 1;            /*!< [4..4] Auto Output Buffer Empty Indication Enable Bit                     */
      __IOM uint32_t DAS        : 1;            /*!< [5..5] Data After Satatus Enable Bit                                      */
      __IOM uint32_t CPOL       : 1;            /*!< [6..6] Clock Polarity Sample Bit                                          */
      __IOM uint32_t IWRAP      : 1;            /*!< [7..7] Input Buffer Wrap-Around Enable Bit                                */
      __IOM uint32_t SIMUL      : 1;            /*!< [8..8] Simultaneous Read/Write Enable Bit                                 */
      __IM  uint32_t BUSY       : 1;            /*!< [9..9] SPIS Busy Bit                                                      */
      __IOM uint32_t ONESHOT    : 1;            /*!< [10..10] Write One Shot Enable Bit                                        */
            uint32_t            : 21;
    } SPISCFG_b;
  } ;
  
  union {
    __IOM uint32_t INTEN;                       /*!< (@ 0x00000004) INTERRUPT ENABLE REGISTER                                  */
    
    struct {
      __IOM uint32_t OBEEN      : 1;            /*!< [0..0] Output Buffer Empty Interrupt Enable                               */
      __IOM uint32_t OBHEEN     : 1;            /*!< [1..1] Output Buffer Half Empty Interrupt Enable                          */
      __IOM uint32_t IBFEN      : 1;            /*!< [2..2] Input Buffer Full Interrupt Enable                                 */
      __IOM uint32_t IBHFEN     : 1;            /*!< [3..3] Input Buffer Half Full Interrupt Enable                            */
      __IOM uint32_t EOREN      : 1;            /*!< [4..4] End-of-Data for Read Transaction Interrupt Enable                  */
      __IOM uint32_t EOWEN      : 1;            /*!< [5..5] End-of-Data for Write Transaction Interrupt Enable                 */
      __IOM uint32_t STSREN     : 1;            /*!< [6..6] Status Read Interrupt Enable                                       */
      __IOM uint32_t IBHF2EN    : 1;            /*!< [7..7] Input Buffer Half Full 2 Interrupt Enable                          */
      __IOM uint32_t CSREEN     : 1;            /*!< [8..8] CS# Rising Edge Interrupt Enable                                   */
      __IOM uint32_t CSFEEN     : 1;            /*!< [9..9] CS# Falling Edge Interrupt Enable                                  */
            uint32_t            : 22;
    } INTEN_b;
  } ;
  
  union {
    __IOM uint32_t STATUS;                      /*!< (@ 0x00000008) STATUS REGISTER                                            */
    
    struct {
      __IOM uint32_t OBE        : 1;            /*!< [0..0] Output Buffer Empty                                                */
      __IOM uint32_t OBHE       : 1;            /*!< [1..1] Output Buffer Half Empty                                           */
      __IOM uint32_t IBF        : 1;            /*!< [2..2] Input Buffer Full                                                  */
      __IOM uint32_t IBHF       : 1;            /*!< [3..3] Input Buffer Half Full                                             */
      __IOM uint32_t EOR        : 1;            /*!< [4..4] End-of-Data for Read Transaction                                   */
      __IOM uint32_t EOW        : 1;            /*!< [5..5] End-of-Data for Write Transaction                                  */
      __IOM uint32_t STSR       : 1;            /*!< [6..6] Device Status Read                                                 */
      __IOM uint32_t IBHF2      : 1;            /*!< [7..7] Input Buffe Half Full 2                                            */
      __IOM uint32_t CSRE       : 1;            /*!< [8..8] CS# Rising Edge                                                    */
      __IOM uint32_t CSFE       : 1;            /*!< [9..9] CS# Falling Edge                                                   */
            uint32_t            : 22;
    } STATUS_b;
  } ;
  
  union {
    __IM  uint32_t CAPABILITY;                  /*!< (@ 0x0000000C) ADDRESS REGISTER                                           */
    
    struct {
      __IM  uint32_t IBSZ       : 4;            /*!< [3..0] Input Buffer Size                                                  */
      __IM  uint32_t OBSZ       : 4;            /*!< [7..4] Output Buffer Size                                                 */
            uint32_t            : 24;
    } CAPABILITY_b;
  } ;
  
  union {
    __IM  uint32_t IBFPTR;                      /*!< (@ 0x00000010) INPUT BUFFER POINTER REGISTER                              */
    
    struct {
      __IM  uint32_t IBFPTR     : 8;            /*!< [7..0] Input Buffer Pointer                                               */
            uint32_t            : 24;
    } IBFPTR_b;
  } ;
  
  union {
    __IM  uint32_t OBFPTR;                      /*!< (@ 0x00000014) OUTPUT BUFFER POINTER REGISTER                             */
    
    struct {
      __IM  uint32_t OBFPTR     : 8;            /*!< [7..0] Output Buffer Pointer                                              */
            uint32_t            : 24;
    } OBFPTR_b;
  } ;
  
  union {
    __IOM uint32_t OBFTHR;                      /*!< (@ 0x00000018) OUTPUT BUFFER THRESHOLD REGISTER                           */
    
    struct {
      __IOM uint32_t OBFLV      : 7;            /*!< [6..0] Output Buffer Level                                                */
      __IOM uint32_t OBFLVDIS   : 1;            /*!< [7..7] Threshold Disable Bit                                              */
            uint32_t            : 24;
    } OBFTHR_b;
  } ;
  
  union {
    __IOM uint32_t IBFTHR1;                     /*!< (@ 0x0000001C) OUTPUT BUFFER THRESHOLD REGISTER                           */
    
    struct {
      __IOM uint32_t OBFLV      : 7;            /*!< [6..0] Output Buffer Level                                                */
      __IOM uint32_t OBFLVDIS   : 1;            /*!< [7..7] Threshold Disable Bit                                              */
            uint32_t            : 24;
    } IBFTHR1_b;
  } ;
  
  union {
    __IOM uint32_t IBFTHR2;                     /*!< (@ 0x00000020) OUTPUT BUFFER THRESHOLD REGISTER                           */
    
    struct {
      __IOM uint32_t OBFLV      : 7;            /*!< [6..0] Output Buffer Level                                                */
      __IOM uint32_t OBFLVDIS   : 1;            /*!< [7..7] Threshold Disable Bit                                              */
            uint32_t            : 24;
    } IBFTHR2_b;
  } ;
  __IM  uint32_t  IBF[128];                     /*!< (@ 0x00000024) INPUT BUFFER REGISTER                                      */
  __IOM uint32_t  OBF[128];                     /*!< (@ 0x00000224) OUTPUT BUFFER REGISTER                                     */
  
  union {
    __IOM uint32_t DEVSTS;                      /*!< (@ 0x00000424) DEVICE STATUS REGISTER                                     */
    
    struct {
            uint32_t            : 6;
      __IOM uint32_t OBES       : 1;            /*!< [6..6] Output Buffer Empty Bit                                            */
      __IOM uint32_t IBFS       : 1;            /*!< [7..7] Input Buffer Full Bit                                              */
            uint32_t            : 24;
    } DEVSTS_b;
  } ;
} SPIS_Type;                                    /*!< Size = 1064 (0x428)                                                       */



/* =========================================================================================================================== */
/* ================                                           T1HZ                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief Timer 1 Hz (T1HZ)
  */

typedef struct {                                /*!< (@ 0x40010830) T1HZ Structure                                             */
  __IOM uint32_t  T1LDCNT;                      /*!< (@ 0x00000000) TIMER 1 Hz LOAD COUNT REGISTER                             */
  __IOM uint32_t  T1CRCNT;                      /*!< (@ 0x00000004) TIMER 1 Hz CURRENT COUNT REGISTER                          */
  
  union {
    __IOM uint32_t T1CTRL;                      /*!< (@ 0x00000008) TIMER 1 Hz CONTROL REGISTER                                */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Timer 1 Hz Enable                                                  */
      __IOM uint32_t RST        : 1;            /*!< [1..1] Timer 1 Hz Reset                                                   */
            uint32_t            : 28;
      __IOM uint32_t ENSETCNT   : 1;            /*!< [30..30] Enable Setting Timer 1 Hz Counter                                */
      __IOM uint32_t ENCPTEST   : 1;            /*!< [31..31] Enable Timer 1 Hz CP Test                                        */
    } T1CTRL_b;
  } ;
  __IOM uint32_t  SETT1CNT;                     /*!< (@ 0x0000000C) SET TIMER 1 Hz CURRENT COUNT REGISTER                      */
} T1HZ_Type;                                    /*!< Size = 16 (0x10)                                                          */



/* =========================================================================================================================== */
/* ================                                           EFUSE                                           ================ */
/* =========================================================================================================================== */


/**
  * @brief eFuse Controller (EFUSE)
  */

typedef struct {                                /*!< (@ 0x40010900) EFUSE Structure                                            */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000000) CONTROL REGISTER                                           */
    
    struct {
            uint32_t            : 27;
      __IOM uint32_t ATLDTRIG   : 1;            /*!< [27..27] Trigger auto-load                                                */
      __IOM uint32_t PS         : 1;            /*!< [28..28] Power Switch                                                     */
      __IOM uint32_t PD         : 1;            /*!< [29..29] Power Down                                                       */
      __IOM uint32_t MODE       : 1;            /*!< [30..30] Operation Mode                                                   */
      __IOM uint32_t EN         : 1;            /*!< [31..31] Enable eFuse                                                     */
    } CTRL_b;
  } ;
  
  union {
    __IOM uint32_t ADDR;                        /*!< (@ 0x00000004) ADDRESS REGISTER                                           */
    
    struct {
      __IOM uint32_t IDX        : 6;            /*!< [5..0] Index of eFuse                                                     */
      __IM  uint32_t ATLDEN     : 1;            /*!< [6..6] Enable Auto-load                                                   */
      __IOM uint32_t ATLDDN     : 1;            /*!< [7..7] Auto-load Done                                                     */
      __IOM uint32_t STRIDX     : 6;            /*!< [13..8] Start Index of auto-load operation                                */
            uint32_t            : 18;
    } ADDR_b;
  } ;
  __IOM uint32_t  PGDATA;                       /*!< (@ 0x00000008) PROGRAM DATA REGISTER                                      */
  __IM  uint32_t  RDDATA;                       /*!< (@ 0x0000000C) READ DATA REGISTER                                         */
  
  union {
    __IOM uint32_t PROGT;                       /*!< (@ 0x00000010) PROGRAM TIMING REGISTER                                    */
    
    struct {
      __IOM uint32_t PTPP       : 12;           /*!< [11..0] Tpp                                                               */
            uint32_t            : 4;
      __IOM uint32_t PTPI       : 8;            /*!< [23..16] Tpi                                                              */
      __IOM uint32_t PTEPS      : 8;            /*!< [31..24] Teps                                                             */
    } PROGT_b;
  } ;
  
  union {
    __IOM uint32_t RDT;                         /*!< (@ 0x00000014) READ TIMING REGISTER                                       */
    
    struct {
      __IOM uint32_t RTESR      : 8;            /*!< [7..0] Tesr                                                               */
      __IOM uint32_t RTRPW      : 8;            /*!< [15..8] Trpw                                                              */
      __IOM uint32_t RTRC       : 8;            /*!< [23..16] Trc                                                              */
            uint32_t            : 8;
    } RDT_b;
  } ;
} EFUSE_Type;                                   /*!< Size = 24 (0x18)                                                          */



/* =========================================================================================================================== */
/* ================                                          SYSTEM                                           ================ */
/* =========================================================================================================================== */


/**
  * @brief System Controller (SYSTEM)
  */

typedef struct {                                /*!< (@ 0x40100000) SYSTEM Structure                                           */
  
  union {
    __IOM uint32_t SYSCLK;                      /*!< (@ 0x00000000) SYSCLK POWER ENABLE REGISTER                               */
    
    struct {
            uint32_t            : 2;
      __IOM uint32_t _USB       : 1;            /*!< [2..2] _USB Power                                                         */
      __IOM uint32_t _LC        : 1;            /*!< [3..3] _LC Power                                                          */
      __IOM uint32_t _XTAL32    : 1;            /*!< [4..4] _XTAL32 Power                                                      */
      __IOM uint32_t _RC32KREF  : 1;            /*!< [5..5] _RC32KREF Power                                                    */
      __IOM uint32_t _PECI      : 1;            /*!< [6..6] _PECI Power                                                        */
      __IOM uint32_t _PLL       : 1;            /*!< [7..7] _PLL Power                                                         */
      __IOM uint32_t _RC25MREF  : 1;            /*!< [8..8] RC25MREF Power                                                     */
      __IOM uint32_t _RC25MCAL  : 1;            /*!< [9..9] _RC25M Calibration Power                                           */
      __IOM uint32_t _RC25M     : 1;            /*!< [10..10] _RC25M Power                                                     */
      __IOM uint32_t _LED_PWM   : 1;            /*!< [11..11] _LED_PWM Power                                                   */
      __IOM uint32_t _I2C       : 1;            /*!< [12..12] _I2C Power                                                       */
      __IOM uint32_t _LOWCLK    : 1;            /*!< [13..13] _LOWCLK Power                                                    */
      __IOM uint32_t _ESPI      : 1;            /*!< [14..14] _ESPI Power                                                      */
      __IOM uint32_t _APB       : 1;            /*!< [15..15] _APB Power                                                       */
            uint32_t            : 16;
    } SYSCLK_b;
  } ;
  
  union {
    __IOM uint32_t SYSCLKSEL;                   /*!< (@ 0x00000004) SYSTEM CLK SELECT REGISTER                                 */
    
    struct {
            uint32_t            : 4;
      __IOM uint32_t S_LALU     : 2;            /*!< [5..4] select LALU source                                                 */
      __IOM uint32_t S_RC32KPAD : 2;            /*!< [7..6] select 32K clock pad source                                        */
            uint32_t            : 12;
      __IOM uint32_t _LED_PWM   : 2;            /*!< [21..20] select _LED_PWM clock source                                     */
      __IOM uint32_t _I2C       : 2;            /*!< [23..22] _I2C Clock Select                                                */
      __IOM uint32_t LOWCLK     : 2;            /*!< [25..24] LOW Clock Select                                                 */
      __IOM uint32_t _ESPI      : 2;            /*!< [27..26] _ESPI Clock Select                                               */
      __IOM uint32_t APB        : 2;            /*!< [29..28] APB Clock Select                                                 */
      __IOM uint32_t _SYSTEM    : 2;            /*!< [31..30] _SYSTEM Clock Select                                             */
    } SYSCLKSEL_b;
  } ;
  
  union {
    __IOM uint32_t IPCLK0;                      /*!< (@ 0x00000008) IP_CLOCK_0 POWER REGISTER                                  */
    
    struct {
      __IOM uint32_t _PWM8      : 1;            /*!< [0..0] _PWM8 IP Clock POWER                                               */
      __IOM uint32_t _PWM7      : 1;            /*!< [1..1] _PWM7 IP Clock Power                                               */
      __IOM uint32_t _PWM6      : 1;            /*!< [2..2] _PWM6 IP Clock Power                                               */
      __IOM uint32_t _PWM5      : 1;            /*!< [3..3] _PWM5 IP Clock Power                                               */
      __IOM uint32_t _PWM4      : 1;            /*!< [4..4] _PWM4 IP Clock Power                                               */
      __IOM uint32_t _PWM3      : 1;            /*!< [5..5] _PWM3 IP Clock Power                                               */
      __IOM uint32_t _PWM2      : 1;            /*!< [6..6] _PWM2 IP Clock Power                                               */
      __IOM uint32_t _PWM1      : 1;            /*!< [7..7] _PWM1 IP Clock Power                                               */
      __IOM uint32_t _LED4      : 1;            /*!< [8..8] _LED4 IP Clock Power                                               */
      __IOM uint32_t _LED3      : 1;            /*!< [9..9] _LED3 IP Clock Power                                               */
      __IOM uint32_t _LED2      : 1;            /*!< [10..10] _LED2 IP Clock Power                                             */
      __IOM uint32_t _LED1      : 1;            /*!< [11..11] _LED1 IP Clock Power                                             */
      __IOM uint32_t _EMI7      : 1;            /*!< [12..12] _EMI7 IP Clock Power                                             */
      __IOM uint32_t _EMI6      : 1;            /*!< [13..13] _EMI6 IP Clock Power                                             */
      __IOM uint32_t _EMI5      : 1;            /*!< [14..14] _EMI5 IP Clock Power                                             */
      __IOM uint32_t _EMI4      : 1;            /*!< [15..15] _EMI4 IP Clock Power                                             */
      __IOM uint32_t _EMI3      : 1;            /*!< [16..16] _EMI3 IP Clock Power                                             */
      __IOM uint32_t _EMI2      : 1;            /*!< [17..17] _EMI2 IP Clock Power                                             */
      __IOM uint32_t _EMI1      : 1;            /*!< [18..18] _EMI1 IP Clock Power                                             */
      __IOM uint32_t _EMI0      : 1;            /*!< [19..19] _EMI0 IP Clock Power                                             */
      __IOM uint32_t _PORT80    : 1;            /*!< [20..20] _PORT80 IP Clock Power                                           */
      __IOM uint32_t _PMPORT3   : 1;            /*!< [21..21] _PMPORT3 IP Clock Power                                          */
      __IOM uint32_t _PMPORT2   : 1;            /*!< [22..22] _PMPORT2 IP Clock Power                                          */
      __IOM uint32_t _PMPORT1   : 1;            /*!< [23..23] _PMPORT1 IP Clock Power                                          */
      __IOM uint32_t _PMPORT0   : 1;            /*!< [24..24] _PMPORT0 IP Clock Power                                          */
      __IOM uint32_t _ACPI      : 1;            /*!< [25..25] _ACPI IP Clock Power                                             */
      __IOM uint32_t _KBC       : 1;            /*!< [26..26] _KBC IP Clock Power                                              */
      __IOM uint32_t _ESPI      : 1;            /*!< [27..27] _ESPI IP Clock Power                                             */
      __IOM uint32_t _DMA       : 1;            /*!< [28..28] _DMA IP Clock Power                                              */
      __IOM uint32_t _OTP       : 1;            /*!< [29..29] _OTP IP Clock Power                                              */
      __IOM uint32_t KR4        : 1;            /*!< [30..30] KR4 IP Clock Power                                               */
      __IOM uint32_t _BUS       : 1;            /*!< [31..31] _BUS IP Clock Power                                              */
    } IPCLK0_b;
  } ;
  
  union {
    __IOM uint32_t IPCLK1;                      /*!< (@ 0x0000000C) IP_CLOCK_1 POWER EREGISTER                                 */
    
    struct {
      __IOM uint32_t _TACH5     : 1;            /*!< [0..0] _TACH5 IP Clock Power                                              */
      __IOM uint32_t _TACH4     : 1;            /*!< [1..1] _TACH4 IP Clock Power                                              */
      __IOM uint32_t _TACH3     : 1;            /*!< [2..2] _TACH3 IP Clock Power                                              */
      __IOM uint32_t _TACH2     : 1;            /*!< [3..3] _TACH2 IP Clock Power                                              */
      __IOM uint32_t _TACH1     : 1;            /*!< [4..4] _TACH1 IP Clock Power                                              */
      __IOM uint32_t _PS2_H3    : 1;            /*!< [5..5] _PS2_H3 IP Clock Power                                             */
      __IOM uint32_t _PS2_H2    : 1;            /*!< [6..6] _PS2_H2 IP Clock Power                                             */
      __IOM uint32_t _PS2_H1    : 1;            /*!< [7..7] _PS2_H1 IP Clock Power                                             */
            uint32_t            : 3;
      __IOM uint32_t _I2C11     : 1;            /*!< [11..11] _I2C11 IP Clock Power                                            */
      __IOM uint32_t _I2C10     : 1;            /*!< [12..12] _I2C10 IP Clock Power                                            */
      __IOM uint32_t _I2C9      : 1;            /*!< [13..13] _I2C9 IP Clock Power                                             */
      __IOM uint32_t _I2C8      : 1;            /*!< [14..14] _I2C8 IP Clock Power                                             */
      __IOM uint32_t _I2C7      : 1;            /*!< [15..15] _I2C7 IP Clock Power                                             */
      __IOM uint32_t _I2C6      : 1;            /*!< [16..16] _I2C6 IP Clock Power                                             */
      __IOM uint32_t _I2C5      : 1;            /*!< [17..17] _I2C5 IP Clock Power                                             */
      __IOM uint32_t _I2C4      : 1;            /*!< [18..18] _I2C4 IP Clock Power                                             */
      __IOM uint32_t _I2C3      : 1;            /*!< [19..19] _I2C3 IP Clock Power                                             */
      __IOM uint32_t _I2C2      : 1;            /*!< [20..20] _I2C2 IP Clock Power                                             */
      __IOM uint32_t _I2C1      : 1;            /*!< [21..21] _I2C1 IP Clock Power                                             */
      __IOM uint32_t _UART2     : 1;            /*!< [22..22] _UART2 IP Clock Power                                            */
      __IOM uint32_t _UART1     : 1;            /*!< [23..23] _UART1 IP Clock Power                                            */
      __IOM uint32_t _UART0     : 1;            /*!< [24..24] _UART0 IP Clock Power                                            */
      __IOM uint32_t _ADC       : 1;            /*!< [25..25] _ADC IP Clock Power                                              */
      __IOM uint32_t _PWM14     : 1;            /*!< [26..26] _PWM14 IP Clock Power                                            */
      __IOM uint32_t _PWM13     : 1;            /*!< [27..27] _PWM13 IP Clock Power                                            */
      __IOM uint32_t _PWM12     : 1;            /*!< [28..28] _PWM12 IP Clock Power                                            */
      __IOM uint32_t _PWM11     : 1;            /*!< [29..29] _PWM11 IP Clock Power                                            */
      __IOM uint32_t _PWM10     : 1;            /*!< [30..30] _PWM10 IP Clock Power                                            */
      __IOM uint32_t _PWM9      : 1;            /*!< [31..31] _PWM9 IP Clock Power                                             */
    } IPCLK1_b;
  } ;
  
  union {
    __IOM uint32_t IPCLK2;                      /*!< (@ 0x00000010) IP_CLOCK_2 POWER EREGISTER                                 */
    
    struct {
      __IOM uint32_t _I3C2      : 1;            /*!< [0..0] _I3C2 IP Clock Power                                               */
      __IOM uint32_t _I3C1      : 1;            /*!< [1..1] _I3C1 IP Clock Power                                               */
      __IOM uint32_t _I3C4      : 1;            /*!< [2..2] _I3C4 IP Clock Power                                               */
      __IOM uint32_t _LPT       : 1;            /*!< [3..3] _LPT IP Clock Power                                                */
      __IOM uint32_t _VINLED    : 1;            /*!< [4..4] _VINLED IP Clock Power                                             */
      __IOM uint32_t _I2CDBG    : 1;            /*!< [5..5] _I2CDBG IP Clock Power                                             */
      __IOM uint32_t _MICROCNT  : 1;            /*!< [6..6] _MICROCNT IP Clock Power                                           */
      __IOM uint32_t _CEC2      : 1;            /*!< [7..7] _CEC2 IP Clock Power                                               */
      __IOM uint32_t _CEC1      : 1;            /*!< [8..8] _CEC1 IP Clock Power                                               */
      __IOM uint32_t _LEDSTP    : 1;            /*!< [9..9] _LEDSTP IP Clock Power                                             */
      __IOM uint32_t _SLWTMR3   : 1;            /*!< [10..10] _SLWTMR3 IP Clock Power                                          */
      __IOM uint32_t _SLWTMR2   : 1;            /*!< [11..11] _SLWTMR2 IP Clock Power                                          */
      __IOM uint32_t _SLWTMR1   : 1;            /*!< [12..12] _SLWTMR1 IP Clock Power                                          */
      __IOM uint32_t _SLWTMR0   : 1;            /*!< [13..13] _SLWTMR0 IP Clock Power                                          */
            uint32_t            : 1;
      __IOM uint32_t _KBM       : 1;            /*!< [15..15] _KBM IP Clock Power                                              */
      __IOM uint32_t _TMR5      : 1;            /*!< [16..16] _TMR5 IP Clock Power                                             */
      __IOM uint32_t _TMR4      : 1;            /*!< [17..17] _TMR4 IP Clock Power                                             */
      __IOM uint32_t _TMR3      : 1;            /*!< [18..18] _TMR3 IP Clock Power                                             */
      __IOM uint32_t _TMR2      : 1;            /*!< [19..19] _TMR2 IP Clock Power                                             */
      __IOM uint32_t _TMR1      : 1;            /*!< [20..20] _TMR1 IP Clock Power                                             */
      __IOM uint32_t _TMR0      : 1;            /*!< [21..21] _TMR0 IP Clock Power                                             */
      __IOM uint32_t _GPIO      : 1;            /*!< [22..22] _GPIO IP Clock Power                                             */
      __IOM uint32_t _TACH14    : 1;            /*!< [23..23] _TACH14 IP Clock Power                                           */
      __IOM uint32_t _TACH13    : 1;            /*!< [24..24] _TACH13 IP Clock Power                                           */
      __IOM uint32_t _TACH12    : 1;            /*!< [25..25] _TACH12 IP Clock Power                                           */
      __IOM uint32_t _TACH11    : 1;            /*!< [26..26] _TACH11 IP Clock Power                                           */
      __IOM uint32_t _TACH10    : 1;            /*!< [27..27] _TACH10 IP Clock Power                                           */
      __IOM uint32_t _TACH9     : 1;            /*!< [28..28] _TACH9 IP Clock Power                                            */
      __IOM uint32_t _TACH8     : 1;            /*!< [29..29] _TACH8 IP Clock Power                                            */
      __IOM uint32_t _TACH7     : 1;            /*!< [30..30] _TACH7 IP Clock Power                                            */
      __IOM uint32_t _TACH6     : 1;            /*!< [31..31] _TACH6 IP Clock Power                                            */
    } IPCLK2_b;
  } ;
  
  union {
    union {
      __IOM uint32_t APBCLK0;                   /*!< (@ 0x00000014) APB_CLOCK_0 POWER EREGISTER                                */
      
      struct {
        __IOM uint32_t _PS2_3   : 1;            /*!< [0..0] _PS2_3 APB Clock Power                                             */
        __IOM uint32_t _PS2_2   : 1;            /*!< [1..1] _PS2_2 APB Clock Power                                             */
        __IOM uint32_t _PS2_1   : 1;            /*!< [2..2] _PS2_1 APB Clock Power                                             */
        __IOM uint32_t _LEDSTP  : 1;            /*!< [3..3] _LEDSTP APB Clock Power                                            */
        __IOM uint32_t _LED4    : 1;            /*!< [4..4] _LED4 APB Clock Power                                              */
        __IOM uint32_t _LED3    : 1;            /*!< [5..5] _LED3 APB Clock Power                                              */
        __IOM uint32_t _LED2    : 1;            /*!< [6..6] _LED2 APB Clock Power                                              */
        __IOM uint32_t _LED1    : 1;            /*!< [7..7] _LED1 APB Clock Power                                              */
        __IOM uint32_t _KBM     : 1;            /*!< [8..8] _KBM APB Clock Power                                               */
        __IOM uint32_t _I2CDBG  : 1;            /*!< [9..9] _I2CDBG APB Clock Power                                            */
        __IOM uint32_t _I2C11   : 1;            /*!< [10..10] _I2C11 APB Clock Power                                           */
        __IOM uint32_t _I2C10   : 1;            /*!< [11..11] _I2C10 APB Clock Power                                           */
        __IOM uint32_t _I2C9    : 1;            /*!< [12..12] _I2C9 APB Clock Power                                            */
        __IOM uint32_t _I2C8    : 1;            /*!< [13..13] _I2C8 APB Clock Power                                            */
        __IOM uint32_t _I2C7    : 1;            /*!< [14..14] _I2C7 APB Clock Power                                            */
        __IOM uint32_t _I2C6    : 1;            /*!< [15..15] _I2C6 APB Clock Power                                            */
        __IOM uint32_t _I2C5    : 1;            /*!< [16..16] _I2C5 APB Clock Power                                            */
        __IOM uint32_t _I2C4    : 1;            /*!< [17..17] _I2C4 APB Clock Power                                            */
        __IOM uint32_t _I2C3    : 1;            /*!< [18..18] _I2C3 APB Clock Power                                            */
        __IOM uint32_t _I2C2    : 1;            /*!< [19..19] _I2C2 APB Clock Power                                            */
        __IOM uint32_t _I2C1    : 1;            /*!< [20..20] _I2C1 APB Clock Power                                            */
        __IOM uint32_t _GPIO    : 1;            /*!< [21..21] _GPIO APB Clock Power                                            */
        __IOM uint32_t _CEC2    : 1;            /*!< [22..22] _CEC2 APB Clock Power                                            */
        __IOM uint32_t _CEC1    : 1;            /*!< [23..23] _CEC1 APB Clock Power                                            */
        __IOM uint32_t _ADC     : 1;            /*!< [24..24] _ADC APB Clock Power                                             */
              uint32_t          : 7;
      } APBCLK0_b;
    } ;
    
    union {
      __IOM uint32_t IPCLK3;                    /*!< (@ 0x00000014) IP_CLOCK_3 POWER EREGISTER                                 */
      
      struct {
              uint32_t          : 25;
        __IOM uint32_t _RTMR    : 1;            /*!< [25..25] _RTMR IP Clock Power                                             */
        __IOM uint32_t _PS2_L3  : 1;            /*!< [26..26] _PS2_L3 IP Clock Power                                           */
        __IOM uint32_t _PS2_L2  : 1;            /*!< [27..27] _PS2_L2 IP Clock Power                                           */
        __IOM uint32_t _PS2_L1  : 1;            /*!< [28..28] _PS2_L1 Clock Power                                              */
        __IOM uint32_t _RC32KREF : 1;           /*!< [29..29] _RC32KREF IP Clock Power                                         */
        __IOM uint32_t _PECI    : 1;            /*!< [30..30] _PECI IP Clock Power                                             */
        __IOM uint32_t _I3C3    : 1;            /*!< [31..31] _I3C3 IP Clock Power                                             */
      } IPCLK3_b;
    } ;
  };
  
  union {
    __IOM uint32_t APBCLK1;                     /*!< (@ 0x00000018) APB_CLOCK_1 POWER EREGISTER                                */
    
    struct {
      __IOM uint32_t _TACH8     : 1;            /*!< [0..0] _TACH8 APB Clock Power                                             */
      __IOM uint32_t _TACH7     : 1;            /*!< [1..1] _TACH7 APB Clock Power                                             */
      __IOM uint32_t _TACH6     : 1;            /*!< [2..2] _TACH6 APB Clock Power                                             */
      __IOM uint32_t _TACH5     : 1;            /*!< [3..3] _TACH5 APB Clock Power                                             */
      __IOM uint32_t _TACH4     : 1;            /*!< [4..4] _TACH4 APB Clock Power                                             */
      __IOM uint32_t _TACH3     : 1;            /*!< [5..5] _TACH3 APB Clock Power                                             */
      __IOM uint32_t _TACH2     : 1;            /*!< [6..6] _TACH2 APB Clock Power                                             */
      __IOM uint32_t _TACH1     : 1;            /*!< [7..7] _TACH1 APB Clock Power                                             */
      __IOM uint32_t _SPIM      : 1;            /*!< [8..8] _SPIM APB Clock Power                                              */
      __IOM uint32_t _SPIS      : 1;            /*!< [9..9] _SPIS APB Clock Power                                              */
      __IOM uint32_t _SLWTMR3   : 1;            /*!< [10..10] _SLWTMR3 APB Clock Power                                         */
      __IOM uint32_t _SLWTMR2   : 1;            /*!< [11..11] _SLWTMR2 APB Clock Power                                         */
      __IOM uint32_t _SLWTMR1   : 1;            /*!< [12..12] _SLWTMR1 APB Clock Power                                         */
      __IOM uint32_t _SLWTMR0   : 1;            /*!< [13..13] _SLWTMR0 APB Clock Power                                         */
      __IOM uint32_t _RTMR      : 1;            /*!< [14..14] _RTMR APB Clock Power                                            */
      __IOM uint32_t _RTC       : 1;            /*!< [15..15] _RTC APB Clock Power                                             */
      __IOM uint32_t _PWRBTN1   : 1;            /*!< [16..16] _PWRBTN1 APB Clock Power                                         */
      __IOM uint32_t _PWRBTN0   : 1;            /*!< [17..17] _PWRBTN0 APB Clock Power                                         */
      __IOM uint32_t _PWM14     : 1;            /*!< [18..18] _PWM14 APB Clock Power                                           */
      __IOM uint32_t _PWM13     : 1;            /*!< [19..19] _PWM13 APB Clock Power                                           */
      __IOM uint32_t _PWM12     : 1;            /*!< [20..20] _PWM12 APB Clock Power                                           */
      __IOM uint32_t _PWM11     : 1;            /*!< [21..21] _PWM11 APB Clock Power                                           */
      __IOM uint32_t _PWM10     : 1;            /*!< [22..22] _PWM10 APB Clock Power                                           */
      __IOM uint32_t _PWM9      : 1;            /*!< [23..23] _PWM9 APB Clock Power                                            */
      __IOM uint32_t _PWM8      : 1;            /*!< [24..24] _PWM8 APB Clock Power                                            */
      __IOM uint32_t _PWM7      : 1;            /*!< [25..25] _PWM7 APB Clock Power                                            */
      __IOM uint32_t _PWM6      : 1;            /*!< [26..26] _PWM6 APB Clock Power                                            */
      __IOM uint32_t _PWM5      : 1;            /*!< [27..27] _PWM5 APB Clock Power                                            */
      __IOM uint32_t _PWM4      : 1;            /*!< [28..28] _PWM4 APB Clock Power                                            */
      __IOM uint32_t _PWM3      : 1;            /*!< [29..29] _PWM3 APB Clock Power                                            */
      __IOM uint32_t _PWM2      : 1;            /*!< [30..30] _PWM2 APB Clock Power                                            */
      __IOM uint32_t _PWM1      : 1;            /*!< [31..31] _PWM1 APB Clock Power                                            */
    } APBCLK1_b;
  } ;
  
  union {
    union {
      __IOM uint32_t IPCLK4;                    /*!< (@ 0x0000001C) IP_CLOCK_4 POWER EREGISTER                                 */
      
      struct {
              uint32_t          : 5;
        __IOM uint32_t _USBDPHY : 1;            /*!< [5..5] _USBDPHY IP Clock Power                                            */
        __IOM uint32_t _USB     : 1;            /*!< [6..6] _USB IP Clock Power                                                */
        __IOM uint32_t _TRC_CLK : 1;            /*!< [7..7] _TRC_CLK IP Clock Power                                            */
        __IOM uint32_t _LALU    : 1;            /*!< [8..8] _LALU IP Clock Power                                               */
              uint32_t          : 23;
      } IPCLK4_b;
    } ;
    
    union {
      __IOM uint32_t APBCLK2;                   /*!< (@ 0x0000001C) APB_CLOCK_2 POWER EREGISTER                                */
      
      struct {
              uint32_t          : 9;
        __IOM uint32_t _TRC     : 1;            /*!< [9..9] _TRC APB Clock Power                                               */
        __IOM uint32_t _PECI    : 1;            /*!< [10..10] _PECI APB Clock Power                                            */
        __IOM uint32_t _LPT     : 1;            /*!< [11..11] _LPT APB Clock Power                                             */
        __IOM uint32_t _WDT     : 1;            /*!< [12..12] _WDT APB Clock Power                                             */
        __IOM uint32_t _UART2   : 1;            /*!< [13..13] _UART2 APB Clock Power                                           */
        __IOM uint32_t _UART1   : 1;            /*!< [14..14] _UART1 APB Clock Power                                           */
        __IOM uint32_t _UART0   : 1;            /*!< [15..15] _UART0 APB Clock Power                                           */
        __IOM uint32_t _TMR5    : 1;            /*!< [16..16] _TMR5 APB Clock Power                                            */
        __IOM uint32_t _TMR4    : 1;            /*!< [17..17] _TMR4 APB Clock Power                                            */
        __IOM uint32_t _TMR3    : 1;            /*!< [18..18] _TMR3 APB Clock Power                                            */
        __IOM uint32_t _TMR2    : 1;            /*!< [19..19] _TMR2 APB Clock Power                                            */
        __IOM uint32_t _TMR1    : 1;            /*!< [20..20] _TMR1 APB Clock Power                                            */
        __IOM uint32_t _TMR0    : 1;            /*!< [21..21] _TMR0 APB Clock Power                                            */
        __IOM uint32_t _TACH14  : 1;            /*!< [22..22] _TACH14 APB Clock Power                                          */
        __IOM uint32_t _TACH13  : 1;            /*!< [23..23] _TACH13 APB Clock Power                                          */
        __IOM uint32_t _TACH12  : 1;            /*!< [24..24] _TACH12 APB Clock Power                                          */
        __IOM uint32_t _TACH11  : 1;            /*!< [25..25] _TACH11 APB Clock Power                                          */
        __IOM uint32_t _TACH10  : 1;            /*!< [26..26] _TACH10 APB Clock Power                                          */
        __IOM uint32_t _TACH9   : 1;            /*!< [27..27] _TACH9 APB Clock Power                                           */
              uint32_t          : 4;
      } APBCLK2_b;
    } ;
  };
  
  union {
    __IOM uint32_t IPDIV0;                      /*!< (@ 0x00000020) IP_CLOCK_DIVIDER_0 REGISTER                                */
    
    struct {
      __IOM uint32_t _ESPI      : 2;            /*!< [1..0] _ESPI IP Clock Divider                                             */
            uint32_t            : 22;
      __IOM uint32_t _DMA       : 2;            /*!< [25..24] _DMA IP Clock Divider                                            */
      __IOM uint32_t _OTP       : 2;            /*!< [27..26] _OTP IP Clock Divider                                            */
            uint32_t            : 2;
      __IOM uint32_t _BUS       : 2;            /*!< [31..30] _BUS IP Clock Divider                                            */
    } IPDIV0_b;
  } ;
  
  union {
    __IOM uint32_t IPDIV1;                      /*!< (@ 0x00000024) IP_CLOCK_DIVIDER_1 REGISTER                                */
    
    struct {
      __IOM uint32_t _PWM8      : 2;            /*!< [1..0] _PWM8 IP Clock Divider                                             */
      __IOM uint32_t _PWM7      : 2;            /*!< [3..2] _PWM7 IP Clock Divider                                             */
      __IOM uint32_t _PWM6      : 2;            /*!< [5..4] _PWM6 IP Clock Divider                                             */
      __IOM uint32_t _PWM5      : 2;            /*!< [7..6] _PWM5 IP Clock Divider                                             */
      __IOM uint32_t _PWM4      : 2;            /*!< [9..8] _PWM4 IP Clock Divider                                             */
      __IOM uint32_t _PWM3      : 2;            /*!< [11..10] _PWM3 IP Clock Divider                                           */
      __IOM uint32_t _PWM2      : 2;            /*!< [13..12] _PWM2 IP Clock Divider                                           */
      __IOM uint32_t _PWM1      : 2;            /*!< [15..14] _PWM1 IP Clock Divider                                           */
      __IOM uint32_t _LED4      : 2;            /*!< [17..16] _LED4 IP Clock Divider                                           */
      __IOM uint32_t _LED3      : 2;            /*!< [19..18] _LED3 IP Clock Divider                                           */
      __IOM uint32_t _LED2      : 2;            /*!< [21..20] _LED2 IP Clock Divider                                           */
      __IOM uint32_t _LED1      : 2;            /*!< [23..22] _LED1 IP Clock Divider                                           */
      __IOM uint32_t _EMI7      : 2;            /*!< [25..24] _EMI7 IP Clock Divider                                           */
      __IOM uint32_t _EMI6      : 2;            /*!< [27..26] _EMI6 IP Clock Divider                                           */
      __IOM uint32_t _EMI5      : 2;            /*!< [29..28] _EMI5 IP Clock Divider                                           */
      __IOM uint32_t _EMI4      : 2;            /*!< [31..30] _EMI4 IP Clock Divider                                           */
    } IPDIV1_b;
  } ;
  
  union {
    __IOM uint32_t IPDIV2;                      /*!< (@ 0x00000028) IP_CLOCK_DIVIDER_2 REGISTER                                */
    
    struct {
      __IOM uint32_t _I2C5      : 2;            /*!< [1..0] _I2C5 IP Clock Divider                                             */
      __IOM uint32_t _I2C4      : 2;            /*!< [3..2] _I2C4 IP Clock Divider                                             */
      __IOM uint32_t _I2C3      : 2;            /*!< [5..4] _I2C3 IP Clock Divider                                             */
      __IOM uint32_t _I2C2      : 2;            /*!< [7..6] _I2C2 IP Clock Divider                                             */
      __IOM uint32_t _I2C1      : 2;            /*!< [9..8] _I2C1 IP Clock Divider                                             */
      __IOM uint32_t _UART2     : 2;            /*!< [11..10] _UART2 IP Clock Divider                                          */
      __IOM uint32_t _UART1     : 2;            /*!< [13..12] _UART1 IP Clock Divider                                          */
      __IOM uint32_t _UART0     : 2;            /*!< [15..14] _UART0 IP Clock Divider                                          */
      __IOM uint32_t _ADC       : 3;            /*!< [18..16] _ADC IP Clock Divider                                            */
      __IOM uint32_t _PWM14     : 2;            /*!< [20..19] _PWM14 IP Clock Divider                                          */
      __IOM uint32_t _PWM13     : 2;            /*!< [22..21] _PWM13 IP Clock Divider                                          */
      __IOM uint32_t _PWM12     : 2;            /*!< [24..23] _PWM12 IP Clock Divider                                          */
      __IOM uint32_t _PWM11     : 2;            /*!< [26..25] _PWM11 IP Clock Divider                                          */
      __IOM uint32_t _PWM10     : 2;            /*!< [28..27] _PWM10 IP Clock Divider                                          */
      __IOM uint32_t _PWM9      : 2;            /*!< [30..29] _PWM9 IP Clock Divider                                           */
            uint32_t            : 1;
    } IPDIV2_b;
  } ;
  
  union {
    __IOM uint32_t IPDIV3;                      /*!< (@ 0x0000002C) IP_CLOCK_DIVIDER_3 REGISTER                                */
    
    struct {
            uint32_t            : 20;
      __IOM uint32_t _I2C11     : 2;            /*!< [21..20] _I2C11 IP Clock Divider                                          */
      __IOM uint32_t _I2C10     : 2;            /*!< [23..22] _I2C10 IP Clock Divider                                          */
      __IOM uint32_t _I2C9      : 2;            /*!< [25..24] _I2C9 IP Clock Divider                                           */
      __IOM uint32_t _I2C8      : 2;            /*!< [27..26] _I2C8 IP Clock Divider                                           */
      __IOM uint32_t _I2C7      : 2;            /*!< [29..28] _I2C7 IP Clock Divider                                           */
      __IOM uint32_t _I2C6      : 2;            /*!< [31..30] _I2C6 IP Clock Divider                                           */
    } IPDIV3_b;
  } ;
  
  union {
    __IOM uint32_t IPDIV4;                      /*!< (@ 0x00000030) IP_CLOCK_DIVIDER_4 REGISTER                                */
    
    struct {
            uint32_t            : 7;
      __IOM uint32_t _TMR5      : 4;            /*!< [10..7] _TMR5 IP Clock Divider                                            */
      __IOM uint32_t _TMR4      : 4;            /*!< [14..11] _TMR4 IP Clock Divider                                           */
      __IOM uint32_t _TMR3      : 4;            /*!< [18..15] _TMR3 IP Clock Divider                                           */
      __IOM uint32_t _TMR2      : 4;            /*!< [22..19] _TMR2 IP Clock Divider                                           */
      __IOM uint32_t _TMR1      : 4;            /*!< [26..23] _TMR1 IP Clock Divider                                           */
      __IOM uint32_t _TMR0      : 4;            /*!< [30..27] _TMR0 IP Clock Divider                                           */
            uint32_t            : 1;
    } IPDIV4_b;
  } ;
  
  union {
    union {
      __IOM uint32_t APBDIV0;                   /*!< (@ 0x00000034) APB_CLOCK_DIVIDER_0 REGISTER                               */
      
      struct {
        __IOM uint32_t _I2C2    : 2;            /*!< [1..0] _I2C2 APB Clock Divider                                            */
        __IOM uint32_t _I2C1    : 2;            /*!< [3..2] _I2C1 APB Clock Divider                                            */
        __IOM uint32_t _GPIO    : 2;            /*!< [5..4] _GPIO APB Clock Divider                                            */
        __IOM uint32_t _CEC2    : 2;            /*!< [7..6] _CEC2 APB Clock Divider                                            */
        __IOM uint32_t _CEC1    : 2;            /*!< [9..8] _CEC1 APB Clock Divider                                            */
        __IOM uint32_t _ADC     : 2;            /*!< [11..10] _ADC APB Clock Divider                                           */
              uint32_t          : 20;
      } APBDIV0_b;
    } ;
    
    union {
      __IOM uint32_t IPDIV5;                    /*!< (@ 0x00000034) IP_CLOCK_DIVIDER_5 REGISTER                                */
      
      struct {
              uint32_t          : 18;
        __IOM uint32_t _I3C3    : 2;            /*!< [19..18] _I3C3 IP Clock Divider                                           */
        __IOM uint32_t _I3C2    : 2;            /*!< [21..20] _I3C2 IP Clock Divider                                           */
        __IOM uint32_t _I3C1    : 2;            /*!< [23..22] _I3C1 IP Clock Divider                                           */
        __IOM uint32_t _I3C4    : 2;            /*!< [25..24] _I3C4 IP Clock Divider                                           */
              uint32_t          : 6;
      } IPDIV5_b;
    } ;
  };
  
  union {
    __IOM uint32_t APBDIV1;                     /*!< (@ 0x00000038) APB_CLOCK_DIVIDER_1 REGISTER                               */
    
    struct {
      __IOM uint32_t _LEDSTP    : 2;            /*!< [1..0] _LEDSTP APB Clock Divider                                          */
      __IOM uint32_t _LED4      : 2;            /*!< [3..2] _LED4 APB Clock Divider                                            */
      __IOM uint32_t _LED3      : 2;            /*!< [5..4] _LED3 APB Clock Divider                                            */
      __IOM uint32_t _LED2      : 2;            /*!< [7..6] _LED2 APB Clock Divider                                            */
      __IOM uint32_t _LED1      : 2;            /*!< [9..8] _LED1 APB Clock Divider                                            */
      __IOM uint32_t _KBM       : 2;            /*!< [11..10] _KBM APB Clock Divider                                           */
      __IOM uint32_t _I2CDBG    : 2;            /*!< [13..12] _I2CDBG APB Clock Divider                                        */
      __IOM uint32_t _I2C11     : 2;            /*!< [15..14] _I2C11 APB Clock Divider                                         */
      __IOM uint32_t _I2C10     : 2;            /*!< [17..16] _I2C10 APB Clock Divider                                         */
      __IOM uint32_t _I2C9      : 2;            /*!< [19..18] _I2C9 APB Clock Divider                                          */
      __IOM uint32_t _I2C8      : 2;            /*!< [21..20] _I2C8 APB Clock Divider                                          */
      __IOM uint32_t _I2C7      : 2;            /*!< [23..22] _I2C7 APB Clock Divider                                          */
      __IOM uint32_t _I2C6      : 2;            /*!< [25..24] _I2C6 APB Clock Divider                                          */
      __IOM uint32_t _I2C5      : 2;            /*!< [27..26] _I2C5 APB Clock Divider                                          */
      __IOM uint32_t _I2C4      : 2;            /*!< [29..28] _I2C4 APB Clock Divider                                          */
      __IOM uint32_t _I2C3      : 2;            /*!< [31..30] _I2C3 APB Clock Divider                                          */
    } APBDIV1_b;
  } ;
  
  union {
    __IOM uint32_t APBDIV2;                     /*!< (@ 0x0000003C) APB_CLOCK_DIVIDER_2 REGISTER                               */
    
    struct {
      __IOM uint32_t _PWM13     : 2;            /*!< [1..0] _PWM13 APB Clock Divider                                           */
      __IOM uint32_t _PWM12     : 2;            /*!< [3..2] _PWM12 APB Clock Divider                                           */
      __IOM uint32_t _PWM11     : 2;            /*!< [5..4] _PWM11 APB Clock Divider                                           */
      __IOM uint32_t _PWM10     : 2;            /*!< [7..6] _PWM10 APB Clock Divider                                           */
      __IOM uint32_t _PWM9      : 2;            /*!< [9..8] _PWM9 APB Clock Divider                                            */
      __IOM uint32_t _PWM8      : 2;            /*!< [11..10] _PWM8 APB Clock Divider                                          */
      __IOM uint32_t _PWM7      : 2;            /*!< [13..12] _PWM7 APB Clock Divider                                          */
      __IOM uint32_t _PWM6      : 2;            /*!< [15..14] _PWM6 APB Clock Divider                                          */
      __IOM uint32_t _PWM5      : 2;            /*!< [17..16] _PWM5 APB Clock Divider                                          */
      __IOM uint32_t _PWM4      : 2;            /*!< [19..18] _PWM4 APB Clock Divider                                          */
      __IOM uint32_t _PWM3      : 2;            /*!< [21..20] _PWM3 APB Clock Divider                                          */
      __IOM uint32_t _PWM2      : 2;            /*!< [23..22] _PWM2 APB Clock Divider                                          */
      __IOM uint32_t _PWM1      : 2;            /*!< [25..24] _PWM1 APB Clock Divider                                          */
      __IOM uint32_t _PS2_3     : 2;            /*!< [27..26] _PS2_3 APB Clock Divider                                         */
      __IOM uint32_t _PS2_2     : 2;            /*!< [29..28] _PS2_2 APB Clock Divider                                         */
      __IOM uint32_t _PS2_1     : 2;            /*!< [31..30] _PS2_1 APB Clock Divider                                         */
    } APBDIV2_b;
  } ;
  
  union {
    __IOM uint32_t APBDIV3;                     /*!< (@ 0x00000040) APB_CLOCK_DIVIDER_3 REGISTER                               */
    
    struct {
      __IOM uint32_t _TACH5     : 2;            /*!< [1..0] _TACH5 APB Clock Divider                                           */
      __IOM uint32_t _TACH4     : 2;            /*!< [3..2] _TACH4 APB Clock Divider                                           */
      __IOM uint32_t _TACH3     : 2;            /*!< [5..4] _TACH3 APB Clock Divider                                           */
      __IOM uint32_t _TACH2     : 2;            /*!< [7..6] _TACH2 APB Clock Divider                                           */
      __IOM uint32_t _TACH1     : 2;            /*!< [9..8] _TACH1 APB Clock Divider                                           */
      __IOM uint32_t _SPIM      : 2;            /*!< [11..10] _SPIM APB Clock Divider                                          */
      __IOM uint32_t _SPIS      : 2;            /*!< [13..12] _SPIS APB Clock Divider                                          */
      __IOM uint32_t _SLWTMR3   : 2;            /*!< [15..14] _SLWTMR3 APB Clock Divider                                       */
      __IOM uint32_t _SLWTMR2   : 2;            /*!< [17..16] _SLWTMR2 APB Clock Divider                                       */
      __IOM uint32_t _SLWTMR1   : 2;            /*!< [19..18] _SLWTMR1 APB Clock Divider                                       */
      __IOM uint32_t _SLWTMR0   : 2;            /*!< [21..20] _SLWTMR0 APB Clock Divider                                       */
      __IOM uint32_t _RTMR      : 2;            /*!< [23..22] _RTMR APB Clock Divider                                          */
      __IOM uint32_t _RTC       : 2;            /*!< [25..24] _RTC APB Clock Divider                                           */
      __IOM uint32_t _PWRBTN1   : 2;            /*!< [27..26] _PWRBTN1 APB Clock Divider                                       */
      __IOM uint32_t _PWRBTN0   : 2;            /*!< [29..28] _PWRBTN0 APB Clock Divider                                       */
      __IOM uint32_t _PWM14     : 2;            /*!< [31..30] _PWM14 APB Clock Divider                                         */
    } APBDIV3_b;
  } ;
  
  union {
    __IOM uint32_t APBDIV4;                     /*!< (@ 0x00000044) APB_CLOCK_DIVIDER_4 REGISTER                               */
    
    struct {
      __IOM uint32_t _UART0     : 2;            /*!< [1..0] _UART0 APB Clock Divider                                           */
      __IOM uint32_t _TMR5      : 2;            /*!< [3..2] _TMR5 APB Clock Divider                                            */
      __IOM uint32_t _TMR4      : 2;            /*!< [5..4] _TMR4 APB Clock Divider                                            */
      __IOM uint32_t _TMR3      : 2;            /*!< [7..6] _TMR3 APB Clock Divider                                            */
      __IOM uint32_t _TMR2      : 2;            /*!< [9..8] _TMR2 APB Clock Divider                                            */
      __IOM uint32_t _TMR1      : 2;            /*!< [11..10] _TMR1 APB Clock Divider                                          */
      __IOM uint32_t _TMR0      : 2;            /*!< [13..12] _TMR0 APB Clock Divider                                          */
      __IOM uint32_t _TACH14    : 2;            /*!< [15..14] _TACH14 APB Clock Divider                                        */
      __IOM uint32_t _TACH13    : 2;            /*!< [17..16] _TACH13 APB Clock Divider                                        */
      __IOM uint32_t _TACH12    : 2;            /*!< [19..18] _TACH12 APB Clock Divider                                        */
      __IOM uint32_t _TACH11    : 2;            /*!< [21..20] _TACH11 APB Clock Divider                                        */
      __IOM uint32_t _TACH10    : 2;            /*!< [23..22] _TACH10 APB Clock Divider                                        */
      __IOM uint32_t _TACH9     : 2;            /*!< [25..24] _TACH9 APB Clock Divider                                         */
      __IOM uint32_t _TACH8     : 2;            /*!< [27..26] _TACH8 APB Clock Divider                                         */
      __IOM uint32_t _TACH7     : 2;            /*!< [29..28] _TACH7 APB Clock Divider                                         */
      __IOM uint32_t _TACH6     : 2;            /*!< [31..30] _TACH6 APB Clock Divider                                         */
    } APBDIV4_b;
  } ;
  
  union {
    union {
      __IOM uint32_t APBDIV5;                   /*!< (@ 0x00000048) APB_CLOCK_DIVIDER_5 REGISTER                               */
      
      struct {
              uint32_t          : 11;
        __IOM uint32_t _PECI    : 2;            /*!< [12..11] _PECI APB Clock Divider                                          */
        __IOM uint32_t _LPT     : 2;            /*!< [14..13] _LPT APB Clock Divider                                           */
        __IOM uint32_t _WDT     : 2;            /*!< [16..15] _WDT APB Clock Divider                                           */
        __IOM uint32_t _UART2   : 2;            /*!< [18..17] _UART2 APB Clock Divider                                         */
        __IOM uint32_t _UART1   : 2;            /*!< [20..19] _UART1 APB Clock Divider                                         */
              uint32_t          : 11;
      } APBDIV5_b;
    } ;
    
    union {
      __IOM uint32_t IPDIV6;                    /*!< (@ 0x00000048) IP_CLOCK_DIVIDER_6 REGISTER                                */
      
      struct {
              uint32_t          : 6;
        __IOM uint32_t _USB     : 1;            /*!< [6..6] _USB IP Clock Divider                                              */
              uint32_t          : 25;
      } IPDIV6_b;
    } ;
  };
  
  union {
    __IOM uint32_t CLKGATING0;                  /*!< (@ 0x0000004C) IP_CLOCK_GATING_0 EREGISTER                                */
    
    struct {
      __IOM uint32_t _PWM8      : 1;            /*!< [0..0] _PWM8 IP Clock Gating                                              */
      __IOM uint32_t _PWM7      : 1;            /*!< [1..1] _PWM7 IP Clock Gating                                              */
      __IOM uint32_t _PWM6      : 1;            /*!< [2..2] _PWM6 IP Clock Gating                                              */
      __IOM uint32_t _PWM5      : 1;            /*!< [3..3] _PWM5 IP Clock Gating                                              */
      __IOM uint32_t _PWM4      : 1;            /*!< [4..4] _PWM4 IP Clock Gating                                              */
      __IOM uint32_t _PWM3      : 1;            /*!< [5..5] _PWM3 IP Clock Gating                                              */
      __IOM uint32_t _PWM2      : 1;            /*!< [6..6] _PWM2 IP Clock Gating                                              */
      __IOM uint32_t _PWM1      : 1;            /*!< [7..7] _PWM1 IP Clock Gating                                              */
      __IOM uint32_t _LED4      : 1;            /*!< [8..8] _LED4 IP Clock Gating                                              */
      __IOM uint32_t _LED3      : 1;            /*!< [9..9] _LED3 IP Clock Gating                                              */
      __IOM uint32_t _LED2      : 1;            /*!< [10..10] _LED2 IP Clock Gating                                            */
      __IOM uint32_t _LED1      : 1;            /*!< [11..11] _LED1 IP Clock Gating                                            */
      __IOM uint32_t _EMI7      : 1;            /*!< [12..12] _EMI7 IP Clock Gating                                            */
      __IOM uint32_t _EMI6      : 1;            /*!< [13..13] _EMI6 IP Clock Gating                                            */
      __IOM uint32_t _EMI5      : 1;            /*!< [14..14] _EMI5 IP Clock Gating                                            */
      __IOM uint32_t _EMI4      : 1;            /*!< [15..15] _EMI4 IP Clock Gating                                            */
      __IOM uint32_t _EMI3      : 1;            /*!< [16..16] _EMI3 IP Clock Gating                                            */
      __IOM uint32_t _EMI2      : 1;            /*!< [17..17] _EMI2 IP Clock Gating                                            */
      __IOM uint32_t _EMI1      : 1;            /*!< [18..18] _EMI1 IP Clock Gating                                            */
      __IOM uint32_t _EMI0      : 1;            /*!< [19..19] _EMI0 IP Clock Gating                                            */
      __IOM uint32_t _PORT80    : 1;            /*!< [20..20] _PORT80 IP Clock Gating                                          */
      __IOM uint32_t _PMPORT3   : 1;            /*!< [21..21] _PMPORT3 IP Clock Gating                                         */
      __IOM uint32_t _PMPORT2   : 1;            /*!< [22..22] _PMPORT2 IP Clock Gating                                         */
      __IOM uint32_t _PMPORT1   : 1;            /*!< [23..23] _PMPORT1 IP Clock Gating                                         */
      __IOM uint32_t _PMPORT0   : 1;            /*!< [24..24] _PMPORT0 IP Clock Gating                                         */
      __IOM uint32_t _ACPI      : 1;            /*!< [25..25] _ACPI IP Clock Gating                                            */
      __IOM uint32_t _KBC       : 1;            /*!< [26..26] _KBC IP Clock Gating                                             */
      __IOM uint32_t _ESPI      : 1;            /*!< [27..27] _ESPI IP Clock Gating                                            */
      __IOM uint32_t _DMA       : 1;            /*!< [28..28] _DMA IP Clock Gating                                             */
      __IOM uint32_t PUF_OTP    : 1;            /*!< [29..29] PUF_OTP IP Clock Gating                                          */
            uint32_t            : 1;
      __IOM uint32_t _BUS       : 1;            /*!< [31..31] _BUS IP Clock Gating                                             */
    } CLKGATING0_b;
  } ;
  
  union {
    __IOM uint32_t CLKGATING1;                  /*!< (@ 0x00000050) IP_CLOCK_GATING_1 EREGISTER                                */
    
    struct {
      __IOM uint32_t _TACH5     : 1;            /*!< [0..0] _TACH5 IP Clock Gating                                             */
      __IOM uint32_t _TACH4     : 1;            /*!< [1..1] _TACH4 IP Clock Gating                                             */
      __IOM uint32_t _TACH3     : 1;            /*!< [2..2] _TACH3 IP Clock Gating                                             */
      __IOM uint32_t _TACH2     : 1;            /*!< [3..3] _TACH2 IP Clock Gating                                             */
      __IOM uint32_t _TACH1     : 1;            /*!< [4..4] _TACH1 IP Clock Gating                                             */
      __IOM uint32_t _PS2_H3    : 1;            /*!< [5..5] _PS2_H3 IP Clock Gating                                            */
      __IOM uint32_t _PS2_H2    : 1;            /*!< [6..6] _PS2_H2 IP Clock Gating                                            */
      __IOM uint32_t _PS2_H1    : 1;            /*!< [7..7] _PS2_H1 IP Clock Gating                                            */
      __IOM uint32_t SWD        : 1;            /*!< [8..8] SWD IP Clock Gating                                                */
      __IOM uint32_t REF25M     : 1;            /*!< [9..9] REF25M IP Clock Gating                                             */
      __IOM uint32_t RC24M      : 1;            /*!< [10..10] RC24M IP Clock Gating                                            */
      __IOM uint32_t _I2C11     : 1;            /*!< [11..11] _I2C11 IP Clock Gating                                           */
      __IOM uint32_t _I2C10     : 1;            /*!< [12..12] _I2C10 IP Clock Gating                                           */
      __IOM uint32_t _I2C9      : 1;            /*!< [13..13] _I2C9 IP Clock Gating                                            */
      __IOM uint32_t _I2C8      : 1;            /*!< [14..14] _I2C8 IP Clock Gating                                            */
      __IOM uint32_t _I2C7      : 1;            /*!< [15..15] _I2C7 IP Clock Gating                                            */
      __IOM uint32_t _I2C6      : 1;            /*!< [16..16] _I2C6 IP Clock Gating                                            */
      __IOM uint32_t _I2C5      : 1;            /*!< [17..17] _I2C5 IP Clock Gating                                            */
      __IOM uint32_t _I2C4      : 1;            /*!< [18..18] _I2C4 IP Clock Gating                                            */
      __IOM uint32_t _I2C3      : 1;            /*!< [19..19] _I2C3 IP Clock Gating                                            */
      __IOM uint32_t _I2C2      : 1;            /*!< [20..20] _I2C2 IP Clock Gating                                            */
      __IOM uint32_t _I2C1      : 1;            /*!< [21..21] _I2C1 IP Clock Gating                                            */
      __IOM uint32_t _UART2     : 1;            /*!< [22..22] _UART2 IP Clock Gating                                           */
      __IOM uint32_t _UART1     : 1;            /*!< [23..23] _UART1 IP Clock Gating                                           */
      __IOM uint32_t _UART0     : 1;            /*!< [24..24] _UART0 IP Clock Gating                                           */
      __IOM uint32_t _ADC       : 1;            /*!< [25..25] _ADC IP Clock Gating                                             */
      __IOM uint32_t _PWM14     : 1;            /*!< [26..26] _PWM14 IP Clock Gating                                           */
      __IOM uint32_t _PWM13     : 1;            /*!< [27..27] _PWM13 IP Clock Gating                                           */
      __IOM uint32_t _PWM12     : 1;            /*!< [28..28] _PWM12 IP Clock Gating                                           */
      __IOM uint32_t _PWM11     : 1;            /*!< [29..29] _PWM11 IP Clock Gating                                           */
      __IOM uint32_t _PWM10     : 1;            /*!< [30..30] _PWM10 IP Clock Gating                                           */
      __IOM uint32_t _PWM9      : 1;            /*!< [31..31] _PWM9 IP Clock Gating                                            */
    } CLKGATING1_b;
  } ;
  
  union {
    __IOM uint32_t CLKGATING2;                  /*!< (@ 0x00000054) IP_CLOCK_GATING_2 EREGISTER                                */
    
    struct {
      __IOM uint32_t _I3C2      : 1;            /*!< [0..0] _I3C2 IP Clock Gating                                              */
      __IOM uint32_t _I3C1      : 1;            /*!< [1..1] _I3C1 IP Clock Gating                                              */
      __IOM uint32_t _I3C4      : 1;            /*!< [2..2] _I3C4 IP Clock Gating                                              */
      __IOM uint32_t _LPT       : 1;            /*!< [3..3] _LPT IP Clock Gating                                               */
      __IOM uint32_t _VINLED    : 1;            /*!< [4..4] _VINLED IP Clock Gating                                            */
      __IOM uint32_t _I2CDBG    : 1;            /*!< [5..5] _I2CDBG IP Clock Gating                                            */
      __IOM uint32_t _MICROCNT  : 1;            /*!< [6..6] _MICROCNT IP Clock Gating                                          */
      __IOM uint32_t _CEC2      : 1;            /*!< [7..7] _CEC2 IP Clock Gating                                              */
      __IOM uint32_t _CEC1      : 1;            /*!< [8..8] _CEC1 IP Clock Gating                                              */
      __IOM uint32_t _LEDSTP    : 1;            /*!< [9..9] _LEDSTP IP Clock Gating                                            */
      __IOM uint32_t _SLWTMR3   : 1;            /*!< [10..10] _SLWTMR3 IP Clock Gating                                         */
      __IOM uint32_t _SLWTMR2   : 1;            /*!< [11..11] _SLWTMR2 IP Clock Gating                                         */
      __IOM uint32_t _SLWTMR1   : 1;            /*!< [12..12] _SLWTMR1 IP Clock Gating                                         */
      __IOM uint32_t _SLWTMR0   : 1;            /*!< [13..13] _SLWTMR0 IP Clock Gating                                         */
      __IOM uint32_t _KBMSCAN   : 1;            /*!< [14..14] _KBMSCAN IP Clock Gating                                         */
      __IOM uint32_t _KBM       : 1;            /*!< [15..15] _KBM IP Clock Gating                                             */
      __IOM uint32_t _TMR5      : 1;            /*!< [16..16] _TMR5 IP Clock Gating                                            */
      __IOM uint32_t _TMR4      : 1;            /*!< [17..17] _TMR4 IP Clock Gating                                            */
      __IOM uint32_t _TMR3      : 1;            /*!< [18..18] _TMR3 IP Clock Gating                                            */
      __IOM uint32_t _TMR2      : 1;            /*!< [19..19] _TMR2 IP Clock Gating                                            */
      __IOM uint32_t _TMR1      : 1;            /*!< [20..20] _TMR1 IP Clock Gating                                            */
      __IOM uint32_t _TMR0      : 1;            /*!< [21..21] _TMR0 IP Clock Gating                                            */
      __IOM uint32_t _GPIO      : 1;            /*!< [22..22] _GPIO IP Clock Gating                                            */
      __IOM uint32_t _TACH14    : 1;            /*!< [23..23] _TACH14 IP Clock Gating                                          */
      __IOM uint32_t _TACH13    : 1;            /*!< [24..24] _TACH13 IP Clock Gating                                          */
      __IOM uint32_t _TACH12    : 1;            /*!< [25..25] _TACH12 IP Clock Gating                                          */
      __IOM uint32_t _TACH11    : 1;            /*!< [26..26] _TACH11 IP Clock Gating                                          */
      __IOM uint32_t _TACH10    : 1;            /*!< [27..27] _TACH10 IP Clock Gating                                          */
      __IOM uint32_t _TACH9     : 1;            /*!< [28..28] _TACH9 IP Clock Gating                                           */
      __IOM uint32_t _TACH8     : 1;            /*!< [29..29] _TACH8 IP Clock Gating                                           */
      __IOM uint32_t _TACH7     : 1;            /*!< [30..30] _TACH7 IP Clock Gating                                           */
      __IOM uint32_t _TACH6     : 1;            /*!< [31..31] _TACH6 IP Clock Gating                                           */
    } CLKGATING2_b;
  } ;
  
  union {
    union {
      __IOM uint32_t APBGATING0;                /*!< (@ 0x00000058) APB_CLOCK_GATING_0 EREGISTER                               */
      
      struct {
        __IOM uint32_t _PS2_3   : 1;            /*!< [0..0] _PS2_3 APB Clock Gating                                            */
        __IOM uint32_t _PS2_2   : 1;            /*!< [1..1] _PS2_2 APB Clock Gating                                            */
        __IOM uint32_t _PS2_1   : 1;            /*!< [2..2] _PS2_1 APB Clock Gating                                            */
        __IOM uint32_t _LEDSTP  : 1;            /*!< [3..3] _LEDSTP APB Clock Gating                                           */
        __IOM uint32_t _LED4    : 1;            /*!< [4..4] _LED4 APB Clock Gating                                             */
        __IOM uint32_t _LED3    : 1;            /*!< [5..5] _LED3 APB Clock Gating                                             */
        __IOM uint32_t _LED2    : 1;            /*!< [6..6] _LED2 APB Clock Gating                                             */
        __IOM uint32_t _LED1    : 1;            /*!< [7..7] _LED1 APB Clock Gating                                             */
        __IOM uint32_t _KBM     : 1;            /*!< [8..8] _KBM APB Clock Gating                                              */
        __IOM uint32_t _I2CDBG  : 1;            /*!< [9..9] _I2CDBG APB Clock Gating                                           */
        __IOM uint32_t _I2C11   : 1;            /*!< [10..10] _I2C11 APB Clock Gating                                          */
        __IOM uint32_t _I2C10   : 1;            /*!< [11..11] _I2C10 APB Clock Gating                                          */
        __IOM uint32_t _I2C9    : 1;            /*!< [12..12] _I2C9 APB Clock Gating                                           */
        __IOM uint32_t _I2C8    : 1;            /*!< [13..13] _I2C8 APB Clock Gating                                           */
        __IOM uint32_t _I2C7    : 1;            /*!< [14..14] _I2C7 APB Clock Gating                                           */
        __IOM uint32_t _I2C6    : 1;            /*!< [15..15] _I2C6 APB Clock Gating                                           */
        __IOM uint32_t _I2C5    : 1;            /*!< [16..16] _I2C5 APB Clock Gating                                           */
        __IOM uint32_t _I2C4    : 1;            /*!< [17..17] _I2C4 APB Clock Gating                                           */
        __IOM uint32_t _I2C3    : 1;            /*!< [18..18] _I2C3 APB Clock Gating                                           */
        __IOM uint32_t _I2C2    : 1;            /*!< [19..19] _I2C2 APB Clock Gating                                           */
        __IOM uint32_t _I2C1    : 1;            /*!< [20..20] _I2C1 APB Clock Gating                                           */
        __IOM uint32_t _GPIO    : 1;            /*!< [21..21] _GPIO APB Clock Gating                                           */
        __IOM uint32_t _CEC2    : 1;            /*!< [22..22] _CEC2 APB Clock Gating                                           */
        __IOM uint32_t _CEC1    : 1;            /*!< [23..23] _CEC1 APB Clock Gating                                           */
        __IOM uint32_t _ADC     : 1;            /*!< [24..24] _ADC APB Clock Gating                                            */
              uint32_t          : 7;
      } APBGATING0_b;
    } ;
    
    union {
      __IOM uint32_t CLKGATING3;                /*!< (@ 0x00000058) IP_CLOCK_GATING_3 EREGISTER                                */
      
      struct {
              uint32_t          : 25;
        __IOM uint32_t _RTMR    : 1;            /*!< [25..25] _RTMR IP Clock Gating                                            */
        __IOM uint32_t _PS2_L3  : 1;            /*!< [26..26] _PS2_L3 IP Clock Gating                                          */
        __IOM uint32_t _PS2_L2  : 1;            /*!< [27..27] _PS2_L2 IP Clock Gating                                          */
        __IOM uint32_t _PS2_L1  : 1;            /*!< [28..28] _PS2_L1 IP Clock Gating                                          */
        __IOM uint32_t REF_32K  : 1;            /*!< [29..29] REF_32K IP Clock Gating                                          */
        __IOM uint32_t _PECI    : 1;            /*!< [30..30] _PECI IP Clock Gating                                            */
        __IOM uint32_t _I3C3    : 1;            /*!< [31..31] _I3C3 IP Clock Gating                                            */
      } CLKGATING3_b;
    } ;
  };
  
  union {
    __IOM uint32_t APBGATING1;                  /*!< (@ 0x0000005C) APB_CLOCK_GATING_1 EREGISTER                               */
    
    struct {
      __IOM uint32_t _TACH8     : 1;            /*!< [0..0] _TACH8 APB Clock Gating                                            */
      __IOM uint32_t _TACH7     : 1;            /*!< [1..1] _TACH7 APB Clock Gating                                            */
      __IOM uint32_t _TACH6     : 1;            /*!< [2..2] _TACH6 APB Clock Gating                                            */
      __IOM uint32_t _TACH5     : 1;            /*!< [3..3] _TACH5 APB Clock Gating                                            */
      __IOM uint32_t _TACH4     : 1;            /*!< [4..4] _TACH4 APB Clock Gating                                            */
      __IOM uint32_t _TACH3     : 1;            /*!< [5..5] _TACH3 APB Clock Gating                                            */
      __IOM uint32_t _TACH2     : 1;            /*!< [6..6] _TACH2 APB Clock Gating                                            */
      __IOM uint32_t _TACH1     : 1;            /*!< [7..7] _TACH1 APB Clock Gating                                            */
      __IOM uint32_t _SPIM      : 1;            /*!< [8..8] _SPIM APB Clock Gating                                             */
      __IOM uint32_t _SPIS      : 1;            /*!< [9..9] _SPIS APB Clock Gating                                             */
      __IOM uint32_t _SLWTMR3   : 1;            /*!< [10..10] _SLWTMR3 APB Clock Gating                                        */
      __IOM uint32_t _SLWTMR2   : 1;            /*!< [11..11] _SLWTMR2 APB Clock Gating                                        */
      __IOM uint32_t _SLWTMR1   : 1;            /*!< [12..12] _SLWTMR1 APB Clock Gating                                        */
      __IOM uint32_t _SLWTMR0   : 1;            /*!< [13..13] _SLWTMR0 APB Clock Gating                                        */
      __IOM uint32_t _RTMR      : 1;            /*!< [14..14] _RTMR APB Clock Gating                                           */
      __IOM uint32_t _RTC       : 1;            /*!< [15..15] _RTC APB Clock Gating                                            */
      __IOM uint32_t _PWRBTN1   : 1;            /*!< [16..16] _PWRBTN1 APB Clock Gating                                        */
      __IOM uint32_t _PWRBTN0   : 1;            /*!< [17..17] _PWRBTN0 APB Clock Gating                                        */
      __IOM uint32_t _PWM14     : 1;            /*!< [18..18] _PWM14 APB Clock Gating                                          */
      __IOM uint32_t _PWM13     : 1;            /*!< [19..19] _PWM13 APB Clock Gating                                          */
      __IOM uint32_t _PWM12     : 1;            /*!< [20..20] _PWM12 APB Clock Gating                                          */
      __IOM uint32_t _PWM11     : 1;            /*!< [21..21] _PWM11 APB Clock Gating                                          */
      __IOM uint32_t _PWM10     : 1;            /*!< [22..22] _PWM10 APB Clock Gating                                          */
      __IOM uint32_t _PWM9      : 1;            /*!< [23..23] _PWM9 APB Clock Gating                                           */
      __IOM uint32_t _PWM8      : 1;            /*!< [24..24] _PWM8 APB Clock Gating                                           */
      __IOM uint32_t _PWM7      : 1;            /*!< [25..25] _PWM7 APB Clock Gating                                           */
      __IOM uint32_t _PWM6      : 1;            /*!< [26..26] _PWM6 APB Clock Gating                                           */
      __IOM uint32_t _PWM5      : 1;            /*!< [27..27] _PWM5 APB Clock Gating                                           */
      __IOM uint32_t _PWM4      : 1;            /*!< [28..28] _PWM4 APB Clock Gating                                           */
      __IOM uint32_t _PWM3      : 1;            /*!< [29..29] _PWM3 APB Clock Gating                                           */
      __IOM uint32_t _PWM2      : 1;            /*!< [30..30] _PWM2 APB Clock Gating                                           */
      __IOM uint32_t _PWM1      : 1;            /*!< [31..31] _PWM1 APB Clock Gating                                           */
    } APBGATING1_b;
  } ;
  
  union {
    union {
      __IOM uint32_t CLKGATING4;                /*!< (@ 0x00000060) IP_CLOCK_GATING_4 EREGISTER                                */
      
      struct {
              uint32_t          : 5;
        __IOM uint32_t _USBDHY  : 1;            /*!< [5..5] _USBDHY IP Clock Gating                                            */
        __IOM uint32_t _USB     : 1;            /*!< [6..6] _USB IP Clock Gating                                               */
        __IOM uint32_t _TRC     : 1;            /*!< [7..7] _TRC IP Clock Gating                                               */
        __IOM uint32_t _LALU    : 1;            /*!< [8..8] _LALU IP Clock Gating                                              */
              uint32_t          : 23;
      } CLKGATING4_b;
    } ;
    
    union {
      __IOM uint32_t APBGATING2;                /*!< (@ 0x00000060) APB_CLOCK_GATING_2 EREGISTER                               */
      
      struct {
              uint32_t          : 9;
        __IOM uint32_t _TRC     : 1;            /*!< [9..9] _TRC APB Clock Gating                                              */
        __IOM uint32_t _PECI    : 1;            /*!< [10..10] _PECI APB Clock Gating                                           */
        __IOM uint32_t _LPT     : 1;            /*!< [11..11] _LPT APB Clock Gating                                            */
        __IOM uint32_t _WDT     : 1;            /*!< [12..12] _WDT APB Clock Gating                                            */
        __IOM uint32_t _UART2   : 1;            /*!< [13..13] _UART2 APB Clock Gating                                          */
        __IOM uint32_t _UART1   : 1;            /*!< [14..14] _UART1 APB Clock Gating                                          */
        __IOM uint32_t _UART0   : 1;            /*!< [15..15] _UART0 APB Clock Gating                                          */
        __IOM uint32_t _TMR5    : 1;            /*!< [16..16] _TMR5 APB Clock Gating                                           */
        __IOM uint32_t _TMR4    : 1;            /*!< [17..17] _TMR4 APB Clock Gating                                           */
        __IOM uint32_t _TMR3    : 1;            /*!< [18..18] _TMR3 APB Clock Gating                                           */
        __IOM uint32_t _TMR2    : 1;            /*!< [19..19] _TMR2 APB Clock Gating                                           */
        __IOM uint32_t _TMR1    : 1;            /*!< [20..20] _TMR1 APB Clock Gating                                           */
        __IOM uint32_t _TMR0    : 1;            /*!< [21..21] _TMR0 APB Clock Gating                                           */
        __IOM uint32_t _TACH14  : 1;            /*!< [22..22] _TACH14 APB Clock Gating                                         */
        __IOM uint32_t _TACH13  : 1;            /*!< [23..23] _TACH13 APB Clock Gating                                         */
        __IOM uint32_t _TACH12  : 1;            /*!< [24..24] _TACH12 APB Clock Gating                                         */
        __IOM uint32_t _TACH11  : 1;            /*!< [25..25] _TACH11 APB Clock Gating                                         */
        __IOM uint32_t _TACH10  : 1;            /*!< [26..26] _TACH10 APB Clock Gating                                         */
        __IOM uint32_t _TACH9   : 1;            /*!< [27..27] _TACH9 APB Clock Gating                                          */
              uint32_t          : 4;
      } APBGATING2_b;
    } ;
  };
  
  union {
    __IOM uint32_t IPRST0;                      /*!< (@ 0x00000064) IP_RESET_0 EREGISTER                                       */
    
    struct {
      __IOM uint32_t _PWM8      : 1;            /*!< [0..0] _PWM8 IP Reset                                                     */
      __IOM uint32_t _PWM7      : 1;            /*!< [1..1] _PWM7 IP Reset                                                     */
      __IOM uint32_t _PWM6      : 1;            /*!< [2..2] _PWM6 IP Reset                                                     */
      __IOM uint32_t _PWM5      : 1;            /*!< [3..3] _PWM5 IP Reset                                                     */
      __IOM uint32_t _PWM4      : 1;            /*!< [4..4] _PWM4 IP Reset                                                     */
      __IOM uint32_t _PWM3      : 1;            /*!< [5..5] _PWM3 IP Reset                                                     */
      __IOM uint32_t _PWM2      : 1;            /*!< [6..6] _PWM2 IP Reset                                                     */
      __IOM uint32_t _PWM1      : 1;            /*!< [7..7] _PWM1 IP Reset                                                     */
      __IOM uint32_t _LED4      : 1;            /*!< [8..8] _LED4 IP Reset                                                     */
      __IOM uint32_t _LED3      : 1;            /*!< [9..9] _LED3 IP Reset                                                     */
      __IOM uint32_t _LED2      : 1;            /*!< [10..10] _LED2 IP Reset                                                   */
      __IOM uint32_t _LED1      : 1;            /*!< [11..11] _LED1 IP Reset                                                   */
      __IOM uint32_t _EMI7      : 1;            /*!< [12..12] _EMI7 IP Reset                                                   */
      __IOM uint32_t _EMI6      : 1;            /*!< [13..13] _EMI6 IP Reset                                                   */
      __IOM uint32_t _EMI5      : 1;            /*!< [14..14] _EMI5 IP Reset                                                   */
      __IOM uint32_t _EMI4      : 1;            /*!< [15..15] _EMI4 IP Reset                                                   */
      __IOM uint32_t _EMI3      : 1;            /*!< [16..16] _EMI3 IP Reset                                                   */
      __IOM uint32_t _EMI2      : 1;            /*!< [17..17] _EMI2 IP Reset                                                   */
      __IOM uint32_t _EMI1      : 1;            /*!< [18..18] _EMI1 IP Reset                                                   */
      __IOM uint32_t _EMI0      : 1;            /*!< [19..19] _EMI0 IP Reset                                                   */
      __IOM uint32_t _PORT80    : 1;            /*!< [20..20] _PORT80 IP Reset                                                 */
      __IOM uint32_t _PMPORT3   : 1;            /*!< [21..21] _PMPORT3 IP Reset                                                */
      __IOM uint32_t _PMPORT2   : 1;            /*!< [22..22] _PMPORT2 IP Reset                                                */
      __IOM uint32_t _PMPORT1   : 1;            /*!< [23..23] _PMPORT1 IP Reset                                                */
      __IOM uint32_t _PMPORT0   : 1;            /*!< [24..24] _PMPORT0 IP Reset                                                */
      __IOM uint32_t _ACPI      : 1;            /*!< [25..25] _ACPI IP Reset                                                   */
      __IOM uint32_t _KBC       : 1;            /*!< [26..26] _KBC IP Reset                                                    */
      __IOM uint32_t _ESPI      : 1;            /*!< [27..27] _ESPI IP Reset                                                   */
      __IOM uint32_t _DMA       : 1;            /*!< [28..28] _DMA IP Reset                                                    */
      __IOM uint32_t PUF_OTP    : 1;            /*!< [29..29] PUF_OTP IP Reset                                                 */
            uint32_t            : 1;
      __IOM uint32_t _BUS       : 1;            /*!< [31..31] _BUS IP Reset                                                    */
    } IPRST0_b;
  } ;
  
  union {
    __IOM uint32_t IPRST1;                      /*!< (@ 0x00000068) IP_RESET_1 EREGISTER                                       */
    
    struct {
      __IOM uint32_t _TACH5     : 1;            /*!< [0..0] _TACH5 IP Reset                                                    */
      __IOM uint32_t _TACH4     : 1;            /*!< [1..1] _TACH4 IP Reset                                                    */
      __IOM uint32_t _TACH3     : 1;            /*!< [2..2] _TACH3 IP Reset                                                    */
      __IOM uint32_t _TACH2     : 1;            /*!< [3..3] _TACH2 IP Reset                                                    */
      __IOM uint32_t _TACH1     : 1;            /*!< [4..4] _TACH1 IP Reset                                                    */
      __IOM uint32_t _PS2_H3    : 1;            /*!< [5..5] _PS2_H3 IP Reset                                                   */
      __IOM uint32_t _PS2_H2    : 1;            /*!< [6..6] _PS2_H2 IP Reset                                                   */
      __IOM uint32_t _PS2_H1    : 1;            /*!< [7..7] _PS2_H1 IP Reset                                                   */
      __IOM uint32_t RST        : 1;            /*!< [8..8] RST IP Reset                                                       */
      __IOM uint32_t RST25M     : 1;            /*!< [9..9] RST25M IP Reset                                                    */
      __IOM uint32_t RST24M     : 1;            /*!< [10..10] RST24M IP Reset                                                  */
      __IOM uint32_t _I2C11     : 1;            /*!< [11..11] _I2C11 IP Reset                                                  */
      __IOM uint32_t _I2C10     : 1;            /*!< [12..12] _I2C10 IP Reset                                                  */
      __IOM uint32_t _I2C9      : 1;            /*!< [13..13] _I2C9 IP Reset                                                   */
      __IOM uint32_t _I2C8      : 1;            /*!< [14..14] _I2C8 IP Reset                                                   */
      __IOM uint32_t _I2C7      : 1;            /*!< [15..15] _I2C7 IP Reset                                                   */
      __IOM uint32_t _I2C6      : 1;            /*!< [16..16] _I2C6 IP Reset                                                   */
      __IOM uint32_t _I2C5      : 1;            /*!< [17..17] _I2C5 IP Reset                                                   */
      __IOM uint32_t _I2C4      : 1;            /*!< [18..18] _I2C4 IP Reset                                                   */
      __IOM uint32_t _I2C3      : 1;            /*!< [19..19] _I2C3 IP Reset                                                   */
      __IOM uint32_t _I2C2      : 1;            /*!< [20..20] _I2C2 IP Reset                                                   */
      __IOM uint32_t _I2C1      : 1;            /*!< [21..21] _I2C1 IP Reset                                                   */
      __IOM uint32_t _UART2     : 1;            /*!< [22..22] _UART2 IP Reset                                                  */
      __IOM uint32_t _UART1     : 1;            /*!< [23..23] _UART1 IP Reset                                                  */
      __IOM uint32_t _UART0     : 1;            /*!< [24..24] _UART0 IP Reset                                                  */
      __IOM uint32_t _ADC       : 1;            /*!< [25..25] _ADC IP Reset                                                    */
      __IOM uint32_t _PWM14     : 1;            /*!< [26..26] _PWM14 IP Reset                                                  */
      __IOM uint32_t _PWM13     : 1;            /*!< [27..27] _PWM13 IP Reset                                                  */
      __IOM uint32_t _PWM12     : 1;            /*!< [28..28] _PWM12 IP Reset                                                  */
      __IOM uint32_t _PWM11     : 1;            /*!< [29..29] _PWM11 IP Reset                                                  */
      __IOM uint32_t _PWM10     : 1;            /*!< [30..30] _PWM10 IP Reset                                                  */
      __IOM uint32_t _PWM9      : 1;            /*!< [31..31] _PWM9 IP Reset                                                   */
    } IPRST1_b;
  } ;
  
  union {
    __IOM uint32_t IPRST2;                      /*!< (@ 0x0000006C) IP_RESET_2 EREGISTER                                       */
    
    struct {
      __IOM uint32_t _I3C2      : 1;            /*!< [0..0] _I3C2 IP Reset                                                     */
      __IOM uint32_t _I3C1      : 1;            /*!< [1..1] _I3C1 IP Reset                                                     */
      __IOM uint32_t _I3C4      : 1;            /*!< [2..2] _I3C4 IP Reset                                                     */
      __IOM uint32_t _LPT       : 1;            /*!< [3..3] _LPT IP Reset                                                      */
      __IOM uint32_t _VINLED    : 1;            /*!< [4..4] _VINLED IP Reset                                                   */
      __IOM uint32_t _I2CDBG    : 1;            /*!< [5..5] _I2CDBG IP Reset                                                   */
      __IOM uint32_t _MICROCNT  : 1;            /*!< [6..6] _MICROCNT IP Reset                                                 */
      __IOM uint32_t _CEC2      : 1;            /*!< [7..7] _CEC2 IP Reset                                                     */
      __IOM uint32_t _CEC1      : 1;            /*!< [8..8] _CEC1 IP Reset                                                     */
      __IOM uint32_t _LEDSTP    : 1;            /*!< [9..9] _LEDSTP IP Reset                                                   */
      __IOM uint32_t _SLWTMR3   : 1;            /*!< [10..10] _SLWTMR3 IP Reset                                                */
      __IOM uint32_t _SLWTMR2   : 1;            /*!< [11..11] _SLWTMR2 IP Reset                                                */
      __IOM uint32_t _SLWTMR1   : 1;            /*!< [12..12] _SLWTMR1 IP Reset                                                */
      __IOM uint32_t _SLWTMR0   : 1;            /*!< [13..13] _SLWTMR0 IP Reset                                                */
      __IOM uint32_t _KBMSCAN   : 1;            /*!< [14..14] _KBMSCAN IP Reset                                                */
      __IOM uint32_t _KBM       : 1;            /*!< [15..15] _KBM IP Reset                                                    */
      __IOM uint32_t _TMR5      : 1;            /*!< [16..16] _TMR5 IP Reset                                                   */
      __IOM uint32_t _TMR4      : 1;            /*!< [17..17] _TMR4 IP Reset                                                   */
      __IOM uint32_t _TMR3      : 1;            /*!< [18..18] _TMR3 IP Reset                                                   */
      __IOM uint32_t _TMR2      : 1;            /*!< [19..19] _TMR2 IP Reset                                                   */
      __IOM uint32_t _TMR1      : 1;            /*!< [20..20] _TMR1 IP Reset                                                   */
      __IOM uint32_t _TMR0      : 1;            /*!< [21..21] _TMR0 IP Reset                                                   */
      __IOM uint32_t _GPIO      : 1;            /*!< [22..22] _GPIO IP Reset                                                   */
      __IOM uint32_t _TACH14    : 1;            /*!< [23..23] _TACH14 IP Reset                                                 */
      __IOM uint32_t _TACH13    : 1;            /*!< [24..24] _TACH13 IP Reset                                                 */
      __IOM uint32_t _TACH12    : 1;            /*!< [25..25] _TACH12 IP Reset                                                 */
      __IOM uint32_t _TACH11    : 1;            /*!< [26..26] _TACH11 IP Reset                                                 */
      __IOM uint32_t _TACH10    : 1;            /*!< [27..27] _TACH10 IP Reset                                                 */
      __IOM uint32_t _TACH9     : 1;            /*!< [28..28] _TACH9 IP Reset                                                  */
      __IOM uint32_t _TACH8     : 1;            /*!< [29..29] _TACH8 IP Reset                                                  */
      __IOM uint32_t _TACH7     : 1;            /*!< [30..30] _TACH7 IP Reset                                                  */
      __IOM uint32_t _TACH6     : 1;            /*!< [31..31] _TACH6 IP Reset                                                  */
    } IPRST2_b;
  } ;
  
  union {
    union {
      __IOM uint32_t APBRST0;                   /*!< (@ 0x00000070) APB_RESET_0 EREGISTER                                      */
      
      struct {
        __IOM uint32_t _PS2_3   : 1;            /*!< [0..0] _PS2_3 APB Reset                                                   */
        __IOM uint32_t _PS2_2   : 1;            /*!< [1..1] _PS2_2 APB Reset                                                   */
        __IOM uint32_t _PS2_1   : 1;            /*!< [2..2] _PS2_1 APB Reset                                                   */
        __IOM uint32_t _LEDSTP  : 1;            /*!< [3..3] _LEDSTP APB Reset                                                  */
        __IOM uint32_t _LED4    : 1;            /*!< [4..4] _LED4 APB Reset                                                    */
        __IOM uint32_t _LED3    : 1;            /*!< [5..5] _LED3 APB Reset                                                    */
        __IOM uint32_t _LED2    : 1;            /*!< [6..6] _LED2 APB Reset                                                    */
        __IOM uint32_t _LED1    : 1;            /*!< [7..7] _LED1 APB Reset                                                    */
        __IOM uint32_t _KBM     : 1;            /*!< [8..8] _KBM APB Reset                                                     */
        __IOM uint32_t _I2CDBG  : 1;            /*!< [9..9] _I2CDBG APB Reset                                                  */
        __IOM uint32_t _I2C11   : 1;            /*!< [10..10] _I2C11 APB Reset                                                 */
        __IOM uint32_t _I2C10   : 1;            /*!< [11..11] _I2C10 APB Reset                                                 */
        __IOM uint32_t _I2C9    : 1;            /*!< [12..12] _I2C9 APB Reset                                                  */
        __IOM uint32_t _I2C8    : 1;            /*!< [13..13] _I2C8 APB Reset                                                  */
        __IOM uint32_t _I2C7    : 1;            /*!< [14..14] _I2C7 APB Reset                                                  */
        __IOM uint32_t _I2C6    : 1;            /*!< [15..15] _I2C6 APB Reset                                                  */
        __IOM uint32_t _I2C5    : 1;            /*!< [16..16] _I2C5 APB Reset                                                  */
        __IOM uint32_t _I2C4    : 1;            /*!< [17..17] _I2C4 APB Reset                                                  */
        __IOM uint32_t _I2C3    : 1;            /*!< [18..18] _I2C3 APB Reset                                                  */
        __IOM uint32_t _I2C2    : 1;            /*!< [19..19] _I2C2 APB Reset                                                  */
        __IOM uint32_t _I2C1    : 1;            /*!< [20..20] _I2C1 APB Reset                                                  */
        __IOM uint32_t _GPIO    : 1;            /*!< [21..21] _GPIO APB Reset                                                  */
        __IOM uint32_t _CEC2    : 1;            /*!< [22..22] _CEC2 APB Reset                                                  */
        __IOM uint32_t _CEC1    : 1;            /*!< [23..23] _CEC1 APB Reset                                                  */
        __IOM uint32_t _ADC     : 1;            /*!< [24..24] _ADC APB Reset                                                   */
              uint32_t          : 7;
      } APBRST0_b;
    } ;
    
    union {
      __IOM uint32_t IPRST3;                    /*!< (@ 0x00000070) IP_RESET_3 EREGISTER                                       */
      
      struct {
              uint32_t          : 25;
        __IOM uint32_t _RTMR    : 1;            /*!< [25..25] _RTMR IP Reset                                                   */
        __IOM uint32_t _PS2_L3  : 1;            /*!< [26..26] _PS2_L3 IP Reset                                                 */
        __IOM uint32_t _PS2_L2  : 1;            /*!< [27..27] _PS2_L2 IP Reset                                                 */
        __IOM uint32_t _PS2_L1  : 1;            /*!< [28..28] _PS2_L1 IP Resetg                                                */
        __IOM uint32_t REF32K   : 1;            /*!< [29..29] REF32K IP Reset                                                  */
        __IOM uint32_t _PECI    : 1;            /*!< [30..30] _PECI IP Reset                                                   */
        __IOM uint32_t _I3C3    : 1;            /*!< [31..31] _I3C3 IP Reset                                                   */
      } IPRST3_b;
    } ;
  };
  
  union {
    __IOM uint32_t APBRST1;                     /*!< (@ 0x00000074) APB_RESET_1 EREGISTER                                      */
    
    struct {
      __IOM uint32_t _TACH8     : 1;            /*!< [0..0] _TACH8 APB Reset                                                   */
      __IOM uint32_t _TACH7     : 1;            /*!< [1..1] _TACH7 APB Reset                                                   */
      __IOM uint32_t _TACH6     : 1;            /*!< [2..2] _TACH6 APB Reset                                                   */
      __IOM uint32_t _TACH5     : 1;            /*!< [3..3] _TACH5 APB Reset                                                   */
      __IOM uint32_t _TACH4     : 1;            /*!< [4..4] _TACH4 APB Reset                                                   */
      __IOM uint32_t _TACH3     : 1;            /*!< [5..5] _TACH3 APB Reset                                                   */
      __IOM uint32_t _TACH2     : 1;            /*!< [6..6] _TACH2 APB Reset                                                   */
      __IOM uint32_t _TACH1     : 1;            /*!< [7..7] _TACH1 APB Reset                                                   */
      __IOM uint32_t _SPIM      : 1;            /*!< [8..8] _SPIM APB Reset                                                    */
      __IOM uint32_t _SPIS      : 1;            /*!< [9..9] _SPIS APB Reset                                                    */
      __IOM uint32_t _SLWTMR3   : 1;            /*!< [10..10] _SLWTMR3 APB Reset                                               */
      __IOM uint32_t _SLWTMR2   : 1;            /*!< [11..11] _SLWTMR2 APB Reset                                               */
      __IOM uint32_t _SLWTMR1   : 1;            /*!< [12..12] _SLWTMR1 APB Reset                                               */
      __IOM uint32_t _SLWTMR0   : 1;            /*!< [13..13] _SLWTMR0 APB Reset                                               */
      __IOM uint32_t _RTMR      : 1;            /*!< [14..14] _RTMR APB Reset                                                  */
      __IOM uint32_t _RTC       : 1;            /*!< [15..15] _RTC APB Reset                                                   */
            uint32_t            : 2;
      __IOM uint32_t _PWM14     : 1;            /*!< [18..18] _PWM14 APB Reset                                                 */
      __IOM uint32_t _PWM13     : 1;            /*!< [19..19] _PWM13 APB Reset                                                 */
      __IOM uint32_t _PWM12     : 1;            /*!< [20..20] _PWM12 APB Reset                                                 */
      __IOM uint32_t _PWM11     : 1;            /*!< [21..21] _PWM11 APB Reset                                                 */
      __IOM uint32_t _PWM10     : 1;            /*!< [22..22] _PWM10 APB Reset                                                 */
      __IOM uint32_t _PWM9      : 1;            /*!< [23..23] _PWM9 APB Reset                                                  */
      __IOM uint32_t _PWM8      : 1;            /*!< [24..24] _PWM8 APB Reset                                                  */
      __IOM uint32_t _PWM7      : 1;            /*!< [25..25] _PWM7 APB Reset                                                  */
      __IOM uint32_t _PWM6      : 1;            /*!< [26..26] _PWM6 APB Reset                                                  */
      __IOM uint32_t _PWM5      : 1;            /*!< [27..27] _PWM5 APB Reset                                                  */
      __IOM uint32_t _PWM4      : 1;            /*!< [28..28] _PWM4 APB Reset                                                  */
      __IOM uint32_t _PWM3      : 1;            /*!< [29..29] _PWM3 APB Reset                                                  */
      __IOM uint32_t _PWM2      : 1;            /*!< [30..30] _PWM2 APB Reset                                                  */
      __IOM uint32_t _PWM1      : 1;            /*!< [31..31] _PWM1 APB Reset                                                  */
    } APBRST1_b;
  } ;
  
  union {
    union {
      __IOM uint32_t IPRST4;                    /*!< (@ 0x00000078) IP_RESET_4 EREGISTER                                       */
      
      struct {
        __IOM uint32_t _USBDHY  : 1;            /*!< [0..0] _USBDHY IP Reset                                                   */
              uint32_t          : 5;
        __IOM uint32_t _USB     : 1;            /*!< [6..6] _USB IP Reset                                                      */
        __IOM uint32_t _TRC     : 1;            /*!< [7..7] _TRC IP Reset                                                      */
        __IOM uint32_t _LALU    : 1;            /*!< [8..8] _LALU IP Reset                                                     */
              uint32_t          : 23;
      } IPRST4_b;
    } ;
    
    union {
      __IOM uint32_t APBRST2;                   /*!< (@ 0x00000078) APB_RESET_2 EREGISTER                                      */
      
      struct {
              uint32_t          : 9;
        __IOM uint32_t _TRC     : 1;            /*!< [9..9] _TRC APB Reset                                                     */
        __IOM uint32_t _PECI    : 1;            /*!< [10..10] _PECI APB Reset                                                  */
        __IOM uint32_t _LPT     : 1;            /*!< [11..11] _LPT APB Reset                                                   */
              uint32_t          : 1;
        __IOM uint32_t _UART2   : 1;            /*!< [13..13] _UART2 APB Reset                                                 */
        __IOM uint32_t _UART1   : 1;            /*!< [14..14] _UART1 APB Reset                                                 */
        __IOM uint32_t _UART0   : 1;            /*!< [15..15] _UART0 APB Reset                                                 */
        __IOM uint32_t _TMR5    : 1;            /*!< [16..16] _TMR5 APB Reset                                                  */
        __IOM uint32_t _TMR4    : 1;            /*!< [17..17] _TMR4 APB Reset                                                  */
        __IOM uint32_t _TMR3    : 1;            /*!< [18..18] _TMR3 APB Reset                                                  */
        __IOM uint32_t _TMR2    : 1;            /*!< [19..19] _TMR2 APB Reset                                                  */
        __IOM uint32_t _TMR1    : 1;            /*!< [20..20] _TMR1 APB Reset                                                  */
        __IOM uint32_t _TMR0    : 1;            /*!< [21..21] _TMR0 APB Reset                                                  */
        __IOM uint32_t _TACH14  : 1;            /*!< [22..22] _TACH14 APB Reset                                                */
        __IOM uint32_t _TACH13  : 1;            /*!< [23..23] _TACH13 APB Reset                                                */
        __IOM uint32_t _TACH12  : 1;            /*!< [24..24] _TACH12 APB Reset                                                */
        __IOM uint32_t _TACH11  : 1;            /*!< [25..25] _TACH11 APB Reset                                                */
        __IOM uint32_t _TACH10  : 1;            /*!< [26..26] _TACH10 APB Reset                                                */
        __IOM uint32_t _TACH9   : 1;            /*!< [27..27] _TACH9 APB Reset                                                 */
              uint32_t          : 4;
      } APBRST2_b;
    } ;
  };
  
  union {
    __IOM uint32_t SOFTRST0;                    /*!< (@ 0x0000007C) SOFT_RESET_0 EREGISTER                                     */
    
    struct {
      __IOM uint32_t _PWM7      : 1;            /*!< [0..0] _PWM7 Soft Reset                                                   */
      __IOM uint32_t _PWM6      : 1;            /*!< [1..1] _PWM6 Soft Reset                                                   */
      __IOM uint32_t _PWM5      : 1;            /*!< [2..2] _PWM5 Soft Reset                                                   */
      __IOM uint32_t _PWM4      : 1;            /*!< [3..3] _PWM4 Soft Reset                                                   */
      __IOM uint32_t _PWM3      : 1;            /*!< [4..4] _PWM3 Soft Reset                                                   */
      __IOM uint32_t _PWM2      : 1;            /*!< [5..5] _PWM2 Soft Reset                                                   */
      __IOM uint32_t _PWM1      : 1;            /*!< [6..6] _PWM1 Soft Reset                                                   */
            uint32_t            : 3;
      __IOM uint32_t _LEDSTP    : 1;            /*!< [10..10] _LEDSTP Soft Reset                                               */
      __IOM uint32_t _LED4      : 1;            /*!< [11..11] _LED4 Soft Reset                                                 */
      __IOM uint32_t _LED3      : 1;            /*!< [12..12] _LED3 Soft Reset                                                 */
      __IOM uint32_t _LED2      : 1;            /*!< [13..13] _LED2 Soft Reset                                                 */
      __IOM uint32_t _LED1      : 1;            /*!< [14..14] _LED1 Soft Reset                                                 */
      __IOM uint32_t _KBM       : 1;            /*!< [15..15] _KBM Soft Reset                                                  */
      __IOM uint32_t _I2CDBG    : 1;            /*!< [16..16] _I2CDBG Soft Reset                                               */
      __IOM uint32_t _I2C11     : 1;            /*!< [17..17] _I2C11 Soft Reset                                                */
      __IOM uint32_t _I2C10     : 1;            /*!< [18..18] _I2C10 Soft Reset                                                */
      __IOM uint32_t _I2C9      : 1;            /*!< [19..19] _I2C9 Soft Reset                                                 */
      __IOM uint32_t _I2C8      : 1;            /*!< [20..20] _I2C8 Soft Reset                                                 */
      __IOM uint32_t _I2C7      : 1;            /*!< [21..21] _I2C7 Soft Reset                                                 */
      __IOM uint32_t _I2C6      : 1;            /*!< [22..22] _I2C6 Soft Reset                                                 */
      __IOM uint32_t _I2C5      : 1;            /*!< [23..23] _I2C5 Soft Reset                                                 */
      __IOM uint32_t _I2C4      : 1;            /*!< [24..24] _I2C4 Soft Reset                                                 */
      __IOM uint32_t _I2C3      : 1;            /*!< [25..25] _I2C3 Soft Reset                                                 */
      __IOM uint32_t _I2C2      : 1;            /*!< [26..26] _I2C2 Soft Reset                                                 */
      __IOM uint32_t _I2C1      : 1;            /*!< [27..27] _I2C1 Soft Reset                                                 */
      __IOM uint32_t _GPIO      : 1;            /*!< [28..28] _GPIO Soft Reset                                                 */
      __IOM uint32_t _CEC2      : 1;            /*!< [29..29] _CEC2 Soft Reset                                                 */
      __IOM uint32_t _CEC1      : 1;            /*!< [30..30] _CEC1 Soft Reset                                                 */
      __IOM uint32_t _ADC       : 1;            /*!< [31..31] _ADC Soft Reset                                                  */
    } SOFTRST0_b;
  } ;
  
  union {
    __IOM uint32_t SOFTRST1;                    /*!< (@ 0x00000080) SOFT_RESET_1 EREGISTER                                     */
    
    struct {
      __IOM uint32_t TMR0       : 1;            /*!< [0..0] TMR0 Soft Reset                                                    */
      __IOM uint32_t _TACH14    : 1;            /*!< [1..1] _TACH14 Soft Reset                                                 */
      __IOM uint32_t _TACH13    : 1;            /*!< [2..2] _TACH13 Soft Reset                                                 */
      __IOM uint32_t _TACH12    : 1;            /*!< [3..3] _TACH12 Soft Reset                                                 */
      __IOM uint32_t _TACH11    : 1;            /*!< [4..4] _TACH11 Soft Reset                                                 */
      __IOM uint32_t _TACH10    : 1;            /*!< [5..5] _TACH10 Soft Reset                                                 */
      __IOM uint32_t _TACH9     : 1;            /*!< [6..6] _TACH9 Soft Reset                                                  */
      __IOM uint32_t _TACH8     : 1;            /*!< [7..7] _TACH8 Soft Reset                                                  */
      __IOM uint32_t _TACH7     : 1;            /*!< [8..8] _TACH7 Soft Reset                                                  */
      __IOM uint32_t _TACH6     : 1;            /*!< [9..9] _TACH6 Soft Reset                                                  */
      __IOM uint32_t _TACH5     : 1;            /*!< [10..10] _TACH5 Soft Reset                                                */
      __IOM uint32_t _TACH4     : 1;            /*!< [11..11] _TACH4 Soft Reset                                                */
      __IOM uint32_t _TACH3     : 1;            /*!< [12..12] _TACH3 Soft Reset                                                */
      __IOM uint32_t _TACH2     : 1;            /*!< [13..13] _TACH2 Soft Reset                                                */
      __IOM uint32_t _TACH1     : 1;            /*!< [14..14] _TACH1 Soft Reset                                                */
      __IOM uint32_t _SPIM      : 1;            /*!< [15..15] _SPIM Soft Reset                                                 */
      __IOM uint32_t _SPIS      : 1;            /*!< [16..16] _SPIS Soft Reset                                                 */
      __IOM uint32_t _SLWTMR3   : 1;            /*!< [17..17] _SLWTMR3 Soft Reset                                              */
      __IOM uint32_t _SLWTMR2   : 1;            /*!< [18..18] _SLWTMR2 Soft Reset                                              */
      __IOM uint32_t _SLWTMR1   : 1;            /*!< [19..19] _SLWTMR1 Soft Reset                                              */
      __IOM uint32_t _SLWTMR0   : 1;            /*!< [20..20] _SLWTMR0 Soft Reset                                              */
      __IOM uint32_t _RTMR      : 1;            /*!< [21..21] _RTMR Soft Reset                                                 */
            uint32_t            : 3;
      __IOM uint32_t _PWM14     : 1;            /*!< [25..25] _PWM14 Soft Reset                                                */
      __IOM uint32_t _PWM13     : 1;            /*!< [26..26] _PWM13 Soft Reset                                                */
      __IOM uint32_t _PWM12     : 1;            /*!< [27..27] _PWM12 Soft Reset                                                */
      __IOM uint32_t _PWM11     : 1;            /*!< [28..28] _PWM11 Soft Reset                                                */
      __IOM uint32_t _PWM10     : 1;            /*!< [29..29] _PWM10 Soft Reset                                                */
      __IOM uint32_t _PWM9      : 1;            /*!< [30..30] _PWM9 Soft Reset                                                 */
      __IOM uint32_t _PWM8      : 1;            /*!< [31..31] _PWM8 Soft Reset                                                 */
    } SOFTRST1_b;
  } ;
  
  union {
    __IOM uint32_t SOFTRST2;                    /*!< (@ 0x00000084) SOFT_RESET_2 EREGISTER                                     */
    
    struct {
      __IOM uint32_t _TRC       : 1;            /*!< [0..0] _TRC Soft Reset                                                    */
      __IOM uint32_t _PECI      : 1;            /*!< [1..1] _PECI Soft Reset                                                   */
      __IOM uint32_t _LPT       : 1;            /*!< [2..2] _LPT Soft Reset                                                    */
            uint32_t            : 1;
      __IOM uint32_t _UART2     : 1;            /*!< [4..4] _UART2 Soft Reset                                                  */
      __IOM uint32_t _UART1     : 1;            /*!< [5..5] _UART1 Soft Reset                                                  */
      __IOM uint32_t _UART0     : 1;            /*!< [6..6] _UART0 Soft Reset                                                  */
      __IOM uint32_t _TMR5      : 1;            /*!< [7..7] _TMR5 Soft Reset                                                   */
      __IOM uint32_t _TMR4      : 1;            /*!< [8..8] _TMR4 Soft Reset                                                   */
      __IOM uint32_t _TMR3      : 1;            /*!< [9..9] _TMR3 Soft Reset                                                   */
      __IOM uint32_t _TMR2      : 1;            /*!< [10..10] _TMR2 Soft Reset                                                 */
      __IOM uint32_t _TMR1      : 1;            /*!< [11..11] _TMR1 Soft Reset                                                 */
            uint32_t            : 20;
    } SOFTRST2_b;
  } ;
  __IM  uint32_t  RESERVED[30];
  
  union {
    __IOM uint32_t CPU;                         /*!< (@ 0x00000100) CPU REGISTER                                               */
    
    struct {
      __IOM uint32_t RSTALL     : 1;            /*!< [0..0] Soft Reset All Regsiters                                           */
      __IOM uint32_t LDO2REGRST : 1;            /*!< [1..1] Reset LDO2 Register                                                */
      __IOM uint32_t PADRSTLOG  : 1;            /*!< [2..2] Pad Reset Log                                                      */
      __IOM uint32_t LDO2RST    : 1;            /*!< [3..3] LDO2 Reset                                                         */
      __IOM uint32_t CPUNUM     : 6;            /*!< [9..4] CPU NUM                                                            */
      __IOM uint32_t CPUINIRST  : 1;            /*!< [10..10] CPU Initial Access Address Reset                                 */
      __IOM uint32_t CPUINIREM  : 1;            /*!< [11..11] CPU Initial Access Address Remain                                */
            uint32_t            : 11;
      __IOM uint32_t RESETINLOCK : 1;           /*!< [23..23] Lock pad_reset_in                                                */
            uint32_t            : 8;
    } CPU_b;
  } ;
  
  union {
    __IOM uint32_t SLPCTRL;                     /*!< (@ 0x00000104) SYSTEM SLEEP CONTROL REGISTER                              */
    
    struct {
            uint32_t            : 1;
      __IOM uint32_t SLPMDSEL   : 1;            /*!< [1..1] Sleep Mode Selection                                               */
      __IOM uint32_t ESPIWKEN   : 1;            /*!< [2..2] ESPI Wake-up Enable                                                */
      __IOM uint32_t PS2WKEN    : 1;            /*!< [3..3] PS2 Wake-up Enable                                                 */
      __IOM uint32_t I2CWKEN    : 1;            /*!< [4..4] I2C Wake-up Enable                                                 */
      __IOM uint32_t GPIOWKEN   : 1;            /*!< [5..5] GPIO Wake-up Enable                                                */
      __IOM uint32_t _SPISWKEN  : 1;            /*!< [6..6] SPIS Wake-up Enable                                                */
            uint32_t            : 2;
      __IOM uint32_t SYSLOCKBIT : 1;            /*!< [9..9] Debug lock bit                                                     */
            uint32_t            : 22;
    } SLPCTRL_b;
  } ;
  __IM  uint32_t  RESERVED1;
  
  union {
    __IOM uint32_t VIVOCTRL;                    /*!< (@ 0x0000010C) VINVOUT CONTROL REGISTER                                   */
    
    struct {
      __IOM uint32_t VIN0MD     : 1;            /*!< [0..0] VIN0 Mode Selection                                                */
      __IOM uint32_t VIN1MD     : 1;            /*!< [1..1] VIN1 Mode Selection                                                */
      __IOM uint32_t VIN2MD     : 1;            /*!< [2..2] VIN2 Mode Selection                                                */
      __IOM uint32_t VIN3MD     : 1;            /*!< [3..3] VIN3 Mode Selection                                                */
      __IOM uint32_t VIN4MD     : 1;            /*!< [4..4] VIN4 Mode Selection                                                */
      __IOM uint32_t VIN5MD     : 1;            /*!< [5..5] VI5 Mode Selection                                                 */
      __IM  uint32_t VIN0STS    : 1;            /*!< [6..6] VIN0 Status                                                        */
      __IM  uint32_t VIN1STS    : 1;            /*!< [7..7] VIN1 Status                                                        */
      __IM  uint32_t VIN2STS    : 1;            /*!< [8..8] VIN2 Status                                                        */
      __IM  uint32_t VIN3STS    : 1;            /*!< [9..9] VIN3 Status                                                        */
      __IM  uint32_t VIN4STS    : 1;            /*!< [10..10] VIN4 Status                                                      */
      __IM  uint32_t VIN5STS    : 1;            /*!< [11..11] VIN5 Status                                                      */
      __IOM uint32_t VIN0POL    : 1;            /*!< [12..12] VIN0 Polarity                                                    */
      __IOM uint32_t VIN1POL    : 1;            /*!< [13..13] VIN1 Polarity                                                    */
      __IOM uint32_t VIN2POL    : 1;            /*!< [14..14] VIN2 Polarity                                                    */
      __IOM uint32_t VIN3POL    : 1;            /*!< [15..15] VIN3 Polarity                                                    */
      __IOM uint32_t VODEVALUE  : 1;            /*!< [16..16] VOUT default value                                               */
      __IOM uint32_t VIN5POL    : 1;            /*!< [17..17] VIN5 Polarity                                                    */
      __IOM uint32_t VOUTDATA   : 1;            /*!< [18..18] Slect VOUT data                                                  */
      __IOM uint32_t LEDBLINKEN : 1;            /*!< [19..19] Enable H/W led blink after select Vin log                        */
      __IOM uint32_t SELLEDPAD  : 4;            /*!< [23..20] Select LED pad out force blinking                                */
      __IOM uint32_t SELLEDPAD_IN : 3;          /*!< [26..24] Select Vin source for hardware LED blinking                      */
      __IOM uint32_t VIVOLOG    : 2;            /*!< [28..27] VIN VOUT LOG                                                     */
      __IM  uint32_t BLINKMODE  : 2;            /*!< [30..29] Select led blink mode                                            */
      __IOM uint32_t VOUTMD     : 1;            /*!< [31..31] VOUT Mode Selection                                              */
    } VIVOCTRL_b;
  } ;
  
  union {
    __IM  uint32_t VIVOMON;                     /*!< (@ 0x00000110) VIN VOUT MONITOR REGISTER                                  */
    
    struct {
      __IM  uint32_t VIN0MD     : 1;            /*!< [0..0] VIN0 Mode Monitor                                                  */
      __IM  uint32_t VIN1MD     : 1;            /*!< [1..1] VIN1 Mode Monitor                                                  */
      __IM  uint32_t VIN2MD     : 1;            /*!< [2..2] VIN2 Mode Monitor                                                  */
      __IM  uint32_t VIN3MD     : 1;            /*!< [3..3] VIN3 Mode Monitor                                                  */
      __IM  uint32_t VIN4MD     : 1;            /*!< [4..4] VIN4 Mode Monitor                                                  */
      __IM  uint32_t VIN5MD     : 1;            /*!< [5..5] VIN5 Mode Monitor                                                  */
      __IM  uint32_t VIN0STS    : 1;            /*!< [6..6] VIN0 Status Monitor                                                */
      __IM  uint32_t VIN1STS    : 1;            /*!< [7..7] VIN1 Status Monitor                                                */
      __IM  uint32_t VIN2STS    : 1;            /*!< [8..8] VIN2 Status Monitor                                                */
      __IM  uint32_t VIN3STS    : 1;            /*!< [9..9] VIN3 Status Monitor                                                */
      __IM  uint32_t VIN4STS    : 1;            /*!< [10..10] VIN4 Status Monitor                                              */
      __IM  uint32_t VIN5STS    : 1;            /*!< [11..11] VIN5 Status Monitor                                              */
      __IM  uint32_t VIN0POL    : 1;            /*!< [12..12] VIN0 Polarity Monitor                                            */
      __IM  uint32_t VIN1POL    : 1;            /*!< [13..13] VIN1 Polarity Monitor                                            */
      __IM  uint32_t VIN2POL    : 1;            /*!< [14..14] VIN2 Polarity Monitor                                            */
      __IM  uint32_t VIN3POL    : 1;            /*!< [15..15] VIN3 Polarity Monitor                                            */
      __IM  uint32_t VODEVALUE  : 1;            /*!< [16..16] VOUT Default Value                                               */
      __IM  uint32_t VIN5POL    : 1;            /*!< [17..17] VIN5 Polarity Monitor                                            */
      __IM  uint32_t VOUTDATA   : 1;            /*!< [18..18] Select VOUT Data Monitor                                         */
      __IM  uint32_t BLINKMODE  : 2;            /*!< [20..19] Led Blink Mode Status                                            */
            uint32_t            : 5;
      __IM  uint32_t ENVIWR     : 1;            /*!< [26..26] Enable VIN Register Write Monitor                                */
      __IM  uint32_t VIVOLOG    : 2;            /*!< [28..27] VIN VOUT LOG Monitor                                             */
      __IM  uint32_t VBATTOP    : 1;            /*!< [29..29] VBAT top status out data                                         */
      __IM  uint32_t HIT1HZ     : 1;            /*!< [30..30] 1 Hz _TMR Status                                                 */
      __IM  uint32_t VOUTMD     : 1;            /*!< [31..31] VOUT MODE SELECTION MONITOR                                      */
    } VIVOMON_b;
  } ;
  
  union {
    __IOM uint32_t LDO2WR;                      /*!< (@ 0x00000114) LDO2 WRITE REGISTER                                        */
    
    struct {
      __IOM uint32_t VIWR       : 1;            /*!< [0..0] Write VIN Register Enable                                          */
      __IOM uint32_t VIVOLOGWR  : 1;            /*!< [1..1] Write VIN VOUT LOG Register Enable                                 */
      __IOM uint32_t VOUTWR     : 1;            /*!< [2..2] Write VOUT Register Enable                                         */
      __IOM uint32_t LDO2WREN   : 1;            /*!< [3..3] Write LDO2 Register Enable                                         */
      __IOM uint32_t _RC32KWR   : 1;            /*!< [4..4] Write _RC32K Register Enable                                       */
            uint32_t            : 27;
    } LDO2WR_b;
  } ;
  
  union {
    __IOM uint32_t LDOCTRL;                     /*!< (@ 0x00000118) LDO CONTROL REGISTER                                       */
    
    struct {
            uint32_t            : 3;
      __IOM uint32_t LDO2PWREN  : 1;            /*!< [3..3] LDO2 Power Enable                                                  */
            uint32_t            : 3;
      __IOM uint32_t LDO3PWREN  : 1;            /*!< [7..7] LDO3 Power Enable                                                  */
            uint32_t            : 21;
      __IOM uint32_t LDO0CFG    : 1;            /*!< [29..29] LDO0 configuration select                                        */
      __IOM uint32_t LDO2CFG    : 1;            /*!< [30..30] LDO2 configuration select                                        */
      __IOM uint32_t LDO3CFG    : 1;            /*!< [31..31] LDO3 configuration select                                        */
    } LDOCTRL_b;
  } ;
  __IM  uint32_t  RESERVED2;
  
  union {
    __IOM uint32_t PLLCTRL;                     /*!< (@ 0x00000120) PLL CONTROL REGISTER                                       */
    
    struct {
      __IOM uint32_t PWREN      : 1;            /*!< [0..0] PLL Power Enable                                                   */
            uint32_t            : 18;
      __IOM uint32_t ARDY       : 1;            /*!< [19..19] PLL Analog Ready and Stable                                      */
      __IOM uint32_t DRDY       : 1;            /*!< [20..20] PLL Digital Ready and Stable                                     */
            uint32_t            : 11;
    } PLLCTRL_b;
  } ;
  __IM  uint32_t  RESERVED3;
  
  union {
    __IOM uint32_t RC32KCTRL;                   /*!< (@ 0x00000128) RC32K CONTROL REGISTER                                     */
    
    struct {
      __IOM uint32_t PWREN      : 1;            /*!< [0..0] RC32K Power Enable                                                 */
      __IOM uint32_t CAL        : 6;            /*!< [6..1] RC32K Calibration Coefficient                                      */
            uint32_t            : 23;
      __IOM uint32_t REGWR_32K  : 1;            /*!< [30..30] RC32K Register Write Enable                                      */
            uint32_t            : 1;
    } RC32KCTRL_b;
  } ;
  
  union {
    __IOM uint32_t TESTOUT0;                    /*!< (@ 0x0000012C) TESTOUT0 REGISTER                                          */
    
    struct {
      __IOM uint32_t TESTSEL0   : 7;            /*!< [6..0] TEST OUT 0 SEL                                                     */
            uint32_t            : 1;
      __IOM uint32_t TESTSEL1   : 7;            /*!< [14..8] TEST OUT 1 SEL                                                    */
            uint32_t            : 1;
      __IOM uint32_t TESTSEL2   : 7;            /*!< [22..16] TEST OUT 2 SEL                                                   */
            uint32_t            : 1;
      __IOM uint32_t TESTSEL3   : 8;            /*!< [31..24] TEST OUT 3 SEL                                                   */
    } TESTOUT0_b;
  } ;
  
  union {
    __IOM uint32_t TESTOUT1;                    /*!< (@ 0x00000130) TESTOUT1 REGISTER                                          */
    
    struct {
      __IOM uint32_t TESTSEL4   : 7;            /*!< [6..0] TEST OUT 4 SEL                                                     */
            uint32_t            : 1;
      __IOM uint32_t TESTSEL5   : 7;            /*!< [14..8] TEST OUT 5 SEL                                                    */
            uint32_t            : 1;
      __IOM uint32_t TESTSEL6   : 7;            /*!< [22..16] TEST OUT 6 SEL                                                   */
            uint32_t            : 1;
      __IOM uint32_t TESTSEL7   : 8;            /*!< [31..24] TEST OUT 7 SEL                                                   */
    } TESTOUT1_b;
  } ;
  
  union {
    __IOM uint32_t IXCINPUTSEL;                 /*!< (@ 0x00000134) IxC Input Signal Select REGISTER                           */
    
    struct {
      __IOM uint32_t _I2C1_SCL_SEL : 1;         /*!< [0..0] SELECT _I2C01 SCL _GPIO                                            */
      __IOM uint32_t _I2C1_SDA_SEL : 1;         /*!< [1..1] SELECT _I2C01 SDA _GPIO                                            */
      __IOM uint32_t _I2C3_SCL_SEL : 2;         /*!< [3..2] SELECT _I2C03 SCL _GPIO                                            */
      __IOM uint32_t _I2C3_SDA_SEL : 1;         /*!< [4..4] SELECT _I2C03 SDA _GPIO                                            */
      __IOM uint32_t _I2C4_SCL_SEL : 1;         /*!< [5..5] SELECT _I2C04 SCL _GPIO                                            */
      __IOM uint32_t _I2C4_SDA_SEL : 1;         /*!< [6..6] SELECT _I2C04 SDA _GPIO                                            */
      __IOM uint32_t _I2C6_SCL_SEL : 1;         /*!< [7..7] SELECT _I2C06 SCL _GPIO                                            */
      __IOM uint32_t _I2C6_SDA_SEL : 1;         /*!< [8..8] SELECT _I2C06 SDA _GPIO                                            */
      __IOM uint32_t _I2C8_SCL_SEL : 1;         /*!< [9..9] SELECT _I2C08 SCL _GPIO                                            */
      __IOM uint32_t _I2C8_SDA_SEL : 1;         /*!< [10..10] SELECT _I2C08 SDA _GPIO                                          */
      __IOM uint32_t _I2C9_SCL_SEL : 1;         /*!< [11..11] SELECT _I2C09 SCL _GPIO                                          */
      __IOM uint32_t _I2C9_SDA_SEL : 1;         /*!< [12..12] SELECT _I2C09 SDA _GPIO                                          */
      __IOM uint32_t _I2C10_SCL_SEL : 1;        /*!< [13..13] SELECT _I2C10 SCL _GPIO                                          */
      __IOM uint32_t _I2C10_SDA_SEL : 1;        /*!< [14..14] SELECT _I2C10 SDA _GPIO                                          */
      __IOM uint32_t _I2C11_SCL_SEL : 1;        /*!< [15..15] SELECT _I2C11 SCL _GPIO                                          */
      __IOM uint32_t _I2C11_SDA_SEL : 1;        /*!< [16..16] SELECT _I2C11 SDA _GPIO                                          */
      __IOM uint32_t _I3C04_SCL_SEL : 1;        /*!< [17..17] SELECT _I3C04 SCL _GPIO                                          */
      __IOM uint32_t _I3C04_SDA_SEL : 1;        /*!< [18..18] SELECT _I3C04 SDA _GPIO                                          */
            uint32_t            : 13;
    } IXCINPUTSEL_b;
  } ;
  
  union {
    __IOM uint32_t _UART__LPT_INPUT_SEL;        /*!< (@ 0x00000138) _UART _LPT Input Signal Select REGISTER                    */
    
    struct {
      __IOM uint32_t _LPT_ACK_SEL : 1;          /*!< [0..0] SELECT _LPT ACK _GPIO                                              */
      __IOM uint32_t _LPT__BUSY_SEL : 1;        /*!< [1..1] SELECT _LPT _BUSY _GPIO                                            */
      __IOM uint32_t _LPT_D0_SEL : 1;           /*!< [2..2] SELECT _LPT_D0 _GPIO                                               */
      __IOM uint32_t _LPT_D1_SEL : 1;           /*!< [3..3] SELECT _LPT_D1 _GPIO                                               */
      __IOM uint32_t _LPT_D2_SEL : 1;           /*!< [4..4] SELECT _LPT_D2 _GPIO                                               */
      __IOM uint32_t _LPT_D3_SEL : 1;           /*!< [5..5] SELECT _LPT_D3 _GPIO                                               */
      __IOM uint32_t _LPT_D4_SEL : 1;           /*!< [6..6] SELECT _LPT_D4 _GPIO                                               */
      __IOM uint32_t _LPT_D5_SEL : 1;           /*!< [7..7] SELECT _LPT_D5 _GPIO                                               */
      __IOM uint32_t _LPT_D6_SEL : 1;           /*!< [8..8] SELECT _LPT_D6 _GPIO                                               */
      __IOM uint32_t _LPT_D7_SEL : 1;           /*!< [9..9] SELECT _LPT_D7 _GPIO                                               */
      __IOM uint32_t _LPT_ERR_SEL : 1;          /*!< [10..10] SELECT _LPT_ERR _GPIO                                            */
      __IOM uint32_t _LPT_PE_SEL : 1;           /*!< [11..11] SELECT _LPT_PE _GPIO                                             */
      __IOM uint32_t _LPT_SLCT_SEL : 1;         /*!< [12..12] SELECT _LPT_SLCT _GPIO                                           */
      __IOM uint32_t _UART01_RX_SEL : 1;        /*!< [13..13] SELECT _UART01 RX _GPIO                                          */
      __IOM uint32_t _UART01_DCD_SEL : 2;       /*!< [15..14] SELECT _UART01 DCD _GPIO                                         */
      __IOM uint32_t _UART01_CTS_SEL : 2;       /*!< [17..16] SELECT _UART01 CTS _GPIO                                         */
      __IOM uint32_t _UART01_DSR_SEL : 1;       /*!< [18..18] SELECT _UART01 DSR _GPIO                                         */
      __IOM uint32_t _UART01_RI_SEL : 2;        /*!< [20..19] SELECT _UART01 RI _GPIO                                          */
      __IOM uint32_t _UART02_CTS_SEL : 2;       /*!< [22..21] SELECT _UART02 CTS _GPIO                                         */
      __IOM uint32_t _UART02_DCD_SEL : 2;       /*!< [24..23] SELECT _UART02 DCD _GPIO                                         */
      __IOM uint32_t _UART02_DSR_SEL : 2;       /*!< [26..25] SELECT _UART02 DSR _GPIO                                         */
      __IOM uint32_t _UART02_RI_SEL : 1;        /*!< [27..27] SELECT _UART02 RI _GPIO                                          */
      __IOM uint32_t _UART02_RX_SEL : 1;        /*!< [28..28] SELECT _UART02 RX _GPIO                                          */
            uint32_t            : 3;
    } _UART__LPT_INPUT_SEL_b;
  } ;
  
  union {
    __IOM uint32_t IP_INPUT_SEL;                /*!< (@ 0x0000013C) IP Input Signal Select REGISTER                            */
    
    struct {
      __IOM uint32_t _TACH01_SEL : 2;           /*!< [1..0] SELECT _TACH01 _GPIO                                               */
      __IOM uint32_t _TACH02_SEL : 2;           /*!< [3..2] SELECT _TACH02 _GPIO                                               */
      __IOM uint32_t _TACH03_SEL : 2;           /*!< [5..4] SELECT _TACH03 _GPIO                                               */
      __IOM uint32_t _TACH04_SEL : 2;           /*!< [7..6] SELECT _TACH04 _GPIO                                               */
      __IOM uint32_t _TACH05_SEL : 2;           /*!< [9..8] SELECT _TACH05 _GPIO                                               */
      __IOM uint32_t _TACH06_SEL : 2;           /*!< [11..10] SELECT _TACH06 _GPIO                                             */
      __IOM uint32_t _TACH07_SEL : 1;           /*!< [12..12] SELECT _TACH07 _GPIO                                             */
      __IOM uint32_t _TACH08_SEL : 1;           /*!< [13..13] SELECT _TACH08 _GPIO                                             */
            uint32_t            : 4;
      __IOM uint32_t _CEC1_SEL  : 1;            /*!< [18..18] SELECT _CEC1 _GPIO                                               */
      __IOM uint32_t _CEC2_SEL  : 1;            /*!< [19..19] SELECT _CEC2 _GPIO                                               */
      __IOM uint32_t ESPI_RST_SEL : 1;          /*!< [20..20] SELECT ESPI_RST _GPIO                                            */
      __IOM uint32_t LPC_RST_SEL : 1;           /*!< [21..21] SELECT LPC_RST _GPIO                                             */
      __IOM uint32_t KSI6_SEL   : 1;            /*!< [22..22] SELECT KSI6 _GPIO                                                */
      __IOM uint32_t KSI7_SEL   : 1;            /*!< [23..23] SELECT KSI7 _GPIO                                                */
      __IOM uint32_t _LEDSTP_SEL : 1;           /*!< [24..24] SELECT _LEDSTP _GPIO                                             */
      __IOM uint32_t _PS2_CLK1_SEL : 2;         /*!< [26..25] SELECT PS2 CLK01 _GPIO                                           */
      __IOM uint32_t _PS2_DATA1_SEL : 1;        /*!< [27..27] SELECT PS2 DATA1 _GPIO                                           */
      __IOM uint32_t _PS2_CLK2_SEL : 1;         /*!< [28..28] SELECT PS2 CLK2 _GPIO                                            */
      __IOM uint32_t _PS2_DATA2_SEL : 1;        /*!< [29..29] SELECT PS2 DATA2 _GPIO                                           */
      __IOM uint32_t _PWRBTN_SEL : 2;           /*!< [31..30] SELECT _PWRBTN _GPIO                                             */
    } IP_INPUT_SEL_b;
  } ;
  
  union {
    __IOM uint32_t SPIC_INPUT_SEL;              /*!< (@ 0x00000140) SPIC Input Signal Select REGISTER                          */
    
    struct {
      __IOM uint32_t _SPIC01_IO0_SEL : 2;       /*!< [1..0] SELECT SPIC01 IO0 GPIO                                             */
      __IOM uint32_t _SPIC01_IO1_SEL : 2;       /*!< [3..2] SELECT SPIC01 IO1 GPIO                                             */
      __IOM uint32_t _SPIC01_IO2_SEL : 2;       /*!< [5..4] SELECT SPIC01 IO2 GPIO                                             */
      __IOM uint32_t _SPIC01_IO3_SEL : 2;       /*!< [7..6] SELECT SPIC01 IO3 GPIO                                             */
            uint32_t            : 24;
    } SPIC_INPUT_SEL_b;
  } ;
  __IM  uint32_t  RESERVED4[249];
  
  union {
    __IOM uint32_t WUE;                         /*!< (@ 0x00000528) WAKE UP ENABLE REGISTER                                    */
    
    struct {
      __IOM uint32_t _ESPIRST   : 1;            /*!< [0..0] ESPI RESET WAKE UP ENABLE                                          */
      __IOM uint32_t _ESPI      : 1;            /*!< [1..1] ESPI WAKE UP ENABLE                                                */
      __IOM uint32_t _PECI      : 1;            /*!< [2..2] PECI WAKE UP ENABLE                                                */
            uint32_t            : 29;
    } WUE_b;
  } ;
  __IM  uint32_t  RESERVED5[181];
  
  union {
    __IOM uint32_t _SPI_FLASH_IO0;              /*!< (@ 0x00000800) _SPI_flash_io0 CONTROL REGISTER                            */
    
    struct {
      __IOM uint32_t INPUT_DETECT : 1;          /*!< [0..0] Input detection                                                    */
      __IOM uint32_t DRIVING    : 1;            /*!< [1..1] DRIVING                                                            */
      __IOM uint32_t _GPIOSLEW  : 1;            /*!< [2..2] _GPIO output value                                                 */
      __IOM uint32_t PULLDOWN   : 1;            /*!< [3..3] PULL DOWN Disable                                                  */
      __IOM uint32_t PULLUP     : 1;            /*!< [4..4] PULL UP Disable                                                    */
      __IOM uint32_t SCHMITTER  : 1;            /*!< [5..5] SCHMITTER Disable                                                  */
      __IOM uint32_t INPUTMODE  : 1;            /*!< [6..6] INPUT DRIVING                                                      */
            uint32_t            : 25;
    } _SPI_FLASH_IO0_b;
  } ;
  
  union {
    __IOM uint32_t _SPI_FLASH_IO1;              /*!< (@ 0x00000804) _SPI_flash_io1 CONTROL REGISTER                            */
    
    struct {
      __IOM uint32_t INPUT_DETECT : 1;          /*!< [0..0] Input detection                                                    */
      __IOM uint32_t DRIVING    : 1;            /*!< [1..1] DRIVING                                                            */
      __IOM uint32_t _GPIOSLEW  : 1;            /*!< [2..2] _GPIO output value                                                 */
      __IOM uint32_t PULLDOWN   : 1;            /*!< [3..3] PULL DOWN Disable                                                  */
      __IOM uint32_t PULLUP     : 1;            /*!< [4..4] PULL UP Disable                                                    */
      __IOM uint32_t SCHMITTER  : 1;            /*!< [5..5] SCHMITTER Disable                                                  */
      __IOM uint32_t INPUTMODE  : 1;            /*!< [6..6] INPUT DRIVING                                                      */
            uint32_t            : 25;
    } _SPI_FLASH_IO1_b;
  } ;
  
  union {
    __IOM uint32_t _SPI_FLASH_IO2;              /*!< (@ 0x00000808) _SPI_flash_io2 CONTROL REGISTER                            */
    
    struct {
      __IOM uint32_t INPUT_DETECT : 1;          /*!< [0..0] Input detection                                                    */
      __IOM uint32_t DRIVING    : 1;            /*!< [1..1] DRIVING                                                            */
      __IOM uint32_t _GPIOSLEW  : 1;            /*!< [2..2] _GPIO output value                                                 */
      __IOM uint32_t PULLDOWN   : 1;            /*!< [3..3] PULL DOWN Disable                                                  */
      __IOM uint32_t PULLUP     : 1;            /*!< [4..4] PULL UP Disable                                                    */
      __IOM uint32_t SCHMITTER  : 1;            /*!< [5..5] SCHMITTER Disable                                                  */
      __IOM uint32_t INPUTMODE  : 1;            /*!< [6..6] INPUT DRIVING                                                      */
            uint32_t            : 25;
    } _SPI_FLASH_IO2_b;
  } ;
  
  union {
    __IOM uint32_t _SPI_FLASH_IO3A;             /*!< (@ 0x0000080C) _SPI_flash_io3a CONTROL REGISTER                           */
    
    struct {
      __IOM uint32_t INPUT_DETECT : 1;          /*!< [0..0] Input detection                                                    */
      __IOM uint32_t DRIVING    : 1;            /*!< [1..1] DRIVING                                                            */
      __IOM uint32_t _GPIOSLEW  : 1;            /*!< [2..2] _GPIO output value                                                 */
      __IOM uint32_t PULLDOWN   : 1;            /*!< [3..3] PULL DOWN Disable                                                  */
      __IOM uint32_t PULLUP     : 1;            /*!< [4..4] PULL UP Disable                                                    */
      __IOM uint32_t SCHMITTER  : 1;            /*!< [5..5] SCHMITTER Disable                                                  */
      __IOM uint32_t INPUTMODE  : 1;            /*!< [6..6] INPUT DRIVING                                                      */
            uint32_t            : 25;
    } _SPI_FLASH_IO3A_b;
  } ;
  
  union {
    __IOM uint32_t _SPI_FLASH_CSN;              /*!< (@ 0x00000810) _SPI_flash_CSN CONTROL REGISTER                            */
    
    struct {
      __IOM uint32_t INPUT_DETECT : 1;          /*!< [0..0] Input detection                                                    */
      __IOM uint32_t DRIVING    : 1;            /*!< [1..1] DRIVING                                                            */
      __IOM uint32_t _GPIOSLEW  : 1;            /*!< [2..2] _GPIO output value                                                 */
      __IOM uint32_t PULLDOWN   : 1;            /*!< [3..3] PULL DOWN Disable                                                  */
      __IOM uint32_t PULLUP     : 1;            /*!< [4..4] PULL UP Disable                                                    */
      __IOM uint32_t SCHMITTER  : 1;            /*!< [5..5] SCHMITTER Disable                                                  */
      __IOM uint32_t INPUTMODE  : 1;            /*!< [6..6] INPUT DRIVING                                                      */
            uint32_t            : 25;
    } _SPI_FLASH_CSN_b;
  } ;
  
  union {
    __IOM uint32_t _SPI_FLASH_SCLK;             /*!< (@ 0x00000814) _SPI_flash_SCLK CONTROL REGISTER                           */
    
    struct {
      __IOM uint32_t INPUT_DETECT : 1;          /*!< [0..0] Input detection                                                    */
      __IOM uint32_t DRIVING    : 1;            /*!< [1..1] DRIVING                                                            */
      __IOM uint32_t _GPIOSLEW  : 1;            /*!< [2..2] _GPIO output value                                                 */
      __IOM uint32_t PULLDOWN   : 1;            /*!< [3..3] PULL DOWN Disable                                                  */
      __IOM uint32_t PULLUP     : 1;            /*!< [4..4] PULL UP Disable                                                    */
      __IOM uint32_t SCHMITTER  : 1;            /*!< [5..5] SCHMITTER Disable                                                  */
      __IOM uint32_t INPUTMODE  : 1;            /*!< [6..6] INPUT DRIVING                                                      */
            uint32_t            : 25;
    } _SPI_FLASH_SCLK_b;
  } ;
  
  union {
    __IOM uint32_t _PECI_PAD;                   /*!< (@ 0x00000818) _PECI_PAD CONTROL REGISTER                                 */
    
    struct {
      __IOM uint32_t INPUT_DETECT : 1;          /*!< [0..0] Input detection                                                    */
      __IOM uint32_t DRIVING    : 1;            /*!< [1..1] DRIVING                                                            */
      __IOM uint32_t _GPIOSLEW  : 1;            /*!< [2..2] _GPIO output value                                                 */
      __IOM uint32_t PULLDOWN   : 1;            /*!< [3..3] PULL DOWN Disable                                                  */
      __IOM uint32_t PULLUP     : 1;            /*!< [4..4] PULL UP Disable                                                    */
      __IOM uint32_t SCHMITTER  : 1;            /*!< [5..5] SCHMITTER Disable                                                  */
            uint32_t            : 26;
    } _PECI_PAD_b;
  } ;
  
  union {
    __IOM uint32_t VREF_VTT_PAD;                /*!< (@ 0x0000081C) VREF_VTT PAD CONTROL REGISTER                              */
    
    struct {
      __IOM uint32_t vref_vtt_pwr_cut : 1;      /*!< [0..0] vref_vtt_pwr_cut                                                   */
            uint32_t            : 31;
    } VREF_VTT_PAD_b;
  } ;
  
  union {
    __IOM uint32_t _ESPI_CLK_PAD;               /*!< (@ 0x00000820) _ESPI_CLK PAD CONTROL REGISTER                             */
    
    struct {
      __IOM uint32_t INPUT_DETECT : 1;          /*!< [0..0] Input detection                                                    */
      __IOM uint32_t DRIVING    : 1;            /*!< [1..1] DRIVING                                                            */
      __IOM uint32_t _GPIOSLEW  : 1;            /*!< [2..2] _GPIO output value                                                 */
      __IOM uint32_t PULLDOWN   : 1;            /*!< [3..3] PULL DOWN Disable                                                  */
      __IOM uint32_t PULLUP     : 1;            /*!< [4..4] PULL UP Disable                                                    */
      __IOM uint32_t SCHMITTER  : 1;            /*!< [5..5] SCHMITTER Disable                                                  */
      __IOM uint32_t INPUTMODE  : 1;            /*!< [6..6] INPUT DRIVING                                                      */
      __IOM uint32_t PWRCUT     : 1;            /*!< [7..7] _ESPI_clk_pwr_cut                                                  */
            uint32_t            : 24;
    } _ESPI_CLK_PAD_b;
  } ;
  
  union {
    __IOM uint32_t _ESPI_IO0_PAD;               /*!< (@ 0x00000824) _ESPI_IO0 PAD CONTROL REGISTER                             */
    
    struct {
      __IOM uint32_t INPUT_DETECT : 1;          /*!< [0..0] Input detection                                                    */
      __IOM uint32_t DRIVING    : 1;            /*!< [1..1] DRIVING                                                            */
      __IOM uint32_t _GPIOSLEW  : 1;            /*!< [2..2] _GPIO output value                                                 */
      __IOM uint32_t PULLDOWN   : 1;            /*!< [3..3] PULL DOWN Disable                                                  */
      __IOM uint32_t PULLUP     : 1;            /*!< [4..4] PULL UP Disable                                                    */
      __IOM uint32_t SCHMITTER  : 1;            /*!< [5..5] SCHMITTER Disable                                                  */
      __IOM uint32_t INPUTMODE  : 1;            /*!< [6..6] INPUT DRIVING                                                      */
      __IOM uint32_t PWRCUT     : 1;            /*!< [7..7] _ESPI_clk_pwr_cut                                                  */
            uint32_t            : 24;
    } _ESPI_IO0_PAD_b;
  } ;
  
  union {
    __IOM uint32_t _ESPI_IO1_PAD;               /*!< (@ 0x00000828) _ESPI_IO1 PAD CONTROL REGISTER                             */
    
    struct {
      __IOM uint32_t INPUT_DETECT : 1;          /*!< [0..0] Input detection                                                    */
      __IOM uint32_t DRIVING    : 1;            /*!< [1..1] DRIVING                                                            */
      __IOM uint32_t _GPIOSLEW  : 1;            /*!< [2..2] _GPIO output value                                                 */
      __IOM uint32_t PULLDOWN   : 1;            /*!< [3..3] PULL DOWN Disable                                                  */
      __IOM uint32_t PULLUP     : 1;            /*!< [4..4] PULL UP Disable                                                    */
      __IOM uint32_t SCHMITTER  : 1;            /*!< [5..5] SCHMITTER Disable                                                  */
      __IOM uint32_t INPUTMODE  : 1;            /*!< [6..6] INPUT DRIVING                                                      */
      __IOM uint32_t PWRCUT     : 1;            /*!< [7..7] _ESPI_clk_pwr_cut                                                  */
            uint32_t            : 24;
    } _ESPI_IO1_PAD_b;
  } ;
  
  union {
    __IOM uint32_t _ESPI_IO2_PAD;               /*!< (@ 0x0000082C) _ESPI_IO2 PAD CONTROL REGISTER                             */
    
    struct {
      __IOM uint32_t INPUT_DETECT : 1;          /*!< [0..0] Input detection                                                    */
      __IOM uint32_t DRIVING    : 1;            /*!< [1..1] DRIVING                                                            */
      __IOM uint32_t _GPIOSLEW  : 1;            /*!< [2..2] _GPIO output value                                                 */
      __IOM uint32_t PULLDOWN   : 1;            /*!< [3..3] PULL DOWN Disable                                                  */
      __IOM uint32_t PULLUP     : 1;            /*!< [4..4] PULL UP Disable                                                    */
      __IOM uint32_t SCHMITTER  : 1;            /*!< [5..5] SCHMITTER Disable                                                  */
      __IOM uint32_t INPUTMODE  : 1;            /*!< [6..6] INPUT DRIVING                                                      */
      __IOM uint32_t PWRCUT     : 1;            /*!< [7..7] _ESPI_clk_pwr_cut                                                  */
            uint32_t            : 24;
    } _ESPI_IO2_PAD_b;
  } ;
  
  union {
    __IOM uint32_t _ESPI_IO3_PAD;               /*!< (@ 0x00000830) _ESPI_IO3 PAD CONTROL REGISTER                             */
    
    struct {
      __IOM uint32_t INPUT_DETECT : 1;          /*!< [0..0] Input detection                                                    */
      __IOM uint32_t DRIVING    : 1;            /*!< [1..1] DRIVING                                                            */
      __IOM uint32_t _GPIOSLEW  : 1;            /*!< [2..2] _GPIO output value                                                 */
      __IOM uint32_t PULLDOWN   : 1;            /*!< [3..3] PULL DOWN Disable                                                  */
      __IOM uint32_t PULLUP     : 1;            /*!< [4..4] PULL UP Disable                                                    */
      __IOM uint32_t SCHMITTER  : 1;            /*!< [5..5] SCHMITTER Disable                                                  */
      __IOM uint32_t INPUTMODE  : 1;            /*!< [6..6] INPUT DRIVING                                                      */
      __IOM uint32_t PWRCUT     : 1;            /*!< [7..7] _ESPI_clk_pwr_cut                                                  */
            uint32_t            : 24;
    } _ESPI_IO3_PAD_b;
  } ;
  
  union {
    __IM  uint32_t _GPIOSTS0;                   /*!< (@ 0x00000834) _GPIO STATUS REGISTER                                      */
    
    struct {
      __IM  uint32_t _GPIO000   : 1;            /*!< [0..0] _GPIO000 STATUS Monitor                                            */
      __IM  uint32_t _GPIO001   : 1;            /*!< [1..1] _GPIO001 STATUS Monitor                                            */
      __IM  uint32_t _GPIO002   : 1;            /*!< [2..2] _GPIO002 STATUS Monitor                                            */
      __IM  uint32_t _GPIO003   : 1;            /*!< [3..3] _GPIO003 STATUS Monitor                                            */
      __IM  uint32_t _GPIO004   : 1;            /*!< [4..4] _GPIO004 STATUS Monitor                                            */
      __IM  uint32_t _GPIO005   : 1;            /*!< [5..5] _GPIO005 STATUS Monitor                                            */
      __IM  uint32_t _GPIO006   : 1;            /*!< [6..6] _GPIO006 STATUS Monitor                                            */
      __IM  uint32_t _GPIO007   : 1;            /*!< [7..7] _GPIO007 STATUS Monitor                                            */
      __IM  uint32_t _GPIO008   : 1;            /*!< [8..8] _GPIO008 STATUS Monitor                                            */
      __IM  uint32_t _GPIO009   : 1;            /*!< [9..9] _GPIO009 STATUS Monitor                                            */
      __IM  uint32_t _GPIO010   : 1;            /*!< [10..10] _GPIO010 STATUS Monitor                                          */
      __IM  uint32_t _GPIO011   : 1;            /*!< [11..11] _GPIO011 STATUS Monitor                                          */
      __IM  uint32_t _GPIO012   : 1;            /*!< [12..12] _GPIO012 STATUS Monitor                                          */
      __IM  uint32_t _GPIO013   : 1;            /*!< [13..13] _GPIO013 STATUS Monitor                                          */
      __IM  uint32_t _GPIO014   : 1;            /*!< [14..14] _GPIO014 STATUS Monitor                                          */
      __IM  uint32_t _GPIO015   : 1;            /*!< [15..15] _GPIO015 STATUS Monitor                                          */
      __IM  uint32_t _GPIO016   : 1;            /*!< [16..16] _GPIO016 STATUS Monitor                                          */
      __IM  uint32_t _GPIO017   : 1;            /*!< [17..17] _GPIO017 STATUS Monitor                                          */
      __IM  uint32_t _GPIO018   : 1;            /*!< [18..18] _GPIO018 STATUS Monitor                                          */
      __IM  uint32_t _GPIO019   : 1;            /*!< [19..19] _GPIO019 STATUS Monitor                                          */
      __IM  uint32_t _GPIO020   : 1;            /*!< [20..20] _GPIO020 STATUS Monitor                                          */
      __IM  uint32_t _GPIO021   : 1;            /*!< [21..21] _GPIO021 STATUS Monitor                                          */
      __IM  uint32_t _GPIO022   : 1;            /*!< [22..22] _GPIO022 STATUS Monitor                                          */
      __IM  uint32_t _GPIO023   : 1;            /*!< [23..23] _GPIO023 STATUS Monitor                                          */
      __IM  uint32_t _GPIO024   : 1;            /*!< [24..24] _GPIO024 STATUS Monitor                                          */
      __IM  uint32_t _GPIO025   : 1;            /*!< [25..25] _GPIO025 STATUS Monitor                                          */
      __IM  uint32_t _GPIO026   : 1;            /*!< [26..26] _GPIO026 STATUS Monitor                                          */
      __IM  uint32_t _GPIO027   : 1;            /*!< [27..27] _GPIO027 STATUS Monitor                                          */
      __IM  uint32_t _GPIO028   : 1;            /*!< [28..28] _GPIO028 STATUS Monitor                                          */
      __IM  uint32_t _GPIO029   : 1;            /*!< [29..29] _GPIO029 STATUS Monitor                                          */
      __IM  uint32_t _GPIO030   : 1;            /*!< [30..30] _GPIO030 STATUS Monitor                                          */
      __IM  uint32_t _GPIO031   : 1;            /*!< [31..31] _GPIO031 STATUS Monitor                                          */
    } _GPIOSTS0_b;
  } ;
  
  union {
    __IM  uint32_t _GPIOSTS1;                   /*!< (@ 0x00000838) _GPIO1 STATUS REGISTER                                     */
    
    struct {
      __IM  uint32_t _GPIO032   : 1;            /*!< [0..0] _GPIO032 STATUS Monitor                                            */
      __IM  uint32_t _GPIO033   : 1;            /*!< [1..1] _GPIO033 STATUS Monitor                                            */
      __IM  uint32_t _GPIO034   : 1;            /*!< [2..2] _GPIO034 STATUS Monitor                                            */
      __IM  uint32_t _GPIO035   : 1;            /*!< [3..3] _GPIO035 STATUS Monitor                                            */
      __IM  uint32_t _GPIO036   : 1;            /*!< [4..4] _GPIO036 STATUS Monitor                                            */
      __IM  uint32_t _GPIO037   : 1;            /*!< [5..5] _GPIO037 STATUS Monitor                                            */
      __IM  uint32_t _GPIO038   : 1;            /*!< [6..6] _GPIO038 STATUS Monitor                                            */
      __IM  uint32_t _GPIO039   : 1;            /*!< [7..7] _GPIO039 STATUS Monitor                                            */
      __IM  uint32_t _GPIO040   : 1;            /*!< [8..8] _GPIO040 STATUS Monitor                                            */
      __IM  uint32_t _GPIO041   : 1;            /*!< [9..9] _GPIO041 STATUS Monitor                                            */
      __IM  uint32_t _GPIO042   : 1;            /*!< [10..10] _GPIO042 STATUS Monitor                                          */
      __IM  uint32_t _GPIO043   : 1;            /*!< [11..11] _GPIO043 STATUS Monitor                                          */
      __IM  uint32_t _GPIO044   : 1;            /*!< [12..12] _GPIO044 STATUS Monitor                                          */
      __IM  uint32_t _GPIO045   : 1;            /*!< [13..13] _GPIO045 STATUS Monitor                                          */
      __IM  uint32_t _GPIO046   : 1;            /*!< [14..14] _GPIO046 STATUS Monitor                                          */
      __IM  uint32_t _GPIO047   : 1;            /*!< [15..15] _GPIO047 STATUS Monitor                                          */
      __IM  uint32_t _GPIO048   : 1;            /*!< [16..16] _GPIO048 STATUS Monitor                                          */
      __IM  uint32_t _GPIO049   : 1;            /*!< [17..17] _GPIO049 STATUS Monitor                                          */
      __IM  uint32_t _GPIO050   : 1;            /*!< [18..18] _GPIO050 STATUS Monitor                                          */
      __IM  uint32_t _GPIO051   : 1;            /*!< [19..19] _GPIO051 STATUS Monitor                                          */
      __IM  uint32_t _GPIO052   : 1;            /*!< [20..20] _GPIO052 STATUS Monitor                                          */
      __IM  uint32_t _GPIO053   : 1;            /*!< [21..21] _GPIO053 STATUS Monitor                                          */
      __IM  uint32_t _GPIO054   : 1;            /*!< [22..22] _GPIO054 STATUS Monitor                                          */
      __IM  uint32_t _GPIO055   : 1;            /*!< [23..23] _GPIO055 STATUS Monitor                                          */
      __IM  uint32_t _GPIO056   : 1;            /*!< [24..24] _GPIO056 STATUS Monitor                                          */
      __IM  uint32_t _GPIO057   : 1;            /*!< [25..25] _GPIO057 STATUS Monitor                                          */
      __IM  uint32_t _GPIO058   : 1;            /*!< [26..26] _GPIO058 STATUS Monitor                                          */
      __IM  uint32_t _GPIO059   : 1;            /*!< [27..27] _GPIO059 STATUS Monitor                                          */
      __IM  uint32_t _GPIO060   : 1;            /*!< [28..28] _GPIO060 STATUS Monitor                                          */
      __IM  uint32_t _GPIO061   : 1;            /*!< [29..29] _GPIO061 STATUS Monitor                                          */
      __IM  uint32_t _GPIO062   : 1;            /*!< [30..30] _GPIO062 STATUS Monitor                                          */
      __IM  uint32_t _GPIO063   : 1;            /*!< [31..31] _GPIO063 STATUS Monitor                                          */
    } _GPIOSTS1_b;
  } ;
  
  union {
    __IM  uint32_t _GPIOSTS2;                   /*!< (@ 0x0000083C) _GPIO2 STATUS REGISTER                                     */
    
    struct {
      __IM  uint32_t _GPIO064   : 1;            /*!< [0..0] _GPIO064 STATUS Monitor                                            */
      __IM  uint32_t _GPIO065   : 1;            /*!< [1..1] _GPIO065 STATUS Monitor                                            */
      __IM  uint32_t _GPIO066   : 1;            /*!< [2..2] _GPIO066 STATUS Monitor                                            */
      __IM  uint32_t _GPIO067   : 1;            /*!< [3..3] _GPIO067 STATUS Monitor                                            */
      __IM  uint32_t _GPIO068   : 1;            /*!< [4..4] _GPIO068 STATUS Monitor                                            */
      __IM  uint32_t _GPIO069   : 1;            /*!< [5..5] _GPIO069 STATUS Monitor                                            */
      __IM  uint32_t _GPIO070   : 1;            /*!< [6..6] _GPIO070 STATUS Monitor                                            */
      __IM  uint32_t _GPIO071   : 1;            /*!< [7..7] _GPIO071 STATUS Monitor                                            */
      __IM  uint32_t _GPIO072   : 1;            /*!< [8..8] _GPIO072 STATUS Monitor                                            */
      __IM  uint32_t _GPIO073   : 1;            /*!< [9..9] _GPIO073 STATUS Monitor                                            */
      __IM  uint32_t _GPIO074   : 1;            /*!< [10..10] _GPIO074 STATUS Monitor                                          */
      __IM  uint32_t _GPIO075   : 1;            /*!< [11..11] _GPIO075 STATUS Monitor                                          */
      __IM  uint32_t _GPIO076   : 1;            /*!< [12..12] _GPIO076 STATUS Monitor                                          */
      __IM  uint32_t _GPIO077   : 1;            /*!< [13..13] _GPIO077 STATUS Monitor                                          */
      __IM  uint32_t _GPIO078   : 1;            /*!< [14..14] _GPIO078 STATUS Monitor                                          */
      __IM  uint32_t _GPIO079   : 1;            /*!< [15..15] _GPIO079 STATUS Monitor                                          */
      __IM  uint32_t _GPIO080   : 1;            /*!< [16..16] _GPIO080 STATUS Monitor                                          */
      __IM  uint32_t _GPIO081   : 1;            /*!< [17..17] _GPIO081 STATUS Monitor                                          */
      __IM  uint32_t _GPIO082   : 1;            /*!< [18..18] _GPIO082 STATUS Monitor                                          */
      __IM  uint32_t _GPIO083   : 1;            /*!< [19..19] _GPIO083 STATUS Monitor                                          */
      __IM  uint32_t _GPIO084   : 1;            /*!< [20..20] _GPIO084 STATUS Monitor                                          */
      __IM  uint32_t _GPIO085   : 1;            /*!< [21..21] _GPIO085 STATUS Monitor                                          */
      __IM  uint32_t _GPIO086   : 1;            /*!< [22..22] _GPIO086 STATUS Monitor                                          */
      __IM  uint32_t _GPIO087   : 1;            /*!< [23..23] _GPIO087 STATUS Monitor                                          */
      __IM  uint32_t _GPIO088   : 1;            /*!< [24..24] _GPIO088 STATUS Monitor                                          */
      __IM  uint32_t _GPIO089   : 1;            /*!< [25..25] _GPIO089 STATUS Monitor                                          */
      __IM  uint32_t _GPIO090   : 1;            /*!< [26..26] _GPIO090 STATUS Monitor                                          */
      __IM  uint32_t _GPIO091   : 1;            /*!< [27..27] _GPIO091 STATUS Monitor                                          */
      __IM  uint32_t _GPIO092   : 1;            /*!< [28..28] _GPIO092 STATUS Monitor                                          */
      __IM  uint32_t _GPIO093   : 1;            /*!< [29..29] _GPIO093 STATUS Monitor                                          */
      __IM  uint32_t _GPIO094   : 1;            /*!< [30..30] _GPIO094 STATUS Monitor                                          */
      __IM  uint32_t _GPIO095   : 1;            /*!< [31..31] _GPIO095 STATUS Monitor                                          */
    } _GPIOSTS2_b;
  } ;
  
  union {
    __IM  uint32_t _GPIOSTS3;                   /*!< (@ 0x00000840) _GPIO3 STATUS REGISTER                                     */
    
    struct {
      __IM  uint32_t _GPIO096   : 1;            /*!< [0..0] _GPIO096 STATUS Monitor                                            */
      __IM  uint32_t _GPIO097   : 1;            /*!< [1..1] _GPIO097 STATUS Monitor                                            */
      __IM  uint32_t _GPIO098   : 1;            /*!< [2..2] _GPIO098 STATUS Monitor                                            */
      __IM  uint32_t _GPIO099   : 1;            /*!< [3..3] _GPIO099 STATUS Monitor                                            */
      __IM  uint32_t _GPIO100   : 1;            /*!< [4..4] _GPIO100 STATUS Monitor                                            */
      __IM  uint32_t _GPIO101   : 1;            /*!< [5..5] _GPIO101 STATUS Monitor                                            */
      __IM  uint32_t _GPIO102   : 1;            /*!< [6..6] _GPIO102 STATUS Monitor                                            */
      __IM  uint32_t _GPIO103   : 1;            /*!< [7..7] _GPIO103 STATUS Monitor                                            */
      __IM  uint32_t _GPIO104   : 1;            /*!< [8..8] _GPIO104 STATUS Monitor                                            */
      __IM  uint32_t _GPIO105   : 1;            /*!< [9..9] _GPIO105 STATUS Monitor                                            */
      __IM  uint32_t _GPIO106   : 1;            /*!< [10..10] _GPIO106 STATUS Monitor                                          */
      __IM  uint32_t _GPIO107   : 1;            /*!< [11..11] _GPIO107 STATUS Monitor                                          */
      __IM  uint32_t _GPIO108   : 1;            /*!< [12..12] _GPIO108 STATUS Monitor                                          */
      __IM  uint32_t _GPIO109   : 1;            /*!< [13..13] _GPIO109 STATUS Monitor                                          */
      __IM  uint32_t _GPIO110   : 1;            /*!< [14..14] _GPIO110 STATUS Monitor                                          */
      __IM  uint32_t _GPIO111   : 1;            /*!< [15..15] _GPIO111 STATUS Monitor                                          */
      __IM  uint32_t _GPIO112   : 1;            /*!< [16..16] _GPIO112 STATUS Monitor                                          */
      __IM  uint32_t _GPIO113   : 1;            /*!< [17..17] _GPIO113 STATUS Monitor                                          */
      __IM  uint32_t _GPIO114   : 1;            /*!< [18..18] _GPIO114 STATUS Monitor                                          */
      __IM  uint32_t _GPIO115   : 1;            /*!< [19..19] _GPIO115 STATUS Monitor                                          */
      __IM  uint32_t _GPIO116   : 1;            /*!< [20..20] _GPIO116 STATUS Monitor                                          */
      __IM  uint32_t _GPIO117   : 1;            /*!< [21..21] _GPIO117 STATUS Monitor                                          */
      __IM  uint32_t _GPIO118   : 1;            /*!< [22..22] _GPIO118 STATUS Monitor                                          */
      __IM  uint32_t _GPIO119   : 1;            /*!< [23..23] _GPIO119 STATUS Monitor                                          */
      __IM  uint32_t _GPIO120   : 1;            /*!< [24..24] _GPIO120 STATUS Monitor                                          */
      __IM  uint32_t _GPIO121   : 1;            /*!< [25..25] _GPIO121 STATUS Monitor                                          */
      __IM  uint32_t _GPIO122   : 1;            /*!< [26..26] _GPIO122 STATUS Monitor                                          */
      __IM  uint32_t _GPIO123   : 1;            /*!< [27..27] _GPIO123 STATUS Monitor                                          */
      __IM  uint32_t _GPIO124   : 1;            /*!< [28..28] _GPIO124 STATUS Monitor                                          */
      __IM  uint32_t _GPIO125   : 1;            /*!< [29..29] _GPIO125 STATUS Monitor                                          */
      __IM  uint32_t _GPIO126   : 1;            /*!< [30..30] _GPIO126 STATUS Monitor                                          */
      __IM  uint32_t _GPIO127   : 1;            /*!< [31..31] _GPIO127 STATUS Monitor                                          */
    } _GPIOSTS3_b;
  } ;
  
  union {
    __IM  uint32_t _GPIOSTS4;                   /*!< (@ 0x00000844) _GPIO4 STATUS REGISTER                                     */
    
    struct {
      __IM  uint32_t _GPIO128   : 1;            /*!< [0..0] _GPIO128 STATUS Monitor                                            */
      __IM  uint32_t _GPIO129   : 1;            /*!< [1..1] _GPIO129 STATUS Monitor                                            */
      __IM  uint32_t _GPIO130   : 1;            /*!< [2..2] _GPIO130 STATUS Monitor                                            */
      __IM  uint32_t _GPIO131   : 1;            /*!< [3..3] _GPIO131 STATUS Monitor                                            */
      __IM  uint32_t _GPIO132   : 1;            /*!< [4..4] _GPIO132 STATUS Monitor                                            */
      __IM  uint32_t _GPIO133   : 1;            /*!< [5..5] _GPIO133 STATUS Monitor                                            */
      __IM  uint32_t _GPIO134   : 1;            /*!< [6..6] _GPIO134 STATUS Monitor                                            */
      __IM  uint32_t _GPIO135   : 1;            /*!< [7..7] _GPIO135 STATUS Monitor                                            */
      __IM  uint32_t _GPIO136   : 1;            /*!< [8..8] _GPIO136 STATUS Monitor                                            */
      __IM  uint32_t _GPIO137   : 1;            /*!< [9..9] _GPIO137 STATUS Monitor                                            */
      __IM  uint32_t _GPIO138   : 1;            /*!< [10..10] _GPIO138 STATUS Monitor                                          */
      __IM  uint32_t _GPIO139   : 1;            /*!< [11..11] _GPIO139 STATUS Monitor                                          */
      __IM  uint32_t _GPIO140   : 1;            /*!< [12..12] _GPIO140 STATUS Monitor                                          */
      __IM  uint32_t _GPIO141   : 1;            /*!< [13..13] _GPIO141 STATUS Monitor                                          */
      __IM  uint32_t _GPIO142   : 1;            /*!< [14..14] _GPIO142 STATUS Monitor                                          */
      __IM  uint32_t _GPIO143   : 1;            /*!< [15..15] _GPIO143 STATUS Monitor                                          */
      __IM  uint32_t _GPIO144   : 1;            /*!< [16..16] _GPIO144 STATUS Monitor                                          */
      __IM  uint32_t _GPIO145   : 1;            /*!< [17..17] _GPIO145 STATUS Monitor                                          */
      __IM  uint32_t _GPIO146   : 1;            /*!< [18..18] _GPIO146 STATUS Monitor                                          */
      __IM  uint32_t _GPIO147   : 1;            /*!< [19..19] _GPIO147 STATUS Monitor                                          */
      __IM  uint32_t _GPIO148   : 1;            /*!< [20..20] _GPIO148 STATUS Monitor                                          */
      __IM  uint32_t _GPIO149   : 1;            /*!< [21..21] _GPIO149 STATUS Monitor                                          */
      __IM  uint32_t _GPIO150   : 1;            /*!< [22..22] _GPIO150 STATUS Monitor                                          */
      __IM  uint32_t _GPIO151   : 1;            /*!< [23..23] _GPIO151 STATUS Monitor                                          */
      __IM  uint32_t _GPIO152   : 1;            /*!< [24..24] _GPIO152 STATUS Monitor                                          */
            uint32_t            : 7;
    } _GPIOSTS4_b;
  } ;
  __IM  uint32_t  RESERVED6[47];
  
  union {
    __IOM uint32_t _32KSRC;                     /*!< (@ 0x00000904) 32K CLK SOURCE REGISTER                                    */
    
    struct {
            uint32_t            : 31;
      __IOM uint32_t S_32KSRC   : 1;            /*!< [31..31] 32K Clock Source                                                 */
    } _32KSRC_b;
  } ;
  __IM  uint32_t  RESERVED7[62];
  
  union {
    __IOM uint32_t VBATIPCLK;                   /*!< (@ 0x00000A00) VBAT IP CLK REGISTER                                       */
    
    struct {
      __IOM uint32_t _RTCPWR    : 1;            /*!< [0..0] _RTC IP Clock Power                                                */
      __IOM uint32_t _PWRBTN1PWR : 1;           /*!< [1..1] _PWRBTN1 IP Clock Power                                            */
      __IOM uint32_t _PWRBTN0PWR : 1;           /*!< [2..2] _PWRBTN0 IP Clock Power                                            */
      __IOM uint32_t _WDTPWR    : 1;            /*!< [3..3] _WDT Clock Power                                                   */
      __IOM uint32_t _RC32KPWR  : 1;            /*!< [4..4] _RC32K Power                                                       */
      __IOM uint32_t _PWRBTN1RST : 1;           /*!< [5..5] _PWRBTN1 IP Reset                                                  */
      __IOM uint32_t _PWRBTN0RST : 1;           /*!< [6..6] _PWRBTN0 IP Reset                                                  */
      __IOM uint32_t _WDTRST    : 1;            /*!< [7..7] _WDT IP Reset                                                      */
      __IOM uint32_t _RTCRST    : 1;            /*!< [8..8] _RTC IP Reset                                                      */
      __IOM uint32_t _RC32KRST  : 1;            /*!< [9..9] _RC32K IP Reset                                                    */
      __IOM uint32_t _PWRBTN1GAT : 1;           /*!< [10..10] _PWRBTN1 IP Clock Gating                                         */
      __IOM uint32_t _PWRBTN0GAT : 1;           /*!< [11..11] _PWRBTN0 IP Clock Gating                                         */
      __IOM uint32_t _WDTGAT    : 1;            /*!< [12..12] _WDT IP Clock Gating                                             */
      __IOM uint32_t _RTCGAT    : 1;            /*!< [13..13] _RTC IP Clock Gating                                             */
      __IOM uint32_t _RC32KGAT  : 1;            /*!< [14..14] _RC32K IP Clock Gating                                           */
            uint32_t            : 2;
      __IOM uint32_t S_RC32K    : 2;            /*!< [18..17] select 32K clock source                                          */
      __IOM uint32_t _RC32KCAL  : 1;            /*!< [19..19] _RC32K Calibration Power                                         */
      __IOM uint32_t _RC32K     : 1;            /*!< [20..20] _RC32K IP Clock Power                                            */
            uint32_t            : 11;
    } VBATIPCLK_b;
  } ;
} SYSTEM_Type;                                  /*!< Size = 2564 (0xa04)                                                       */



/* =========================================================================================================================== */
/* ================                                            PKE                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief Lalu Public Key Engine (PKE)
  */

typedef struct {                                /*!< (@ 0x40080000) PKE Structure                                              */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000000) CONTROL REGISTER                                           */
    
    struct {
            uint32_t            : 5;
      __IOM uint32_t CURVES     : 1;            /*!< [5..5] CURVES                                                             */
      __IOM uint32_t TYPE       : 2;            /*!< [7..6] PKE Function Type                                                  */
      __IOM uint32_t BITLEN     : 16;           /*!< [23..8] Precise RSA/ECC Length in Bits                                    */
            uint32_t            : 1;
      __IOM uint32_t BYTESWAP   : 1;            /*!< [25..25] Bytes Swap Enable                                                */
      __IOM uint32_t WORDSWAP   : 1;            /*!< [26..26] Word Swap Enable                                                 */
            uint32_t            : 3;
      __IOM uint32_t START      : 1;            /*!< [30..30] Start PKE Function                                               */
            uint32_t            : 1;
    } CTRL_b;
  } ;
  __IM  uint32_t  RESERVED;
  
  union {
    __IOM uint32_t STS;                         /*!< (@ 0x00000008) STATUS REGISTER                                            */
    
    struct {
      __IOM uint32_t FIN        : 1;            /*!< [0..0] ECC/RSA Operation Finish                                           */
      __IOM uint32_t PRIMEERR   : 1;            /*!< [1..1] Prime Check Error                                                  */
      __IOM uint32_t R2MODN     : 1;            /*!< [2..2] R^2 Mod N Fail                                                     */
      __IOM uint32_t ECCODDPOINT : 1;           /*!< [3..3] ECC Odd Point                                                      */
      __IOM uint32_t ECCZEQUZERO : 1;           /*!< [4..4] ECC Z = 0                                                          */
      __IOM uint32_t MODINVERR  : 1;            /*!< [5..5] Modulat Inverse Fail                                               */
      __IOM uint32_t INTPUTERR  : 1;            /*!< [6..6] N Less Input Message For RSA                                       */
      __IOM uint32_t EXPERR     : 1;            /*!< [7..7] No Valid Exponent                                                  */
      __IOM uint32_t IDLE       : 1;            /*!< [8..8] PKE Idle                                                           */
            uint32_t            : 23;
    } STS_b;
  } ;
  __IM  uint32_t  RESERVED1[2];
  
  union {
    __IOM uint32_t SWAPADDR;                    /*!< (@ 0x00000014) SWAP ADDRESS REGISTER                                      */
    
    struct {
            uint32_t            : 5;
      __IOM uint32_t ADDR       : 7;            /*!< [11..5] Word Swap Base Address in Word                                    */
            uint32_t            : 20;
    } SWAPADDR_b;
  } ;
  __IM  uint32_t  RESERVED2[16378];
  __IOM uint32_t  MMEM;                         /*!< (@ 0x00010000) PKE MAIN MEMORY                                            */
  __IM  uint32_t  RESERVED3[16383];
  __IOM uint32_t  TMEM;                         /*!< (@ 0x00020000) PKE TEMPORARY MEMORY                                       */
  __IM  uint32_t  RESERVED4[16383];
  __IOM uint32_t  IROM;                         /*!< (@ 0x00030000) PKE INSTRUCTION MEMORY                                     */
} PKE_Type;                                     /*!< Size = 196612 (0x30004)                                                   */



/* =========================================================================================================================== */
/* ================                                            PQC                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief Lalu Post Quantum Cryptography Engine (PQC)
  */

typedef struct {                                /*!< (@ 0x400C0000) PQC Structure                                              */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000000) CONTROL REGISTER                                           */
    
    struct {
            uint32_t            : 5;
      __IOM uint32_t CURVES     : 1;            /*!< [5..5] CURVES                                                             */
      __IOM uint32_t TYPE       : 2;            /*!< [7..6] PKE Function Type                                                  */
      __IOM uint32_t BITLEN     : 16;           /*!< [23..8] Precise RSA/ECC Length in Bits                                    */
            uint32_t            : 1;
      __IOM uint32_t BYTESWAP   : 1;            /*!< [25..25] Bytes Swap Enable                                                */
      __IOM uint32_t WORDSWAP   : 1;            /*!< [26..26] Word Swap Enable                                                 */
            uint32_t            : 3;
      __IOM uint32_t START      : 1;            /*!< [30..30] Start PKE Function                                               */
            uint32_t            : 1;
    } CTRL_b;
  } ;
  __IM  uint32_t  RESERVED;
  
  union {
    __IOM uint32_t STS;                         /*!< (@ 0x00000008) STATUS REGISTER                                            */
    
    struct {
      __IOM uint32_t FIN        : 1;            /*!< [0..0] ECC/RSA Operation Finish                                           */
      __IOM uint32_t PRIMEERR   : 1;            /*!< [1..1] Prime Check Error                                                  */
      __IOM uint32_t R2MODN     : 1;            /*!< [2..2] R^2 Mod N Fail                                                     */
      __IOM uint32_t ECCODDPOINT : 1;           /*!< [3..3] ECC Odd Point                                                      */
      __IOM uint32_t ECCZEQUZERO : 1;           /*!< [4..4] ECC Z = 0                                                          */
      __IOM uint32_t MODINVERR  : 1;            /*!< [5..5] Modulat Inverse Fail                                               */
      __IOM uint32_t INTPUTERR  : 1;            /*!< [6..6] N Less Input Message For RSA                                       */
      __IOM uint32_t EXPERR     : 1;            /*!< [7..7] No Valid Exponent                                                  */
      __IOM uint32_t IDLE       : 1;            /*!< [8..8] PKE Idle                                                           */
            uint32_t            : 23;
    } STS_b;
  } ;
  __IM  uint32_t  RESERVED1[2];
  
  union {
    __IOM uint32_t SWAPADDR;                    /*!< (@ 0x00000014) SWAP ADDRESS REGISTER                                      */
    
    struct {
            uint32_t            : 5;
      __IOM uint32_t ADDR       : 7;            /*!< [11..5] Word Swap Base Address in Word                                    */
            uint32_t            : 20;
    } SWAPADDR_b;
  } ;
  __IM  uint32_t  RESERVED2[16378];
  __IOM uint32_t  MMEM;                         /*!< (@ 0x00010000) PKE MAIN MEMORY                                            */
  __IM  uint32_t  RESERVED3[16383];
  __IOM uint32_t  TMEM;                         /*!< (@ 0x00020000) PKE TEMPORARY MEMORY                                       */
  __IM  uint32_t  RESERVED4[16383];
  __IOM uint32_t  IROM;                         /*!< (@ 0x00030000) PKE INSTRUCTION MEMORY                                     */
} PQC_Type;                                     /*!< Size = 196612 (0x30004)                                                   */



/* =========================================================================================================================== */
/* ================                                           EMI0                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief EMI Controller (EMI0)
  */

typedef struct {                                /*!< (@ 0x40221000) EMI0 Structure                                             */
  
  union {
    __IOM uint32_t CFG;                         /*!< (@ 0x00000000) CONFIGURATION REGISTER                                     */
    
    struct {
      __IOM uint32_t STSADDR    : 2;            /*!< [1..0] Host Status Location at Memory Base Address                        */
      __IOM uint32_t WRPROT     : 1;            /*!< [2..2] Write Protect                                                      */
            uint32_t            : 29;
    } CFG_b;
  } ;
  
  union {
    __IOM uint32_t INTCTRL;                     /*!< (@ 0x00000004) INTERRUPT CONTROL REGISTER                                 */
    
    struct {
      __IOM uint32_t IRQEN      : 1;            /*!< [0..0] Auto IRQ Enable                                                    */
      __IOM uint32_t TGLV       : 1;            /*!< [1..1] Trigger Level Select                                               */
      __IOM uint32_t INTEN      : 1;            /*!< [2..2] Interrupt Enable                                                   */
            uint32_t            : 29;
    } INTCTRL_b;
  } ;
  
  union {
    __IOM uint32_t IRQNUM;                      /*!< (@ 0x00000008) IRQ NUMBER REGISTER                                        */
    
    struct {
      __IOM uint32_t NUM        : 8;            /*!< [7..0] Number of IRQ                                                      */
            uint32_t            : 24;
    } IRQNUM_b;
  } ;
  __IOM uint32_t  SAR;                          /*!< (@ 0x0000000C) SAR REGISTER                                               */
  
  union {
    __IOM uint32_t INTSTS;                      /*!< (@ 0x00000010) INTERRUPT STATUS REGISTER                                  */
    
    struct {
      __IOM uint32_t STS        : 1;            /*!< [0..0] Indicate Wheather The Interrupt is Occurred                        */
            uint32_t            : 31;
    } INTSTS_b;
  } ;
} EMI_Type;                                     /*!< Size = 20 (0x14)                                                          */



/* =========================================================================================================================== */
/* ================                                           GPIO                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief GPIO Controller (GPIO)
  */

typedef struct {                                /*!< (@ 0x40230000) GPIO Structure                                             */
  
  union {
    __IOM uint32_t GCR[160];                    /*!< (@ 0x00000000) CONTROL REGISTER                                           */
    
    struct {
      __IOM uint32_t DIR        : 1;            /*!< [0..0] Direction                                                          */
      __IOM uint32_t INDETEN    : 1;            /*!< [1..1] Input Detection Enable                                             */
      __IOM uint32_t INVOLMD    : 1;            /*!< [2..2] Input Voltage Mode                                                 */
      __IM  uint32_t PINSTS     : 1;            /*!< [3..3] Status of GPIO Pin                                                 */
            uint32_t            : 4;
      __IOM uint32_t MFCTRL     : 3;            /*!< [10..8] Multiple Function Control                                         */
      __IOM uint32_t OUTDRV     : 1;            /*!< [11..11] Driving Current Selection                                        */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [12..12] Slew Rate Selection                                              */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [13..13] Internal Pull-Down Resistor Enable                               */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [14..14] Internal Pull-Up Resistor Enable                                 */
      __IOM uint32_t SCHEN      : 1;            /*!< [15..15] Schmitter-Trigger Enable                                         */
      __IOM uint32_t OUTMD      : 1;            /*!< [16..16] Output Mode Selection                                            */
      __IOM uint32_t OUTCTRL    : 1;            /*!< [17..17] Output Control                                                   */
            uint32_t            : 6;
      __IOM uint32_t INTCTRL    : 3;            /*!< [26..24] Type of GPIO Interrupt                                           */
            uint32_t            : 1;
      __IOM uint32_t INTEN      : 1;            /*!< [28..28] Interrupt Enable                                                 */
            uint32_t            : 2;
      __IOM uint32_t INTSTS     : 1;            /*!< [31..31] Interrupt Status                                                 */
    } GCR_b[160];
  } ;
} GPIO_Type;                                    /*!< Size = 640 (0x280)                                                        */



/* =========================================================================================================================== */
/* ================                                           IOPAD                                           ================ */
/* =========================================================================================================================== */


/**
  * @brief I/O Pad Controller (IOPAD)
  */

typedef struct {                                /*!< (@ 0x40091000) IOPAD Structure                                            */
  
  union {
    __IOM uint32_t FLASHWP;                     /*!< (@ 0x00000000) INTERNAL FLASH_WP PAD CONTROL REGISTER                     */
    
    struct {
      __IOM uint32_t INDETEN    : 1;            /*!< [0..0] Input Detection Enable                                             */
      __IOM uint32_t OUTDRV     : 1;            /*!< [1..1] Pin Status                                                         */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [2..2] Slew-rate selection                                                */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [3..3] Pull-Down Enable                                                   */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [4..4] Pull-Up Enable                                                     */
      __IOM uint32_t SCHEN      : 1;            /*!< [5..5] Schmitter-Trigger Enable                                           */
      __IOM uint32_t INVOLMD    : 1;            /*!< [6..6] Input Voltage Mode                                                 */
            uint32_t            : 25;
    } FLASHWP_b;
  } ;
  
  union {
    __IOM uint32_t FLASHHOLD;                   /*!< (@ 0x00000004) INTERNAL FLASH_HOLD PAD CONTROL REGISTER                   */
    
    struct {
      __IOM uint32_t INDETEN    : 1;            /*!< [0..0] Input Detection Enable                                             */
      __IOM uint32_t OUTDRV     : 1;            /*!< [1..1] Pin Status                                                         */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [2..2] Slew-rate selection                                                */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [3..3] Pull-Down Enable                                                   */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [4..4] Pull-Up Enable                                                     */
      __IOM uint32_t SCHEN      : 1;            /*!< [5..5] Schmitter-Trigger Enable                                           */
      __IOM uint32_t INVOLMD    : 1;            /*!< [6..6] Input Voltage Mode                                                 */
            uint32_t            : 25;
    } FLASHHOLD_b;
  } ;
  
  union {
    __IOM uint32_t FLASHSI;                     /*!< (@ 0x00000008) INTERNAL FLASH_SI PAD CONTROL REGISTER                     */
    
    struct {
      __IOM uint32_t INDETEN    : 1;            /*!< [0..0] Input Detection Enable                                             */
      __IOM uint32_t OUTDRV     : 1;            /*!< [1..1] Pin Status                                                         */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [2..2] Slew-rate selection                                                */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [3..3] Pull-Down Enable                                                   */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [4..4] Pull-Up Enable                                                     */
      __IOM uint32_t SCHEN      : 1;            /*!< [5..5] Schmitter-Trigger Enable                                           */
      __IOM uint32_t INVOLMD    : 1;            /*!< [6..6] Input Voltage Mode                                                 */
            uint32_t            : 25;
    } FLASHSI_b;
  } ;
  
  union {
    __IOM uint32_t FLASHSO;                     /*!< (@ 0x0000000C) INTERNAL FLASH_SO PAD CONTROL REGISTER                     */
    
    struct {
      __IOM uint32_t INDETEN    : 1;            /*!< [0..0] Input Detection Enable                                             */
      __IOM uint32_t OUTDRV     : 1;            /*!< [1..1] Pin Status                                                         */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [2..2] Slew-rate selection                                                */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [3..3] Pull-Down Enable                                                   */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [4..4] Pull-Up Enable                                                     */
      __IOM uint32_t SCHEN      : 1;            /*!< [5..5] Schmitter-Trigger Enable                                           */
      __IOM uint32_t INVOLMD    : 1;            /*!< [6..6] Input Voltage Mode                                                 */
            uint32_t            : 25;
    } FLASHSO_b;
  } ;
  
  union {
    __IOM uint32_t FLASHCS;                     /*!< (@ 0x00000010) INTERNAL FLASH_CS PAD CONTROL REGISTER                     */
    
    struct {
      __IOM uint32_t INDETEN    : 1;            /*!< [0..0] Input Detection Enable                                             */
      __IOM uint32_t OUTDRV     : 1;            /*!< [1..1] Pin Status                                                         */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [2..2] Slew-rate selection                                                */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [3..3] Pull-Down Enable                                                   */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [4..4] Pull-Up Enable                                                     */
      __IOM uint32_t SCHEN      : 1;            /*!< [5..5] Schmitter-Trigger Enable                                           */
      __IOM uint32_t INVOLMD    : 1;            /*!< [6..6] Input Voltage Mode                                                 */
            uint32_t            : 25;
    } FLASHCS_b;
  } ;
  
  union {
    __IOM uint32_t FLASHCLK;                    /*!< (@ 0x00000014) INTERNAL FLASH_CLK PAD CONTROL REGISTER                    */
    
    struct {
      __IOM uint32_t INDETEN    : 1;            /*!< [0..0] Input Detection Enable                                             */
      __IOM uint32_t OUTDRV     : 1;            /*!< [1..1] Pin Status                                                         */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [2..2] Slew-rate selection                                                */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [3..3] Pull-Down Enable                                                   */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [4..4] Pull-Up Enable                                                     */
      __IOM uint32_t SCHEN      : 1;            /*!< [5..5] Schmitter-Trigger Enable                                           */
      __IOM uint32_t INVOLMD    : 1;            /*!< [6..6] Input Voltage Mode                                                 */
            uint32_t            : 25;
    } FLASHCLK_b;
  } ;
  
  union {
    __IOM uint32_t PECI;                        /*!< (@ 0x00000018) PECI PAD CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t INDETEN    : 1;            /*!< [0..0] Input Detection Enable                                             */
      __IOM uint32_t OUTDRV     : 1;            /*!< [1..1] Pin Status                                                         */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [2..2] Slew-rate selection                                                */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [3..3] Pull-Down Enable                                                   */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [4..4] Pull-Up Enable                                                     */
      __IOM uint32_t SCHEN      : 1;            /*!< [5..5] Schmitter-Trigger Enable                                           */
      __IOM uint32_t INVOLMD    : 1;            /*!< [6..6] Input Voltage Mode                                                 */
            uint32_t            : 25;
    } PECI_b;
  } ;
  __IM  uint32_t  RESERVED[3];
  
  union {
    __IOM uint32_t ESPICLK;                     /*!< (@ 0x00000028) ESPI CLK CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t INDETEN    : 1;            /*!< [0..0] Input Detection Enable                                             */
      __IOM uint32_t OUTDRV     : 1;            /*!< [1..1] Pin Status                                                         */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [2..2] Slew-rate selection                                                */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [3..3] Pull-Down Enable                                                   */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [4..4] Pull-Up Enable                                                     */
      __IOM uint32_t SCHEN      : 1;            /*!< [5..5] Schmitter-Trigger Enable                                           */
      __IOM uint32_t INVOLMD    : 1;            /*!< [6..6] Input Voltage Mode                                                 */
            uint32_t            : 25;
    } ESPICLK_b;
  } ;
  
  union {
    __IOM uint32_t ESPIIO0;                     /*!< (@ 0x0000002C) ESPI IO0 CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t INDETEN    : 1;            /*!< [0..0] Input Detection Enable                                             */
      __IOM uint32_t OUTDRV     : 1;            /*!< [1..1] Pin Status                                                         */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [2..2] Slew-rate selection                                                */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [3..3] Pull-Down Enable                                                   */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [4..4] Pull-Up Enable                                                     */
      __IOM uint32_t SCHEN      : 1;            /*!< [5..5] Schmitter-Trigger Enable                                           */
      __IOM uint32_t INVOLMD    : 1;            /*!< [6..6] Input Voltage Mode                                                 */
            uint32_t            : 25;
    } ESPIIO0_b;
  } ;
  
  union {
    __IOM uint32_t ESPIIO1;                     /*!< (@ 0x00000030) ESPI IO1 CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t INDETEN    : 1;            /*!< [0..0] Input Detection Enable                                             */
      __IOM uint32_t OUTDRV     : 1;            /*!< [1..1] Pin Status                                                         */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [2..2] Slew-rate selection                                                */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [3..3] Pull-Down Enable                                                   */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [4..4] Pull-Up Enable                                                     */
      __IOM uint32_t SCHEN      : 1;            /*!< [5..5] Schmitter-Trigger Enable                                           */
      __IOM uint32_t INVOLMD    : 1;            /*!< [6..6] Input Voltage Mode                                                 */
            uint32_t            : 25;
    } ESPIIO1_b;
  } ;
  
  union {
    __IOM uint32_t ESPIIO2;                     /*!< (@ 0x00000034) ESPI IO2 CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t INDETEN    : 1;            /*!< [0..0] Input Detection Enable                                             */
      __IOM uint32_t OUTDRV     : 1;            /*!< [1..1] Pin Status                                                         */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [2..2] Slew-rate selection                                                */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [3..3] Pull-Down Enable                                                   */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [4..4] Pull-Up Enable                                                     */
      __IOM uint32_t SCHEN      : 1;            /*!< [5..5] Schmitter-Trigger Enable                                           */
      __IOM uint32_t INVOLMD    : 1;            /*!< [6..6] Input Voltage Mode                                                 */
            uint32_t            : 25;
    } ESPIIO2_b;
  } ;
  
  union {
    __IOM uint32_t ESPIIO3;                     /*!< (@ 0x00000038) ESPI IO3 CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t INDETEN    : 1;            /*!< [0..0] Input Detection Enable                                             */
      __IOM uint32_t OUTDRV     : 1;            /*!< [1..1] Pin Status                                                         */
      __IOM uint32_t SLEWRATE   : 1;            /*!< [2..2] Slew-rate selection                                                */
      __IOM uint32_t PULLDWEN   : 1;            /*!< [3..3] Pull-Down Enable                                                   */
      __IOM uint32_t PULLUPEN   : 1;            /*!< [4..4] Pull-Up Enable                                                     */
      __IOM uint32_t SCHEN      : 1;            /*!< [5..5] Schmitter-Trigger Enable                                           */
      __IOM uint32_t INVOLMD    : 1;            /*!< [6..6] Input Voltage Mode                                                 */
            uint32_t            : 25;
    } ESPIIO3_b;
  } ;
  __IM  uint32_t  RESERVED1;
  
  union {
    __IM  uint32_t GPIOSTS0;                    /*!< (@ 0x00000040) GPIO STATUS 0 REGISTER                                     */
    
    struct {
      __IM  uint32_t GPIO0      : 1;            /*!< [0..0] GPIO 0 Status                                                      */
      __IM  uint32_t GPIO1      : 1;            /*!< [1..1] GPIO 1 Status                                                      */
      __IM  uint32_t GPIO2      : 1;            /*!< [2..2] GPIO 2 Status                                                      */
      __IM  uint32_t GPIO3      : 1;            /*!< [3..3] GPIO 3 Status                                                      */
      __IM  uint32_t GPIO4      : 1;            /*!< [4..4] GPIO 4 Status                                                      */
      __IM  uint32_t GPIO5      : 1;            /*!< [5..5] GPIO 5 Status                                                      */
      __IM  uint32_t GPIO6      : 1;            /*!< [6..6] GPIO 6 Status                                                      */
      __IM  uint32_t GPIO7      : 1;            /*!< [7..7] GPIO 7 Status                                                      */
      __IM  uint32_t GPIO8      : 1;            /*!< [8..8] GPIO 8 Status                                                      */
      __IM  uint32_t GPIO9      : 1;            /*!< [9..9] GPIO 9 Status                                                      */
      __IM  uint32_t GPIO10     : 1;            /*!< [10..10] GPIO 10 Status                                                   */
      __IM  uint32_t GPIO11     : 1;            /*!< [11..11] GPIO 11 Status                                                   */
      __IM  uint32_t GPIO12     : 1;            /*!< [12..12] GPIO 12 Status                                                   */
      __IM  uint32_t GPIO13     : 1;            /*!< [13..13] GPIO 13 Status                                                   */
      __IM  uint32_t GPIO14     : 1;            /*!< [14..14] GPIO 14 Status                                                   */
      __IM  uint32_t GPIO15     : 1;            /*!< [15..15] GPIO 15 Status                                                   */
      __IM  uint32_t GPIO16     : 1;            /*!< [16..16] GPIO 16 Status                                                   */
      __IM  uint32_t GPIO17     : 1;            /*!< [17..17] GPIO 17 Status                                                   */
      __IM  uint32_t GPIO18     : 1;            /*!< [18..18] GPIO 18 Status                                                   */
      __IM  uint32_t GPIO19     : 1;            /*!< [19..19] GPIO 19 Status                                                   */
      __IM  uint32_t GPIO20     : 1;            /*!< [20..20] GPIO 20 Status                                                   */
      __IM  uint32_t GPIO21     : 1;            /*!< [21..21] GPIO 21 Status                                                   */
      __IM  uint32_t GPIO22     : 1;            /*!< [22..22] GPIO 22 Status                                                   */
      __IM  uint32_t GPIO23     : 1;            /*!< [23..23] GPIO 23 Status                                                   */
      __IM  uint32_t GPIO24     : 1;            /*!< [24..24] GPIO 24 Status                                                   */
      __IM  uint32_t GPIO25     : 1;            /*!< [25..25] GPIO 25 Status                                                   */
      __IM  uint32_t GPIO26     : 1;            /*!< [26..26] GPIO 26 Status                                                   */
      __IM  uint32_t GPIO27     : 1;            /*!< [27..27] GPIO 27 Status                                                   */
      __IM  uint32_t GPIO28     : 1;            /*!< [28..28] GPIO 28 Status                                                   */
      __IM  uint32_t GPIO29     : 1;            /*!< [29..29] GPIO 29 Status                                                   */
      __IM  uint32_t GPIO30     : 1;            /*!< [30..30] GPIO 30 Status                                                   */
      __IM  uint32_t GPIO31     : 1;            /*!< [31..31] GPIO 31 Status                                                   */
    } GPIOSTS0_b;
  } ;
  
  union {
    __IM  uint32_t GPIOSTS1;                    /*!< (@ 0x00000044) GPIO STATUS 1 REGISTER                                     */
    
    struct {
      __IM  uint32_t GPIO32     : 1;            /*!< [0..0] GPIO 32 Status                                                     */
      __IM  uint32_t GPIO33     : 1;            /*!< [1..1] GPIO 33 Status                                                     */
      __IM  uint32_t GPIO34     : 1;            /*!< [2..2] GPIO 34 Status                                                     */
      __IM  uint32_t GPIO35     : 1;            /*!< [3..3] GPIO 35 Status                                                     */
      __IM  uint32_t GPIO36     : 1;            /*!< [4..4] GPIO 36 Status                                                     */
      __IM  uint32_t GPIO37     : 1;            /*!< [5..5] GPIO 37 Status                                                     */
      __IM  uint32_t GPIO38     : 1;            /*!< [6..6] GPIO 38 Status                                                     */
      __IM  uint32_t GPIO39     : 1;            /*!< [7..7] GPIO 39 Status                                                     */
      __IM  uint32_t GPIO40     : 1;            /*!< [8..8] GPIO 40 Status                                                     */
      __IM  uint32_t GPIO41     : 1;            /*!< [9..9] GPIO 41 Status                                                     */
      __IM  uint32_t GPIO42     : 1;            /*!< [10..10] GPIO 42 Status                                                   */
      __IM  uint32_t GPIO43     : 1;            /*!< [11..11] GPIO 43 Status                                                   */
      __IM  uint32_t GPIO44     : 1;            /*!< [12..12] GPIO 44 Status                                                   */
      __IM  uint32_t GPIO45     : 1;            /*!< [13..13] GPIO 45 Status                                                   */
      __IM  uint32_t GPIO46     : 1;            /*!< [14..14] GPIO 46 Status                                                   */
      __IM  uint32_t GPIO47     : 1;            /*!< [15..15] GPIO 47 Status                                                   */
      __IM  uint32_t GPIO48     : 1;            /*!< [16..16] GPIO 48 Status                                                   */
      __IM  uint32_t GPIO49     : 1;            /*!< [17..17] GPIO 49 Status                                                   */
      __IM  uint32_t GPIO50     : 1;            /*!< [18..18] GPIO 50 Status                                                   */
      __IM  uint32_t GPIO51     : 1;            /*!< [19..19] GPIO 51 Status                                                   */
      __IM  uint32_t GPIO52     : 1;            /*!< [20..20] GPIO 52 Status                                                   */
      __IM  uint32_t GPIO53     : 1;            /*!< [21..21] GPIO 53 Status                                                   */
      __IM  uint32_t GPIO54     : 1;            /*!< [22..22] GPIO 54 Status                                                   */
      __IM  uint32_t GPIO55     : 1;            /*!< [23..23] GPIO 55 Status                                                   */
      __IM  uint32_t GPIO56     : 1;            /*!< [24..24] GPIO 56 Status                                                   */
      __IM  uint32_t GPIO57     : 1;            /*!< [25..25] GPIO 57 Status                                                   */
      __IM  uint32_t GPIO58     : 1;            /*!< [26..26] GPIO 58 Status                                                   */
      __IM  uint32_t GPIO59     : 1;            /*!< [27..27] GPIO 59 Status                                                   */
      __IM  uint32_t GPIO60     : 1;            /*!< [28..28] GPIO 60 Status                                                   */
      __IM  uint32_t GPIO61     : 1;            /*!< [29..29] GPIO 61 Status                                                   */
      __IM  uint32_t GPIO62     : 1;            /*!< [30..30] GPIO 62 Status                                                   */
      __IM  uint32_t GPIO63     : 1;            /*!< [31..31] GPIO 63 Status                                                   */
    } GPIOSTS1_b;
  } ;
  
  union {
    __IM  uint32_t GPIOSTS2;                    /*!< (@ 0x00000048) GPIO STATUS 2 REGISTER                                     */
    
    struct {
      __IM  uint32_t GPIO64     : 1;            /*!< [0..0] GPIO 64 Status                                                     */
      __IM  uint32_t GPIO65     : 1;            /*!< [1..1] GPIO 65 Status                                                     */
      __IM  uint32_t GPIO66     : 1;            /*!< [2..2] GPIO 66 Status                                                     */
      __IM  uint32_t GPIO67     : 1;            /*!< [3..3] GPIO 67 Status                                                     */
      __IM  uint32_t GPIO68     : 1;            /*!< [4..4] GPIO 68 Status                                                     */
      __IM  uint32_t GPIO69     : 1;            /*!< [5..5] GPIO 69 Status                                                     */
      __IM  uint32_t GPIO70     : 1;            /*!< [6..6] GPIO 70 Status                                                     */
      __IM  uint32_t GPIO71     : 1;            /*!< [7..7] GPIO 71 Status                                                     */
      __IM  uint32_t GPIO72     : 1;            /*!< [8..8] GPIO 72 Status                                                     */
      __IM  uint32_t GPIO73     : 1;            /*!< [9..9] GPIO 73 Status                                                     */
      __IM  uint32_t GPIO74     : 1;            /*!< [10..10] GPIO 74 Status                                                   */
      __IM  uint32_t GPIO75     : 1;            /*!< [11..11] GPIO 75 Status                                                   */
      __IM  uint32_t GPIO76     : 1;            /*!< [12..12] GPIO 76 Status                                                   */
      __IM  uint32_t GPIO77     : 1;            /*!< [13..13] GPIO 77 Status                                                   */
      __IM  uint32_t GPIO78     : 1;            /*!< [14..14] GPIO 78 Status                                                   */
      __IM  uint32_t GPIO79     : 1;            /*!< [15..15] GPIO 79 Status                                                   */
      __IM  uint32_t GPIO80     : 1;            /*!< [16..16] GPIO 80 Status                                                   */
      __IM  uint32_t GPIO81     : 1;            /*!< [17..17] GPIO 81 Status                                                   */
      __IM  uint32_t GPIO82     : 1;            /*!< [18..18] GPIO 82 Status                                                   */
      __IM  uint32_t GPIO83     : 1;            /*!< [19..19] GPIO 83 Status                                                   */
      __IM  uint32_t GPIO84     : 1;            /*!< [20..20] GPIO 84 Status                                                   */
      __IM  uint32_t GPIO85     : 1;            /*!< [21..21] GPIO 85 Status                                                   */
      __IM  uint32_t GPIO86     : 1;            /*!< [22..22] GPIO 86 Status                                                   */
      __IM  uint32_t GPIO87     : 1;            /*!< [23..23] GPIO 87 Status                                                   */
      __IM  uint32_t GPIO88     : 1;            /*!< [24..24] GPIO 88 Status                                                   */
      __IM  uint32_t GPIO89     : 1;            /*!< [25..25] GPIO 89 Status                                                   */
      __IM  uint32_t GPIO90     : 1;            /*!< [26..26] GPIO 90 Status                                                   */
      __IM  uint32_t GPIO91     : 1;            /*!< [27..27] GPIO 91 Status                                                   */
      __IM  uint32_t GPIO92     : 1;            /*!< [28..28] GPIO 92 Status                                                   */
      __IM  uint32_t GPIO93     : 1;            /*!< [29..29] GPIO 93 Status                                                   */
      __IM  uint32_t GPIO94     : 1;            /*!< [30..30] GPIO 94 Status                                                   */
      __IM  uint32_t GPIO95     : 1;            /*!< [31..31] GPIO 95 Status                                                   */
    } GPIOSTS2_b;
  } ;
  
  union {
    __IM  uint32_t GPIOSTS3;                    /*!< (@ 0x0000004C) GPIO STATUS 3 REGISTER                                     */
    
    struct {
      __IM  uint32_t GPIO96     : 1;            /*!< [0..0] GPIO 96 Status                                                     */
      __IM  uint32_t GPIO97     : 1;            /*!< [1..1] GPIO 97 Status                                                     */
      __IM  uint32_t GPIO98     : 1;            /*!< [2..2] GPIO 98 Status                                                     */
      __IM  uint32_t GPIO99     : 1;            /*!< [3..3] GPIO 99 Status                                                     */
      __IM  uint32_t GPIO100    : 1;            /*!< [4..4] GPIO 100 Status                                                    */
      __IM  uint32_t GPIO101    : 1;            /*!< [5..5] GPIO 101 Status                                                    */
      __IM  uint32_t GPIO102    : 1;            /*!< [6..6] GPIO 102 Status                                                    */
      __IM  uint32_t GPIO103    : 1;            /*!< [7..7] GPIO 103 Status                                                    */
      __IM  uint32_t GPIO104    : 1;            /*!< [8..8] GPIO 104 Status                                                    */
      __IM  uint32_t GPIO105    : 1;            /*!< [9..9] GPIO 105 Status                                                    */
      __IM  uint32_t GPIO106    : 1;            /*!< [10..10] GPIO 106 Status                                                  */
      __IM  uint32_t GPIO107    : 1;            /*!< [11..11] GPIO 107 Status                                                  */
      __IM  uint32_t GPIO108    : 1;            /*!< [12..12] GPIO 108 Status                                                  */
      __IM  uint32_t GPIO109    : 1;            /*!< [13..13] GPIO 109 Status                                                  */
      __IM  uint32_t GPIO110    : 1;            /*!< [14..14] GPIO 110 Status                                                  */
      __IM  uint32_t GPIO111    : 1;            /*!< [15..15] GPIO 111 Status                                                  */
      __IM  uint32_t GPIO112    : 1;            /*!< [16..16] GPIO 112 Status                                                  */
      __IM  uint32_t GPIO113    : 1;            /*!< [17..17] GPIO 113 Status                                                  */
      __IM  uint32_t GPIO114    : 1;            /*!< [18..18] GPIO 114 Status                                                  */
      __IM  uint32_t GPIO115    : 1;            /*!< [19..19] GPIO 115 Status                                                  */
      __IM  uint32_t GPIO116    : 1;            /*!< [20..20] GPIO 116 Status                                                  */
      __IM  uint32_t GPIO117    : 1;            /*!< [21..21] GPIO 117 Status                                                  */
      __IM  uint32_t GPIO118    : 1;            /*!< [22..22] GPIO 118 Status                                                  */
      __IM  uint32_t GPIO119    : 1;            /*!< [23..23] GPIO 119 Status                                                  */
      __IM  uint32_t GPIO120    : 1;            /*!< [24..24] GPIO 120 Status                                                  */
      __IM  uint32_t GPIO121    : 1;            /*!< [25..25] GPIO 121 Status                                                  */
      __IM  uint32_t GPIO122    : 1;            /*!< [26..26] GPIO 122 Status                                                  */
      __IM  uint32_t GPIO123    : 1;            /*!< [27..27] GPIO 123 Status                                                  */
      __IM  uint32_t GPIO124    : 1;            /*!< [28..28] GPIO 124 Status                                                  */
      __IM  uint32_t GPIO125    : 1;            /*!< [29..29] GPIO 125 Status                                                  */
      __IM  uint32_t GPIO126    : 1;            /*!< [30..30] GPIO 126 Status                                                  */
      __IM  uint32_t GPIO127    : 1;            /*!< [31..31] GPIO 127 Status                                                  */
    } GPIOSTS3_b;
  } ;
  
  union {
    __IM  uint32_t GPIOSTS4;                    /*!< (@ 0x00000050) GPIO STATUS 4 REGISTER                                     */
    
    struct {
      __IM  uint32_t GPIO128    : 1;            /*!< [0..0] GPIO 128 Status                                                    */
      __IM  uint32_t GPIO129    : 1;            /*!< [1..1] GPIO 129 Status                                                    */
      __IM  uint32_t GPIO130    : 1;            /*!< [2..2] GPIO 130 Status                                                    */
      __IM  uint32_t GPIO131    : 1;            /*!< [3..3] GPIO 131 Status                                                    */
            uint32_t            : 28;
    } GPIOSTS4_b;
  } ;
} IOPAD_Type;                                   /*!< Size = 84 (0x54)                                                          */



/* =========================================================================================================================== */
/* ================                                           PECI                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief PECI Controller (PECI)
  */

typedef struct {                                /*!< (@ 0x40221900) PECI Structure                                             */
  
  union {
    __IOM uint32_t TX;                          /*!< (@ 0x00000000) TX DATA REGISTER                                           */
    
    struct {
      __IOM uint32_t DATA       : 8;            /*!< [7..0] Tx data                                                            */
            uint32_t            : 24;
    } TX_b;
  } ;
  
  union {
    __IM  uint32_t RX;                          /*!< (@ 0x00000004) RX DATA REGISTER                                           */
    
    struct {
      __IM  uint32_t DATA       : 8;            /*!< [7..0] Rx data                                                            */
            uint32_t            : 24;
    } RX_b;
  } ;
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000008) CONTROL REGISTER                                           */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable PECI Contoller                                              */
            uint32_t            : 2;
      __IOM uint32_t RST        : 1;            /*!< [3..3] Reset PECI Contoller                                               */
            uint32_t            : 1;
      __IOM uint32_t FIFORST    : 1;            /*!< [5..5] Reset PECI FIFO                                                    */
      __IOM uint32_t TXEN       : 1;            /*!< [6..6] Enable PECI Tx                                                     */
      __IOM uint32_t INTEN      : 1;            /*!< [7..7] Enable PECI Interrupt                                              */
            uint32_t            : 24;
    } CTRL_b;
  } ;
  
  union {
    __IOM uint32_t STS0;                        /*!< (@ 0x0000000C) STATUS REGISTER #0                                         */
    
    struct {
      __IOM uint32_t BOFSTS     : 1;            /*!< [0..0] Begin of Frame Status                                              */
      __IOM uint32_t EOFSTS     : 1;            /*!< [1..1] End of Frame Status                                                */
      __IM  uint32_t ERRSTS     : 1;            /*!< [2..2] Error Status                                                       */
            uint32_t            : 4;
      __IM  uint32_t INTSTS     : 1;            /*!< [7..7] Interrupt Status                                                   */
            uint32_t            : 24;
    } STS0_b;
  } ;
  
  union {
    __IM  uint32_t STS1;                        /*!< (@ 0x00000010) STATUS REGISTER #1                                         */
    
    struct {
      __IM  uint32_t TXFULL     : 1;            /*!< [0..0] Tx FIFO is Full                                                    */
      __IM  uint32_t TXEMPTY    : 1;            /*!< [1..1] Tx FIFO is Empty                                                   */
      __IM  uint32_t RXFULL     : 1;            /*!< [2..2] Rx FIFO is Full                                                    */
      __IM  uint32_t RXEMPTY    : 1;            /*!< [3..3] Rx FIFO is Empty                                                   */
            uint32_t            : 3;
      __IM  uint32_t BUSY       : 1;            /*!< [7..7] PECI is busy                                                       */
            uint32_t            : 24;
    } STS1_b;
  } ;
  
  union {
    __IOM uint32_t ERR;                         /*!< (@ 0x00000014) ERROR REGISTER                                             */
    
    struct {
      __IOM uint32_t FCSERR     : 1;            /*!< [0..0] FCS Error                                                          */
      __IOM uint32_t BUSERR     : 1;            /*!< [1..1] Bus Error                                                          */
            uint32_t            : 2;
      __IOM uint32_t TXOV       : 1;            /*!< [4..4] Tx Buffer Overflow                                                 */
      __IOM uint32_t TXUDRUN    : 1;            /*!< [5..5] Tx Buffer Underrun                                                 */
      __IOM uint32_t RXOV       : 1;            /*!< [6..6] Rx Buffer Overflow                                                 */
      __IOM uint32_t CLKERR     : 1;            /*!< [7..7] Error State of Unnormal PECI Clock                                 */
            uint32_t            : 24;
    } ERR_b;
  } ;
  
  union {
    __IOM uint32_t INTEN0;                      /*!< (@ 0x00000018) INTERRUPT ENABLE REGISTER #0                               */
    
    struct {
      __IOM uint32_t BOFEN      : 1;            /*!< [0..0] Enable BOF Interrupt                                               */
      __IOM uint32_t EOFEN      : 1;            /*!< [1..1] Enable EOF Interrupt                                               */
      __IOM uint32_t ERREN      : 1;            /*!< [2..2] Enable ERR Interrupt                                               */
            uint32_t            : 29;
    } INTEN0_b;
  } ;
  
  union {
    __IOM uint32_t INTEN1;                      /*!< (@ 0x0000001C) INTERRUPT ENABLE REGISTER #1                               */
    
    struct {
            uint32_t            : 1;
      __IOM uint32_t WFEEN      : 1;            /*!< [1..1] Enable WFE Interrupt                                               */
      __IOM uint32_t RFFEN      : 1;            /*!< [2..2] Enable RFF Interrupt                                               */
            uint32_t            : 29;
    } INTEN1_b;
  } ;
  
  union {
    __IOM uint32_t OBTL;                        /*!< (@ 0x00000020) OPTIMAL BIT TIME REGISTER LOW                              */
    
    struct {
      __IOM uint32_t VAL        : 8;            /*!< [7..0] Bit time low value                                                 */
            uint32_t            : 24;
    } OBTL_b;
  } ;
  
  union {
    __IOM uint32_t OBTH;                        /*!< (@ 0x00000024) OPTIMAL BIT TIME REGISTER HIGH                             */
    
    struct {
      __IOM uint32_t VAL        : 8;            /*!< [7..0] Bit time high value                                                */
            uint32_t            : 24;
    } OBTH_b;
  } ;
  __IM  uint32_t  RESERVED[8];
  
  union {
    __IOM uint32_t CTRL1;                       /*!< (@ 0x00000048) PECI CONTROL 1 REGISTER                                    */
    
    struct {
            uint32_t            : 7;
      __IOM uint32_t DM         : 1;            /*!< [7..7] Drive Mode                                                         */
            uint32_t            : 24;
    } CTRL1_b;
  } ;
} PECI_Type;                                    /*!< Size = 76 (0x4c)                                                          */



/* =========================================================================================================================== */
/* ================                                            KBC                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief KBC Controller (KBC)
  */

typedef struct {                                /*!< (@ 0x40221700) KBC Structure                                              */
  
  union {
    __IOM uint32_t STS;                         /*!< (@ 0x00000000) STATUS REGISTER                                            */
    
    struct {
      __IM  uint32_t OBF        : 1;            /*!< [0..0] Output Buffer Full Status                                          */
      __IM  uint32_t IBF        : 1;            /*!< [1..1] Iutput Buffer Full Status                                          */
      __IOM uint32_t STS0       : 1;            /*!< [2..2] Genral Purpose Status #0                                           */
      __IM  uint32_t CMDSEL     : 1;            /*!< [3..3] Command or Data Seletion                                           */
      __IOM uint32_t STS1       : 1;            /*!< [4..4] Genral Purpose Status #1                                           */
      __IOM uint32_t STS2       : 1;            /*!< [5..5] Genral Purpose Status #2                                           */
      __IOM uint32_t STS3       : 1;            /*!< [6..6] Genral Purpose Status #3                                           */
      __IOM uint32_t STS4       : 1;            /*!< [7..7] Genral Purpose Status #4                                           */
      __IOM uint32_t CLROBF     : 1;            /*!< [8..8] Clear OBF                                                          */
      __IOM uint32_t CLRIBF     : 1;            /*!< [9..9] Clear IBF                                                          */
      __IM  uint32_t OBFSTS     : 1;            /*!< [10..10] Output Buffer Full Status                                        */
      __IOM uint32_t CLROBSTS   : 1;            /*!< [11..11] Clear Output Buffer Full Status                                  */
            uint32_t            : 20;
    } STS_b;
  } ;
  
  union {
    __IOM uint32_t IB;                          /*!< (@ 0x00000004) INPUT BUFFER REGISTER                                      */
    
    struct {
      __IOM uint32_t IBDAT      : 8;            /*!< [7..0] Inpurt Buffer Data                                                 */
      __IOM uint32_t IBCLR      : 1;            /*!< [8..8] Input Buffer Clear                                                 */
            uint32_t            : 23;
    } IB_b;
  } ;
  
  union {
    __IOM uint32_t OB;                          /*!< (@ 0x00000008) OUTPUT BUFFER REGISTER                                     */
    
    struct {
      __IOM uint32_t OBDAT      : 8;            /*!< [7..0] Output Buffer Data                                                 */
      __IOM uint32_t OBCLR      : 1;            /*!< [8..8] Output Buffer Clear                                                */
            uint32_t            : 23;
    } OB_b;
  } ;
  
  union {
    __IOM uint32_t PTADDR;                      /*!< (@ 0x0000000C) PORT ADDRESS REGISTER                                      */
    
    struct {
      __IOM uint32_t DATADDR    : 16;           /*!< [15..0] Data Port Address                                                 */
      __IOM uint32_t CMDOFS     : 3;            /*!< [18..16] Comand Port Offset                                               */
            uint32_t            : 13;
    } PTADDR_b;
  } ;
  
  union {
    __IOM uint32_t INTEN;                       /*!< (@ 0x00000010) INTERRUPT ENABLE REGISTER                                  */
    
    struct {
      __IOM uint32_t OBFINTEN   : 1;            /*!< [0..0] Output Buffer Interrupt Enable                                     */
      __IOM uint32_t IBFINTEN   : 1;            /*!< [1..1] Iutput Buffer Interrupt Enable                                     */
            uint32_t            : 30;
    } INTEN_b;
  } ;
  
  union {
    __IOM uint32_t VWCTRL0;                     /*!< (@ 0x00000014) VIRTUAL WIRE CONTROL REGISTER #0                           */
    
    struct {
      __IOM uint32_t IRQEN      : 1;            /*!< [0..0] Auto IRQ Enable                                                    */
      __IOM uint32_t TGLV       : 1;            /*!< [1..1] Trigger Level Select                                               */
            uint32_t            : 30;
    } VWCTRL0_b;
  } ;
  
  union {
    __IOM uint32_t VWCTRL1;                     /*!< (@ 0x00000018) VIRTUAL WIRE CONTROL REGISTER #1                           */
    
    struct {
      __IOM uint32_t IRQNUM     : 8;            /*!< [7..0] IRQ Number                                                         */
      __IOM uint32_t ACTSEL     : 1;            /*!< [8..8] Activate Enable                                                    */
            uint32_t            : 23;
    } VWCTRL1_b;
  } ;
} KBC_Type;                                     /*!< Size = 28 (0x1c)                                                          */



/* =========================================================================================================================== */
/* ================                                           ACPI                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief ACPI Controller (ACPI)
  */

typedef struct {                                /*!< (@ 0x40221400) ACPI Structure                                             */
  
  union {
    __IOM uint32_t STS;                         /*!< (@ 0x00000000) STATUS REGISTER                                            */
    
    struct {
      __IM  uint32_t OBF        : 1;            /*!< [0..0] Output Buffer Full Status                                          */
      __IM  uint32_t IBF        : 1;            /*!< [1..1] Iutput Buffer Full Status                                          */
      __IOM uint32_t STS0       : 1;            /*!< [2..2] Genral Purpose Status #0                                           */
      __IM  uint32_t CMDSEL     : 1;            /*!< [3..3] Command or Data Seletion                                           */
      __IOM uint32_t STS1       : 1;            /*!< [4..4] Genral Purpose Status #1                                           */
      __IOM uint32_t STS2       : 1;            /*!< [5..5] Genral Purpose Status #2                                           */
      __IOM uint32_t STS3       : 1;            /*!< [6..6] Genral Purpose Status #3                                           */
      __IOM uint32_t STS4       : 1;            /*!< [7..7] Genral Purpose Status #4                                           */
      __IOM uint32_t CLROBF     : 1;            /*!< [8..8] Clear OBF                                                          */
      __IOM uint32_t CLRIBF     : 1;            /*!< [9..9] Clear IBF                                                          */
      __IM  uint32_t OBFSTS     : 1;            /*!< [10..10] Output Buffer Full Status                                        */
      __IOM uint32_t CLROBSTS   : 1;            /*!< [11..11] Clear Output Buffer Full Status                                  */
            uint32_t            : 20;
    } STS_b;
  } ;
  
  union {
    __IOM uint32_t IB;                          /*!< (@ 0x00000004) INPUT BUFFER REGISTER                                      */
    
    struct {
      __IOM uint32_t IBDAT      : 8;            /*!< [7..0] Inpurt Buffer Data                                                 */
      __IOM uint32_t IBCLR      : 1;            /*!< [8..8] Input Buffer Clear                                                 */
            uint32_t            : 23;
    } IB_b;
  } ;
  
  union {
    __IOM uint32_t OB;                          /*!< (@ 0x00000008) OUTPUT BUFFER REGISTER                                     */
    
    struct {
      __IOM uint32_t OBDAT      : 8;            /*!< [7..0] Output Buffer Data                                                 */
      __IOM uint32_t OBCLR      : 1;            /*!< [8..8] Output Buffer Clear                                                */
            uint32_t            : 23;
    } OB_b;
  } ;
  
  union {
    __IOM uint32_t PTADDR;                      /*!< (@ 0x0000000C) PORT ADDRESS REGISTER                                      */
    
    struct {
      __IOM uint32_t DATADDR    : 16;           /*!< [15..0] Data Port Address                                                 */
      __IOM uint32_t CMDOFS     : 3;            /*!< [18..16] Coomand Port Offset                                              */
            uint32_t            : 13;
    } PTADDR_b;
  } ;
  
  union {
    __IOM uint32_t INTEN;                       /*!< (@ 0x00000010) INTERRUPT ENABLE REGISTER                                  */
    
    struct {
      __IOM uint32_t OBFINTEN   : 1;            /*!< [0..0] Output Buffer Interrupt Enable                                     */
      __IOM uint32_t IBFINTEN   : 1;            /*!< [1..1] Iutput Buffer Interrupt Enable                                     */
            uint32_t            : 30;
    } INTEN_b;
  } ;
  
  union {
    __IOM uint32_t VWCTRL0;                     /*!< (@ 0x00000014) VIRTUAL WIRE CONTROL REGISTER #0                           */
    
    struct {
      __IOM uint32_t IRQEN      : 1;            /*!< [0..0] Auto IRQ Enable                                                    */
      __IOM uint32_t TGLV       : 1;            /*!< [1..1] Trigger Level Select                                               */
            uint32_t            : 30;
    } VWCTRL0_b;
  } ;
  
  union {
    __IOM uint32_t VWCTRL1;                     /*!< (@ 0x00000018) VIRTUAL WIRE CONTROL REGISTER #1                           */
    
    struct {
      __IOM uint32_t IRQNUM     : 8;            /*!< [7..0] IRQ Number                                                         */
      __IOM uint32_t ACTSEL     : 1;            /*!< [8..8] Activate Enable                                                    */
            uint32_t            : 23;
    } VWCTRL1_b;
  } ;
} ACPI_Type;                                    /*!< Size = 28 (0x1c)                                                          */



/* =========================================================================================================================== */
/* ================                                          PMPORT0                                          ================ */
/* =========================================================================================================================== */


/**
  * @brief PM Port Controller (PMPORT0)
  */

typedef struct {                                /*!< (@ 0x4022141C) PMPORT0 Structure                                          */
  
  union {
    __IOM uint32_t STS;                         /*!< (@ 0x00000000) STATUS REGISTER                                            */
    
    struct {
      __IM  uint32_t OBF        : 1;            /*!< [0..0] Output Buffer Full Status                                          */
      __IM  uint32_t IBF        : 1;            /*!< [1..1] Iutput Buffer Full Status                                          */
      __IOM uint32_t STS0       : 1;            /*!< [2..2] Genral Purpose Status #0                                           */
      __IM  uint32_t CMDSEL     : 1;            /*!< [3..3] Command or Data Seletion                                           */
      __IOM uint32_t STS1       : 1;            /*!< [4..4] Genral Purpose Status #1                                           */
      __IOM uint32_t STS2       : 1;            /*!< [5..5] Genral Purpose Status #2                                           */
      __IOM uint32_t STS3       : 1;            /*!< [6..6] Genral Purpose Status #3                                           */
      __IOM uint32_t STS4       : 1;            /*!< [7..7] Genral Purpose Status #4                                           */
      __IOM uint32_t CLROBF     : 1;            /*!< [8..8] Clear OBF                                                          */
      __IOM uint32_t CLRIBF     : 1;            /*!< [9..9] Clear IBF                                                          */
      __IM  uint32_t OBFSTS     : 1;            /*!< [10..10] Output Buffer Full Status                                        */
      __IOM uint32_t CLROBSTS   : 1;            /*!< [11..11] Clear Output Buffer Full Status                                  */
            uint32_t            : 20;
    } STS_b;
  } ;
  
  union {
    __IOM uint32_t IB;                          /*!< (@ 0x00000004) INPUT BUFFER REGISTER                                      */
    
    struct {
      __IOM uint32_t IBDAT      : 8;            /*!< [7..0] Inpurt Buffer Data                                                 */
      __IOM uint32_t IBCLR      : 1;            /*!< [8..8] Input Buffer Clear                                                 */
            uint32_t            : 23;
    } IB_b;
  } ;
  
  union {
    __IOM uint32_t OB;                          /*!< (@ 0x00000008) OUTPUT BUFFER REGISTER                                     */
    
    struct {
      __IOM uint32_t OBDAT      : 8;            /*!< [7..0] Output Buffer Data                                                 */
      __IOM uint32_t OBCLR      : 1;            /*!< [8..8] Output Buffer Clear                                                */
            uint32_t            : 23;
    } OB_b;
  } ;
  
  union {
    __IOM uint32_t PTADDR;                      /*!< (@ 0x0000000C) PORT ADDRESS REGISTER                                      */
    
    struct {
      __IOM uint32_t DATADDR    : 16;           /*!< [15..0] Data Port Address                                                 */
      __IOM uint32_t CMDOFS     : 3;            /*!< [18..16] Comand Port Offset                                               */
            uint32_t            : 13;
    } PTADDR_b;
  } ;
  
  union {
    __IOM uint32_t INTEN;                       /*!< (@ 0x00000010) INTERRUPT ENABLE REGISTER                                  */
    
    struct {
      __IOM uint32_t OBFINTEN   : 1;            /*!< [0..0] Output Buffer Interrupt Enable                                     */
      __IOM uint32_t IBFINTEN   : 1;            /*!< [1..1] Iutput Buffer Interrupt Enable                                     */
            uint32_t            : 30;
    } INTEN_b;
  } ;
  
  union {
    __IOM uint32_t VWCTRL0;                     /*!< (@ 0x00000014) VIRTUAL WIRE CONTROL REGISTER #0                           */
    
    struct {
      __IOM uint32_t IRQEN      : 1;            /*!< [0..0] Auto IRQ Enable                                                    */
      __IOM uint32_t TGLV       : 1;            /*!< [1..1] Trigger Level Select                                               */
            uint32_t            : 30;
    } VWCTRL0_b;
  } ;
  
  union {
    __IOM uint32_t VWCTRL1;                     /*!< (@ 0x00000018) VIRTUAL WIRE CONTROL REGISTER #1                           */
    
    struct {
      __IOM uint32_t IRQNUM     : 8;            /*!< [7..0] IRQ Number                                                         */
      __IOM uint32_t ACTSEL     : 1;            /*!< [8..8] Activate Enable                                                    */
            uint32_t            : 23;
    } VWCTRL1_b;
  } ;
} PMPORT_Type;                                  /*!< Size = 28 (0x1c)                                                          */



/* =========================================================================================================================== */
/* ================                                           DIO0                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief Dedicated I/O Controller (DIO0)
  */

typedef struct {                                /*!< (@ 0x400A02C0) DIO0 Structure                                             */
  
  union {
    __IOM uint32_t BASEADDR;                    /*!< (@ 0x00000000) BASE ADDRESS REGISTER                                      */
    
    struct {
      __IOM uint32_t ADDR       : 16;           /*!< [15..0] Base address                                                      */
            uint32_t            : 16;
    } BASEADDR_b;
  } ;
  
  union {
    __IOM uint32_t ENABLE;                      /*!< (@ 0x00000004) DIO ENABLE REGISTER                                        */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Dedicated I/O Activate                                             */
            uint32_t            : 31;
    } ENABLE_b;
  } ;
} DIO_Type;                                     /*!< Size = 8 (0x8)                                                            */



/* =========================================================================================================================== */
/* ================                                          PORT80                                           ================ */
/* =========================================================================================================================== */


/**
  * @brief PORT80 Controller (PORT80)
  */

typedef struct {                                /*!< (@ 0x40221200) PORT80 Structure                                           */
  
  union {
    __IM  uint32_t STS;                         /*!< (@ 0x00000000) STATUS REGISTER                                            */
    
    struct {
      __IM  uint32_t FIFRDPTR   : 4;            /*!< [3..0] FIFO Read Pointer                                                  */
      __IM  uint32_t FIFWRPTR   : 4;            /*!< [7..4] FIFO Read Pointer                                                  */
            uint32_t            : 24;
    } STS_b;
  } ;
  
  union {
    __IOM uint32_t CFG;                         /*!< (@ 0x00000004) CONFIGURATION REGISTER                                     */
    
    struct {
      __IOM uint32_t CLRFLG     : 1;            /*!< [0..0] Clear Flag                                                         */
      __IOM uint32_t THRENUM    : 2;            /*!< [2..1] FIFO Threshold                                                     */
            uint32_t            : 4;
      __IOM uint32_t THREEN     : 1;            /*!< [7..7] Enable Threshold                                                   */
      __IOM uint32_t UARTPASS   : 1;            /*!< [8..8] Enable Port80 Decode to UART                                       */
            uint32_t            : 23;
    } CFG_b;
  } ;
  
  union {
    __IOM uint32_t INTEN;                       /*!< (@ 0x00000008) INTERRUPT ENABLE REGISTER                                  */
    
    struct {
      __IOM uint32_t THREINTEN  : 1;            /*!< [0..0] Enable Threshold Interrupt                                         */
            uint32_t            : 31;
    } INTEN_b;
  } ;
  
  union {
    __IOM uint32_t DATA;                        /*!< (@ 0x0000000C) DATA REGISTER                                              */
    
    struct {
      __IOM uint32_t DATA       : 32;           /*!< [31..0] Data                                                              */
    } DATA_b;
  } ;
  
  union {
    __IOM uint32_t ADDR;                        /*!< (@ 0x00000010) ADDRESS REGISTER                                           */
    
    struct {
      __IOM uint32_t ADDR       : 16;           /*!< [15..0] Address                                                           */
            uint32_t            : 16;
    } ADDR_b;
  } ;
} PORT80_Type;                                  /*!< Size = 20 (0x14)                                                          */



/* =========================================================================================================================== */
/* ================                                           ESPI                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief eSPI Controller (ESPI)
  */

typedef struct {                                /*!< (@ 0x40220000) ESPI Structure                                             */
  
  union {
    __IOM uint32_t EPSTS;                       /*!< (@ 0x00000000) PERIPHERAL CH. STATUS REGISTER                             */
    
    struct {
      __IOM uint32_t MWDONE     : 1;            /*!< [0..0] Peripheral Ch. Memory Write Done                                   */
      __IOM uint32_t MWADMS     : 1;            /*!< [1..1] Peripheral Ch. Memory Address Mismatch                             */
      __IOM uint32_t MRDONE     : 1;            /*!< [2..2] Peripheral Ch. Memory Read Done                                    */
      __IOM uint32_t LTXDONE    : 1;            /*!< [3..3] Peripheral Ch. LTR Message Tx Done                                 */
      __IOM uint32_t CLRSTS     : 1;            /*!< [4..4] Peripheral Ch. Clear Status                                        */
      __IOM uint32_t IORW       : 1;            /*!< [5..5] Peripheral Ch. IO R/W Short Status                                 */
            uint32_t            : 26;
    } EPSTS_b;
  } ;
  
  union {
    __IOM uint32_t EPCFG;                       /*!< (@ 0x00000004) PERIPHERAL CH. CONFIGURTAION REGISTER                      */
    
    struct {
      __IOM uint32_t CHEN       : 1;            /*!< [0..0] Peripheral Ch. Enable                                              */
      __IOM uint32_t CHRDY      : 1;            /*!< [1..1] Peripheral Ch. Ready                                               */
      __IOM uint32_t BMTREN     : 1;            /*!< [2..2] Peripheral Ch. Bus Master Enable                                   */
            uint32_t            : 1;
      __IOM uint32_t MXPLSUP    : 3;            /*!< [6..4] Peripheral Ch. Maximum Payload Size Supported                      */
            uint32_t            : 1;
      __IOM uint32_t MXPLSEL    : 3;            /*!< [10..8] Peripheral Ch. Maximum Payload Size Selected                      */
            uint32_t            : 1;
      __IOM uint32_t MXRDSZ     : 3;            /*!< [14..12] Peripheral Ch. Maximum Read Request Size                         */
            uint32_t            : 17;
    } EPCFG_b;
  } ;
  
  union {
    __IOM uint32_t EPINTEN;                     /*!< (@ 0x00000008) PERIPHERAL CH. INTERRUPT ENABLE REGISTER                   */
    
    struct {
      __IOM uint32_t CFGCHGEN   : 1;            /*!< [0..0] Peripheral Ch. Configuration Change Interrupt Enable               */
      __IOM uint32_t MEMWREN    : 1;            /*!< [1..1] Peripheral Ch. Memory Write Interrupt Enable                       */
      __IOM uint32_t MEMRDEN    : 1;            /*!< [2..2] Peripheral Ch. Memory Read Interrupt Enable                        */
      __IOM uint32_t IORWEN     : 1;            /*!< [3..3] Peripheral Ch. IO R/W Short Interrupt Enable                       */
            uint32_t            : 28;
    } EPINTEN_b;
  } ;
  
  union {
    __IM  uint32_t EPRXINFO;                    /*!< (@ 0x0000000C) PERIPHERAL CH. RECEIVE PACKET INFORMATION REGISTER         */
    
    struct {
      __IM  uint32_t LENGTH     : 12;           /*!< [11..0] Peripheral Ch. Rx Command Length                                  */
      __IM  uint32_t TAG        : 4;            /*!< [15..12] Peripheral Ch. RX Command Tag                                    */
      __IM  uint32_t CYCLE      : 8;            /*!< [23..16] Peripheral Ch. RX Command Cycle                                  */
      __IM  uint32_t OPCODE     : 8;            /*!< [31..24] Peripheral Ch. RX Command Opcode                                 */
    } EPRXINFO_b;
  } ;
  __IOM uint32_t  EPRXADRH;                     /*!< (@ 0x00000010) PERIPHERAL CH. RECEIVE ADDRESS HIGH REGISTER               */
  __IOM uint32_t  EPRXADRL;                     /*!< (@ 0x00000014) PERIPHERAL CH. RECEIVE ADDRESS LOW REGISTER                */
  __IOM uint32_t  EPCPRADR;                     /*!< (@ 0x00000018) PERIPHERAL CH. COMPARE ADDRESS REGISTER                    */
  
  union {
    __IOM uint32_t EPCPRVLD;                    /*!< (@ 0x0000001C) PERIPHERAL CH. COMPARE ADDRESS VALID REGISTER              */
    
    struct {
      __IOM uint32_t VALIDEN    : 1;            /*!< [0..0] Peripheral Ch. Memory Write Address Compare Enable                 */
            uint32_t            : 31;
    } EPCPRVLD_b;
  } ;
  __IOM uint32_t  EPBUF;                        /*!< (@ 0x00000020) PERIPHERAL CH. BUFFER REGISTER                             */
  
  union {
    __IOM uint32_t EPBUFSZ;                     /*!< (@ 0x00000024) PERIPHERAL CH. BUFFER SIZE REGISTER                        */
    
    struct {
      __IOM uint32_t SIZE       : 4;            /*!< [3..0] Peripheral Ch. Memory Buffer Size                                  */
            uint32_t            : 28;
    } EPBUFSZ_b;
  } ;
  
  union {
    __IOM uint32_t EPPTRCLR;                    /*!< (@ 0x00000028) PERIPHERAL CH. POINTER CLEAR REGISTER                      */
    
    struct {
      __IOM uint32_t WRCLR      : 1;            /*!< [0..0] Reset Peripheral Ch. Memory Write Pointer                          */
      __IOM uint32_t RDCLR      : 1;            /*!< [1..1] Reset Peripheral Ch. Memory Read Pointer                           */
            uint32_t            : 30;
    } EPPTRCLR_b;
  } ;
  
  union {
    __IOM uint32_t ELMSG;                       /*!< (@ 0x0000002C) PERIPHERAL CH. LTR MESSAGE REGISTER                        */
    
    struct {
      __IOM uint32_t MSGDAT     : 16;           /*!< [15..0] Data of LTR Message to be Transfered                              */
      __IOM uint32_t MSGCODE    : 16;           /*!< [31..16] Code of LTR Message to be Transfered                             */
    } ELMSG_b;
  } ;
  __IOM uint32_t  EPMRDA;                       /*!< (@ 0x00000030) PERIPHERAL CH. MEMORY RECEIVE DATA REGISTER                */
  __IOM uint32_t  EPMRADRL;                     /*!< (@ 0x00000034) PERIPHERAL CH. MEMORY RECEIVE ADDRESS LOW REGISTER         */
  __IOM uint32_t  EPMRADRH;                     /*!< (@ 0x00000038) PERIPHERAL CH. MEMORY RECEIVE ADDRESS HIGH REGISTER        */
  
  union {
    __IOM uint32_t EPMRLEN;                     /*!< (@ 0x0000003C) PERIPHERAL CH. MEMORY RECEIVE LENGTH REGISTER              */
    
    struct {
      __IOM uint32_t RXLEN      : 12;           /*!< [11..0] Peripheral Ch. Memory Receive Length                              */
            uint32_t            : 20;
    } EPMRLEN_b;
  } ;
  
  union {
    __IOM uint32_t EVSTS;                       /*!< (@ 0x00000040) VIRTUAL WIRE CH. STATUS REGISTER                           */
    
    struct {
      __IM  uint32_t RXEPT      : 1;            /*!< [0..0] Virtual Wire Ch. Rx FIFO Empty                                     */
      __IOM uint32_t RXOVR      : 1;            /*!< [1..1] Virtual Wire Ch. Rx FIFO Overrun                                   */
      __IM  uint32_t TXFULL     : 1;            /*!< [2..2] Virtual Wire Ch. Tx FIFO Full                                      */
      __IOM uint32_t ILLCHG     : 1;            /*!< [3..3] Virtual Wire Ch. Illegal Index                                     */
      __IOM uint32_t IDX2CHG    : 1;            /*!< [4..4] Virtual Wire Ch. Index2 Changed                                    */
      __IOM uint32_t IDX3CHG    : 1;            /*!< [5..5] Virtual Wire Ch. Index3 Changed                                    */
      __IOM uint32_t IDX7CHG    : 1;            /*!< [6..6] Virtual Wire Ch. Index7 Changed                                    */
      __IOM uint32_t IDX41CHG   : 1;            /*!< [7..7] Virtual Wire Ch. Index41 Changed                                   */
      __IOM uint32_t IDX42CHG   : 1;            /*!< [8..8] Virtual Wire Ch. Index42 Changed                                   */
      __IOM uint32_t IDX43CHG   : 1;            /*!< [9..9] Virtual Wire Ch. Index43 Changed                                   */
      __IOM uint32_t IDX44CHG   : 1;            /*!< [10..10] Virtual Wire Ch. Index44 Changed                                 */
      __IOM uint32_t IDX47CHG   : 1;            /*!< [11..11] Virtual Wire Ch. Index47 Changed                                 */
      __IOM uint32_t IDX4ACHG   : 1;            /*!< [12..12] Virtual Wire Ch. Index4A Changed                                 */
      __IOM uint32_t IDX51CHG   : 1;            /*!< [13..13] Virtual Wire Ch. Index51 Changed                                 */
      __IOM uint32_t IDX61CHG   : 1;            /*!< [14..14] Virtual Wire Ch. Index61 Changed                                 */
      __IOM uint32_t CFGCHG     : 1;            /*!< [15..15] Virtual Wire Ch. Configuration Changed                           */
      __IOM uint32_t TXDONE     : 1;            /*!< [16..16] Virtual Wire Ch. Transaction Done                                */
      __IOM uint32_t RXIDXCLR   : 1;            /*!< [17..17] Virtual Wire Ch. Rx Index Clear                                  */
      __IOM uint32_t RXIDXCGG   : 1;            /*!< [18..18] Virtual Wire Ch. Rx Changed                                      */
      __IOM uint32_t TXEMPTY    : 1;            /*!< [19..19] Virtual Wire Ch. Tx FIFO Empty                                   */
            uint32_t            : 12;
    } EVSTS_b;
  } ;
  
  union {
    __IOM uint32_t EVCFG;                       /*!< (@ 0x00000044) VIRTUAL WIRE CH. CONFIGURATION REGISTER                    */
    
    struct {
      __IOM uint32_t CHEN       : 1;            /*!< [0..0] Virtual Wire Ch. Enable                                            */
      __IOM uint32_t CHRDY      : 1;            /*!< [1..1] Virtual Wire Ch. Ready                                             */
            uint32_t            : 6;
      __IM  uint32_t MAXSUP     : 6;            /*!< [13..8] Virtual Wire Ch. Maximum Count Supported                          */
            uint32_t            : 2;
      __IM  uint32_t MAXCNT     : 6;            /*!< [21..16] Virtual Wire Ch. Maximum Operating Count                         */
            uint32_t            : 10;
    } EVCFG_b;
  } ;
  
  union {
    __IOM uint32_t EVIDX2;                      /*!< (@ 0x00000048) VIRTUAL WIRE CH. RX INDEX 2 REGISTER                       */
    
    struct {
      __IM  uint32_t RXDAT      : 8;            /*!< [7..0] Virtual Wire Ch. Index2 Receive Data                               */
            uint32_t            : 24;
    } EVIDX2_b;
  } ;
  
  union {
    __IOM uint32_t EVIDX3;                      /*!< (@ 0x0000004C) VIRTUAL WIRE CH. RX INDEX 3 REGISTER                       */
    
    struct {
      __IM  uint32_t RXDAT      : 8;            /*!< [7..0] Virtual Wire Ch. Index3 Receive Data                               */
            uint32_t            : 24;
    } EVIDX3_b;
  } ;
  
  union {
    __IOM uint32_t EVIDX7;                      /*!< (@ 0x00000050) VIRTUAL WIRE CH. RX INDEX 7 REGISTER                       */
    
    struct {
      __IM  uint32_t RXDAT      : 8;            /*!< [7..0] Virtual Wire Ch. Index7 Receive Data                               */
            uint32_t            : 24;
    } EVIDX7_b;
  } ;
  
  union {
    __IOM uint32_t EVIDX41;                     /*!< (@ 0x00000054) VIRTUAL WIRE CH. RX INDEX 41 REGISTER                      */
    
    struct {
      __IM  uint32_t RXDAT      : 8;            /*!< [7..0] Virtual Wire Ch. Index41 Receive Data                              */
            uint32_t            : 24;
    } EVIDX41_b;
  } ;
  
  union {
    __IOM uint32_t EVIDX42;                     /*!< (@ 0x00000058) VIRTUAL WIRE CH. RX INDEX 42 REGISTER                      */
    
    struct {
      __IM  uint32_t RXDAT      : 8;            /*!< [7..0] Virtual Wire Ch. Index42 Receive Data                              */
            uint32_t            : 24;
    } EVIDX42_b;
  } ;
  
  union {
    __IOM uint32_t EVIDX43;                     /*!< (@ 0x0000005C) VIRTUAL WIRE CH. RX INDEX 43 REGISTER                      */
    
    struct {
      __IM  uint32_t RXDAT      : 8;            /*!< [7..0] Virtual Wire Ch. Index43 Receive Data                              */
            uint32_t            : 24;
    } EVIDX43_b;
  } ;
  
  union {
    __IOM uint32_t EVIDX44;                     /*!< (@ 0x00000060) VIRTUAL WIRE CH. RX INDEX 44 REGISTER                      */
    
    struct {
      __IM  uint32_t RXDAT      : 8;            /*!< [7..0] Virtual Wire Ch. Index44 Receive Data                              */
            uint32_t            : 24;
    } EVIDX44_b;
  } ;
  
  union {
    __IOM uint32_t EVIDX47;                     /*!< (@ 0x00000064) VIRTUAL WIRE CH. RX INDEX 47 REGISTER                      */
    
    struct {
      __IM  uint32_t RXDAT      : 8;            /*!< [7..0] Virtual Wire Ch. Index47 Receive Data                              */
            uint32_t            : 24;
    } EVIDX47_b;
  } ;
  
  union {
    __IOM uint32_t EVIDX4A;                     /*!< (@ 0x00000068) VIRTUAL WIRE CH. RX INDEX 4A REGISTER                      */
    
    struct {
      __IM  uint32_t RXDAT      : 8;            /*!< [7..0] Virtual Wire Ch. Index4A Receive Data                              */
            uint32_t            : 24;
    } EVIDX4A_b;
  } ;
  
  union {
    __IOM uint32_t EVTXDAT;                     /*!< (@ 0x0000006C) VIRTUAL WIRE CH. TX DATA REGISTER                          */
    
    struct {
      __IOM uint32_t TXDAT      : 8;            /*!< [7..0] Virtual Wire Ch. Transport Data                                    */
      __IOM uint32_t TXIDX      : 8;            /*!< [15..8] Virtual Wire Ch. Transport Index                                  */
            uint32_t            : 16;
    } EVTXDAT_b;
  } ;
  
  union {
    __IOM uint32_t EVPVIDX;                     /*!< (@ 0x00000070) VIRTUAL WIRE CH. PRIVILEGE INDEX REGISTER                  */
    
    struct {
      __IM  uint32_t PVIDX      : 8;            /*!< [7..0] Virtual Wire Ch. Transport Privilege Index                         */
            uint32_t            : 24;
    } EVPVIDX_b;
  } ;
  
  union {
    __IOM uint32_t EVRXINTEN;                   /*!< (@ 0x00000074) VIRTUAL WIRE CH. RX INTERRUPT ENABLE REGISTER              */
    
    struct {
      __IOM uint32_t CFGCHGEN   : 1;            /*!< [0..0] Virtual Wire Ch. Configuration Change Interrupt Enable             */
            uint32_t            : 1;
      __IOM uint32_t TXILGEN    : 1;            /*!< [2..2] Virtual Wire Ch. TX Illegal Interrupt Enable                       */
      __IOM uint32_t RXIDXEN    : 1;            /*!< [3..3] Virtual Wire Ch. Recevie Other Index Data Change Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t RXIDX2EN   : 1;            /*!< [4..4] Virtual Wire Ch. Recevie Index2 Data Change Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t RXIDX3EN   : 1;            /*!< [5..5] Virtual Wire Ch. Recevie Index3 Data Change Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t RXIDX7EN   : 1;            /*!< [6..6] Virtual Wire Ch. Recevie Index7 Data Change Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t RXIDX41EN  : 1;            /*!< [7..7] Virtual Wire Ch. Recevie Index41 Data Change Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t RXIDX42EN  : 1;            /*!< [8..8] Virtual Wire Ch. Recevie Index42 Data Change Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t RXIDX43EN  : 1;            /*!< [9..9] Virtual Wire Ch. Recevie Index43 Data Change Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t RXIDX44EN  : 1;            /*!< [10..10] Virtual Wire Ch. Recevie Index44 Data Change Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t RXIDX47EN  : 1;            /*!< [11..11] Virtual Wire Ch. Recevie Index47 Data Change Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t RXIDX4AEN  : 1;            /*!< [12..12] Virtual Wire Ch. Recevie Index4A Data Change Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t RXIDX51EN  : 1;            /*!< [13..13] Virtual Wire Ch. Recevie Index51 Data Change Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t RXIDX61EN  : 1;            /*!< [14..14] Virtual Wire Ch. Recevie Index61 Data Change Interrupt
                                                     Enable                                                                    */
            uint32_t            : 17;
    } EVRXINTEN_b;
  } ;
  
  union {
    __IOM uint32_t EVTXINTEN;                   /*!< (@ 0x00000078) VIRTUAL WIRE CH. TX INTERRUPT ENABLE REGISTER              */
    
    struct {
      __IOM uint32_t TXMPTEN    : 1;            /*!< [0..0] Virtual Wire Ch. Tx FIFO Empty Interrupt Enable                    */
            uint32_t            : 31;
    } EVTXINTEN_b;
  } ;
  
  union {
    __IOM uint32_t EOSTS;                       /*!< (@ 0x0000007C) OOB CH. STATUS REGISTER                                    */
    
    struct {
      __IM  uint32_t RXPND      : 1;            /*!< [0..0] OOB Ch. RX FIFO Pending                                            */
      __IOM uint32_t RXDONE     : 1;            /*!< [1..1] OOB Ch. Receive Done                                               */
      __IM  uint32_t TXPND      : 1;            /*!< [2..2] OOB Ch. TX FIFO Pending                                            */
      __IOM uint32_t TXDONE     : 1;            /*!< [3..3] OOB Ch. Transaction Done                                           */
      __IOM uint32_t CFGENCHG   : 1;            /*!< [4..4] OOB Ch. Configuration Channel Enable Changed                       */
            uint32_t            : 27;
    } EOSTS_b;
  } ;
  
  union {
    __IOM uint32_t EOCFG;                       /*!< (@ 0x00000080) OOB CH. CONFIGURATION REGISTER                             */
    
    struct {
      __IOM uint32_t CHEN       : 1;            /*!< [0..0] OOB Ch. Enable                                                     */
      __IOM uint32_t CHRDY      : 1;            /*!< [1..1] OOB Ch. Ready                                                      */
            uint32_t            : 2;
      __IM  uint32_t MXSZSUP    : 3;            /*!< [6..4] OOB Ch. Maximum Payload Size Supported                             */
            uint32_t            : 1;
      __IM  uint32_t MXSZSEL    : 3;            /*!< [10..8] OOB Ch. Maximum Payload Size Selected                             */
            uint32_t            : 21;
    } EOCFG_b;
  } ;
  
  union {
    __IOM uint32_t EORXINTEN;                   /*!< (@ 0x00000084) OOB CH. RX INTERRUPT ENABLE REGISTER                       */
    
    struct {
      __IOM uint32_t CHENCHG    : 1;            /*!< [0..0] OOB Ch. Configuration Channel Enable Changed Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t RXEN       : 1;            /*!< [1..1] OOB Ch. Rx Interrupt Enable                                        */
            uint32_t            : 30;
    } EORXINTEN_b;
  } ;
  __IOM uint32_t  EORXBUF;                      /*!< (@ 0x00000088) OOB CH. RX BUFFER REGISTER                                 */
  
  union {
    __IOM uint32_t EORXLEN;                     /*!< (@ 0x0000008C) OOB CH. RX LENGTH REGISTER                                 */
    
    struct {
      __IOM uint32_t LENGTH     : 12;           /*!< [11..0] OOB Ch. Rx Length                                                 */
            uint32_t            : 20;
    } EORXLEN_b;
  } ;
  __IOM uint32_t  EOTXBUF;                      /*!< (@ 0x00000090) OOB CH. TX BUFFER REGISTER                                 */
  
  union {
    __IOM uint32_t EOTXLEN;                     /*!< (@ 0x00000094) OOB CH. TX LENGTH REGISTER                                 */
    
    struct {
      __IOM uint32_t LENGTH     : 8;            /*!< [7..0] OOB Ch. Tx Length                                                  */
            uint32_t            : 24;
    } EOTXLEN_b;
  } ;
  
  union {
    __IOM uint32_t EOTXCTRL;                    /*!< (@ 0x00000098) OOB CH. TX CONTROL REGISTER                                */
    
    struct {
      __IOM uint32_t TXSTR      : 1;            /*!< [0..0] Kick OOB Ch. Start to Transaction                                  */
            uint32_t            : 31;
    } EOTXCTRL_b;
  } ;
  
  union {
    __IOM uint32_t EOTXINTEN;                   /*!< (@ 0x0000009C) OOB CH. TX INTERRUPT ENABLE REGISTER                       */
    
    struct {
      __IOM uint32_t TXEN       : 1;            /*!< [0..0] OOB Ch. Tx Interrupt Enable                                        */
            uint32_t            : 31;
    } EOTXINTEN_b;
  } ;
  
  union {
    __IOM uint32_t EFSTS;                       /*!< (@ 0x000000A0) FLASH CH. STATUS REGISTER                                  */
    
    struct {
      __IOM uint32_t MAFTXDN    : 1;            /*!< [0..0] Flash Ch. MAF Transfer Done                                        */
      __IOM uint32_t MAFREOVR   : 1;            /*!< [1..1] Flash Ch. MAF Overrun                                              */
      __IOM uint32_t MAFREUDR   : 1;            /*!< [2..2] Flash Ch. MAF Underrun                                             */
      __IOM uint32_t SAFDONE    : 1;            /*!< [3..3] Flash Ch. SAF Transfer Done                                        */
      __IOM uint32_t SAFRW      : 1;            /*!< [4..4] Flash Ch. SAF Transfer Type                                        */
      __IOM uint32_t SAFERS     : 1;            /*!< [5..5] Flash Ch. SAF Transfer Type Erase                                  */
      __IOM uint32_t CHENCHG    : 1;            /*!< [6..6] Flash Ch. Configuration Channel Enable Changed                     */
      __IOM uint32_t OP2        : 1;            /*!< [7..7] Flash Ch. Receive RPMC OP2 Command                                 */
            uint32_t            : 24;
    } EFSTS_b;
  } ;
  
  union {
    __IOM uint32_t EFCONF;                      /*!< (@ 0x000000A4) FLASH CH. CONFIGURATION REGISTER                           */
    
    struct {
      __IOM uint32_t CHEN       : 1;            /*!< [0..0] Flash Access Ch. Enable                                            */
      __IOM uint32_t CHRDY      : 1;            /*!< [1..1] Flash Access Ch. Ready                                             */
      __IOM uint32_t ERBLKSZ    : 3;            /*!< [4..2] Flash Block Erase Size                                             */
      __IOM uint32_t MXPLSUP    : 3;            /*!< [7..5] Flash Access Channel Maximum Payload Size Supported                */
      __IOM uint32_t MXPLSEL    : 3;            /*!< [10..8] Flash Access Channel Maximum Payload Size Selected                */
      __IOM uint32_t SHAREMD    : 1;            /*!< [11..11] Flash Sharing Mode                                               */
      __IOM uint32_t MXRDSZ     : 3;            /*!< [14..12] Flash Access Channel Maximum Read Request Size                   */
            uint32_t            : 1;
      __IOM uint32_t SHARECAPSP : 2;            /*!< [17..16] Flash Sharing Capability Supported                               */
            uint32_t            : 2;
      __IOM uint32_t RPMCCNT1   : 4;            /*!< [23..20] RPMC Counter on the 1st RPMC Flash device                        */
      __IOM uint32_t RPMCOP1    : 8;            /*!< [31..24] RPMC OP1 Opcode on the 1st RPMC Flash device                     */
    } EFCONF_b;
  } ;
  __IOM uint32_t  EMADR;                        /*!< (@ 0x000000A8) FLASH CH. MAF FLASH ADDRESS REGISTER                       */
  
  union {
    __IOM uint32_t EMTRLEN;                     /*!< (@ 0x000000AC) FLASH CH. MAF TRANSFER LENGTH REGISTER                     */
    
    struct {
      __IOM uint32_t TRLEN      : 12;           /*!< [11..0] Flash Ch. MAF Tx Length                                           */
            uint32_t            : 20;
    } EMTRLEN_b;
  } ;
  __IOM uint32_t  EMBUF;                        /*!< (@ 0x000000B0) FLASH CH. MAF BUFFER REGISTER                              */
  
  union {
    __IOM uint32_t EMCTRL;                      /*!< (@ 0x000000B4) FLASH CH. MAF CONTROL REGISTER                             */
    
    struct {
      __IOM uint32_t START      : 1;            /*!< [0..0] Kick Flash Ch. MAF Start to Transaction                            */
      __IOM uint32_t MDSEL      : 2;            /*!< [2..1] Flash Ch. MAF TR Mode selection                                    */
            uint32_t            : 29;
    } EMCTRL_b;
  } ;
  
  union {
    __IOM uint32_t EMINTEN;                     /*!< (@ 0x000000B8) FLASH CH. MAF INTERRUPT ENABLE REGISTER                    */
    
    struct {
      __IOM uint32_t CHENCHG    : 1;            /*!< [0..0] Flash Ch. Configuration Channel Enable Changed Interrupt
                                                     Enable                                                                    */
      __IOM uint32_t TRDONEEN   : 1;            /*!< [1..1] Flash Ch. MAF Transfer Done Interrupt Enable                       */
            uint32_t            : 30;
    } EMINTEN_b;
  } ;
  __IOM uint32_t  ESBUF;                        /*!< (@ 0x000000BC) FLASH CH. SAF BUFFER REGISTER #0                           */
  
  union {
    __IOM uint32_t ESINTEN;                     /*!< (@ 0x000000C0) FLASH CH. SAF INTERRUPT ENABLE REGISTER                    */
    
    struct {
      __IOM uint32_t TRDONEEN   : 1;            /*!< [0..0] Flash Ch. SAF Transfer Done Interrupt Enable                       */
      __IOM uint32_t ERASEEN    : 1;            /*!< [1..1] Flash Ch. SAF Erase Interrupt Enable                               */
      __IOM uint32_t RPMCEN     : 1;            /*!< [2..2] Flash Ch. SAF RPMC OP2 Interrupt Enable                            */
            uint32_t            : 29;
    } ESINTEN_b;
  } ;
  __IOM uint32_t  ESRXADR;                      /*!< (@ 0x000000C4) FLASH CH. SAF RECEIVE ADDRESS REGISTER                     */
  
  union {
    __IOM uint32_t ESRXLEN;                     /*!< (@ 0x000000C8) FLASH CH. SAF RECEIVE LENGTH REGISTER                      */
    
    struct {
      __IM  uint32_t LENGTH     : 12;           /*!< [11..0] Peripheral Ch. RX Command Length                                  */
            uint32_t            : 20;
    } ESRXLEN_b;
  } ;
  
  union {
    __IOM uint32_t ESPICFG;                     /*!< (@ 0x000000CC) GENERAL CONFIGURATION REGISTER                             */
    
    struct {
      __IOM uint32_t CHSUP      : 8;            /*!< [7..0] Channel Supported                                                  */
            uint32_t            : 4;
      __IOM uint32_t MXWAITALW  : 4;            /*!< [15..12] Maximum WAIT STATE Allowed                                       */
      __IOM uint32_t MXFREQSUP  : 3;            /*!< [18..16] Maximum Frequency Supported                                      */
      __IOM uint32_t ODALRSUP   : 1;            /*!< [19..19] Open Drain Alert Supported                                       */
      __IOM uint32_t OPFREQ     : 3;            /*!< [22..20] Operating Frequency                                              */
      __IOM uint32_t ODALRSEL   : 1;            /*!< [23..23] Open Drain Alert Select                                          */
      __IOM uint32_t IOSUP      : 2;            /*!< [25..24] I/O Mode Support                                                 */
      __IOM uint32_t IOSEL      : 2;            /*!< [27..26] I/O Mode Select                                                  */
      __IOM uint32_t ALRMODE    : 1;            /*!< [28..28] Alert Mode                                                       */
      __IOM uint32_t RTCINBMC   : 1;            /*!< [29..29] RTC-Integrated-BMC                                               */
      __IOM uint32_t RSPMDFEN   : 1;            /*!< [30..30] Response Modifier Enable                                         */
      __IOM uint32_t CRCCHKEN   : 1;            /*!< [31..31] CRC Checking Enable                                              */
    } ESPICFG_b;
  } ;
  
  union {
    __IOM uint32_t ERSTCFG;                     /*!< (@ 0x000000D0) RESET PIN INTERRUPT CONFIGURATION REGISTER                 */
    
    struct {
      __IOM uint32_t RSTMONEN   : 1;            /*!< [0..0] eSPI Reset Pin Monitor Enable                                      */
      __IOM uint32_t RSTPOL     : 1;            /*!< [1..1] eSPI Reset Pin Polarity                                            */
      __IOM uint32_t RSTINTEN   : 1;            /*!< [2..2] eSPI Reset Pin Interrupt Enable                                    */
      __IOM uint32_t RSTSTS     : 1;            /*!< [3..3] eSPI Reset Pin Status                                              */
      __IOM uint32_t CSINTEN    : 1;            /*!< [4..4] eSPI CS Pin Interrupt Enable                                       */
      __IOM uint32_t CSSTS      : 1;            /*!< [5..5] eSPI CS Pin Status                                                 */
            uint32_t            : 26;
    } ERSTCFG_b;
  } ;
  
  union {
    __IOM uint32_t EVIDX51;                     /*!< (@ 0x000000D4) VIRTUAL WIRE CH. RX INDEX 51 REGISTER                      */
    
    struct {
      __IM  uint32_t RXDAT      : 8;            /*!< [7..0] Virtual Wire Ch. Index51 Receive Data                              */
            uint32_t            : 24;
    } EVIDX51_b;
  } ;
  
  union {
    __IOM uint32_t EVIDX61;                     /*!< (@ 0x000000D8) VIRTUAL WIRE CH. RX INDEX 61 REGISTER                      */
    
    struct {
      __IM  uint32_t RXDAT      : 8;            /*!< [7..0] Virtual Wire Ch. Index61 Receive Data                              */
            uint32_t            : 24;
    } EVIDX61_b;
  } ;
  __IOM uint32_t  ESRXC;                        /*!< (@ 0x000000DC) FLASH CH. SAF RECEIVE CYCLE TYPE REGISTER                  */
  
  union {
    __IOM uint32_t EFCFG2;                      /*!< (@ 0x000000E0) FLASH CH. SAF CONFIG #2 REGISTER                           */
    
    struct {
      __IOM uint32_t MXRDSZ     : 3;            /*!< [2..0] Target Maximum Read Request Size Supported                         */
            uint32_t            : 5;
      __IOM uint32_t ERBLKSZ    : 8;            /*!< [15..8] Target Flash Erase Block Size                                     */
      __IOM uint32_t RPMCSP     : 6;            /*!< [21..16] Target RPMC Supported                                            */
      __IOM uint32_t NUMRPMC    : 2;            /*!< [23..22] Number of Target Attached Flash RPMC flash devices               */
            uint32_t            : 8;
    } EFCFG2_b;
  } ;
  
  union {
    __IOM uint32_t EFCFG3;                      /*!< (@ 0x000000E4) FLASH CH. SAF CONFIG #3 REGISTER                           */
    
    struct {
            uint32_t            : 20;
      __IOM uint32_t RPMCCNT2   : 4;            /*!< [23..20] RPMC Counter on the 2nd RPMC Flash device                        */
      __IOM uint32_t RPMCOP2    : 8;            /*!< [31..24] RPMC OP1 Opcode on the 2nd RPMC Flash device                     */
    } EFCFG3_b;
  } ;
  
  union {
    __IOM uint32_t EFCFG4;                      /*!< (@ 0x000000E8) FLASH CH. SAF CONFIG #4 REGISTER                           */
    
    struct {
            uint32_t            : 4;
      __IOM uint32_t RPMCCNT3   : 4;            /*!< [7..4] RPMC Counter on the 3rd RPMC Flash device                          */
      __IOM uint32_t RPMCOP3    : 8;            /*!< [15..8] RPMC OP1 Opcode on the 3rd RPMC Flash device                      */
            uint32_t            : 4;
      __IOM uint32_t RPMCCNT4   : 4;            /*!< [23..20] RPMC Counter on the 4th RPMC Flash device                        */
      __IOM uint32_t RPMCOP4    : 8;            /*!< [31..24] RPMC OP1 Opcode on the 4th RPMC Flash device                     */
    } EFCFG4_b;
  } ;
  __IM  uint32_t  RESERVED[3];
  
  union {
    __IOM uint32_t ELCTRL;                      /*!< (@ 0x000000F8) PERIPHERAL CH. LTR CONTROL REGISTER                        */
    
    struct {
      __IOM uint32_t TXSTR      : 1;            /*!< [0..0] Kick LTR Start to Transaction                                      */
            uint32_t            : 31;
    } ELCTRL_b;
  } ;
  __IOM uint32_t  ESBUF1;                       /*!< (@ 0x000000FC) FLASH CH. SAF RPMC BUFFER1 REGISTER                        */
  
  union {
    __IOM uint32_t ESPRG0;                      /*!< (@ 0x00000100) FLASH CH. SAF PROTECTION GROUP #0 REGISTER                 */
    
    struct {
      __IOM uint32_t TAG0GRP    : 3;            /*!< [2..0] Tag #0 Group                                                       */
            uint32_t            : 1;
      __IOM uint32_t TAG1GRP    : 3;            /*!< [6..4] Tag #1 Group                                                       */
            uint32_t            : 1;
      __IOM uint32_t TAG2GRP    : 3;            /*!< [10..8] Tag #2 Group                                                      */
            uint32_t            : 1;
      __IOM uint32_t TAG3GRP    : 3;            /*!< [14..12] Tag #3 Group                                                     */
            uint32_t            : 1;
      __IOM uint32_t TAG4GRP    : 3;            /*!< [18..16] Tag #4 Group                                                     */
            uint32_t            : 1;
      __IOM uint32_t TAG5GRP    : 3;            /*!< [22..20] Tag #5 Group                                                     */
            uint32_t            : 1;
      __IOM uint32_t TAG6GRP    : 3;            /*!< [26..24] Tag #6 Group                                                     */
            uint32_t            : 1;
      __IOM uint32_t TAG7GRP    : 3;            /*!< [30..28] Tag #7 Group                                                     */
            uint32_t            : 1;
    } ESPRG0_b;
  } ;
  
  union {
    __IOM uint32_t ESPRG1;                      /*!< (@ 0x00000104) FLASH CH. SAF PROTECTION GROUP #1 REGISTER                 */
    
    struct {
      __IOM uint32_t TAG8GRP    : 3;            /*!< [2..0] Tag #8 Group                                                       */
            uint32_t            : 1;
      __IOM uint32_t TAG9GRP    : 3;            /*!< [6..4] Tag #9 Group                                                       */
            uint32_t            : 1;
      __IOM uint32_t TAGAGRP    : 3;            /*!< [10..8] Tag #A Group                                                      */
            uint32_t            : 1;
      __IOM uint32_t TAGBGRP    : 3;            /*!< [14..12] Tag #B Group                                                     */
            uint32_t            : 1;
      __IOM uint32_t TAGCGRP    : 3;            /*!< [18..16] Tag #C Group                                                     */
            uint32_t            : 1;
      __IOM uint32_t TAGDGRP    : 3;            /*!< [22..20] Tag #D Group                                                     */
            uint32_t            : 1;
      __IOM uint32_t TAGEGRP    : 3;            /*!< [26..24] Tag #E Group                                                     */
            uint32_t            : 1;
      __IOM uint32_t TAGFGRP    : 3;            /*!< [30..28] Tag #F Group                                                     */
            uint32_t            : 1;
    } ESPRG1_b;
  } ;
  
  union {
    __IOM uint32_t ESPDRT;                      /*!< (@ 0x00000108) FLASH CH. SAF PROTECTION DIRTY REGISTER                    */
    
    struct {
      __IOM uint32_t TAG0DRT    : 1;            /*!< [0..0] Tag #0 Dirty                                                       */
      __IOM uint32_t TAG1DRT    : 1;            /*!< [1..1] Tag #1 Dirty                                                       */
      __IOM uint32_t TAG2DRT    : 1;            /*!< [2..2] Tag #2 Dirty                                                       */
      __IOM uint32_t TAG3DRT    : 1;            /*!< [3..3] Tag #3 Dirty                                                       */
      __IOM uint32_t TAG4DRT    : 1;            /*!< [4..4] Tag #4 Dirty                                                       */
      __IOM uint32_t TAG5DRT    : 1;            /*!< [5..5] Tag #5 Dirty                                                       */
      __IOM uint32_t TAG6DRT    : 1;            /*!< [6..6] Tag #6 Dirty                                                       */
      __IOM uint32_t TAG7DRT    : 1;            /*!< [7..7] Tag #7 Dirty                                                       */
      __IOM uint32_t TAG8DRT    : 1;            /*!< [8..8] Tag #8 Dirty                                                       */
      __IOM uint32_t TAG9DRT    : 1;            /*!< [9..9] Tag #9 Dirty                                                       */
      __IOM uint32_t TAGADRT    : 1;            /*!< [10..10] Tag #A Dirty                                                     */
      __IOM uint32_t TAGBDRT    : 1;            /*!< [11..11] Tag #B Dirty                                                     */
      __IOM uint32_t TAGCDRT    : 1;            /*!< [12..12] Tag #C Dirty                                                     */
      __IOM uint32_t TAGDDRT    : 1;            /*!< [13..13] Tag #D Dirty                                                     */
      __IOM uint32_t TAGEDRT    : 1;            /*!< [14..14] Tag #E Dirty                                                     */
      __IOM uint32_t TAGFDRT    : 1;            /*!< [15..15] Tag #F Dirty                                                     */
            uint32_t            : 16;
    } ESPDRT_b;
  } ;
  
  union {
    __IOM uint32_t ESP0STR;                     /*!< (@ 0x0000010C) FLASH CH. SAF REGION #0 START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESP0STR_b;
  } ;
  
  union {
    __IOM uint32_t ESP1STR;                     /*!< (@ 0x00000110) FLASH CH. SAF REGION #1 START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESP1STR_b;
  } ;
  
  union {
    __IOM uint32_t ESP2STR;                     /*!< (@ 0x00000114) FLASH CH. SAF REGION #2 START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESP2STR_b;
  } ;
  
  union {
    __IOM uint32_t ESP3STR;                     /*!< (@ 0x00000118) FLASH CH. SAF REGION #3 START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESP3STR_b;
  } ;
  
  union {
    __IOM uint32_t ESP4STR;                     /*!< (@ 0x0000011C) FLASH CH. SAF REGION #4 START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESP4STR_b;
  } ;
  
  union {
    __IOM uint32_t ESP5STR;                     /*!< (@ 0x00000120) FLASH CH. SAF REGION #5 START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESP5STR_b;
  } ;
  
  union {
    __IOM uint32_t ESP6STR;                     /*!< (@ 0x00000124) FLASH CH. SAF REGION #6 START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESP6STR_b;
  } ;
  
  union {
    __IOM uint32_t ESP7STR;                     /*!< (@ 0x00000128) FLASH CH. SAF REGION #7 START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESP7STR_b;
  } ;
  
  union {
    __IOM uint32_t ESP8STR;                     /*!< (@ 0x0000012C) FLASH CH. SAF REGION #8 START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESP8STR_b;
  } ;
  
  union {
    __IOM uint32_t ESP9STR;                     /*!< (@ 0x00000130) FLASH CH. SAF REGION #9 START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESP9STR_b;
  } ;
  
  union {
    __IOM uint32_t ESPASTR;                     /*!< (@ 0x00000134) FLASH CH. SAF REGION #A START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESPASTR_b;
  } ;
  
  union {
    __IOM uint32_t ESPBSTR;                     /*!< (@ 0x00000138) FLASH CH. SAF REGION #B START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESPBSTR_b;
  } ;
  
  union {
    __IOM uint32_t ESPCSTR;                     /*!< (@ 0x0000013C) FLASH CH. SAF REGION #C START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESPCSTR_b;
  } ;
  
  union {
    __IOM uint32_t ESPDSTR;                     /*!< (@ 0x00000140) FLASH CH. SAF REGION #D START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESPDSTR_b;
  } ;
  
  union {
    __IOM uint32_t ESPESTR;                     /*!< (@ 0x00000144) FLASH CH. SAF REGION #E START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESPESTR_b;
  } ;
  
  union {
    __IOM uint32_t ESPFSTR;                     /*!< (@ 0x00000148) FLASH CH. SAF REGION #F START REGISTER                     */
    
    struct {
      __IOM uint32_t ADDR       : 20;           /*!< [19..0] Region Start Address                                              */
            uint32_t            : 12;
    } ESPFSTR_b;
  } ;
  
  union {
    __IOM uint32_t ESP0LEN;                     /*!< (@ 0x0000014C) FLASH CH. SAF REGION #0 LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESP0LEN_b;
  } ;
  
  union {
    __IOM uint32_t ESP1LEN;                     /*!< (@ 0x00000150) FLASH CH. SAF REGION #1 LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESP1LEN_b;
  } ;
  
  union {
    __IOM uint32_t ESP2LEN;                     /*!< (@ 0x00000154) FLASH CH. SAF REGION #2 LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESP2LEN_b;
  } ;
  
  union {
    __IOM uint32_t ESP3LEN;                     /*!< (@ 0x00000158) FLASH CH. SAF REGION #3 LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESP3LEN_b;
  } ;
  
  union {
    __IOM uint32_t ESP4LEN;                     /*!< (@ 0x0000015C) FLASH CH. SAF REGION #4 LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESP4LEN_b;
  } ;
  
  union {
    __IOM uint32_t ESP5LEN;                     /*!< (@ 0x00000160) FLASH CH. SAF REGION #5 LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESP5LEN_b;
  } ;
  
  union {
    __IOM uint32_t ESP6LEN;                     /*!< (@ 0x00000164) FLASH CH. SAF REGION #6 LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESP6LEN_b;
  } ;
  
  union {
    __IOM uint32_t ESP7LEN;                     /*!< (@ 0x00000168) FLASH CH. SAF REGION #7 LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESP7LEN_b;
  } ;
  
  union {
    __IOM uint32_t ESP8LEN;                     /*!< (@ 0x0000016C) FLASH CH. SAF REGION #8 LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESP8LEN_b;
  } ;
  
  union {
    __IOM uint32_t ESP9LEN;                     /*!< (@ 0x00000170) FLASH CH. SAF REGION #9 LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESP9LEN_b;
  } ;
  
  union {
    __IOM uint32_t ESPALEN;                     /*!< (@ 0x00000174) FLASH CH. SAF REGION #A LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESPALEN_b;
  } ;
  
  union {
    __IOM uint32_t ESPBLEN;                     /*!< (@ 0x00000178) FLASH CH. SAF REGION #B LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESPBLEN_b;
  } ;
  
  union {
    __IOM uint32_t ESPCLEN;                     /*!< (@ 0x0000017C) FLASH CH. SAF REGION #C LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESPCLEN_b;
  } ;
  
  union {
    __IOM uint32_t ESPDLEN;                     /*!< (@ 0x00000180) FLASH CH. SAF REGION #D LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESPDLEN_b;
  } ;
  
  union {
    __IOM uint32_t ESPELEN;                     /*!< (@ 0x00000184) FLASH CH. SAF REGION #E LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESPELEN_b;
  } ;
  
  union {
    __IOM uint32_t ESPFLEN;                     /*!< (@ 0x00000188) FLASH CH. SAF REGION #F LENGTH REGISTER                    */
    
    struct {
      __IOM uint32_t LEN        : 20;           /*!< [19..0] Region Length                                                     */
            uint32_t            : 12;
    } ESPFLEN_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRG0;                     /*!< (@ 0x0000018C) FLASH CH. SAF WRITE PROTECTION REGION #0 REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRG0_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRG0;                     /*!< (@ 0x00000190) FLASH CH. SAF READ PROTECTION REGION #0 REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRG0_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRG1;                     /*!< (@ 0x00000194) FLASH CH. SAF WRITE PROTECTION REGION #1 REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRG1_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRG1;                     /*!< (@ 0x00000198) FLASH CH. SAF READ PROTECTION REGION #1 REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRG1_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRG2;                     /*!< (@ 0x0000019C) FLASH CH. SAF WRITE PROTECTION REGION #2 REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRG2_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRG2;                     /*!< (@ 0x000001A0) FLASH CH. SAF READ PROTECTION REGION #2 REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRG2_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRG3;                     /*!< (@ 0x000001A4) FLASH CH. SAF WRITE PROTECTION REGION #3 REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRG3_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRG3;                     /*!< (@ 0x000001A8) FLASH CH. SAF READ PROTECTION REGION #3 REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRG3_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRG4;                     /*!< (@ 0x000001AC) FLASH CH. SAF WRITE PROTECTION REGION #4 REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRG4_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRG4;                     /*!< (@ 0x000001B0) FLASH CH. SAF READ PROTECTION REGION #4 REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRG4_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRG5;                     /*!< (@ 0x000001B4) FLASH CH. SAF WRITE PROTECTION REGION #5 REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRG5_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRG5;                     /*!< (@ 0x000001B8) FLASH CH. SAF READ PROTECTION REGION #5 REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRG5_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRG6;                     /*!< (@ 0x000001BC) FLASH CH. SAF WRITE PROTECTION REGION #6 REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRG6_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRG6;                     /*!< (@ 0x000001C0) FLASH CH. SAF READ PROTECTION REGION #6 REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRG6_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRG7;                     /*!< (@ 0x000001C4) FLASH CH. SAF WRITE PROTECTION REGION #7 REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRG7_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRG7;                     /*!< (@ 0x000001C8) FLASH CH. SAF READ PROTECTION REGION #7 REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRG7_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRG8;                     /*!< (@ 0x000001CC) FLASH CH. SAF WRITE PROTECTION REGION #8 REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRG8_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRG8;                     /*!< (@ 0x000001D0) FLASH CH. SAF READ PROTECTION REGION #8 REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRG8_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRG9;                     /*!< (@ 0x000001D4) FLASH CH. SAF WRITE PROTECTION REGION #9 REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRG9_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRG9;                     /*!< (@ 0x000001D8) FLASH CH. SAF READ PROTECTION REGION #9 REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRG9_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRGA;                     /*!< (@ 0x000001DC) FLASH CH. SAF WRITE PROTECTION REGION #A REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRGA_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRGA;                     /*!< (@ 0x000001E0) FLASH CH. SAF READ PROTECTION REGION #A REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRGA_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRGB;                     /*!< (@ 0x000001E4) FLASH CH. SAF WRITE PROTECTION REGION #B REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRGB_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRGB;                     /*!< (@ 0x000001E8) FLASH CH. SAF READ PROTECTION REGION #B REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRGB_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRGC;                     /*!< (@ 0x000001EC) FLASH CH. SAF WRITE PROTECTION REGION #C REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRGC_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRGC;                     /*!< (@ 0x000001F0) FLASH CH. SAF READ PROTECTION REGION #C REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRGC_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRGD;                     /*!< (@ 0x000001F4) FLASH CH. SAF WRITE PROTECTION REGION #D REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRGD_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRGD;                     /*!< (@ 0x000001F8) FLASH CH. SAF READ PROTECTION REGION #D REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRGD_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRGE;                     /*!< (@ 0x000001FC) FLASH CH. SAF WRITE PROTECTION REGION #E REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRGE_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRGE;                     /*!< (@ 0x00000200) FLASH CH. SAF READ PROTECTION REGION #E REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRGE_b;
  } ;
  
  union {
    __IOM uint32_t ESWPRGF;                     /*!< (@ 0x00000204) FLASH CH. SAF WRITE PROTECTION REGION #F REGISTER          */
    
    struct {
      __IOM uint32_t G0WREN     : 1;            /*!< [0..0] Group #0 Write Enable                                              */
      __IOM uint32_t G1WREN     : 1;            /*!< [1..1] Group #1 Write Enable                                              */
      __IOM uint32_t G2WREN     : 1;            /*!< [2..2] Group #2 Write Enable                                              */
      __IOM uint32_t G3WREN     : 1;            /*!< [3..3] Group #3 Write Enable                                              */
      __IOM uint32_t G4WREN     : 1;            /*!< [4..4] Group #4 Write Enable                                              */
      __IOM uint32_t G5WREN     : 1;            /*!< [5..5] Group #5 Write Enable                                              */
      __IOM uint32_t G6WREN     : 1;            /*!< [6..6] Group #6 Write Enable                                              */
      __IOM uint32_t G7WREN     : 1;            /*!< [7..7] Group #7 Write Enable                                              */
            uint32_t            : 24;
    } ESWPRGF_b;
  } ;
  
  union {
    __IOM uint32_t ESRPRGF;                     /*!< (@ 0x00000208) FLASH CH. SAF READ PROTECTION REGION #F REGISTER           */
    
    struct {
      __IOM uint32_t G0RDEN     : 1;            /*!< [0..0] Group #0 Read Enable                                               */
      __IOM uint32_t G1RDEN     : 1;            /*!< [1..1] Group #1 Read Enable                                               */
      __IOM uint32_t G2RDEN     : 1;            /*!< [2..2] Group #2 Read Enable                                               */
      __IOM uint32_t G3RDEN     : 1;            /*!< [3..3] Group #3 Read Enable                                               */
      __IOM uint32_t G4RDEN     : 1;            /*!< [4..4] Group #4 Read Enable                                               */
      __IOM uint32_t G5RDEN     : 1;            /*!< [5..5] Group #5 Read Enable                                               */
      __IOM uint32_t G6RDEN     : 1;            /*!< [6..6] Group #6 Read Enable                                               */
      __IOM uint32_t G7RDEN     : 1;            /*!< [7..7] Group #7 Read Enable                                               */
            uint32_t            : 24;
    } ESRPRGF_b;
  } ;
  
  union {
    __IOM uint32_t ESPREN;                      /*!< (@ 0x0000020C) FLASH CH. SAF PROTECTION ENABLE REGISTER                   */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Flash Ch. SAF Protection Enable                                    */
            uint32_t            : 31;
    } ESPREN_b;
  } ;
  
  union {
    __IOM uint32_t ESPSTS;                      /*!< (@ 0x00000210) FLASH CH. SAF PROTECTION STATUS REGISTER                   */
    
    struct {
      __IOM uint32_t OVSIZE     : 1;            /*!< [0..0] Host Access Over Size                                              */
      __IOM uint32_t HIT        : 1;            /*!< [1..1] Host Hit Protection Region                                         */
      __IOM uint32_t CRS4K      : 1;            /*!< [2..2] Host Cross 4K boundary                                             */
            uint32_t            : 29;
    } ESPSTS_b;
  } ;
  __IOM uint32_t  ESFLSZ;                       /*!< (@ 0x00000214) FLASH CH. SAF FLASH SIZE REGISTER                          */
  
  union {
    __IOM uint32_t ESPINTEN;                    /*!< (@ 0x00000218) FLASH CH. SAF PROTECTION INTERRUPT ENABLE REGISTER         */
    
    struct {
      __IOM uint32_t OVSIZEEN   : 1;            /*!< [0..0] Host Access Over Size Interrupt Enable                             */
      __IOM uint32_t HITEN      : 1;            /*!< [1..1] Host Hit Protection Region Interrupt Enable                        */
      __IOM uint32_t CRS4KEN    : 1;            /*!< [2..2] Host Cross 4K Boundary Interrupt Enable                            */
            uint32_t            : 29;
    } ESPINTEN_b;
  } ;
  
  union {
    __IOM uint32_t IOSHORTSTS;                  /*!< (@ 0x0000021C) IO SHORT STATUS REGISTER                                   */
    
    struct {
      __IOM uint32_t BYTES      : 2;            /*!< [1..0] Number of IO Short Bytes                                           */
      __IOM uint32_t TYPE       : 1;            /*!< [2..2] Type of IO Short                                                   */
      __IOM uint32_t ACCEPT     : 1;            /*!< [3..3] IO Short Read/Write Accpeted                                       */
      __IOM uint32_t RSPSTART   : 1;            /*!< [4..4] IO Short Read/Write Response Start                                 */
      __IOM uint32_t RSPRSLT    : 1;            /*!< [5..5] IO Short Read/Write Response Result                                */
            uint32_t            : 26;
    } IOSHORTSTS_b;
  } ;
  
  union {
    __IOM uint32_t IOSHORTRDADDR;               /*!< (@ 0x00000220) IO SHORT READ ADDRESS REGISTER                             */
    
    struct {
      __IOM uint32_t ADDR       : 16;           /*!< [15..0] Channel Address                                                   */
            uint32_t            : 16;
    } IOSHORTRDADDR_b;
  } ;
  __IOM uint32_t  IOSHORTRDDATA;                /*!< (@ 0x00000224) IO SHORT DATA REGISTER                                     */
  
  union {
    __IOM uint32_t LDNCFG;                      /*!< (@ 0x00000228) LDN INDEX/DATA CONFIGURATION REGISTER                      */
    
    struct {
      __IOM uint32_t IDX        : 16;           /*!< [15..0] LDN Index Compare Address                                         */
            uint32_t            : 16;
    } LDNCFG_b;
  } ;
  __IOM uint32_t  ID0;                          /*!< (@ 0x0000022C) CHIP ID #0 REGISTER                                        */
  __IOM uint32_t  ID1;                          /*!< (@ 0x00000230) CHIP ID #1 REGISTER                                        */
  __IOM uint32_t  VER;                          /*!< (@ 0x00000234) CHIP VERSION REGISTER                                      */
  
  union {
    __IM  uint32_t EVRXDATA;                    /*!< (@ 0x00000238) VW RX INDEX AND DATA REGISTER                              */
    
    struct {
      __IM  uint32_t DATA       : 8;            /*!< [7..0] VW RX Data                                                         */
      __IM  uint32_t INDEX      : 8;            /*!< [15..8] VW RX Index                                                       */
            uint32_t            : 16;
    } EVRXDATA_b;
  } ;
  
  union {
    __IOM uint32_t EMI0CTRL;                    /*!< (@ 0x0000023C) LDN EMI0 CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t ENABLE     : 1;            /*!< [0..0] EMI0 Enable                                                        */
            uint32_t            : 7;
      __IOM uint32_t SIZE       : 4;            /*!< [11..8] EMI Size Selecttion                                               */
            uint32_t            : 20;
    } EMI0CTRL_b;
  } ;
  
  union {
    __IOM uint32_t EMI1CTRL;                    /*!< (@ 0x00000240) LDN EMI1 CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t ENABLE     : 1;            /*!< [0..0] EMI0 Enable                                                        */
            uint32_t            : 7;
      __IOM uint32_t SIZE       : 4;            /*!< [11..8] EMI Size Selecttion                                               */
            uint32_t            : 20;
    } EMI1CTRL_b;
  } ;
  
  union {
    __IOM uint32_t EMI2CTRL;                    /*!< (@ 0x00000244) LDN EMI2 CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t ENABLE     : 1;            /*!< [0..0] EMI0 Enable                                                        */
            uint32_t            : 7;
      __IOM uint32_t SIZE       : 4;            /*!< [11..8] EMI Size Selecttion                                               */
            uint32_t            : 20;
    } EMI2CTRL_b;
  } ;
  
  union {
    __IOM uint32_t EMI3CTRL;                    /*!< (@ 0x00000248) LDN EMI3 CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t ENABLE     : 1;            /*!< [0..0] EMI0 Enable                                                        */
            uint32_t            : 7;
      __IOM uint32_t SIZE       : 4;            /*!< [11..8] EMI Size Selecttion                                               */
            uint32_t            : 20;
    } EMI3CTRL_b;
  } ;
  
  union {
    __IOM uint32_t EMI4CTRL;                    /*!< (@ 0x0000024C) LDN EMI4 CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t ENABLE     : 1;            /*!< [0..0] EMI0 Enable                                                        */
            uint32_t            : 7;
      __IOM uint32_t SIZE       : 4;            /*!< [11..8] EMI Size Selecttion                                               */
            uint32_t            : 20;
    } EMI4CTRL_b;
  } ;
  
  union {
    __IOM uint32_t EMI5CTRL;                    /*!< (@ 0x00000250) LDN EMI5 CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t ENABLE     : 1;            /*!< [0..0] EMI0 Enable                                                        */
            uint32_t            : 7;
      __IOM uint32_t SIZE       : 4;            /*!< [11..8] EMI Size Selecttion                                               */
            uint32_t            : 20;
    } EMI5CTRL_b;
  } ;
  
  union {
    __IOM uint32_t EMI6CTRL;                    /*!< (@ 0x00000254) LDN EMI6 CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t ENABLE     : 1;            /*!< [0..0] EMI0 Enable                                                        */
            uint32_t            : 7;
      __IOM uint32_t SIZE       : 4;            /*!< [11..8] EMI Size Selecttion                                               */
            uint32_t            : 20;
    } EMI6CTRL_b;
  } ;
  
  union {
    __IOM uint32_t EMI7CTRL;                    /*!< (@ 0x00000258) LDN EMI7 CONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t ENABLE     : 1;            /*!< [0..0] EMI0 Enable                                                        */
            uint32_t            : 7;
      __IOM uint32_t SIZE       : 4;            /*!< [11..8] EMI Size Selecttion                                               */
            uint32_t            : 20;
    } EMI7CTRL_b;
  } ;
  
  union {
    __IOM uint32_t LDNRTC;                      /*!< (@ 0x0000025C) LDN RTC CCONTROL REGISTER                                  */
    
    struct {
      __IOM uint32_t ENABLE     : 1;            /*!< [0..0] LDN RTC Enable                                                     */
            uint32_t            : 7;
      __IOM uint32_t IOADDR     : 8;            /*!< [15..8] IO Port Address                                                   */
            uint32_t            : 16;
    } LDNRTC_b;
  } ;
  __IOM uint32_t  LDNRTCSAR;                    /*!< (@ 0x00000260) LDN RTC BUFFER REGISTER                                    */
  __IOM uint32_t  DIO0ADDR;                     /*!< (@ 0x00000264) DIO 0 MAPPING ADDRESS REGISTER                             */
  __IOM uint32_t  DIO1ADDR;                     /*!< (@ 0x00000268) DIO 1 MAPPING ADDRESS REGISTER                             */
} ESPI_Type;                                    /*!< Size = 620 (0x26c)                                                        */



/* =========================================================================================================================== */
/* ================                                         MONOTONIC                                         ================ */
/* =========================================================================================================================== */


/**
  * @brief Monotonic Counter (MONOTONIC)
  */

typedef struct {                                /*!< (@ 0x400B1400) MONOTONIC Structure                                        */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000000) CONTROL REGISTER                                           */
    
    struct {
      __IOM uint32_t INC0       : 1;            /*!< [0..0] Increase monotonic counter #0 by 1                                 */
            uint32_t            : 7;
      __IOM uint32_t INC1       : 1;            /*!< [8..8] Increase monotonic counter #1 by 1                                 */
            uint32_t            : 7;
      __IOM uint32_t INC2       : 1;            /*!< [16..16] Increase monotonic counter #2 by 1                               */
            uint32_t            : 7;
      __IOM uint32_t INC3       : 1;            /*!< [24..24] Increase monotonic counter #3 by 1                               */
      __IOM uint32_t LOAD       : 1;            /*!< [25..25] Load monotonic counter #0 to #3 to VBAT domain                   */
      __IOM uint32_t UPDATE     : 1;            /*!< [26..26] Update monotonic counter #0 to #3 from VBAT domain               */
            uint32_t            : 5;
    } CTRL_b;
  } ;
  __IOM uint32_t  MCNT0;                        /*!< (@ 0x00000004) MONOTONIC COUNTER #0 REGISTER                              */
  __IOM uint32_t  MCNT1;                        /*!< (@ 0x00000008) MONOTONIC COUNTER #1 REGISTER                              */
  __IOM uint32_t  MCNT2;                        /*!< (@ 0x0000000C) MONOTONIC COUNTER #2 REGISTER                              */
  __IOM uint32_t  MCNT3;                        /*!< (@ 0x00000010) MONOTONIC COUNTER #3 REGISTER                              */
} MONOTONIC_Type;                               /*!< Size = 20 (0x14)                                                          */



/* =========================================================================================================================== */
/* ================                                            MBX                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief Mailbox Controller (MBX)
  */

typedef struct {                                /*!< (@ 0x40221600) MBX Structure                                              */
  
  union {
    __IOM uint32_t STS;                         /*!< (@ 0x00000000) MAILBOX STATUS REGISTER                                    */
    
    struct {
      __IOM uint32_t STS        : 8;            /*!< [7..0] Status                                                             */
            uint32_t            : 24;
    } STS_b;
  } ;
  
  union {
    __IOM uint32_t DATA[48];                    /*!< (@ 0x00000004) MAILBOX DATA[0..47] REGISTER                               */
    
    struct {
      __IOM uint32_t DATA       : 8;            /*!< [7..0] Mailbox Data Buffer                                                */
            uint32_t            : 24;
    } DATA_b[48];
  } ;
  
  union {
    __IOM uint32_t INTCTRL;                     /*!< (@ 0x000000C4) MAILBOX INTERRUPT CONTROL REGISTER                         */
    
    struct {
      __IOM uint32_t IRQEN      : 1;            /*!< [0..0] Auto IRQ Enable                                                    */
      __IOM uint32_t TGLV       : 1;            /*!< [1..1] Trigger Level Select                                               */
      __IOM uint32_t INTEN      : 1;            /*!< [2..2] Interrupt Enable                                                   */
            uint32_t            : 29;
    } INTCTRL_b;
  } ;
  
  union {
    __IOM uint32_t MBXIRQNUM;                   /*!< (@ 0x000000C8) MAILBOX IRQ NUMBER REGISTER                                */
    
    struct {
      __IOM uint32_t NUM        : 8;            /*!< [7..0] IRQ Number                                                         */
            uint32_t            : 24;
    } MBXIRQNUM_b;
  } ;
  
  union {
    __IOM uint32_t MBXINTSTS;                   /*!< (@ 0x000000CC) MAILBOX INTERRUPT STATUS REGISTER                          */
    
    struct {
      __IOM uint32_t STS        : 1;            /*!< [0..0] Indicate wheather the interrupt is occurred                        */
      __IOM uint32_t CLR        : 1;            /*!< [1..1] Write 1 to clear interrupt status                                  */
            uint32_t            : 30;
    } MBXINTSTS_b;
  } ;
} MBX_Type;                                     /*!< Size = 208 (0xd0)                                                         */



/* =========================================================================================================================== */
/* ================                                          LEDSTP                                           ================ */
/* =========================================================================================================================== */


/**
  * @brief LED Strip Controller (LEDSTP)
  */

typedef struct {                                /*!< (@ 0x40251A00) LEDSTP Structure                                           */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000000) LEDSTP CONFIGURATION REGISTER                              */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Enable LED Strip                                                   */
      __IOM uint32_t START      : 1;            /*!< [1..1] START LED Strip                                                    */
      __IOM uint32_t STOP       : 1;            /*!< [2..2] STOP LED Strip                                                     */
      __IOM uint32_t PENDEN     : 1;            /*!< [3..3] LED Strip Pending Code Enable                                      */
      __IOM uint32_t MDSEL      : 2;            /*!< [5..4] LED Strip Mode Selection                                           */
      __IOM uint32_t FINISHINTEN : 1;           /*!< [6..6] LED Strip Finish Interrupt Enable                                  */
      __IOM uint32_t BUSYSTS    : 1;            /*!< [7..7] LED Strip Busy Status                                              */
      __IOM uint32_t FINISHSTS  : 1;            /*!< [8..8] LED Strip Transfer Finish                                          */
            uint32_t            : 7;
      __IOM uint32_t REPCY      : 8;            /*!< [23..16] LED Strip Repeat Cycle                                           */
            uint32_t            : 8;
    } CTRL_b;
  } ;
  
  union {
    __IOM uint32_t CODE0CFG;                    /*!< (@ 0x00000004) LEDSTP 0 CODE PERIOD REGISTER                              */
    
    struct {
      __IOM uint32_t ZCLW       : 6;            /*!< [5..0] LED Strip 0 Code Low Width                                         */
            uint32_t            : 2;
      __IOM uint32_t ZCHW       : 6;            /*!< [13..8] LED Strip 0 Code High Width                                       */
            uint32_t            : 18;
    } CODE0CFG_b;
  } ;
  
  union {
    __IOM uint32_t CODE1CFG;                    /*!< (@ 0x00000008) LEDSTP 1 CODE PERIOD REGISTER                              */
    
    struct {
      __IOM uint32_t OCLW       : 6;            /*!< [5..0] LED Strip 1 Low Width                                              */
            uint32_t            : 2;
      __IOM uint32_t OCHW       : 6;            /*!< [13..8] LED Strip 1 High Width                                            */
            uint32_t            : 18;
    } CODE1CFG_b;
  } ;
  
  union {
    __IOM uint32_t RSTCFG;                      /*!< (@ 0x0000000C) LEDSTP RESET TIME REGISTER                                 */
    
    struct {
      __IOM uint32_t RSTW       : 32;           /*!< [31..0] LED Strip Reset Time                                              */
    } RSTCFG_b;
  } ;
  
  union {
    __IOM uint32_t PCFG;                        /*!< (@ 0x00000010) LEDSTP PENDING TIME REGISTER                               */
    
    struct {
      __IOM uint32_t PENDW      : 6;            /*!< [5..0] LED Strip Pending Time (ms)                                        */
            uint32_t            : 26;
    } PCFG_b;
  } ;
  __IOM uint32_t  TRANSADD;                     /*!< (@ 0x00000014) LEDSTP PATTERN ADDRESS REGISTER                            */
  
  union {
    __IOM uint32_t TRANSLEN;                    /*!< (@ 0x00000018) LEDSTP TRANSFER LENGTH REGISTER                            */
    
    struct {
      __IOM uint32_t TRANSLEN   : 14;           /*!< [13..0] LED Strip Transfer Length (Bytes)                                 */
            uint32_t            : 18;
    } TRANSLEN_b;
  } ;
} LEDSTP_Type;                                  /*!< Size = 28 (0x1c)                                                          */



/* =========================================================================================================================== */
/* ================                                          LEDDBG0                                          ================ */
/* =========================================================================================================================== */


/**
  * @brief LED Debug 0 (LEDDBG0)
  */

typedef struct {                                /*!< (@ 0x4000F930) LEDDBG0 Structure                                          */
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000000) LED DEBUG CONTROL REGISTER                                 */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] LED DEBUG ENABLE                                                   */
      __IOM uint32_t START      : 1;            /*!< [1..1] LED START                                                          */
      __IOM uint32_t STOP       : 1;            /*!< [2..2] LED DEBUG STOP                                                     */
      __IOM uint32_t MDSEL      : 2;            /*!< [4..3] LED DEBUG MODE SELECT                                              */
      __IOM uint32_t INV        : 1;            /*!< [5..5] Inverse Idle Ouput                                                 */
      __IOM uint32_t DATAMD     : 1;            /*!< [6..6] LED Debug Data Mode                                                */
      __IOM uint32_t ORDER      : 1;            /*!< [7..7] BIT TRANSFER ORDER                                                 */
            uint32_t            : 24;
    } CTRL_b;
  } ;
  
  union {
    __IOM uint32_t CODE0CFG;                    /*!< (@ 0x00000004) LEDDBG 0 CODE PERIOD REGISTER                              */
    
    struct {
      __IOM uint32_t ZCLW       : 16;           /*!< [15..0] LED Debug 0 Code Low Width                                        */
      __IOM uint32_t ZCHW       : 16;           /*!< [31..16] LED Debug 0 Code High Width                                      */
    } CODE0CFG_b;
  } ;
  
  union {
    __IOM uint32_t CODE1CFG;                    /*!< (@ 0x00000008) LEDDBG 1 CODE PERIOD REGISTER                              */
    
    struct {
      __IOM uint32_t OCLW       : 16;           /*!< [15..0] LED Debug 1 Low Width                                             */
      __IOM uint32_t OCHW       : 16;           /*!< [31..16] LED Debug 1 High Width                                           */
    } CODE1CFG_b;
  } ;
  __IOM uint32_t  IDLEDLY;                      /*!< (@ 0x0000000C) LEDSTP IDLE DELAY REGISTER                                 */
  __IOM uint32_t  DATA;                         /*!< (@ 0x00000010) LEDDBG DATA REGISTER                                       */
} LEDDBG_Type;                                  /*!< Size = 20 (0x14)                                                          */



/* =========================================================================================================================== */
/* ================                                           SPIM                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief SPIM Controller (SPIM)
  */

typedef struct {                                /*!< (@ 0x40221B00) SPIM Structure                                             */
  
  union {
    __IOM uint32_t CTRL0;                       /*!< (@ 0x00000000) SPIM CONTROL 0 REGISTER                                    */
    
    struct {
      __IOM uint32_t DFS        : 4;            /*!< [3..0] Data Frame Size                                                    */
      __IOM uint32_t FRF        : 2;            /*!< [5..4] Frame Format                                                       */
      __IOM uint32_t SCPH       : 1;            /*!< [6..6] Serial Clock Phase                                                 */
      __IOM uint32_t SCPOL      : 1;            /*!< [7..7] Serial Clock Polarity                                              */
      __IOM uint32_t TMOD       : 2;            /*!< [9..8] Transfer Mode                                                      */
      __IOM uint32_t SLV_OE     : 1;            /*!< [10..10] Slave Output Enable                                              */
      __IOM uint32_t SRL        : 1;            /*!< [11..11] Shift Register Loop                                              */
      __IOM uint32_t CFS        : 4;            /*!< [15..12] Control Frame Size                                               */
      __IOM uint32_t DFS32      : 5;            /*!< [20..16] Data Frame Size                                                  */
      __IOM uint32_t IOMODE     : 2;            /*!< [22..21] IO Mode                                                          */
            uint32_t            : 1;
      __IOM uint32_t SSTE       : 1;            /*!< [24..24] Slave Select Toggle Enable                                       */
      __IOM uint32_t SECONV     : 1;            /*!< [25..25] Set the Endianness for XIP and data register reads               */
            uint32_t            : 6;
    } CTRL0_b;
  } ;
  
  union {
    __IOM uint32_t CTRLR1;                      /*!< (@ 0x00000004) SPIM CONTROL 1 REGISTER                                    */
    
    struct {
      __IOM uint32_t NDF        : 16;           /*!< [15..0] Number of Data Frames                                             */
            uint32_t            : 16;
    } CTRLR1_b;
  } ;
  
  union {
    __IOM uint32_t SPIENR;                      /*!< (@ 0x00000008) SPI ENABLE REGISTER                                        */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] SPI Maste Enable Bit                                               */
            uint32_t            : 31;
    } SPIENR_b;
  } ;
  
  union {
    __IOM uint32_t MWCR;                        /*!< (@ 0x0000000C) Microwire Control Register                                 */
    
    struct {
      __IOM uint32_t MWMOD      : 1;            /*!< [0..0] Microwire Transfer Sequentail Mode                                 */
      __IM  uint32_t MDD        : 1;            /*!< [1..1] Microwire Control                                                  */
      __IOM uint32_t MHS        : 1;            /*!< [2..2] Microwire Handshaking                                              */
            uint32_t            : 29;
    } MWCR_b;
  } ;
  __IOM uint32_t  SER;                          /*!< (@ 0x00000010) SLAVE ENABLE REGISTER                                      */
  
  union {
    __IOM uint32_t BAUDR;                       /*!< (@ 0x00000014) BAUD RATE SELECT REGISTER                                  */
    
    struct {
      __IOM uint32_t SCKDV      : 16;           /*!< [15..0] Clock Divider                                                     */
            uint32_t            : 16;
    } BAUDR_b;
  } ;
  __IOM uint32_t  TXFTLR;                       /*!< (@ 0x00000018) TX THRESHOLD REGISTER                                      */
  __IOM uint32_t  RXFTLR;                       /*!< (@ 0x0000001C) RX THRESHOLD REGISTER                                      */
  __IM  uint32_t  TXFLR;                        /*!< (@ 0x00000020) TX FIFO LEVEL REGISTER                                     */
  __IM  uint32_t  RXFLR;                        /*!< (@ 0x00000024) RX FIFO LEVEL REGISTER                                     */
  
  union {
    __IM  uint32_t STATUS;                      /*!< (@ 0x00000028) STATUS REGISTER                                            */
    
    struct {
      __IM  uint32_t BUSY       : 1;            /*!< [0..0] Busy Flag                                                          */
      __IM  uint32_t TFNF       : 1;            /*!< [1..1] Transmit FIFO Not Full                                             */
      __IM  uint32_t TFE        : 1;            /*!< [2..2] Transmit FIFO Empty                                                */
      __IM  uint32_t RFNE       : 1;            /*!< [3..3] Receive FIFO Not Empty                                             */
      __IM  uint32_t RFF        : 1;            /*!< [4..4] Receive FIFO Full                                                  */
      __IM  uint32_t TXE        : 1;            /*!< [5..5] Transmission Error                                                 */
      __IM  uint32_t DCOL       : 1;            /*!< [6..6] Data Collision Error                                               */
            uint32_t            : 25;
    } STATUS_b;
  } ;
  
  union {
    __IOM uint32_t IMR;                         /*!< (@ 0x0000002C) INTERRUPT MASK REGISTER                                    */
    
    struct {
      __IOM uint32_t TXEIM      : 1;            /*!< [0..0] Transmit FIFO Empty Interrupt Mask                                 */
      __IOM uint32_t TXOIM      : 1;            /*!< [1..1] Transmit FIFO Overflow Interrupt Mask                              */
      __IOM uint32_t RXUIM      : 1;            /*!< [2..2] Receive FIFO Underflow Interrupt Mask                              */
      __IOM uint32_t RXOIM      : 1;            /*!< [3..3] Receive FIFO Overflow Interrupt Mask                               */
      __IOM uint32_t RXFIM      : 1;            /*!< [4..4] Receive FIFO Full Interrupt Mask 0                                 */
      __IOM uint32_t MSTIM      : 1;            /*!< [5..5] Multi-Master Contention Interrupt Mask                             */
      __IOM uint32_t XRXOIM     : 1;            /*!< [6..6] XIP Receive FIFO Overflow Interrupt Mask                           */
            uint32_t            : 25;
    } IMR_b;
  } ;
  
  union {
    __IM  uint32_t ISR;                         /*!< (@ 0x00000030) INTERRUPT STATUS REGISTER                                  */
    
    struct {
      __IM  uint32_t TXEIS      : 1;            /*!< [0..0] Transmit FIFO Empty Interrupt Status                               */
      __IM  uint32_t TXOIS      : 1;            /*!< [1..1] Transmit FIFO Overflow Interrupt Status                            */
      __IM  uint32_t RXUIS      : 1;            /*!< [2..2] Receive FIFO Underflow Interrupt Status                            */
      __IM  uint32_t RXOIS      : 1;            /*!< [3..3] Receive FIFO Overflow Interrupt Status                             */
      __IM  uint32_t RXFIS      : 1;            /*!< [4..4] Receive FIFO Full Interrupt Status                                 */
      __IM  uint32_t MSTIS      : 1;            /*!< [5..5] Multi-Master Contention Interrupt Status                           */
            uint32_t            : 26;
    } ISR_b;
  } ;
  
  union {
    __IM  uint32_t RISR;                        /*!< (@ 0x00000034) INTERRUPT STATUS REGISTER                                  */
    
    struct {
      __IM  uint32_t TXEIR      : 1;            /*!< [0..0] Transmit FIFO Empty Raw Interrupt Status                           */
      __IM  uint32_t TXOIR      : 1;            /*!< [1..1] Transmit FIFO Overflow Raw Interrupt Status                        */
      __IM  uint32_t RXUIR      : 1;            /*!< [2..2] Receive FIFO Underflow Raw Interrupt Status                        */
      __IM  uint32_t RXOIR      : 1;            /*!< [3..3] Receive FIFO Overflow Raw Interrupt Status                         */
      __IM  uint32_t RXFIR      : 1;            /*!< [4..4] Receive FIFO Full Raw Interrupt Status                             */
      __IM  uint32_t MSTIR      : 1;            /*!< [5..5] Multi-Master Contention Raw Interrupt Status                       */
            uint32_t            : 26;
    } RISR_b;
  } ;
  
  union {
    __IM  uint32_t TXOICR;                      /*!< (@ 0x00000038) Transmit FIFO Overflow Interrupt Clear Register            */
    
    struct {
      __IM  uint32_t CLEAR      : 1;            /*!< [0..0] Clear Interrupt                                                    */
            uint32_t            : 31;
    } TXOICR_b;
  } ;
  
  union {
    __IM  uint32_t RXOICR;                      /*!< (@ 0x0000003C) Receive FIFO Overflow Interrupt Clear Register             */
    
    struct {
      __IM  uint32_t CLEAR      : 1;            /*!< [0..0] Clear Interrupt                                                    */
            uint32_t            : 31;
    } RXOICR_b;
  } ;
  
  union {
    __IM  uint32_t RXUICR;                      /*!< (@ 0x00000040) Receive FIFO Underflow Interrupt Clear Register            */
    
    struct {
      __IM  uint32_t CLEAR      : 1;            /*!< [0..0] Clear Interrupt                                                    */
            uint32_t            : 31;
    } RXUICR_b;
  } ;
  
  union {
    __IM  uint32_t MSTICR;                      /*!< (@ 0x00000044) Multi-Master Interrupt Clear Register                      */
    
    struct {
      __IM  uint32_t CLEAR      : 1;            /*!< [0..0] Clear Interrupt                                                    */
            uint32_t            : 31;
    } MSTICR_b;
  } ;
  
  union {
    __IM  uint32_t ICR;                         /*!< (@ 0x00000048) Interrupt Clear Register                                   */
    
    struct {
      __IM  uint32_t CLEAR      : 1;            /*!< [0..0] Clear Interrupt                                                    */
            uint32_t            : 31;
    } ICR_b;
  } ;
  
  union {
    __IOM uint32_t DMACR;                       /*!< (@ 0x0000004C) DMA Control Register                                       */
    
    struct {
      __IOM uint32_t RDMAE      : 1;            /*!< [0..0] Receive DMA Enable                                                 */
      __IOM uint32_t TDMAE      : 1;            /*!< [1..1] Transmit DMA Enable                                                */
            uint32_t            : 30;
    } DMACR_b;
  } ;
  __IOM uint32_t  DMATDLR;                      /*!< (@ 0x00000050) DMA Transmit Data Level Register                           */
  __IOM uint32_t  DMARDLR;                      /*!< (@ 0x00000054) DMA Receive Data Level Register                            */
  __IM  uint32_t  RESERVED[2];
  __IOM uint32_t  DATA[36];                     /*!< (@ 0x00000060) DATA REGISTER                                              */
} SPIM_Type;                                    /*!< Size = 240 (0xf0)                                                         */



/* =========================================================================================================================== */
/* ================                                         MICROCNT                                          ================ */
/* =========================================================================================================================== */


/**
  * @brief Micro Counter Controller (MICROCNT)
  */

typedef struct {                                /*!< (@ 0x40255400) MICROCNT Structure                                         */
  
  union {
    __IOM uint32_t BIAS;                        /*!< (@ 0x00000000) BIAS REGISTER                                              */
    
    struct {
      __IOM uint32_t CNTBIAS    : 8;            /*!< [7..0] Micro Counter Bias                                                 */
      __IOM uint32_t CNTVALUE   : 8;            /*!< [15..8] Micro Counter Value                                               */
            uint32_t            : 16;
    } BIAS_b;
  } ;
  
  union {
    __IOM uint32_t CTRL;                        /*!< (@ 0x00000004) CTRL REGISTER                                              */
    
    struct {
      __IOM uint32_t EN         : 1;            /*!< [0..0] Micro Counter Enable                                               */
      __IOM uint32_t STOP       : 1;            /*!< [1..1] Micro Counter Stop                                                 */
            uint32_t            : 30;
    } CTRL_b;
  } ;
} MICROCNT_Type;                                /*!< Size = 8 (0x8)                                                            */



/* =========================================================================================================================== */
/* ================                                            CEC                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief CEC Controller (CEC)
  */

typedef struct {                                /*!< (@ 0x40254000) CEC Structure                                              */
  
  union {
    __OM  uint32_t TXFIFO;                      /*!< (@ 0x00000000) TX DATA FIFO REGISTER                                      */
    
    struct {
      __OM  uint32_t DATA       : 8;            /*!< [7..0] TX Data FIFO                                                       */
            uint32_t            : 24;
    } TXFIFO_b;
  } ;
  
  union {
    __IM  uint32_t TXSTS;                       /*!< (@ 0x00000004) TX FIFO STATUS REGISTER                                    */
    
    struct {
      __IM  uint32_t TXFULL     : 1;            /*!< [0..0] TX FIFO Full                                                       */
      __IM  uint32_t TXEMPTY    : 1;            /*!< [1..1] TX FIFO Empty                                                      */
            uint32_t            : 30;
    } TXSTS_b;
  } ;
  
  union {
    __IM  uint32_t RXFIFO;                      /*!< (@ 0x00000008) RX DATA FIFO REGISTER                                      */
    
    struct {
      __IM  uint32_t DATA       : 8;            /*!< [7..0] RX Data FIFO                                                       */
            uint32_t            : 24;
    } RXFIFO_b;
  } ;
  
  union {
    __IM  uint32_t RXSTS;                       /*!< (@ 0x0000000C) RX FIFO STATUS REGISTER                                    */
    
    struct {
      __IM  uint32_t RXFULL     : 1;            /*!< [0..0] RX FIFO Full                                                       */
      __IM  uint32_t RXEMPTY    : 1;            /*!< [1..1] RX FIFO Empty                                                      */
            uint32_t            : 30;
    } RXSTS_b;
  } ;
  
  union {
    __IOM uint32_t CECADDR;                     /*!< (@ 0x00000010) CEC LOGICAL ADDRESS REGISTER                               */
    
    struct {
      __IOM uint32_t ADDR       : 4;            /*!< [3..0] CEC Logical Address                                                */
            uint32_t            : 28;
    } CECADDR_b;
  } ;
  
  union {
    __IOM uint32_t CECCTRL;                     /*!< (@ 0x00000014) CEC CONTROL REGISTER                                       */
    
    struct {
      __IOM uint32_t TXSTART    : 1;            /*!< [0..0] CEC TC Start                                                       */
      __IOM uint32_t BTIME      : 1;            /*!< [1..1] CEC Bit Timing                                                     */
      __IOM uint32_t BEOM       : 1;            /*!< [2..2] CEC EOM Bit                                                        */
      __IOM uint32_t NACKEN     : 1;            /*!< [3..3] NACK Enable                                                        */
            uint32_t            : 28;
    } CECCTRL_b;
  } ;
  
  union {
    __IOM uint32_t CECSTS;                      /*!< (@ 0x00000018) CEC STATUS REGISTER                                        */
    
    struct {
      __IOM uint32_t TMERR      : 1;            /*!< [0..0] CEC Bit Timing Error                                               */
      __IOM uint32_t BUSLOW     : 1;            /*!< [1..1] CEC Bus Low                                                        */
      __IOM uint32_t LOSSARB    : 1;            /*!< [2..2] CEC Loss Arbitration                                               */
      __IOM uint32_t ERRBIT     : 1;            /*!< [3..3] CEC Error Bit Notified                                             */
      __IOM uint32_t BLKDONE    : 1;            /*!< [4..4] CEC Block Done                                                     */
      __IOM uint32_t HEADRX     : 1;            /*!< [5..5] CEC Header Recived                                                 */
            uint32_t            : 26;
    } CECSTS_b;
  } ;
  
  union {
    __IOM uint32_t CECINTEN;                    /*!< (@ 0x0000001C) CEC INTERRUPT ENABLE REGISTER                              */
    
    struct {
      __IOM uint32_t TMERREN    : 1;            /*!< [0..0] CEC Bit Timing Error Interrupt Enable                              */
      __IOM uint32_t BUSLOWEN   : 1;            /*!< [1..1] CEC Bus Low Interrupt Enable                                       */
      __IOM uint32_t LOSSARBEN  : 1;            /*!< [2..2] CEC Loss Arbitration Interrupt Enable                              */
      __IOM uint32_t ERRBITEN   : 1;            /*!< [3..3] CEC Error Bit Notified Interrupt Enable                            */
      __IOM uint32_t BLKDONE    : 1;            /*!< [4..4] CEC Block Done Interrupt Enable                                    */
      __IOM uint32_t HEADRX     : 1;            /*!< [5..5] CEC Header Recived Interrupt Enable                                */
            uint32_t            : 26;
    } CECINTEN_b;
  } ;
  
  union {
    __IM  uint32_t CECOPSTS;                    /*!< (@ 0x00000020) CEC OPERATION STATUS REGISTER                              */
    
    struct {
      __IM  uint32_t BUSY       : 1;            /*!< [0..0] CEC In Process                                                     */
      __IM  uint32_t ACK        : 1;            /*!< [1..1] ACK Status from Follower                                           */
      __IM  uint32_t EOM        : 1;            /*!< [2..2] EOM Status in Header Block                                         */
            uint32_t            : 29;
    } CECOPSTS_b;
  } ;
  
  union {
    __IM  uint32_t CECHEADER;                   /*!< (@ 0x00000024) CEC HEADER REGISTER                                        */
    
    struct {
      __IM  uint32_t DESADDR    : 4;            /*!< [3..0] Destination Address                                                */
      __IM  uint32_t INIADDR    : 4;            /*!< [7..4] Initiator Address                                                  */
            uint32_t            : 24;
    } CECHEADER_b;
  } ;
} CEC_Type;                                     /*!< Size = 40 (0x28)                                                          */



/* =========================================================================================================================== */
/* ================                                          I2CDBG                                           ================ */
/* =========================================================================================================================== */


/**
  * @brief I2C Debug Controller (I2CDBG)
  */

typedef struct {                                /*!< (@ 0x40251C00) I2CDBG Structure                                           */
  
  union {
    __IOM uint32_t STS;                         /*!< (@ 0x00000000) STATUS REGISTER                                            */
    
    struct {
      __IOM uint32_t DBGON1     : 1;            /*!< [0..0] Debug On                                                           */
      __IOM uint32_t DBGON2     : 1;            /*!< [1..1] Debug On                                                           */
      __IOM uint32_t DBGON3     : 1;            /*!< [2..2] Debug On                                                           */
      __IOM uint32_t DBGON4     : 1;            /*!< [3..3] Debug On                                                           */
      __IOM uint32_t DBGON5     : 1;            /*!< [4..4] Debug On                                                           */
      __IOM uint32_t DBGON6     : 1;            /*!< [5..5] Debug On                                                           */
      __IOM uint32_t DBGON7     : 1;            /*!< [6..6] Debug On                                                           */
      __IOM uint32_t DBGON8     : 1;            /*!< [7..7] Debug On                                                           */
      __IOM uint32_t DBGON9     : 1;            /*!< [8..8] Debug On                                                           */
      __IOM uint32_t DBGON10    : 1;            /*!< [9..9] Debug On                                                           */
      __IOM uint32_t DBGON11    : 1;            /*!< [10..10] Debug On                                                         */
            uint32_t            : 21;
    } STS_b;
  } ;
  
  union {
    __IOM uint32_t I2CDBGCFG;                   /*!< (@ 0x00000004) I2C DEBUG CONFIGURATION REGISTER                           */
    
    struct {
      __IOM uint32_t DBG1SRC    : 1;            /*!< [0..0] Debug Setting Source                                               */
      __IOM uint32_t DBG1FWEN   : 1;            /*!< [1..1] Debug Firmware Setting                                             */
      __IOM uint32_t DBG2SRC    : 1;            /*!< [2..2] Debug Setting Source                                               */
      __IOM uint32_t DBG2FWEN   : 1;            /*!< [3..3] Debug Firmware Setting                                             */
      __IOM uint32_t DBG3SRC    : 1;            /*!< [4..4] Debug Setting Source                                               */
      __IOM uint32_t DBG3FWEN   : 1;            /*!< [5..5] Debug Firmware Setting                                             */
      __IOM uint32_t DBG4SRC    : 1;            /*!< [6..6] Debug Setting Source                                               */
      __IOM uint32_t DBG4FWEN   : 1;            /*!< [7..7] Debug Firmware Setting                                             */
      __IOM uint32_t DBG5SRC    : 1;            /*!< [8..8] Debug Setting Source                                               */
      __IOM uint32_t DBG5FWEN   : 1;            /*!< [9..9] Debug Firmware Setting                                             */
      __IOM uint32_t DBG6SRC    : 1;            /*!< [10..10] Debug Setting Source                                             */
      __IOM uint32_t DBG6FWEN   : 1;            /*!< [11..11] Debug Firmware Setting                                           */
      __IOM uint32_t DBG7SRC    : 1;            /*!< [12..12] Debug Setting Source                                             */
      __IOM uint32_t DBG7FWEN   : 1;            /*!< [13..13] Debug Firmware Setting                                           */
      __IOM uint32_t DBG8SRC    : 1;            /*!< [14..14] Debug Setting Source                                             */
      __IOM uint32_t DBG8FWEN   : 1;            /*!< [15..15] Debug Firmware Setting                                           */
      __IOM uint32_t DBG9SRC    : 1;            /*!< [16..16] Debug Setting Source                                             */
      __IOM uint32_t DBG9FWEN   : 1;            /*!< [17..17] Debug Firmware Setting                                           */
      __IOM uint32_t DBG10SRC   : 1;            /*!< [18..18] Debug Setting Source                                             */
      __IOM uint32_t DBG10FWEN  : 1;            /*!< [19..19] Debug Firmware Setting                                           */
      __IOM uint32_t DBG11SRC   : 1;            /*!< [20..20] Debug Setting Source                                             */
      __IOM uint32_t DBG11FWEN  : 1;            /*!< [21..21] Debug Firmware Setting                                           */
            uint32_t            : 10;
    } I2CDBGCFG_b;
  } ;
  
  union {
    __IOM uint32_t IXCKEYCFG;                   /*!< (@ 0x00000008) I2C/I3C PROTECTION KEY CONFIGURATION REGISTER              */
    
    struct {
      __IOM uint32_t LOCK       : 1;            /*!< [0..0] Lock Access to SPIC                                                */
      __IOM uint32_t CMDSET     : 1;            /*!< [1..1] Set Lock Command                                                   */
      __IM  uint32_t LOCKSTS    : 1;            /*!< [2..2] Lock Status                                                        */
            uint32_t            : 29;
    } IXCKEYCFG_b;
  } ;
  __IOM uint32_t  UNLOCKKEYH;                   /*!< (@ 0x0000000C) UNLOCKED KEY HIGH 4 BYTE REGISTER                          */
  __IOM uint32_t  UNLOCKKEYL;                   /*!< (@ 0x00000010) UNLOCKED KEY LOW 4 BYTE REGISTER                           */
  __IOM uint32_t  LOCKKEYH;                     /*!< (@ 0x00000014) LOCKED KEY HIGH 4 BYTE REGISTER                            */
  __IOM uint32_t  LOCKKEYL;                     /*!< (@ 0x00000018) LOCKED KEY LOW 4 BYTE REGISTER                             */
} I2CDBG_Type;                                  /*!< Size = 28 (0x1c)                                                          */



/* =========================================================================================================================== */
/* ================                                           BRAM                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief Battery RAM (BRAM)
  */

typedef struct {                                /*!< (@ 0x20200000) BRAM Structure                                             */
  __IOM uint8_t   BYTE[256];                    /*!< (@ 0x00000000) BYTE DATA                                                  */
} BRAM_Type;                                    /*!< Size = 256 (0x100)                                                        */



/* =========================================================================================================================== */
/* ================                                           TRAM                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief Turbo memory compare (TRAM)
  */

typedef struct {                                /*!< (@ 0x40255600) TRAM Structure                                             */
  __IOM uint8_t   BYTE[256];                    /*!< (@ 0x00000000) BYTE DATA                                                  */
} TRAM_Type;                                    /*!< Size = 256 (0x100)                                                        */



/* =========================================================================================================================== */
/* ================                                         USB_CORE1                                         ================ */
/* =========================================================================================================================== */


/**
  * @brief USB OTG Controller 1 (USB_CORE1)
  */

typedef struct {                                /*!< (@ 0x50000000) USB_CORE1 Structure                                        */
  
  union {
    __IOM uint32_t GOTGCTL;                     /*!< (@ 0x00000000) Control and Status Register                                */
    
    struct {
      __IM  uint32_t SesReqScs  : 1;            /*!< [0..0] Session Request Success                                            */
      __IOM uint32_t SesReq     : 1;            /*!< [1..1] Session Request                                                    */
      __IOM uint32_t VbvalidOvEn : 1;           /*!< [2..2] VBUS Valid Override Enable                                         */
      __IOM uint32_t VbvalidOvVal : 1;          /*!< [3..3] VBUS Valid OverrideValue (VbvalidOvVal)                            */
      __IOM uint32_t AvalidOvEn : 1;            /*!< [4..4] A-Peripheral Session Valid Override Enable                         */
      __IOM uint32_t AvalidOvVal : 1;           /*!< [5..5] A-Peripheral Session Valid OverrideValue (AvalidOvVal)             */
      __IOM uint32_t BvalidOvEn : 1;            /*!< [6..6] B-Peripheral Session Valid Override Enable                         */
      __IOM uint32_t BvalidOvVal : 1;           /*!< [7..7] B-Peripheral Session Valid OverrideValue (BvalidOvVal)             */
      __IM  uint32_t HstNegScs  : 1;            /*!< [8..8] Host Negotiation Success (HstNegScs)                               */
      __IOM uint32_t HNPReq     : 1;            /*!< [9..9] HNP Request (HNPReq)                                               */
      __IOM uint32_t HstSetHNPEn : 1;           /*!< [10..10] Host Set HNP Enable (HstSetHNPEn)                                */
      __IOM uint32_t DevHNPEn   : 1;            /*!< [11..11] Device HNP Enabled (DevHNPEn)                                    */
      __IOM uint32_t EHEn       : 1;            /*!< [12..12] Embedded Host Enable (EHEn)                                      */
            uint32_t            : 2;
      __IOM uint32_t DbnceFltrBypass : 1;       /*!< [15..15] Debounce Filter Bypass                                           */
      __IM  uint32_t ConIDSts   : 1;            /*!< [16..16] Connector ID Status                                              */
      __IM  uint32_t DbncTime   : 1;            /*!< [17..17] Debounce Time                                                    */
      __IM  uint32_t ASesVld    : 1;            /*!< [18..18] A-session Valid                                                  */
      __IM  uint32_t BSesVld    : 1;            /*!< [19..19] B-session Valid                                                  */
      __IOM uint32_t OTGVer     : 1;            /*!< [20..20] OTG Version                                                      */
      __IM  uint32_t CurMod     : 1;            /*!< [21..21] Current Mode of Operation                                        */
      __IM  uint32_t MultValIdBC : 5;           /*!< [26..22] Multi Valued ID pin (MultValIdBC)                                */
      __IOM uint32_t ChirpEn    : 1;            /*!< [27..27] Core asserting chirp_on before sending an actual Chirp
                                                     K signal on USB.                                                          */
            uint32_t            : 3;
      __IOM uint32_t Testmode_corr_eUSB2 : 1;   /*!< [31..31] Correction for eUSB2 PHY during Test mode                        */
    } GOTGCTL_b;
  } ;
  
  union {
    __IOM uint32_t GOTGINT;                     /*!< (@ 0x00000004) Interrupt Register                                         */
    
    struct {
            uint32_t            : 2;
      __IOM uint32_t SesEndDet  : 1;            /*!< [2..2] Session End Detected                                               */
            uint32_t            : 5;
      __IOM uint32_t SesReqSucStsChng : 1;      /*!< [8..8] Session Request Success Status Change                              */
      __IOM uint32_t HstNegSucStsChng : 1;      /*!< [9..9] Host Negotiation Success Status Change                             */
            uint32_t            : 7;
      __IOM uint32_t HstNegDet  : 1;            /*!< [17..17] Host Negotiation Detected                                        */
      __IOM uint32_t ADevTOUTChg : 1;           /*!< [18..18] A-Device Timeout Change                                          */
      __IOM uint32_t DbnceDone  : 1;            /*!< [19..19] Debounce Done                                                    */
      __IOM uint32_t MultValIpChng : 1;         /*!< [20..20] Indicates change in ACA pin value                                */
            uint32_t            : 11;
    } GOTGINT_b;
  } ;
  
  union {
    __IOM uint32_t GAHBCFG;                     /*!< (@ 0x00000008) AHB Configuration Register                                 */
    
    struct {
      __IOM uint32_t GlblIntrMsk : 1;           /*!< [0..0] Global Interrupt Mask                                              */
      __IOM uint32_t HBstLen    : 4;            /*!< [4..1] Burst Length/Type                                                  */
      __IOM uint32_t DMAEn      : 1;            /*!< [5..5] DMA Enable                                                         */
            uint32_t            : 1;
      __IOM uint32_t NPTxFEmpLvl : 1;           /*!< [7..7] Non-Periodic TxFIFO Empty Level                                    */
      __IOM uint32_t PTxFEmpLvl : 1;            /*!< [8..8] Periodic TxFIFO Empty Level                                        */
            uint32_t            : 12;
      __IOM uint32_t RemMemSupp : 1;            /*!< [21..21] Remote Memory Support                                            */
      __IOM uint32_t NotiAllDmaWrit : 1;        /*!< [22..22] Notify All DMA Write Transactions                                */
      __IOM uint32_t AHBSingle  : 1;            /*!< [23..23] AHB Single Support                                               */
      __IOM uint32_t InvDescEndianess : 1;      /*!< [24..24] Invert Descriptor Endianess                                      */
            uint32_t            : 7;
    } GAHBCFG_b;
  } ;
  
  union {
    __IOM uint32_t GUSBCFG;                     /*!< (@ 0x0000000C) USB Configuration Register                                 */
    
    struct {
      __IOM uint32_t TOutCal    : 3;            /*!< [2..0] HS/FS Timeout Calibration                                          */
      __IOM uint32_t PHYIf      : 1;            /*!< [3..3] PHY Interface                                                      */
      __IOM uint32_t ULPI_UTMI_Sel : 1;         /*!< [4..4] ULPI or UTMI+ Select                                               */
      __IOM uint32_t FSIntf     : 1;            /*!< [5..5] Full-Speed Serial Interface Select                                 */
      __IOM uint32_t PHYSel     : 1;            /*!< [6..6] PHY Selection                                                      */
      __IOM uint32_t DDRSel     : 1;            /*!< [7..7] ULPI DDR Select                                                    */
      __IOM uint32_t SRPCap     : 1;            /*!< [8..8] SRP-Capable                                                        */
      __IOM uint32_t HNPCap     : 1;            /*!< [9..9] HNP-Capable                                                        */
      __IOM uint32_t USBTrdTim  : 4;            /*!< [13..10] USB Turnaround Time                                              */
            uint32_t            : 1;
      __IOM uint32_t PhyLPwrClkSel : 1;         /*!< [15..15] PHY Low-Power Clock Select                                       */
      __IOM uint32_t OtgI2CSel  : 1;            /*!< [16..16] UTMIFS or I2C Interface Select                                   */
      __IOM uint32_t ULPIFsLs   : 1;            /*!< [17..17] ULPI FS/LS Select                                                */
      __IOM uint32_t ULPIAutoRes : 1;           /*!< [18..18] ULPI Auto Resume                                                 */
      __IOM uint32_t ULPIClkSusM : 1;           /*!< [19..19] ULPI Clock SuspendM                                              */
      __IOM uint32_t ULPIExtVbusDrv : 1;        /*!< [20..20] ULPI External VBUS Drive                                         */
      __IOM uint32_t ULPIExtVbusIndicator : 1;  /*!< [21..21] ULPI External VBUS Indicator                                     */
      __IOM uint32_t TermSelDLPulse : 1;        /*!< [22..22] TermSel DLine Pulsing Selection                                  */
      __IOM uint32_t Complement : 1;            /*!< [23..23] Indicator Complement Controls                                    */
      __IOM uint32_t Indicator  : 1;            /*!< [24..24] Indicator Pass Through                                           */
      __IOM uint32_t ULPI       : 1;            /*!< [25..25] ULPI Interface Protect Disable                                   */
      __IOM uint32_t IC_USBCap  : 1;            /*!< [26..26] IC_USB-Capable PHY Interface Selection                           */
      __IOM uint32_t IC_USBTrafCtl : 1;         /*!< [27..27] IC_USB-TrafficPullRemove Control                                 */
      __IOM uint32_t TxEndDelay : 1;            /*!< [28..28] Tx End Delay                                                     */
      __IOM uint32_t ForceHstMode : 1;          /*!< [29..29] Force Host Mode                                                  */
      __IOM uint32_t ForceDevMode : 1;          /*!< [30..30] Force Device Mode                                                */
      __IOM uint32_t CorruptTxPkt : 1;          /*!< [31..31] Corrupt Tx packet                                                */
    } GUSBCFG_b;
  } ;
  
  union {
    __IOM uint32_t GRSTCTL;                     /*!< (@ 0x00000010) Reset Register                                             */
    
    struct {
      __IOM uint32_t CSftRst    : 1;            /*!< [0..0] Core Soft Reset                                                    */
      __IOM uint32_t PIUFSSftRst : 1;           /*!< [1..1] PIU FS Soft Reset                                                  */
      __IOM uint32_t FrmCntrRst : 1;            /*!< [2..2] Host Frame Counter Reset                                           */
      __IOM uint32_t INTknQFlsh : 1;            /*!< [3..3] IN Token Queue Flush                                               */
      __IOM uint32_t RxFFlsh    : 1;            /*!< [4..4] RxFIFO Flush                                                       */
      __IOM uint32_t TxFFlsh    : 1;            /*!< [5..5] TxFIFO Flush                                                       */
      __IOM uint32_t TxFNum     : 5;            /*!< [10..6] TxFIFO Number                                                     */
            uint32_t            : 18;
      __IOM uint32_t CSftRstDone : 1;           /*!< [29..29] Core Soft Reset Done                                             */
      __IM  uint32_t DMAReq     : 1;            /*!< [30..30] DMA Request Signal                                               */
      __IM  uint32_t AHBIdle    : 1;            /*!< [31..31] AHB Master Idle                                                  */
    } GRSTCTL_b;
  } ;
  
  union {
    __IOM uint32_t GINTSTS;                     /*!< (@ 0x00000014) Interrupt Register                                         */
    
    struct {
      __IM  uint32_t CurMod     : 1;            /*!< [0..0] Current Mode of Operation                                          */
      __IOM uint32_t ModeMis    : 1;            /*!< [1..1] Mode Mismatch Interrupt                                            */
      __IM  uint32_t OTGInt     : 1;            /*!< [2..2] OTG Interrupt                                                      */
      __IOM uint32_t Sof        : 1;            /*!< [3..3] Start of (micro)Frame                                              */
      __IM  uint32_t RxFLvl     : 1;            /*!< [4..4] RxFIFO Non-Empty                                                   */
      __IM  uint32_t NPTxFEmp   : 1;            /*!< [5..5] Non-periodic TxFIFO Empty                                          */
      __IM  uint32_t GINNakEff  : 1;            /*!< [6..6] Global IN Non-periodic NAK Effective                               */
      __IM  uint32_t GOUTNakEff : 1;            /*!< [7..7] Global OUT NAK Effective                                           */
      __IOM uint32_t ULPICKINT  : 1;            /*!< [8..8] ULPI Carkit Interrupt                                              */
      __IOM uint32_t I2CINT     : 1;            /*!< [9..9] I2C Interrupt                                                      */
      __IOM uint32_t ErlySusp   : 1;            /*!< [10..10] Early Suspend                                                    */
      __IOM uint32_t USBSusp    : 1;            /*!< [11..11] USB Suspend                                                      */
      __IOM uint32_t USBRst     : 1;            /*!< [12..12] USB Reset                                                        */
      __IOM uint32_t EnumDone   : 1;            /*!< [13..13] Enumeration Done                                                 */
      __IOM uint32_t ISOOutDrop : 1;            /*!< [14..14] Isochronous OUT Packet Dropped Interrupt                         */
      __IOM uint32_t EOPF       : 1;            /*!< [15..15] End of Periodic Frame Interrupt                                  */
      __IOM uint32_t RstrDoneInt : 1;           /*!< [16..16] Restore Done Interrupt                                           */
      __IOM uint32_t EPMis      : 1;            /*!< [17..17] Endpoint Mismatch Interrupt                                      */
      __IM  uint32_t IEPInt     : 1;            /*!< [18..18] IN Endpoints Interrupt                                           */
      __IM  uint32_t OEPInt     : 1;            /*!< [19..19] OUT Endpoints Interrupt                                          */
      __IOM uint32_t incompISOIN : 1;           /*!< [20..20] Incomplete Isochronous IN Transfer                               */
      __IOM uint32_t incomplP   : 1;            /*!< [21..21] Incomplete Periodic Transfer                                     */
      __IOM uint32_t FetSusp    : 1;            /*!< [22..22] Data Fetch Suspended                                             */
      __IOM uint32_t ResetDet   : 1;            /*!< [23..23] Reset Detected Interrupt                                         */
      __IM  uint32_t PrtInt     : 1;            /*!< [24..24] Host Port Interrupt                                              */
      __IM  uint32_t HChInt     : 1;            /*!< [25..25] Host Channels Interrupt                                          */
      __IM  uint32_t PTxFEmp    : 1;            /*!< [26..26] Periodic TxFIFO Empty                                            */
      __IOM uint32_t LPM_Int    : 1;            /*!< [27..27] LPM Transaction Received Interrupt                               */
      __IOM uint32_t ConIDStsChng : 1;          /*!< [28..28] Connector ID Status Change                                       */
      __IOM uint32_t DisconnInt : 1;            /*!< [29..29] Disconnect Detected Interrupt                                    */
      __IOM uint32_t SessReqInt : 1;            /*!< [30..30] Session Request/New Session Detected Interrupt                   */
      __IOM uint32_t WkUpInt    : 1;            /*!< [31..31] Resume/Remote Wakeup Detected Interrupt                          */
    } GINTSTS_b;
  } ;
  
  union {
    __IOM uint32_t GINTMSK;                     /*!< (@ 0x00000018) Interrupt Mask Register                                    */
    
    struct {
            uint32_t            : 1;
      __IOM uint32_t ModeMisMsk : 1;            /*!< [1..1] Mode Mismatch Interrupt Mask                                       */
      __IOM uint32_t OTGIntMsk  : 1;            /*!< [2..2] OTG Interrupt Mask                                                 */
      __IOM uint32_t SofMsk     : 1;            /*!< [3..3] Start of Frame Mask                                                */
      __IOM uint32_t RxFLvlMsk  : 1;            /*!< [4..4] Receive FIFO Non-Empty Mask                                        */
      __IOM uint32_t NPTxFEmpMsk : 1;           /*!< [5..5] Non-periodic TxFIFO Empty Mask                                     */
      __IOM uint32_t GINNakEffMsk : 1;          /*!< [6..6] Global Non-periodic IN NAK Effective Mask, Device only             */
      __IOM uint32_t GOUTNakEffMsk : 1;         /*!< [7..7] Global OUT NAK Effective Mask, Device only                         */
      __IOM uint32_t ULPICKINTMsk : 1;          /*!< [8..8] ULPI Carkit Interrupt Mask                                         */
      __IOM uint32_t I2CIntMsk  : 1;            /*!< [9..9] I2C Interrupt Mask                                                 */
      __IOM uint32_t ErlySuspMsk : 1;           /*!< [10..10] Early Suspend Mask, Device only                                  */
      __IOM uint32_t USBSuspMsk : 1;            /*!< [11..11] USB Suspend Mask, Device only                                    */
      __IOM uint32_t USBRstMsk  : 1;            /*!< [12..12] USB Reset Mask, Device only                                      */
      __IOM uint32_t EnumDoneMsk : 1;           /*!< [13..13] Enumeration Done Mask, Device only                               */
      __IOM uint32_t ISOOutDropMsk : 1;         /*!< [14..14] Isochronous OUT Packet Dropped Interrupt Mask, Device
                                                     only                                                                      */
      __IOM uint32_t EOPFMsk    : 1;            /*!< [15..15] End of Periodic Frame Interrupt Mask, Device only                */
      __IOM uint32_t RstrDoneIntMsk : 1;        /*!< [16..16] Restore Done Interrupt Mask                                      */
      __IOM uint32_t EPMisMsk   : 1;            /*!< [17..17] Endpoint Mismatch Interrupt Mask, Device only                    */
      __IOM uint32_t IEPIntMsk  : 1;            /*!< [18..18] IN Endpoints Interrupt Mask, Device only                         */
      __IOM uint32_t OEPIntMsk  : 1;            /*!< [19..19] OUT Endpoints Interrupt Mask, Device only                        */
      __IOM uint32_t incompISOINMsk : 1;        /*!< [20..20] Incomplete Isochronous IN Transfer Mask, Device only             */
      __IOM uint32_t incomplPMsK : 1;           /*!< [21..21] Incomplete Periodic Transfer Mask, Host only                     */
      __IOM uint32_t FetSuspMsk : 1;            /*!< [22..22] Data Fetch Suspended Mask, Device only                           */
      __IOM uint32_t ResetDetMsk : 1;           /*!< [23..23] Reset Detected Interrupt Mask, Device only                       */
      __IOM uint32_t PrtIntMsk  : 1;            /*!< [24..24] Host Port Interrupt Mask, Host only                              */
      __IOM uint32_t HChIntMsk  : 1;            /*!< [25..25] Host Channels Interrupt Mask, Host only                          */
      __IOM uint32_t PTxFEmpMsk : 1;            /*!< [26..26] Periodic TxFIFO Empty Mask, Host only                            */
      __IOM uint32_t LPM_IntMsk : 1;            /*!< [27..27] LPM Transaction Received Interrupt Mask                          */
      __IOM uint32_t ConIDStsChngMsk : 1;       /*!< [28..28] Connector ID Status Change Mask                                  */
      __IOM uint32_t DisconnIntMsk : 1;         /*!< [29..29] Disconnect Detected Interrupt Mask                               */
      __IOM uint32_t SessReqIntMsk : 1;         /*!< [30..30] Session Request/New Session Detected Interrupt Mask              */
      __IOM uint32_t WkUpIntMsk : 1;            /*!< [31..31] Resume/Remote Wakeup Detected Interrupt Mask                     */
    } GINTMSK_b;
  } ;
  
  union {
    __IM  uint32_t GRXSTSR;                     /*!< (@ 0x0000001C) Receive Status Debug Read Register                         */
    
    struct {
      __IM  uint32_t ChNum      : 4;            /*!< [3..0] Channel Number / Endpoint Number                                   */
      __IM  uint32_t BCnt       : 11;           /*!< [14..4] Byte Count                                                        */
      __IM  uint32_t DPID       : 2;            /*!< [16..15] Data PID                                                         */
      __IM  uint32_t PktSts     : 4;            /*!< [20..17] Packet Status                                                    */
      __IM  uint32_t FN         : 4;            /*!< [24..21] Frame Number (Device only)                                       */
            uint32_t            : 7;
    } GRXSTSR_b;
  } ;
  
  union {
    __IM  uint32_t GRXSTSP;                     /*!< (@ 0x00000020) Receive Status Read/Pop Register                           */
    
    struct {
      __IM  uint32_t ChNum      : 4;            /*!< [3..0] Channel Number / Endpoint Number                                   */
      __IM  uint32_t BCnt       : 11;           /*!< [14..4] Byte Count                                                        */
      __IM  uint32_t DPID       : 2;            /*!< [16..15] Data PID                                                         */
      __IM  uint32_t PktSts     : 4;            /*!< [20..17] Packet Status                                                    */
      __IM  uint32_t FN         : 4;            /*!< [24..21] Frame Number (Device only)                                       */
            uint32_t            : 7;
    } GRXSTSP_b;
  } ;
  __IOM uint32_t  GRXFSIZ;                      /*!< (@ 0x00000024) Receive FIFO Size Register                                 */
  
  union {
    __IOM uint32_t GNPTXFSIZ;                   /*!< (@ 0x00000028) Non-periodic Transmit FIFO Size Register                   */
    
    struct {
      __IOM uint32_t NPTxFStAddr : 11;          /*!< [10..0] Non-periodic Transmit RAM Start Address                           */
            uint32_t            : 5;
      __IOM uint32_t NPTxFDep   : 11;           /*!< [26..16] Non-periodic TxFIFO Depth                                        */
            uint32_t            : 5;
    } GNPTXFSIZ_b;
  } ;
  
  union {
    __IM  uint32_t GNPTXSTS;                    /*!< (@ 0x0000002C) Non-periodic Transmit FIFO/Queue Status Register           */
    
    struct {
      __IM  uint32_t NPTxFSpcAvail : 16;        /*!< [15..0] Non-periodic TxFIFO Space Available                               */
      __IM  uint32_t NPTxQSpcAvail : 8;         /*!< [23..16] Non-periodic Transmit Request Queue Space Available              */
      __IM  uint32_t NPTxQTop   : 7;            /*!< [30..24] Top of the Non-periodic Transmit Request Queue                   */
            uint32_t            : 1;
    } GNPTXSTS_b;
  } ;
  __IOM uint32_t  GI2CCTL;                      /*!< (@ 0x00000030) I2C Access Register                                        */
  
  union {
    __IOM uint32_t GPVNDCTL;                    /*!< (@ 0x00000034) PHY Vendor Control Register                                */
    
    struct {
      __IOM uint32_t RegData    : 8;            /*!< [7..0] Register Data                                                      */
      __IOM uint32_t VCtrl      : 8;            /*!< [15..8] UTMI+ Vendor Control Register Address                             */
      __IOM uint32_t RegAddr    : 6;            /*!< [21..16] Register Address                                                 */
      __IOM uint32_t RegWr      : 1;            /*!< [22..22] Register Write                                                   */
            uint32_t            : 2;
      __IOM uint32_t NewRegReq  : 1;            /*!< [25..25] New Register Request                                             */
      __IM  uint32_t VStsBsy    : 1;            /*!< [26..26] VStatus Busy                                                     */
      __IOM uint32_t VStsDone   : 1;            /*!< [27..27] VStatus Done                                                     */
            uint32_t            : 3;
      __IOM uint32_t DisUlpiDrvr : 1;           /*!< [31..31] Disable ULPI Drivers                                             */
    } GPVNDCTL_b;
  } ;
  __IOM uint32_t  GGPIO;                        /*!< (@ 0x00000038) General Purpose Input/Output Register                      */
  __IOM uint32_t  GUID;                         /*!< (@ 0x0000003C) User ID Register                                           */
  __IM  uint32_t  GSNPSID;                      /*!< (@ 0x00000040) Synopsys ID Register                                       */
  __IM  uint32_t  GHWCFG1;                      /*!< (@ 0x00000044) User Hardware Configuration 1 Register                     */
  
  union {
    __IM  uint32_t GHWCFG2;                     /*!< (@ 0x00000048) User Hardware Configuration 2 Register                     */
    
    struct {
      __IM  uint32_t OtgMode    : 3;            /*!< [2..0] Mode of Operation                                                  */
      __IM  uint32_t OtgArch    : 2;            /*!< [4..3] Architecture                                                       */
      __IM  uint32_t SingPnt    : 1;            /*!< [5..5] Point-to-Point                                                     */
      __IM  uint32_t HSPhyType  : 2;            /*!< [7..6] High-Speed PHY Interface Type                                      */
      __IM  uint32_t FSPhyType  : 2;            /*!< [9..8] Full-Speed PHY Interface Type                                      */
      __IM  uint32_t NumDevEps  : 4;            /*!< [13..10] Number of Device Endpoints                                       */
      __IM  uint32_t NumHstChnl : 4;            /*!< [17..14] Number of Host Channels                                          */
      __IM  uint32_t PerioSupport : 1;          /*!< [18..18] Periodic OUT Channels Supported in Host Mode                     */
      __IM  uint32_t DynFifoSizing : 1;         /*!< [19..19] Dynamic FIFO Sizing Enabled                                      */
      __IM  uint32_t MultiProcIntrpt : 1;       /*!< [20..20] Multi Processor Interrupt Enabled                                */
            uint32_t            : 1;
      __IM  uint32_t NPTxQDepth : 2;            /*!< [23..22] Non-periodic Request Queue Depth                                 */
      __IM  uint32_t PTxQDepth  : 2;            /*!< [25..24] Host Mode Periodic Request Queue Depth                           */
      __IM  uint32_t TknQDepth  : 5;            /*!< [30..26] Device Mode IN Token Sequence Learning Queue Depth               */
      __IM  uint32_t OTG_ENABLE_IC_USB : 1;     /*!< [31..31] IC_USB mode specified for mode of operation                      */
    } GHWCFG2_b;
  } ;
  
  union {
    __IM  uint32_t GHWCFG3;                     /*!< (@ 0x0000004C) User Hardware Configuration 3 Register                     */
    
    struct {
      __IM  uint32_t XferSizeWidth : 4;         /*!< [3..0] Width of Transfer Size Counters                                    */
      __IM  uint32_t PktSizeWidth : 3;          /*!< [6..4] Width of Packet Size Counters                                      */
      __IM  uint32_t OtgEn      : 1;            /*!< [7..7] OTG Function Enabled                                               */
      __IM  uint32_t I2CIntSel  : 1;            /*!< [8..8] I2C Interface Selection                                            */
      __IM  uint32_t VndctlSupt : 1;            /*!< [9..9] Vendor Control Interface Support                                   */
      __IM  uint32_t OptFeature : 1;            /*!< [10..10] Optional Features Removed                                        */
      __IM  uint32_t RstType    : 1;            /*!< [11..11] Reset Style for Clocked Blocks                                   */
      __IM  uint32_t ADPSupport : 1;            /*!< [12..12] ADP Logic Support                                                */
      __IM  uint32_t HSICMode   : 1;            /*!< [13..13] HSIC Mode                                                        */
      __IM  uint32_t BCSupport  : 1;            /*!< [14..14] Battery Charger Support                                          */
      __IM  uint32_t LPMMode    : 1;            /*!< [15..15] LPM Mode                                                         */
      __IM  uint32_t DfifoDepth : 16;           /*!< [31..16] DFIFO Depth                                                      */
    } GHWCFG3_b;
  } ;
  
  union {
    __IM  uint32_t GHWCFG4;                     /*!< (@ 0x00000050) User Hardware Configuration 4 Register                     */
    
    struct {
      __IM  uint32_t NumDevPerioEps : 4;        /*!< [3..0] Number of Device Mode Periodic IN Endpoints                        */
      __IM  uint32_t PartialPwrDn : 1;          /*!< [4..4] Enable Partial Power Down                                          */
      __IM  uint32_t AhbFreq    : 1;            /*!< [5..5] Minimum AHB Frequency Less Than 60 MHz                             */
      __IM  uint32_t Hibernation : 1;           /*!< [6..6] Enable Hibernation                                                 */
      __IM  uint32_t ExtendedHibernation : 1;   /*!< [7..7] Enable Hibernation                                                 */
            uint32_t            : 1;
      __IM  uint32_t EnhancedLPMSupt1 : 1;      /*!< [9..9] Enhanced LPM Support1                                              */
      __IM  uint32_t ServIntFlow : 1;           /*!< [10..10] Service Interval Flow                                            */
      __IM  uint32_t ipgisocSupt : 1;           /*!< [11..11] Interpacket Gap ISOC OUT Worst-case Support                      */
      __IM  uint32_t ACGSupt    : 1;            /*!< [12..12] Active Clock Gating Support                                      */
      __IM  uint32_t EnhancedLPMSupt : 1;       /*!< [13..13] Enhanced LPM Support                                             */
      __IM  uint32_t PhyDataWidth : 2;          /*!< [15..14] UTMI+ PHY/ULPI-to-Internal UTMI+ Wrapper Data Width              */
      __IM  uint32_t NumCtlEps  : 4;            /*!< [19..16] Number of Device Mode Control Endpoints in Addition
                                                     to Endpoint 0                                                             */
      __IM  uint32_t IddgFltr   : 1;            /*!< [20..20] IDDIG Filter Enabled                                             */
      __IM  uint32_t VBusValidFltr : 1;         /*!< [21..21] VBUS Valid Filter Enabled                                        */
      __IM  uint32_t AValidFltr : 1;            /*!< [22..22] a_valid filter enabled                                           */
      __IM  uint32_t BValidFltr : 1;            /*!< [23..23] b_valid filter enabled                                           */
      __IM  uint32_t SessEndFltr : 1;           /*!< [24..24] Session end filter enabled                                       */
      __IM  uint32_t DedFifoMode : 1;           /*!< [25..25] Enable Dedicated Transmit FIFO for device IN Endpoints           */
      __IM  uint32_t INEps      : 4;            /*!< [29..26] Number of Device Mode IN Endpoints Including Control
                                                     Endpoints                                                                 */
      __IM  uint32_t DescDMAEnabled : 1;        /*!< [30..30] Scatter/Gather DMA configuration enabled                         */
      __IM  uint32_t DescDMA    : 1;            /*!< [31..31] Scatter/Gather DMA configuration                                 */
    } GHWCFG4_b;
  } ;
  
  union {
    __IOM uint32_t GLPMCFG;                     /*!< (@ 0x00000054) LPM Config Register                                        */
    
    struct {
      __IOM uint32_t LPMCap     : 1;            /*!< [0..0] LPM-Capable                                                        */
      __IOM uint32_t AppL1Res   : 1;            /*!< [1..1] LPM response programmed by application                             */
      __IOM uint32_t HIRD       : 4;            /*!< [5..2] Host-Initiated Resume Duration                                     */
      __IOM uint32_t bRemoteWake : 1;           /*!< [6..6] RemoteWakeEnable                                                   */
      __IOM uint32_t EnblSlpM   : 1;            /*!< [7..7] Enable utmi_sleep_n                                                */
      __IOM uint32_t HIRD_Thres : 5;            /*!< [12..8] BESL/HIRD Threshold                                               */
      __IOM uint32_t CoreL1Res  : 2;            /*!< [14..13] LPM Response                                                     */
      __IOM uint32_t SlpSts     : 1;            /*!< [15..15] Port Sleep Status                                                */
      __IOM uint32_t L1ResumeOK : 1;            /*!< [16..16] Sleep State Resume OK                                            */
      __IOM uint32_t LPM_Chnl_Indx : 4;         /*!< [20..17] LPM Channel Index                                                */
      __IOM uint32_t LPM_Retry_Cnt : 3;         /*!< [23..21] LPM Retry Count                                                  */
      __IOM uint32_t SndLPM     : 1;            /*!< [24..24] Send LPM Transaction                                             */
      __IM  uint32_t LPM_RetryCnt_Sts : 3;      /*!< [27..25] LPM Retry Count Status                                           */
      __IOM uint32_t LPM_EnBESL : 1;            /*!< [28..28] LPM Enable BESL                                                  */
      __IOM uint32_t LPM_RestoreSlpSts : 1;     /*!< [29..29] LPM Restore Sleep Status                                         */
      __IOM uint32_t HSICCon    : 1;            /*!< [30..30] HSIC-Connect                                                     */
      __IOM uint32_t InvSelHsic : 1;            /*!< [31..31] HSIC-Invert Select HSIC                                          */
    } GLPMCFG_b;
  } ;
  
  union {
    __IOM uint32_t GPWRDN;                      /*!< (@ 0x00000058) Global Power Down Register                                 */
    
    struct {
      __IOM uint32_t PMUIntSel  : 1;            /*!< [0..0] PMU Interrupt Select                                               */
      __IOM uint32_t PMUActv    : 1;            /*!< [1..1] PMU Active                                                         */
      __IOM uint32_t Restore    : 1;            /*!< [2..2] Restore.                                                           */
      __IOM uint32_t PwrDnClmp  : 1;            /*!< [3..3] Power Down Clamp                                                   */
      __IOM uint32_t PwrDnRst_n : 1;            /*!< [4..4] Power Down ResetN                                                  */
      __IOM uint32_t PwrDnSwtch : 1;            /*!< [5..5] Power Down Switch                                                  */
      __IOM uint32_t DisableVBUS : 1;           /*!< [6..6] Disable VBUS                                                       */
      __IOM uint32_t LnStsChng  : 1;            /*!< [7..7] Line State Change                                                  */
      __IOM uint32_t LineStageChangeMsk : 1;    /*!< [8..8] Line State Change Mask                                             */
      __IOM uint32_t ResetDetected : 1;         /*!< [9..9] Reset Detected                                                     */
      __IOM uint32_t ResetDetMsk : 1;           /*!< [10..10] Reset Detect Mask                                                */
      __IOM uint32_t DisconnectDetect : 1;      /*!< [11..11] Disconnect Detect                                                */
      __IOM uint32_t DisconnectDetectMsk : 1;   /*!< [12..12] Disconnect Detect Mask                                           */
      __IOM uint32_t ConnectDet : 1;            /*!< [13..13] Connect Detect                                                   */
      __IOM uint32_t ConnDetMsk : 1;            /*!< [14..14] Connect Detect Mask                                              */
      __IOM uint32_t SRPDetect  : 1;            /*!< [15..15] SRP Detect                                                       */
      __IOM uint32_t SRPDetectMsk : 1;          /*!< [16..16] SRP Detect Mask                                                  */
      __IOM uint32_t StsChngInt : 1;            /*!< [17..17] Status Change Interrupt                                          */
      __IOM uint32_t StsChngIntMsk : 1;         /*!< [18..18] Status Change Interrupt Mask                                     */
      __IM  uint32_t LineState  : 2;            /*!< [20..19] LineState.                                                       */
      __IM  uint32_t IDDIG      : 1;            /*!< [21..21] Status of signal IDDIG                                           */
      __IM  uint32_t BSessVld   : 1;            /*!< [22..22] B Session Valid                                                  */
      __IOM uint32_t ADPInt     : 1;            /*!< [23..23] ADP Interrupt                                                    */
      __IOM uint32_t MultValIdBC : 5;           /*!< [28..24] MultValIdBC                                                      */
            uint32_t            : 3;
    } GPWRDN_b;
  } ;
  
  union {
    __IOM uint32_t GDFIFOCFG;                   /*!< (@ 0x0000005C) Global DFIFO Configuration Register                        */
    
    struct {
      __IOM uint32_t GDFIFOCfg  : 16;           /*!< [15..0] GDFIFO Configuration                                              */
      __IOM uint32_t EPInfoBaseAddr : 16;       /*!< [31..16] Start address of the EP info controller                          */
    } GDFIFOCFG_b;
  } ;
  
  union {
    __IOM uint32_t GADPCTL;                     /*!< (@ 0x00000060) ADP Timer, Control and Status Register                     */
    
    struct {
      __IOM uint32_t PrbDschg   : 2;            /*!< [1..0] Probe Discharge                                                    */
      __IOM uint32_t PrbDelta   : 2;            /*!< [3..2] Probe Delta                                                        */
      __IOM uint32_t PrbPer     : 2;            /*!< [5..4] Probe Period                                                       */
      __IM  uint32_t RTIM       : 11;           /*!< [16..6] RAMP TIME                                                         */
      __IOM uint32_t EnaPrb     : 1;            /*!< [17..17] Enable Probe                                                     */
      __IOM uint32_t EnaSns     : 1;            /*!< [18..18] Enable Sense                                                     */
      __IOM uint32_t ADPRes     : 1;            /*!< [19..19] ADP Reset                                                        */
      __IOM uint32_t ADPEn      : 1;            /*!< [20..20] ADP Enable                                                       */
      __IOM uint32_t AdpPrbInt  : 1;            /*!< [21..21] ADP Probe Interrupt                                              */
      __IOM uint32_t AdpSnsInt  : 1;            /*!< [22..22] ADP Sense Interrupt                                              */
      __IOM uint32_t AdpToutInt : 1;            /*!< [23..23] ADP Timeout Interrupt                                            */
      __IOM uint32_t AdpPrbIntMsk : 1;          /*!< [24..24] ADP Probe Interrupt Mask                                         */
      __IOM uint32_t AdpSnsIntMsk : 1;          /*!< [25..25] ADP Sense Interrupt Mask                                         */
      __IOM uint32_t AdpToutMsk : 1;            /*!< [26..26] ADP Timeout Interrupt Mask                                       */
      __IOM uint32_t AR         : 2;            /*!< [28..27] Access Request                                                   */
            uint32_t            : 3;
    } GADPCTL_b;
  } ;
  
  union {
    __IOM uint32_t GREFCLK;                     /*!< (@ 0x00000064) ref_clk Control Register                                   */
    
    struct {
      __IOM uint32_t SOF_CNT_WKUP_ALERT : 10;   /*!< [9..0] Number of SOF's for wakeup alert                                   */
            uint32_t            : 4;
      __IOM uint32_t RefclkMode : 1;            /*!< [14..14] Refclk Mode of Operation                                         */
      __IOM uint32_t REFCLKPER  : 17;           /*!< [31..15] Period of ref_clk in terms of pico seconds                       */
    } GREFCLK_b;
  } ;
  
  union {
    __IOM uint32_t GINTMSK2;                    /*!< (@ 0x00000068) Interrupt Mask Register 2                                  */
    
    struct {
      __IOM uint32_t WkUpAlertIntMsk : 1;       /*!< [0..0] Remote WakeUp Alert Interrupt Mask                                 */
            uint32_t            : 31;
    } GINTMSK2_b;
  } ;
  
  union {
    __IOM uint32_t GINTSTS2;                    /*!< (@ 0x0000006C) Interrupt Register 2                                       */
    
    struct {
      __IOM uint32_t WkUpAlertInt : 1;          /*!< [0..0] Remote WakeUp Alert Interrupt                                      */
            uint32_t            : 31;
    } GINTSTS2_b;
  } ;
  __IM  uint32_t  RESERVED[36];
  __IOM uint32_t  HPTXFSIZ;                     /*!< (@ 0x00000100) Host Periodic Transmit FIFO Size Register                  */
  
  union {
    __IOM uint32_t DIEPTXF[15];                 /*!< (@ 0x00000104) Device IN Endpoint Transmit FIFO Size Register             */
    
    struct {
      __IOM uint32_t INEPnTxFStAddr : 11;       /*!< [10..0] IN Endpoint FIFOn Transmit RAM Start Address                      */
            uint32_t            : 5;
      __IOM uint32_t INEPnTxFDep : 11;          /*!< [26..16] IN Endpoint TxFIFO Depth (INEPnTxFDep)                           */
            uint32_t            : 5;
    } DIEPTXF_b[15];
  } ;
} USB_OTG_CORE_Type;                            /*!< Size = 320 (0x140)                                                        */



/* =========================================================================================================================== */
/* ================                                         USB_HOST1                                         ================ */
/* =========================================================================================================================== */


/**
  * @brief USB OTG Host Controller 1 (USB_HOST1)
  */

typedef struct {                                /*!< (@ 0x50000400) USB_HOST1 Structure                                        */
  __IOM uint32_t  HCFG;                         /*!< (@ 0x00000000) Host Configuration Register                                */
  __IOM uint32_t  HFIR;                         /*!< (@ 0x00000004) Host Frame Interval Register                               */
  __IM  uint32_t  HFNUM;                        /*!< (@ 0x00000008) Host Frame Number/Frame Time Remaining Register            */
  __IM  uint32_t  RESERVED;
  __IM  uint32_t  HPTXSTS;                      /*!< (@ 0x00000010) Host Periodic Transmit FIFO/Queue Status Register          */
  __IM  uint32_t  HAINT;                        /*!< (@ 0x00000014) Host All Channels Interrupt Register                       */
  __IOM uint32_t  HAINTMSK;                     /*!< (@ 0x00000018) Host All Channels Interrupt Mask Register                  */
  __IOM uint32_t  HFLBAddr;                     /*!< (@ 0x0000001C) Host Frame List Base Address Register                      */
  __IM  uint32_t  RESERVED1[8];
  __IOM uint32_t  HPRT;                         /*!< (@ 0x00000040) Host Port Control and Status Register                      */
  __IM  uint32_t  RESERVED2[47];
  __IOM uint32_t  HCCHAR0;                      /*!< (@ 0x00000100) Host Channel 0 Characteristics Register                    */
  __IOM uint32_t  HCSPLT0;                      /*!< (@ 0x00000104) Host Channel 0 Split Control Register                      */
  __IOM uint32_t  HCINT0;                       /*!< (@ 0x00000108) Host Channel 0 Interrupt Register                          */
  __IOM uint32_t  HCINTMSK0;                    /*!< (@ 0x0000010C) Host Channel 0 Interrupt Mask Register                     */
  __IOM uint32_t  HCTSIZ0;                      /*!< (@ 0x00000110) Host Channel 0 Transfer Size Register                      */
  __IM  uint32_t  RESERVED3[3];
  __IOM uint32_t  HCCHAR1;                      /*!< (@ 0x00000120) Host Channel 1 Characteristics Register                    */
  __IOM uint32_t  HCSPLT1;                      /*!< (@ 0x00000124) Host Channel 1 Split Control Register                      */
  __IOM uint32_t  HCINT1;                       /*!< (@ 0x00000128) Host Channel 1 Interrupt Register                          */
  __IOM uint32_t  HCINTMSK1;                    /*!< (@ 0x0000012C) Host Channel 1 Interrupt Mask Register                     */
  __IOM uint32_t  HCTSIZ1;                      /*!< (@ 0x00000130) Host Channel 1 Transfer Size Register                      */
  __IM  uint32_t  RESERVED4[3];
  __IOM uint32_t  HCCHAR2;                      /*!< (@ 0x00000140) Host Channel 2 Characteristics Register                    */
  __IOM uint32_t  HCSPLT2;                      /*!< (@ 0x00000144) Host Channel 2 Split Control Register                      */
  __IOM uint32_t  HCINT2;                       /*!< (@ 0x00000148) Host Channel 2 Interrupt Register                          */
  __IOM uint32_t  HCINTMSK2;                    /*!< (@ 0x0000014C) Host Channel 2 Interrupt Mask Register                     */
  __IOM uint32_t  HCTSIZ2;                      /*!< (@ 0x00000150) Host Channel 2 Transfer Size Register                      */
  __IM  uint32_t  RESERVED5[3];
  __IOM uint32_t  HCCHAR3;                      /*!< (@ 0x00000160) Host Channel 3 Characteristics Register                    */
  __IOM uint32_t  HCSPLT3;                      /*!< (@ 0x00000164) Host Channel 3 Split Control Register                      */
  __IOM uint32_t  HCINT3;                       /*!< (@ 0x00000168) Host Channel 3 Interrupt Register                          */
  __IOM uint32_t  HCINTMSK3;                    /*!< (@ 0x0000016C) Host Channel 3 Interrupt Mask Register                     */
  __IOM uint32_t  HCTSIZ3;                      /*!< (@ 0x00000170) Host Channel 3 Transfer Size Register                      */
} USB_OTG_HOST_Type;                            /*!< Size = 372 (0x174)                                                        */



/* =========================================================================================================================== */
/* ================                                        USB_DEVICE1                                        ================ */
/* =========================================================================================================================== */


/**
  * @brief USB OTG Device Controller 1 (USB_DEVICE1)
  */

typedef struct {                                /*!< (@ 0x50000800) USB_DEVICE1 Structure                                      */
  
  union {
    __IOM uint32_t DCFG;                        /*!< (@ 0x00000000) Device Configuration Register                              */
    
    struct {
      __IOM uint32_t DevSpd     : 2;            /*!< [1..0] Device Speed for enumeration. 0x0: High speed (USB 2.0,
                                                     30/60MHz) 0x1: Full speed (USB 2.0, 30/60MHz) 0x2: Low
                                                     speed (USB 1.1, 6MHz) 0x3: Full speed (USB 1.1, 48MHz)
                                                     Value after reset: 0x0                                                    */
      __IOM uint32_t NZStsOUTHShk : 1;          /*!< [2..2] Non-Zero-Length Status OUT Handshake on receiving status
                                                     OUT. 0: Send PKT to app, handshake follows endpoint config
                                                     1: STALL on NZL status OUT, do not send to app Value after
                                                     reset: 0x0                                                                */
      __IOM uint32_t Ena32KHzSusp : 1;          /*!< [3..3] Enable 32KHz Suspend mode, only valid for FS PHY. Value
                                                     after reset: 0x0                                                          */
      __IOM uint32_t DevAddr    : 7;            /*!< [10..4] Device address. Must be set after SetAddress command.
                                                     Value after reset: 0x0                                                    */
      __IOM uint32_t PerFrInt   : 2;            /*!< [12..11] Periodic Frame Interval End-Of-Periodic Frame Interrupt
                                                     time. 0x0: 80% 0x1: 85% 0x2: 90% 0x3: 95% Value after reset:
                                                     0x0                                                                       */
      __IOM uint32_t EnDevOutNak : 1;           /*!< [13..13] Enable Device OUT NAK after Bulk OUT transfer complete
                                                     (only for Descriptor DMA mode). Value after reset: 0x0                    */
      __IOM uint32_t XCVRDLY    : 1;            /*!< [14..14] XCVR Delay between xcvr_sel and txvalid during device
                                                     chirp. Value after reset: 0x0                                             */
      __IOM uint32_t ErraticIntMsk : 1;         /*!< [15..15] Erratic Error Interrupt Mask. Value after reset: 0x0             */
            uint32_t            : 1;
      __IOM uint32_t ipgisocSupt : 1;           /*!< [17..17] Worst-Case Inter-Packet Gap ISOC OUT Support. Value
                                                     after reset: 0x1                                                          */
      __IOM uint32_t EPMisCnt   : 5;            /*!< [22..18] IN Endpoint Mismatch Count (Shared FIFO only). Value
                                                     after reset: 0x8                                                          */
      __IOM uint32_t DescDMA    : 1;            /*!< [23..23] Enable Scatter/Gather DMA in device mode. One-time
                                                     programmable after reset. Value after reset: 0x0                          */
      __IOM uint32_t PerSchIntvl : 2;           /*!< [25..24] Periodic Scheduling Interval.                                    */
      __IOM uint32_t ResValid   : 6;            /*!< [31..26] Resume Validation Period. Effective only when Ena32KHzSusp
                                                     is set. Value after reset: 0x2                                            */
    } DCFG_b;
  } ;
  
  union {
    __IOM uint32_t DCTL;                        /*!< (@ 0x00000004) Device Control Register                                    */
    
    struct {
      __IOM uint32_t RmtWkUpSig : 1;            /*!< [0..0] Remote Wakeup Signaling                                            */
      __IOM uint32_t SftDiscon  : 1;            /*!< [1..1] Soft Disconnect                                                    */
      __IM  uint32_t GNPINNakSts : 1;           /*!< [2..2] Global Non-Periodic IN NAK Status                                  */
      __IM  uint32_t GOUTNakSts : 1;            /*!< [3..3] Global OUT NAK Status                                              */
      __IOM uint32_t TstCtl     : 3;            /*!< [6..4] Test Control                                                       */
      __OM  uint32_t SGNPInNak  : 1;            /*!< [7..7] Set Global Non-Periodic IN NAK                                     */
      __OM  uint32_t CGNPInNak  : 1;            /*!< [8..8] Clear Global Non-Periodic IN NAK                                   */
      __OM  uint32_t SGOUTNak   : 1;            /*!< [9..9] Set Global OUT NAK                                                 */
      __OM  uint32_t CGOUTNak   : 1;            /*!< [10..10] Clear Global OUT NAK                                             */
      __IOM uint32_t PWROnPrgDone : 1;          /*!< [11..11] Power-On Programming Done                                        */
            uint32_t            : 1;
      __IOM uint32_t GMC        : 2;            /*!< [14..13] Global Multi Packet Count                                        */
      __IOM uint32_t IgnrFrmNum : 1;            /*!< [15..15] Ignore Frame Number / Enable PTI Mode                            */
      __IOM uint32_t NakOnBble  : 1;            /*!< [16..16] NAK on Babble Error                                              */
      __IOM uint32_t EnContOnBNA : 1;           /*!< [17..17] Continue on BNA (DMA mode)                                       */
      __IOM uint32_t DeepSleepBESLReject : 1;   /*!< [18..18] Enable Deep Sleep BESL Reject                                    */
      __IOM uint32_t ServInt    : 1;            /*!< [19..19] Enable Service Interval scheduling for ISOC IN EP                */
            uint32_t            : 12;
    } DCTL_b;
  } ;
  
  union {
    __IOM uint32_t DSTS;                        /*!< (@ 0x00000008) Device Status Register                                     */
    
    struct {
      __IM  uint32_t SuspSts    : 1;            /*!< [0..0] Device is in suspend state                                         */
      __IM  uint32_t EnumSpd    : 2;            /*!< [2..1] Enumerated USB speed                                               */
      __IM  uint32_t ErrticErr  : 1;            /*!< [3..3] Erratic error detected                                             */
            uint32_t            : 4;
      __IM  uint32_t SOFFN      : 14;           /*!< [21..8] Current frame or microframe number                                */
      __IM  uint32_t DevLnSts   : 2;            /*!< [23..22] Current logic level of USB D+ and D- lines                       */
            uint32_t            : 8;
    } DSTS_b;
  } ;
  __IM  uint32_t  RESERVED;
  
  union {
    __IOM uint32_t DIEPMSK;                     /*!< (@ 0x00000010) Device IN Endpoint Common Interrupt Mask Register          */
    
    struct {
      __IOM uint32_t XferComplMsk : 1;          /*!< [0..0] Transfer completed interrupt mask                                  */
      __IOM uint32_t EPDisbldMsk : 1;           /*!< [1..1] Endpoint disabled interrupt mask                                   */
      __IOM uint32_t AHBErrMsk  : 1;            /*!< [2..2] AHB error interrupt mask                                           */
      __IOM uint32_t TimeOUTMsk : 1;            /*!< [3..3] Timeout condition interrupt mask                                   */
      __IOM uint32_t INTknTXFEmpMsk : 1;        /*!< [4..4] IN token TxFIFO empty interrupt mask                               */
      __IOM uint32_t INTknEPMisMsk : 1;         /*!< [5..5] IN token EP mismatch interrupt mask                                */
      __IOM uint32_t INEPNakEffMsk : 1;         /*!< [6..6] IN EP NAK effective interrupt mask                                 */
            uint32_t            : 1;
      __IOM uint32_t TxfifoUndrnMsk : 1;        /*!< [8..8] FIFO underrun interrupt mask                                       */
      __IOM uint32_t BNAInIntrMsk : 1;          /*!< [9..9] BNA interrupt mask                                                 */
            uint32_t            : 3;
      __IOM uint32_t NAKMsk     : 1;            /*!< [13..13] NAK interrupt mask                                               */
            uint32_t            : 18;
    } DIEPMSK_b;
  } ;
  
  union {
    __IOM uint32_t DOEPMSK;                     /*!< (@ 0x00000014) Device OUT Endpoint Common Interrupt Mask Register         */
    
    struct {
      __IOM uint32_t XferComplMsk : 1;          /*!< [0..0] Transfer completed interrupt mask                                  */
      __IOM uint32_t EPDisbldMsk : 1;           /*!< [1..1] Endpoint disabled interrupt mask                                   */
      __IOM uint32_t AHBErrMsk  : 1;            /*!< [2..2] AHB error interrupt mask                                           */
      __IOM uint32_t SetUPMsk   : 1;            /*!< [3..3] SETUP phase done interrupt mask                                    */
      __IOM uint32_t OUTTknEPdisMsk : 1;        /*!< [4..4] OUT token received when EP disabled mask                           */
      __IOM uint32_t StsPhseRcvdMsk : 1;        /*!< [5..5] Status phase received mask                                         */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets mask                                    */
            uint32_t            : 1;
      __IOM uint32_t OutPktErrMsk : 1;          /*!< [8..8] OUT packet error interrupt mask                                    */
      __IOM uint32_t BnaOutIntrMsk : 1;         /*!< [9..9] BNA interrupt mask                                                 */
            uint32_t            : 2;
      __IOM uint32_t BbleErrMsk : 1;            /*!< [12..12] Babble error interrupt mask                                      */
      __IOM uint32_t NAKMsk     : 1;            /*!< [13..13] NAK interrupt mask                                               */
      __IOM uint32_t NYETMsk    : 1;            /*!< [14..14] NYET interrupt mask                                              */
            uint32_t            : 17;
    } DOEPMSK_b;
  } ;
  
  union {
    __IOM uint32_t DAINT;                       /*!< (@ 0x00000018) Device All Endpoints Interrupt Register                    */
    
    struct {
      __IM  uint32_t InEPInt0   : 1;            /*!< [0..0] IN EP0 interrupt                                                   */
      __IM  uint32_t InEPInt1   : 1;            /*!< [1..1] IN EP1 interrupt                                                   */
      __IM  uint32_t InEPInt2   : 1;            /*!< [2..2] IN EP2 interrupt                                                   */
      __IM  uint32_t InEPInt3   : 1;            /*!< [3..3] IN EP3 interrupt                                                   */
      __IM  uint32_t InEPInt4   : 1;            /*!< [4..4] IN EP4 interrupt                                                   */
      __IM  uint32_t InEPInt5   : 1;            /*!< [5..5] IN EP5 interrupt                                                   */
      __IM  uint32_t InEPInt6   : 1;            /*!< [6..6] IN EP6 interrupt                                                   */
      __IM  uint32_t InEPInt7   : 1;            /*!< [7..7] IN EP7 interrupt                                                   */
      __IM  uint32_t InEPInt8   : 1;            /*!< [8..8] IN EP8 interrupt                                                   */
      __IM  uint32_t InEPInt9   : 1;            /*!< [9..9] IN EP9 interrupt                                                   */
      __IM  uint32_t InEPInt10  : 1;            /*!< [10..10] IN EP10 interrupt                                                */
      __IM  uint32_t InEPInt11  : 1;            /*!< [11..11] IN EP11 interrupt                                                */
      __IM  uint32_t InEPInt12  : 1;            /*!< [12..12] IN EP12 interrupt                                                */
      __IM  uint32_t InEPInt13  : 1;            /*!< [13..13] IN EP13 interrupt                                                */
      __IM  uint32_t InEPInt14  : 1;            /*!< [14..14] IN EP14 interrupt                                                */
      __IM  uint32_t InEPInt15  : 1;            /*!< [15..15] IN EP15 interrupt                                                */
      __IM  uint32_t OutEPInt0  : 1;            /*!< [16..16] OUT EP0 interrupt                                                */
      __IM  uint32_t OutEPInt1  : 1;            /*!< [17..17] OUT EP1 interrupt                                                */
      __IM  uint32_t OutEPInt2  : 1;            /*!< [18..18] OUT EP2 interrupt                                                */
      __IM  uint32_t OutEPInt3  : 1;            /*!< [19..19] OUT EP3 interrupt                                                */
      __IM  uint32_t OutEPInt4  : 1;            /*!< [20..20] OUT EP4 interrupt                                                */
      __IM  uint32_t OutEPInt5  : 1;            /*!< [21..21] OUT EP5 interrupt                                                */
      __IM  uint32_t OutEPInt6  : 1;            /*!< [22..22] OUT EP6 interrupt                                                */
      __IM  uint32_t OutEPInt7  : 1;            /*!< [23..23] OUT EP7 interrupt                                                */
      __IM  uint32_t OutEPInt8  : 1;            /*!< [24..24] OUT EP8 interrupt                                                */
      __IM  uint32_t OutEPInt9  : 1;            /*!< [25..25] OUT EP9 interrupt                                                */
      __IM  uint32_t OutEPInt10 : 1;            /*!< [26..26] OUT EP10 interrupt                                               */
      __IM  uint32_t OutEPInt11 : 1;            /*!< [27..27] OUT EP11 interrupt                                               */
      __IM  uint32_t OutEPInt12 : 1;            /*!< [28..28] OUT EP12 interrupt                                               */
      __IM  uint32_t OutEPInt13 : 1;            /*!< [29..29] OUT EP13 interrupt                                               */
      __IM  uint32_t OutEPInt14 : 1;            /*!< [30..30] OUT EP14 interrupt                                               */
      __IM  uint32_t OutEPInt15 : 1;            /*!< [31..31] OUT EP15 interrupt                                               */
    } DAINT_b;
  } ;
  
  union {
    __IOM uint32_t DAINTMSK;                    /*!< (@ 0x0000001C) Device All Endpoints Interrupt Mask Register               */
    
    struct {
      __IOM uint32_t InEpMsk0   : 1;            /*!< [0..0] IN EP0 interrupt mask                                              */
      __IOM uint32_t InEpMsk1   : 1;            /*!< [1..1] IN EP1 interrupt mask                                              */
      __IOM uint32_t InEpMsk2   : 1;            /*!< [2..2] IN EP2 interrupt mask                                              */
      __IOM uint32_t InEpMsk3   : 1;            /*!< [3..3] IN EP3 interrupt mask                                              */
      __IOM uint32_t InEpMsk4   : 1;            /*!< [4..4] IN EP4 interrupt mask                                              */
      __IOM uint32_t InEpMsk5   : 1;            /*!< [5..5] IN EP5 interrupt mask                                              */
      __IOM uint32_t InEpMsk6   : 1;            /*!< [6..6] IN EP6 interrupt mask                                              */
      __IOM uint32_t InEpMsk7   : 1;            /*!< [7..7] IN EP7 interrupt mask                                              */
      __IOM uint32_t InEpMsk8   : 1;            /*!< [8..8] IN EP8 interrupt mask                                              */
      __IOM uint32_t InEpMsk9   : 1;            /*!< [9..9] IN EP9 interrupt mask                                              */
      __IOM uint32_t InEpMsk10  : 1;            /*!< [10..10] IN EP10 interrupt mask                                           */
      __IOM uint32_t InEpMsk11  : 1;            /*!< [11..11] IN EP11 interrupt mask                                           */
      __IOM uint32_t InEpMsk12  : 1;            /*!< [12..12] IN EP12 interrupt mask                                           */
      __IOM uint32_t InEpMsk13  : 1;            /*!< [13..13] IN EP13 interrupt mask                                           */
      __IOM uint32_t InEpMsk14  : 1;            /*!< [14..14] IN EP14 interrupt mask                                           */
      __IOM uint32_t InEpMsk15  : 1;            /*!< [15..15] IN EP15 interrupt mask                                           */
      __IOM uint32_t OutEPMsk0  : 1;            /*!< [16..16] OUT EP0 interrupt mask                                           */
      __IOM uint32_t OutEPMsk1  : 1;            /*!< [17..17] OUT EP1 interrupt mask                                           */
      __IOM uint32_t OutEPMsk2  : 1;            /*!< [18..18] OUT EP2 interrupt mask                                           */
      __IOM uint32_t OutEPMsk3  : 1;            /*!< [19..19] OUT EP3 interrupt mask                                           */
      __IOM uint32_t OutEPMsk4  : 1;            /*!< [20..20] OUT EP4 interrupt mask                                           */
      __IOM uint32_t OutEPMsk5  : 1;            /*!< [21..21] OUT EP5 interrupt mask                                           */
      __IOM uint32_t OutEPMsk6  : 1;            /*!< [22..22] OUT EP6 interrupt mask                                           */
      __IOM uint32_t OutEPMsk7  : 1;            /*!< [23..23] OUT EP7 interrupt mask                                           */
      __IOM uint32_t OutEPMsk8  : 1;            /*!< [24..24] OUT EP8 interrupt mask                                           */
      __IOM uint32_t OutEPMsk9  : 1;            /*!< [25..25] OUT EP9 interrupt mask                                           */
      __IOM uint32_t OutEPMsk10 : 1;            /*!< [26..26] OUT EP10 interrupt mask                                          */
      __IOM uint32_t OutEPMsk11 : 1;            /*!< [27..27] OUT EP11 interrupt mask                                          */
      __IOM uint32_t OutEPMsk12 : 1;            /*!< [28..28] OUT EP12 interrupt mask                                          */
      __IOM uint32_t OutEPMsk13 : 1;            /*!< [29..29] OUT EP13 interrupt mask                                          */
      __IOM uint32_t OutEPMsk14 : 1;            /*!< [30..30] OUT EP14 interrupt mask                                          */
      __IOM uint32_t OutEPMsk15 : 1;            /*!< [31..31] OUT EP15 interrupt mask                                          */
    } DAINTMSK_b;
  } ;
  __IM  uint32_t  RESERVED1[2];
  __IOM uint32_t  DVBUSDIS;                     /*!< (@ 0x00000028) Device VBUS Discharge Time Register                        */
  __IOM uint32_t  DVBUSPULSE;                   /*!< (@ 0x0000002C) Device VBUS Pulsing Time Register                          */
  __IOM uint32_t  DTHRCTL;                      /*!< (@ 0x00000030) Device Threshold Control Register                          */
  __IOM uint32_t  DIEPEMPMSK;                   /*!< (@ 0x00000034) Device IN Endpoint FIFO Empty Interrupt Mask
                                                                    Register                                                   */
  __IOM uint32_t  DEACHINT;                     /*!< (@ 0x00000038) Device Each Endpoints Interrupt Register                   */
  __IOM uint32_t  DEACHINTMSK;                  /*!< (@ 0x0000003C) Device Each Endpoints Interrupt Mask Register              */
  
  union {
    __IOM uint32_t DIEPEACHMSK0;                /*!< (@ 0x00000040) Device Each IN Endpoint 0 Interrupt Register               */
    
    struct {
      __IOM uint32_t XferComplMsk : 1;          /*!< [0..0] Transfer completed interrupt mask                                  */
      __IOM uint32_t EPDisbldMsk : 1;           /*!< [1..1] EP disabled interrupt mask                                         */
      __IOM uint32_t AHBErrMsk  : 1;            /*!< [2..2] AHB error mask                                                     */
      __IOM uint32_t TimeOUTMsk : 1;            /*!< [3..3] Timeout condition mask                                             */
      __IOM uint32_t INTknTXFEmpMsk : 1;        /*!< [4..4] IN token TxFIFO empty mask                                         */
      __IOM uint32_t INTknEPMisMsk : 1;         /*!< [5..5] IN token EP mismatch mask                                          */
      __IOM uint32_t INEPNakEffMsk : 1;         /*!< [6..6] IN endpoint NAK effective mask                                     */
            uint32_t            : 1;
      __IOM uint32_t TxfifoUndrnMsk : 1;        /*!< [8..8] Fifo underrun mask                                                 */
      __IOM uint32_t BNAInIntrMsk : 1;          /*!< [9..9] BNA interrupt mask                                                 */
            uint32_t            : 3;
      __IOM uint32_t NAKMsk     : 1;            /*!< [13..13] NAK interrupt mask                                               */
            uint32_t            : 18;
    } DIEPEACHMSK0_b;
  } ;
  
  union {
    __IOM uint32_t DIEPEACHMSK_ARR[15];         /*!< (@ 0x00000044) Device Each IN Endpoint Interrupt Registers                */
    
    struct {
      __IOM uint32_t XferComplMsk : 1;          /*!< [0..0] Transfer completed interrupt mask                                  */
      __IOM uint32_t EPDisbldMsk : 1;           /*!< [1..1] Endpoint disabled interrupt mask                                   */
      __IOM uint32_t AHBErrMsk  : 1;            /*!< [2..2] AHB error mask                                                     */
      __IOM uint32_t TimeOUTMsk : 1;            /*!< [3..3] Timeout condition mask (Non-isochronous endpoints)                 */
      __IOM uint32_t INTknTXFEmpMsk : 1;        /*!< [4..4] IN token TxFIFO empty mask                                         */
      __IOM uint32_t INTknEPMisMsk : 1;         /*!< [5..5] IN token EP mismatch mask                                          */
      __IOM uint32_t INEPNakEffMsk : 1;         /*!< [6..6] IN endpoint NAK effective mask                                     */
            uint32_t            : 1;
      __IOM uint32_t TxfifoUndrnMsk : 1;        /*!< [8..8] Fifo underrun mask                                                 */
      __IOM uint32_t BNAInIntrMsk : 1;          /*!< [9..9] BNA interrupt mask                                                 */
            uint32_t            : 3;
      __IOM uint32_t NAKMsk     : 1;            /*!< [13..13] NAK interrupt mask                                               */
            uint32_t            : 18;
    } DIEPEACHMSK_ARR_b[15];
  } ;
  
  union {
    __IOM uint32_t DOEPEACHMSK0;                /*!< (@ 0x00000080) Device Each OUT Endpoint 0 Interrupt Registers             */
    
    struct {
      __IOM uint32_t XferComplMsk : 1;          /*!< [0..0] Transfer completed interrupt mask                                  */
      __IOM uint32_t EPDisbldMsk : 1;           /*!< [1..1] Endpoint disabled interrupt mask                                   */
      __IOM uint32_t AHBErrMsk  : 1;            /*!< [2..2] AHB error mask                                                     */
      __IOM uint32_t SetUPMsk   : 1;            /*!< [3..3] SETUP phase done mask                                              */
      __IOM uint32_t OUTTknEPdisMsk : 1;        /*!< [4..4] OUT token EP disabled mask                                         */
      __IOM uint32_t StsPhsRcvdMsk : 1;         /*!< [5..5] Status phase received mask                                         */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-Back SETUP packets mask                                    */
            uint32_t            : 1;
      __IOM uint32_t OutPktErrMsk : 1;          /*!< [8..8] OUT packet error mask                                              */
      __IOM uint32_t BnaOutIntrMsk : 1;         /*!< [9..9] BNA interrupt mask                                                 */
            uint32_t            : 2;
      __IOM uint32_t BbleErrMsk : 1;            /*!< [12..12] Babble error mask                                                */
      __IOM uint32_t NAKMsk     : 1;            /*!< [13..13] NAK interrupt mask                                               */
      __IOM uint32_t NYETMsk    : 1;            /*!< [14..14] NYET interrupt mask                                              */
            uint32_t            : 17;
    } DOEPEACHMSK0_b;
  } ;
  
  union {
    __IOM uint32_t DOEPEACHMSK_ARR[15];         /*!< (@ 0x00000084) Device Each OUT Endpoint Interrupt Registers               */
    
    struct {
      __IOM uint32_t XferComplMsk : 1;          /*!< [0..0] Transfer completed mask                                            */
      __IOM uint32_t EPDisbldMsk : 1;           /*!< [1..1] Endpoint disabled interrupt mask                                   */
      __IOM uint32_t AHBErrMsk  : 1;            /*!< [2..2] AHB error mask                                                     */
      __IOM uint32_t SetUPMsk   : 1;            /*!< [3..3] SETUP phase done mask                                              */
      __IOM uint32_t OUTTknEPdisMsk : 1;        /*!< [4..4] OUT token received when endpoint disabled mask                     */
      __IOM uint32_t StsPhsRcvdMsk : 1;         /*!< [5..5] Status phase received mask                                         */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-Back SETUP packets received mask                           */
            uint32_t            : 1;
      __IOM uint32_t OutPktErrMsk : 1;          /*!< [8..8] OUT packet error mask                                              */
      __IOM uint32_t BnaOutIntrMsk : 1;         /*!< [9..9] BNA interrupt mask                                                 */
            uint32_t            : 2;
      __IOM uint32_t BbleErrMsk : 1;            /*!< [12..12] Babble error mask                                                */
      __IOM uint32_t NAKMsk     : 1;            /*!< [13..13] NAK response received mask                                       */
      __IOM uint32_t NYETMsk    : 1;            /*!< [14..14] NYET interrupt mask                                              */
            uint32_t            : 17;
    } DOEPEACHMSK_ARR_b[15];
  } ;
} USB_OTG_DEVICE_Type;                          /*!< Size = 192 (0xc0)                                                         */



/* =========================================================================================================================== */
/* ================                                        USB_IN_EP1                                         ================ */
/* =========================================================================================================================== */


/**
  * @brief USB OTG IN-Endpoint Controller 1 (USB_IN_EP1)
  */

typedef struct {                                /*!< (@ 0x50000900) USB_IN_EP1 Structure                                       */
  
  union {
    __IOM uint32_t DIEPCTL0;                    /*!< (@ 0x00000000) Device Control IN Endpoint 0 Control Register              */
    
    struct {
      __IOM uint32_t MPS        : 2;            /*!< [1..0] Maximum packet size                                                */
            uint32_t            : 9;
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint number                                             */
      __IM  uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
            uint32_t            : 1;
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IM  uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] Endpoint STALL handshake                                         */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number                                                    */
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
            uint32_t            : 2;
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL0_b;
  } ;
  __IM  uint32_t  RESERVED;
  
  union {
    __IOM uint32_t DIEPINT0;                    /*!< (@ 0x00000008) Device IN Endpoint 0 Interrupt Register                    */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition                                                  */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective                                          */
      __IOM uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty (IN endpoint only)                             */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] TxFIFO underrun interrupt (IN endpoint only)                       */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] BNA (Buffer Not Available) interrupt                               */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] Packet drop status                                               */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT0_b;
  } ;
  __IM  uint32_t  RESERVED1;
  
  union {
    __IOM uint32_t DIEPTSIZ0;                   /*!< (@ 0x00000010) Device IN Endpoint 0 Transfer Size Register                */
    
    struct {
      __IOM uint32_t XferSize   : 7;            /*!< [6..0] Transfer size in bytes                                             */
            uint32_t            : 12;
      __IOM uint32_t PktCnt     : 2;            /*!< [20..19] USB packet count                                                 */
            uint32_t            : 11;
    } DIEPTSIZ0_b;
  } ;
  __IOM uint32_t  DIEPDMA0;                     /*!< (@ 0x00000014) Device IN Endpoint 0 DMA Address Register                  */
  __IM  uint32_t  DTXFSTS0;                     /*!< (@ 0x00000018) Device IN Endpoint 0 Transmit FIFO Status Register         */
  __IOM uint32_t  DIEPDMAB0;                    /*!< (@ 0x0000001C) Device IN Endpoint 0 Buffer Address Register               */
  
  union {
    __IOM uint32_t DIEPCTL1;                    /*!< (@ 0x00000020) Device Control IN Endpoint 1 Control Register              */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL1_b;
  } ;
  __IM  uint32_t  RESERVED2;
  
  union {
    __IOM uint32_t DIEPINT1;                    /*!< (@ 0x00000028) Device IN Endpoint 1 Interrupt Register                    */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT1_b;
  } ;
  __IM  uint32_t  RESERVED3;
  
  union {
    __IOM uint32_t DIEPTSIZ1;                   /*!< (@ 0x00000030) Device IN Endpoint 1 Transfer Size Register                */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ1_b;
  } ;
  __IOM uint32_t  DIEPDMA1;                     /*!< (@ 0x00000034) Device IN Endpoint 1 DMA Address Register                  */
  __IM  uint32_t  DTXFSTS1;                     /*!< (@ 0x00000038) Device IN Endpoint 1 Transmit FIFO Status Register         */
  __IOM uint32_t  DIEPDMAB1;                    /*!< (@ 0x0000003C) Device IN Endpoint 1 Buffer Address Register               */
  
  union {
    __IOM uint32_t DIEPCTL2;                    /*!< (@ 0x00000040) Device Control IN Endpoint 2 Control Register              */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL2_b;
  } ;
  __IM  uint32_t  RESERVED4;
  
  union {
    __IOM uint32_t DIEPINT2;                    /*!< (@ 0x00000048) Device Control IN Endpoint 2 Control Register              */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT2_b;
  } ;
  __IM  uint32_t  RESERVED5;
  
  union {
    __IOM uint32_t DIEPTSIZ2;                   /*!< (@ 0x00000050) Device IN Endpoint 2 Transfer Size Register                */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ2_b;
  } ;
  __IOM uint32_t  DIEPDMA2;                     /*!< (@ 0x00000054) Device IN Endpoint 2 DMA Address Register                  */
  __IM  uint32_t  DTXFSTS2;                     /*!< (@ 0x00000058) Device IN Endpoint 2 Transmit FIFO Status Register         */
  __IOM uint32_t  DIEPDMAB2;                    /*!< (@ 0x0000005C) Device IN Endpoint 2 Buffer Address Register               */
  
  union {
    __IOM uint32_t DIEPCTL3;                    /*!< (@ 0x00000060) Device Control IN Endpoint 3 Control Register              */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL3_b;
  } ;
  __IM  uint32_t  RESERVED6;
  
  union {
    __IOM uint32_t DIEPINT3;                    /*!< (@ 0x00000068) Device Control IN Endpoint 3 Control Register              */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT3_b;
  } ;
  __IM  uint32_t  RESERVED7;
  
  union {
    __IOM uint32_t DIEPTSIZ3;                   /*!< (@ 0x00000070) Device IN Endpoint 2 Transfer Size Register                */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ3_b;
  } ;
  __IOM uint32_t  DIEPDMA3;                     /*!< (@ 0x00000074) Device IN Endpoint 3 DMA Address Register                  */
  __IM  uint32_t  DTXFSTS3;                     /*!< (@ 0x00000078) Device IN Endpoint 3 Transmit FIFO Status Register         */
  __IOM uint32_t  DIEPDMAB3;                    /*!< (@ 0x0000007C) Device IN Endpoint 3 Buffer Address Register               */
  
  union {
    __IOM uint32_t DIEPCTL4;                    /*!< (@ 0x00000080) Device Control IN Endpoint 4 Control Register              */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL4_b;
  } ;
  __IM  uint32_t  RESERVED8;
  
  union {
    __IOM uint32_t DIEPINT4;                    /*!< (@ 0x00000088) Device Control IN Endpoint 4 Control Register              */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT4_b;
  } ;
  __IM  uint32_t  RESERVED9;
  
  union {
    __IOM uint32_t DIEPTSIZ4;                   /*!< (@ 0x00000090) Device IN Endpoint 4 Transfer Size Register                */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ4_b;
  } ;
  __IOM uint32_t  DIEPDMA4;                     /*!< (@ 0x00000094) Device IN Endpoint 4 DMA Address Register                  */
  __IM  uint32_t  DTXFSTS4;                     /*!< (@ 0x00000098) Device IN Endpoint 4 Transmit FIFO Status Register         */
  __IOM uint32_t  DIEPDMAB4;                    /*!< (@ 0x0000009C) Device IN Endpoint 4 Buffer Address Register               */
  
  union {
    __IOM uint32_t DIEPCTL5;                    /*!< (@ 0x000000A0) Device Control IN Endpoint 5 Control Register              */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL5_b;
  } ;
  __IM  uint32_t  RESERVED10;
  
  union {
    __IOM uint32_t DIEPINT5;                    /*!< (@ 0x000000A8) Device Control IN Endpoint 5 Control Register              */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT5_b;
  } ;
  __IM  uint32_t  RESERVED11;
  
  union {
    __IOM uint32_t DIEPTSIZ5;                   /*!< (@ 0x000000B0) Device IN Endpoint 5 Transfer Size Register                */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ5_b;
  } ;
  __IOM uint32_t  DIEPDMA5;                     /*!< (@ 0x000000B4) Device IN Endpoint 5 DMA Address Register                  */
  __IM  uint32_t  DTXFSTS5;                     /*!< (@ 0x000000B8) Device IN Endpoint 5 Transmit FIFO Status Register         */
  __IOM uint32_t  DIEPDMAB5;                    /*!< (@ 0x000000BC) Device IN Endpoint 5 Buffer Address Register               */
  
  union {
    __IOM uint32_t DIEPCTL6;                    /*!< (@ 0x000000C0) Device Control IN Endpoint 6 Control Register              */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL6_b;
  } ;
  __IM  uint32_t  RESERVED12;
  
  union {
    __IOM uint32_t DIEPINT6;                    /*!< (@ 0x000000C8) Device Control IN Endpoint 6 Control Register              */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT6_b;
  } ;
  __IM  uint32_t  RESERVED13;
  
  union {
    __IOM uint32_t DIEPTSIZ6;                   /*!< (@ 0x000000D0) Device IN Endpoint 6 Transfer Size Register                */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ6_b;
  } ;
  __IOM uint32_t  DIEPDMA6;                     /*!< (@ 0x000000D4) Device IN Endpoint 6 DMA Address Register                  */
  __IM  uint32_t  DTXFSTS6;                     /*!< (@ 0x000000D8) Device IN Endpoint 6 Transmit FIFO Status Register         */
  __IOM uint32_t  DIEPDMAB6;                    /*!< (@ 0x000000DC) Device IN Endpoint 6 Buffer Address Register               */
  
  union {
    __IOM uint32_t DIEPCTL7;                    /*!< (@ 0x000000E0) Device Control IN Endpoint 7 Control Register              */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL7_b;
  } ;
  __IM  uint32_t  RESERVED14;
  
  union {
    __IOM uint32_t DIEPINT7;                    /*!< (@ 0x000000E8) Device Control IN Endpoint 7 Control Register              */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT7_b;
  } ;
  __IM  uint32_t  RESERVED15;
  
  union {
    __IOM uint32_t DIEPTSIZ7;                   /*!< (@ 0x000000F0) Device IN Endpoint 7 Transfer Size Register                */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ7_b;
  } ;
  __IOM uint32_t  DIEPDMA7;                     /*!< (@ 0x000000F4) Device IN Endpoint 7 DMA Address Register                  */
  __IM  uint32_t  DTXFSTS7;                     /*!< (@ 0x000000F8) Device IN Endpoint 7 Transmit FIFO Status Register         */
  __IOM uint32_t  DIEPDMAB7;                    /*!< (@ 0x000000FC) Device IN Endpoint 7 Buffer Address Register               */
  
  union {
    __IOM uint32_t DIEPCTL8;                    /*!< (@ 0x00000100) Device Control IN Endpoint 8 Control Register              */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL8_b;
  } ;
  __IM  uint32_t  RESERVED16;
  
  union {
    __IOM uint32_t DIEPINT8;                    /*!< (@ 0x00000108) Device Control IN Endpoint 8 Control Register              */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT8_b;
  } ;
  __IM  uint32_t  RESERVED17;
  
  union {
    __IOM uint32_t DIEPTSIZ8;                   /*!< (@ 0x00000110) Device IN Endpoint 8 Transfer Size Register                */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ8_b;
  } ;
  __IOM uint32_t  DIEPDMA8;                     /*!< (@ 0x00000114) Device IN Endpoint 8 DMA Address Register                  */
  __IM  uint32_t  DTXFSTS8;                     /*!< (@ 0x00000118) Device IN Endpoint 8 Transmit FIFO Status Register         */
  __IOM uint32_t  DIEPDMAB8;                    /*!< (@ 0x0000011C) Device IN Endpoint 8 Buffer Address Register               */
  
  union {
    __IOM uint32_t DIEPCTL9;                    /*!< (@ 0x00000120) Device Control IN Endpoint 9 Control Register              */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL9_b;
  } ;
  __IM  uint32_t  RESERVED18;
  
  union {
    __IOM uint32_t DIEPINT9;                    /*!< (@ 0x00000128) Device Control IN Endpoint 9 Control Register              */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT9_b;
  } ;
  __IM  uint32_t  RESERVED19;
  
  union {
    __IOM uint32_t DIEPTSIZ9;                   /*!< (@ 0x00000130) Device IN Endpoint 9 Transfer Size Register                */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ9_b;
  } ;
  __IOM uint32_t  DIEPDMA9;                     /*!< (@ 0x00000134) Device IN Endpoint 9 DMA Address Register                  */
  __IM  uint32_t  DTXFSTS9;                     /*!< (@ 0x00000138) Device IN Endpoint 9 Transmit FIFO Status Register         */
  __IOM uint32_t  DIEPDMAB9;                    /*!< (@ 0x0000013C) Device IN Endpoint 9 Buffer Address Register               */
  
  union {
    __IOM uint32_t DIEPCTL10;                   /*!< (@ 0x00000140) Device Control IN Endpoint 10 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL10_b;
  } ;
  __IM  uint32_t  RESERVED20;
  
  union {
    __IOM uint32_t DIEPINT10;                   /*!< (@ 0x00000148) Device Control IN Endpoint 10 Control Register             */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT10_b;
  } ;
  __IM  uint32_t  RESERVED21;
  
  union {
    __IOM uint32_t DIEPTSIZ10;                  /*!< (@ 0x00000150) Device IN Endpoint 10 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ10_b;
  } ;
  __IOM uint32_t  DIEPDMA10;                    /*!< (@ 0x00000154) Device IN Endpoint 10 DMA Address Register                 */
  __IM  uint32_t  DTXFSTS10;                    /*!< (@ 0x00000158) Device IN Endpoint 10 Transmit FIFO Status Register        */
  __IOM uint32_t  DIEPDMAB10;                   /*!< (@ 0x0000015C) Device IN Endpoint 10 Buffer Address Register              */
  
  union {
    __IOM uint32_t DIEPCTL11;                   /*!< (@ 0x00000160) Device Control IN Endpoint 11 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL11_b;
  } ;
  __IM  uint32_t  RESERVED22;
  
  union {
    __IOM uint32_t DIEPINT11;                   /*!< (@ 0x00000168) Device Control IN Endpoint 11 Control Register             */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT11_b;
  } ;
  __IM  uint32_t  RESERVED23;
  
  union {
    __IOM uint32_t DIEPTSIZ11;                  /*!< (@ 0x00000170) Device IN Endpoint 11 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ11_b;
  } ;
  __IOM uint32_t  DIEPDMA11;                    /*!< (@ 0x00000174) Device IN Endpoint 11 DMA Address Register                 */
  __IM  uint32_t  DTXFSTS11;                    /*!< (@ 0x00000178) Device IN Endpoint 11 Transmit FIFO Status Register        */
  __IOM uint32_t  DIEPDMAB11;                   /*!< (@ 0x0000017C) Device IN Endpoint 11 Buffer Address Register              */
  
  union {
    __IOM uint32_t DIEPCTL12;                   /*!< (@ 0x00000180) Device Control IN Endpoint 12 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL12_b;
  } ;
  __IM  uint32_t  RESERVED24;
  
  union {
    __IOM uint32_t DIEPINT12;                   /*!< (@ 0x00000188) Device Control IN Endpoint 12 Control Register             */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT12_b;
  } ;
  __IM  uint32_t  RESERVED25;
  
  union {
    __IOM uint32_t DIEPTSIZ12;                  /*!< (@ 0x00000190) Device IN Endpoint 12 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ12_b;
  } ;
  __IOM uint32_t  DIEPDMA12;                    /*!< (@ 0x00000194) Device IN Endpoint 12 DMA Address Register                 */
  __IM  uint32_t  DTXFSTS12;                    /*!< (@ 0x00000198) Device IN Endpoint 12 Transmit FIFO Status Register        */
  __IOM uint32_t  DIEPDMAB12;                   /*!< (@ 0x0000019C) Device IN Endpoint 12 Buffer Address Register              */
  
  union {
    __IOM uint32_t DIEPCTL13;                   /*!< (@ 0x000001A0) Device Control IN Endpoint 13 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL13_b;
  } ;
  __IM  uint32_t  RESERVED26;
  
  union {
    __IOM uint32_t DIEPINT13;                   /*!< (@ 0x000001A8) Device Control IN Endpoint 13 Control Register             */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT13_b;
  } ;
  __IM  uint32_t  RESERVED27;
  
  union {
    __IOM uint32_t DIEPTSIZ13;                  /*!< (@ 0x000001B0) Device IN Endpoint 13 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ13_b;
  } ;
  __IOM uint32_t  DIEPDMA13;                    /*!< (@ 0x000001B4) Device IN Endpoint 13 DMA Address Register                 */
  __IM  uint32_t  DTXFSTS13;                    /*!< (@ 0x000001B8) Device IN Endpoint 13 Transmit FIFO Status Register        */
  __IOM uint32_t  DIEPDMAB13;                   /*!< (@ 0x000001BC) Device IN Endpoint 13 Buffer Address Register              */
  
  union {
    __IOM uint32_t DIEPCTL14;                   /*!< (@ 0x000001C0) Device Control IN Endpoint 14 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL14_b;
  } ;
  __IM  uint32_t  RESERVED28;
  
  union {
    __IOM uint32_t DIEPINT14;                   /*!< (@ 0x000001C8) Device Control IN Endpoint 14 Control Register             */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT14_b;
  } ;
  __IM  uint32_t  RESERVED29;
  
  union {
    __IOM uint32_t DIEPTSIZ14;                  /*!< (@ 0x000001D0) Device IN Endpoint 14 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ14_b;
  } ;
  __IOM uint32_t  DIEPDMA14;                    /*!< (@ 0x000001D4) Device IN Endpoint 14 DMA Address Register                 */
  __IM  uint32_t  DTXFSTS14;                    /*!< (@ 0x000001D8) Device IN Endpoint 14 Transmit FIFO Status Register        */
  __IOM uint32_t  DIEPDMAB14;                   /*!< (@ 0x000001DC) Device IN Endpoint 14 Buffer Address Register              */
  
  union {
    __IOM uint32_t DIEPCTL15;                   /*!< (@ 0x000001E0) Device Control IN Endpoint 15 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
      __IOM uint32_t NextEp     : 4;            /*!< [14..11] Next endpoint                                                    */
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IOM uint32_t DPID       : 1;            /*!< [16..16] Data PID or even/odd microframe                                  */
      __IOM uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
      __IOM uint32_t TxFNum     : 4;            /*!< [25..22] TxFIFO number (IN endpoint only)                                 */
      __IOM uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __IOM uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __IOM uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID or set even (micro)frame                           */
      __IOM uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID or set odd (micro)frame                            */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DIEPCTL15_b;
  } ;
  __IM  uint32_t  RESERVED30;
  
  union {
    __IOM uint32_t DIEPINT15;                   /*!< (@ 0x000001E8) Device Control IN Endpoint 15 Control Register             */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error interrupt                                                */
      __IOM uint32_t TimeOUT    : 1;            /*!< [3..3] Timeout condition interrupt                                        */
      __IOM uint32_t INTknTXFEmp : 1;           /*!< [4..4] IN token received when TxFIFO empty                                */
      __IOM uint32_t INTknEPMis : 1;            /*!< [5..5] IN token received with EP mismatch                                 */
      __IOM uint32_t INEPNakEff : 1;            /*!< [6..6] IN endpoint NAK effective interrupt                                */
      __IM  uint32_t TxFEmp     : 1;            /*!< [7..7] Transmit FIFO empty interrupt                                      */
      __IOM uint32_t TxfifoUndrn : 1;           /*!< [8..8] Tx FIFO underrun interrupt                                         */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
            uint32_t            : 17;
    } DIEPINT15_b;
  } ;
  __IM  uint32_t  RESERVED31;
  
  union {
    __IOM uint32_t DIEPTSIZ15;                  /*!< (@ 0x000001F0) Device IN Endpoint 15 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] USB packet count                                                 */
      __IOM uint32_t MC         : 2;            /*!< [30..29] Packets per microframe for IN endpoint                           */
            uint32_t            : 1;
    } DIEPTSIZ15_b;
  } ;
  __IOM uint32_t  DIEPDMA15;                    /*!< (@ 0x000001F4) Device IN Endpoint 15 DMA Address Register                 */
  __IM  uint32_t  DTXFSTS15;                    /*!< (@ 0x000001F8) Device IN Endpoint 15 Transmit FIFO Status Register        */
  __IOM uint32_t  DIEPDMAB15;                   /*!< (@ 0x000001FC) Device IN Endpoint 15 Buffer Address Register              */
} USB_OTG_IN_EP_Type;                           /*!< Size = 512 (0x200)                                                        */



/* =========================================================================================================================== */
/* ================                                        USB_OUT_EP1                                        ================ */
/* =========================================================================================================================== */


/**
  * @brief USB OTG Out-Endpoint Controller 1 (USB_OUT_EP1)
  */

typedef struct {                                /*!< (@ 0x50000B00) USB_OUT_EP1 Structure                                      */
  
  union {
    __IOM uint32_t DOEPCTL0;                    /*!< (@ 0x00000000) Device Control OUT Endpoint 0 Control Register             */
    
    struct {
      __IM  uint32_t MPS        : 2;            /*!< [1..0] Max packet size for EP0 (0:64B, 1:32B, 2:16B, 3:8B)                */
            uint32_t            : 13;
      __IM  uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint (always 1 for control 0)                     */
            uint32_t            : 1;
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status (0: non-NAK, 1: NAK handshakes)                       */
      __IM  uint32_t EPType     : 2;            /*!< [19..18] Endpoint type (always control)                                   */
      __IOM uint32_t Snp        : 1;            /*!< [20..20] Snp                                                              */
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake (write 1 to set, core clears)                    */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK (write 1 to clear)                                     */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK (write 1 to set)                                         */
            uint32_t            : 2;
      __IM  uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable status                                          */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable (write 1 to set)                                 */
    } DOEPCTL0_b;
  } ;
  __IM  uint32_t  RESERVED;
  
  union {
    __IOM uint32_t DOEPINT0;                    /*!< (@ 0x00000008) Device OUT Endpoint 0 Interrupt Register                   */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control OUT)                                */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error                                                     */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT0_b;
  } ;
  __IM  uint32_t  RESERVED1;
  
  union {
    __IOM uint32_t DOEPTSIZ0;                   /*!< (@ 0x00000010) Device OUT Endpoint 0 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 7;            /*!< [6..0] Transfer size in bytes                                             */
            uint32_t            : 12;
      __IOM uint32_t PktCnt     : 1;            /*!< [19..19] Packet count                                                     */
            uint32_t            : 9;
      __IOM uint32_t SUPCnt     : 2;            /*!< [30..29] SETUP packet count                                               */
            uint32_t            : 1;
    } DOEPTSIZ0_b;
  } ;
  __IOM uint32_t  DOEPDMA0;                     /*!< (@ 0x00000014) Device OUT Endpoint 0 DMA Address Register                 */
  __IM  uint32_t  RESERVED2;
  __IOM uint32_t  DOEPDMAB0;                    /*!< (@ 0x0000001C) Device OUT Endpoint 0 DMA Buffer Address Register          */
  
  union {
    __IOM uint32_t DOEPCTL1;                    /*!< (@ 0x00000020) Device Control OUT Endpoint 1 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL1_b;
  } ;
  __IM  uint32_t  RESERVED3;
  
  union {
    __IOM uint32_t DOEPINT1;                    /*!< (@ 0x00000028) Device OUT Endpoint 1 Interrupt Register                   */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT1_b;
  } ;
  __IM  uint32_t  RESERVED4;
  
  union {
    __IOM uint32_t DOEPTSIZ1;                   /*!< (@ 0x00000030) Device OUT Endpoint 1 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ1_b;
  } ;
  __IOM uint32_t  DOEPDMA1;                     /*!< (@ 0x00000034) Device OUT Endpoint 1 DMA Address Register                 */
  __IM  uint32_t  RESERVED5;
  __IM  uint32_t  DOEPDMAB1;                    /*!< (@ 0x0000003C) Device OUT Endpoint 1 Buffer Address Register              */
  
  union {
    __IOM uint32_t DOEPCTL2;                    /*!< (@ 0x00000040) Device Control OUT Endpoint 2 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL2_b;
  } ;
  __IM  uint32_t  RESERVED6;
  
  union {
    __IOM uint32_t DOEPINT2;                    /*!< (@ 0x00000048) Device OUT Endpoint 2 Interrupt Register                   */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT2_b;
  } ;
  __IM  uint32_t  RESERVED7;
  
  union {
    __IOM uint32_t DOEPTSIZ2;                   /*!< (@ 0x00000050) Device OUT Endpoint 2 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ2_b;
  } ;
  __IOM uint32_t  DOEPDMA2;                     /*!< (@ 0x00000054) Device OUT Endpoint 2 DMA Address Register                 */
  __IM  uint32_t  RESERVED8;
  __IM  uint32_t  DOEPDMAB2;                    /*!< (@ 0x0000005C) Device OUT Endpoint 2 Buffer Address Register              */
  
  union {
    __IOM uint32_t DOEPCTL3;                    /*!< (@ 0x00000060) Device Control OUT Endpoint 2 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL3_b;
  } ;
  __IM  uint32_t  RESERVED9;
  
  union {
    __IOM uint32_t DOEPINT3;                    /*!< (@ 0x00000068) Device OUT Endpoint 3 Interrupt Register                   */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT3_b;
  } ;
  __IM  uint32_t  RESERVED10;
  
  union {
    __IOM uint32_t DOEPTSIZ3;                   /*!< (@ 0x00000070) Device OUT Endpoint 3 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ3_b;
  } ;
  __IOM uint32_t  DOEPDMA3;                     /*!< (@ 0x00000074) Device OUT Endpoint 3 DMA Address Register                 */
  __IM  uint32_t  RESERVED11;
  __IM  uint32_t  DOEPDMAB3;                    /*!< (@ 0x0000007C) Device OUT Endpoint 3 Buffer Address Register              */
  
  union {
    __IOM uint32_t DOEPCTL4;                    /*!< (@ 0x00000080) Device Control OUT Endpoint 4 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL4_b;
  } ;
  __IM  uint32_t  RESERVED12;
  
  union {
    __IOM uint32_t DOEPINT4;                    /*!< (@ 0x00000088) Device OUT Endpoint 4 Interrupt Register                   */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT4_b;
  } ;
  __IM  uint32_t  RESERVED13;
  
  union {
    __IOM uint32_t DOEPTSIZ4;                   /*!< (@ 0x00000090) Device OUT Endpoint 4 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ4_b;
  } ;
  __IOM uint32_t  DOEPDMA4;                     /*!< (@ 0x00000094) Device OUT Endpoint 4 DMA Address Register                 */
  __IM  uint32_t  RESERVED14;
  __IM  uint32_t  DOEPDMAB4;                    /*!< (@ 0x0000009C) Device OUT Endpoint 4 Buffer Address Register              */
  
  union {
    __IOM uint32_t DOEPCTL5;                    /*!< (@ 0x000000A0) Device Control OUT Endpoint 5 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL5_b;
  } ;
  __IM  uint32_t  RESERVED15;
  
  union {
    __IOM uint32_t DOEPINT5;                    /*!< (@ 0x000000A8) Device OUT Endpoint 5 Interrupt Register                   */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT5_b;
  } ;
  __IM  uint32_t  RESERVED16[2];
  __IOM uint32_t  DOEPDMA5;                     /*!< (@ 0x000000B4) Device OUT Endpoint 5 DMA Address Register                 */
  __IM  uint32_t  RESERVED17;
  __IM  uint32_t  DOEPDMAB5;                    /*!< (@ 0x000000BC) Device OUT Endpoint 5 Buffer Address Register              */
  
  union {
    __IOM uint32_t DOEPCTL6;                    /*!< (@ 0x000000C0) Device Control OUT Endpoint 6 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL6_b;
  } ;
  __IM  uint32_t  RESERVED18;
  
  union {
    __IOM uint32_t DOEPINT6;                    /*!< (@ 0x000000C8) Device OUT Endpoint 6 Interrupt Register                   */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT6_b;
  } ;
  __IM  uint32_t  RESERVED19[2];
  __IOM uint32_t  DOEPDMA6;                     /*!< (@ 0x000000D4) Device OUT Endpoint 6 DMA Address Register                 */
  __IM  uint32_t  RESERVED20;
  __IM  uint32_t  DOEPDMAB6;                    /*!< (@ 0x000000DC) Device OUT Endpoint 6 Buffer Address Register              */
  
  union {
    __IOM uint32_t DOEPCTL7;                    /*!< (@ 0x000000E0) Device Control OUT Endpoint 7 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL7_b;
  } ;
  __IM  uint32_t  RESERVED21;
  
  union {
    __IOM uint32_t DOEPINT7;                    /*!< (@ 0x000000E8) Device OUT Endpoint 7 Interrupt Register                   */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT7_b;
  } ;
  __IM  uint32_t  RESERVED22[2];
  __IOM uint32_t  DOEPDMA7;                     /*!< (@ 0x000000F4) Device OUT Endpoint 7 DMA Address Register                 */
  __IM  uint32_t  RESERVED23;
  __IM  uint32_t  DOEPDMAB7;                    /*!< (@ 0x000000FC) Device OUT Endpoint 7 Buffer Address Register              */
  
  union {
    __IOM uint32_t DOEPCTL8;                    /*!< (@ 0x00000100) Device Control OUT Endpoint 8 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL8_b;
  } ;
  __IM  uint32_t  RESERVED24;
  
  union {
    __IOM uint32_t DOEPINT8;                    /*!< (@ 0x00000108) Device OUT Endpoint 8 Interrupt Register                   */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT8_b;
  } ;
  __IM  uint32_t  RESERVED25;
  
  union {
    __IOM uint32_t DOEPTSIZ5;                   /*!< (@ 0x00000110) Device OUT Endpoint 5 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ5_b;
  } ;
  __IOM uint32_t  DOEPDMA8;                     /*!< (@ 0x00000114) Device OUT Endpoint 8 DMA Address Register                 */
  __IM  uint32_t  RESERVED26;
  __IM  uint32_t  DOEPDMAB8;                    /*!< (@ 0x0000011C) Device OUT Endpoint 8 Buffer Address Register              */
  
  union {
    __IOM uint32_t DOEPCTL9;                    /*!< (@ 0x00000120) Device Control OUT Endpoint 9 Control Register             */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL9_b;
  } ;
  __IM  uint32_t  RESERVED27;
  
  union {
    __IOM uint32_t DOEPINT9;                    /*!< (@ 0x00000128) Device OUT Endpoint 9 Interrupt Register                   */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT9_b;
  } ;
  __IM  uint32_t  RESERVED28;
  
  union {
    __IOM uint32_t DOEPTSIZ6;                   /*!< (@ 0x00000130) Device OUT Endpoint 6 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ6_b;
  } ;
  __IOM uint32_t  DOEPDMA9;                     /*!< (@ 0x00000134) Device OUT Endpoint 9 DMA Address Register                 */
  __IM  uint32_t  RESERVED29;
  __IM  uint32_t  DOEPDMAB9;                    /*!< (@ 0x0000013C) Device OUT Endpoint 9 Buffer Address Register              */
  
  union {
    __IOM uint32_t DOEPCTL10;                   /*!< (@ 0x00000140) Device Control OUT Endpoint 10 Control Register            */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL10_b;
  } ;
  __IM  uint32_t  RESERVED30;
  
  union {
    __IOM uint32_t DOEPINT10;                   /*!< (@ 0x00000148) Device OUT Endpoint 10 Interrupt Register                  */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT10_b;
  } ;
  __IM  uint32_t  RESERVED31;
  
  union {
    __IOM uint32_t DOEPTSIZ7;                   /*!< (@ 0x00000150) Device OUT Endpoint 7 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ7_b;
  } ;
  __IOM uint32_t  DOEPDMA10;                    /*!< (@ 0x00000154) Device OUT Endpoint 10 DMA Address Register                */
  __IM  uint32_t  RESERVED32;
  __IM  uint32_t  DOEPDMAB10;                   /*!< (@ 0x0000015C) Device OUT Endpoint 10 Buffer Address Register             */
  
  union {
    __IOM uint32_t DOEPCTL11;                   /*!< (@ 0x00000160) Device Control OUT Endpoint 11 Control Register            */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL11_b;
  } ;
  __IM  uint32_t  RESERVED33;
  
  union {
    __IOM uint32_t DOEPINT11;                   /*!< (@ 0x00000168) Device OUT Endpoint 11 Interrupt Register                  */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT11_b;
  } ;
  __IM  uint32_t  RESERVED34;
  
  union {
    __IOM uint32_t DOEPTSIZ8;                   /*!< (@ 0x00000170) Device OUT Endpoint 8 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ8_b;
  } ;
  __IOM uint32_t  DOEPDMA11;                    /*!< (@ 0x00000174) Device OUT Endpoint 11 DMA Address Register                */
  __IM  uint32_t  RESERVED35;
  __IM  uint32_t  DOEPDMAB11;                   /*!< (@ 0x0000017C) Device OUT Endpoint 11 Buffer Address Register             */
  
  union {
    __IOM uint32_t DOEPCTL12;                   /*!< (@ 0x00000180) Device Control OUT Endpoint 12 Control Register            */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL12_b;
  } ;
  __IM  uint32_t  RESERVED36;
  
  union {
    __IOM uint32_t DOEPINT12;                   /*!< (@ 0x00000188) Device OUT Endpoint 12 Interrupt Register                  */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT12_b;
  } ;
  __IM  uint32_t  RESERVED37;
  
  union {
    __IOM uint32_t DOEPTSIZ9;                   /*!< (@ 0x00000190) Device OUT Endpoint 9 Transfer Size Register               */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ9_b;
  } ;
  __IOM uint32_t  DOEPDMA12;                    /*!< (@ 0x00000194) Device OUT Endpoint 12 DMA Address Register                */
  __IM  uint32_t  RESERVED38;
  __IM  uint32_t  DOEPDMAB12;                   /*!< (@ 0x0000019C) Device OUT Endpoint 12 Buffer Address Register             */
  
  union {
    __IOM uint32_t DOEPCTL13;                   /*!< (@ 0x000001A0) Device Control OUT Endpoint 13 Control Register            */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL13_b;
  } ;
  __IM  uint32_t  RESERVED39;
  
  union {
    __IOM uint32_t DOEPINT13;                   /*!< (@ 0x000001A8) Device OUT Endpoint 13 Interrupt Register                  */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT13_b;
  } ;
  __IM  uint32_t  RESERVED40;
  
  union {
    __IOM uint32_t DOEPTSIZ10;                  /*!< (@ 0x000001B0) Device OUT Endpoint 10 Transfer Size Register              */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ10_b;
  } ;
  __IOM uint32_t  DOEPDMA13;                    /*!< (@ 0x000001B4) Device OUT Endpoint 13 DMA Address Register                */
  __IM  uint32_t  RESERVED41;
  __IM  uint32_t  DOEPDMAB13;                   /*!< (@ 0x000001BC) Device OUT Endpoint 13 Buffer Address Register             */
  
  union {
    __IOM uint32_t DOEPCTL14;                   /*!< (@ 0x000001C0) Device Control OUT Endpoint 14 Control Register            */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL14_b;
  } ;
  __IM  uint32_t  RESERVED42;
  
  union {
    __IOM uint32_t DOEPINT14;                   /*!< (@ 0x000001C8) Device OUT Endpoint 14 Interrupt Register                  */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT14_b;
  } ;
  __IM  uint32_t  RESERVED43;
  
  union {
    __IOM uint32_t DOEPTSIZ11;                  /*!< (@ 0x000001D0) Device OUT Endpoint 11 Transfer Size Register              */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ11_b;
  } ;
  __IOM uint32_t  DOEPDMA14;                    /*!< (@ 0x000001D4) Device OUT Endpoint 14 DMA Address Register                */
  __IM  uint32_t  RESERVED44;
  __IM  uint32_t  DOEPDMAB14;                   /*!< (@ 0x000001DC) Device OUT Endpoint 14 Buffer Address Register             */
  
  union {
    __IOM uint32_t DOEPCTL15;                   /*!< (@ 0x000001E0) Device Control OUT Endpoint 15 Control Register            */
    
    struct {
      __IOM uint32_t MPS        : 11;           /*!< [10..0] Maximum packet size in bytes                                      */
            uint32_t            : 4;
      __IOM uint32_t USBActEP   : 1;            /*!< [15..15] USB active endpoint                                              */
      __IM  uint32_t DPID       : 1;            /*!< [16..16] Endpoint DATA PID (micro)frame number                            */
      __IM  uint32_t NAKSts     : 1;            /*!< [17..17] NAK status                                                       */
      __IOM uint32_t EPType     : 2;            /*!< [19..18] Endpoint type                                                    */
            uint32_t            : 1;
      __IOM uint32_t Stall      : 1;            /*!< [21..21] STALL handshake                                                  */
            uint32_t            : 4;
      __OM  uint32_t CNAK       : 1;            /*!< [26..26] Clear NAK                                                        */
      __OM  uint32_t SNAK       : 1;            /*!< [27..27] Set NAK                                                          */
      __OM  uint32_t SetD0PID   : 1;            /*!< [28..28] Set DATA0 PID / Even frame                                       */
      __OM  uint32_t SetD1PID   : 1;            /*!< [29..29] Set DATA1 PID / Odd frame                                        */
      __IOM uint32_t EPDis      : 1;            /*!< [30..30] Endpoint disable                                                 */
      __IOM uint32_t EPEna      : 1;            /*!< [31..31] Endpoint enable                                                  */
    } DOEPCTL15_b;
  } ;
  __IM  uint32_t  RESERVED45;
  
  union {
    __IOM uint32_t DOEPINT15;                   /*!< (@ 0x000001E8) Device OUT Endpoint 15 Interrupt Register                  */
    
    struct {
      __IOM uint32_t XferCompl  : 1;            /*!< [0..0] Transfer completed interrupt                                       */
      __IOM uint32_t EPDisbld   : 1;            /*!< [1..1] Endpoint disabled interrupt                                        */
      __IOM uint32_t AHBErr     : 1;            /*!< [2..2] AHB error                                                          */
      __IOM uint32_t SetUp      : 1;            /*!< [3..3] SETUP phase done                                                   */
      __IOM uint32_t OUTTknEPdis : 1;           /*!< [4..4] OUT token received when endpoint disabled                          */
      __IOM uint32_t StsPhseRcvd : 1;           /*!< [5..5] Status phase received (control write)                              */
      __IOM uint32_t Back2BackSETup : 1;        /*!< [6..6] Back-to-back SETUP packets received                                */
            uint32_t            : 1;
      __IOM uint32_t OutPktErr  : 1;            /*!< [8..8] OUT packet error                                                   */
      __IOM uint32_t BNAIntr    : 1;            /*!< [9..9] Buffer not available interrupt                                     */
            uint32_t            : 1;
      __IOM uint32_t PktDrpSts  : 1;            /*!< [11..11] ISOC OUT packet dropped                                          */
      __IOM uint32_t BbleErr    : 1;            /*!< [12..12] Babble error interrupt                                           */
      __IOM uint32_t NAKIntrpt  : 1;            /*!< [13..13] NAK interrupt                                                    */
      __IOM uint32_t NYETIntrpt : 1;            /*!< [14..14] NYET interrupt                                                   */
      __IOM uint32_t StupPktRcvd : 1;           /*!< [15..15] Setup packet received                                            */
            uint32_t            : 16;
    } DOEPINT15_b;
  } ;
  __IM  uint32_t  RESERVED46;
  
  union {
    __IOM uint32_t DOEPTSIZ12;                  /*!< (@ 0x000001F0) Device OUT Endpoint 12 Transfer Size Register              */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ12_b;
  } ;
  __IOM uint32_t  DOEPDMA15;                    /*!< (@ 0x000001F4) Device OUT Endpoint 15 DMA Address Register                */
  __IM  uint32_t  RESERVED47;
  __IM  uint32_t  DOEPDMAB15;                   /*!< (@ 0x000001FC) Device OUT Endpoint 15 Buffer Address Register             */
  __IM  uint32_t  RESERVED48[4];
  
  union {
    __IOM uint32_t DOEPTSIZ13;                  /*!< (@ 0x00000210) Device OUT Endpoint 13 Transfer Size Register              */
    
    struct {
      __IOM uint32_t XferSize   : 19;           /*!< [18..0] Transfer size in bytes                                            */
      __IOM uint32_t PktCnt     : 10;           /*!< [28..19] Packet count                                                     */
      __IM  uint32_t RxDPID     : 2;            /*!< [30..29] Received data PID / Setup packet count                           */
            uint32_t            : 1;
    } DOEPTSIZ13_b;
  } ;
} USB_OTG_OUT_EP_Type;                          /*!< Size = 532 (0x214)                                                        */



/* =========================================================================================================================== */
/* ================                                           LALU                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief LALU 1 (LALU)
  */

typedef struct {                                /*!< (@ 0x40050000) LALU Structure                                             */
  __IOM uint32_t  RESERVE;                      /*!< (@ 0x00000000) BYTE DATA                                                  */
} LALU_Type;                                    /*!< Size = 4 (0x4)                                                            */



/* =========================================================================================================================== */
/* ================                                            PUF                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief Crypto PUF (PUF)
  */

typedef struct {                                /*!< (@ 0x400D0000) PUF Structure                                              */
  __IOM uint32_t  RESERVE;                      /*!< (@ 0x00000000) BYTE DATA                                                  */
} PUF_Type;                                     /*!< Size = 4 (0x4)                                                            */



/* =========================================================================================================================== */
/* ================                                            LPT                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief LPT (LPT)
  */

typedef struct {                                /*!< (@ 0x40255300) LPT Structure                                              */
  __IOM uint32_t  RESERVE;                      /*!< (@ 0x00000000) BYTE DATA                                                  */
} LPT_Type;                                     /*!< Size = 4 (0x4)                                                            */



/* =========================================================================================================================== */
/* ================                                          PUFOTP                                           ================ */
/* =========================================================================================================================== */


/**
  * @brief PUFOTP (PUFOTP)
  */

typedef struct {                                /*!< (@ 0x40264000) PUFOTP Structure                                           */
  __IOM uint32_t  RESERVE;                      /*!< (@ 0x00000000) BYTE DATA                                                  */
} PUFOTP_Type;                                  /*!< Size = 4 (0x4)                                                            */


/** @} */ /* End of group Device_Peripheral_peripherals */


/* =========================================================================================================================== */
/* ================                          Device Specific Peripheral Address Map                           ================ */
/* =========================================================================================================================== */


/** @addtogroup Device_Peripheral_peripheralAddr
  * @{
  */

#define SHA2DMA_BASE                0x40041000UL
#define SHA3_BASE                   0x40042000UL
#define AES_BASE                    0x40043000UL
#define AES2_BASE                   0x40044000UL
#define DMA_BASE                    0x40002000UL
#define WDT_BASE                    0x40253F00UL
#define PWRBTN0_BASE                0x40253D00UL
#define PWRBTN1_BASE                0x40253E00UL
#define RTC_BASE                    0x40253C00UL
#define SLWTMR0_BASE                0x40253700UL
#define SLWTMR1_BASE                0x40253800UL
#define SLWTMR2_BASE                0x40253900UL
#define SLWTMR3_BASE                0x40253A00UL
#define TMR0_BASE                   0x40253100UL
#define TMR1_BASE                   0x40253200UL
#define TMR2_BASE                   0x40253300UL
#define TMR3_BASE                   0x40253400UL
#define TMR4_BASE                   0x40253500UL
#define TMR5_BASE                   0x40253600UL
#define RTMR_BASE                   0x40253B00UL
#define I2C1_BASE                   0x40250000UL
#define I2C2_BASE                   0x40250200UL
#define I2C3_BASE                   0x40250400UL
#define I2C4_BASE                   0x40250600UL
#define I2C5_BASE                   0x40250800UL
#define I2C6_BASE                   0x40250A00UL
#define I2C7_BASE                   0x40250C00UL
#define I2C8_BASE                   0x40250E00UL
#define I2C9_BASE                   0x40251000UL
#define I2C10_BASE                  0x40251200UL
#define I2C11_BASE                  0x40251400UL
#define I3C0_BASE                   0x40003000UL
#define I3C1_BASE                   0x40004000UL
#define PWM1_BASE                   0x40254500UL
#define PWM2_BASE                   0x40254600UL
#define PWM3_BASE                   0x40254700UL
#define PWM4_BASE                   0x40254800UL
#define PWM5_BASE                   0x40254900UL
#define PWM6_BASE                   0x40254A00UL
#define PWM7_BASE                   0x40254B00UL
#define PWM8_BASE                   0x40254C00UL
#define PWM9_BASE                   0x40254D00UL
#define PWM10_BASE                  0x40254E00UL
#define PWM11_BASE                  0x40254F00UL
#define PWM12_BASE                  0x40255000UL
#define PWM13_BASE                  0x40255100UL
#define PWM14_BASE                  0x40255200UL
#define LED1_BASE                   0x40252D00UL
#define LED2_BASE                   0x40252E00UL
#define LED3_BASE                   0x40252F00UL
#define LED4_BASE                   0x40253000UL
#define TACHO1_BASE                 0x40251F00UL
#define TACHO2_BASE                 0x40252000UL
#define TACHO3_BASE                 0x40252100UL
#define TACHO4_BASE                 0x40252200UL
#define TACHO5_BASE                 0x40252300UL
#define TACHO6_BASE                 0x40252400UL
#define TACHO7_BASE                 0x40252500UL
#define TACHO8_BASE                 0x40252600UL
#define TACHO9_BASE                 0x40252700UL
#define TACHO10_BASE                0x40252800UL
#define TACHO11_BASE                0x40252900UL
#define TACHO12_BASE                0x40252A00UL
#define TACHO13_BASE                0x40252B00UL
#define TACHO14_BASE                0x40252C00UL
#define ADC_BASE                    0x40251900UL
#define PS2_1_BASE                  0x40254300UL
#define PS2_2_BASE                  0x40254400UL
#define PS2_3_BASE                  0x40255700UL
#define KBM_BASE                    0x40254200UL
#define UART_BASE                   0x40251600UL
#define UART1_BASE                  0x40251700UL
#define UART2_BASE                  0x40251800UL
#define SPIC1_BASE                  0x40000000UL
#define SPIC2_BASE                  0x40001000UL
#define SPIS_BASE                   0x40222400UL
#define T1HZ_BASE                   0x40010830UL
#define EFUSE_BASE                  0x40010900UL
#define SYSTEM_BASE                 0x40100000UL
#define PKE_BASE                    0x40080000UL
#define PQC_BASE                    0x400C0000UL
#define EMI0_BASE                   0x40221000UL
#define EMI1_BASE                   0x40221020UL
#define EMI2_BASE                   0x40221040UL
#define EMI3_BASE                   0x40221060UL
#define EMI4_BASE                   0x40221080UL
#define EMI5_BASE                   0x402210A0UL
#define EMI6_BASE                   0x402210C0UL
#define EMI7_BASE                   0x402210E0UL
#define GPIO_BASE                   0x40230000UL
#define IOPAD_BASE                  0x40091000UL
#define PECI_BASE                   0x40221900UL
#define KBC_BASE                    0x40221700UL
#define ACPI_BASE                   0x40221400UL
#define PMPORT0_BASE                0x4022141CUL
#define PMPORT1_BASE                0x40221438UL
#define PMPORT2_BASE                0x40221454UL
#define PMPORT3_BASE                0x40221470UL
#define DIO0_BASE                   0x400A02C0UL
#define DIO1_BASE                   0x400A02C8UL
#define PORT80_BASE                 0x40221200UL
#define ESPI_BASE                   0x40220000UL
#define MONOTONIC_BASE              0x400B1400UL
#define MBX_BASE                    0x40221600UL
#define LEDSTP_BASE                 0x40251A00UL
#define LEDDBG0_BASE                0x4000F930UL
#define LEDDBG1_BASE                0x4000F944UL
#define SPIM_BASE                   0x40221B00UL
#define MICROCNT_BASE               0x40255400UL
#define CEC_BASE                    0x40254000UL
#define CEC1_BASE                   0x40254100UL
#define I2CDBG_BASE                 0x40251C00UL
#define BRAM_BASE                   0x20200000UL
#define TRAM_BASE                   0x40255600UL
#define USB_CORE1_BASE              0x50000000UL
#define USB_HOST1_BASE              0x50000400UL
#define USB_DEVICE1_BASE            0x50000800UL
#define USB_IN_EP1_BASE             0x50000900UL
#define USB_OUT_EP1_BASE            0x50000B00UL
#define USB_CORE2_BASE              0x50040000UL
#define USB_HOST2_BASE              0x50040400UL
#define USB_DEVICE2_BASE            0x50040800UL
#define USB_IN_EP2_BASE             0x50040900UL
#define USB_OUT_EP2_BASE            0x50040B00UL
#define LALU_BASE                   0x40050000UL
#define LALU2_BASE                  0x40090000UL
#define PUF_BASE                    0x400D0000UL
#define LPT_BASE                    0x40255300UL
#define PUFOTP_BASE                 0x40264000UL

/** @} */ /* End of group Device_Peripheral_peripheralAddr */


/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */


/** @addtogroup Device_Peripheral_declaration
  * @{
  */

#define SHA2DMA                     ((SHA2DMA_Type*)           SHA2DMA_BASE)
#define SHA3                        ((SHA3_Type*)              SHA3_BASE)
#define AES                         ((AES_Type*)               AES_BASE)
#define AES2                        ((AES_Type*)               AES2_BASE)
#define DMA                         ((DMA_Type*)               DMA_BASE)
#define WDT                         ((WDT_Type*)               WDT_BASE)
#define PWRBTN0                     ((PWRBTN_Type*)            PWRBTN0_BASE)
#define PWRBTN1                     ((PWRBTN_Type*)            PWRBTN1_BASE)
#define RTC                         ((RTC_Type*)               RTC_BASE)
#define SLWTMR0                     ((TIMER_Type*)             SLWTMR0_BASE)
#define SLWTMR1                     ((TIMER_Type*)             SLWTMR1_BASE)
#define SLWTMR2                     ((TIMER_Type*)             SLWTMR2_BASE)
#define SLWTMR3                     ((TIMER_Type*)             SLWTMR3_BASE)
#define TMR0                        ((TIMER_Type*)             TMR0_BASE)
#define TMR1                        ((TIMER_Type*)             TMR1_BASE)
#define TMR2                        ((TIMER_Type*)             TMR2_BASE)
#define TMR3                        ((TIMER_Type*)             TMR3_BASE)
#define TMR4                        ((TIMER_Type*)             TMR4_BASE)
#define TMR5                        ((TIMER_Type*)             TMR5_BASE)
#define RTMR                        ((TIMER_Type*)             RTMR_BASE)
#define I2C1                        ((I2C_Type*)               I2C1_BASE)
#define I2C2                        ((I2C_Type*)               I2C2_BASE)
#define I2C3                        ((I2C_Type*)               I2C3_BASE)
#define I2C4                        ((I2C_Type*)               I2C4_BASE)
#define I2C5                        ((I2C_Type*)               I2C5_BASE)
#define I2C6                        ((I2C_Type*)               I2C6_BASE)
#define I2C7                        ((I2C_Type*)               I2C7_BASE)
#define I2C8                        ((I2C_Type*)               I2C8_BASE)
#define I2C9                        ((I2C_Type*)               I2C9_BASE)
#define I2C10                       ((I2C_Type*)               I2C10_BASE)
#define I2C11                       ((I2C_Type*)               I2C11_BASE)
#define I3C0                        ((I3C_Type*)               I3C0_BASE)
#define I3C1                        ((I3C_Type*)               I3C1_BASE)
#define PWM1                        ((PWM_Type*)               PWM1_BASE)
#define PWM2                        ((PWM_Type*)               PWM2_BASE)
#define PWM3                        ((PWM_Type*)               PWM3_BASE)
#define PWM4                        ((PWM_Type*)               PWM4_BASE)
#define PWM5                        ((PWM_Type*)               PWM5_BASE)
#define PWM6                        ((PWM_Type*)               PWM6_BASE)
#define PWM7                        ((PWM_Type*)               PWM7_BASE)
#define PWM8                        ((PWM_Type*)               PWM8_BASE)
#define PWM9                        ((PWM_Type*)               PWM9_BASE)
#define PWM10                       ((PWM_Type*)               PWM10_BASE)
#define PWM11                       ((PWM_Type*)               PWM11_BASE)
#define PWM12                       ((PWM_Type*)               PWM12_BASE)
#define PWM13                       ((PWM_Type*)               PWM13_BASE)
#define PWM14                       ((PWM_Type*)               PWM14_BASE)
#define LED1                        ((PWMLED_Type*)            LED1_BASE)
#define LED2                        ((PWMLED_Type*)            LED2_BASE)
#define LED3                        ((PWMLED_Type*)            LED3_BASE)
#define LED4                        ((PWMLED_Type*)            LED4_BASE)
#define TACHO1                      ((TACH1_Type*)             TACHO1_BASE)
#define TACHO2                      ((TACH1_Type*)             TACHO2_BASE)
#define TACHO3                      ((TACH1_Type*)             TACHO3_BASE)
#define TACHO4                      ((TACH1_Type*)             TACHO4_BASE)
#define TACHO5                      ((TACH1_Type*)             TACHO5_BASE)
#define TACHO6                      ((TACH1_Type*)             TACHO6_BASE)
#define TACHO7                      ((TACH1_Type*)             TACHO7_BASE)
#define TACHO8                      ((TACH1_Type*)             TACHO8_BASE)
#define TACHO9                      ((TACH1_Type*)             TACHO9_BASE)
#define TACHO10                     ((TACH1_Type*)             TACHO10_BASE)
#define TACHO11                     ((TACH1_Type*)             TACHO11_BASE)
#define TACHO12                     ((TACH1_Type*)             TACHO12_BASE)
#define TACHO13                     ((TACH1_Type*)             TACHO13_BASE)
#define TACHO14                     ((TACH1_Type*)             TACHO14_BASE)
#define ADC                         ((ADC_Type*)               ADC_BASE)
#define PS2_1                       ((PS2_Type*)               PS2_1_BASE)
#define PS2_2                       ((PS2_Type*)               PS2_2_BASE)
#define PS2_3                       ((PS2_Type*)               PS2_3_BASE)
#define KBM                         ((KBM_Type*)               KBM_BASE)
#define UART                        ((UART_Type*)              UART_BASE)
#define UART1                       ((UART_Type*)              UART1_BASE)
#define UART2                       ((UART_Type*)              UART2_BASE)
#define SPIC1                       ((SPIC_Type*)              SPIC1_BASE)
#define SPIC2                       ((SPIC_Type*)              SPIC2_BASE)
#define SPIS                        ((SPIS_Type*)              SPIS_BASE)
#define T1HZ                        ((T1HZ_Type*)              T1HZ_BASE)
#define EFUSE                       ((EFUSE_Type*)             EFUSE_BASE)
#define SYSTEM                      ((SYSTEM_Type*)            SYSTEM_BASE)
#define PKE                         ((PKE_Type*)               PKE_BASE)
#define PQC                         ((PQC_Type*)               PQC_BASE)
#define EMI0                        ((EMI_Type*)               EMI0_BASE)
#define EMI1                        ((EMI_Type*)               EMI1_BASE)
#define EMI2                        ((EMI_Type*)               EMI2_BASE)
#define EMI3                        ((EMI_Type*)               EMI3_BASE)
#define EMI4                        ((EMI_Type*)               EMI4_BASE)
#define EMI5                        ((EMI_Type*)               EMI5_BASE)
#define EMI6                        ((EMI_Type*)               EMI6_BASE)
#define EMI7                        ((EMI_Type*)               EMI7_BASE)
#define GPIO                        ((GPIO_Type*)              GPIO_BASE)
#define IOPAD                       ((IOPAD_Type*)             IOPAD_BASE)
#define PECI                        ((PECI_Type*)              PECI_BASE)
#define KBC                         ((KBC_Type*)               KBC_BASE)
#define ACPI                        ((ACPI_Type*)              ACPI_BASE)
#define PMPORT0                     ((PMPORT_Type*)            PMPORT0_BASE)
#define PMPORT1                     ((PMPORT_Type*)            PMPORT1_BASE)
#define PMPORT2                     ((PMPORT_Type*)            PMPORT2_BASE)
#define PMPORT3                     ((PMPORT_Type*)            PMPORT3_BASE)
#define DIO0                        ((DIO_Type*)               DIO0_BASE)
#define DIO1                        ((DIO_Type*)               DIO1_BASE)
#define PORT80                      ((PORT80_Type*)            PORT80_BASE)
#define ESPI                        ((ESPI_Type*)              ESPI_BASE)
#define MONOTONIC                   ((MONOTONIC_Type*)         MONOTONIC_BASE)
#define MBX                         ((MBX_Type*)               MBX_BASE)
#define LEDSTP                      ((LEDSTP_Type*)            LEDSTP_BASE)
#define LEDDBG0                     ((LEDDBG_Type*)            LEDDBG0_BASE)
#define LEDDBG1                     ((LEDDBG_Type*)            LEDDBG1_BASE)
#define SPIM                        ((SPIM_Type*)              SPIM_BASE)
#define MICROCNT                    ((MICROCNT_Type*)          MICROCNT_BASE)
#define CEC                         ((CEC_Type*)               CEC_BASE)
#define CEC1                        ((CEC_Type*)               CEC1_BASE)
#define I2CDBG                      ((I2CDBG_Type*)            I2CDBG_BASE)
#define BRAM                        ((BRAM_Type*)              BRAM_BASE)
#define TRAM                        ((TRAM_Type*)              TRAM_BASE)
#define USB_CORE1                   ((USB_OTG_CORE_Type*)      USB_CORE1_BASE)
#define USB_HOST1                   ((USB_OTG_HOST_Type*)      USB_HOST1_BASE)
#define USB_DEVICE1                 ((USB_OTG_DEVICE_Type*)    USB_DEVICE1_BASE)
#define USB_IN_EP1                  ((USB_OTG_IN_EP_Type*)     USB_IN_EP1_BASE)
#define USB_OUT_EP1                 ((USB_OTG_OUT_EP_Type*)    USB_OUT_EP1_BASE)
#define USB_CORE2                   ((USB_OTG_CORE_Type*)      USB_CORE2_BASE)
#define USB_HOST2                   ((USB_OTG_HOST_Type*)      USB_HOST2_BASE)
#define USB_DEVICE2                 ((USB_OTG_DEVICE_Type*)    USB_DEVICE2_BASE)
#define USB_IN_EP2                  ((USB_OTG_IN_EP_Type*)     USB_IN_EP2_BASE)
#define USB_OUT_EP2                 ((USB_OTG_OUT_EP_Type*)    USB_OUT_EP2_BASE)
#define LALU                        ((LALU_Type*)              LALU_BASE)
#define LALU2                       ((LALU_Type*)              LALU2_BASE)
#define PUF                         ((PUF_Type*)               PUF_BASE)
#define LPT                         ((LPT_Type*)               LPT_BASE)
#define PUFOTP                      ((PUFOTP_Type*)            PUFOTP_BASE)

/* EMI0CTRL */
#define ESPI_EMI0CTRL_ENABLE    BIT(0)
#define ESPI_EMI0CTRL_SIZE_256  BIT(10)
#define ESPI_EMI0CTRL_SIZE_1K   BIT(9) | BIT(10)
#define ESPI_EMI0CTRL_SIZE_2K   BIT(8) | BIT(9) | BIT(10)
#define ESPI_EMI0CTRL_SIZE_4K   BIT(11)

/** @} */ /* End of group Device_Peripheral_declaration */
