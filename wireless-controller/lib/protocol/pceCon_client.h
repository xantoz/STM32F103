/**
 * @file pceCon_client.h
 *
 * @brief Routines to implement the client side (the controller) of the PCE controller protocol.
 *        It is pretty much an implementation of 74HC157 in software.
 */

#ifndef _PCECON_CLIENT_
#define _PCECON_CLIENT_

#include "pceCon.h"

/**
 * @brief PCE Controller client struct
 */
struct pceCon_client
{
    pceCon_btn_t btn; //!< Button state (bitmap)
    enum
    {
        pceCon_2BTN, //!< emulate a 2 button controller (double poll)
        pceCon_6BTN, //!< emulate a 6 button (avenue pad) controller (quadruple poll)
    } mode;
    bool enabled;         //!< Set to true while enable pin is low
    uint32_t pollCtr;     //!< Incremented each time enabled goes high
    struct
    {
        struct GPIO_PortPin enable;   //!< Enable (active low)
        struct GPIO_PortPin select;   //!< Select
        struct GPIO_PortPin output1Y; //!< up or I (regular 2-button pad)
        struct GPIO_PortPin output2Y; //!< right or II (regular 2-button pad)
        struct GPIO_PortPin output3Y; //!< down or select (regular 2-button pad)
        struct GPIO_PortPin output4Y; //!< left or start (regular 2-button pad)
    } pin;
};

/**
 * @brief Initialize hardware for the pin&port definitions in pceCon_client.
 *
 * @note Does not enable any peripheral clocks, the user of this function is responsible for
 *       ensuring that relevant peripheral clocks have been enabled.
 *
 * @param client [in/out] Pointer to the pceCon_client struct that we want to init
 *
 * @return Status: true on success, false on failure
 */
bool pceCon_client_init(struct pceCon_client *client);

/**
 * @brief Update button state
 *
 * @param client [in/out] Pointer to the snesCon_client object
 * @param btn    [in]     Button bitmap
 */
void pceCon_client_update(struct pceCon_client *client, pceCon_btn_t btn);

/**
 * @brief Enable pin interrupt routine
 *
 * @note  Call this on IRQ of enable pin rising & falling flank
 * @note  Should be higher priority than select pin IRQ
 *
 * @param client [in/out] Pointer to the pceCon_client object
 */
void pceCon_client_enable(struct pceCon_client *client);


/**
 * @brief Select pin interrupt routine
 *
 * @note  Call this on IRQ on select pin rising & falling flank
 * @note  Should be lower priority than enable pin IRQ
 *
 * @param client [in/out] Pointer to the pceCon_client object
 */
void pceCon_client_select(struct pceCon_client *client);

#endif
