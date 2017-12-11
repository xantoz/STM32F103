#ifndef _COMMON_RF_
#define _COMMON_RF_

#include <nRF24L01/nRF24L01.h>
#include <protocol/common.h>

enum rf_TxRx
{
    rf_Tx,
    rf_Rx
};

/**
 * Init the RF device
 *
 * @param txrx         [in] Whether to init in TX or RX mode
 * @param recv_message [in] Function pointer to RX callback (Not used in TX mode: Just pass NULL)
 */
void rf_init(enum rf_TxRx txrx,
             void (*recv_message)(const struct nRF24L01*, uint8_t pipeNo,
                                  const void *data, size_t len));

/**
 * Send data over RF
 */
void rf_send(const void *data, size_t len);

/**
 * Set TX address from one of 16 presets (defined in the rf module).
 *
 * @param addr [in] A number between 0 and 15
 */
void rf_setTxAddress(uint8_t addr);

/**
 * Set RX address for a particular pipe from one of 16 presets (defined in the rf module).
 *
 * @param addr [in] A number between 0 and 15
 * @param pipe [in] Which pipe number to affect. From 0 to 5
 */
void rf_setRxAddress(uint8_t addr, uint8_t pipe);

#endif
