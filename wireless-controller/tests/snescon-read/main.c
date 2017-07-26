#include "protocol/snesCon_read.h"

#include "gpio.h"
#include "rcc.h"
#include "clock.h"
#include "systick.h"
#include "utils.h"
#include "debug.h"

static const uint8_t pollFreq = 60;  //!< How often to poll the controller [Hz]

static snesCon_btn_t buttonState;

static const struct snesCon_pins snesCon_def =
{
    .clock = {&GPIOB, 7},
    .latch = {&GPIOB, 8},
    .data  = {&GPIOB, 9}
};

static const struct GPIO_PortPin debugLeds[] = {
    {&GPIOB, 12},
    {&GPIOB, 13},
    {&GPIOB, 14},
    {&GPIOB, 15},

    {&GPIOA, 9},
    {&GPIOA, 10},
    {&GPIOA, 11},
    {&GPIOA, 12},

    {&GPIOB, 11},
    {&GPIOB, 10},
    {&GPIOB, 1},
    {&GPIOB, 0},
};

static void init_debugLeds()
{
    for (unsigned i = 0; i < ARRAYLEN(debugLeds); ++i)
        GPIO_setMODE_setCNF(&debugLeds[i], GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
}

static void update_debugLeds(snesCon_btn_t btn)
{
    for (unsigned i = 0; i < ARRAYLEN(debugLeds); ++i)
    {
        GPIO_setBit(&debugLeds[i], (btn & 0x1) == 0x1);
        btn >>= 1;
    }
}

// static volatile const GPIO_Port * const asdf = GPIOA;

void main(void)
{
    clock_setSysClockHSE_24MHz();

    __disable_irq();

    // Enable clock to GPIOA & GPIOB & GPIOC
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA;
    RCC.APB2ENR |= RCC_APB2Periph_GPIOB;
    RCC.APB2ENR |= RCC_APB2Periph_GPIOC;

    GPIO_setMODE_setCNF(&GPIOC, 13, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    init_debugLeds();

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
    update_debugLeds(buttonState);

    static volatile uint8_t cntr = 0;
    ++cntr;
    if (cntr == 30)
    {
        println_u32_hex(buttonState);
        cntr = 0;
    }
}
