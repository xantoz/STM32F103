#include "types.h"
#include "utils.h"
// #include "stm32f10x.h"
#include "rcc.h"
#include "flash.h"
#include "debug.h"
#include "systick.h"

/* These variables are used to pass memory locations from the linker script to our code. */
extern uint8_t _estack[];
extern uint8_t _etext[];
extern uint8_t _sidata[];
extern uint8_t _sdata[];
extern uint8_t _edata[];
extern uint8_t _sbss[];
extern uint8_t _ebss[];

void deadend(void);                                         // neverending handler
void Reset_Handler(void);

// We define some standard handler names here - they all default to deadend but may be
// changed by implementing a real function with that name. So if they are triggered but
// undefined, we'll just stop.

#define DEFAULTS_TO(func) __attribute__ ((weak, alias (#func)))

void main(void) DEFAULTS_TO(deadend);

// Core interrupt handlers
void NMI_Handler(void) DEFAULTS_TO(deadend);
void HardFault_Handler(void) DEFAULTS_TO(deadend);
void MemManage_Handler(void) DEFAULTS_TO(deadend);
void BusFault_Handler(void) DEFAULTS_TO(deadend);
void UsageFault_Handler(void) DEFAULTS_TO(deadend);
void SVC_Handler(void) DEFAULTS_TO(deadend);
void DebugMon_Handler(void) DEFAULTS_TO(deadend);
void PendSV_Handler(void) DEFAULTS_TO(deadend);
void Systick_Handler(void) DEFAULTS_TO(deadend);

// STM32F103_MD interrupt handlers
void WWDG_IRQHandler(void) DEFAULTS_TO(deadend);
void PVD_IRQHandler(void) DEFAULTS_TO(deadend);
void TAMPER_IRQHandler(void) DEFAULTS_TO(deadend);
void RTC_IRQHandler(void) DEFAULTS_TO(deadend);
void FLASH_IRQHandler(void) DEFAULTS_TO(deadend);
void RCC_IRQHandler(void) DEFAULTS_TO(deadend);
void EXTI0_IRQHandler(void) DEFAULTS_TO(deadend);
void EXTI1_IRQHandler(void) DEFAULTS_TO(deadend);
void EXTI2_IRQHandler(void) DEFAULTS_TO(deadend);
void EXTI3_IRQHandler(void) DEFAULTS_TO(deadend);
void EXTI4_IRQHandler(void) DEFAULTS_TO(deadend);
void DMA1_Channel1_IRQHandler(void) DEFAULTS_TO(deadend);
void DMA1_Channel2_IRQHandler(void) DEFAULTS_TO(deadend);
void DMA1_Channel3_IRQHandler(void) DEFAULTS_TO(deadend);
void DMA1_Channel4_IRQHandler(void) DEFAULTS_TO(deadend);
void DMA1_Channel5_IRQHandler(void) DEFAULTS_TO(deadend);
void DMA1_Channel6_IRQHandler(void) DEFAULTS_TO(deadend);
void DMA1_Channel7_IRQHandler(void) DEFAULTS_TO(deadend);
void ADC1_2_IRQHandler(void) DEFAULTS_TO(deadend);
void USB_HP_CAN1_TX_IRQHandler(void) DEFAULTS_TO(deadend);
void USB_LP_CAN1_RX0_IRQHandler(void) DEFAULTS_TO(deadend);
void CAN1_RX1_IRQHandler(void) DEFAULTS_TO(deadend);
void CAN1_SCE_IRQHandler(void) DEFAULTS_TO(deadend);
void EXTI9_5_IRQHandler(void) DEFAULTS_TO(deadend);
void TIM1_BRK_IRQHandler(void) DEFAULTS_TO(deadend);
void TIM1_UP_IRQHandler(void) DEFAULTS_TO(deadend);
void TIM1_TRG_COM_IRQHandler(void) DEFAULTS_TO(deadend);
void TIM1_CC_IRQHandler(void) DEFAULTS_TO(deadend);
void TIM2_IRQHandler(void) DEFAULTS_TO(deadend);
void TIM3_IRQHandler(void) DEFAULTS_TO(deadend);
void TIM4_IRQHandler(void) DEFAULTS_TO(deadend);
void I2C1_EV_IRQHandler(void) DEFAULTS_TO(deadend);
void I2C1_ER_IRQHandler(void) DEFAULTS_TO(deadend);
void I2C2_EV_IRQHandler(void) DEFAULTS_TO(deadend);
void I2C2_ER_IRQHandler(void) DEFAULTS_TO(deadend);
void SPI1_IRQHandler(void) DEFAULTS_TO(deadend);
void SPI2_IRQHandler(void) DEFAULTS_TO(deadend);
void USART1_IRQHandler(void) DEFAULTS_TO(deadend);
void USART2_IRQHandler(void) DEFAULTS_TO(deadend);
void USART3_IRQHandler(void) DEFAULTS_TO(deadend);
void EXTI15_10_IRQHandler(void) DEFAULTS_TO(deadend);
void RTCAlarm_IRQHandler(void) DEFAULTS_TO(deadend);
void USBWakeUp_IRQHandler(void) DEFAULTS_TO(deadend);

// TODO: use WDOG or define a default interrupt/exception handler that resets the system

/* The vector table - contains the initial stack pointer and pointers to boot code as well as
   interrupt and fault handler pointers. The processor will expect this to be located at address
   0x0, so we put it into a separate linker section. */
IN_SECTION(".isr_vector")
const void* vtable[] = {
    &_estack,                    // Stack top
    Reset_Handler,               // Reset
    NMI_Handler,                 // NMI
    HardFault_Handler,           // Hard fault
    MemManage_Handler,           // Memory management fault
    BusFault_Handler,            // Bus fault
    UsageFault_Handler,          // Usage fault
    NULL,                        // RESERVED
    NULL,                        // RESERVED
    NULL,                        // RESERVED
    NULL,                        // RESERVED
    SVC_Handler,                 // SVCall
    DebugMon_Handler,            // Reserved for Debug
    NULL,                        // RESERVED
    PendSV_Handler,              // PendSV handler
    Systick_Handler,             // The SysTick handler
    WWDG_IRQHandler,             // IRQ0
    PVD_IRQHandler,              // IRQ1
    TAMPER_IRQHandler,           // IRQ2
    RTC_IRQHandler,              // IRQ3
    FLASH_IRQHandler,            // IRQ4
    RCC_IRQHandler,              // IRQ5
    EXTI0_IRQHandler,            // IRQ6
    EXTI1_IRQHandler,            // IRQ7
    EXTI2_IRQHandler,            // IRQ8
    EXTI3_IRQHandler,            // IRQ9
    EXTI4_IRQHandler,            // IRQ10
    DMA1_Channel1_IRQHandler,    // IRQ11
    DMA1_Channel2_IRQHandler,    // IRQ12
    DMA1_Channel3_IRQHandler,    // IRQ13
    DMA1_Channel4_IRQHandler,    // IRQ14
    DMA1_Channel5_IRQHandler,    // IRQ15
    DMA1_Channel6_IRQHandler,    // IRQ16
    DMA1_Channel7_IRQHandler,    // IRQ17
    ADC1_2_IRQHandler,           // IRQ18
    USB_HP_CAN1_TX_IRQHandler,   // IRQ19
    USB_LP_CAN1_RX0_IRQHandler,  // IRQ20
    CAN1_RX1_IRQHandler,         // IRQ21
    CAN1_SCE_IRQHandler,         // IRQ22
    EXTI9_5_IRQHandler,          // IRQ23
    TIM1_BRK_IRQHandler,         // IRQ24
    TIM1_UP_IRQHandler,          // IRQ25
    TIM1_TRG_COM_IRQHandler,     // IRQ26
    TIM1_CC_IRQHandler,          // IRQ27
    TIM2_IRQHandler,             // IRQ28
    TIM3_IRQHandler,             // IRQ29
    TIM4_IRQHandler,             // IRQ30
    I2C1_EV_IRQHandler,          // IRQ31
    I2C1_ER_IRQHandler,          // IRQ32
    I2C2_EV_IRQHandler,          // IRQ33
    I2C2_ER_IRQHandler,          // IRQ34
    SPI1_IRQHandler,             // IRQ35
    SPI2_IRQHandler,             // IRQ36
    USART1_IRQHandler,           // IRQ37
    USART2_IRQHandler,           // IRQ38
    USART3_IRQHandler,           // IRQ39
    EXTI15_10_IRQHandler,        // IRQ40
    RTCAlarm_IRQHandler,         // IRQ41
    USBWakeUp_IRQHandler,        // IRQ42
    deadend,                     // IRQ43
    deadend,                     // IRQ44
    deadend,                     // IRQ45
    deadend,                     // IRQ46
    deadend,                     // IRQ47
    deadend,                     // IRQ48
    deadend,                     // IRQ49
    deadend,                     // IRQ50
    deadend,                     // IRQ51
    deadend,                     // IRQ52
    deadend,                     // IRQ53
    deadend,                     // IRQ54
    deadend,                     // IRQ55
    deadend,                     // IRQ56
    deadend,                     // IRQ57
    deadend,                     // IRQ58
    deadend,                     // IRQ59
    deadend,                     // IRQ60
    deadend,                     // IRQ61
    deadend,                     // IRQ62
    deadend,                     // IRQ63
    deadend,                     // IRQ64
    deadend,                     // IRQ65
    deadend,                     // IRQ66
    deadend,                     // IRQ67
};

static void systemInit()
{
    /* Reset the RCC clock configuration to the default reset state(for debug purpose) */

    RCC.CR |= (uint32_t)0x00000001;        // Set HSION bit
    RCC.CFGR &= (uint32_t)0xF0FF0000;      // Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits
    RCC.CR &= (uint32_t)0xFEF6FFFF;        // Reset HSEON, CSSON and PLLON bits
    RCC.CR &= (uint32_t)0xFFFBFFFF;        // Reset HSEBYP bit
    RCC.CFGR &= (uint32_t)0xFF80FFFF;      // Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits
    RCC.CIR = 0x009F0000;                  // Disable all interrupts and clear pending bits

    RCC.APB2ENR |= RCC_APB2Periph_AFIO;    // Enable AFIO (not really much point in ever shutting it down)
}

void Reset_Handler(void)
{
    __disable_irq();

    // copy initial values of variables (non-const globals and static variables) from FLASH to RAM
    uint8_t *mirror = _sidata;                             //copy from here
    uint8_t *ram = _sdata;                                 //copy to here
    while (ram < _edata) *(ram++) = *(mirror++);

    // set uninitialized globals (and globals initialized to zero) to zero
    ram = _sbss;
    while (ram < _ebss) *(ram++) = 0;

    systemInit();

    __enable_irq();

    //jump into main user code (which should setup needed timers and interrupts or not return at all)
    main();

    //after main, sleep until an interrupt occurs
    while (true) {
        __WFI();
    }
}

/* default handler for unimplemented interrupts or faults. Stay here for debugger to pick up */
void deadend(void) {
    while(1);
}
