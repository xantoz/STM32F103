#ifndef _MEMORYMAP_
#define _MEMORYMAP_

#include "types.h"

extern volatile uint32_t SRAM_BASE[];
extern volatile uint32_t PERIPH_BASE[];

extern volatile uint32_t SRAM_BB_BASE[];
extern volatile uint32_t PERIPH_BB_BASE[];

extern volatile uint32_t FSMC_R_BASE[];

/*!< Memory mapping of Cortex-M3 Hardware */
extern volatile uint32_t SCS_BASE[];
extern volatile uint32_t ITM_BASE[];
extern volatile uint32_t CoreDebug_BASE[];
extern volatile uint32_t SysTick_BASE[];
extern volatile uint32_t NVIC_BASE[];
extern volatile uint32_t SCB_BASE[];

/*!< Peripheral memory map */
extern volatile uint32_t APB1PERIPH_BASE[];
extern volatile uint32_t APB2PERIPH_BASE[];
extern volatile uint32_t AHBPERIPH_BASE[];

extern volatile uint32_t AFIO_BASE[];
extern volatile uint32_t EXTI_BASE[];

extern volatile uint32_t FLASH_R_BASE[];
extern volatile uint32_t OB_BASE[];

extern volatile uint32_t GPIOA_BASE[];
extern volatile uint32_t GPIOB_BASE[];
extern volatile uint32_t GPIOC_BASE[];


#endif
