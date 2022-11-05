#define USE_UBENCH
#ifdef USE_UBENCH
#define UBENCH_IMPLEMENTATION
#include "../ubut/ubench.h"
#endif

#include "../common.h"

// HT count for both C and C++
// this yield very high number --> 
// #define MV_LEFT 22
#define MV_LEFT 6
// #define MV_LEFT 2
#define HT_COUNT (1 << MV_LEFT)

//----------------------------------------------------------------------------
// https://gist.github.com/skeeto/8e7934318560ac739c126749d428a413
//----------------------------------------------------------------------------
// C hash table benchmarks
// Using an integer hash:
//   $ cc -O3 bench.c
// Using AES-NI:
//   $ cc -O3 -maes bench.c
#include <assert.h>
#include <stdint.h>
#include <string.h>
#if __amd64__ && __AES__
#  include <immintrin.h>
#  include <wmmintrin.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct val { char s[8]; };

static struct val
make(int x)
{
    struct val v = {{0}};
    int len = 1 + (x>9) + (x>99) + (x>999) + (x>9999) + (x>99999) +
              (x>999999) + (x>9999999);
    switch (len) {
        case 8: v.s[7] = '0' + x%10; x /= 10;  // fallthrough
        case 7: v.s[6] = '0' + x%10; x /= 10;  // fallthrough
        case 6: v.s[5] = '0' + x%10; x /= 10;  // fallthrough
        case 5: v.s[4] = '0' + x%10; x /= 10;  // fallthrough
        case 4: v.s[3] = '0' + x%10; x /= 10;  // fallthrough
        case 3: v.s[2] = '0' + x%10; x /= 10;  // fallthrough
        case 2: v.s[1] = '0' + x%10; x /= 10;  // fallthrough
        case 1: v.s[0] = '0' + x%10;
    }
    return v;
}

static uint64_t
hash(struct val v)
{
    #if __amd64__ && __AES__
    __m128i h = _mm_loadu_si64(v.s);
    h = _mm_aesenc_si128(h, h);
    h = _mm_aesenc_si128(h, h);
    h = _mm_aesenc_si128(h, h);
    return _mm_cvtsi128_si64(h);
    #else
    uint64_t h;
    memcpy(&h, v.s, 8);
    return h * 1111111111111111111;
    #endif
}

static int
equal(struct val a, struct val b)
{
    return !memcmp(a.s, b.s, 8);
}

static int
next(uint64_t hash, int exp, int i)
{
    unsigned mask = (1 << exp) - 1;
    unsigned step = hash>>(64 - exp) | 1;
    return (i + step) & mask;
}
//----------------------------------------------------------------------------------
#ifdef USE_UBENCH
UBENCH( hash_jamboree, c_version )
#else
static void hash_jamboree_c_version(void) 
#endif
{

    // int len = 0;
    static struct val ht[HT_COUNT + 1] = {};

    memset( ht, 0, sizeof(struct val[HT_COUNT + 1]));

    int i = RANDINRANGE(0, HT_COUNT);

    // for (int i = 0; i < HT_COUNT ; i++) {
        struct val v = make(i);
        uint64_t h = hash(v);
        for (int i = h;;) {
            i = next(h, HT_COUNT + 1, i);
            if (!ht[i].s[0]) {
                ht[i] = v;
                // len++;
                break;
            }
            assert(!equal(ht[i], v));
        }
    // }
    // assert(len == HT_COUNT);
}

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus

//----------------------------------------------------------------------------
// https://gist.github.com/skeeto/8e7934318560ac739c126749d428a413
//----------------------------------------------------------------------------
// C++ hash table benchmark
// #include <assert.h>
#include <set>
#include <string>

#ifdef USE_UBENCH
UBENCH( hash_jamboree, cplusplus_version )
#else
static void hash_jamboree_cplusplusversion()
#endif
{
    // int len = 0;
    static std::set<std::string> ht;

    ht.clear();

     int i = RANDINRANGE(0, HT_COUNT);

    // for (int i = 0; i < HT_COUNT ; i++) {
        auto v = std::to_string(i);
        auto r = ht.insert(v);

        (void)v;
        (void)r;
        // len += r.second;
    // }
    // assert(len == HT_COUNT);
}

//----------------------------------------------------------------------------

#ifdef USE_UBENCH

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

  UBENCH_STATE();
  int main(int argc, const char *const argv[]) { 
    UBUT_CONSOLE_COLOR_RESET();
    UBUT_WARN("\nHT_COUNT = %d \n", HT_COUNT );
    srand(time(NULL));
	return ubench_main(argc, argv);      
  }

  #ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus

#else

int main(void)
{
    srand(time(NULL));
    hash_jamboree_c_version();
    hash_jamboree_cplusplusversion();
    return 42; // 
}

#endif
