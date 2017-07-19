#include "types.h"
#include "utils.h"
#include "rcc.h"
#include "clock.h"
#include "debug.h"
#include "afio.h"
#include "exti.h"
#include "nvic.h"
#include "IRQn.h"

#include "nRF24L01/nRF24L01.h"

#include "spi-setup.h"

#define DELAY 200000

// SETUP: A0 receives IRQ when recv
static void exti_setup()
{
    // Interrupt EXTIO0 on falling edge of PB0
    EXTI.IMR  |=  0x00000001; // Enable interrupt from EXTI0
    EXTI.FTSR |=  0x00000001; // Set falling trigger select for EXTI0
    EXTI.RTSR &= ~0x00000001; // Disable rising trigger select for EXTI0
    NVIC_EnableInterrupt(EXTI0_IRQn);
    AFIO_mapEXTI(0, AFIO_EXTI_PortA); // Map PA[0] to EXTI0
    GPIO_setMODE_setCNF(&GPIOA, 0, GPIO_MODE_Input, GPIO_Input_CNF_PullupPulldown);
    GPIO_setPin(&GPIOA, 0); // Pull-up
}

static void recv_message(const struct nRF24L01 *dev, const void *data, size_t len)
{
    assert(len == 2);
    uint16_t msg = *((uint16_t*)data);

    static bool state = true;
    state = !state;
    if (state)
        GPIO_resetPin(&GPIOC, 13);
    else
        GPIO_setPin(&GPIOC, 13);

    print("Got message: ");
    println_u32_dec(msg);

    // Set port B
    GPIOB.ODR = msg;
}

// Settings for the nRF24L01
static const struct nRF24L01_Options rfDev_opts = {
    .CSN = {&GPIOA, 3},
    .CE  = {&GPIOA, 1},

    .airDataRate    = nRF24L01_2Mbps,
    .useACK         = nRF24L01_NoACK,
    .useCRC         = nRF24L01_NoCRC,
    .retransmission = nRF24L01_NoRetransmission,
    .mode           = nRF24L01_RX,

    .payloadWidth   = 2,
    .channel        = 33,

    .spi_sendrecv   = &spi_sendrecv,
    .rx_cb          = &recv_message
};

struct nRF24L01 rfDev;

void main()
{
    clock_setSysClockHSE();
    // clock_setSysClockHSE_24MHz();

    delay_us(1000000);

    __disable_irq();

    // Enable clock to all GPIO:s
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;

    RCC.APB2ENR |= RCC_APB2Periph_SPI1;    // Enable clock to SPI1

    GPIO_setMODE_setCNF(&GPIOC, 13, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_setPin(&GPIOC, 13);
    GPIO_setMODE_setCNF(&GPIOC, 15, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_resetPin(&GPIOC, 15);

    // enable entire PORT B as output
    for (uint8_t i = 0; i < 15; ++i)
        GPIO_setMODE_setCNF(&GPIOB, i, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    GPIO_setPin(&GPIOC, 15);
    spi_setup();
    print("spi setup done\n");
    GPIO_resetPin(&GPIOC, 15);

    GPIO_setPin(&GPIOC, 15);
    exti_setup();
    print("exti setup done\n");
    GPIO_resetPin(&GPIOC, 15);

    GPIO_setPin(&GPIOC, 15);
    nRF24L01_init(&rfDev_opts, &rfDev);
    print("nRF24L01 initted\n");
    GPIO_resetPin(&GPIOC, 15);

    GPIO_resetPin(&GPIOC, 13);

    __enable_irq();

    while (true)
    {
        delay_us(1000000);
        print("boop\n");
        println_u32_hex(rfDev.status);
    };
}

void EXTI0_IRQHandler(void)
{
    __disable_irq();
    print("EXTI0\n");
    // EXTI0 is connected to the interrupt line coming from the nRF24L01
    nRF24L01_interrupt(&rfDev);

    EXTI.PR = 0xffffffff;
    __enable_irq();
}
