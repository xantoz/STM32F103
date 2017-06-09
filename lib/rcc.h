#ifndef _RCC_
#define _RCC_

#include "types.h"
#include "memorymap.h"

/**
 * @brief Reset and Clock Control
 */
typedef struct
{
    HW_RW CR;       //!< Clock control register
    HW_RW CFGR;     //!< Clock confguration register
    HW_RW CIR;      //!< Clock interrupt register
    HW_RW APB2RSTR; //!< APB2 peripheral reset register
    HW_RW APB1RSTR; //!< APB1 peripheral reset register
    HW_RW AHBENR;   //!< AHB peripheral clock enable register
    HW_RW APB2ENR;  //!< APB2 peripheral clock enable register
    HW_RW APB1ENR;  //!< APB1 peripheral clock enable register
    HW_RW BDCR;     //!< Backup domain control register
    HW_RW CSR;      //!< Control/status register
} RCC_Struct;

// TODO: Use enums instead of defines?

extern volatile RCC_Struct RCC;

#define RCC_APB1Periph_TIM2              ((uint32_t)0x00000001)
#define RCC_APB1Periph_TIM3              ((uint32_t)0x00000002)
#define RCC_APB1Periph_TIM4              ((uint32_t)0x00000004)
#define RCC_APB1Periph_TIM5              ((uint32_t)0x00000008)
#define RCC_APB1Periph_TIM6              ((uint32_t)0x00000010)
#define RCC_APB1Periph_TIM7              ((uint32_t)0x00000020)
#define RCC_APB1Periph_TIM12             ((uint32_t)0x00000040)
#define RCC_APB1Periph_TIM13             ((uint32_t)0x00000080)
#define RCC_APB1Periph_TIM14             ((uint32_t)0x00000100)
#define RCC_APB1Periph_WWDG              ((uint32_t)0x00000800)
#define RCC_APB1Periph_SPI2              ((uint32_t)0x00004000)
#define RCC_APB1Periph_SPI3              ((uint32_t)0x00008000)
#define RCC_APB1Periph_USART2            ((uint32_t)0x00020000)
#define RCC_APB1Periph_USART3            ((uint32_t)0x00040000)
#define RCC_APB1Periph_UART4             ((uint32_t)0x00080000)
#define RCC_APB1Periph_UART5             ((uint32_t)0x00100000)
#define RCC_APB1Periph_I2C1              ((uint32_t)0x00200000)
#define RCC_APB1Periph_I2C2              ((uint32_t)0x00400000)
#define RCC_APB1Periph_USB               ((uint32_t)0x00800000)
#define RCC_APB1Periph_CAN1              ((uint32_t)0x02000000)
#define RCC_APB1Periph_CAN2              ((uint32_t)0x04000000)
#define RCC_APB1Periph_BKP               ((uint32_t)0x08000000)
#define RCC_APB1Periph_PWR               ((uint32_t)0x10000000)
#define RCC_APB1Periph_DAC               ((uint32_t)0x20000000)
#define RCC_APB1Periph_CEC               ((uint32_t)0x40000000)

#define RCC_APB2Periph_AFIO              ((uint32_t)0x00000001)
#define RCC_APB2Periph_GPIOA             ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_APB2Periph_GPIOC             ((uint32_t)0x00000010)
#define RCC_APB2Periph_GPIOD             ((uint32_t)0x00000020)
#define RCC_APB2Periph_GPIOE             ((uint32_t)0x00000040)
#define RCC_APB2Periph_GPIOF             ((uint32_t)0x00000080)
#define RCC_APB2Periph_GPIOG             ((uint32_t)0x00000100)
#define RCC_APB2Periph_ADC1              ((uint32_t)0x00000200)
#define RCC_APB2Periph_ADC2              ((uint32_t)0x00000400)
#define RCC_APB2Periph_TIM1              ((uint32_t)0x00000800)
#define RCC_APB2Periph_SPI1              ((uint32_t)0x00001000)
#define RCC_APB2Periph_TIM8              ((uint32_t)0x00002000)
#define RCC_APB2Periph_USART1            ((uint32_t)0x00004000)
#define RCC_APB2Periph_ADC3              ((uint32_t)0x00008000)
#define RCC_APB2Periph_TIM15             ((uint32_t)0x00010000)
#define RCC_APB2Periph_TIM16             ((uint32_t)0x00020000)
#define RCC_APB2Periph_TIM17             ((uint32_t)0x00040000)
#define RCC_APB2Periph_TIM9              ((uint32_t)0x00080000)
#define RCC_APB2Periph_TIM10             ((uint32_t)0x00100000)
#define RCC_APB2Periph_TIM11             ((uint32_t)0x00200000)

#define RCC_AHBPeriph_DMA1               ((uint32_t)0x00000001)
#define RCC_AHBPeriph_DMA2               ((uint32_t)0x00000002)
#define RCC_AHBPeriph_SRAM               ((uint32_t)0x00000004)
#define RCC_AHBPeriph_FLITF              ((uint32_t)0x00000010)
#define RCC_AHBPeriph_CRC                ((uint32_t)0x00000040)
#ifndef STM32F10X_CL
 #define RCC_AHBPeriph_FSMC              ((uint32_t)0x00000100)
 #define RCC_AHBPeriph_SDIO              ((uint32_t)0x00000400)
#else
 #define RCC_AHBPeriph_OTG_FS            ((uint32_t)0x00001000)
 #define RCC_AHBPeriph_ETH_MAC           ((uint32_t)0x00004000)
 #define RCC_AHBPeriph_ETH_MAC_Tx        ((uint32_t)0x00008000)
 #define RCC_AHBPeriph_ETH_MAC_Rx        ((uint32_t)0x00010000)
#endif /* STM32F10X_CL */

// TODO: redo above with bitfield names also (like CMSIS, but from reference manual) TODO: Use
// x-macros to avoid unneccesary repetition (while x-macroing: figure out a way to define _Pos &
// _Msk defines as well?)

/********************  Bit definition for RCC_CR register  ********************/
#define RCC_CR_HSION   ((uint32_t)0x00000001)       /*!< Internal High Speed clock enable */
#define RCC_CR_HSIRDY  ((uint32_t)0x00000002)       /*!< Internal High Speed clock ready flag */
#define RCC_CR_HSITRIM ((uint32_t)0x000000F8)       /*!< Internal High Speed clock trimming */
#define RCC_CR_HSICAL  ((uint32_t)0x0000FF00)       /*!< Internal High Speed clock Calibration */
#define RCC_CR_HSEON   ((uint32_t)0x00010000)       /*!< External High Speed clock enable */
#define RCC_CR_HSERDY  ((uint32_t)0x00020000)       /*!< External High Speed clock ready flag */
#define RCC_CR_HSEBYP  ((uint32_t)0x00040000)       /*!< External High Speed clock Bypass */
#define RCC_CR_CSSON   ((uint32_t)0x00080000)       /*!< Clock Security System enable */
#define RCC_CR_PLLON   ((uint32_t)0x01000000)       /*!< PLL enable */
#define RCC_CR_PLLRDY  ((uint32_t)0x02000000)       /*!< PLL clock ready flag */

#ifdef STM32F10X_CL // For STM32F10X_CL only
 #define RCC_CR_PLL2ON  ((uint32_t)0x04000000)       /*!< PLL2 enable */
 #define RCC_CR_PLL2RDY ((uint32_t)0x08000000)       /*!< PLL2 clock ready flag */
 #define RCC_CR_PLL3ON  ((uint32_t)0x10000000)       /*!< PLL3 enable */
 #define RCC_CR_PLL3RDY ((uint32_t)0x20000000)       /*!< PLL3 clock ready flag */
#endif

/*!<******************  Bit definition for RCC_CIR register  ********************/
#define RCC_CIR_LSIRDYF    ((uint32_t)0x00000001)     /*!< LSI Ready Interrupt flag */
#define RCC_CIR_LSERDYF    ((uint32_t)0x00000002)     /*!< LSE Ready Interrupt flag */
#define RCC_CIR_HSIRDYF    ((uint32_t)0x00000004)     /*!< HSI Ready Interrupt flag */
#define RCC_CIR_HSERDYF    ((uint32_t)0x00000008)     /*!< HSE Ready Interrupt flag */
#define RCC_CIR_PLLRDYF    ((uint32_t)0x00000010)     /*!< PLL Ready Interrupt flag */
#define RCC_CIR_CSSF       ((uint32_t)0x00000080)     /*!< Clock Security System Interrupt flag */
#define RCC_CIR_LSIRDYIE   ((uint32_t)0x00000100)     /*!< LSI Ready Interrupt Enable */
#define RCC_CIR_LSERDYIE   ((uint32_t)0x00000200)     /*!< LSE Ready Interrupt Enable */
#define RCC_CIR_HSIRDYIE   ((uint32_t)0x00000400)     /*!< HSI Ready Interrupt Enable */
#define RCC_CIR_HSERDYIE   ((uint32_t)0x00000800)     /*!< HSE Ready Interrupt Enable */
#define RCC_CIR_PLLRDYIE   ((uint32_t)0x00001000)     /*!< PLL Ready Interrupt Enable */
#define RCC_CIR_LSIRDYC    ((uint32_t)0x00010000)     /*!< LSI Ready Interrupt Clear */
#define RCC_CIR_LSERDYC    ((uint32_t)0x00020000)     /*!< LSE Ready Interrupt Clear */
#define RCC_CIR_HSIRDYC    ((uint32_t)0x00040000)     /*!< HSI Ready Interrupt Clear */
#define RCC_CIR_HSERDYC    ((uint32_t)0x00080000)     /*!< HSE Ready Interrupt Clear */
#define RCC_CIR_PLLRDYC    ((uint32_t)0x00100000)     /*!< PLL Ready Interrupt Clear */
#define RCC_CIR_CSSC       ((uint32_t)0x00800000)     /*!< Clock Security System Interrupt Clear */

#ifdef STM32F10X_CL // For STM32F10X_CL only
 #define RCC_CIR_PLL2RDYF   ((uint32_t)0x00000020)     /*!< PLL2 Ready Interrupt flag */
 #define RCC_CIR_PLL3RDYF   ((uint32_t)0x00000040)     /*!< PLL3 Ready Interrupt flag */
 #define RCC_CIR_PLL2RDYIE  ((uint32_t)0x00002000)     /*!< PLL2 Ready Interrupt Enable */
 #define RCC_CIR_PLL3RDYIE  ((uint32_t)0x00004000)     /*!< PLL3 Ready Interrupt Enable */
 #define RCC_CIR_PLL2RDYC   ((uint32_t)0x00200000)     /*!< PLL2 Ready Interrupt Clear */
 #define RCC_CIR_PLL3RDYC   ((uint32_t)0x00400000)     /*!< PLL3 Ready Interrupt Clear */
#endif

//!< ****************** Bit definition for RCC_CFGR register  ********************
#define RCC_CFGR_MCO          ((uint32_t)(0b111 << 24)) //!< MCO[2:0] (Bits 26:24) Microcontroller clock output
#define RCC_CFGR_MCO_NOCLOCK  ((uint32_t)(0b000 << 24)) //!< No clock
#define RCC_CFGR_MCO_SYSCLK   ((uint32_t)(0b100 << 24)) //!< output SYSCLK
#define RCC_CFGR_MCO_HSI      ((uint32_t)(0b101 << 24)) //!< output HSI
#define RCC_CFGR_MCO_HSE      ((uint32_t)(0b110 << 24)) //!< output HSE
#define RCC_CFGR_MCO_PLLDiv2  ((uint32_t)(0b111 << 24)) //!< output PLL clock divided by 2
// Note: This clock output may have some truncated cycles at startup or during MCO clock
// source switching.
// When the System Clock is selected to output to the MCO pin, make sure that this clock
// does not exceed 50 MHz (the maximum IO speed).

#define RCC_CFGR_USBPRE       ((uint32_t)(0b1 << 22))   //!< USB prescaler

// Caution: the PLL output frequency must not exceed 72 MHz
#define RCC_CFGR_PLLMUL_Pos    ((uint32_t)18)
#define RCC_CFGR_PLLMUL_Msk    ((uint32_t)(0b1111 << 18))
#define RCC_CFGR_PLLMUL        ((uint32_t)(0b1111 << 18)) //!< PLLMUL[3:0] (Bits 21:18) PLL mult factor
#define RCC_CFGR_PLLMUL2       ((uint32_t)(0b0000 << 18)) //!< PLL input clock*2
#define RCC_CFGR_PLLMUL3       ((uint32_t)(0b0001 << 18)) //!< PLL input clock*3
#define RCC_CFGR_PLLMUL4       ((uint32_t)(0b0010 << 18)) //!< PLL input clock*4
#define RCC_CFGR_PLLMUL5       ((uint32_t)(0b0011 << 18)) //!< PLL input clock*5
#define RCC_CFGR_PLLMUL6       ((uint32_t)(0b0100 << 18)) //!< PLL input clock*6
#define RCC_CFGR_PLLMUL7       ((uint32_t)(0b0101 << 18)) //!< PLL input clock*7
#define RCC_CFGR_PLLMUL8       ((uint32_t)(0b0110 << 18)) //!< PLL input clock*8
#define RCC_CFGR_PLLMUL9       ((uint32_t)(0b0111 << 18)) //!< PLL input clock*9
#define RCC_CFGR_PLLMUL10      ((uint32_t)(0b1000 << 18)) //!< PLL input clock10
#define RCC_CFGR_PLLMUL11      ((uint32_t)(0b1001 << 18)) //!< PLL input clock*11
#define RCC_CFGR_PLLMUL12      ((uint32_t)(0b1010 << 18)) //!< PLL input clock*12
#define RCC_CFGR_PLLMUL13      ((uint32_t)(0b1011 << 18)) //!< PLL input clock*13
#define RCC_CFGR_PLLMUL14      ((uint32_t)(0b1100 << 18)) //!< PLL input clock*14
#define RCC_CFGR_PLLMUL15      ((uint32_t)(0b1101 << 18)) //!< PLL input clock*15
#define RCC_CFGR_PLLMUL16      ((uint32_t)(0b1110 << 18)) //!< PLL input clock*16
#define RCC_CFGR_PLLMUL16_ALT  ((uint32_t)(0b1111 << 18)) //!< PLL input clock*16

#define RCC_CFGR_PLLXTPRE          ((uint32_t)(0b1 << 17))  //!< HSE divider for PLL entry
#define RCC_CFGR_PLLXTPRE_HSE_Div1 ((uint32_t)(0b0 << 17))  //!< HSE clock not divided
#define RCC_CFGR_PLLXTPRE_HSE_Div2 ((uint32_t)(0b1 << 17))  //!< HSE clock divided by 2

#define RCC_CFGR_PLLSRC          ((uint32_t)(0b1 << 16))  //!< PLL entry clock source
#define RCC_CFGR_PLLSRC_HSI_Div2 ((uint32_t)(0b0 << 16))  //!< HSI clock/2 as PLL input
#define RCC_CFGR_PLLSRC_HSE      ((uint32_t)(0b1 << 16))  //!< HSE clock as PLL input (Ev. divider controlled by PLLXTPRE)

#define RCC_CFGR_ADCPRE            ((uint32_t)(0b11 << 14)) //!< ADCPRE[1:0] (Bits 15:14) ADC prescaler
#define RCC_CFGR_ADCPRE_PCLK2_Div2 ((uint32_t)(0b00 << 14)) //!< PCLK2 Divided by 2
#define RCC_CFGR_ADCPRE_PCLK2_Div4 ((uint32_t)(0b01 << 14)) //!< PCLK2 Divided by 4
#define RCC_CFGR_ADCPRE_PCLK2_Div6 ((uint32_t)(0b10 << 14)) //!< PCLK2 Divided by 6
#define RCC_CFGR_ADCPRE_PCLK2_Div8 ((uint32_t)(0b11 << 14)) //!< PCLK2 Divided by 8

#define RCC_CFGR_PPRE2            ((uint32_t)(0b111 << 11)) //!< PPRE2[2:0] (Bits 13:11) APB2 (high-speed) prescaler
#define RCC_CFGR_PPRE2_HCLK_Div1  ((uint32_t)(0b000 << 11)) //!< HCLK not divided
#define RCC_CFGR_PPRE2_HCLK_Div2  ((uint32_t)(0b100 << 11)) //!< HCLK/2
#define RCC_CFGR_PPRE2_HCLK_Div4  ((uint32_t)(0b101 << 11)) //!< HCLK/4
#define RCC_CFGR_PPRE2_HCLK_Div8  ((uint32_t)(0b110 << 11)) //!< HCLK/8
#define RCC_CFGR_PPRE2_HCLK_Div16 ((uint32_t)(0b111 << 11)) //!< HCLK/16

#define RCC_CFGR_PPRE1            ((uint32_t)(0b111 << 8)) //!< PPRE1[2:0] (Bits 10:8) APB1 (low-speed) prescaler
#define RCC_CFGR_PPRE1_HCLK_Div1  ((uint32_t)(0b000 << 8)) //!< HCLK not divided
#define RCC_CFGR_PPRE1_HCLK_Div2  ((uint32_t)(0b100 << 8)) //!< HCLK/2
#define RCC_CFGR_PPRE1_HCLK_Div4  ((uint32_t)(0b101 << 8)) //!< HCLK/4
#define RCC_CFGR_PPRE1_HCLK_Div8  ((uint32_t)(0b110 << 8)) //!< HCLK/8
#define RCC_CFGR_PPRE1_HCLK_Div16 ((uint32_t)(0b111 << 8)) //!< HCLK/16

#define RCC_CFGR_HPRE               ((uint32_t)(0b1111 << 4)) //!< HPRE[3:0] (Bits 7:4) AHB prescaler (HCLK prescaler)
#define RCC_CFGR_HPRE_SYSCLK_Div1   ((uint32_t)(0b0000 << 4)) //!< SYSCLK not divided
#define RCC_CFGR_HPRE_SYSCLK_Div2   ((uint32_t)(0b1000 << 4)) //!< SYSCLK/2
#define RCC_CFGR_HPRE_SYSCLK_Div4   ((uint32_t)(0b1001 << 4)) //!< SYSCLK/4
#define RCC_CFGR_HPRE_SYSCLK_Div8   ((uint32_t)(0b1010 << 4)) //!< SYSCLK/8
#define RCC_CFGR_HPRE_SYSCLK_Div16  ((uint32_t)(0b1011 << 4)) //!< SYSCLK/16
#define RCC_CFGR_HPRE_SYSCLK_Div64  ((uint32_t)(0b1100 << 4)) //!< SYSCLK/64
#define RCC_CFGR_HPRE_SYSCLK_Div128 ((uint32_t)(0b1101 << 4)) //!< SYSCLK/128
#define RCC_CFGR_HPRE_SYSCLK_Div256 ((uint32_t)(0b1110 << 4)) //!< SYSCLK/256
#define RCC_CFGR_HPRE_SYSCLK_Div512 ((uint32_t)(0b1111 << 4)) //!< SYSCLK/512
// Note: The prefetch buffer must be kept on when using a prescaler different from 1 on the AHB
// clock. (Refer to Reading the Flash memory section in the reference manual for more details.)

#define RCC_CFGR_SWS     ((uint32_t)(0b11 << 2)) //!< SWS[1:0] (Bits 3:2) System clock switch status
#define RCC_CFGR_SWS_HSI ((uint32_t)(0b00 << 2)) //!< HSI used as SYSCLK
#define RCC_CFGR_SWS_HSE ((uint32_t)(0b01 << 2)) //!< HSE used as SYSCLK
#define RCC_CFGR_SWS_PLL ((uint32_t)(0b10 << 2)) //!< PLL used as SYSCLK

#define RCC_CFGR_SW     ((uint32_t)(0b11 << 0)) //!< SW[1:0] (Bits 1:0) System clock switch
#define RCC_CFGR_SW_HSI ((uint32_t)(0b00 << 0)) //!< use HSI as SYSCLK
#define RCC_CFGR_SW_HSE ((uint32_t)(0b01 << 0)) //!< use HSE as SYSCLK
#define RCC_CFGR_SW_PLL ((uint32_t)(0b10 << 0)) //!< use PLL as SYSCLK


// TODO: Bit definitions for all RCC registers

#endif /* _RCC_ */
