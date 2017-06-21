#ifndef _GPIO_
#define _GPIO_

#include "types.h"
#include "vfunc.h"
#include "utils.h"
#include "debug.h"

/**
 * @brief GPIO
 */
struct GPIO_Port
{
    HW_RW CRL;  //!< Port configuration register low
    HW_RW CRH;  //!< Port configuration register high
    HW_RO IDR;  //!< Port input data register
    HW_RW ODR;  //!< Port output data register
    HW_WO BSRR; //!< Port bit set/reset register
    HW_WO BRR;  //!< Port bit reset register
    HW_RW LCKR; //!< Port configuration lock register
};

extern volatile struct GPIO_Port GPIOA;
extern volatile struct GPIO_Port GPIOB;
extern volatile struct GPIO_Port GPIOC;

#define IS_GPIO(x) ((&(x) == &GPIOA) || (&(x) == &GPIOB) || (&(x) == &GPIOC))

/**
 * @brief Simple tuple that can be used to refer to a specific pin on a specific port.
 */
struct GPIO_PortPin
{
    volatile struct GPIO_Port *port;
    uint8_t pin;
};

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
static inline PURE HW_RW *GPIO_getCR(volatile struct GPIO_Port * const GPIOx,
                                const uint8_t pin)
{
    return (pin >= 8) ? &GPIOx->CRH : &GPIOx->CRL;
}

void __GPIO_setMODE_impl(volatile struct GPIO_Port * const GPIOx,
                         const uint8_t pin,
                         const uint8_t mode);
#define _GPIO_setMODE3(GPIOx, pin, mode) __GPIO_setMODE_impl((GPIOx), (pin), (mode))
#define _GPIO_setMODE2(portpin, mode)    __GPIO_setMODE_impl((portpin)->port, (portpin)->pin, (mode))
#define GPIO_setMODE(...) VFUNC(_GPIO_setMODE, __VA_ARGS__)

void __GPIO_setCNF_impl(volatile struct GPIO_Port * const GPIOx,
                        const uint8_t pin,
                        const uint8_t cnf);
#define _GPIO_setCNF3(GPIOx, pin, cnf) __GPIO_setCNF_impl((GPIOx), (pin), (cnf))
#define _GPIO_setCNF2(portpin, cnf)    __GPIO_setCNF_impl((portpin)->port, (portpin)->pin, (cnf))
#define GPIO_setCNF(...) VFUNC(_GPIO_setCNF, __VA_ARGS__)

void __GPIO_setMODE_setCNF_impl(volatile struct GPIO_Port * const GPIOx,
                                const uint8_t pin,
                                const uint8_t cnf,
                                const uint8_t mode);
#define _GPIO_setMODE_setCNF4(GPIOx, pin, mode, cnf) __GPIO_setMODE_setCNF_impl((GPIOx), (pin), (mode), (cnf))
#define _GPIO_setMODE_setCNF3(portpin, mode, cnf)    __GPIO_setMODE_setCNF_impl((portpin)->port, (portpin)->pin, (mode), (cnf))
#define GPIO_setMODE_setCNF(...) VFUNC(_GPIO_setMODE_setCNF, __VA_ARGS__)

#define __GPIO_define_portPin_alias(NAME)                               \
    static INLINE void NAME##1(const struct GPIO_PortPin * const portpin) { NAME##2(portpin->port, portpin->pin); }

#define GPIO_setPin(...) VFUNC(__GPIO_setPin, __VA_ARGS__)
static INLINE void __GPIO_setPin2(volatile struct GPIO_Port * const port, const uint8_t pin) {
    build_assert(IS_GPIO(*port), "First argument is not GPIO port");
    build_assert(pin <= 15, "Pin number too high");

    port->BSRR = (1 << pin);
}
__GPIO_define_portPin_alias(__GPIO_setPin)

#define GPIO_resetPin(...) VFUNC(__GPIO_resetPin, __VA_ARGS__)
static INLINE void __GPIO_resetPin2(volatile struct GPIO_Port * const port, const uint8_t pin) {
    build_assert(IS_GPIO(*port), "First argument is not GPIO port");
    build_assert(pin <= 15, "Pin number too high");

    port->BRR = (1 << pin);
}
__GPIO_define_portPin_alias(__GPIO_resetPin)

#define GPIO_read(...) VFUNC(__GPIO_read, __VA_ARGS__)
static INLINE bool __GPIO_read2(volatile struct GPIO_Port * const port, const uint8_t pin) {
    build_assert(IS_GPIO(*port), "First argument is not GPIO port");
    build_assert(pin <= 15, "Pin number too high");

    return (port->IDR >> pin) & 0x1;
}
static INLINE bool __GPIO_read1(const struct GPIO_PortPin * const portpin) {
    return __GPIO_read2(portpin->port, portpin->pin);
}

#endif /* _GPIO_ */
