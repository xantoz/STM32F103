/**
 * @brief Defines and structs
 */

#ifndef _COMMON_CONFIG_
#define _COMMON_CONFIG_

/* **** Misc ports **** */

#define LED ((struct GPIO_PortPin){&GPIOC, 13})

// Interrupt priorities
#define snesCon_IRQ_Priority  1
#define pceCon_IRQ_Priority   0
#define nRF24L01_IRQ_Priority 4

/* **** Settings for the nRF24L01 **** */

#define nRF24L01_SPI         SPI1
#define nRF24L01_SPI_MAXFREQ 9000000

#define nRF24L01_IRQ_PortPin ((struct GPIO_PortPin){&GPIOA, 2})
#define nRF24L01_IRQHandler  EXTI2_IRQHandler
#define nRF24L01_IRQn        EXTI2_IRQn

#define nRF24L01_CSN_PortPin ((struct GPIO_PortPin){&GPIOA, 4})
#define nRF24L01_CE_PortPin  ((struct GPIO_PortPin){&GPIOA, 3})

/* **** Settings for DIP switch **** */
// DIP on pins B12, B13, B14, B15
#define DIP_Msk 0x0fff
#define DIP_Pos 12
#define DIP_Port GPIOB

#endif
