#ifndef _FLASH_
#define _FLASH_

#include "types.h"

/**
  * @brief FLASH Registers
  */

struct FLASH_Regs
{
    HW_RW ACR;
    HW_RW KEYR;
    HW_RW OPTKEYR;
    HW_RW SR;
    HW_RW CR;
    HW_RW AR;
    HW_RW RESERVED;
    HW_RW OBR;
    HW_RW WRPR;
#ifdef STM32F10X_XL
    HW_RO RESERVED1[8];
    HW_RW KEYR2;
    HW_RO RESERVED2;
    HW_RW SR2;
    HW_RW CR2;
    HW_RW AR2;
#endif /* STM32F10X_XL */
};

extern volatile struct FLASH_Regs FLASH;

/**
  * @brief Option Bytes Registers
  */

struct OB_Regs
{
    __IO uint16_t RDP;
    __IO uint16_t USER;
    __IO uint16_t Data0;
    __IO uint16_t Data1;
    __IO uint16_t WRP0;
    __IO uint16_t WRP1;
    __IO uint16_t WRP2;
    __IO uint16_t WRP3;
};

extern volatile struct OB_Regs OB;

/*******************  Bit definition for FLASH_ACR register  ******************/
#define  FLASH_ACR_LATENCY                   ((uint8_t)0x03)               /*!< LATENCY[2:0] bits (Latency) */
#define  FLASH_ACR_LATENCY_0                 ((uint8_t)0x00)               /*!< Bit 0 */
#define  FLASH_ACR_LATENCY_1                 ((uint8_t)0x01)               /*!< Bit 0 */
#define  FLASH_ACR_LATENCY_2                 ((uint8_t)0x02)               /*!< Bit 1 */

#define  FLASH_ACR_HLFCYA                    ((uint8_t)0x08)               /*!< Flash Half Cycle Access Enable */
#define  FLASH_ACR_PRFTBE                    ((uint8_t)0x10)               /*!< Prefetch Buffer Enable */
#define  FLASH_ACR_PRFTBS                    ((uint8_t)0x20)               /*!< Prefetch Buffer Status */

/******************  Bit definition for FLASH_KEYR register  ******************/
#define  FLASH_KEYR_FKEYR                    ((uint32_t)0xFFFFFFFF)        /*!< FPEC Key */

/*****************  Bit definition for FLASH_OPTKEYR register  ****************/
#define  FLASH_OPTKEYR_OPTKEYR               ((uint32_t)0xFFFFFFFF)        /*!< Option Byte Key */

/******************  Bit definition for FLASH_SR register  *******************/
#define  FLASH_SR_BSY                        ((uint8_t)0x01)               /*!< Busy */
#define  FLASH_SR_PGERR                      ((uint8_t)0x04)               /*!< Programming Error */
#define  FLASH_SR_WRPRTERR                   ((uint8_t)0x10)               /*!< Write Protection Error */
#define  FLASH_SR_EOP                        ((uint8_t)0x20)               /*!< End of operation */

/*******************  Bit definition for FLASH_CR register  *******************/
#define  FLASH_CR_PG                         ((uint16_t)0x0001)            /*!< Programming */
#define  FLASH_CR_PER                        ((uint16_t)0x0002)            /*!< Page Erase */
#define  FLASH_CR_MER                        ((uint16_t)0x0004)            /*!< Mass Erase */
#define  FLASH_CR_OPTPG                      ((uint16_t)0x0010)            /*!< Option Byte Programming */
#define  FLASH_CR_OPTER                      ((uint16_t)0x0020)            /*!< Option Byte Erase */
#define  FLASH_CR_STRT                       ((uint16_t)0x0040)            /*!< Start */
#define  FLASH_CR_LOCK                       ((uint16_t)0x0080)            /*!< Lock */
#define  FLASH_CR_OPTWRE                     ((uint16_t)0x0200)            /*!< Option Bytes Write Enable */
#define  FLASH_CR_ERRIE                      ((uint16_t)0x0400)            /*!< Error Interrupt Enable */
#define  FLASH_CR_EOPIE                      ((uint16_t)0x1000)            /*!< End of operation interrupt enable */

/*******************  Bit definition for FLASH_AR register  *******************/
#define  FLASH_AR_FAR                        ((uint32_t)0xFFFFFFFF)        /*!< Flash Address */

/******************  Bit definition for FLASH_OBR register  *******************/
#define  FLASH_OBR_OPTERR                    ((uint16_t)0x0001)            /*!< Option Byte Error */
#define  FLASH_OBR_RDPRT                     ((uint16_t)0x0002)            /*!< Read protection */

#define  FLASH_OBR_USER                      ((uint16_t)0x03FC)            /*!< User Option Bytes */
#define  FLASH_OBR_WDG_SW                    ((uint16_t)0x0004)            /*!< WDG_SW */
#define  FLASH_OBR_nRST_STOP                 ((uint16_t)0x0008)            /*!< nRST_STOP */
#define  FLASH_OBR_nRST_STDBY                ((uint16_t)0x0010)            /*!< nRST_STDBY */
#define  FLASH_OBR_BFB2                      ((uint16_t)0x0020)            /*!< BFB2 */

/******************  Bit definition for FLASH_WRPR register  ******************/
#define  FLASH_WRPR_WRP                        ((uint32_t)0xFFFFFFFF)        /*!< Write Protect */

/*----------------------------------------------------------------------------*/

/******************  Bit definition for FLASH_RDP register  *******************/
#define  FLASH_RDP_RDP                       ((uint32_t)0x000000FF)        /*!< Read protection option byte */
#define  FLASH_RDP_nRDP                      ((uint32_t)0x0000FF00)        /*!< Read protection complemented option byte */

/******************  Bit definition for FLASH_USER register  ******************/
#define  FLASH_USER_USER                     ((uint32_t)0x00FF0000)        /*!< User option byte */
#define  FLASH_USER_nUSER                    ((uint32_t)0xFF000000)        /*!< User complemented option byte */

/******************  Bit definition for FLASH_Data0 register  *****************/
#define  FLASH_Data0_Data0                   ((uint32_t)0x000000FF)        /*!< User data storage option byte */
#define  FLASH_Data0_nData0                  ((uint32_t)0x0000FF00)        /*!< User data storage complemented option byte */

/******************  Bit definition for FLASH_Data1 register  *****************/
#define  FLASH_Data1_Data1                   ((uint32_t)0x00FF0000)        /*!< User data storage option byte */
#define  FLASH_Data1_nData1                  ((uint32_t)0xFF000000)        /*!< User data storage complemented option byte */

/******************  Bit definition for FLASH_WRP0 register  ******************/
#define  FLASH_WRP0_WRP0                     ((uint32_t)0x000000FF)        /*!< Flash memory write protection option bytes */
#define  FLASH_WRP0_nWRP0                    ((uint32_t)0x0000FF00)        /*!< Flash memory write protection complemented option bytes */

/******************  Bit definition for FLASH_WRP1 register  ******************/
#define  FLASH_WRP1_WRP1                     ((uint32_t)0x00FF0000)        /*!< Flash memory write protection option bytes */
#define  FLASH_WRP1_nWRP1                    ((uint32_t)0xFF000000)        /*!< Flash memory write protection complemented option bytes */

/******************  Bit definition for FLASH_WRP2 register  ******************/
#define  FLASH_WRP2_WRP2                     ((uint32_t)0x000000FF)        /*!< Flash memory write protection option bytes */
#define  FLASH_WRP2_nWRP2                    ((uint32_t)0x0000FF00)        /*!< Flash memory write protection complemented option bytes */

/******************  Bit definition for FLASH_WRP3 register  ******************/
#define  FLASH_WRP3_WRP3                     ((uint32_t)0x00FF0000)        /*!< Flash memory write protection option bytes */
#define  FLASH_WRP3_nWRP3                    ((uint32_t)0xFF000000)        /*!< Flash memory write protection complemented option bytes */

#endif /* _FLASH_ */
