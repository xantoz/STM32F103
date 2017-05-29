.equ DHCSR, 0xE000EDF0          // Debug Halting and Control Register

// ASM stub to the semihosting interface
.global send_command
send_command:
     ldr r2, =DHCSR             // Load the address to DHCSR
     ldr r2, [r2]               // Load the contents of DHCSR
     mov r3, #0x01              // Thumb16 doesn't support TST with immediate
     tst r2, r3                 // Check the lowest bit (C_DEBUGEN)
     beq ret                    // Return if it wasn't set (ALU flag Z == 0)
     bkpt #0xAB
ret: bx lr
