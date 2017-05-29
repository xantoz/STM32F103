#include "systick.h"

#include "clock.h"

bool systick_startSysTick_ms(const uint32_t ms)
{
    // SysTick RELOAD should be loaded with N-1 to fire every N clock pulses
    const uint32_t reload = (ms*g_clock.sysTickFreq)/1000 - 1;
    // TODO: fail when overflow?

    // Fail when reload == 0 or when the value will not fit into the 24-bit wide register
    if ((reload == 0) || (reload & 0xff000000))
        return false;

    SysTick->CTRL &= ~(SysTick_CTRL_TICKINT | SysTick_CTRL_ENABLE);
    SysTick->LOAD = reload;
    SysTick->VAL = 0;
    SysTick->CTRL &= ~(SysTick_CTRL_COUNTFLAG);
    SysTick->CTRL |= SysTick_CTRL_TICKINT | SysTick_CTRL_ENABLE;

    return true;
}
