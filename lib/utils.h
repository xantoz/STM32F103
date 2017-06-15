#ifndef _UTILS_
#define _UTILS_

#define NOP()  do { __asm volatile ("NOP\n"); } while (0)
#define BKPT() do { __asm volatile ("BKPT\n"); } while (0)

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
#include "vfunc.h"

/**
 * @brief NOP-based/cycle-counting based delay routine. Works regardless of current SYSCLK
 *        frequency, by virtue of using g_clock.sysclkFreq for scaling (see clock.h).
 *
 * @note The lower the SYSCLK frequency is, the longer the minimum possible delay becomes. The
 *       overhead, while accounted for, is at least 12 cycles long, which translates to a minimum
 *       delay of 1.5 us at 8 MHz SYSCLK. So do not expect this to be accurate for small values of
 *       us relative to SYSCLK, or very accurate overall. However it should be usable enough when
 *       precise timings are not needed. It becomes more accurate for larger SYSCLK as more time is
 *       spent inside the tight loop, relative to on the overhead.
 *
 * @note If you do not wish to have the delay prolonged by interrupts, make sure to disable them
 *       before calling.
 *
 * @param us [in]   Delay in microseconds
 */
extern void delay_us(int32_t us);

#define BITBAND_SRAM_REF 0x20000000
#define BITBAND_SRAM_BASE 0x22000000
#define BITBAND_SRAM(a,b) ((bool*)(BITBAND_SRAM_BASE + ((a)-BITBAND_SRAM_REF)*32 + ((b)*4))) // Convert SRAM address
#define BITBAND_PERI_REF 0x40000000
#define BITBAND_PERI_BASE 0x42000000
#define BITBAND_PERI(a,b) ((bool*)(BITBAND_PERI_BASE + ((a)-BITBAND_PERI_REF)*32 + ((b)*4))) // Convert PERI address

#define BB_MAGIC(a,b)                               \
    ({                                              \
        const uint8_t *__a  = (uint8_t*)(a);        \
        const uint8_t *ref  = __a & 0x60000000;     \
        const uint8_t *base = ref | 0x02000000;     \
        (bool*)(base + (__a - ref)*32 + ((b)*4));   \
    })

#define BITSET(x) ()
#define BITRESET(x) ()

#define DIV_ROUND_CLOSEST(n, d) \
    ((((n) < 0) ^ ((d) < 0)) ? (((n) - (d)/2)/(d)) : (((n) + (d)/2)/(d)))

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

// Usage examples: SET(SPI1, SPI_CR1, BR, PCLK_Div64)
//                 SET(SPI1, SPI_CR1, LSBFIRST)
//                 RESET(SPI1, SPI_CR1, LSBFIRST)
#define __SET4(periph_instance, periph_reg, field, value) ((periph_instance).reg = ((periph_instance).reg & ~(periph_reg##_##field)) | periph_reg##_##field##_##value)
#define __SET3(periph_instance, periph_reg, field) ((periph_instance).reg |= (periph_reg##_##field))
#define SET(...) VFUNC(__SET, __VA_ARGS__)

#define RESET(periph_instance, periph_reg, field) ((periph_instance).reg &= ~(periph_reg##_##field))

#endif
