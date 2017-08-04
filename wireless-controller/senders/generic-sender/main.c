#include "gpio.h"
#include "rcc.h"
#include "clock.h"
#include "systick.h"
#include "utils.h"
#include "debug.h"

#include "spi.h"
#include "nRF24L01/nRF24L01.h"

#include "config.h"

struct nRF24L01 rfDev;

void main()
{
    clock_setSysClockHSE_24MHz();

    __disable_irq();

    // Enable clock to GPIOA & GPIOB & GPIOC
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;
    nRF24L01_SPI_enableClock();

    for (uint8_t i = 0; i < 15; ++i)
    {
        struct GPIO_PortPin pin = { &GPIOA, i };
        GPIO_setMODE_setCNF(&pin, GPIO_MODE_Input, GPIO_Input_CNF_PullupPulldown);
        GPIO_resetPin(&pin); // Set pull-down
    }

    SPI_initAsMaster(&nRF24L01_SPI, &spi_opts);
    nRF24L01_init(&rfDev_opts, &rfDev);

    if (!systick_startSysTick_us(DIV_ROUND_CLOSEST(1000000u, POLLFREQ)))
        die("Could not set up systick");

    __enable_irq();
}

void Systick_Handler()
{
    const uint16_t buttonState = GPIOA.IDR;
    nRF24L01_send(&rfDev, &buttonState);
}
