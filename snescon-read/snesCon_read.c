#include "snesCon_read.h"

#include "lib/utils.h"

bool snesCon_read_init(const snesCon_read_t * const def)
{
    GPIO_setMODE_setCNF(def->clock.port, def->clock.pin, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_setMODE_setCNF(def->latch.port, def->latch.pin, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_resetPin(&def->latch);
    GPIO_setPin(&def->clock);                               // Clock is active low

    GPIO_setMODE_setCNF(def->data.port, def->data.pin, GPIO_MODE_Input, GPIO_Input_CNF_PullupPulldown);
    GPIO_resetPin(&def->data); // Set pull-down

    return true;
}

snesCon_btn_t snesCon_read_tick(const snesCon_read_t * const def)
{
    GPIO_setPin(&def->latch);
    delay_us(12);
    GPIO_resetPin(&def->latch);

    delay_us(6);

    // The zeroth button (B) is driven already on falling latch
    // The rest on rising clock
    snesCon_btn_t btn = 0;
    for (uint8_t i = 0; i < 16; ++i)
    {
        GPIO_resetPin(&def->clock);
        delay_us(12);
        btn |= (!GPIO_read(&def->data)) << i;               // Button pressed when low
        GPIO_setPin(&def->clock);
        delay_us(12);
    }

    return btn;
}
