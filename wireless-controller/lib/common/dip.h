/**
 * @file dip.h
 *
 * @brief Functions to read the DIP switch (used for address selection)
 */

#include <types.h>

// TODO: Multiplexing several DIP switches

void dip_init();

uint8_t dip_read();
