#ifndef _SNESCON_RECEIVER_CONFIG_
#define _SNESCON_RECEIVER_CONFIG_

#include "common/config.h"

/* **** Settings for the SNES client **** */

#define snesCon_IRQHandler EXTI9_5_IRQHandler
#define snesCon_IRQn       EXTI9_5_IRQn
#define snesCon_PINS \
    ((struct snesCon_pins){ .clock = {&GPIOB, 7}, .latch = {&GPIOB, 8}, .data  = {&GPIOB, 9} })

#endif
