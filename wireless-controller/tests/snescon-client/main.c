#include "protocol/snesCon_client.h"

#include "gpio.h"
#include "debug.h"
#include "clock.h"
#include "rcc.h"
#include "systick.h"

#define CLOCK {&GPIOB, 0}
#define LATCH {&GPIOB, 1}
#define DATA  {&GPIOB, 10}
#define CLOCK_IRQHandler EXTI0_IRQHandler
#define LATCH_IRQHandler EXTI1_IRQHandler

struct snesCon_client controller = {
    .pinDef = {
        .clock = CLOCK,
        .latch = LATCH,
        .data =  DATA,
    }
};

void main()
{
    clock_setSysClockHSE_24MHz();

    __disable_irq();

    // Enable clock to all GPIO:s
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;

    snesCon_client_init(&controller);

    if (!systick_startSysTick_ms(500))
        die("Could not set up systick");

    __enable_irq();

    return;
}

void Systick_Handler()
{
    static uint32_t cntr = 0;
    static snesCon_btn_t konamiCode[] = {
        snesCon_BUTTON_Up,
        snesCon_BUTTON_NONE,
        snesCon_BUTTON_Up,
        snesCon_BUTTON_NONE,

        snesCon_BUTTON_Down,
        snesCon_BUTTON_NONE,
        snesCon_BUTTON_Down,
        snesCon_BUTTON_NONE,

        snesCon_BUTTON_Left,
        snesCon_BUTTON_NONE,
        snesCon_BUTTON_Right,
        snesCon_BUTTON_NONE,

        snesCon_BUTTON_Left,
        snesCon_BUTTON_NONE,
        snesCon_BUTTON_Right,
        snesCon_BUTTON_NONE,

        snesCon_BUTTON_Left,
        snesCon_BUTTON_NONE,
        snesCon_BUTTON_Right,
        snesCon_BUTTON_NONE,

        snesCon_BUTTON_B,
        snesCon_BUTTON_NONE,
        snesCon_BUTTON_A,
        snesCon_BUTTON_NONE,
    };

    snesCon_client_update(&controller, konamiCode[cntr++ % ARRAYLEN(konamiCode)]);
}

void LATCH_IRQHandler()
{
    snesCon_client_latch(&controller);
    EXTI.PR = 0x1 << LATCH.pin;
}

void CLOCK_IRQHandler()
{
    snesCon_client_clock(&controller);
    EXTI.PR = 0x1 << CLOCK.pin;
}
