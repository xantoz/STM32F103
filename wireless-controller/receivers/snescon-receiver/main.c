#include "gpio.h"
#include "rcc.h"
#include "clock.h"
#include "utils.h"
#include "debug.h"
#include "exti.h"
#include "IRQn.h"
#include "nvic.h"

#include "spi.h"
#include "nRF24L01/nRF24L01.h"

#include "protocol/snesCon_client.h"
#include "common/debugLeds.h"

#include "config.h"

struct nRF24L01 rfDev;

struct snesCon_client controller = {
    .pinDef = snesCon_PINS
};

void main()
{
    clock_setSysClockHSE_24MHz();

    __disable_irq();

    delay_us(1000000);

    // Enable clock to GPIOA & GPIOB & GPIOC
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;
    RCC.APB2ENR |= RCC_APB2Periph_SPI1;    // Enable clock to SPI1

    GPIO_setMODE_setCNF(&LED, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    NVIC_setInterruptPriority(nRF24L01_IRQn, 4);
    NVIC_setInterruptPriority(snesCon_IRQn, 3);

    debugLeds_init();

    SPI_initAsMaster(&nRF24L01_SPI, &spi_opts);
    EXTI_enableInterrupt(&nRF24L01_IRQ_PortPin, EXTI_FALLING);
    GPIO_setMODE_setCNF(&nRF24L01_IRQ_PortPin, GPIO_MODE_Input, GPIO_Input_CNF_Floating);
    nRF24L01_init(&rfDev_opts_rx, &rfDev);
    nRF24L01_interrupt(&rfDev);

    snesCon_client_init(&controller);

    GPIO_resetPin(&LED);

    __enable_irq();
}

void snesCon_IRQHandler()
{
    const uint32_t LATCH_Msk = (0x1 << snesCon_PINS.latch.pin);
    const uint32_t CLOCK_Msk = (0x1 << snesCon_PINS.clock.pin);

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
}

void nRF24L01_IRQHandler(void)
{
    nRF24L01_interrupt(&rfDev);
    EXTI.PR = 0x1 << nRF24L01_IRQ_PortPin.pin;
}

void recv_message(UNUSED const struct nRF24L01 *dev, UNUSED uint8_t pipeNo,
                  const void *data, size_t len)
{
    assert(len == sizeof(snesCon_btn_t));
    const snesCon_btn_t msg = *((snesCon_btn_t*)data);
    snesCon_client_update(&controller, msg);
    debugLeds_update(msg);

    static bool state = true;
    GPIO_setBit(&LED, state);
    state = !state;
}
