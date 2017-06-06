#include "snesCon_read.h"

#include "lib/gpio.h"
#include "lib/rcc.h"
#include "lib/clock.h"
#include "lib/systick.h"
#include "lib/utils.h"

#include "lib/debug.h"

static const uint8_t pollFreq = 60;  //!< How often to poll the controller [Hz]

static snesCon_btn_t buttonState;

static const snesCon_read_t snesCon_def =
{
    .clock = {&GPIOA, 6},
    .latch = {&GPIOA, 7},
    .data  = {&GPIOB, 0}
};

// static volatile const GPIO_Port * const asdf = GPIOA;

void main(void)
{
    clock_setSysClockHSE_24MHz();

    // Enable clock to GPIOA & GPIOB & GPIOC
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA;
    RCC.APB2ENR |= RCC_APB2Periph_GPIOB;
    RCC.APB2ENR |= RCC_APB2Periph_GPIOC;

    GPIO_setMODE_setCNF(&GPIOC, 13, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    if (!systick_startSysTick_us(DIV_ROUND_CLOSEST(100000U, pollFreq)))
        die("Could not set up systick");

    snesCon_read_init(&snesCon_def);

    while (true)
    {
        GPIO_resetPin(&GPIOC, 13);
        delay_us_int(3000000);
        GPIO_setPin(&GPIOC, 13);
        delay_us_int(3000000);
    }
}

void systick_handler(void)
{
    buttonState = snesCon_read_tick(&snesCon_def);
    static volatile uint8_t cntr = 0;
    ++cntr;
    if (cntr == 30)
    {
        print_hex(buttonState);
        cntr = 0;
    }
}
