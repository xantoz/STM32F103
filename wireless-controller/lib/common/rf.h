#ifndef _COMMON_RF_
#define _COMMON_RF_

struct nRF24L01 g_rfDev;

enum rf_TxRx
{
    rf_Tx,
    rf_Rx
};

void rf_init(enum rf_TxRx txrx);

#endif
