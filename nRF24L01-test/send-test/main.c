#include "lib/types.h"
#include "lib/utils.h"
#include "lib/rcc.h"
#include "lib/clock.h"
#include "lib/debug.h"

#include "lib/nRF24L01/nRF24L01.h"

#include "common/spi-setup.h"

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

    GPIO_setMODE_setCNF(&GPIOC, 13, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_setMODE_setCNF(&GPIOC, 15, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_resetPin(&GPIOC, 15);

    GPIO_setPin(&GPIOC, 15);
    spi_setup();
    GPIO_resetPin(&GPIOC, 15);

    GPIO_setPin(&GPIOC, 15);
    nRF24L01_init(&rfDev_opts, &rfDev);
    GPIO_resetPin(&GPIOC, 15);

    uint16_t msg = 110;
    while (true)
    {
        GPIO_resetPin(&GPIOC, 13);
        delay_us(DELAY);

        nRF24L01_send(&rfDev, &msg);
        println_u32_dec(msg);
        println_u32_hex(rfDev.status);
        // spi_send(msg % 256);

        GPIO_setPin(&GPIOC, 13);
        delay_us(DELAY);
        ++msg;
    }
}
