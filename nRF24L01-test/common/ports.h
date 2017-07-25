#ifndef _nRF24l01_TEST_PORTS_
#define _nRF24l01_TEST_PORTS_

#define LED             ((struct GPIO_PortPin){&GPIOC, 13})
#define DEBUG_INIT_PIN  ((struct GPIO_PortPin){&GPIOC, 15})

#define CSN_PortPin     ((struct GPIO_PortPin){&GPIOA, 4})
#define CE_PortPin      ((struct GPIO_PortPin){&GPIOA, 3})

#define IRQ_EXTI_IRQHandler EXTI2_IRQHandler
#define IRQ_EXTI_PortPin    ((struct GPIO_PortPin){&GPIOA, 2})

#endif  /* _nRF24l01_TEST_PORTS_ */
