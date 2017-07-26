#include "gpio.h"
#include "rcc.h"
#include "clock.h"
#include "systick.h"
#include "utils.h"
#include "debug.h"

#include "spi.h"
#include "nRF24L01/nRF24L01.h"
#include "protocol/snesCon_read.h"

#include "config.h"

struct nRF24L01 rfDev;

static uint8_t spi_sendrecv(const uint8_t data)
{
    SPI_send(&nRF24L01_SPI, data);
    return SPI_recv(&nRF24L01_SPI);
}

void main()
{
    clock_setSysClockHSE_24MHz();

    __disable_irq();

    // Enable clock to GPIOA & GPIOB & GPIOC
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;
    RCC.APB2ENR |= RCC_APB2Periph_SPI1;    // Enable clock to SPI1

    snesCon_read_init(&snesCon_def);
    SPI_initAsMaster(&nRF24L01_SPI, &spi_opts);
    nRF24L01_init(&rfDev_opts, &rfDev);

    if (!systick_startSysTick_us(DIV_ROUND_CLOSEST(1000000u, POLLFREQ)))
        die("Could not set up systick");

    __enable_irq();
}

void Systick_Handler()
{
    const snesCon_btn_t buttonState = snesCon_read_tick(&snesCon_def);
    nRF24L01_send(&rfDev, &buttonState);
}
