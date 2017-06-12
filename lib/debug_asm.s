.syntax unified

.equ DHCSR, 0xE000EDF0          // Debug Halting and Control Register

// ASM stub to the semihosting interface
// Returns without doing anything, unless in debug mode (C_DEBUGEN bit set in DHCSR)
.section .text
.global send_command
send_command:
     ldr r2, =DHCSR             // Load the address to DHCSR
     ldr r2, [r2]               // Load the contents of DHCSR
     tst r2, #0x01              // Check the lowest bit (C_DEBUGEN)
     beq 1f                     // Return if it wasn't set (ALU flag Z == 0)
     bkpt #0xAB
1:   bx lr
