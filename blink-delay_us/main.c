#include "utils.h"
#include "rcc.h"
#include "gpio.h"
#include "clock.h"

#include "debug.h"

void main(void)
{
    clock_setSysClockHSI_24MHz();

    // Enable clock to GPIOC
    RCC.APB2ENR |= RCC_APB2Periph_GPIOC;


    GPIO_setMODE_setCNF(&GPIOC, 13, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    uint8_t cnt = 0;
    while (1) {
        if (cnt % 16 == 0)
        {
            print("HSI\n");
            clock_setSysClockHSI();
        }
        if (cnt % 16 == 3)
        {
            print("HSI_24MHz\n");
            clock_setSysClockHSI_24MHz();
        }
        if (cnt % 16 == 7)
        {
            print("HSE\n");
            clock_setSysClockHSE();
        }
        if (cnt % 16 == 11)
        {
            print("HSE_24Mhz\n");
            clock_setSysClockHSE_24MHz();
        }
        ++cnt;

        GPIO_setPin(&GPIOC, 13);
        delay_us(1000000);
        GPIO_resetPin(&GPIOC, 13);
        delay_us(1000000);
    }
}
