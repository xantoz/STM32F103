/**
 * @file exti.h
 *
 * @brief EXTI (External Interrupt) register definitions
 */

#ifndef _EXTI_
#define _EXTI_

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

#endif
