#include "memorymap.h"
#include "utils.h"
#include "rcc.h"
#include "gpio.h"
#include "clock.h"
#include "systick.h"

#include "debug.h"

void main(void)
{
    clock_setSysClockHSE();

    // Enable clock to GPIOC
    RCC->APB2ENR |= RCC_APB2Periph_GPIOC;

    GPIO_setMODE(GPIOC, 13, GPIO_MODE_Output_10MHz);
    GPIO_setCNF(GPIOC, 13, GPIO_Output_CNF_GPPushPull);

    if (!systick_startSysTick_ms(1000))
        die("Could not set up systick");

    uint32_t asdf = 0;
    while (true)
    {
        if (asdf & 512)
            print_hex(SysTick->VAL);
        ++asdf;
    }
}

void systick_handler(void)
{
    static uint32_t counter = 0;
    // BKPT();
    // print("systick\n");
    switch (counter & 1)
    {
        case 0:
            GPIOC->BSRR = 1 << 13;
            break;
        case 1:
            GPIOC->BRR  = 1 << 13;
            break;
    }
    ++counter;
}
