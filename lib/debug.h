/**
 * Contains debug functions, such as printing text to the debugger console and
 * other semihosting goodies.
 */

#ifndef _DEBUG_
#define _DEBUG_

#include "types.h"
#include "vfunc.h"
#include "utils.h"

/**
 * @brief Output a single character to the debug console.
 */
void put_char(char c);

/**
 * @brief Read a single character
 */
char read_char();

/**
 * @brief Write len bytes from data to file handle fhandle on host system.
 *        fhandle = 1 can be used for stdout, fhandle = 2 for stderr.
 *
 * @params fhandle [in]: File handle
 *         s [in]: Pointer to memory
 *         len [in]: Length of string
 */
void write(uint32_t fhandle, void const * const s, size_t len);

/**
 * @brief Print a string to the debug console.
 *
 * @params s [in]: Null-terminated string
 */
void print(const char *s);

/**
 * @brief Prints a uint32_t in hexadecimal to the debug console.
 */
void print_u32_hex(const uint32_t val);

/**
 * @brief Prints a uint32_t in hexadecimal, followed by a newline, to the debug console.
 */
void println_u32_hex(const uint32_t val);

/**
 * @brief Prints a uint32_t in decimal to the debug console.
 */
void print_u32_dec(const uint32_t val);

/**
 * @brief Prints a uint32_t in decimal, followed by a newline, to the debug console.
 */
void println_u32_dec(const uint32_t val);

/**
 * @brief Execute command in host system shell.
 *
 * @params cmd [in]: Null-terminated string
 */
void system(const char *cmd);

/**
 * @brief Get the time as a unix timestamp from the host system.
 */
uint32_t time();

/**
 * @brief Get the number of centiseconds since execution started
 */
int32_t clock();

/**
 * @brief Print error message to the debug console, then die (enter eternal spin-loop of doom)
 *
 * @params s [in]: Null-terminated string
 *
 * @todo: Have a macro define/setting where this does not take any string, and arguments passed to
 *        it are thrown away (do: #define die(s) die()). The idea is that it can be turned on to
 *        save some space (no string literals) for release builds.
 */
__attribute__((noreturn))
void die(const char *s);

#define static_assert _Static_assert

#ifdef __GNUC__
/**
 * @brief Build time assert if condition is able to be promoted to constant (e.g. function arguments
 *        in function marked inline)
 *
 * A lax alternative to static_assert that works even with non-static arguments. (such as arguments
 * from functions) given to it. Only performs the assertion if it is possible to promote the value
 * to constant at compile time.
 *
 * A typical use case is within functions marked inline (used instead of macros), so that their
 * arguments can be checked for errors at build time, much like compiler warnings for bad format
 * strings etc. (unless, of course, they have been called with arguments that are not resolveable at
 * build time).
 *
 * @todo TODO: Maybe this should be called build_assert_warning or static_warning instead, and just
 *             emit warnings. Although then we need to provide a definition of the function carrying
 *             the attribute, whose call is not optimized out by the compiler (it *should* on the
 *             other hand be optimized out when not called)
 */
#define build_assert(...) VFUNC(__build_assert, __VA_ARGS__)
#define __build_assert2(cond, text) __build_assert_impl(cond, text, __COUNTER__)
#define __build_assert1(cond) __build_assert_impl(cond, #cond " @ "  __FILE__ ":" TOSTRING(__LINE__), __COUNTER__)

#define __build_assert_impl(cond, text, cntr)                           \
    do {                                                                \
        __attribute__((error("BUILD ASSERT:" text)))                    \
        void TOKENPASTE(__build_assert_error, cntr)(void);              \
                                                                        \
        if (__builtin_constant_p((cond)) && !(cond))                    \
            TOKENPASTE(__build_assert_error, cntr)();                   \
    } while (0)
#else
// Implementations of build_assert for other compilers can be put here
#warning "build_assert not implemented for this compiler, all checks will silently pass"
#define build_assert(...)
#endif /* __GNUC__ */

#if 0
__attribute__((error("ASSERTION ALWAYS FALSE")))
void __compiletime_assert_error(void);

// TODO: have the compile time check for regular assert even when we have NDEBUG
#define __assert_impl(cond, text)                                       \
    do {                                                                \
        if (__builtin_constant_p(cond) && !(cond))                      \
            __compiletime_assert_error();                               \
        if (!(cond))                                                    \
            die((text));                                                \
    } while (0)
#else
#define __assert_impl(cond, text) do { if (!(cond)) die((text)); } while (0)
#endif

/**
 * @brief Variant of assert that is not affected by whether NDEBUG is defined or not
 */
#define assert_always(...) VFUNC(__assert_always, __VA_ARGS__)
#define __assert_always2(cond, text) __assert_impl((cond), (text))
#define __assert_always1(cond) __assert_always2((cond), #cond " @ "  __FILE__ ":" TOSTRING(__LINE__))

#ifndef NDEBUG
#define assert(...) assert_always(__VA_ARGS__)
#else
#define assert(...)
#endif

#endif
