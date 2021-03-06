#include "utils.h"
#include "rcc.h"
#include "gpio.h"
#include "clock.h"

#include "debug.h"

void delay(volatile uint32_t delay)
{
    for(; delay; --delay );
}

void main(void)
{
    // clock_setSysClockHSE();
    // clock_setSysClockHSE_24MHz();
    clock_setSysClockHSI_24MHz();

    // Enable clock to GPIOC
    RCC.APB2ENR |= RCC_APB2Periph_GPIOC;

    print("GPIOC.CRH before: ");
    println_u32_hex(GPIOC.CRH);
    print("GPIOC.CRL before: ");
    println_u32_hex(GPIOC.CRL);

    GPIO_setMODE_setCNF(&GPIOC, 13, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    // GPIO_setMODE(GPIOC, 13, GPIO_MODE_Output_10MHz);
    // GPIO_setCNF(GPIOC, 13, GPIO_Output_CNF_GPPushPull);

    put_char('h');
    put_char('e');
    put_char('j');
    put_char('\n');

    print("GPIOC.CRH after: ");
    println_u32_hex(GPIOC.CRH);
    print("GPIOC.CRL after: ");
    println_u32_hex(GPIOC.CRL);

    print("Hello world\n");
    println_u32_hex(time());
    println_u32_hex(clock());
    println_u32_hex(time());

    uint8_t cnt = 0;
    while (1) {
        if (cnt % 24 == 0)
        {
            print("HSI\n");
            clock_setSysClockHSI();
        }
        if (cnt % 24 == 3)
        {
            print("HSI_48Mhz\n");
            clock_setSysClockHSI_48MHz();
        }
        if (cnt % 24 == 7)
        {
            print("HSI_24MHz\n");
            clock_setSysClockHSI_24MHz();
        }
        if (cnt % 24 == 11)
        {
            print("HSE\n");
            clock_setSysClockHSE();
        }
        if (cnt % 24 == 15)
        {
            print("HSE_24Mhz\n");
            clock_setSysClockHSE_24MHz();
        }
        if (cnt % 24 == 19)
        {
            print("HSE_48Mhz\n");
            clock_setSysClockHSE_48MHz();
        }
        ++cnt;

        GPIOC.BSRR = 1 << 13;
        // GPIOC.ODR = 0;
        delay(0xAFFFF);
        // delay(0xAFFFF/2);
        GPIOC.BRR  = 1 << 13;
        // GPIOC.ODR = ~0;
        delay(0xAFFFF);
        // delay(0xAFFFF/2);
    }
}
