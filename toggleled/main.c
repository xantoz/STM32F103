#include "lib/types.h"
#include "lib/utils.h"
#include "lib/rcc.h"
#include "lib/gpio.h"
#include "lib/afio.h"
#include "lib/clock.h"
#include "lib/systick.h"
#include "lib/exti.h"
#include "lib/nvic.h"

#include "lib/debug.h"

void main(void)
{
    clock_setSysClockHSE();

    __disable_irq();

    // Enable clock to GPIO:s
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;

    GPIO_setMODE_setCNF(&GPIOC, 13, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    // Interrupt EXTIO0 on falling edge of PB0
    GPIO_setMODE_setCNF(&GPIOB, 0, GPIO_MODE_Input, GPIO_Input_CNF_PullupPulldown);
    GPIO_setPin(&GPIOB, 0); // Pull-up
    EXTI.IMR |= 0x00000001; // Enable interrupt from EXTI0
    AFIO.EXTICR[0] &= ~(AFIO_EXTICR1_EXTI0);
    AFIO.EXTICR[0] |= AFIO_EXTICR1_EXTI0_PB;
    NVIC_EnableInterrupt(EXTI0_IRQn);

    __enable_irq();

    uint32_t cntr = 0;
    while (true)
    {
        if ((cntr % 1234090) == 0)
            print_hex(cntr);
        ++cntr;
    }
}

void EXTIO0_IRQHandler(void)
{
    static bool state = 0;
    state = !state;
    if (state)
        GPIO_resetPin(&GPIOC, 13);
    else
        GPIO_setPin(&GPIOC, 13);
}
