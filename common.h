#pragma once


#ifndef __clang__
#error CLANG Please
#endif

#if __cplusplus >= 202002L
// C++20 (and later) code
#define CONSTEXPRETVAL constexpr
#else // lower than C++20
#define CONSTEXPRETVAL
#endif

#undef DO_PRAGMA_
#undef DO_PRAGMA
// Using this you can create shorthands for various pragmas like this
// #define POISON(name) DO_PRAGMA(GCC poison name)
// POISON(puts) // becomes _Pragma("GCC poison puts")
#define DO_PRAGMA_(x) _Pragma (#x)
#define DO_PRAGMA(x) DO_PRAGMA_(x)

#undef STR_
#undef DEFER_
#undef XPND_
#undef DBJ_ERM

#define XPND_(X) X
#define STR_(X) #X
#define DEFER_(M,...) M(__VA_ARGS__)
// #define DBJ_ERM(X) _Pragma(STR_(GCC error(X " at line " DEFER_(STR_,__LINE__))))

// The following will produce warning messages
// #pragma message "some DBJ diagnostic message"
// #pragma GCC warning "DBJ TODO: replace deprecated feature"
// The following will produce an error message and stop the compilation
// #pragma GCC error "DBJ Not supported"

#ifndef __cplusplus
#if __STDC_VERSION__  < 201710L
// DBJ_ERM("ISO C 17 or better please, __STD_VERSION__ is: " DEFER_(STR_,__STD_VERSION__))
// #error "ISO C 17 or better please"
#endif
#endif //  __cplusplus ?

#pragma clang system_header

// #define __STDC_WANT_LIB_EXT1__ 1
// Annex K? Do not do this, here is why
// https://www.reddit.com/r/C_Programming/comments/gp2uh5/comment/frjgn1x/?utm_source=share&utm_medium=web2x&context=3

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h> // For random(), RAND_MAX
#include <memory.h>
#include <assert.h>

#undef HERE
#undef FX

#define HERE printf("\nLine : %6d", __LINE__)
#define FX(F, X) printf("\n%-6d %s : " F, __LINE__, (#X), (X))

#undef MALLOC
#define MALLOC(SIZE_) calloc(1,SIZE_)

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/memccpy.html
// https://en.cppreference.com/w/c/string/byte/memccpy
// we are inside clang and we know memccpy is a "builtin", thus we
// simply declare it here; the POSIX name bellow is required by clang, 
extern void *_memccpy(void *, const void *, const int, const size_t );


__attribute__((constructor))
__inline void init_random(void)
{
    srand(time(NULL));
}

// this produces a biased result
#define RANDINRANGE(max,min) ((rand() % (max + 1 - min)) + min)

// https://stackoverflow.com/a/6852396/10870835
// Assumes 0 <= max <= RAND_MAX
// Returns in the closed interval [0, max]
__inline long random_at_most(long max) {
  unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
//    x = random();
   x = rand();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (unsigned long)x);

  // Truncated division is intentional
  return x/bin_size;
}
