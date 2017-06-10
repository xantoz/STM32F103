#ifndef _NVIC_
#define _NVIC_

#include "types.h"
#include "memorymap.h"

/**
 * memory mapped structure for Nested Vectored Interrupt Controller (NVIC)
 */
typedef struct
{
  __IO uint32_t ISER[8];                      /*!< Offset: 0x000  Interrupt Set Enable Register           */
       uint32_t RESERVED0[24];
  __IO uint32_t ICER[8];                      /*!< Offset: 0x080  Interrupt Clear Enable Register         */
       uint32_t RSERVED1[24];
  __IO uint32_t ISPR[8];                      /*!< Offset: 0x100  Interrupt Set Pending Register          */
       uint32_t RESERVED2[24];
  __IO uint32_t ICPR[8];                      /*!< Offset: 0x180  Interrupt Clear Pending Register        */
       uint32_t RESERVED3[24];
  __IO uint32_t IABR[8];                      /*!< Offset: 0x200  Interrupt Active bit Register           */
       uint32_t RESERVED4[56];
  __IO uint8_t  IP[240];                      /*!< Offset: 0x300  Interrupt Priority Register (8Bit wide) */
       uint32_t RESERVED5[644];
  __O  uint32_t STIR;                         /*!< Offset: 0xE00  Software Trigger Interrupt Register     */
}  NVIC_Regs;

extern volatile NVIC_Regs NVIC;

#endif
