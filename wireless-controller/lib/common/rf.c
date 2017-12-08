#include "rf.h"
#include "config.h"

#include <exti.h>
#include <IRQn.h>
#include <spi.h>
#include <nvic.h>
#include <debug.h>

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

static struct nRF24L01 rfDev = {
    .CSN = nRF24L01_CSN_PortPin,
    .CE  = nRF24L01_CE_PortPin,
    .IRQ = nRF24L01_IRQ_PortPin,

    .airDataRate      = nRF24L01_2Mbps,
    .power            = nRF24L01_TXPower_Minus0dBm,
    .useCRC           = nRF24L01_CRC,
    .retransmit.count = 0,

    .pipe[0] = { .enable = true, .payloadWidth = sizeof(btn_t) },
    .channel = 33,

    .spi_sendrecv = &spi_sendrecv,
};

void rf_init(enum rf_TxRx txrx,
             void (*recv_message)(const struct nRF24L01*, uint8_t pipeNo,
                                  const void *data, size_t len))
{
    SPI_initAsMaster(&nRF24L01_SPI, &spi_opts);
    NVIC_setInterruptPriority(nRF24L01_IRQn, 4);
    rfDev.rx_cb = recv_message;
    rfDev.mode = (txrx == rf_Tx) ? nRF24L01_TX : nRF24L01_RX;
    nRF24L01_init(&rfDev);
}

void rf_send(const btn_t *btn)
{
    nRF24L01_send(&rfDev, btn, sizeof(*btn));
}

void nRF24L01_IRQHandler(void)
{
    GPIO_resetPin(&LED);
    nRF24L01_interrupt(&rfDev);
    EXTI.PR = 0x1 << nRF24L01_IRQ_PortPin.pin;
    GPIO_setPin(&LED);
}
