#ifndef _SNESCON_RECEIVER_CONFIG_
#define _SNESCON_RECEIVER_CONFIG_

#include "common/config.h"

/* **** Settings for the PCE client **** */

#define pceCon_IRQHandler EXTI9_5_IRQHandler
#define pceCon_IRQn       EXTI9_5_IRQn

#define ENABLE_PortPin ((struct GPIO_PortPin){&GPIOB, 9})
#define SELECT_PortPin ((struct GPIO_PortPin){&GPIOB, 8})

#define OUTPUT_Port  GPIOB
#define OUTPUT1Y_Pin 7
#define OUTPUT2Y_Pin 6
#define OUTPUT3Y_Pin 5
#define OUTPUT4Y_Pin 4
#define OUTPUT_Msk   0xff0f
#define OUTPUT_Pos   4
static const uint8_t c_outputPins[] = {OUTPUT1Y_Pin, OUTPUT2Y_Pin, OUTPUT3Y_Pin, OUTPUT4Y_Pin};

#define SIXBUTTON 1

#define DEBUG_ENABLE_INTERRUPT 0
#define DEBUG_PortPin ((struct GPIO_PortPin){&GPIOA, 8})

#endif
