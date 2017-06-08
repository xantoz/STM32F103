#ifndef _SPI_
#define _SPI_

#include "types.h"

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

enum SPI_CR1_Flg
{

};

enum SPI_CR2_Flg
{
};

enum SPI_SR_Flg
{
};

enum SPI_CRCRPR_Flg { SPI_CRCPR_CRCPOLY  = 0x00007fff; /*!< CRC polynomial bitmask */ };
enum SPI_RXCRCR_Flg { SPI_RXCRCR_CRCPOLY = 0x00007fff; /*!< RX CRC bitmask */ };
enum SPI_TXCRCR_Flg { SPI_TXCRCR_CRCPOLY = 0x00007fff; /*!< TX CRC bitmask */ };



#endif
