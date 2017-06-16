#ifndef _UTILS_
#define _UTILS_

#include "types.h"
#include "vfunc.h"

#if defined(__GNUC__) && defined(__STRICT_ANSI__)
#define asm __asm__
#endif

#define NOP()  do { asm("NOP\n"); } while (0)
#define BKPT() do { asm("BKPT\n"); } while (0)

static inline void __enable_irq()        { asm("cpsie i"); }
static inline void __disable_irq()       { asm("cpsid i"); }

static inline void __enable_fault_irq()  { asm("cpsie f"); }
static inline void __disable_fault_irq() { asm("cpsid f"); }

static inline void __NOP()               { asm("nop"); }
static inline void __WFI()               { asm("wfi"); }
static inline void __WFE()               { asm("wfe"); }
static inline void __SEV()               { asm("sev"); }
static inline void __ISB()               { asm("isb"); }
static inline void __DSB()               { asm("dsb"); }
static inline void __DMB()               { asm("dmb"); }
static inline void __CLREX()             { asm("clrex"); }

extern uint32_t __get_PRIMASK();
extern void  __set_PRIMASK(uint32_t primask);

/**
 * @brief Re-entrant IRQ lock functions
 */
typedef uint32_t irq_lock_t;
#define LOCK_IRQ(lock) do { (lock) = __get_PRIMASK(); __disable_irq(); } while (0)
#define UNLOCK_IRQ(lock) do { __set_PRIMASK((lock)); } while (0)


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
