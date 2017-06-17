#include "clock.h"

#include "utils.h"
#include "rcc.h"
#include "systick.h"
#include "flash.h"
#include "debug.h"

// TODO: Functions for setting ADC clock manually
// TODO: early exits if we detect that settings are what we are going to set them to

struct clocks g_clock = {
    // Default values on bootup, if the clock speeds haven't been changed
    .sysclkFreq = CLOCK_HSI_Hz,         // SYSCLK == HSI == 8 MHz on bootup
    .hclkFreq = CLOCK_HSI_Hz,           // HCLK == SYSCLK == HSI by default
    .pclk2Freq = CLOCK_HSI_Hz,          // PCLK2 == HCLK == HSI by default
    .pclk1Freq = CLOCK_HSI_Hz,          // PCLK1 == HCLK == HSI by default
    .sysTickFreq = CLOCK_HSI_Hz/8,      // SysTick FREQ == SYSCLK/8 == HSI/8 by default
    .timerFreq = CLOCK_HSI_Hz,          // TIMxCLK == PCLK2*1 if APB2 prescaler == 1 else TIMxCLK == PCLK2*2
    .adcClkFreq = CLOCK_HSI_Hz/2,       // ADCCLK == PCLK2/2 == HSI/2 by default
};

static const uint32_t MHz = 1000000; // Convert from Hz to MHz

/// Macro to check at compile time that frequency is not out of range
// TODO: Backwards compatible definition so we compile without C11 ?
#define CASSERT_SYSCLKFREQ(f) \
    _Static_assert(0 < (f) && (f) < SYSCLK_MAX, "SYSCLK frequency out of range")

/// Macro to check at compile time that frequency is not out of range
// TODO: Backwards compatible definition so we compile without C11 ?
#define CASSERT_PCLK2FREQ(f) \
    _Static_assert(0 < (f) && (f) < PCLK2_MAX, "PCLK2 frequency out of range")

// Helper to get FLASH_ACR_LATENCY, based on what we are setting SYSCLK to
#define GET_FLASH_ACR_LATENCY(f)                                                         \
    ({                                                                                   \
        CASSERT_SYSCLKFREQ(f);                                                           \
        const uint32_t _f = (f);                                                         \
        (_f <= 24*MHz)                ? FLASH_ACR_LATENCY_0 : /* f <= 24 MHz */          \
        (24*MHz < _f && _f <= 48*MHz) ? FLASH_ACR_LATENCY_1 : /* 24 MHz < f <= 48 MHz */ \
        (48*MHz < _f && _f <= 72*MHz) ? FLASH_ACR_LATENCY_2 : /* 48 MHz < f <= 72 MHz */ \
                                        0b111;                /* undefined */            \
    })

// Helper to get a suitable ADCPRE given PCLK2 freq, so that ADCCLK is not running at more than 14 MHz
#define GET_ADCPRE(pclk2Freq)                                           \
    ({                                                                  \
        CASSERT_PCLK2FREQ(pclk2Freq);                                   \
        const uint32_t _pclk2Freq = (pclk2Freq);                        \
        const uint32_t _flag =                                          \
            (_pclk2Freq > (6*14)*MHz) ? RCC_CFGR_ADCPRE_PCLK2_Div8 :    \
            (_pclk2Freq > (4*14)*MHz) ? RCC_CFGR_ADCPRE_PCLK2_Div6 :    \
            (_pclk2Freq > (2*14)*MHz) ? RCC_CFGR_ADCPRE_PCLK2_Div4 :    \
                                        RCC_CFGR_ADCPRE_PCLK2_Div2;     \
        _flag;                                                          \
    })

// Returns the frequency of PLLCLK in Hz (by reading back register settings)
static uint32_t getPLLFreq(uint32_t cfgr)
{
    uint32_t pllsrc = cfgr & RCC_CFGR_PLLSRC;
    uint32_t pllsrc_freq;
    if (pllsrc == RCC_CFGR_PLLSRC_HSE)
    {
        uint32_t pllxtpre = cfgr & RCC_CFGR_PLLXTPRE;
        pllsrc_freq = (pllxtpre == RCC_CFGR_PLLXTPRE_HSE_Div2) ? CLOCK_HSE_Hz/2 : CLOCK_HSE_Hz;
    }
    else /* (pllsrc == RCC_CFGR_PLLSRC_HSI_Div2) */
    {
        pllsrc_freq = CLOCK_HSI_Hz/2;
    }

    // Use the property that the multiplication is the integer value of the PLLMUL field + 2 (with
    // the exception of 0b1111, which is an extra way to PLLMUL16)
    uint32_t pllmul = cfgr & RCC_CFGR_PLLMUL_Msk;
    uint32_t pllmul_multiplicand = (pllmul == RCC_CFGR_PLLMUL16_ALT) ? 16 : (pllmul >> RCC_CFGR_PLLMUL_Pos) + 2;

    return pllsrc_freq * pllmul_multiplicand;
}

// Updates the values in g_clock by reading back register settings
static void updateClockFreqs()
{
    uint32_t cfgr = RCC.CFGR; // Take a snapshot of the register, get rid of volatile accesses

    // Get SYSCLK frequency
    uint32_t sws = cfgr & RCC_CFGR_SWS;
    g_clock.sysclkFreq =
        (sws == RCC_CFGR_SWS_HSI) ? CLOCK_HSI_Hz :
        (sws == RCC_CFGR_SWS_HSE) ? CLOCK_HSE_Hz :
        (sws == RCC_CFGR_SWS_PLL) ? getPLLFreq(cfgr) : 0;
    assert(g_clock.sysclkFreq == 0, "read undefined value for RCC_CFGR SWS");

    // Get HCLK (AHB) frequency
    uint32_t hpre = cfgr & RCC_CFGR_HPRE;
    uint32_t hpre_div =
        (hpre == RCC_CFGR_HPRE_SYSCLK_Div1)   ? 1 :
        (hpre == RCC_CFGR_HPRE_SYSCLK_Div2)   ? 2 :
        (hpre == RCC_CFGR_HPRE_SYSCLK_Div4)   ? 4 :
        (hpre == RCC_CFGR_HPRE_SYSCLK_Div8)   ? 8 :
        (hpre == RCC_CFGR_HPRE_SYSCLK_Div16)  ? 16 :
        (hpre == RCC_CFGR_HPRE_SYSCLK_Div64)  ? 64 :
        (hpre == RCC_CFGR_HPRE_SYSCLK_Div128) ? 128 :
        (hpre == RCC_CFGR_HPRE_SYSCLK_Div256) ? 256:
        (hpre == RCC_CFGR_HPRE_SYSCLK_Div512) ? 512 : 0;
    assert(hpre_div == 0, "ERROR: Invalid HPRE setting in RCC_CFGR");
    g_clock.hclkFreq = g_clock.sysclkFreq/hpre_div;

    // Get PCLK2 (APB2) frequency
    uint32_t ppre2 = cfgr & RCC_CFGR_PPRE2;
    uint32_t ppre2_div =
        (ppre2 == RCC_CFGR_PPRE2_HCLK_Div1)  ? 1 :
        (ppre2 == RCC_CFGR_PPRE2_HCLK_Div2)  ? 2 :
        (ppre2 == RCC_CFGR_PPRE2_HCLK_Div4)  ? 4 :
        (ppre2 == RCC_CFGR_PPRE2_HCLK_Div8)  ? 8 :
        (ppre2 == RCC_CFGR_PPRE2_HCLK_Div16) ? 16 : 0;
    assert(ppre2_div == 0, "ERROR: Invalid PPRE2 setting in RCC_CFGR");

    g_clock.pclk2Freq = g_clock.hclkFreq/ppre2_div;
    // Set timer frequency (depends on ppre2)
    g_clock.timerFreq = (ppre2_div == 1) ? g_clock.pclk2Freq : g_clock.pclk2Freq*2;

    // Get PCLK1 (APB1) frequency
    uint32_t ppre1 = cfgr & RCC_CFGR_PPRE1;
    uint32_t ppre1_div =
        (ppre1 == RCC_CFGR_PPRE1_HCLK_Div1)  ? 1 :
        (ppre1 == RCC_CFGR_PPRE1_HCLK_Div2)  ? 2 :
        (ppre1 == RCC_CFGR_PPRE1_HCLK_Div4)  ? 4 :
        (ppre1 == RCC_CFGR_PPRE1_HCLK_Div8)  ? 8 :
        (ppre1 == RCC_CFGR_PPRE1_HCLK_Div16) ? 16 : 0;
    assert(ppre1_div == 0, "ERROR: Invalid PPRE1 setting in RCC_CFGR");
    g_clock.pclk1Freq = g_clock.hclkFreq/ppre1_div;

    // Get ADCCLK frequency
    uint32_t adcpre = cfgr & RCC_CFGR_ADCPRE;
    uint32_t adcClk_div =  // The ADCPRE bits are exhaustive, so no need to check for PCLK2_Div2
        (adcpre == RCC_CFGR_ADCPRE_PCLK2_Div8) ? 8 :
        (adcpre == RCC_CFGR_ADCPRE_PCLK2_Div6) ? 6 :
        (adcpre == RCC_CFGR_ADCPRE_PCLK2_Div4) ? 4 : 2;
    g_clock.adcClkFreq = g_clock.pclk2Freq/adcClk_div;
    assert(g_clock.adcClkFreq > ADCCLK_MAX, "ERROR: ADCCLK > 14 MHz");

    // Get SysTick frequency
    uint32_t systick_clksource = SysTick.CTRL & SysTick_CTRL_CLKSOURCE;
    g_clock.sysTickFreq = g_clock.hclkFreq;
    if (systick_clksource == SysTick_CTRL_CLKSOURCE_HCLK_Div8)
        g_clock.sysTickFreq /= 8;
}

// Helper function to turn on HSE clock, and wait for it to start up
static void startHSE()
{
    static const uint16_t HSE_STARTUP_TIMEOUT = 0x0500; //!< Time out for HSE start up

    RCC.CR |= RCC_CR_HSEON;
    volatile uint16_t StartUpCounter = 0;
    volatile uint32_t HSEStatus = 0;
    do
    {
        HSEStatus = RCC.CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if (StartUpCounter >= HSE_STARTUP_TIMEOUT || !(RCC.CR & RCC_CR_HSERDY))
        die("HSE WILL NOT COME UP");
}

// Helper function to turn on HSI clock, and wait for it to start up
static void startHSI()
{
    static const uint16_t HSI_STARTUP_TIMEOUT = 0x0500;  //!< Time out for HSE start up

    RCC.CR |= RCC_CR_HSION;
    volatile uint16_t StartUpCounter = 0;
    volatile uint32_t HSIStatus = 0;
    do
    {
        HSIStatus = RCC.CR & RCC_CR_HSIRDY;
        StartUpCounter++;
    } while ((HSIStatus == 0) && (StartUpCounter != HSI_STARTUP_TIMEOUT));

    if (StartUpCounter >= HSI_STARTUP_TIMEOUT || !(RCC.CR & RCC_CR_HSIRDY))
        die("HSI WILL NOT COME UP");
}

void clock_setSysClockHSE()
{
    CASSERT_SYSCLKFREQ(CLOCK_HSE_Hz);

    irq_lock_t lock;
    LOCK_IRQ(lock);

    startHSE();

    FLASH.ACR |= FLASH_ACR_PRFTBE; // Enable prefetch buffer
    FLASH.ACR &= ~FLASH_ACR_LATENCY;
    FLASH.ACR |= GET_FLASH_ACR_LATENCY(CLOCK_HSE_Hz);

    RCC.CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1);
    RCC.CFGR |= RCC_CFGR_HPRE_SYSCLK_Div1;     /* HCLK = SYSCLK */
    RCC.CFGR |= RCC_CFGR_PPRE2_HCLK_Div1;      /* PCLK2 = HCLK */
    RCC.CFGR |= RCC_CFGR_PPRE1_HCLK_Div1;      /* PCLK1 = HCLK */

    // Set ADCPRE to suitable setting, so that it is not more than 14 MHz
    RCC.CFGR &= ~(RCC_CFGR_ADCPRE);
    RCC.CFGR |= GET_ADCPRE(CLOCK_HSE_Hz); // Get by PCLK2 frequency

    // Select HSE as system clock source
    RCC.CFGR &= ~RCC_CFGR_SW;
    RCC.CFGR |= RCC_CFGR_SW_HSE;

    // Wait till HSE is used as system clock source
    while ((RCC.CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);

    // Update the clock global variables to reflect changes
    updateClockFreqs();

    UNLOCK_IRQ(lock);
}

void clock_setSysClockHSE_24MHz()
{
    CASSERT_SYSCLKFREQ((CLOCK_HSE_Hz/2)*6);

    irq_lock_t lock;
    LOCK_IRQ(lock);

    startHSE();

    FLASH.ACR |= FLASH_ACR_PRFTBE;    // Enable prefetch buffer
    FLASH.ACR &= ~FLASH_ACR_LATENCY;
    FLASH.ACR |= GET_FLASH_ACR_LATENCY((CLOCK_HSE_Hz/2)*6);

    RCC.CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1);
    RCC.CFGR |= RCC_CFGR_HPRE_SYSCLK_Div1;     /* HCLK = SYSCLK */
    RCC.CFGR |= RCC_CFGR_PPRE2_HCLK_Div1;      /* PCLK2 = HCLK */
    RCC.CFGR |= RCC_CFGR_PPRE1_HCLK_Div1;      /* PCLK1 = HCLK */

    // Set ADCPRE to suitable setting, so that it is not more than 14 MHz
    RCC.CFGR &= ~(RCC_CFGR_ADCPRE);
    RCC.CFGR |= GET_ADCPRE(((CLOCK_HSE_Hz/2)*6)/1/1); // Get by PCLK2 frequency

    // PLL configuration: (HSE / 2) * 6 = 24 MHz
    RCC.CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMUL);
    RCC.CFGR |= (RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLXTPRE_HSE_Div2 | RCC_CFGR_PLLMUL6);

    // Enable PLL
    RCC.CR |= RCC_CR_PLLON;

    // Wait till PLL is ready
    while ((RCC.CR & RCC_CR_PLLRDY) == 0);

    // Select PLL as system clock source
    RCC.CFGR &= ~(RCC_CFGR_SW);
    RCC.CFGR |= RCC_CFGR_SW_PLL;

    // Wait till PLL is used as system clock source
    while ((RCC.CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    // Update the clock global variables to reflect changes
    updateClockFreqs();

    UNLOCK_IRQ(lock);
}

void clock_setSysClockHSI_24MHz()
{
    CASSERT_SYSCLKFREQ((CLOCK_HSI_Hz/2)*6);

    irq_lock_t lock;
    LOCK_IRQ(lock);

    startHSI();

    FLASH.ACR |= FLASH_ACR_PRFTBE;    // Enable prefetch buffer
    FLASH.ACR &= ~FLASH_ACR_LATENCY;  // Flash 0 wait state
    FLASH.ACR |= GET_FLASH_ACR_LATENCY((CLOCK_HSI_Hz/2)*6);

    RCC.CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1);
    RCC.CFGR |= RCC_CFGR_HPRE_SYSCLK_Div1;     /* HCLK = SYSCLK */
    RCC.CFGR |= RCC_CFGR_PPRE2_HCLK_Div1;      /* PCLK2 = HCLK */
    RCC.CFGR |= RCC_CFGR_PPRE1_HCLK_Div1;      /* PCLK1 = HCLK */

    // Set ADCPRE to suitable setting, so that it is not more than 14 MHz
    RCC.CFGR &= ~(RCC_CFGR_ADCPRE);
    RCC.CFGR |= GET_ADCPRE(((CLOCK_HSI_Hz/2)*6)/1/1); // Get by PCLK2 frequency

    // PLL configuration: (HSI / 2) * 6 = 24 MHz
    RCC.CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL);
    RCC.CFGR |= (RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMUL6);

    // Enable PLL
    RCC.CR |= RCC_CR_PLLON;

    // Wait till PLL is ready
    while ((RCC.CR & RCC_CR_PLLRDY) == 0);

    // Select PLL as system clock source
    RCC.CFGR &= ~(RCC_CFGR_SW);
    RCC.CFGR |= RCC_CFGR_SW_PLL;

    // Wait till PLL is used as system clock source
    while ((RCC.CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    // Update the clock global variables to reflect changes
    updateClockFreqs();

    UNLOCK_IRQ(lock);
}

void clock_setSysClockHSI()
{
    CASSERT_SYSCLKFREQ(CLOCK_HSI_Hz);

    irq_lock_t lock;
    LOCK_IRQ(lock);

    startHSI();

    FLASH.ACR |= FLASH_ACR_PRFTBE; // Enable flash prefetch buffer
    FLASH.ACR &= ~FLASH_ACR_LATENCY;
    FLASH.ACR |= GET_FLASH_ACR_LATENCY(CLOCK_HSI_Hz);

    // HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK
    RCC.CFGR &= (RCC.CFGR & ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1)) | (RCC_CFGR_HPRE_SYSCLK_Div1 | RCC_CFGR_PPRE2_HCLK_Div1 | RCC_CFGR_PPRE1_HCLK_Div1);

    // Set ADCPRE to suitable setting, so that it is not more than 14 MHz
    RCC.CFGR &= ~(RCC_CFGR_ADCPRE);
    RCC.CFGR |= GET_ADCPRE(CLOCK_HSI_Hz); // Get by PCLK2 frequency

    // Select HSI as system clock source
    RCC.CFGR = (RCC.CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_HSI;

    // Wait till HSI is used as system clock source
    while ((RCC.CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);

    // Update the clock global variables to reflect changes
    updateClockFreqs();

    UNLOCK_IRQ(lock);
}

void clock_setSysTick_HCLK()
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    SysTick.CTRL &= ~SysTick_CTRL_CLKSOURCE;
    SysTick.CTRL |= SysTick_CTRL_CLKSOURCE_HCLK;

    // Just update g_clock directly, since we haven't changed RCC_CFGR
    g_clock.sysTickFreq = g_clock.hclkFreq;

    UNLOCK_IRQ(lock);
}

void clock_setSysTick_HCLK_Div8()
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    SysTick.CTRL &= ~SysTick_CTRL_CLKSOURCE;
    SysTick.CTRL |= SysTick_CTRL_CLKSOURCE_HCLK_Div8;

    // Just update g_clock directly, since we haven't changed RCC_CFGR
    g_clock.sysTickFreq = g_clock.hclkFreq/8;

    UNLOCK_IRQ(lock);
}
