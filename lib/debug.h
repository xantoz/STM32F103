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
 * @brief Prints a uint32_t in hexadecimal, followed by a newline, to the debug console.
 */
void print_hex(const uint32_t val);

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
void die(const char *s);

#define static_assert _Static_assert

#ifdef __GNUC__
__attribute__((error("ASSERTION ALWAYS FALSE")))
void __build_assert_error(void);

// TODO: Kind of want to break this out as an alternative to static_assert (when we want to check
// something at build-time, but only when possible). Problem: Difficult outside GCC. Perhaps call it
// static_warning, or build_warning?
#define __build_assert(cond,text)                   \
    do {                                            \
        if (__builtin_constant_p(cond) && !(cond))  \
            __build_assert_error();                 \
    } while (0)

// TODO: have the compile time check for regular assert even when we have NDEBUG
#define __assert_impl(cond, text)                                       \
    do {                                                                \
        __build_assert((cond),(text));                                  \
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
