#include "types.h"
#include "utils.h"
#include "rcc.h"
#include "clock.h"
#include "debug.h"

#include "nRF24L01/nRF24L01.h"

#include "../common/spi-setup.h"
#include "../common/ports.h"

#define DELAY 200000

// Settings for the nRF24L01
static const struct nRF24L01_Options rfDev_opts = {
    .CSN = {&GPIOA, 3},
    .CE  = {&GPIOA, 1},

    .airDataRate    = nRF24L01_2Mbps,
    .power          = nRF24L01_TXPower_Minus0dBm,
    .useACK         = nRF24L01_NoACK,
    .useCRC         = nRF24L01_NoCRC,
    .retransmission = nRF24L01_NoRetransmission,
    .mode           = nRF24L01_TX,

    .payloadWidth   = 2,
    .channel        = 33,

    .spi_sendrecv   = &spi_sendrecv,
    .rx_cb          = NULL
};

struct nRF24L01 rfDev;

void main()
{
    clock_setSysClockHSE();
    // clock_setSysClockHSE_24MHz();

    delay_us(1000000);

    // Enable clock to all GPIO:s
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;

    RCC.APB2ENR |= RCC_APB2Periph_SPI1;    // Enable clock to SPI1

    GPIO_setMODE_setCNF(&LED, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_setMODE_setCNF(&DEBUG_INIT_PIN, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_resetPin(&DEBUG_INIT_PIN);

    GPIO_setPin(&DEBUG_INIT_PIN);
    spi_setup();
    GPIO_resetPin(&DEBUG_INIT_PIN);

    GPIO_setPin(&DEBUG_INIT_PIN);
    nRF24L01_init(&rfDev_opts, &rfDev);
    GPIO_resetPin(&DEBUG_INIT_PIN);

    uint16_t msg = 110;
    while (true)
    {
        GPIO_resetPin(&LED);
        delay_us(DELAY);

        nRF24L01_send(&rfDev, &msg);
        println_u32_dec(msg);
        println_u32_hex(rfDev.status);
        // spi_send(msg % 256);

        GPIO_setPin(&LED);
        delay_us(DELAY);
        ++msg;
    }
}
