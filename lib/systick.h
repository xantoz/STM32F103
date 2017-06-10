#ifndef _SYSTICK_
#define _SYSTICK_

#include "types.h"
#include "memorymap.h"

/**
 * @brief memory mapped structure for SysTick
 */
typedef struct
{
    HW_RW CTRL;      //!< Offset: 0x00  SysTick Control and Status Register
    HW_RW LOAD;      //!< Offset: 0x04  SysTick Reload Value Register
    HW_RW VAL;       //!< Offset: 0x08  SysTick Current Value Register
    HW_WO CALIB;     //!< Offset: 0x0C  SysTick Calibration Register
} SysTick_Regs;

extern volatile SysTick_Regs SysTick;

/* SysTick Control / Status Register Definitions */
#define SysTick_CTRL_COUNTFLAG ((uint32_t)(1 << 16)) //!< Returns 1 if timer counted to 0 since last time this was read.

#define SysTick_CTRL_CLKSOURCE_Pos       ((uint32_t)2)
#define SysTick_CTRL_CLKSOURCE_Msk       ((uint32_t)(0b1 << SysTick_CTRL_CLKSOURCE_Pos))

/*
**  Clock source selection. If no reference clock is provided, it is held at 1
**  and so gives the same time as the core clock. The core clock must be at
**  least 2.5 times faster than the reference clock. If it is not, the count
**  values are Unpredictable.
*/
#define SysTick_CTRL_CLKSOURCE           ((uint32_t)(0b1 << 2))          //!< Clock source selection
#define SysTick_CTRL_CLKSOURCE_AHB_Div8  ((uint32_t)(0b0 << 2))          //!< AHB/8
#define SysTick_CTRL_CLKSOURCE_HCLK_Div8 SysTick_CTRL_CLKSOURCE_AHB_Div8 //!< HCLK/8 (AHB/8)
#define SysTick_CTRL_CLKSOURCE_AHB       ((uint32_t)(0b1 << 2))          //!< Processor clock (AHB)
#define SysTick_CTRL_CLKSOURCE_HCLK      SysTick_CTRL_CLKSOURCE_AHB      //!< HCLK (AHB)

#define SysTick_CTRL_TICKINT     ((uint32_t)(0b1 << 1)) //!< SysTick exception request enable
#define SysTick_CTRL_TICKINT_ON  ((uint32_t)(0b1 << 1)) //!< Counting down to zero does not assert the SysTick exception request
#define SysTick_CTRL_TICKINT_OFF ((uint32_t)(0b0 << 1)) //!< Counting down to zero to asserts the SysTick exception request.

//!< 1 = counter operates in a multi-shot way. That is, counter loads with the
//!Reload value and thenbegins counting down. On- reaching 0, it sets the
//!COUNTFLAG to 1 and optionally pends the SysTick handler, based on TICKINT. It
//!then loads the Reload value again, and begins counting. 0 = counter disabled.
#define SysTick_CTRL_ENABLE      ((uint32_t)(0b1))

/* SysTick Reload Register Definitions */
#define SysTick_LOAD_RELOAD_Pos             0                     //!< SysTick LOAD: RELOAD Position
#define SysTick_LOAD_RELOAD_Msk            ((uint32_t)0x00ffffff) //!< SysTick LOAD: RELOAD Mask

/* SysTick Current Register Definitions */
#define SysTick_VAL_CURRENT_Pos             0                     //!< SysTick VAL: CURRENT Position
#define SysTick_VAL_CURRENT_Msk            ((uint32_t)0x00ffffff) //!< SysTick VAL: CURRENT Mask

/* SysTick Calibration Register Definitions */
#define SysTick_CALIB_NOREF_Pos            31                                             /*!< SysTick CALIB: NOREF Position */
#define SysTick_CALIB_NOREF_Msk            (1ul << SysTick_CALIB_NOREF_Pos)               /*!< SysTick CALIB: NOREF Mask */

#define SysTick_CALIB_SKEW_Pos             30                                             /*!< SysTick CALIB: SKEW Position */
#define SysTick_CALIB_SKEW_Msk             (1ul << SysTick_CALIB_SKEW_Pos)                /*!< SysTick CALIB: SKEW Mask */

#define SysTick_CALIB_TENMS_Pos             0                                             /*!< SysTick CALIB: TENMS Position */
#define SysTick_CALIB_TENMS_Msk            (0xFFFFFFul << SysTick_VAL_CURRENT_Pos)        /*!< SysTick CALIB: TENMS Mask */

/**
 * @brief Set up systick reload register so that it counts to 0, and triggers
 *        the interrupt, every ms milliseconds.
 *
 * @return true on success, false on failure
 */
bool systick_startSysTick_ms(const uint32_t ms);

/**
 * @brief Set up systick reload register so that it counts to 0, and triggers
 *        the interrupt, every us microseconds.
 *
 * @return true on success, false on failure
 */
bool systick_startSysTick_us(const uint32_t us);


#endif /* _SYSTICK_ */
