#ifndef _UTILS_
#define _UTILS_

#define NOP() { __asm volatile ( "NOP\n"); }
#define BKPT() { __asm volatile ( "BKPT\n"); }

#include "types.h"

/** puts the CPU to sleep until an interrupt occurs */
void waitForInterrupt();

/** temporarily disable interrupts (for atomic access - only do this for a short time) */
void disableInterrupts();

/** re-enable interrupts */
void enableInterrupts();

#define SET(peripheral, reg, field, value) ((peripheral)->(reg) = ((peripheral)->(reg) & ~(peripheral##_##reg##_##field)) | peripheral##_##reg##_##field##_##value)

#endif
