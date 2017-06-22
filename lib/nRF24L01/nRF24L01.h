/**
 * @brief nRF24L01 driver
 */

#ifndef _NRF24L01_
#define _NRF24L01_

#include "../types.h"
#include "../gpio.h"

enum nRF24L01_RF_DR
{
    nRF24L01_RF_DR_1Mbps = 0b00000000,
    nRF24L01_RF_DR_2Mbps = 0b00001000,
};

enum nRF24L01_RF_PWR
{
    nRF24L01_RF_PWR_Minus18dBm = 0b00000000,
    nRF24L01_RF_PWR_Minus12dBm = 0b00000010,
    nRF24L01_RF_PWR_Minus6dBm  = 0b00000100,
    nRF24L01_RF_PWR_Minus0dBm  = 0b00000110,
};

enum nRF24L01_UseAck
{
    nRF24L01_NoACK,
    nRF24L01_ACK,
};

enum nRF24L01_Retransmission
{
    nRF24L01_NoRetransmission,
    nRF24L01_Retransmission,
};

struct nRF24L01
{
    GPIO_PortPin CSN; //!< Chip Select Not pin
    GPIO_PortPin CE;  //!< CE pin
    
    //! Pointer to function that sends 8 bits and then reads 8 bits back, with
    //! the least significant bit first. This function should not toggle the
    //! chip select pin, that is handled separately (the chip select pin (CSN)
    //! may indeed be low during two or more calls of this function)
    uint8_t (*spi_sendrecv)(uint8_t);

    // The following options are all settings
    enum nRF24L01_RF_DR          airDataRate;
    enum nRF24L01_RF_PWR         power;
    enum nRF24L01_UseAck         useAck;
    enum nRF24L01_Retransmission retransmission;
    uint8_t channel; //!< Value from 0 to 127. Take care that channels are
                     //!spaced 2 apart when running in 2mbps mode
};

/**
 * @brief Initialize nRF24L01
 */
bool nRF24L01_init(struct nRF24L01 *device);

/**
 * @brief This should be called by the interrupt routine connected to the
 *        interrupt pin from the nRF24L01
 */
void nRF24L01_interrupt(struct nRF24L01 *device);


#endif  /* _NRF24L01_ */
