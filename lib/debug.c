#include "debug.h"

#include "utils.h"
#include "string.h"

// Semihosting commands
#define SYS_WRITE  0x05
#define SYS_WRITE0 0x04
#define SYS_WRITEC 0x03
#define SYS_SYSTEM 0x12
#define SYS_TIME   0x11
#define SYS_CLOCK  0x10
#define SYS_READC  0x07

// static uint32_t send_command(register uint32_t command, register void *message)
// {
//     register uint32_t r0 asm("r0") = command;
//     register void *r1 asm("r1") = message;
//     __asm__ volatile ( "bkpt #0xAB;"
//                        : "+r" (r0)
//                        : "r" (r1)
//                        : "memory" );
//     return r0;
// }

extern uint32_t send_command(uint32_t command, void *message);

void put_char(char c)
{
    send_command(SYS_WRITEC, &c);
}

char read_char()
{
    return (char)send_command(SYS_READC, 0);
}

void write(uint32_t fhandle, void const * const s, size_t len)
{
    uint32_t m[] = { fhandle, (uint32_t)s, len/sizeof(char) };
    send_command(SYS_WRITE, m);
}

void print(const char *s)
{
    // writing to stdout doesn't seem to work, we need stderr
    // send_command(SYS_WRITE0, (void*)s);
    write(2/*stderr*/, s, strlen(s));
}

void print_hex(const uint32_t val)
{
    static char const * const hexLut = "0123456789ABCDEF";
    static const size_t nibbles = 2*sizeof(uint32_t); // = 8
    char str[2 + nibbles + 1]; // '0x' + 8 nibbles + newline
    str[0] = '0'; str[1] = 'x';
    for (uint32_t i = 0; i < nibbles; ++i)
    {
        str[2 + (nibbles-1) - i] = hexLut[(val >> i*4) & 0x0f];
    }
    str[2 + nibbles] = '\n';

    write(2/*stderr*/, str, sizeof(str));
}

void system(const char *cmd)
{
    uint32_t m[] = { (uint32_t)cmd, strlen(cmd)/sizeof(char) };
    send_command(SYS_SYSTEM, m);
}

uint32_t time()
{
    return send_command(SYS_TIME, NULL);
}

int32_t clock()
{
    uint32_t result = send_command(SYS_CLOCK, 0);
    return *((int32_t*)(&result));
}

// TODO: cause hardfault (or other exception) instead to let an overridable handler dcide what to
// do? Or perhaps just a custom handler Die_Handler?
// TODO: do we need to enable debug before we BKPT, or indeed before we BKPT 0xAB?
void die(const char *s)
{
#ifdef __GNUC__
    print("Caller address: ");
    print_hex((uint32_t)__builtin_return_address(0));
#endif
    print(s);
    put_char('\n');
    while (1)
        BKPT();
}
