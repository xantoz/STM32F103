.global delay_us

// TODO: copy this function to RAM on startup for more predictable execution? (need to add section for functions like this to linkscript)

// Source for the cycle counts of instructions: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0337h/CHDDIGAC.html

delay_us:                 // Cycle count (dumb: ignore potential pipeline effects, assume branches always take 2 cycles when taken)
    cpsie i               // 1 [disable irq]

    // Load g_clock.sysclkFreq to r1
    ldr r1,=g_clock       // 2
    ldr r1, [r1, #0]      // 2 [sysclkFreq is the zeroth element of g_clock]

    // Approximate g_clock.sysclkFreq/1000000 with
    // (g_clock.sysclkFreq*17) >> 24, which has the nice property of
    // running in constant, predictable, time, as well as
    // being accurate (no difference from a div) for the usual values of sysclkFreq.
    // Note that we assume that sysclkFreq is never going to be lower than 1 MHz (because otherwise we risk bottoming out at 0)
    mov r2, #17           // 1
    mul r1,r1,r2          // 1
    lsr r1,#24            // 1

    mul r0,r0,r1          // 1 [r0 = us * sysclkFreq_MHz]
    sub r0, #(10+4+(3-1)) // 1 [subtract cycles up to and including this one + cycles before the loop + cycles after the loop (3-1, because branch before 2: was not taken)]

    asr r0, #2            // 1 [divide by number of cycles taken up by one spin of the loop == 4]
    cmp r0, #0            // 1
    ble 2f                // 1<usu> (not taken), 2-4 when taken (assume 2) (means we delay an extra cycle when a too small count was sent in r0)
    nop                   // 1 [Use same amount of cycles whether or not the branch above was taken]
1:
    sub r0, #1            // 1
    cmp r0, #0            // 1
    bne 1b                // 2<usu> (2-4), 1 when not taken
2:
    cpsid i               // 1 [enable irq]
    bx lr                 // 2<usu> (2 to 4)