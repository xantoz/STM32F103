#ifndef _UTILS_
#define _UTILS_

#define NOP()  { __asm volatile ("NOP\n"); }
#define BKPT() { __asm volatile ("BKPT\n"); }

static inline void __enable_irq()               { __asm volatile ("cpsie i"); }
static inline void __disable_irq()              { __asm volatile ("cpsid i"); }

static inline void __enable_fault_irq()         { __asm volatile ("cpsie f"); }
static inline void __disable_fault_irq()        { __asm volatile ("cpsid f"); }

static inline void __NOP()                      { __asm volatile ("nop"); }
static inline void __WFI()                      { __asm volatile ("wfi"); }
static inline void __WFE()                      { __asm volatile ("wfe"); }
static inline void __SEV()                      { __asm volatile ("sev"); }
static inline void __ISB()                      { __asm volatile ("isb"); }
static inline void __DSB()                      { __asm volatile ("dsb"); }
static inline void __DMB()                      { __asm volatile ("dmb"); }
static inline void __CLREX()                    { __asm volatile ("clrex"); }

#include "types.h"

/**
 * @brief NOP-based/cycle-counting based delay routine. Works regardless of current SYSCLK
 *        frequency, by virtue of using g_clock.sysclkFreq for scaling (see clock.h).
 *
 * @note The lower the SYSCLK frequency is, the lower the minimum possible delay becomes. So do not
 *       expect it to be able to delay for as short as 1 us. The overhead is at least 16 cycles
 *       long, which translates to a minimum delay of 2 us at 8 MHz. So do not expect this to be
 *       accurate for small values of us relative to SYSCLK, or very accurate overall, but it should
 *       be usable enough for things where the exactness of the delay is not very important. It
 *       becomes more accurate for larger SYSCLK as more time is spent inside the tight loop.
 *
 * @note Interrupts are disabled while this function is executing
 *
 * @param us [in]   Delay in microseconds
 */
extern void delay_us(int32_t us);

/**
 * @brief Same as delay_us, but interruptible
 *
 * @param us [in]   Delay in microseconds
 */
extern void delay_us_int(int32_t us);

#define __DIV_ROUND_CLOSEST_INT(n, d) \
    ((((n) < 0) ^ ((d) < 0)) ? (((n) - (d)/2)/(d)) : (((n) + (d)/2)/(d)))
#define __DIV_ROUND_CLOSEST_UINT(n, d) (((n) - (d)/2)/(d))
#define _DIV_ROUND_CLOSEST_INT(n,d)                                     \
    _Generic((d),                                                       \
             unsigned char:  __DIV_ROUND_CLOSEST_INT((n),(d)),          \
             unsigned short: __DIV_ROUND_CLOSEST_INT((n),(d)),          \
             unsigned int:   __DIV_ROUND_CLOSEST_INT((n),(d)),          \
             unsigned long:  __DIV_ROUND_CLOSEST_INT((n),(d)),          \
             signed char:    __DIV_ROUND_CLOSEST_INT((n),(d)),          \
             signed int:     __DIV_ROUND_CLOSEST_INT((n),(d)),          \
             signed short:   __DIV_ROUND_CLOSEST_INT((n),(d)),          \
             signed long:    __DIV_ROUND_CLOSEST_INT((n),(d)))
#define _DIV_ROUND_CLOSEST_UINT(n,d)                                    \
    _Generic((d),                                                       \
             unsigned char:  __DIV_ROUND_CLOSEST_UINT((n),(d)),         \
             unsigned short: __DIV_ROUND_CLOSEST_UINT((n),(d)),         \
             unsigned int:   __DIV_ROUND_CLOSEST_UINT((n),(d)),         \
             unsigned long:  __DIV_ROUND_CLOSEST_UINT((n),(d)),         \
             signed char:    __DIV_ROUND_CLOSEST_INT((n),(d)),          \
             signed int:     __DIV_ROUND_CLOSEST_INT((n),(d)),          \
             signed short:   __DIV_ROUND_CLOSEST_INT((n),(d)),          \
             signed long:    __DIV_ROUND_CLOSEST_INT((n),(d)))
#define DIV_ROUND_CLOSEST(n,d)                                     \
    _Generic((n),                                                  \
             unsigned char:  _DIV_ROUND_CLOSEST_UINT((n),(d)),     \
             unsigned short: _DIV_ROUND_CLOSEST_UINT((n),(d)),     \
             unsigned int:   _DIV_ROUND_CLOSEST_UINT((n),(d)),     \
             unsigned long:  _DIV_ROUND_CLOSEST_UINT((n),(d)),     \
             signed char:    _DIV_ROUND_CLOSEST_INT((n),(d)),      \
             signed short:   _DIV_ROUND_CLOSEST_INT((n),(d)),      \
             signed int:     _DIV_ROUND_CLOSEST_INT((n),(d)),      \
             signed long:    _DIV_ROUND_CLOSEST_INT((n),(d)))

#define SET(peripheral, reg, field, value) ((peripheral).(reg) = ((peripheral).(reg) & ~(peripheral##_##reg##_##field)) | peripheral##_##reg##_##field##_##value)

#endif
