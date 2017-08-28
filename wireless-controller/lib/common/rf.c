#include "rf.h"
#include "config.h"

#include <protocol/common.h>

#include <exti.h>
#include <IRQn.h>
#include <spi.h>
#include <nvic.h>

#include <nRF24L01/nRF24L01.h>

static const struct SPI_Options spi_opts = {
    .maxFreq = nRF24L01_SPI_MAXFREQ,
    .mapping = AFIO_DEFAULT,
    .outputMode = SPI_PushPull,
    .inputMode  = SPI_PullDown,
    .nss = SPI_SoftwareNSS,
    .cpol = 0,
    .cpha = 0,
};

static uint8_t spi_sendrecv(const uint8_t data)
{
    SPI_send(&nRF24L01_SPI, data);
    return SPI_recv(&nRF24L01_SPI);
}

// This function needs to be defined in another module somewhere to receive payloads from nRF24L01
void recv_message(const struct nRF24L01 *dev, uint8_t pipeNo, const void *data, size_t len);

static const struct nRF24L01_Options rfDev_opts_tx = {
        .CSN = nRF24L01_CSN_PortPin,
        .CE  = nRF24L01_CE_PortPin,
        .IRQ = nRF24L01_IRQ_PortPin,

        .airDataRate      = nRF24L01_2Mbps,
        .power            = nRF24L01_TXPower_Minus0dBm,
        .useCRC           = nRF24L01_CRC,
        .retransmit.count = 0,

        .pipe[0] = { .enable = true, .payloadWidth = sizeof(btn_t) },
        .channel = 33,

        .mode             = nRF24L01_TX,
        .spi_sendrecv     = &spi_sendrecv
};

static const struct nRF24L01_Options rfDev_opts_rx = {
        .CSN = nRF24L01_CSN_PortPin,
        .CE  = nRF24L01_CE_PortPin,
        .IRQ = nRF24L01_IRQ_PortPin,

        .airDataRate      = nRF24L01_2Mbps,
        .power            = nRF24L01_TXPower_Minus0dBm,
        .useCRC           = nRF24L01_CRC,
        .retransmit.count = 0,

        .pipe[0] = { .enable = true, .payloadWidth = sizeof(btn_t) },
        .channel = 33,

        .mode             = nRF24L01_RX,
        .spi_sendrecv     = &spi_sendrecv
        .rx_cv            = &recv_message,
};

void rf_init(enum rf_TxRx txrx)
{
    SPI_initAsMaster(&nRF24L01_SPI, &spi_opts);
    NVIC_setInterruptPriority(nRF24L01_IRQn, 4);
    nRF24L01_init((txrx == rf_Tx) ? &rfDev_opts_tx : &rfDev_opts_rx, &g_rfDev);
}

void nRF24L01_IRQHandler(void)
{
    GPIO_resetPin(&LED);
    nRF24L01_interrupt(&g_rfDev);
    EXTI.PR = 0x1 << nRF24L01_IRQ_PortPin.pin;
    GPIO_setPin(&LED);
}
