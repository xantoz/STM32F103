/**
 * @file  debugLeds.h
 *
 * @brief Quick and dirty display of of SNES controller button states on GPIO pins
 */

#ifndef _WIRELESS_CONTROLLER_DEBUGLEDS_
#define _WIRELESS_CONTROLLER_DEBUGLEDS_

#include <protocol/snesCon.h>

void debugLeds_init();
void debugLeds_update(snesCon_btn_t btn);

#endif
