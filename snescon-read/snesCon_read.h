/**
 * @file snesCon_read.h
 *
 * @brief Routines for reading the state of a SNES controller connected to GPIO pins.
 */

#ifndef _SNESCON_READ_
#define _SNESCON_READ_

#include "snesCon.h"

#include "lib/types.h"

/**
 * @brief Initialize hardware for the pin&port definition in def.
 *
 * @note Does not enable any peripheral clocks, the user of this function is responsible for
 *       ensuring that relevant peripheral clocks have been enabled.
 *
 * @param def [in] Definition of GPIO ports & pins
 *
 * @return Status: true on success, false on failure
 */
bool snesCon_read_init(const struct snesCon_pins * const def);

/**
 * @brief Read controller state. Meant to be run periodically on a timer tick.
 *
 * @param def  [in]  Definition of GPIO ports & pins. snesCon_read_init must have been called for this
 *                   pin definition beforehand.
 *
 * @return SNES controller button state
 *
 */
snesCon_btn_t snesCon_read_tick(const struct snesCon_pins * const def);

#endif
