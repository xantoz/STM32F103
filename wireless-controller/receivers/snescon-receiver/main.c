#include <gpio.h>
#include <rcc.h>
#include <clock.h>
#include <utils.h>
#include <debug.h>
#include <exti.h>
#include <IRQn.h>
#include <nvic.h>

#include <protocol/snesCon_client.h>
#include <common/rf.h>
#include <common/dip.h>

#include "config.h"

struct snesCon_client controller = {
    .pinDef = snesCon_PINS
};

static void maybe_update_addr()
{
    static uint8_t last_addr = 0;
    const uint8_t new_addr = dip_read();
    if (new_addr != last_addr)
    {
        rf_setRxAddress(new_addr, 0);
        last_addr = new_addr;
    }
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

    static uint32_t cntr = 0;
    if (++cntr % 16)
        maybe_update_addr();
}

static void recv_message(UNUSED const struct nRF24L01 *dev,
                         UNUSED uint8_t pipeNo,
                         const void *data, size_t len)
{
    assert(len == sizeof(snesCon_btn_t));
    const snesCon_btn_t msg = *((snesCon_btn_t*)data);
    snesCon_client_update(&controller, msg);

    static bool state = true;
    GPIO_setBit(&LED, state);
    state = !state;
}

void main()
{
    clock_setSysClockHSE_24MHz();

    __disable_irq();

    delay_us(1000000);

    // Enable clock to GPIOA & GPIOB & GPIOC
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;
    RCC.APB2ENR |= RCC_APB2Periph_SPI1;    // Enable clock to SPI1

    GPIO_setMODE_setCNF(&LED, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    rf_init(rf_Rx, &recv_message, sizeof(snesCon_btn_t), 1);

    dip_init();

    NVIC_setInterruptPriority(snesCon_IRQn, snesCon_IRQ_Priority);
    snesCon_client_init(&controller);

    GPIO_resetPin(&LED);

    __enable_irq();
}
