//-----------------------------------------------------------------------------
// Platform-specific functions and macros

#pragma once

void SetAffinity ( int cpu );

#ifndef __x86_64__
 #if defined(__x86_64) || defined(_M_AMD64) || defined(_M_X64)
  #define  __x86_64__
 #endif
#endif

#ifndef HAVE_INT64
 #if (__WORDSIZE >= 64) || defined(HAVE_SSE42)
  #define HAVE_INT64
 #endif
#endif

//-----------------------------------------------------------------------------
// Microsoft Visual Studio

#if defined(_MSC_VER)

#define FORCE_INLINE	__forceinline
#define	NEVER_INLINE  __declspec(noinline)

#include <stdlib.h>
#include <math.h>   // Has to be included before intrin.h or VC complains about 'ceil'
#include <intrin.h> // for __rdtsc
#include <stdint.h>

#define ROTL32(x,y)	_rotl(x,y)
#define ROTL64(x,y)	_rotl64(x,y)
#define ROTR32(x,y)	_rotr(x,y)
#define ROTR64(x,y)	_rotr64(x,y)

#pragma warning(disable : 4127) // "conditional expression is constant" in the if()s for avalanchetest
#pragma warning(disable : 4100)
#pragma warning(disable : 4702)

#define BIG_CONSTANT(x) (x)

// Read Time Stamp Counter
#define rdtsc() __rdtsc()

#define popcount8(x)  __popcnt(x)

//-----------------------------------------------------------------------------
// Other compilers

#else	//	!defined(_MSC_VER)

#include <stdlib.h>
#include <stdint.h>

#define	FORCE_INLINE inline __attribute__((always_inline))
#define	NEVER_INLINE __attribute__((noinline))

#ifdef HAVE_BIT32
#define popcount8(x) __builtin_popcountll(x)
#else
#define popcount8(x) __builtin_popcountl(x)
#endif

inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
  return (x << r) | (x >> (32 - r));
}

inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
  return (x << r) | (x >> (64 - r));
}

inline uint32_t rotr32 ( uint32_t x, int8_t r )
{
  return (x >> r) | (x << (32 - r));
}

inline uint64_t rotr64 ( uint64_t x, int8_t r )
{
  return (x >> r) | (x << (64 - r));
}

#define	ROTL32(x,y)	rotl32(x,y)
#define ROTL64(x,y)	rotl64(x,y)
#define	ROTR32(x,y)	rotr32(x,y)
#define ROTR64(x,y)	rotr64(x,y)

#if __WORDSIZE == 64
#define BIG_CONSTANT(x) (x##UL)
#else
#define BIG_CONSTANT(x) (x##ULL)
#endif

__inline__ unsigned long long int rdtsc()
{
#ifdef __x86_64__
    unsigned int a, d;
    __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
    return (unsigned long)a | ((unsigned long)d << 32);
#elif defined(__i386__)
    unsigned long long int x;
    __asm__ volatile ("rdtsc" : "=A" (x));
    return x;
#else
#define NO_CYCLE_COUNTER
    return 0;
#endif
}

#include <strings.h>
#define _stricmp strcasecmp

#endif	//	!defined(_MSC_VER)

//-----------------------------------------------------------------------------

#ifndef __WORDSIZE
# ifdef HAVE_BIT32
#  define __WORDSIZE 32
# else
#  define __WORDSIZE 64
# endif
#endif
