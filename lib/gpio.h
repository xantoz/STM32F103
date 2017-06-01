#ifndef _GPIO_
#define _GPIO_

#include "types.h"
#include "memorymap.h"
#include "vfunc.h"

/**
 * @brief GPIO
 */
typedef struct
{
    HW_RW CRL;  //!< Port configuration register low
    HW_RW CRH;  //!< Port configuration register high
    HW_RO IDR;  //!< Port input data register
    HW_RW ODR;  //!< Port output data register
    HW_WO BSRR; //!< Port bit set/reset register
    HW_WO BRR;  //!< Port bit reset register
    HW_RW LCKR; //!< Port configuration lock register
} GPIO_Port;

extern volatile GPIO_Port GPIOA;
extern volatile GPIO_Port GPIOB;
extern volatile GPIO_Port GPIOC;

/**
 * @brief Simple tuple that can be used to refer to a specific pin on a specific port.
 * Defined here as a convenience.
 */
typedef struct
{
    volatile GPIO_Port *port;
    uint8_t pin;
} GPIO_PortPin;

// Settings for CNF bits in CRL/CRH, input mode
#define GPIO_Input_CNF_Analog         0b00 //!< Analog mode
#define GPIO_Input_CNF_Floating       0b01 //!< Floating input
#define GPIO_Input_CNF_PullupPulldown 0b10 //!< Input with pull-up/pull-down
#define GPIO_Input_CNF_Reserved       0b11 //!< Reserved

// Settings for CNF bits in CRL/CRH, output mode
#define GPIO_Output_CNF_GPPushPull    0b00 //!< General purpose output push-pull
#define GPIO_Output_CNF_GPOpenDrain   0b01 //!< General purpose output open drain
#define GPIO_Output_CNF_AFPushPull    0b10 //!< Alternate function output push-pull
#define GPIO_Output_CNF_AFOpenDrain   0b11 //!< Alternate function output push-pull

// Settings for MODE bits in CRL/CRH
#define GPIO_MODE_Input               0b00 //!< Input mode (reset state)
#define GPIO_MODE_Output_10MHz        0b01 //!< Output mode, max speed 10 MHz
#define GPIO_MODE_Output_2MHz         0b10 //!< Output mode, max speed 2 MHz
#define GPIO_MODE_Output_50MHz        0b11 //!< Output mode, max speed 50 MHz

// TODO: add compile-time & maybe runtime assertions to check validity (compile-time case: Might
// need to restructure some inline funcs as macros)

/** @brief Returns pointer to CRL or CRH of GPIOx, depending on the pin */
__attribute__((pure))
static inline HW_RW *GPIO_getCR(volatile GPIO_Port * const GPIOx,
                                const uint8_t pin)
{
    return (pin >= 8) ? &GPIOx->CRH : &GPIOx->CRL;
}

static inline void GPIO_setMODE(volatile GPIO_Port * const GPIOx,
                                const uint8_t pin,
                                const uint8_t mode)
{
    uint32_t shift = ((pin % 8)*4);
    uint32_t mask = 0b11 << shift;
    uint32_t val  = mode << shift;
    HW_RW *CR = GPIO_getCR(GPIOx, pin);
    *CR = (*CR & ~mask) | (val & mask);
}

static inline void GPIO_setCNF(volatile GPIO_Port * const GPIOx,
                               const uint8_t pin,
                               const uint8_t cnf)
{
    uint32_t shift = ((pin % 8)*4 + 2);
    uint32_t mask = 0b11 << shift;
    uint32_t val  = cnf << shift;
    HW_RW *CR = GPIO_getCR(GPIOx, pin);
    *CR = (*CR & ~mask) | (val & mask);
}

static inline void GPIO_setMODE_setCNF(volatile GPIO_Port * const GPIOx,
                                       const uint8_t pin,
                                       const uint8_t cnf,
                                       const uint8_t mode)
{
    uint32_t shift = ((pin % 8)*4);
    uint32_t mask = 0b1111 << shift;
    uint32_t val  = (cnf | (mode << 2)) << shift;
    HW_RW *CR = GPIO_getCR(GPIOx, pin);
    *CR = (*CR & ~mask) | (val & mask);
}


#define __GPIO_define_portPin_alias(NAME) \
    static inline void NAME##1(const GPIO_PortPin * const portpin) { NAME##2(portpin->port, portpin->pin); }

#define GPIO_setPin(...) VFUNC(__GPIO_setPin, __VA_ARGS__)
static inline void __GPIO_setPin2(volatile GPIO_Port * const port, const uint8_t pin) {
    port->BSRR = (1 << pin);
}
__GPIO_define_portPin_alias(__GPIO_setPin)

#define GPIO_resetPin(...) VFUNC(__GPIO_resetPin, __VA_ARGS__)
static inline void __GPIO_resetPin2(volatile GPIO_Port * const port, const uint8_t pin) {
    port->BRR = (1 << pin);
}
__GPIO_define_portPin_alias(__GPIO_resetPin)

#define GPIO_read(...) VFUNC(__GPIO_read, __VA_ARGS__)
static inline bool __GPIO_read2(volatile GPIO_Port * const port, const uint8_t pin) {
    return (port->IDR >> pin) & 0x1;
}
static inline bool __GPIO_read1(const GPIO_PortPin * const portpin) {
    return __GPIO_read2(portpin->port, portpin->pin);
}

#endif /* _GPIO_ */
