#include "systick.h"

#include "clock.h"

static bool systick_startSysTick_impl(const uint32_t multiplicand, const uint32_t dividend)
{
    // SysTick RELOAD should be loaded with N-1 to fire every N clock pulses
    // Use 64-bit multiplication and division to decrease risk of overflowing in the multiplication
    const uint32_t reload = (uint32_t)(((uint64_t)multiplicand*g_clock.sysTickFreq)/dividend - 1); 

    // Fail when reload == 0 or when the value will not fit into the 24-bit wide register
    if ((reload == 0) || (reload & 0xff000000))
        return false;

    SysTick.CTRL &= ~(SysTick_CTRL_TICKINT | SysTick_CTRL_ENABLE);
    SysTick.LOAD = reload;
    SysTick.VAL = 0;
    SysTick.CTRL &= ~(SysTick_CTRL_COUNTFLAG);
    SysTick.CTRL |= SysTick_CTRL_TICKINT | SysTick_CTRL_ENABLE;

    return true;
}

bool systick_startSysTick_ms(const uint32_t ms)
{
    return systick_startSysTick_impl(ms, 1000);
}

bool systick_startSysTick_us(const uint32_t us)
{
    return systick_startSysTick_impl(us, 1000000);
}
