#ifndef _SPI_
#define _SPI_

#include "types.h"
#include "gpio.h"
#include "afio.h"

/**
 * @brief Register definitions for SPI peripheral
 */
typedef struct
{
    __IO uint16_t CR1;      //!< SPI control register 1
    uint16_t reserved0;
    __IO uint16_t CR2;      //!< SPI control register 2
    uint16_t reserved1;
    __IO uint16_t SR;       //!< SPI status register
    uint16_t reserved3;
    __IO uint16_t DR;       //!< SPI data register
    uint16_t reserved4;
    __IO uint16_t CRCPR;    //!< SPI CRC polynomial register
    uint16_t reserved5;
    __IO uint16_t RXCRCR;   //!< SPI RX crc register
    uint16_t reserved6;
    __IO uint16_t TXCRCR;   //!< SPI RX crc register
    uint16_t reserved7;
#if defined(STM32F10X_HD) || defined(STM32F10X_XL)
    __IO uint16_t I2SCFGR;  //!< I2S configuration register
    uint16_t reserved8;
    __IO uint16_t I2SPR;    //!< I2S prescaler register
    uint16_t reserved9;
#endif
} SPI_Struct;

extern volatile SPI_Struct SPI1;
extern volatile SPI_Struct SPI2;
extern volatile SPI_Struct SPI3;

/**
 * @brief Bit definitions for SPI_CR1 register
 */
enum SPI_CR1_Flg
{
    SPI_CR1_BIDIMODE          = 1 << 15,    //!< Bidirectional data mode enable
    SPI_CR1_BIDIOE            = 1 << 14,    //!< Output enable in bidirectional mode
    SPI_CR1_CRCEN             = 1 << 13,    //!< Hardware CRC calculation enable
    SPI_CR1_CRCNEXT           = 1 << 12,    //!< CRC transfer next

    SPI_CR1_DFF               = 1 << 11,    //!< Data frame format
    SPI_CR1_DFF_8bit          = 0 << 11,    //!< 8-bit data frame format for TX/RF
    SPI_CR1_DFF_16bit         = 1 << 11,    //!< 16-bit data frame format for TX/RF
    // Note: This bit should only be written when SPE == 0

    SPI_CR1_RXONLY            = 1 << 10,    //!< Receive only
    SPI_CR1_SSM               = 1 << 9,     //!< Software slave management enable/disable
    SPI_CR1_SSI               = 1 << 8,     //!< Internal slave select

    SPI_CR1_LSBFIRST          = 1 << 7,     //!< Frame format.
    SPI_CR1_LSBFIRST_MSBFIRST = 0 << 7,     //!< MSB transmitted first
    SPI_CR1_LSBFIRST_LSBFIRST = 1 << 7,     //!< LSB transmitted first
    // Note: This bit should not be changed when communication is ongoing

    SPI_CR1_SPE               = 1 << 6,     //!< SPI enable

    SPI_CR1_BR                = 0b111 << 3, //!< Bits [5:3] Baud rate control
    SPI_CR1_BR_PCLK_Div2      = 0b000 << 3, //!< f_{PCLK}/2
    SPI_CR1_BR_PCLK_Div4      = 0b001 << 3, //!< f_{PCLK}/4
    SPI_CR1_BR_PCLK_Div8      = 0b010 << 3, //!< f_{PCLK}/8
    SPI_CR1_BR_PCLK_Div16     = 0b011 << 3, //!< f_{PCLK}/16
    SPI_CR1_BR_PCLK_Div32     = 0b100 << 3, //!< f_{PCLK}/32
    SPI_CR1_BR_PCLK_Div64     = 0b101 << 3, //!< f_{PCLK}/64
    SPI_CR1_BR_PCLK_Div128    = 0b110 << 3, //!< f_{PCLK}/128
    SPI_CR1_BR_PCLK_Div256    = 0b111 << 3, //!< f_{PCLK}/256
    // Note: These bits should not be changed when communication is ongoing
    //       Not used in I2S mode

    SPI_CR1_MSTR              = 1 << 2,     //!< Master selection
    SPI_CR1_MSTR_Slave        = 0 << 2,     //!< Slave configuration
    SPI_CR1_MSTR_Master       = 1 << 2,     //!< Master configuration
    // Note: this bit should not be changed when communication is ongoing
    //       Not used in I2S mode

    SPI_CR1_CPOL              = 1 << 1,     //!< Clock polarity. O == CK to 0 when idle, 1 == CK to 1 when idle

    SPI_CR1_CPHA              = 1 << 0,     //!< Clock phase.
    // 0 == The first clock transition is the first data capture edge
    // 1 == The second clock transition is the first data capture edge
};

/**
 * @brief Bit definitions for SPI_CR2 register
 */
enum SPI_CR2_Flg
{
    SPI_CR2_TXEIE      = 1 << 7,   //!< Tx buffer empty interrupt enable
    SPI_CR2_RXNEIE     = 1 << 6,   //!< Rx buffer not empty interrupt enable
    SPI_CR2_ERRIE      = 1 << 5,   //!< Error interrupt enable
    SPI_CR2_SSOE       = 1 << 2,   //!< SS output enable
    SPI_CR2_TXDMAEN    = 1 << 1,   //!< Tx buffer DMA enable
    SPI_CR2_RXDMAEN    = 1 << 0,   //!< Rx buffer DMA enable
};

/**
 * @brief Bit definitions for SPI_SR register
 */
enum SPI_SR_Flg
{
    SPI_SR_BSY        = 1 << 7,    //!< Busy flag
    SPI_SR_OVR        = 1 << 6,    //!< Overrun flag
    SPI_SR_MODF       = 1 << 5,    //!< Mode fault
    SPI_SR_CRCERR     = 1 << 4,    //!< CRC error flag
    SPI_SR_UDR        = 1 << 3,    //!< Underrun flag (Note: Not used in SPI mode)
    SPI_SR_CHSIDE     = 1 << 2,    //!< Channel side  (Note: Not used in SPI mode)
    SPI_SR_TXE        = 1 << 1,    //!< Transmit buffer empty
    SPI_SR_RXNE       = 1 << 0,    //!< Receive buffer not empty
};

enum SPI_CRCRPR_Flg { SPI_CRCPR_CRCPOLY  = 0x7fff /*!< CRC polynomial bitmask */ };
enum SPI_RXCRCR_Flg { SPI_RXCRCR_CRCPOLY = 0x7fff /*!< RX CRC bitmask */ };
enum SPI_TXCRCR_Flg { SPI_TXCRCR_CRCPOLY = 0x7fff /*!< TX CRC bitmask */ };

/**
 * @brief Helper function that stores to flag of the first SPI_CR1_BR_DivX flag that will result in a baud
 * rate below or equal to maxFreq, when running at the current PCLK2 or PCLK1.
 *
 * @note  Uses g_clock to get the base frequency
 *
 * @param [in]   spi         For which SPI peripheral (determines whether we use g_clock.pclk2Freq or g_clock.pclk1freq)
 * @param [in]   maxFreq     Maximum frequency/baud rate in Hz
 * @param [out]  flag        Set to the SPI_CR1_BR_DivX flag, unless NULL
 * @param [out]  actualFreq  Set to the actual baud rate this will give, unless NULL
 *
 * @return true if a divisor was found, false if no baud rate below maxFreq available
 *
 * Example usage:
 *   uint16_t flag;
 *   uint32_t actualFreq;
 *   if (!spi_getBaudRateDivisorFromMaxFreq(&SPI1, 1000000, &flag, &actualFreq))
 *       die("Could not get suitable SPI baud rate");
 *   SPI1.CR1 &= ~SPI_CR1_BR;
 *   SPI1.CR1 |= flag;
 *   // actualFreq now contains the actual baud rate
 */
bool spi_getBaudRateDivisorFromMaxFreq(SPI_Struct const * const spi, uint32_t maxFreq,
                                       uint16_t *flag, uint32_t *actualFreq);

enum SPI_OutputMode
{
    SPI_PushPull,
    SPI_OpenDrain
};

enum SPI_InputMode
{
    SPI_Floating,
    SPI_PullUp,
    SPI_PullDown
};

/**
 * @brief Write me
 */
void SPI1_SetupGpio(enum AF_Mapping mapping, enum SPI_OutputMode outputMode, enum SPI_InputMode inputMode);

/**
 * @brief Write me
 */
void SPI2_SetupGpio(enum SPI_OutputMode outputMode, enum SPI_InputMode inputMode);

#endif
