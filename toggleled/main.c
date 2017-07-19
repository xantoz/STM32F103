#include "types.h"
#include "utils.h"
#include "rcc.h"
#include "gpio.h"
#include "afio.h"
#include "clock.h"
#include "systick.h"
#include "exti.h"
#include "nvic.h"

#include "debug.h"

void main(void)
{
    clock_setSysClockHSE();

    __disable_irq();

    // Enable clock to GPIO:s
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;

    GPIO_setMODE_setCNF(&GPIOC, 13, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    // Interrupt EXTIO0 on falling edge of PB0
    EXTI.IMR  |=  0x00000001; // Enable interrupt from EXTI0
    EXTI.FTSR |=  0x00000001; // Set falling trigger select for EXTI0
    EXTI.RTSR &= ~0x00000001; // Disable rising trigger select for EXTI0
    NVIC_EnableInterrupt(EXTI0_IRQn);
    AFIO_mapEXTI(0, AFIO_EXTI_PortB); // Map PB[0] to EXTI0
    GPIO_setMODE_setCNF(&GPIOB, 0, GPIO_MODE_Input, GPIO_Input_CNF_PullupPulldown);
    GPIO_setPin(&GPIOB, 0); // Pull-up

    __enable_irq();

    uint32_t cntr = 0;
    while (true)
    {
        if ((cntr % 1234090) == 0)
        {
            println_u32_hex(cntr);
            // EXTI.SWIER = 0x00000001;
        }
        ++cntr;
    }
}

void EXTI0_IRQHandler(void)
{
    static bool state = true;
    print("EXTIO0\n");
    state = !state;
    if (state)
        GPIO_resetPin(&GPIOC, 13);
    else
        GPIO_setPin(&GPIOC, 13);

    EXTI.PR = 0xffffffff;
}
