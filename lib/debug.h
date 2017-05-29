/**
 * Contains debug functions, such as printing text to the debugger console and
 * other semihosting goodies.
 */

#ifndef _DEBUG_
#define _DEBUG_

#include "types.h"

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
void write(uint32_t fhandle, const void *s, size_t len);

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
 */
void die(const char *s);

#endif
