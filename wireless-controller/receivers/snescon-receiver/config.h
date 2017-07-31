#ifndef _SNESCON_RECEIVER_CONFIG_
#define _SNESCON_RECEIVER_CONFIG_

#define LED ((struct GPIO_PortPin){&GPIOC, 13})

/* **** Settings for the SNES client **** */

#define snesCon_IRQHandler EXTI9_5_IRQHandler
#define snesCon_IRQn       EXTI9_5_IRQn
#define snesCon_PINS \
    ((struct snesCon_pins){ .clock = {&GPIOB, 7}, .latch = {&GPIOB, 8}, .data  = {&GPIOB, 9} })

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
#define nRF24L01_IRQn  EXTI2_IRQn
static uint8_t spi_sendrecv(const uint8_t data);
static void recv_message(const struct nRF24L01 *dev, uint8_t pipeNo, const void *data, size_t len);
static const struct nRF24L01_Options rfDev_opts = {
    .CSN = {&GPIOA, 4},
    .CE  = {&GPIOA, 3},

    .airDataRate      = nRF24L01_2Mbps,
    .useACK           = nRF24L01_NoACK,
    .useCRC           = nRF24L01_CRC,
    .retransmit.count = 0,
    .mode             = nRF24L01_RX,

    .payloadWidth     = sizeof(snesCon_btn_t),
    .channel          = 33,

    .spi_sendrecv     = &spi_sendrecv,
    .rx_cb            = &recv_message,
};

#endif
