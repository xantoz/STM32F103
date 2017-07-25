/**
 * @file exti.h
 *
 * @brief EXTI (External Interrupt) register definitions
 */

#ifndef _EXTI_
#define _EXTI_

#include "types.h"
#include "gpio.h"

/**
 * @brief EXTI register definitions
 */
struct EXTI_Regs
{
    __IO uint32_t IMR;   //!< Interrupt mask register
    __IO uint32_t EMR;   //!< Event mask register
    __IO uint32_t RTSR;  //!< Rising trigger selection register
    __IO uint32_t FTSR;  //!< Falling trigger selection register
    __IO uint32_t SWIER; //!< Software interrupt event register
    __IO uint32_t PR;    //!< Pending register
};

extern volatile struct EXTI_Regs EXTI;

/**
 * @brief EXTI flank trigger selector
 */
enum EXTI_Flank
{
    EXTI_NONE,    //!< Trigger on no flank (effectively disabling, useful for SW-triggered interrupts)
    EXTI_RISING,  //!< Trigger on rising flank
    EXTI_FALLING, //!< Trigger on falling flank
    EXTI_BOTH,    //!< Trigger on both flanks
};

/**
 * @brief Enable EXTI IRQ for `portpin` on flank `flank`
 *
 * @note  GPIO setup needs to be done separately
 * @note  Pins 0-4 have an interrupt handler each
 * @note  Pins 5-9 share the same interrupt handler, you will have to inspect EXTI.PR
 * @note  Pins 10-15 share the same interrupt handler, you will have to inspect EXTI.PR
 */
void EXTI_enableInterrupt(const struct GPIO_PortPin * const portpin, enum EXTI_Flank flank);

/**
 * @brief Disable EXTI IRQ for `portpin`
 */
void EXTI_disableInterrupt(const struct GPIO_PortPin * const portpin);

#endif
