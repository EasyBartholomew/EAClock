#ifndef _LOCAL_DEF_H_
#define _LOCAL_DEF_H_

#ifdef ATMEGA32
#include <stdint-gcc.h>

#define ARCH_MAX 0x08
typedef uint8_t archb_type;

typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;

typedef signed char s8_t;
typedef int16_t s16_t;
typedef int32_t s32_t;
typedef int64_t s64_t;

typedef u8_t l_t;
typedef float d32_t;

#define FALSE 0
#define TRUE 1

typedef s64_t tu_t;
typedef u16_t msize_t;
typedef u8_t fsize_t;


#elif defined WIN32

typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long long u64_t;

typedef signed char s8_t;
typedef signed short s16_t;
typedef signed int s32_t;
typedef signed long long s64_t;

typedef float d32_t;

#else
#error NO_PLATFORM_DEFINED
#endif


#endif // !_LOCAL_DEF_H_
