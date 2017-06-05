/**
 * @brief High-level interface to set the various clocks, etc.
 */

#ifndef _CLOCK_
#define _CLOCK_

#include "types.h"

#define CLOCK_HSE_Hz ((uint32_t)8000000) //!< Speed of the HSE (external) clock in Hz (board dependent)
#define CLOCK_HSI_Hz ((uint32_t)8000000) //!< Speed of the HSI (internal) clock in Hz

/**
 * \brief Clock frequencies struct
 */
typedef struct
{
    uint32_t sysclkFreq;   //!< Current SYSCLK speed in Hz
    uint32_t hclkFreq;     //!< Current HCLK speed in Hz
    uint32_t pclk2Freq;    //!< Current PCLK2 speed in Hz
    uint32_t pclk1Freq;    //!< Currentl PCLK1 speed in Hz
    uint32_t sysTickFreq;  //!< Currentl SysTick speed in Hz
    uint32_t timerFreq;    //!< Current TIMxCLK speed in Hz
} clock_t;

/**
 * \brief Clock frequencies global variable
 */
extern clock_t g_clock;

/**
 * @brief Set SYSCLK to HSE, and adjust FLASH parameters.
 *        set HCLK = SYSCLK/1, PCLK2 = HCLK/1, PCLK1 = HCLK/1
 *
 * @note  Updates g_clock
 */
void clock_setSysClockHSE();

/**
 * @brief Set SYSCLK to 24 MHz via PLLCLK and HSE
 *        set SYSCLK = (HSE/2)*6, HCLK = SYSCLK/1, PCLK1 = HCLK/1, PCLK2 = HCLK/1
 *
 * @note  Updates g_clock
 */
void clock_setSysClockHSE_24MHz();

/**
 * @brief Set SYSCLK to 24 MHz via PLLCLK and HSI.
 *        set SYSCLK = (HSI/2)*6, HCLK = SYSCLK/1, PCLK1 = HCLK/1, PCLK2 = HCLK/1
 *
 * @note  Updates g_clock
 */
void clock_setSysClockHSI_24MHz();

/**
 * @brief set SYSCLK to HSI
 *
 * @note  Updates g_clock
 */
void clock_setSysClockHSI();

/**
 * @brief Set SysTick to HCLK
 *
 * @note  Updates g_clock
 */
void clock_setSysTick_HCLK();

/**
 * @brief Set SysTick to HCLK/8
 *
 * @note  Updates g_clock
 */
void clock_setSysTick_HCLK_Div8();


#endif
