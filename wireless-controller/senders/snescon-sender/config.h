#ifndef _SNESCON_SENDER_CONFIG_
#define _SNESCON_SENDER_CONFIG_

/* **** Settings for the SNES controller **** */
#define POLLFREQ 1200

static const struct snesCon_pins snesCon_def = {
    .clock = {&GPIOB, 7},
    .latch = {&GPIOB, 8},
    .data  = {&GPIOB, 9},
};

/* **** Settings for the nRF24L01 **** */

#define nRF24L01_SPI         SPI1
#define nRF24L01_SPI_MAXFREQ 8000000

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
static uint8_t spi_sendrecv(const uint8_t data);
static const struct nRF24L01_Options rfDev_opts = {
    .CSN = {&GPIOA, 4},
    .CE  = {&GPIOA, 3},

    .airDataRate      = nRF24L01_2Mbps,
    .power            = nRF24L01_TXPower_Minus0dBm,
    .useACK           = nRF24L01_NoACK,
    .useCRC           = nRF24L01_CRC,
    .retransmit.count = 0,
    .mode             = nRF24L01_TX,

    .payloadWidth     = sizeof(snesCon_btn_t),
    .channel          = 33,

    .spi_sendrecv     = &spi_sendrecv,
};

#endif
