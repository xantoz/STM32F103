/**
 * @file  clock.h
 *
 * @brief High-level interface to set the various clocks, etc.
 */

#ifndef _CLOCK_
#define _CLOCK_

#include "types.h"

#define CLOCK_HSE_Hz ((uint32_t)8000000) //!< Speed of the HSE (external) clock in Hz (board dependent)
#define CLOCK_HSI_Hz ((uint32_t)8000000) //!< Speed of the HSI (internal) clock in Hz

#define SYSCLK_MAX ((uint32_t)72000000)                     //!< SYSCLK max frequency
#define PCLK2_MAX  ((uint32_t)72000000)                     //!< SYSCLK max frequency
#define PCLK1_MAX  ((uint32_t)36000000)                     //!< SYSCLK max frequency
#define ADCCLK_MAX ((uint32_t)14000000)                     //!< ADCCLK max frequency

/**
 * @brief Clock frequencies struct
 */
struct clocks
{
/** @privatesection */
    uint32_t sysclkFreq;   //!< Current SYSCLK speed in Hz
    uint32_t hclkFreq;     //!< Current HCLK speed in Hz
    uint32_t pclk2Freq;    //!< Current PCLK2 speed in Hz
    uint32_t pclk1Freq;    //!< Currentl PCLK1 speed in Hz
    uint32_t sysTickFreq;  //!< Currentl SysTick speed in Hz
    uint32_t timerFreq;    //!< Current TIMxCLK speed in Hz
    uint32_t adcClkFreq;   //!< Current ADCCLK speed in Hz
};

/**
 * @brief   Clock frequencies global variable
 * @details This variable is automatically updated by the functions in this module.
 */
extern struct clocks g_clock;

/**
 * @brief   Set SYSCLK to HSE.
 *
 * @details Sets HCLK = SYSCLK/1, PCLK2 = HCLK/1, PCLK1 = HCLK/1, ADCPRE, and adjusts FLASH parameters.
 *
 * @note    Updates g_clock
 */
void clock_setSysClockHSE();

/**
 * @brief   Set SYSCLK to 24 MHz via PLLCLK and HSE.
 *
 * @details Sets SYSCLK = (HSE/2)*6, HCLK = SYSCLK/1, PCLK1 = HCLK/1, PCLK2 = HCLK/1, ADCPRE, and
 *          adjusts FLASH parameters.
 *
 * @note    Updates g_clock
 */
void clock_setSysClockHSE_24MHz();

/**
 * @brief   Set SYSCLK to 24 MHz via PLLCLK and HSI.
 *
 * @details Sets SYSCLK = (HSI/2)*6, HCLK = SYSCLK/1, PCLK1 = HCLK/1, PCLK2 = HCLK/1, ADCPRE and
 *          adjusts FLASH parameters.
 *
 * @note    Updates g_clock
 */
void clock_setSysClockHSI_24MHz();

/**
 * @brief   Set SYSCLK to 48 MHz via PLLCLK and HSE.
 *
 * @details Sets SYSCLK = HSE*6, HCLK = SYSCLK/1, PCLK1 = HCLK/2, PCLK2 = HCLK/1, ADCPRE, and
 *          adjusts FLASH parameters.
 *
 * @note    Updates g_clock
 */
void clock_setSysClockHSE_48MHz();

/**
 * @brief   Set SYSCLK to 48 MHz via PLLCLK and HSI.
 *
 * @details Sets SYSCLK = (HSI/2)*12, HCLK = SYSCLK/1, PCLK1 = HCLK/2, PCLK2 = HCLK/1, ADCPRE and
 *          adjusts FLASH parameters.
 *
 * @note    Updates g_clock
 */
void clock_setSysClockHSI_48MHz();

/**
 * @brief   Set SYSCLK to HSI
 *
 * @details Sets HCLK = SYSCLK/1, PCLK2 = HCLK/1, PCLK1 = HCLK/1, ADCPRE, and adjusts
 *          FLASH parameters.
 *
 * @note    Updates g_clock
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
