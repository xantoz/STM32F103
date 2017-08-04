#ifndef _GENERIC_SENDER_CONFIG_
#define _GENERIC_SENDER_CONFIG_

#define POLLFREQ 1200

/* **** Settings for the nRF24L01 **** */
#define nRF24L01_SPI         SPI2
#define nRF24L01_SPI_MAXFREQ 8000000
static void nRF24L01_SPI_enableClock() { RCC.APB1ENR |= RCC_APB1Periph_SPI2; }
static uint8_t spi_sendrecv(const uint8_t data)
{
    SPI_send(&nRF24L01_SPI, data);
    return SPI_recv(&nRF24L01_SPI);
}
static const struct SPI_Options spi_opts = {
    .maxFreq = nRF24L01_SPI_MAXFREQ,
    .outputMode = SPI_PushPull,
    .inputMode  = SPI_PullDown,
    .nss = SPI_SoftwareNSS,
    .cpol = 0,
    .cpha = 0,
};

#define nRF24L01_IRQ_PortPin ((struct GPIO_PortPin){&GPIOB, 1})
#define nRF24L01_IRQHandler  EXTI1_IRQHandler

static const struct nRF24L01_Options rfDev_opts = {
    .CSN = {&GPIOB, 12},
    .CE  = {&GPIOB, 11},

    .airDataRate      = nRF24L01_2Mbps,
    .power            = nRF24L01_TXPower_Minus0dBm,
    .useACK           = nRF24L01_NoACK,
    .useCRC           = nRF24L01_CRC,
    .retransmit.count = 0,
    .mode             = nRF24L01_TX,

    .payloadWidth     = sizeof(uint16_t),
    .channel          = 33,

    .spi_sendrecv     = &spi_sendrecv,
};

#endif
