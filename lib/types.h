#ifndef _TYPES_
#define _TYPES_

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

typedef char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef long long int64_t;

typedef uint32_t size_t;

#ifndef __cplusplus
#undef bool
#undef true
#undef false

typedef _Bool bool;
#define false 0
#define true (!false)
#endif /* __cplusplus */

#ifndef NULL
#define NULL ((void*)0)
#endif


/* Some definitions for registers memory mapping. All just resolve to volatile. This way, allowed usage can be seen in the typedefs. HW_WO: Write only, HW_RO: Read only, HW_RW: Read/Write, HW_RS: Reserved (do not access), HW_UU: Unused (padding etc.) */

typedef volatile uint32_t HW_WO;
typedef volatile const uint32_t HW_RO;
typedef volatile uint32_t HW_RW;
typedef volatile uint32_t HW_RS;
typedef volatile uint32_t HW_UU;

#ifdef __cplusplus
  #define     __I     volatile           //!< defines 'read only' permissions
#else
  #define     __I     volatile const     //!< defines 'read only' permissions
#endif
#define     __O     volatile             //!< defines 'write only' permissions
#define     __IO    volatile             //!< defines 'read / write' permissions


#endif
