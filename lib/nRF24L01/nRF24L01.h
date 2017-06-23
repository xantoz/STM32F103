/**
 * @brief nRF24L01 driver
 *
 * @file  nRF24L01.h
 */

#ifndef _NRF24L01_
#define _NRF24L01_

#include "../types.h"
#include "../gpio.h"

enum nRF24L01_AirDataRate
{
    nRF24L01_1Mbps,
    nRF24L01_2Mbps,
};

enum nRF24L01_TXPower
{
    nRF24L01_TXPower_Minus18dBm,
    nRF24L01_TXPower_Minus12dBm,
    nRF24L01_TXPower_Minus6dBm,
    nRF24L01_TXPower_Minus0dBm,
};

enum nRF24L01_UseACK
{
    nRF24L01_NoACK,
    nRF24L01_ACK,
};

enum nRF24L01_UseCRC
{
    nRF24L01_NoCRC,
    nRF24L01_CRC,
};

enum nRF24L01_Retransmission
{
    nRF24L01_NoRetransmission,
    nRF24L01_Retransmission,
};

enum nRF24L01_Mode
{
    nRF24L01_RX,
    nRF24L01_TX,
};

struct nRF24L01
{
    struct GPIO_PortPin CSN; //!< Chip Select Not pin. Active LOW
    struct GPIO_PortPin CE;  //!< Chip Enable pin. Active HIGH

    //! Pointer to function that sends 8 bits and then reads 8 bits back, with
    //! the least significant bit first. This function should not toggle the
    //! chip select pin, that is handled separately (the chip select pin (CSN)
    //! may indeed be low during two or more calls of this function)
    uint8_t (*spi_sendrecv)(uint8_t);

    //! RX mode only. Callback function called for each recevied word. Called
    //! with a pointer to the device, a pointer to the received word, and the
    //! length of the received word.
    void (*rx_cb)(const struct nRF24L01*, const void *data, size_t len);

    uint8_t status;  //!< Holds STATUS from last command to nRF24L01

    // The following options are all settings
    enum nRF24L01_AirDataRate    airDataRate;
    enum nRF24L01_TXPower        power;
    enum nRF24L01_UseACK         useACK;
    enum nRF24L01_UseCRC         useCRC;
    enum nRF24L01_Retransmission retransmission;
    enum nRF24L01_Mode           mode;                    //!< Mode of operation

    uint8_t payloadWidth; //!< How many bytes in one payload. This driver only
                          //! supports a static payload size. 0 < payloadWidth <= 32

    uint8_t channel; //!< Value from 0 to 127. Take care that channels are
                     //! spaced 2 apart when running in 2mbps mode
};

/**
 * @brief Initialize nRF24L01
 *
 * @param dev [in/out] nRF24L01 device object
 */
bool nRF24L01_init(struct nRF24L01 *dev);

/**
 * @brief This should be called by the interrupt routine connected to the
 *        interrupt pin from the nRF24L01
 *
 * @param dev [in/out] nRF24L01 device object
 *
 * @note  When in RX mode calls dev->rx_cb
 */
void nRF24L01_interrupt(struct nRF24L01 *dev);

#endif  /* _NRF24L01_ */
