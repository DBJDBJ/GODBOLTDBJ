/*

SCROLL DOWN FOR RESULTS!!

*/
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int clockid_t;

#include <https://raw.githubusercontent.com/sheredom/ubench.h/master/ubench.h>

#define FOR(K, S) for (unsigned int K; K < S; K++)

#define PV(F, X) printf("\n#%-4d %16s : " F, __LINE__, #X, (X))



/////////////////////////////////////////////////////
int original_string_compare(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

/////////////////////////////////////////////////////
int optimized_string_compare(const char *str1, const char *str2) {
    for (; *str1 == *str2; ++str1, ++str2) {
        if (*str1 == '\0') {
            return 0;
        }
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}
/////////////////////////////////////////////////////
#define OPTIMIZED_STRING_COMPARE(str1, str2, result)                  \
    do {                                                              \
        const char *_s1 = (str1);                                     \
        const char *_s2 = (str2);                                     \
        while (*_s1 == *_s2) {                                        \
            if (*_s1 == '\0') {                                       \
                result = 0;                                           \
                break;                                                \
            }                                                         \
            ++_s1;                                                    \
            ++_s2;                                                    \
        }                                                             \
        if (result != 0) {                                            \
            result = (*(unsigned char *)_s1 - *(unsigned char *)_s2); \
        }                                                             \
    } while (0)

/////////////////////////////////////////////////////
static const char *string1 =
    "appleappleappleappleappleappleappleappleappleappleappleappleappleappleappl"
    "eappleappleappleappleapple";
static const char *string2 =
    "apricotapricotapricotapricotapricotapricotapricotapricotapricotapricotapri"
    "cotapricotapricotapricotapricot";
static const int LOOP_SIZE = 10000000;

UBENCH(original, compare) {
    for (int i = 0; i < LOOP_SIZE; ++i) {
        int dumsy = original_string_compare(string1, string2);
        dumsy = 0;
    }
}

UBENCH(optimized, compare) {
    for (int i = 0; i < LOOP_SIZE; ++i) {
        int dumsy = optimized_string_compare(string1, string2);
        dumsy = 0;
    }
}

UBENCH(original, optimized_macro_compare) {
    for (int i = 0; i < LOOP_SIZE; ++i) {
        int dumsy = 0;
        OPTIMIZED_STRING_COMPARE(string1, string2, dumsy);
        dumsy = 0;
    }
}

UBENCH_MAIN();
/*
[==========] Running 3 benchmarks.
[ RUN ] original.compare
[ OK ] original.compare (mean 17.239ms, confidence interval +- 5.163851%)
[ RUN ] optimized.compare
[ OK ] optimized.compare (mean 23.400ms, confidence interval +- 9.309059%)
[ RUN ] original.optimized_macro_compare
[ OK ] original.optimized_macro_compare (mean 23.735ms, confidence interval +- 10.664262%)
[==========] 3 benchmarks ran.
[ PASSED ] 3 benchmarks.

Thank you for providing the benchmark results. Let's analyze these results:

1. original.compare:
   - Mean execution time: 17.239ms
   - Confidence interval: ±5.163851%

2. optimized.compare:
   - Mean execution time: 23.400ms
   - Confidence interval: ±9.309059%

3. original.optimized_macro_compare:
   - Mean execution time: 23.735ms
   - Confidence interval: ±10.664262%

Observations:

1. Surprisingly, the original implementation performs best, with the lowest mean execution time of 17.239ms.
2. Both the "optimized" function and the macro-based implementation perform worse than the original, with mean execution times of 23.400ms and 23.735ms respectively.
3. The confidence intervals for the optimized versions are larger, indicating more variability in their performance.

Possible explanations:

1. Compiler optimizations: The compiler might be applying more effective optimizations to the simpler, original implementation.
2. Branch prediction: The original implementation's while loop condition might be easier for the CPU to predict, leading to better performance.
3. Memory access patterns: The original implementation's straightforward approach might result in more cache-friendly memory access patterns.
4. Macro overhead: The macro-based implementation, despite being intended as an optimization, might introduce some overhead due to its more complex structure.
5. Test data characteristics: The specific test strings used might favor the original implementation. Different string patterns could yield different results.

Recommendations:

1. Profile the code to understand where exactly the time is being spent in each implementation.
2. Test with different string patterns and lengths to see if the results are consistent across various inputs.
3. Examine the assembly output to understand how the compiler is optimizing each version.
4. Consider other optimization techniques, such as SIMD instructions for parallel comparison of multiple characters.
5. Ensure that the benchmark is accurately measuring what you intend, and that external factors are not influencing the results.

Remember that micro-optimizations like these can be highly dependent on the specific hardware, compiler, and use case. 
Always profile in your target environment with realistic data to make informed optimization decisions.


*/
