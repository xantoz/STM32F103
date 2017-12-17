#include "dip.h"

#include "config.h"

#include <gpio.h>

void dip_init()
{
    for (unsigned pin = DIP_Pos; pin < (DIP_Pos + 4); ++pin)
    {
        GPIO_setMODE_setCNF(&DIP_Port, pin, GPIO_MODE_Input, GPIO_Input_CNF_PullupPulldown);
        GPIO_setPin(&DIP_Port, pin);  // Set pull-up
    }
}

uint8_t dip_read()
{
    return (DIP_Port.IDR & DIP_Msk) >> DIP_Pos;
}
