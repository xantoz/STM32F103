#include "protocol/snesCon_client.h"

#include "gpio.h"
#include "debug.h"
#include "clock.h"
#include "rcc.h"
#include "systick.h"
#include "exti.h"

#define LED ((struct GPIO_PortPin){&GPIOC, 13})

#define CLOCK ((struct GPIO_PortPin){&GPIOB, 7})
#define LATCH ((struct GPIO_PortPin){&GPIOB, 8})
#define DATA  ((struct GPIO_PortPin){&GPIOB, 9})

struct snesCon_client controller = {
    .pinDef = {
        .clock = CLOCK,
        .latch = LATCH,
        .data =  DATA,
    }
};

void main()
{
    clock_setSysClockHSE_48MHz();

    __disable_irq();

    // Enable clock to all GPIO:s
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;

    GPIO_setMODE_setCNF(&LED, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_setPin(&LED);

    snesCon_client_init(&controller);
    snesCon_client_update(&controller, 0x0000);

    if (!systick_startSysTick_ms(1000))
        die("Could not set up systick");

    __enable_irq();

    return;
}

static const snesCon_btn_t konamiCode[] = {
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

void Systick_Handler()
{
    static bool state = false;
    state = !state;
    if (state)
        GPIO_setPin(&LED);
    else
        GPIO_resetPin(&LED);

    static uint32_t cntr = 0;

    // snesCon_client_update(&controller, konamiCode[cntr++ % ARRAYLEN(konamiCode)]);

    if (state)
    {
        uint16_t rolf = (cntr++ % 12);
        println_u32_dec(rolf);
        snesCon_client_update(&controller, 1 << rolf);
        print("press ");

    }
    else
    {
        snesCon_client_update(&controller, 0);
        print("release\n");
    }
}

void EXTI9_5_IRQHandler()
{
    const uint32_t LATCH_Msk = (0x1 << LATCH.pin);
    const uint32_t CLOCK_Msk = (0x1 << CLOCK.pin);

    if (EXTI.PR & CLOCK_Msk)
    {
        snesCon_client_clock(&controller);
        EXTI.PR = CLOCK_Msk;
    }
    if (EXTI.PR & LATCH_Msk)
    {
        snesCon_client_latch(&controller);
        EXTI.PR = LATCH_Msk;
    }

    // EXTI.PR = ~0;
}
