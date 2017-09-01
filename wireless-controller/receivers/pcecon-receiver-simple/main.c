/**
 * @file main.c
 *
 * @brief A PCE controller receiver with a faster, hard-coded, implementation of the protocol,
 *        for lower interrupt latency.
 */

#include <gpio.h>
#include <rcc.h>
#include <clock.h>
#include <utils.h>
#include <debug.h>
#include <exti.h>
#include <IRQn.h>
#include <nvic.h>
#include <afio.h>

#include <protocol/pceCon.h>
#include <common/debugLeds.h>
#include <common/rf.h>

#include "config.h"

void main()
{
    clock_setSysClockHSE_72MHz();

    __disable_irq();

    // delay_us(1000000);

    // Enable clock to GPIOA & GPIOB & GPIOC
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;
    RCC.APB2ENR |= RCC_APB2Periph_SPI1;    // Enable clock to SPI1

    // Disable JTAG (but keep SWD) to free PB3 and PB4 for GPIO use
    AFIO.MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    GPIO_setMODE_setCNF(&LED, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    rf_init(rf_Rx);

    NVIC_setInterruptPriority(pceCon_IRQn, pceCon_IRQ_Priority);
    for (unsigned i = 0; i < ARRAYLEN(c_outputPins); ++i)
        GPIO_setMODE_setCNF(&OUTPUT_Port, c_outputPins[i],
                            GPIO_MODE_Output_50MHz, GPIO_Output_CNF_GPOpenDrain);
    GPIO_setMODE_setCNF(&ENABLE_PortPin, GPIO_MODE_Input, GPIO_Input_CNF_Floating);
    EXTI_enableInterrupt(&ENABLE_PortPin, EXTI_RISING);
    GPIO_setMODE_setCNF(&SELECT_PortPin, GPIO_MODE_Input, GPIO_Input_CNF_Floating);
    EXTI_enableInterrupt(&SELECT_PortPin, EXTI_BOTH);

    debugLeds_init();

    if (DEBUG_ENABLE_INTERRUPT)
    {
        GPIO_setMODE_setCNF(&DEBUG_PortPin, GPIO_MODE_Output_50MHz, GPIO_Output_CNF_GPPushPull);
        GPIO_resetPin(&DEBUG_PortPin);
    }

    GPIO_resetPin(&LED);

    __enable_irq();
}

static uint16_t g_btn = 0;
#if SIXBUTTON == 1
void pceCon_IRQHandler()
{
    const uint32_t ENABLE_Msk = (0x1 << ENABLE_PortPin.pin);
    const uint32_t SELECT_Msk = (0x1 << SELECT_PortPin.pin);
    static uint32_t cntr = 0;
    irq_lock_t lock;

    if (EXTI.PR & ENABLE_Msk)
    {
        ++cntr;
        if (DEBUG_ENABLE_INTERRUPT)
        {
            GPIO_togglePin(&DEBUG_PortPin);
        }
        EXTI.PR = ENABLE_Msk;
    }

    uint32_t shift = (GPIO_read(&SELECT_PortPin)) ? 4 : 0;
    if (cntr & 1)
        shift += 8;

    const uint32_t out = (g_btn >> shift) & 0x0f;
    LOCK_IRQ(lock);
    GPIOB.ODR = (GPIOB.ODR & OUTPUT_Msk) | (out << OUTPUT_Pos);
    UNLOCK_IRQ(lock);

    if (EXTI.PR & SELECT_Msk)
        EXTI.PR = SELECT_Msk;
}
#else
void pceCon_IRQHandler()
{
    const uint32_t ENABLE_Msk = (0x1 << ENABLE_PortPin.pin);
    const uint32_t SELECT_Msk = (0x1 << SELECT_PortPin.pin);
    static uint32_t cntr = 0;

    const uint32_t shift = (GPIO_read(&GPIOB, 8)) ? 4 : 0;
    const uint32_t out = (g_btn >> shift) & 0x0f;
    const uint32_t odr = GPIOB.ODR;
    GPIOB.ODR = (odr & OUTPUT_Msk) | (out << OUTPUT_Pos);

    if (EXTI.PR & SELECT_Msk)
    {
        EXTI.PR = SELECT_Msk;
    }
    if (EXTI.PR & ENABLE_Msk)
    {
        ++cntr;
        EXTI.PR = ENABLE_Msk;
    }
}
#endif
void recv_message(UNUSED const struct nRF24L01 *dev, UNUSED uint8_t pipeNo,
                  const void *data, size_t len)
{
    assert(len == sizeof(pceCon_btn_t));
    const pceCon_btn_t msg = *((pceCon_btn_t*)data);
    // Order of bits reversed since the order of the output pins is reversed (TODO: use non-inverted order)
    g_btn = ~((0xf                              << 12) |
              ((!!(msg & pceCon_BUTTON_III))    << 11) |
              ((!!(msg & pceCon_BUTTON_IV))     << 10) |
              ((!!(msg & pceCon_BUTTON_V))      << 9)  |
              ((!!(msg & pceCon_BUTTON_VI))     << 8)  |
              ((!!(msg & pceCon_BUTTON_Up))     << 7)  |
              ((!!(msg & pceCon_BUTTON_Right))  << 6)  |
              ((!!(msg & pceCon_BUTTON_Down))   << 5)  |
              ((!!(msg & pceCon_BUTTON_Left))   << 4)  |
              ((!!(msg & pceCon_BUTTON_II))     << 3)  |
              ((!!(msg & pceCon_BUTTON_I))      << 2)  |
              ((!!(msg & pceCon_BUTTON_Select)) << 1)  |
              ((!!(msg & pceCon_BUTTON_Run))    << 0));

    debugLeds_update(msg);
}
