/**
 * @file  snesCon.h
 *
 * @brief Common SNES controller definitions.
 */

#ifndef _SNESCON_
#define _SNESCON_

#include "lib/types.h"

/**
 * @brief An unsigned integer large enough to store the state of all buttons on an SNES controller.
 */
typedef uint16_t snesCon_btn_t;

/**
 * @brief Button definitions for SNES controller.
 */
enum snesCon_buttons
{
    snesCon_BUTTON_B_Pos = 0,
    snesCon_BUTTON_Y_Pos = 1,
    snesCon_BUTTON_Select_Pos = 2,
    snesCon_BUTTON_Start_Pos = 3,
    snesCon_BUTTON_Up_Pos = 4,
    snesCon_BUTTON_Down_Pos = 5,
    snesCon_BUTTON_Left_Pos = 6,
    snesCon_BUTTON_Right_Pos = 7,
    snesCon_BUTTON_A_Pos = 8,
    snesCon_BUTTON_X_Pos = 9,
    snesCon_BUTTON_L_Pos = 10,
    snesCon_BUTTON_R_Pos = 11,
    
    snesCon_BUTTON_B_Msk      = (1 << snesCon_BUTTON_B_Pos),
    snesCon_BUTTON_Y_Msk      = (1 << snesCon_BUTTON_Y_Pos),
    snesCon_BUTTON_Select_Msk = (1 << snesCon_BUTTON_Select_Pos),
    snesCon_BUTTON_Start_Msk  = (1 << snesCon_BUTTON_Start_Pos),
    snesCon_BUTTON_Up_Msk     = (1 << snesCon_BUTTON_Up_Pos),
    snesCon_BUTTON_Down_Msk   = (1 << snesCon_BUTTON_Down_Pos),
    snesCon_BUTTON_Left_Msk   = (1 << snesCon_BUTTON_Left_Pos),
    snesCon_BUTTON_Right_Msk  = (1 << snesCon_BUTTON_Right_Pos),
    snesCon_BUTTON_A_Msk      = (1 << snesCon_BUTTON_A_Pos),
    snesCon_BUTTON_X_Msk      = (1 << snesCon_BUTTON_X_Pos),
    snesCon_BUTTON_L_Msk      = (1 << snesCon_BUTTON_L_Pos),
    snesCon_BUTTON_R_Msk      = (1 << snesCon_BUTTON_R_Pos)
};
    


#endif
