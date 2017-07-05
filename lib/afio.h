#ifndef _AFIO_
#define _AFIO_

#include "utils.h"
#include "debug.h"  // For assertions

/**
 * @brief Alternate Function IO register defs
 */
struct AFIO_Regs
{
    __IO uint32_t EVCR;
    __IO uint32_t MAPR;
    __IO uint32_t EXTICR[4];
    uint32_t RESERVED0;
    __IO uint32_t MAPR2;
};

extern volatile struct AFIO_Regs AFIO;

/**
 * @brief definition for AFIO_EVCR register
 */
enum AFIO_EVCR_Flg
{
    AFIO_EVCR_PIN      = 0x0F,               /*!< PIN[3:0] bits (Pin selection) */
    AFIO_EVCR_PIN_0    = 0x01,               /*!< Bit 0 */
    AFIO_EVCR_PIN_1    = 0x02,               /*!< Bit 1 */
    AFIO_EVCR_PIN_2    = 0x04,               /*!< Bit 2 */
    AFIO_EVCR_PIN_3    = 0x08,               /*!< Bit 3 */

    /*!< PIN configuration */
    AFIO_EVCR_PIN_PX0  = 0x00,               /*!< Pin 0 selected */
    AFIO_EVCR_PIN_PX1  = 0x01,               /*!< Pin 1 selected */
    AFIO_EVCR_PIN_PX2  = 0x02,               /*!< Pin 2 selected */
    AFIO_EVCR_PIN_PX3  = 0x03,               /*!< Pin 3 selected */
    AFIO_EVCR_PIN_PX4  = 0x04,               /*!< Pin 4 selected */
    AFIO_EVCR_PIN_PX5  = 0x05,               /*!< Pin 5 selected */
    AFIO_EVCR_PIN_PX6  = 0x06,               /*!< Pin 6 selected */
    AFIO_EVCR_PIN_PX7  = 0x07,               /*!< Pin 7 selected */
    AFIO_EVCR_PIN_PX8  = 0x08,               /*!< Pin 8 selected */
    AFIO_EVCR_PIN_PX9  = 0x09,               /*!< Pin 9 selected */
    AFIO_EVCR_PIN_PX10 = 0x0A,               /*!< Pin 10 selected */
    AFIO_EVCR_PIN_PX11 = 0x0B,               /*!< Pin 11 selected */
    AFIO_EVCR_PIN_PX12 = 0x0C,               /*!< Pin 12 selected */
    AFIO_EVCR_PIN_PX13 = 0x0D,               /*!< Pin 13 selected */
    AFIO_EVCR_PIN_PX14 = 0x0E,               /*!< Pin 14 selected */
    AFIO_EVCR_PIN_PX15 = 0x0F,               /*!< Pin 15 selected */

    AFIO_EVCR_PORT     = 0x70,               /*!< PORT[2:0] bits (Port selection) */
    AFIO_EVCR_PORT_0   = 0x10,               /*!< Bit 0 */
    AFIO_EVCR_PORT_1   = 0x20,               /*!< Bit 1 */
    AFIO_EVCR_PORT_2   = 0x40,               /*!< Bit 2 */

    /*!< PORT configuration */
    AFIO_EVCR_PORT_PA  = 0x00,               /*!< Port A selected */
    AFIO_EVCR_PORT_PB  = 0x10,               /*!< Port B selected */
    AFIO_EVCR_PORT_PC  = 0x20,               /*!< Port C selected */
    AFIO_EVCR_PORT_PD  = 0x30,               /*!< Port D selected */
    AFIO_EVCR_PORT_PE  = 0x40,               /*!< Port E selected */

    AFIO_EVCR_EVOE     = 0x80,               /*!< Event Output Enable */
};

/**
 * @brief Bit definition for AFIO_MAPR register
 */
enum AFIO_MAPR_Flg
{
    AFIO_MAPR_SPI1_REMAP                = 0x00000001,        /*!< SPI1 remapping */
    AFIO_MAPR_I2C1_REMAP                = 0x00000002,        /*!< I2C1 remapping */
    AFIO_MAPR_USART1_REMAP              = 0x00000004,        /*!< USART1 remapping */
    AFIO_MAPR_USART2_REMAP              = 0x00000008,        /*!< USART2 remapping */

    /*!< USART3_REMAP configuration */
    AFIO_MAPR_USART3_REMAP              = 0x00000030,        /*!< USART3_REMAP[1:0] bits (USART3 remapping) */
    AFIO_MAPR_USART3_REMAP_NOREMAP      = 0x00000000,        /*!< No remap (TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14) */
    AFIO_MAPR_USART3_REMAP_PARTIALREMAP = 0x00000010,        /*!< Partial remap (TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14) */
    AFIO_MAPR_USART3_REMAP_FULLREMAP    = 0x00000030,        /*!< Full remap (TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12) */

    /*!< TIM1_REMAP configuration */
    AFIO_MAPR_TIM1_REMAP                = 0x000000C0,        /*!< TIM1_REMAP[1:0] bits (TIM1 remapping) */
    AFIO_MAPR_TIM1_REMAP_NOREMAP        = 0x00000000,        /*!< No remap (ETR/PA12, CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PB12, CH1N/PB13, CH2N/PB14, CH3N/PB15) */
    AFIO_MAPR_TIM1_REMAP_PARTIALREMAP   = 0x00000040,        /*!< Partial remap (ETR/PA12, CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PA6, CH1N/PA7, CH2N/PB0, CH3N/PB1) */
    AFIO_MAPR_TIM1_REMAP_FULLREMAP      = 0x000000C0,        /*!< Full remap (ETR/PE7, CH1/PE9, CH2/PE11, CH3/PE13, CH4/PE14, BKIN/PE15, CH1N/PE8, CH2N/PE10, CH3N/PE12) */

    /*!< TIM2_REMAP configuration */
    AFIO_MAPR_TIM2_REMAP                = 0x00000300,        /*!< TIM2_REMAP[1:0] bits (TIM2 remapping) */
    AFIO_MAPR_TIM2_REMAP_NOREMAP        = 0x00000000,        /*!< No remap (CH1/ETR/PA0, CH2/PA1, CH3/PA2, CH4/PA3) */
    AFIO_MAPR_TIM2_REMAP_PARTIALREMAP1  = 0x00000100,        /*!< Partial remap (CH1/ETR/PA15, CH2/PB3, CH3/PA2, CH4/PA3) */
    AFIO_MAPR_TIM2_REMAP_PARTIALREMAP2  = 0x00000200,        /*!< Partial remap (CH1/ETR/PA0, CH2/PA1, CH3/PB10, CH4/PB11) */
    AFIO_MAPR_TIM2_REMAP_FULLREMAP      = 0x00000300,        /*!< Full remap (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11) */

    /*!< TIM3_REMAP configuration */
    AFIO_MAPR_TIM3_REMAP                = 0x00000C00,        /*!< TIM3_REMAP[1:0] bits (TIM3 remapping) */
    AFIO_MAPR_TIM3_REMAP_NOREMAP        = 0x00000000,        /*!< No remap (CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1) */
    AFIO_MAPR_TIM3_REMAP_PARTIALREMAP   = 0x00000800,        /*!< Partial remap (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1) */
    AFIO_MAPR_TIM3_REMAP_FULLREMAP      = 0x00000C00,        /*!< Full remap (CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9) */

    AFIO_MAPR_TIM4_REMAP                = 0x00001000,        /*!< TIM4_REMAP bit (TIM4 remapping) */

    /*!< CAN_REMAP configuration */
    AFIO_MAPR_CAN_REMAP                 = 0x00006000,        /*!< CAN_REMAP[1:0] bits (CAN Alternate function remapping) */
    AFIO_MAPR_CAN_REMAP_REMAP1          = 0x00000000,        /*!< CANRX mapped to PA11, CANTX mapped to PA12 */
    AFIO_MAPR_CAN_REMAP_REMAP2          = 0x00004000,        /*!< CANRX mapped to PB8, CANTX mapped to PB9 */
    AFIO_MAPR_CAN_REMAP_REMAP3          = 0x00006000,        /*!< CANRX mapped to PD0, CANTX mapped to PD1 */

    AFIO_MAPR_PD01_REMAP                = 0x00008000,        /*!< Port D0/Port D1 mapping on OSC_IN/OSC_OUT */

    AFIO_MAPR_TIM5CH4_IREMAP            = 0x00010000,        /*!< TIM5 Channel4 Internal Remap */

    AFIO_MAPR_ADC1_ETRGINJ_REMAP        = 0x00020000,        /*!< ADC 1 External Trigger Injected Conversion remapping */
    AFIO_MAPR_ADC1_ETRGREG_REMAP        = 0x00040000,        /*!< ADC 1 External Trigger Regular Conversion remapping */

    AFIO_MAPR_ADC2_ETRGINJ_REMAP        = 0x00080000,        /*!< ADC 2 External Trigger Injected Conversion remapping */
    AFIO_MAPR_ADC2_ETRGREG_REMAP        = 0x00100000,        /*!< ADC 2 External Trigger Regular Conversion remapping */

    /*!< SWJ_CFG configuration */
    AFIO_MAPR_SWJ_CFG                   = 0x07000000,        /*!< SWJ_CFG[2:0] bits (Serial Wire JTAG configuration) */
    AFIO_MAPR_SWJ_CFG_RESET             = 0x00000000,        /*!< Full SWJ (JTAG-DP + SW-DP) : Reset State */
    AFIO_MAPR_SWJ_CFG_NOJNTRST          = 0x01000000,        /*!< Full SWJ (JTAG-DP + SW-DP) but without JNTRST */
    AFIO_MAPR_SWJ_CFG_JTAGDISABLE       = 0x02000000,        /*!< JTAG-DP Disabled and SW-DP Enabled */
    AFIO_MAPR_SWJ_CFG_DISABLE           = 0x04000000,        /*!< JTAG-DP Disabled and SW-DP Disabled */
};

/**
 * @brief Bit definitions for AFIO_EXTICRx_EXTIy registers.
 *
 * @note Needs to be shifted (increments of 4 bits), and applied to the right EXTICRx, to choose
 *       which EXTIy to affect. See the function AFIO_mapEXTI()
 */
enum AFIO_EXTICRx_Flg
{
    AFIO_EXTICRx_EXTIy    = 0xf,  //!< Mask for an EXTIy

    AFIO_EXTICRx_EXTIy_PA = 0x0,  //!< PA[y] pin
    AFIO_EXTICRx_EXTIy_PB = 0x1,  //!< PB[y] pin
    AFIO_EXTICRx_EXTIy_PC = 0x2,  //!< PC[y] pin
    AFIO_EXTICRx_EXTIy_PD = 0x3,  //!< PD[y] pin
    AFIO_EXTICRx_EXTIy_PE = 0x4,  //!< PE[y] pin
    AFIO_EXTICRx_EXTIy_PF = 0x5,  //!< PF[y] pin
    AFIO_EXTICRx_EXTIy_PG = 0x6,  //!< PG[y] pin

    // Aliases for the above that are easier to type & remember
    AFIO_EXTI_PortA = AFIO_EXTICRx_EXTIy_PA,  //!< PA[y] pin (alias of AFIO_EXTICRx_EXTIy_PA)
    AFIO_EXTI_PortB = AFIO_EXTICRx_EXTIy_PB,  //!< PB[y] pin (alias of AFIO_EXTICRx_EXTIy_PB)
    AFIO_EXTI_PortC = AFIO_EXTICRx_EXTIy_PC,  //!< PC[y] pin (alias of AFIO_EXTICRx_EXTIy_PC)
    AFIO_EXTI_PortD = AFIO_EXTICRx_EXTIy_PD,  //!< PD[y] pin (alias of AFIO_EXTICRx_EXTIy_PD)
    AFIO_EXTI_PortE = AFIO_EXTICRx_EXTIy_PE,  //!< PE[y] pin (alias of AFIO_EXTICRx_EXTIy_PE)
    AFIO_EXTI_PortF = AFIO_EXTICRx_EXTIy_PF,  //!< PF[y] pin (alias of AFIO_EXTICRx_EXTIy_PF)
    AFIO_EXTI_PortG = AFIO_EXTICRx_EXTIy_PG,  //!< PG[y] pin (alias of AFIO_EXTICRx_EXTIy_PG)
};

/**
 * @brief Map GPIO port to EXTI line
 *
 * @param EXTIy     [in] which EXTI to use
 * @param port [in] Which GPIO port
 */
static inline void AFIO_mapEXTI(uint8_t EXTIy, enum AFIO_EXTICRx_Flg port)
{
    assert(AFIO_EXTICRx_EXTIy_PA <= port && port <= AFIO_EXTICRx_EXTIy_PG);
    assert(EXTIy <= 15);

    irq_lock_t lock;
    LOCK_IRQ(lock);

    const uint32_t exticr_x = (EXTIy / 4);
    const uint32_t bitpos   = (EXTIy % 4);
    const uint32_t mask = ~(AFIO_EXTICRx_EXTIy << bitpos);
    const uint32_t flag = (port << bitpos);
    AFIO.EXTICR[exticr_x] = (AFIO.EXTICR[exticr_x] & mask) | flag;

    UNLOCK_IRQ(lock);
}

#if defined(STM32F10X_LD_VL) || defined(STM32F10X_MD_VL) || defined(STM32F10X_HD_VL) || defined(STM32F10X_HD_VL) || defined(STM32F10X_XL)
#define HAVE_MAPR2
#endif

#ifdef HAVE_MAPR2
/**
 * @brief Bit definition for AFIO_MAPR2 register
 */
enum AFIO_MAPR2_Flg
{
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
    AFIO_MAPR2_TIM15_REMAP         = 0x00000001,        /*!< TIM15 remapping */
    AFIO_MAPR2_TIM16_REMAP         = 0x00000002,        /*!< TIM16 remapping */
    AFIO_MAPR2_TIM17_REMAP         = 0x00000004,        /*!< TIM17 remapping */
    AFIO_MAPR2_CEC_REMAP           = 0x00000008,        /*!< CEC remapping */
    AFIO_MAPR2_TIM1_DMA_REMAP      = 0x00000010,        /*!< TIM1_DMA remapping */
#elif defined(STM32F10X_HD_VL)
    AFIO_MAPR2_TIM13_REMAP         = 0x00000100,        /*!< TIM13 remapping */
    AFIO_MAPR2_TIM14_REMAP         = 0x00000200,        /*!< TIM14 remapping */
    AFIO_MAPR2_FSMC_NADV_REMAP     = 0x00000400,        /*!< FSMC NADV remapping */
    AFIO_MAPR2_TIM67_DAC_DMA_REMAP = 0x00000800,        /*!< TIM6/TIM7 and DAC DMA remapping */
    AFIO_MAPR2_TIM12_REMAP         = 0x00001000,        /*!< TIM12 remapping */
    AFIO_MAPR2_MISC_REMAP          = 0x00002000,        /*!< Miscellaneous remapping */
#elif defined(STM32F10X_XL)
    AFIO_MAPR2_TIM9_REMAP          = 0x00000020,        /*!< TIM9 remapping */
    AFIO_MAPR2_TIM10_REMAP         = 0x00000040,        /*!< TIM10 remapping */
    AFIO_MAPR2_TIM11_REMAP         = 0x00000080,        /*!< TIM11 remapping */
    AFIO_MAPR2_TIM13_REMAP         = 0x00000100,        /*!< TIM13 remapping */
    AFIO_MAPR2_TIM14_REMAP         = 0x00000200,        /*!< TIM14 remapping */
    AFIO_MAPR2_FSMC_NADV_REMAP     = 0x00000400,        /*!< FSMC NADV remapping */
#endif
};
#endif /* HAVE_MAPR2 */

/**
 * @brief Enum defined here as a convenience for use in client functions that
 * take as an argument the 'default' mapping, or the 'alternate' one.
 */
enum AF_Mapping
{
    AFIO_DEFAULT,
    AFIO_ALTERNATE,
};

#endif /* _AFIO_ */
