#include "snesCon_read.h"

#include "lib/utils.h"

bool snesCon_read_init(const snesCon_read_t * const def)
{
    GPIO_setMODE_setCNF(def->clock.port, def->clock.pin, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_setMODE_setCNF(def->latch.port, def->latch.pin, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_resetPin(&def->latch);
    GPIO_resetPin(&def->clock);

    GPIO_setMODE_setCNF(def->data.port, def->data.pin, GPIO_MODE_Input, GPIO_Input_CNF_PullupPulldown);
    GPIO_resetPin(&def->data); // Set pull-down

    return true;
}

/**
 * delay for 12 us
 *
 * TODO: Currently hardcoded for 8 MHz SYSCLK. Make snazzy variant that can work out how many NOPs
 * are needed, based on g_clock.sysclkFreq
 */
static void snesCon_delay()
{
    __disable_irq();
    for (volatile uint8_t i = 0; i < 12; ++i)
    {
        // 8 NOPs at 8 MHz == 1 us
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
    }
    __enable_irq();
}

/**
 * delay for 6 us
 *
 * TODO: Currently hardcoded for 8 MHz SYSCLK. Make snazzy variant that can work out how many NOPs
 * are needed, based on g_clock.sysclkFreq
 */
static void snesCon_halfDelay()
{
    __disable_irq();
    for (volatile uint8_t i = 0; i < 6; ++i)
    {
        // 8 NOPs at 8 MHz == 1 us
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
    }
    __enable_irq();
}

snesCon_btn_t snesCon_read_tick(const snesCon_read_t * const def)
{
    GPIO_setPin(&def->latch);
    snesCon_delay();
    GPIO_resetPin(&def->latch);

    snesCon_halfDelay();

    snesCon_btn_t btn = 0;
    for (uint8_t i = 0; i < 16; ++i)
    {
        GPIO_setPin(&def->clock);
        snesCon_delay();
        btn |= (!!GPIO_read(&def->data)) << i;
        GPIO_resetPin(&def->clock);
        snesCon_delay();
    }

    return btn;
}
