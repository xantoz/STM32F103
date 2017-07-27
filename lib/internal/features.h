/**
 * @brief A collection of auto-detecting defines used for conditional compilation
 *
 * @note Must contain defines only. This files is might also be included in assembly source files
 *       that need to conditionally include code.
 *
 * @file features.h
 */
#ifndef _INTERNAL_FEATURES_
#define _INTERNAL_FEATURES_

#if defined(__GNUC__) || defined(__CLANG__)
#define HAVE_GCC_EXTENDED_ASM 1
#endif

#if defined(__GNUC__) || defined(__CLANG__) || defined(__ARMCC_VERSION__)
#define HAVE_GCC_STYLE_ATTRIBUTES 1
#endif

#endif /* _INTERNAL_FEATURES_ */
