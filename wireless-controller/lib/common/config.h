/**
 * @brief Defines and structs
 */

#ifndef _COMMON_CONFIG_
#define _COMMON_CONFIG_

/* **** Misc ports **** */

#define LED ((struct GPIO_PortPin){&GPIOC, 13})


/* **** Settings for the nRF24L01 **** */

#define nRF24L01_SPI         SPI1
#define nRF24L01_SPI_MAXFREQ 8000000
static uint8_t spi_sendrecv(const uint8_t data)
{
    SPI_send(&nRF24L01_SPI, data);
    return SPI_recv(&nRF24L01_SPI);
}
static const struct SPI_Options spi_opts = {
    .maxFreq = nRF24L01_SPI_MAXFREQ,
    .mapping = AFIO_DEFAULT,
    .outputMode = SPI_PushPull,
    .inputMode  = SPI_PullDown,
    .nss = SPI_SoftwareNSS,
    .cpol = 0,
    .cpha = 0,
};

#define nRF24L01_IRQ_PortPin ((struct GPIO_PortPin){&GPIOA, 2})
#define nRF24L01_IRQHandler  EXTI2_IRQHandler
#define nRF24L01_IRQn        EXTI2_IRQn
#define nRF24L01_CSN_PortPin {&GPIOA, 4}
#define nRF24L01_CE_PortPin  {&GPIOA, 3}
static const struct nRF24L01_Options rfDev_opts_tx = {
    .CSN = nRF24L01_CSN_PortPin,
    .CE  = nRF24L01_CE_PortPin,

    .airDataRate      = nRF24L01_2Mbps,
    .power            = nRF24L01_TXPower_Minus0dBm,
    .useACK           = nRF24L01_NoACK,
    .useCRC           = nRF24L01_CRC,
    .retransmit.count = 0,
    .mode             = nRF24L01_TX,

    .payloadWidth     = sizeof(btn_t),
    .channel          = 33,

    .spi_sendrecv     = &spi_sendrecv,
};

// This function needs to be defined somewhere to receive payloads from nRF24L01
void recv_message(const struct nRF24L01 *dev, uint8_t pipeNo, const void *data, size_t len);
static const struct nRF24L01_Options rfDev_opts_rx = {
    .CSN = nRF24L01_CSN_PortPin,
    .CE  = nRF24L01_CE_PortPin,

    .airDataRate      = nRF24L01_2Mbps,
    .useACK           = nRF24L01_NoACK,
    .useCRC           = nRF24L01_CRC,
    .retransmit.count = 0,
    .mode             = nRF24L01_RX,

    .payloadWidth     = sizeof(btn_t),
    .channel          = 33,

    .spi_sendrecv     = &spi_sendrecv,
    .rx_cb            = &recv_message,
};


#endif
