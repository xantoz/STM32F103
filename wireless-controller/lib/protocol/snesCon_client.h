/**
 * @file snesCon_client.h
 *
 * @brief Routines to implement the client side (the controller) of the SNES controller protocol. It
 *        is basically just a software implementation of a 16-bit shift register.
 */

#ifndef _SNESCON_CLIENT_
#define _SNESCON_CLIENT_

#include "types.h"
#include "gpio.h"

#include "snesCon.h"

struct snesCon_client
{
    snesCon_btn_t reg;
    snesCon_btn_t latchReg;
    struct snesCon_pins pinDef;
};

/**
 * @brief Initialize hardware for the pin&port definitions in snesCon_client.
 *
 * @note Does not enable any peripheral clocks, the user of this function is responsible for
 *       ensuring that relevant peripheral clocks have been enabled.
 *
 * @param client [in] Pointer to the struct snesCon_client that we want to init
 *
 * @return Status: true on success, false on failure
 */
bool snesCon_client_init(const struct snesCon_client * const client);

/**
 * @brief "Shift out" value to virtual shift register
 *
 * @param client [in/out] Pointer to the snesCon_client object
 * @param btn    [in]     Button bitmap
 */
void snesCon_client_update(struct snesCon_client *client, snesCon_btn_t btn);

/**
 * @brief Latch value to output register + start driving first bit (zeroth button)
 *
 * @note  Call this on IRQ on latch pin rising flank
 * @note  Bit is inverted on pin (1 == low, 0 == high). Since low means button pressed
 *
 * @param client [in/out] Pointer to the snesCon_client object
 */
void snesCon_client_latch(struct snesCon_client *client);


/**
 * @brief Clock out one bit on the DATA pin
 *
 * @note  Call this on IRQ on clock pin positive flank
 * @note  Bit is inverted on pin (1 == low, 0 == high). Since low means button pressed
 *
 * @param client [in/out] Pointer to the snesCon_client object
 */
void snesCon_client_clock(struct snesCon_client *client);

#endif
