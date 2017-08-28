/**
 * @brief Common definitions for protocols
 */

#ifndef _PROTOCOL_COMMON_
#define _PROTOCOL_COMMON_

#include <types.h>

/**
 * @brief A type large enough to represent all-digital pads with up to 16 buttons. Which button
 *        corresponds to which bit is protocol specific.
 */
typedef uint16_t btn_t;

#endif
