#ifndef _AFIO_
#define _AFIO_


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
 *   @brief Bit definition for AFIO_EXTICR1 register
 */
enum AFIO_EXTICR1_Flg
{
    AFIO_EXTICR1_EXTI0    = 0x000F,            /*!< EXTI 0 configuration */
    AFIO_EXTICR1_EXTI1    = 0x00F0,            /*!< EXTI 1 configuration */
    AFIO_EXTICR1_EXTI2    = 0x0F00,            /*!< EXTI 2 configuration */
    AFIO_EXTICR1_EXTI3    = 0xF000,            /*!< EXTI 3 configuration */

    /*!< EXTI0 configuration */
    AFIO_EXTICR1_EXTI0_PA = 0x0000,            /*!< PA[0] pin */
    AFIO_EXTICR1_EXTI0_PB = 0x0001,            /*!< PB[0] pin */
    AFIO_EXTICR1_EXTI0_PC = 0x0002,            /*!< PC[0] pin */
    AFIO_EXTICR1_EXTI0_PD = 0x0003,            /*!< PD[0] pin */
    AFIO_EXTICR1_EXTI0_PE = 0x0004,            /*!< PE[0] pin */
    AFIO_EXTICR1_EXTI0_PF = 0x0005,            /*!< PF[0] pin */
    AFIO_EXTICR1_EXTI0_PG = 0x0006,            /*!< PG[0] pin */

    /*!< EXTI1 configuration */
    AFIO_EXTICR1_EXTI1_PA = 0x0000,            /*!< PA[1] pin */
    AFIO_EXTICR1_EXTI1_PB = 0x0010,            /*!< PB[1] pin */
    AFIO_EXTICR1_EXTI1_PC = 0x0020,            /*!< PC[1] pin */
    AFIO_EXTICR1_EXTI1_PD = 0x0030,            /*!< PD[1] pin */
    AFIO_EXTICR1_EXTI1_PE = 0x0040,            /*!< PE[1] pin */
    AFIO_EXTICR1_EXTI1_PF = 0x0050,            /*!< PF[1] pin */
    AFIO_EXTICR1_EXTI1_PG = 0x0060,            /*!< PG[1] pin */

    /*!< EXTI2 configuration */
    AFIO_EXTICR1_EXTI2_PA = 0x0000,            /*!< PA[2] pin */
    AFIO_EXTICR1_EXTI2_PB = 0x0100,            /*!< PB[2] pin */
    AFIO_EXTICR1_EXTI2_PC = 0x0200,            /*!< PC[2] pin */
    AFIO_EXTICR1_EXTI2_PD = 0x0300,            /*!< PD[2] pin */
    AFIO_EXTICR1_EXTI2_PE = 0x0400,            /*!< PE[2] pin */
    AFIO_EXTICR1_EXTI2_PF = 0x0500,            /*!< PF[2] pin */
    AFIO_EXTICR1_EXTI2_PG = 0x0600,            /*!< PG[2] pin */

    /*!< EXTI3 configuration */
    AFIO_EXTICR1_EXTI3_PA = 0x0000,            /*!< PA[3] pin */
    AFIO_EXTICR1_EXTI3_PB = 0x1000,            /*!< PB[3] pin */
    AFIO_EXTICR1_EXTI3_PC = 0x2000,            /*!< PC[3] pin */
    AFIO_EXTICR1_EXTI3_PD = 0x3000,            /*!< PD[3] pin */
    AFIO_EXTICR1_EXTI3_PE = 0x4000,            /*!< PE[3] pin */
    AFIO_EXTICR1_EXTI3_PF = 0x5000,            /*!< PF[3] pin */
    AFIO_EXTICR1_EXTI3_PG = 0x6000,            /*!< PG[3] pin */
};

/**
 * @brief Bit definition for AFIO_EXTICR2 register
 */
enum AFIO_EXTICR2_Flg
{
    AFIO_EXTICR2_EXTI4    = 0x000F,            /*!< EXTI 4 configuration */
    AFIO_EXTICR2_EXTI5    = 0x00F0,            /*!< EXTI 5 configuration */
    AFIO_EXTICR2_EXTI6    = 0x0F00,            /*!< EXTI 6 configuration */
    AFIO_EXTICR2_EXTI7    = 0xF000,            /*!< EXTI 7 configuration */

    /*!< EXTI4 configuration */
    AFIO_EXTICR2_EXTI4_PA = 0x0000,            /*!< PA[4] pin */
    AFIO_EXTICR2_EXTI4_PB = 0x0001,            /*!< PB[4] pin */
    AFIO_EXTICR2_EXTI4_PC = 0x0002,            /*!< PC[4] pin */
    AFIO_EXTICR2_EXTI4_PD = 0x0003,            /*!< PD[4] pin */
    AFIO_EXTICR2_EXTI4_PE = 0x0004,            /*!< PE[4] pin */
    AFIO_EXTICR2_EXTI4_PF = 0x0005,            /*!< PF[4] pin */
    AFIO_EXTICR2_EXTI4_PG = 0x0006,            /*!< PG[4] pin */

    /*!< EXTI5 configuration */
    AFIO_EXTICR2_EXTI5_PA = 0x0000,            /*!< PA[5] pin */
    AFIO_EXTICR2_EXTI5_PB = 0x0010,            /*!< PB[5] pin */
    AFIO_EXTICR2_EXTI5_PC = 0x0020,            /*!< PC[5] pin */
    AFIO_EXTICR2_EXTI5_PD = 0x0030,            /*!< PD[5] pin */
    AFIO_EXTICR2_EXTI5_PE = 0x0040,            /*!< PE[5] pin */
    AFIO_EXTICR2_EXTI5_PF = 0x0050,            /*!< PF[5] pin */
    AFIO_EXTICR2_EXTI5_PG = 0x0060,            /*!< PG[5] pin */

    /*!< EXTI6 configuration */
    AFIO_EXTICR2_EXTI6_PA = 0x0000,            /*!< PA[6] pin */
    AFIO_EXTICR2_EXTI6_PB = 0x0100,            /*!< PB[6] pin */
    AFIO_EXTICR2_EXTI6_PC = 0x0200,            /*!< PC[6] pin */
    AFIO_EXTICR2_EXTI6_PD = 0x0300,            /*!< PD[6] pin */
    AFIO_EXTICR2_EXTI6_PE = 0x0400,            /*!< PE[6] pin */
    AFIO_EXTICR2_EXTI6_PF = 0x0500,            /*!< PF[6] pin */
    AFIO_EXTICR2_EXTI6_PG = 0x0600,            /*!< PG[6] pin */

    /*!< EXTI7 configuration */
    AFIO_EXTICR2_EXTI7_PA = 0x0000,            /*!< PA[7] pin */
    AFIO_EXTICR2_EXTI7_PB = 0x1000,            /*!< PB[7] pin */
    AFIO_EXTICR2_EXTI7_PC = 0x2000,            /*!< PC[7] pin */
    AFIO_EXTICR2_EXTI7_PD = 0x3000,            /*!< PD[7] pin */
    AFIO_EXTICR2_EXTI7_PE = 0x4000,            /*!< PE[7] pin */
    AFIO_EXTICR2_EXTI7_PF = 0x5000,            /*!< PF[7] pin */
    AFIO_EXTICR2_EXTI7_PG = 0x6000,            /*!< PG[7] pin */
};

/**
 * @brief Bit definition for AFIO_EXTICR3 register
 */
enum AFIO_EXTICR3_Flg
{
    AFIO_EXTICR3_EXTI8     = 0x000F,            /*!< EXTI 8 configuration */
    AFIO_EXTICR3_EXTI9     = 0x00F0,            /*!< EXTI 9 configuration */
    AFIO_EXTICR3_EXTI10    = 0x0F00,            /*!< EXTI 10 configuration */
    AFIO_EXTICR3_EXTI11    = 0xF000,            /*!< EXTI 11 configuration */

    /*!< EXTI8 configuration */
    AFIO_EXTICR3_EXTI8_PA  = 0x0000,            /*!< PA[8] pin */
    AFIO_EXTICR3_EXTI8_PB  = 0x0001,            /*!< PB[8] pin */
    AFIO_EXTICR3_EXTI8_PC  = 0x0002,            /*!< PC[8] pin */
    AFIO_EXTICR3_EXTI8_PD  = 0x0003,            /*!< PD[8] pin */
    AFIO_EXTICR3_EXTI8_PE  = 0x0004,            /*!< PE[8] pin */
    AFIO_EXTICR3_EXTI8_PF  = 0x0005,            /*!< PF[8] pin */
    AFIO_EXTICR3_EXTI8_PG  = 0x0006,            /*!< PG[8] pin */

    /*!< EXTI9 configuration */
    AFIO_EXTICR3_EXTI9_PA  = 0x0000,            /*!< PA[9] pin */
    AFIO_EXTICR3_EXTI9_PB  = 0x0010,            /*!< PB[9] pin */
    AFIO_EXTICR3_EXTI9_PC  = 0x0020,            /*!< PC[9] pin */
    AFIO_EXTICR3_EXTI9_PD  = 0x0030,            /*!< PD[9] pin */
    AFIO_EXTICR3_EXTI9_PE  = 0x0040,            /*!< PE[9] pin */
    AFIO_EXTICR3_EXTI9_PF  = 0x0050,            /*!< PF[9] pin */
    AFIO_EXTICR3_EXTI9_PG  = 0x0060,            /*!< PG[9] pin */

    /*!< EXTI10 configuration */
    AFIO_EXTICR3_EXTI10_PA = 0x0000,            /*!< PA[10] pin */
    AFIO_EXTICR3_EXTI10_PB = 0x0100,            /*!< PB[10] pin */
    AFIO_EXTICR3_EXTI10_PC = 0x0200,            /*!< PC[10] pin */
    AFIO_EXTICR3_EXTI10_PD = 0x0300,            /*!< PD[10] pin */
    AFIO_EXTICR3_EXTI10_PE = 0x0400,            /*!< PE[10] pin */
    AFIO_EXTICR3_EXTI10_PF = 0x0500,            /*!< PF[10] pin */
    AFIO_EXTICR3_EXTI10_PG = 0x0600,            /*!< PG[10] pin */

    /*!< EXTI11 configuration */
    AFIO_EXTICR3_EXTI11_PA = 0x0000,            /*!< PA[11] pin */
    AFIO_EXTICR3_EXTI11_PB = 0x1000,            /*!< PB[11] pin */
    AFIO_EXTICR3_EXTI11_PC = 0x2000,            /*!< PC[11] pin */
    AFIO_EXTICR3_EXTI11_PD = 0x3000,            /*!< PD[11] pin */
    AFIO_EXTICR3_EXTI11_PE = 0x4000,            /*!< PE[11] pin */
    AFIO_EXTICR3_EXTI11_PF = 0x5000,            /*!< PF[11] pin */
    AFIO_EXTICR3_EXTI11_PG = 0x6000,            /*!< PG[11] pin */
};

/**
 * @brief Bit definition for AFIO_EXTICR4 register
 */
enum AFIO_EXTICR4_Flg
{
    AFIO_EXTICR4_EXTI12    = 0x000F,            /*!< EXTI 12 configuration */
    AFIO_EXTICR4_EXTI13    = 0x00F0,            /*!< EXTI 13 configuration */
    AFIO_EXTICR4_EXTI14    = 0x0F00,            /*!< EXTI 14 configuration */
    AFIO_EXTICR4_EXTI15    = 0xF000,            /*!< EXTI 15 configuration */

    /*!< EXTI12 configuration */
    AFIO_EXTICR4_EXTI12_PA = 0x0000,            /*!< PA[12] pin */
    AFIO_EXTICR4_EXTI12_PB = 0x0001,            /*!< PB[12] pin */
    AFIO_EXTICR4_EXTI12_PC = 0x0002,            /*!< PC[12] pin */
    AFIO_EXTICR4_EXTI12_PD = 0x0003,            /*!< PD[12] pin */
    AFIO_EXTICR4_EXTI12_PE = 0x0004,            /*!< PE[12] pin */
    AFIO_EXTICR4_EXTI12_PF = 0x0005,            /*!< PF[12] pin */
    AFIO_EXTICR4_EXTI12_PG = 0x0006,            /*!< PG[12] pin */

    /*!< EXTI13 configuration */
    AFIO_EXTICR4_EXTI13_PA = 0x0000,            /*!< PA[13] pin */
    AFIO_EXTICR4_EXTI13_PB = 0x0010,            /*!< PB[13] pin */
    AFIO_EXTICR4_EXTI13_PC = 0x0020,            /*!< PC[13] pin */
    AFIO_EXTICR4_EXTI13_PD = 0x0030,            /*!< PD[13] pin */
    AFIO_EXTICR4_EXTI13_PE = 0x0040,            /*!< PE[13] pin */
    AFIO_EXTICR4_EXTI13_PF = 0x0050,            /*!< PF[13] pin */
    AFIO_EXTICR4_EXTI13_PG = 0x0060,            /*!< PG[13] pin */

    /*!< EXTI14 configuration */
    AFIO_EXTICR4_EXTI14_PA = 0x0000,            /*!< PA[14] pin */
    AFIO_EXTICR4_EXTI14_PB = 0x0100,            /*!< PB[14] pin */
    AFIO_EXTICR4_EXTI14_PC = 0x0200,            /*!< PC[14] pin */
    AFIO_EXTICR4_EXTI14_PD = 0x0300,            /*!< PD[14] pin */
    AFIO_EXTICR4_EXTI14_PE = 0x0400,            /*!< PE[14] pin */
    AFIO_EXTICR4_EXTI14_PF = 0x0500,            /*!< PF[14] pin */
    AFIO_EXTICR4_EXTI14_PG = 0x0600,            /*!< PG[14] pin */

    /*!< EXTI15 configuration */
    AFIO_EXTICR4_EXTI15_PA = 0x0000,            /*!< PA[15] pin */
    AFIO_EXTICR4_EXTI15_PB = 0x1000,            /*!< PB[15] pin */
    AFIO_EXTICR4_EXTI15_PC = 0x2000,            /*!< PC[15] pin */
    AFIO_EXTICR4_EXTI15_PD = 0x3000,            /*!< PD[15] pin */
    AFIO_EXTICR4_EXTI15_PE = 0x4000,            /*!< PE[15] pin */
    AFIO_EXTICR4_EXTI15_PF = 0x5000,            /*!< PF[15] pin */
    AFIO_EXTICR4_EXTI15_PG = 0x6000,            /*!< PG[15] pin */
};

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
    DEFAULT,
    ALTERNATE
};

#endif /* _AFIO_ */
