#pragma once

/*
   (c) by dbj@dbj.org CC BY SA 4.0

   this was common.h but that is too dangerous and common name :wink:
   important: this whole repo is compiled on windows with clang
   and most importaqntly: with NO std argument.

   Thus, the language is whatever clang decided it is ...

*/
#ifndef __clang__
#error CLANG Please
#endif

#ifdef __cpluplus
#define DBJ_EXTERN_C_BEGIN extern "C" {
#define DBJ_EXTERN_C_END   }
#else   // not C++
#define DBJ_EXTERN_C_BEGIN
#define DBJ_EXTERN_C_END
#endif

// count of on native arrays is not always possible
// clang will warn you
#define DBJ_COUNTOF(a) (sizeof(a) / sizeof(0 [a]))

#undef DBJ_COMMON_API
// be warry of : __inline , it can bloat the code considerably
// #define DBJ_COMMON_API __inline
#define DBJ_COMMON_API static inline
// #define DBJ_COMMON_API static

#pragma clang system_header

//------------------------------------------------------------------------------
#undef DBJ_WIN
#define MYPREFIX DBJ_
#if defined(_WIN32) || defined(_WIN64)
#define DBJ_WIN 1
#else
#define DBJ_WIN 0
#endif
//------------------------------------------------------------------------------

// https://pvs-studio.com/en/blog/posts/k0040/
#if DBJ_WIN
#define WIN32_LEAN_AND_MEAN
#include <crtdbg.h>
#include <windows.h>

// non destructive assert, why haven't you thought of this before?
// in release build it does not remove its argument
#ifdef _DEBUG
// carefull: _ASSERTE requires standard cl.exe runtime in a DLL
// iut does not work in static libs
#define dbj_assert(x_) assert(x_)
#else
#define dbj_assert(x_) (__typeof(x_)) (x_)
#endif
#else   // no WIN API
#include <assert.h>
#ifdef _DEBUG
#define dbj_assert(x_) assert(x_)
#else
#define dbj_assert(x_) (__typeof(x_)) (x_)
#endif
#endif   // no WIN API

// #define __STDC_WANT_LIB_EXT1__ 1
// Annex K? Do not do this, here is why
// https://www.reddit.com/r/C_Programming/comments/gp2uh5/comment/frjgn1x/?utm_source=share&utm_medium=web2x&context=3

#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>   // For random(), RAND_MAX
#include <time.h>

// poor man's logging
// iuf you know how to redirect the stderr to a file it will work
// behind a gui too
#define dbj_err_log(...) fprintf(stderr, __VA_ARGS__)

#if __cplusplus >= 202002L
// C++20 (and later) code
#define CONSTEXPR constexpr
#else   // lower than C++20
#define CONSTEXPR
#endif

#undef DBJ_HERE
#undef DBJ_FX
#define DBJ_HERE     dbj_err_log("\nLine : %6d", __LINE__)
#define DBJ_FX(F, X) dbj_err_log("\n%-6d %s : " F, __LINE__, (#X), (X))

#undef MALLOC
#define MALLOC(SIZE_) calloc(1, SIZE_)

//------------------------------------------------------------------------------
// NOTE we have functions from this point onward
DBJ_EXTERN_C_BEGIN

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/memccpy.html
// https://en.cppreference.com/w/c/string/byte/memccpy
// we are inside clang and we know memccpy is a "builtin", thus we
// simply declare it here; the POSIX name bellow is required by clang,
extern void *_memccpy(void *, const void *, const int, const size_t);

// hides the rand seed init and makes sure it is always done on time
__attribute__((constructor)) DBJ_COMMON_API void
init_random(void) {
    srand(time(NULL));
}

// this produces a biased result, but in 99% use cases, who cares
#undef DBJ_RANDINRANGE
#define DBJ_RANDINRANGE(max, min) ((rand() % (max + 1 - min)) + min)

// https://stackoverflow.com/a/6852396/10870835
// Assumes 0 <= max <= RAND_MAX
// Returns in the closed interval [0, max]
// be warry of : __inline , it can bloat the code considerably
DBJ_COMMON_API long
random_at_most(long max) {
    unsigned long
        // max <= RAND_MAX < ULONG_MAX, so this is okay.
        num_bins = (unsigned long) max + 1,
        num_rand = (unsigned long) RAND_MAX + 1, bin_size = num_rand / num_bins,
        defect = num_rand % num_bins;

    long x;
    do {
        //    x = random();
        x = rand();
    }
    // This is carefully written not to overflow
    while (num_rand - defect <= (unsigned long) x);

    // Truncated division is intentional
    return x / bin_size;
}

// nicked from https://github.com/skeeto/w64devkit/blob/master/src/alias.c
// this is made for _WIN64
//------------------------------------------------------------------------------
#if DBJ_WIN

#define LSTR(s) XSTR(s)
#define XSTR(s) L## #s
#define LEXE    LSTR(EXE)
#define LCMD    LSTR(CMD)

// of course you can increase
#define DBJ_MAX_STRLEN 0xFFFF

// with max len sanity check in a debug mode
DBJ_COMMON_API size_t
xstrlen(WCHAR *s) {
    size_t n = 1;
    while (*s++) {
        n++;
        dbj_assert(!(DBJ_MAX_STRLEN > n));
    }
    return n;
}

DBJ_COMMON_API void
xmemcpy(void *dst, void *src, size_t len) {
    unsigned char *d = dst, *s = src;
    for (size_t i = 0; i < len; i++)
        d[i] = s[i];
}

/* Find the end of argv[0]. */
DBJ_COMMON_API WCHAR *
endof_argv_0(WCHAR *s) {
    if (s[0] == 34) {
        /* quoted argv[0] */
        for (s++;; s++) {
            switch (*s) {
            case 0:
                return s;
            case 34:
                return s + 1;
            }
        }
    } else {
        /* unquoted argv[0] */
        for (;; s++) {
            switch (*s) {
            case 0:
            case 9:
            case 32:
                return s;
            }
        }
    }
}

/* Find the final file component. */
DBJ_COMMON_API WCHAR *
filename_last(WCHAR *s) {
    for (WCHAR *r = s;; s++) {
        switch (*s) {
        case 0:
            return r;
        case 47:
        case 92:
            r = s + 1;
        }
    }
}

// universal SEH filter logger
/*
codes are in winnt.h
int main()
{
    __try {
        // SEH raised
    }
    __except(dbj_filter_seh(GetExceptionCode(), GetExceptionInformation())) {
        dbj_err_log("SEH caught");
    }
*/
int
dbj_filter_seh(int code, PEXCEPTION_POINTERS ex) {
    dbj_err_log("SEH Filtering, code: %d ", code);
    return EXCEPTION_EXECUTE_HANDLER;
}

//------------------------------------------------------------------------------
#endif   // DBJ_WIN

DBJ_EXTERN_C_END
