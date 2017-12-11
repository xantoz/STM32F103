#include <gpio.h>
#include <rcc.h>
#include <clock.h>
#include <systick.h>
#include <utils.h>
#include <debug.h>

#include <nRF24L01/nRF24L01.h>
#include <protocol/snesCon_read.h>

#include <common/rf.h>

#include "config.h"

void main()
{
    clock_setSysClockHSE_24MHz();

    __disable_irq();

    // Enable clock to GPIOA & GPIOB & GPIOC
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;
    RCC.APB2ENR |= RCC_APB2Periph_SPI1;    // Enable clock to SPI1

    snesCon_read_init(&snesCon_def);
    rf_init(rf_Tx, NULL);

    if (!systick_startSysTick_us(DIV_ROUND_CLOSEST(1000000u, POLLFREQ)))
        die("Could not set up systick");

    __enable_irq();
}

void Systick_Handler()
{
    const snesCon_btn_t buttonState = snesCon_read_tick(&snesCon_def);
    rf_send(&buttonState, sizeof(buttonState));
}
