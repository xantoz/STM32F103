#include "gpio.h"

void GPIO_setMODE(volatile GPIO_Port * const GPIOx,
                  const uint8_t pin,
                  const uint8_t mode)
{
    const uint32_t shift = ((pin % 8)*4);
    const uint32_t mask = 0b11 << shift;
    const uint32_t val  = mode << shift;
    HW_RW *CR = GPIO_getCR(GPIOx, pin);
    *CR = (*CR & ~mask) | (val & mask);
}

void GPIO_setCNF(volatile GPIO_Port * const GPIOx,
                 const uint8_t pin,
                 const uint8_t cnf)
{
    const uint32_t shift = ((pin % 8)*4 + 2);
    const uint32_t mask = 0b11 << shift;
    const uint32_t val  = cnf << shift;
    HW_RW *CR = GPIO_getCR(GPIOx, pin);
    *CR = (*CR & ~mask) | (val & mask);
}

void GPIO_setMODE_setCNF(volatile GPIO_Port * const GPIOx,
                         const uint8_t pin,
                         const uint8_t cnf,
                         const uint8_t mode)
{
    const uint32_t shift = ((pin % 8)*4);
    const uint32_t mask = 0b1111 << shift;
    const uint32_t val  = (cnf | (mode << 2)) << shift;
    HW_RW *CR = GPIO_getCR(GPIOx, pin);
    *CR = (*CR & ~mask) | (val & mask);
}
