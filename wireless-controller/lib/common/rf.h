#ifndef _COMMON_RF_
#define _COMMON_RF_

#include <nRF24L01/nRF24L01.h>

extern struct nRF24L01 g_rfDev;

enum rf_TxRx
{
    rf_Tx,
    rf_Rx
};

/**
 * Override this function in another module to receive messages from the RF module (if
 * configured in RX mode)
 */
void recv_message(const struct nRF24L01 *dev,
                  uint8_t pipeNo,
                  const void *data, size_t len) DEFAULTS_TO(__recv_message_unimpl);

void rf_init(enum rf_TxRx txrx);

#endif
