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
    nRF24L01_1Mbps = 0,
    nRF24L01_2Mbps,
};

enum nRF24L01_TXPower
{
    nRF24L01_TXPower_Minus18dBm = 0,
    nRF24L01_TXPower_Minus12dBm,
    nRF24L01_TXPower_Minus6dBm,
    nRF24L01_TXPower_Minus0dBm,
};

enum nRF24L01_UseACK
{
    nRF24L01_ACK = 0,
    nRF24L01_NoACK,
};

enum nRF24L01_UseCRC
{
    nRF24L01_CRC = 0,
    nRF24L01_NoCRC,
};

enum nRF24L01_Retransmit_Delay
{
    nRF24L01_Retransmit_Delay_250us = 0,
    nRF24L01_Retransmit_Delay_500us,
    nRF24L01_Retransmit_Delay_750us,
    nRF24L01_Retransmit_Delay_1000us,
    nRF24L01_Retransmit_Delay_1250us,
    nRF24L01_Retransmit_Delay_1500us,
    nRF24L01_Retransmit_Delay_1750us,
    nRF24L01_Retransmit_Delay_2000us,
    nRF24L01_Retransmit_Delay_2250us,
    nRF24L01_Retransmit_Delay_2500us,
    nRF24L01_Retransmit_Delay_2750us,
    nRF24L01_Retransmit_Delay_3000us,
    nRF24L01_Retransmit_Delay_3250us,
    nRF24L01_Retransmit_Delay_3500us,
    nRF24L01_Retransmit_Delay_3750us,
    nRF24L01_Retransmit_Delay_4000us,
};

enum nRF24L01_Mode
{
    nRF24L01_RX = 0,
    nRF24L01_TX,
};

enum nRF24L01_AddressWidth
{
    nRF24L01_AddressWidth_5bytes = 0,  // 5 bytes by default
    nRF24L01_AddressWidth_3bytes,
    nRF24L01_AddressWidth_4bytes,
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
    enum nRF24L01_AddressWidth   addressWidth;
    struct
    {
        uint8_t count; //!< 0 to 15. How many times to attempt retransmission. Use 0 to disable.
        enum nRF24L01_Retransmit_Delay delay; //!< Retransmit delay. Only meaningful if retransmit.count > 0
    } retransmit;
    enum nRF24L01_Mode           mode;                    //!< Mode of operation

    // TODO: change to 1 < payloadWidth <= 32
    uint8_t payloadWidth; //!< How many bytes in one payload. This driver only
                          //! supports a static payload size. 0 < payloadWidth <= 32

    uint8_t channel; //!< Value from 0 to 127. Take care that channels are
                     //! spaced 2 apart when running in 2mbps mode


    //! Pointer to function that sends 8 bits and then reads 8 bits back, with
    //! the least significant bit first. This function should not toggle the
    //! chip select pin, that is handled separately (the chip select pin (CSN)
    //! may indeed be low during two or more calls of this function).
    //!
    //! The reason for providing this function as a callback is to be more
    //! portable and flexible, so that we may use different SPI hardware, or
    //! maybe just bitbanged SPI, without changing the code
    uint8_t (*spi_sendrecv)(uint8_t);

    //! RX mode only. Callback function called for each received word. Called with the pipe number,
    //! a pointer to the device, a pointer to the received word, and the length of the received
    //! word. May be NULL.
    void (*rx_cb)(const struct nRF24L01*, uint8_t pipeNo, const void *data, size_t len);
    //! TX mode only. Callback function called on TX_DS. May be NULL.
    void (*tx_ds_cb)(const struct nRF24L01*);
    //! TX mode with retransmissions only. Called on MAX_RT. May be NULL.
    void (*max_rt_cb)(const struct nRF24L01*);
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

// TODO: deinit function (follow recommended shutdown procedure)?

/**
 * @brief Send data.
 *
 * @param dev     [in/out] nRF24L01 device object
 * @param payload [in]     Pointer to buffer containing payload to send. This buffer needs to be
 *                         payloadWidth bytes large.
 *
 * @note  Must be in TX mode
 */
void nRF24L01_send(struct nRF24L01 *dev, const void *payload);

/**
 * @brief Reads all words in the input FIFO and dispatches them to rx_cb.
 *
 * @param dev [in/out] nRF24L01 device object
 *
 * @note  RX mode only. calls rx_cb
 * @note  If using interrupts you do not need to use this directly. nRF24L01_interrupt will call
 *        this function when it receives a TX_DR interrupt.
 */
void nRF24L01_rxDispatchFIFO(struct nRF24L01 *dev);

/**
 * @brief This should be called by the interrupt routine connected to the
 *        IRQ pin coming from the nRF24L01
 *
 * @param dev [in/out] nRF24L01 device object
 *
 * @note  When in RX mode calls rx_cb
 */
void nRF24L01_interrupt(struct nRF24L01 *dev);

/**
 * @brief Set TX address
 *
 * @note addr needs to be as long as specified in address width when
 *       the nRF24L01 object was created.
 */
void nRF24L01_setTxAddress(struct nRF24L01 *dev, uint8_t *addr);
/**
 * @brief Set RX P0 address
 *
 * @note addr needs to be as long as specified in address width when
 *       the nRF24L01 object was created.
 */
void nRF24L01_setRxP0Address(struct nRF24L01 *dev, uint8_t *addr);
/**
 * @brief Set RX P1 address
 *
 * @note addr needs to be as long as specified in address width when
 *       the nRF24L01 object was created.
 */
void nRF24L01_setRxP1Address(struct nRF24L01 *dev, uint8_t *addr);
void nRF24L01_setRxP2Address(struct nRF24L01 *dev, uint8_t addr);
void nRF24L01_setRxP3Address(struct nRF24L01 *dev, uint8_t addr);
void nRF24L01_setRxP4Address(struct nRF24L01 *dev, uint8_t addr);
void nRF24L01_setRxP5Address(struct nRF24L01 *dev, uint8_t addr);

#endif  /* _NRF24L01_ */
