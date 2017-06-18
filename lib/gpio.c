#include "gpio.h"

#include "utils.h"

#define IS_GPIO(x) ((&(x) == &GPIOA) || (&(x) == &GPIOB) || (&(x) == &GPIOC))

void __GPIO_setMODE_impl(volatile struct GPIO_Port * const GPIOx,
                         const uint8_t pin,
                         const uint8_t mode)
{
    assert(IS_GPIO(*GPIOx));
    assert(pin <= 15);
    irq_lock_t lock;
    LOCK_IRQ(lock);

    const uint32_t shift = ((pin % 8)*4);
    const uint32_t mask = 0b11 << shift;
    const uint32_t val  = mode << shift;
    HW_RW * const CR = GPIO_getCR(GPIOx, pin);
    *CR = (*CR & ~mask) | (val & mask);

    UNLOCK_IRQ(lock);
}

void __GPIO_setCNF_impl(volatile struct GPIO_Port * const GPIOx,
                        const uint8_t pin,
                        const uint8_t cnf)
{
    assert(IS_GPIO(*GPIOx));
    assert(pin <= 15);
    irq_lock_t lock;
    LOCK_IRQ(lock);

    const uint32_t shift = ((pin % 8)*4 + 2);
    const uint32_t mask = 0b11 << shift;
    const uint32_t val  = cnf << shift;
    HW_RW * const CR = GPIO_getCR(GPIOx, pin);
    *CR = (*CR & ~mask) | (val & mask);

    UNLOCK_IRQ(lock);
}

void __GPIO_setMODE_setCNF_impl(volatile struct GPIO_Port * const GPIOx,
                                const uint8_t pin,
                                const uint8_t cnf,
                                const uint8_t mode)
{
    assert(IS_GPIO(*GPIOx));
    assert(pin <= 15);
    irq_lock_t lock;
    LOCK_IRQ(lock);

    const uint32_t shift = ((pin % 8)*4);
    const uint32_t mask = 0b1111 << shift;
    const uint32_t val  = (cnf | (mode << 2)) << shift;
    HW_RW * const CR = GPIO_getCR(GPIOx, pin);
    *CR = (*CR & ~mask) | (val & mask);

    UNLOCK_IRQ(lock);
}
