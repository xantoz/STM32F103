/**
 * @brief Register definitions etc. for internal use by the nRF24L01 driver
 *
 * @file  nRF24L01_internal.h
 *
 * @note Since it is only for internal use by nRF24L01.c, flags etc. the prefix is in many cases omitted
 */

#ifndef _NRF24L01_INTERNAL_
#define _NRF24L01_INTERNAL_

/**
 * @brief Bit definitions for nRF24L01 commands
 *
 * @note The W_REGISTER and R_REGISTER commands can operate on single or multi-byte registers. When
 *       accessing multi-byte registers you read or write to the MSBit of LSByte first. You can
 *       terminate the writing before all bytes in a multi-byte register are written, leaving the
 *       unwritten MSByte(s) unchanged. For exam- ple, the LSByte of RX_ADDR_P0 can be modified by
 *       writing only one byte to the RX_ADDR_P0 register. The content of the status register is
 *       always read to MISO after a high to low transition on CSN. enum nRF24L01_Commands
 */
enum nRF24L01_Command
{
    //! 0b000AAAAA, where AAAAA = 5 bit reg addr. 1 to 5 Data bytes LSByte first
    R_REGISTER   = 0b00000000,
    //! 0b001AAAAA, where AAAAA = 5 bit reg addr. 1 to 5 Data bytes LSByte first. Executable in
    //! power down or stand by modes only
    W_REGISTER   = 0b00100000,
    //! 1 to 32 data bytes, LSByte first. A read operation always starts at byte 0.
    R_RX_PAYLOAD = 0b01100001,
    //! 1 to 32 data bytes, LSByte first. A write operation always starts at byte 0.
    W_TX_PAYLOAD = 0b10100000,
    //! Flush TX FIFO, 0 data bytes
    FLUSH_TX     = 0b11100010,
    //! Flush RX FIFO, 0 data bytes
    FLUSH_RX     = 0b11100010,
    //! Used for a PTX device. Repeatedly transmit last package as long as CE is high
    REUSE_TX_PL  = 0b11100011,

    /** This write command followed by data 0x73 activates the following features:
          • R_RX_PL_WID
          • W_ACK_PAYLOAD
          • W_TX_PAYLOAD_NOACK
        A new ACTIVATE command with the same data deactivates them again. This is executable in
        power down or stand by modes only. The R_RX_PL_WID, W_ACK_PAYLOAD, and W_TX_PAYLOAD_NOACK
        features registers are initially in a deactivated state; a write has no effect, a read only
        results in zeros on MISO. To activate these registers, use the ACTIVATE command followed by
        data 0x73. Then they can be accessed as any other register in nRF24L01. Use the same command
        and data to deactivate the registers again. */
    ACTIVATE     = 0b01010000,

    R_RX_PL_WID   = 0b01100000, //!< Read RX-payload width for the top R_RX_PAYLOAD in the RX FIFO.

    /** 0b10101PPP | Used in RX mode. Write Payload to be transmitted together with ACK packet on
        PIPE PPP. (PPP valid in the range from 000 to 101). Maximum three ACK packet payloads can be
        pending. Payloads with same PPP are handled using first in - first out principle. Write
        payload: 1– 32 bytes. A write operation always starts at byte 0.
     */
    W_ACK_PAYLOAD = 0b10101000, //!< 1 to 32 data bytes, LSByte FIRST

    W_TX_PAYLOAD_NO_ACK = 0b1011000, //!< Used in TX mode. Disables AUTOACK on this specific packet

    NOP          = 0b11111111, //!< NOP. Can be used to read the STATUS register
};

/**
 * @brief nRF24L01 register map
 */
enum nRF24L01_Register
{
    CONFIG_Reg     = 0x00, //!< Config register
    EN_AA_Reg      = 0x01, //!< Enhanced Shockburst(TM) (pipe 0 to 5)
    EN_RXADDR_Reg  = 0x02, //!< Enable RX Address (pipe 0 to 5)
    SETUP_AW_Reg   = 0x03, //!< Setup of Address Widths (common for all data pipes)
    SETUP_RETR_Reg = 0x04, //!< Setup of Automatic Retransmission
    RF_CH_Reg      = 0x05, //!< RF channel (bits 6:0)
    RF_SETUP_Reg   = 0x06, //!< RF Setup Register
    STATUS_Reg     = 0x07, //!< STATUS register
    OBSERVE_TX_Reg = 0x08, //!< Transmit observer register
    CD_Reg         = 0x09, //!< Carrier detect register

    /** Receive address data pipe 0. 5 Bytes maximum length. (LSByte is written first. Write the
        number of bytes defined by SETUP_AW) */
    RX_ADDR_P0_Reg = 0x0A, //!< [rst: 0xE7E7E7E7E7]
    /** Receive address data pipe 1. 5 Bytes maximum length. (LSByte is written first. Write the
        number of bytes defined by SETUP_AW) */
    RX_ADDR_P1_Reg = 0x0B, //!< [rst: 0xC2C2C2C2C2]
    //! Receive address data pipe 2. Only LSB. MSBytes is equal to RX_ADDR_P1[39:8]
    RX_ADDR_P2_Reg = 0x0C, //!< [rst: 0xC3]
    //! Receive address data pipe 3. Only LSB. MSBytes is equal to RX_ADDR_P1[39:8]
    RX_ADDR_P3_Reg = 0x0D, //!< [rst: 0xC4]
    //! Receive address data pipe 4. Only LSB. MSBytes is equal to RX_ADDR_P1[39:8]
    RX_ADDR_P4_Reg = 0x0E, //!< [rst: 0xC5]
    //! Receive address data pipe 5. Only LSB. MSBytes is equal to RX_ADDR_P1[39:8]
    RX_ADDR_P5_Reg = 0x0F, //!< [rst: 0xC6]

    /** Transmit address. Used for a PTX device only. (LSByte is written first) Set RX_ADDR_P0 equal
       to this address to handle automatic acknowledge if this is a PTX device with Enhanced
       ShockBurst(TM) enabled */
    TX_ADDR_Reg = 0x10, //!< [rst: 0xE7E7E7E7E7]

    RX_PW_P0_Reg = 0x11, //!< bits [5:0]: Number of bytes in RX payloid in data pipe 0 (1 to 32 bytes)
    RX_PW_P1_Reg = 0x12, //!< bits [5:0]: Number of bytes in RX payloid in data pipe 1 (1 to 32 bytes)
    RX_PW_P2_Reg = 0x13, //!< bits [5:0]: Number of bytes in RX payloid in data pipe 2 (1 to 32 bytes)
    RX_PW_P3_Reg = 0x14, //!< bits [5:0]: Number of bytes in RX payloid in data pipe 3 (1 to 32 bytes)
    RX_PW_P4_Reg = 0x15, //!< bits [5:0]: Number of bytes in RX payloid in data pipe 4 (1 to 32 bytes)
    RX_PW_P5_Reg = 0x16, //!< bits [5:0]: Number of bytes in RX payloid in data pipe 5 (1 to 32 bytes)

    FIFO_STATUS_Reg = 0x17, //!< FIFO status register. READ ONLY

    DYNPD_Reg = 0x1C,   //!< Enable dynamic payload length
    FEATURE_Reg = 0x1D, //!< Feature register
};

/**
 * @brief CONFIG register bit definition
 */
enum nRF24L01_CONFIG_Flg
{
    CONFIG_MASK_RX_DR  = 0x1 << 6, //!< [rst: 0] Mask interrupt caused by RX_DR. 1 == mask, 0 == no mask
    CONFIG_MASK_TX_DS  = 0x1 << 5, //!< [rst: 0] Mask interrupt caused by TX_DS. 1 == mask, 0 == no mask
    CONFIG_MASK_MAX_RT = 0x1 << 4, //!< [rst: 0] Mask interrupt caused by MAX_RT. 1 == mask, 0 == no mask
    CONFIG_EN_CRC      = 0x1 << 3, //!< [rst: 0] Enable CRC. Forced high if some bit in EN_AA is set
    CONFIG_CRC0        = 0x1 << 2, //!< [rst: 0] CRC Encoding scheme: 0 == 1 byte, 1 == 2 bytes
    CONFIG_PWR_UP      = 0x1 << 1, //!< [rst: 0] 1: POWER UP, 0: POWER DOWN
    CONFIG_PRIM_RX     = 0x1 << 0, //!< [rst: 0] RX/TX control. 1 == PRX, 0 == PTX
};

/**
 * @brief EN_AA register bit definition
 */
enum EN_AA_Reg
{
    EN_AA_ENAA_P0 = 1 << 0, //!< [rst: 1] Enable auto-ack for data pipe 0
    EN_AA_ENAA_P1 = 1 << 1, //!< [rst: 1] Enable auto-ack for data pipe 1
    EN_AA_ENAA_P2 = 1 << 2, //!< [rst: 1] Enable auto-ack for data pipe 2
    EN_AA_ENAA_P3 = 1 << 3, //!< [rst: 1] Enable auto-ack for data pipe 3
    EN_AA_ENAA_P4 = 1 << 4, //!< [rst: 1] Enable auto-ack for data pipe 4
    EN_AA_ENAA_P5 = 1 << 5, //!< [rst: 1] Enable auto-ack for data pipe 5

    EN_AA_ENAA_None = 0x00,
    EN_AA_ENAA_All = (EN_AA_ENAA_P0 | EN_AA_ENAA_P1 | EN_AA_ENAA_P2 |
                      EN_AA_ENAA_P3 | EN_AA_ENAA_P4 | EN_AA_ENAA_P5),
};

/**
 * @brief EN_RXADDR register bit definition
 */
enum EN_RXADDR_Flg
{
    EN_RXADDR_ERX_P0 = 1 << 0, //!< [rst: 0] Enable data pipe 0
    EN_RXADDR_ERX_P1 = 1 << 1, //!< [rst: 1] Enable data pipe 1
    EN_RXADDR_ERX_P2 = 1 << 2, //!< [rst: 0] Enable data pipe 2
    EN_RXADDR_ERX_P3 = 1 << 3, //!< [rst: 0] Enable data pipe 3
    EN_RXADDR_ERX_P4 = 1 << 4, //!< [rst: 0] Enable data pipe 4
    EN_RXADDR_ERX_P5 = 1 << 5, //!< [rst: 0] Enable data pipe 5

    EN_RXADDR_ERX_All = (EN_RXADDR_ERX_P0 | EN_RXADDR_ERX_P1 | EN_RXADDR_ERX_P2 |
                         EN_RXADDR_ERX_P3 | EN_RXADDR_ERX_P4 | EN_RXADDR_ERX_P5),
};

/**
 * @brief SETUP_AW register bit definition
 */
enum nRF24L01_SETUP_AW_Flg
{
    SETUP_AW_AW        = 0b00000011, //!< [rst: 0b11] RX/TX address field width
    SETUP_AW_AW_3bytes = 0b00000001, //!< RX/TX address field 3 bytes wide
    SETUP_AW_AW_4bytes = 0b00000010, //!< RX/TX address field 4 bytes wide
    SETUP_AW_AW_5bytes = 0b00000011, //!< RX/TX address field 5 bytes wide (default)
};

/**
 * @brief SETUP_RETR register bit definition
 */
enum nRF24L01_SETUP_RETR_Flg
{
    SETUP_RETR_ARD        = 0b1111 << 4, //!< [rst: 0b0000] Auto retransmit delay
    SETUP_RETR_ARD_250us  = 0 << 4,
    SETUP_RETR_ARD_500us  = 1 << 4,
    SETUP_RETR_ARD_750us  = 2 << 4,
    SETUP_RETR_ARD_1000us = 3 << 4,
    SETUP_RETR_ARD_1250us = 4 << 4,
    SETUP_RETR_ARD_1500us = 5 << 4,
    SETUP_RETR_ARD_1750us = 6 << 4,
    SETUP_RETR_ARD_2000us = 7 << 4,
    SETUP_RETR_ARD_2250us = 8 << 4,
    SETUP_RETR_ARD_2500us = 9 << 4,
    SETUP_RETR_ARD_2750us = 10 << 4,
    SETUP_RETR_ARD_3000us = 11 << 4,
    SETUP_RETR_ARD_3250us = 12 << 4,
    SETUP_RETR_ARD_3500us = 13 << 4,
    SETUP_RETR_ARD_3750us = 14 << 4,
    SETUP_RETR_ARD_4000us = 15 << 4,

    SETUP_RETR_ARC                    = 0b1111 << 0, //!< [rst: 0b0011] Auto retransmit count
    SETUP_RETR_ARC_Retransmit_Disable = 0 << 0,
    SETUP_RETR_ARC_Retransmit_1       = 1 << 0,
    SETUP_RETR_ARC_Retransmit_2       = 2 << 0,
    SETUP_RETR_ARC_Retransmit_3       = 3 << 0,
    SETUP_RETR_ARC_Retransmit_4       = 4 << 0,
    SETUP_RETR_ARC_Retransmit_5       = 5 << 0,
    SETUP_RETR_ARC_Retransmit_6       = 6 << 0,
    SETUP_RETR_ARC_Retransmit_7       = 7 << 0,
    SETUP_RETR_ARC_Retransmit_8       = 8 << 0,
    SETUP_RETR_ARC_Retransmit_9       = 9 << 0,
    SETUP_RETR_ARC_Retransmit_10      = 10 << 0,
    SETUP_RETR_ARC_Retransmit_11      = 11 << 0,
    SETUP_RETR_ARC_Retransmit_12      = 12 << 0,
    SETUP_RETR_ARC_Retransmit_13      = 13 << 0,
    SETUP_RETR_ARC_Retransmit_14      = 14 << 0,
    SETUP_RETR_ARC_Retransmit_15      = 15 << 0,
};

/**
 * @brief RF_SETUP register bit definition
 */
enum nRF24L01_RF_SETUP_Flg
{
    RF_SETUP_PLL_LOCK          = 0x1 << 4,  //!< Force PLL lock signal, only used in test
    RF_SETUP_RF_DR             = 0x1 << 3,  //!< Air data rate. 0 == 1Mbps, 1 == 2Mbps

    RF_SETUP_RF_PWR            = 0b11 << 1, //!< [rst: 0b11] Set RF output power in TX mode
    RF_SETUP_RF_PWR_Minus18dBm = 0b00 << 1,
    RF_SETUP_RF_PWR_Minus12dBm = 0b01 << 1,
    RF_SETUP_RF_PWR_Minus6dBm  = 0b10 << 1,
    RF_SETUP_RF_PWR_Minus0dBm  = 0b11 << 1,

    RF_SETUP_LNA_HCURR         = 0x1,       //!< Setup LNA gain
};

/**
 * @brief STATUS register bit definition
 */
enum nRF24L01_STATUS
{
    //! Data Ready RX FIFO interrupt. Asserted when new data arrives on RX FIFO. Write 1 to clear bit.
    STATUS_RX_DR = 0x1 << 6,    //!< [rst: 0]
    //! Data Sent TX FIFO interrupt. Asserted when packed transmitted on TX. if AUTO_ACK is
    //! activated, this bit is set high only when ACK is recieved. Write 1 to clear bit.
    STATUS_TX_DS = 0x1 << 5,    //!< [rst: 0]
    //! Maximum number of TX retransmits interrupt. Write 1 to clear bit. If MAX_RT is asserted it
    //! must be cleared to enable further communication.
    STATUS_MAX_RT  = 0x1 << 4,  //!< [rst: 0]

    //! Data pipe number for the payload available for reading from RX_FIFO
    STATUS_RX_P_NO = 0b111 << 1,       //!< [rst: 0b111]
    STATUS_RX_P_NO_Pos = 1,
    STATUS_RX_P_NO_Pipe0 = 0b001 << 1,
    STATUS_RX_P_NO_Pipe1 = 0b010 << 1,
    STATUS_RX_P_NO_Pipe2 = 0b011 << 1,
    STATUS_RX_P_NO_Pipe3 = 0b100 << 1,
    STATUS_RX_P_NO_Pipe4 = 0b101 << 1,
    STATUS_RX_P_NO_Pipe5 = 0b110 << 1,
    STATUS_RX_P_NO_RX_FIFO_EMPTY = 0b111 << 1,

    STATUS_TX_FULL = 0x1 << 0, //!< [rst: 0] TX FIFO full flag. 1 == FIFO full, 0 == FIFO not full
};

/**
 * @brief OBSERVE_TX register bit definition
 */
enum nRF24L01_OBSERVE_TX_Flg
{
    //! Count lost packets. The counter is overflow protected to 15, and discontinues at max until
    //! reset. The counter is reset by writing to RF_CH.
    OBSERVE_TX_PLOS_CNT = 0b11111000,
    OBSERVE_TX_ARC_CNT  = 0b00000111,
};

/**
 * @brief CD register bit definition
 */
enum nRF24L01_CD_Flg
{
    CD_CD = 0b00000001, //!< Carrier detect
};

/**
 * @brief FIFO_STATUS register bit definition
 */
enum nRF24L01_FIFO_STATUS_Flg
{
    /** Reuse last transmitted data packet if set high. The packet is repeatedly retransmitted as
        long as CE is high. TX_REUSE is set by the SPI com- mand REUSE_TX_PL, and is reset by the
        SPI commands W_TX_PAYLOAD or FLUSH TX */
    FIFO_STATUS_TX_REUSE = 0x1 << 6, //!< [rst: 0] (RO)
    FIFO_STATUS_TX_FULL  = 0x1 << 5, //!< [rst: 0] TX FIFO full flag  (RO)
    FIFO_STATUS_TX_EMPTY = 0x1 << 4, //!< [rst: 1] TX FIFO empty flag (RO)
    FIFO_STATUS_RX_FULL  = 0x1 << 1, //!< [rst: 0] RX FIFO full flag  (RO)
    FIFO_STATUS_RX_EMPTY = 0x1 << 0, //!< [rst: 0] RX FIFO empty flag (RO)
};

/**
 * @brief DYNPD register bit definition
 */
enum nRF24L01_DYNPD_Flg
{
    DYNPD_DPL_P5 = 0x1 << 5, //!< Enable dyn. payload length for data pipe 5
    DYNPD_DPL_P4 = 0x1 << 4, //!< Enable dyn. payload length for data pipe 4
    DYNPD_DPL_P3 = 0x1 << 3, //!< Enable dyn. payload length for data pipe 3
    DYNPD_DPL_P2 = 0x1 << 2, //!< Enable dyn. payload length for data pipe 2
    DYNPD_DPL_P1 = 0x1 << 1, //!< Enable dyn. payload length for data pipe 1
    DYNPD_DPL_P0 = 0x1 << 0, //!< Enable dyn. payload length for data pipe 0
};

/**
 * @brief FEATURE register bit definition
 */
enum nRF24L01_FEATURE_Flg
{
    FEATURE_EN_DPL     = 0x1 << 2, //!< Enable dynamic payload length
    FEATURE_EN_ACK_PAY = 0x1 << 1, //!< Enable payload with ACK
    FEATURE_EN_DYN_ACK = 0x1 << 0, //!< Enables the W_TX_PAYLOAD_NOACK command
};

#define REGISTER_MASK 0b00011111

#define DUMMY 0xDE   // Dummy bits sent when we just want to receive

#endif /* _NRF24L01_INTERNAL_ */
