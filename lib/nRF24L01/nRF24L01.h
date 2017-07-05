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

/**
 * @brief Struct for an initialized nRF24L01
 */
struct nRF24L01
{
    uint8_t status;                         //!< Holds STATUS from last command to nRF24L01
    struct nRF24L01_Options const * conf;   //!< Device options
};

/**
 * @brief Struct to hold options for setting up nRF24L01
 *
 * @note  Possible to use as static const, so that it is completely stored in Flash
 */
struct nRF24L01_Options
{
    struct GPIO_PortPin CSN; //!< Chip Select Not pin. Active LOW
    struct GPIO_PortPin CE;  //!< Chip Enable pin. Active HIGH

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


    //! Pointer to function that sends 8 bits and then reads 8 bits back, with
    //! the least significant bit first. This function should not toggle the
    //! chip select pin, that is handled separately (the chip select pin (CSN)
    //! may indeed be low during two or more calls of this function)
    uint8_t (*spi_sendrecv)(uint8_t);

    //! RX mode only. Callback function called for each recevied word. Called
    //! with a pointer to the device, a pointer to the received word, and the
    //! length of the received word. May be NULL.
    void (*rx_cb)(const struct nRF24L01*, const void *data, size_t len);
};

/**
 * @brief Initialize nRF24L01
 *
 * @param options [in]  Struct with device options and callbacks
 * @param dev     [out] nRF24L01 device object
 *
 * Example usage:
 * @code
 * static const struct nRF24L01_Options rfDev_opts = {
 *     .spi_sendrecv = &SPI1_SendAndReceive,
 *     .airDataRate = nRF24L01_2Mbps,
 *     .power = nRF24L01_TXPower_Minus12dBm,
 *     ...
 * };
 * struct nRF24L01 rfDev;
 * nRF24L01_init(&rfDev_opts, &rfDev);
 * @endcode
 */
bool nRF24L01_init(struct nRF24L01_Options const * const options, struct nRF24L01 *dev);

/**
 * @brief Send data.
 *
 * @param dev     [in/out] nRF24L01 device object
 * @param payload [in]     Pointer to buffer containing payload to send. This buffer needs to be
 *                         payloadWidth bytes large.
 *
 * @note  Must be in TX mode
 */
void nRF24L01_send(struct nRF24L01 *dev, const uint8_t *payload);

/**
 * @brief This should be called by the interrupt routine connected to the
 *        IRQ pin coming from the nRF24L01
 *
 * @param dev [in/out] nRF24L01 device object
 *
 * @note  When in RX mode calls rx_cb
 */
void nRF24L01_interrupt(struct nRF24L01 *dev);

#endif  /* _NRF24L01_ */
