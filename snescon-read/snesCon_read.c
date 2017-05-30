#include "snesCon_read.h"

#include "lib/utils.h"

bool snesCon_read_init(const snesCon_read_t * const def)
{
    GPIO_setMODE_setCNF(def->clock.port, def->clock.pin, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_setMODE_setCNF(def->latch.port, def->latch.pin, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    
    GPIO_setMODE_setCNF(def->data.port, def->data.pin, GPIO_MODE_Input, GPIO_Input_CNF_PullupPulldown);

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
}

void snesCon_read_tick(const snesCon_read_t * const def, snesCon_btn_t * const buttonState)
{
    GPIO_setPin(&def->latch);
    snesCon_delay();
    GPIO_resetPin(&def->latch);
    
}
