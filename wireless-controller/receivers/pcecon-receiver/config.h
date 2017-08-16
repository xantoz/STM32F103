#ifndef _SNESCON_RECEIVER_CONFIG_
#define _SNESCON_RECEIVER_CONFIG_

#include "common/config.h"

/* **** Settings for the SNES client **** */

#define pceCon_IRQHandler EXTI9_5_IRQHandler
#define pceCon_IRQn       EXTI9_5_IRQn

struct pceCon_client controller =
{
    .mode = pceCon_2BTN,
    .pin = {
        .enable   = {&GPIOB, 9},
        .select   = {&GPIOB, 8},
        .output1Y = {&GPIOB, 7},
        .output2Y = {&GPIOB, 6},
        .output3Y = {&GPIOB, 5},
        .output4Y = {&GPIOB, 4},
    }
};

#endif
