/**
 * @file  pceCon.h
 *
 * @brief Common PCE controller definitions.
 */

#ifndef _PCECON_
#define _PCECON_

#include "types.h"
#include "gpio.h"

#include "common.h"

/**
 * @brief An unsigned integer large enough to store the state of all buttons on a PCE controller
 *        (6-button pads included)
 */
typedef btn_t pceCon_btn_t;

/**
 * @brief Button definitions for PCE controller.
 *
 * @note  Since the PCE protocol is not a serial protocol this does not have any correlation
 *        with the internal order of buttons or anything like that. Instead this is defined so
 *        that we get a convenient mapping of PCE controller and (S)NES controller.
 */
enum pceCon_buttons
{
    pceCon_BUTTON_II_Pos = 0,
    pceCon_BUTTON_I_Pos = 1,
    pceCon_BUTTON_Select_Pos = 2,
    pceCon_BUTTON_Start_Pos = 3,
    pceCon_BUTTON_Up_Pos = 4,
    pceCon_BUTTON_Down_Pos = 5,
    pceCon_BUTTON_Left_Pos = 6,
    pceCon_BUTTON_Right_Pos = 7,
    pceCon_BUTTON_III_Pos = 8,
    pceCon_BUTTON_IV_Pos = 9,
    pceCon_BUTTON_V_Pos = 10,
    pceCon_BUTTON_VI_Pos = 11,

    pceCon_BUTTON_NONE   = 0,
    pceCon_BUTTON_II     = (1 << pceCon_BUTTON_II_Pos),
    pceCon_BUTTON_I      = (1 << pceCon_BUTTON_I_Pos),
    pceCon_BUTTON_Select = (1 << pceCon_BUTTON_Select_Pos),
    pceCon_BUTTON_Start  = (1 << pceCon_BUTTON_Start_Pos),
    pceCon_BUTTON_Up     = (1 << pceCon_BUTTON_Up_Pos),
    pceCon_BUTTON_Down   = (1 << pceCon_BUTTON_Down_Pos),
    pceCon_BUTTON_Left   = (1 << pceCon_BUTTON_Left_Pos),
    pceCon_BUTTON_Right  = (1 << pceCon_BUTTON_Right_Pos),
    pceCon_BUTTON_III    = (1 << pceCon_BUTTON_III_Pos),
    pceCon_BUTTON_IV     = (1 << pceCon_BUTTON_IV_Pos),
    pceCon_BUTTON_V      = (1 << pceCon_BUTTON_V_Pos),
    pceCon_BUTTON_VI     = (1 << pceCon_BUTTON_VI_Pos)
};

#endif
