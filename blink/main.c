#include "lib/utils.h"
#include "lib/rcc.h"
#include "lib/gpio.h"
#include "lib/clock.h"

#include "lib/debug.h"

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
    println_hex(GPIOC.CRH);
    print("GPIOC.CRL before: ");
    println_hex(GPIOC.CRL);

    GPIO_setMODE_setCNF(&GPIOC, 13, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    // GPIO_setMODE(GPIOC, 13, GPIO_MODE_Output_10MHz);
    // GPIO_setCNF(GPIOC, 13, GPIO_Output_CNF_GPPushPull);

    put_char('h');
    put_char('e');
    put_char('j');
    put_char('\n');

    print("GPIOC.CRH after: ");
    println_hex(GPIOC.CRH);
    print("GPIOC.CRL after: ");
    println_hex(GPIOC.CRL);

    print("Hello world\n");
    println_hex(time());
    println_hex(clock());
    println_hex(time());

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
