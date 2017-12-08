#ifndef _COMMON_RF_
#define _COMMON_RF_

#include <nRF24L01/nRF24L01.h>
#include <protocol/common.h>

extern struct nRF24L01 g_rfDev;

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
 * Send button state over RF
 */
void rf_send(const btn_t *btn);

#endif
