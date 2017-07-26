#include "gpio.h"
#include "rcc.h"
#include "clock.h"
#include "systick.h"
#include "utils.h"
#include "debug.h"

#include "protocol/snesCon_read.h"
#include "common/debugLeds.h"

static const uint8_t pollFreq = 120;  //!< How often to poll the controller [Hz]

static snesCon_btn_t buttonState;

static const struct snesCon_pins snesCon_def =
{
    .clock = {&GPIOB, 7},
    .latch = {&GPIOB, 8},
    .data  = {&GPIOB, 9}
};

void main(void)
{
    clock_setSysClockHSE_24MHz();

    __disable_irq();

    // Enable clock to GPIOA & GPIOB & GPIOC
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA;
    RCC.APB2ENR |= RCC_APB2Periph_GPIOB;
    RCC.APB2ENR |= RCC_APB2Periph_GPIOC;

    GPIO_setMODE_setCNF(&GPIOC, 13, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    debugLeds_init();

    snesCon_read_init(&snesCon_def);

    if (!systick_startSysTick_us(DIV_ROUND_CLOSEST(1000000u, pollFreq)))
        die("Could not set up systick");

    __enable_irq();

    while (true)
    {
        GPIO_resetPin(&GPIOC, 13);
        delay_us(3000000);
        GPIO_setPin(&GPIOC, 13);
        delay_us(3000000);
    }
}

void Systick_Handler(void)
{
    buttonState = snesCon_read_tick(&snesCon_def);
    debugLeds_update(buttonState);

    static volatile uint8_t cntr = 0;
    ++cntr;
    if (cntr == 30)
    {
        println_u32_hex(buttonState);
        cntr = 0;
    }
}
