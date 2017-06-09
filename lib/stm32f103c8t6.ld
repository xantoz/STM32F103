/* Simple linker script for LPC1343 */

/* MEMORY describes the device's memory blocks and their addresses */

/* TODO: consider having stack below data */

MEMORY {
  flash (rx)  : ORIGIN = 0x08000000, LENGTH = 128K /*(UP TO 128K) TODO: how much flash do we have actually? */
  ram   (rwx) : ORIGIN = 0x20000000, LENGTH =  20K
}

/* Start of stack (TODO: how much SRAM do we have, actually?) */
/* _LD_STACK_TOP = 0x20001ff0;  */
_estack = 0x20005000;

/*
** Peripherals
*/
SRAM_BASE      = ORIGIN(ram); /*!< SRAM base address in the alias region */
PERIPH_BASE    = 0x40000000;  /*!< Peripheral base address in the alias region */

SRAM_BB_BASE   = 0x22000000; /*!< SRAM base address in the bit-band region */
PERIPH_BB_BASE = 0x42000000; /*!< Peripheral base address in the bit-band region */

FSMC_R_BASE    = 0xA0000000;    /*!< FSMC registers base address */

/*!< Memory mapping of Cortex-M3 Hardware */
SCS_BASE  = 0xE000E000;           /*!< System Control Space Base Address */
ITM_BASE  = 0xE0000000;           /*!< ITM Base Address */
CoreDebug = 0xE000EDF0;           /*!< Core Debug Base Address */
SysTick   = (SCS_BASE + 0x0010);  /*!< SysTick Base Address */
NVIC      = (SCS_BASE + 0x0100);  /*!< NVIC Base Address */
SCB       = (SCS_BASE + 0x0D00);  /*!< System Control Block Base Address */

/*!< Peripheral memory map */
APB1PERIPH_BASE = (PERIPH_BASE);
APB2PERIPH_BASE = (PERIPH_BASE + 0x10000);
AHBPERIPH_BASE  = (PERIPH_BASE + 0x20000);

AFIO = (APB2PERIPH_BASE + 0x0000);
EXTI = (APB2PERIPH_BASE + 0x0400);

FLASH_R_BASE = (AHBPERIPH_BASE + 0x2000); /*!< Flash registers base address */
FLASH        = FLASH_R_BASE;              /*!< Flash registers */
OB_BASE      = 0x1FFFF800;                /*!< Flash Option Bytes base address */
OB           = 0x1FFFF800;                /*!< Flash Option Bytes */

GPIOA = (APB2PERIPH_BASE + 0x0800);
GPIOB = (APB2PERIPH_BASE + 0x0C00);
GPIOC = (APB2PERIPH_BASE + 0x1000);

SPI1 = (APB2PERIPH_BASE + 0x3000);
SPI2 = (APB1PERIPH_BASE + 0x3800);
SPI3 = (APB1PERIPH_BASE + 0x3C00);

RCC = AHBPERIPH_BASE + 0x1000;

/* SECTIONS lists where various parts data and code should go.
   Since they all just relate to the addressed defined above,
   porting code to other MCUs usually does not requre changes. */

SECTIONS {

    /* vector table must be located at beginning of flash,
     * so we assign it to a separate linker section.
     * It contains stack top address, start code and
     * interrupt handler pointers.  */
    .isr_vector : {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >flash

    /* .text section contains code and readonly (const) globals */
    .text : {
         . = ALIGN(4);
        *(.text)
        *(.text.*)
        *(.rodata)                 /* read-only data (constants) */
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
        . = ALIGN(4);
        _etext = .;
        _sidata = _etext; /* Start of initial values of .data stored in flash*/
    } >flash

    /* data section contains read-write (non-const) globals and static variables
     * initialized to a value other than zero. The addresses are mapped to RAM
     * (the code needs to write to it), but initial values put to a mirror region
     * in FLASH (because we need to have the values at startup). Bootstrap code
     * will copy the values from FLASH to RAM. Note that bootstrap code wants this
     * section to follow the text section directly. */
    .data : AT(_sidata) {
         . = ALIGN(4);
        _sdata = .;     /* Remember position (=start of initialized RAM) */
        KEEP(*(.data))
        KEEP(*(.data.*))
        . = ALIGN(4);
        _edata = .;     /* Remember position (=end of initialized RAM) */
    } >ram

    /* bss section contains globals and static variables either initialized to zero
     * or not initialized at all. These are not mirrored in FLASH. Their memory
     * needs to be zeroed at startup. We'll clear it in the bootstrap code. Note that
     * bootstrap code wants this section to follow the data section directly. */
    .bss : {
        _sbss = .;     /* Remember position (=start of uninitialized RAM) */
        *(.bss)
        _ebss = .;     /* Remember position (=end of uninitialized RAM) */
    } >ram
}
