#ifndef _SNESCON_SENDER_CONFIG_
#define _SNESCON_SENDER_CONFIG_

#include "common/config.h"

/* **** Settings for the SNES controller **** */
#define POLLFREQ 1200

static const struct snesCon_pins snesCon_def = {
    .clock = {&GPIOB, 7},
    .latch = {&GPIOB, 8},
    .data  = {&GPIOB, 9},
};

#endif
