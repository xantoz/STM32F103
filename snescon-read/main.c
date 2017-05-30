#include "snesCon_read.h"

#include "lib/gpio.h"
#include "lib/rcc.h"
#include "lib/clock.h"
#include "lib/systick.h"

#include "lib/debug.h"

static const uint8_t pollFreq = 60;  //!< How often to poll the controller [Hz]

static snesCon_btn_t buttonState;

static const snesCon_read_t snesCon_def = 
{
    // TODO: switch to register locations defined in link script instead so that we can just go: &GPIOA here
    .clock = {GPIOA_BASE, 0}, 
    .latch = {GPIOA_BASE, 1},
    .data  = {GPIOA_BASE, 2}
};

// static volatile const GPIO_Port * const asdf = GPIOA;

void main(void)
{
    clock_setSysClockHSE();

    // Enable clock to GPIOC
    RCC->APB2ENR |= RCC_APB2Periph_GPIOA;

    if (!systick_startSysTick_ms(1000))
        die("Could not set up systick");

    snesCon_read_init(&snesCon_def);
}

void systick_handler(void)
{
    snesCon_read_tick(&snesCon_def, &buttonState);
}
