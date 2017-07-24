#ifndef _nRF24l01_TEST_PORTS_
#define _nRF24l01_TEST_PORTS_

#define LED             ((struct GPIO_PortPin){&GPIOC, 13})
#define DEBUG_INIT_PIN  ((struct GPIO_PortPin){&GPIOC, 15})

#define CSN_PortPin     ((struct GPIO_PortPin){&GPIOA, 4})
#define CE_PortPin      ((struct GPIO_PortPin){&GPIOA, 3})

#define IRQ_EXTI_PIN        2
#define IRQ_EXTI_PORT       A
#define IRQ_EXTI_AFIO_Port  TOKENPASTE2(AFIO_EXTI_Port, IRQ_EXTI_PORT)
#define IRQ_EXTI_IRQn       TOKENPASTE3(EXTI, IRQ_EXTI_PIN, _IRQn)
#define IRQ_EXTI_PortPin    ((struct GPIO_PortPin){& TOKENPASTE(GPIO, IRQ_EXTI_PORT), IRQ_EXTI_PIN})
#define IRQ_EXTI_IRQHandler TOKENPASTE3(EXTI, IRQ_EXTI_PIN, _IRQHandler)

#endif  /* _nRF24l01_TEST_PORTS_ */
