#include "debugLeds.h"

#include "utils.h"

static const struct GPIO_PortPin debugLeds[] = {
    {&GPIOB, 12},
    {&GPIOB, 13},
    {&GPIOB, 14},
    {&GPIOB, 15},

    {&GPIOA, 9},
    {&GPIOA, 10},
    {&GPIOA, 11},
    {&GPIOA, 12},

    {&GPIOB, 11},
    {&GPIOB, 10},
    {&GPIOB, 1},
    {&GPIOB, 0},
};

void debugLeds_init()
{
    for (unsigned i = 0; i < ARRAYLEN(debugLeds); ++i)
        GPIO_setMODE_setCNF(&debugLeds[i], GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
}

void debugLeds_update(snesCon_btn_t btn)
{
    for (unsigned i = 0; i < ARRAYLEN(debugLeds); ++i)
    {
        GPIO_setBit(&debugLeds[i], (btn & 0x1) == 0x1);
        btn >>= 1;
    }
}
