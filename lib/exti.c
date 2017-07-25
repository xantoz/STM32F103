#include "exti.h"

#include "afio.h"
#include "nvic.h"
#include "IRQn.h"
#include "debug.h"

static enum AFIO_EXTICRx_Flg EXTI_GPIOxToAFIOPort(volatile const struct GPIO_Port * const port)
{
    if (port == &GPIOA)
        return AFIO_EXTI_PortA;
    else if (port == &GPIOB)
        return AFIO_EXTI_PortB;
    else if (port ==  &GPIOC)
        return AFIO_EXTI_PortC;
    else if (port ==  &GPIOD)
        return AFIO_EXTI_PortD;
    else if (port ==  &GPIOE)
        return AFIO_EXTI_PortE;
    else if (port ==  &GPIOF)
        return AFIO_EXTI_PortF;
    else if (port ==  &GPIOG)
        return AFIO_EXTI_PortG;

    die("no such port");
}

static enum IRQn EXTI_pinNrToIRQn(uint8_t pin)
{
    if (pin <= 4)
    {
        return EXTI0_IRQn + pin;
    }
    else if (5 <= pin && pin <= 9)
    {
        return EXTI9_5_IRQn;
    }
    else if (9 <= pin && pin <= 15)
    {
        return EXTI15_10_IRQn;
    }

    die("pin out of range");
}

void EXTI_enableInterrupt(const struct GPIO_PortPin * const portpin, enum EXTI_Flank flank)
{
    assert(IS_GPIO(*portpin->port));
    assert(portpin->pin <= 15);

    irq_lock_t lock;
    LOCK_IRQ(lock);

    const uint32_t bit = 0x1 << portpin->pin;
    switch (flank)
    {
        case EXTI_NONE:
            EXTI.FTSR &= ~bit; // Disable falling trigger select
            EXTI.RTSR &= ~bit; // Disable rising trigger select
            break;
        case EXTI_RISING:
            EXTI.FTSR &= ~bit; // Disable falling trigger select
            EXTI.RTSR |=  bit; // Enable rising trigger select
            break;
        case EXTI_FALLING:
            EXTI.FTSR |=  bit; // Enable falling trigger select
            EXTI.RTSR &= ~bit; // Disable rising trigger select
            break;
        case EXTI_BOTH:
            EXTI.FTSR |=  bit; // Enable falling trigger select
            EXTI.RTSR |=  bit; // Enable rising trigger select
            break;
    }
    AFIO_mapEXTI(portpin->pin, EXTI_GPIOxToAFIOPort(portpin->port)); // Map port
    EXTI.PR = bit;                                          // Clear bit in pending register
    NVIC_enableInterrupt(EXTI_pinNrToIRQn(portpin->pin));
    EXTI.IMR |= bit;                                        // Enable interrupt for EXTIx

    UNLOCK_IRQ(lock);
}

void EXTI_disableInterrupt(const struct GPIO_PortPin * const portpin)
{
    assert(IS_GPIO(*portpin->port));
    assert(portpin->pin <= 15);

    irq_lock_t lock;
    LOCK_IRQ(lock);

    const uint32_t bit = 0x1 << portpin->pin;
    NVIC_disableInterrupt(EXTI_pinNrToIRQn(portpin->pin));
    EXTI.IMR &= ~bit;

    UNLOCK_IRQ(lock);
}
