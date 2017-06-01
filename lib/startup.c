#include "types.h"
#include "utils.h"
// #include "stm32f10x.h"
#include "memorymap.h"
#include "rcc.h"
#include "flash.h"
#include "debug.h"
#include "systick.h"

void bootstrap(void);	//bootstrap that will call main later
void deadend(void);	//neverending handler

/* These variables are used to pass memory locations from the linker script to our code. */
extern uint8_t _LD_STACK_TOP;
extern uint8_t _LD_END_OF_TEXT;
extern uint8_t _LD_START_OF_DATA;
extern uint8_t _LD_END_OF_DATA;
extern uint8_t _LD_END_OF_BSS;

/* we define some standard handler names here - they all default to deadend but may be changed by
 * implementing a real function with that name. So if they are triggered but undefined, we'll just
 * stop. DEFAULT_IMP defines a weak alias. */

#define DEFAULTS_TO(func) __attribute__ ((weak, alias (#func)))

void main(void);// DEFAULTS_TO(deadend);
void systick_handler(void) DEFAULTS_TO(deadend);
// TODO: define more interrupt handlers here

/* The vector table - contains the initial stack pointer and pointers to boot code as well as
   interrupt and fault handler pointers. The processor will expect this to be located at address
   0x0, so we put it into a separate linker section. */
__attribute__ ((section(".vectors")))
const void* vtable[] = {
    &_LD_STACK_TOP,          //Stack top
    bootstrap,               //Reset
    deadend,                 //NMI
    deadend,                 //Hard fault
    deadend,                 //Memory management fault
    deadend,                 //Bus fault
    deadend,                 //Usage fault
    NULL,                    //RESERVED
    NULL,                    //RESERVED
    NULL,                    //RESERVED
    NULL,                    //RESERVED
    deadend,                 //SVCall
    deadend,                 //Reserved for Debug
    NULL,                    //RESERVED
    deadend,                 //PendSV handler
    systick_handler,         //The SysTick handler
    deadend,                 //IRQ0
    deadend,                 //IRQ1
    deadend,                 //IRQ2
    deadend,                 //IRQ3
    deadend,                 //IRQ4
    deadend,                 //IRQ5
    deadend,                 //IRQ6
    deadend,                 //IRQ7
    deadend,                 //IRQ8
    deadend,                 //IRQ9
    deadend,                 //IRQ10
    deadend,                 //IRQ11
    deadend,                 //IRQ12
    deadend,                 //IRQ13
    deadend,                 //IRQ14
    deadend,                 //IRQ15
    deadend,                 //IRQ16
    deadend,                 //IRQ17
    deadend,                 //IRQ18
    deadend,                 //IRQ19
    deadend,                 //IRQ20
    deadend,                 //IRQ21
    deadend,                 //IRQ22
    deadend,                 //IRQ23
    deadend,                 //IRQ24
    deadend,                 //IRQ25
    deadend,                 //IRQ26
    deadend,                 //IRQ27
    deadend,                 //IRQ28
    deadend,                 //IRQ29
    deadend,                 //IRQ30
    deadend,                 //IRQ31
    deadend,                 //IRQ32
    deadend,                 //IRQ33
    deadend,                 //IRQ34
    deadend,                 //IRQ35
    deadend,                 //IRQ36
    deadend,                 //IRQ37
    deadend,                 //IRQ38
    deadend,                 //IRQ39
    deadend,                 //IRQ40
    deadend,                 //IRQ41
    deadend,                 //IRQ42
    deadend,                 //IRQ43
    deadend,                 //IRQ44
    deadend,                 //IRQ45
    deadend,                 //IRQ46
    deadend,                 //IRQ47
    deadend,                 //IRQ48
    deadend,                 //IRQ49
    deadend,                 //IRQ50
    deadend,                 //IRQ51
    deadend,                 //IRQ52
    deadend,                 //IRQ53
    deadend,                 //IRQ54
    deadend,                 //IRQ55
    deadend,                 //IRQ56
    deadend,                 //IRQ57
    deadend,                 //IRQ58
    deadend,                 //IRQ59
    deadend,                 //IRQ60
    deadend,                 //IRQ61
    deadend,                 //IRQ62
    deadend,                 //IRQ63
    deadend,                 //IRQ64
    deadend,                 //IRQ65
    deadend,                 //IRQ66
    deadend,                 //IRQ67
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
}

void bootstrap(void)
{
    //copy initial values of variables (non-const globals and static variables) from FLASH to RAM
    uint8_t* mirror = &_LD_END_OF_TEXT; //copy from here
    uint8_t* ram = &_LD_START_OF_DATA;	//copy to here
    while (ram < (&_LD_END_OF_DATA)) *(ram++) = *(mirror++);

    //set uninitialized globals (and globals initialized to zero) to zero
    while (ram < (&_LD_END_OF_BSS)) *(ram++) = 0;

    systemInit();

    //jump into main user code (which should setup needed timers and interrupts or not return at all)
    main();

    //after main, sleep until an interrupt occurs
    while (true) {
        waitForInterrupt();
    }
}

/* default handler for unimplemented interrupts or faults. Stay here for debugger to pick up */
void deadend(void) {
    while(1);
}
