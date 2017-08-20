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

#include "protocol/pceCon_client.h"
#include "common/debugLeds.h"

#include "config.h"

struct nRF24L01 rfDev;

void main()
{
    // clock_setSysClockHSE_24MHz();
    clock_setSysClockHSE_72MHz();

    __disable_irq();

    delay_us(1000000);

    // Enable clock to GPIOA & GPIOB & GPIOC
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;
    RCC.APB2ENR |= RCC_APB2Periph_SPI1;    // Enable clock to SPI1

    // Disable JTAG (but keep SWD) to free PB3 and PB4 for GPIO use
    AFIO.MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    GPIO_setMODE_setCNF(&LED, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    NVIC_setInterruptPriority(nRF24L01_IRQn, 4);
    NVIC_setInterruptPriority(pceCon_IRQn, 0);

    SPI_initAsMaster(&nRF24L01_SPI, &spi_opts);
    EXTI_enableInterrupt(&nRF24L01_IRQ_PortPin, EXTI_FALLING);
    GPIO_setMODE_setCNF(&nRF24L01_IRQ_PortPin, GPIO_MODE_Input, GPIO_Input_CNF_Floating);
    nRF24L01_init(&rfDev_opts_rx, &rfDev);

    debugLeds_init();

    pceCon_client_init(&controller);

    GPIO_resetPin(&LED);

    __enable_irq();
}

void nRF24L01_IRQHandler(void)
{
    nRF24L01_interrupt(&rfDev);
    EXTI.PR = 0x1 << nRF24L01_IRQ_PortPin.pin;
}

IN_SECTION(".text.fastcode")
void pceCon_IRQHandler()
{
    const uint32_t ENABLE_Msk = (0x1 << controller.pin.enable.pin);
    const uint32_t SELECT_Msk = (0x1 << controller.pin.select.pin);

    if (EXTI.PR & SELECT_Msk)
    {
        pceCon_client_select(&controller);
        EXTI.PR = SELECT_Msk;
    }
    if (EXTI.PR & ENABLE_Msk)
    {
        pceCon_client_enable(&controller);
        EXTI.PR = ENABLE_Msk;
    }
}

void recv_message(UNUSED const struct nRF24L01 *dev, UNUSED uint8_t pipeNo,
                         const void *data, size_t len)
{
    assert(len == sizeof(pceCon_btn_t));
    const pceCon_btn_t msg = *((pceCon_btn_t*)data);
    pceCon_client_update(&controller, msg);

    debugLeds_update(msg);

    static bool state = true;
    GPIO_setBit(&LED, state);
    state = !state;
}
