
#include "godboltdbj.h"

DBJ_EXTERN_C_BEGIN

// we can imagine the whole world data is in units we
// define as 3x3 matrix of int's
typedef struct mx_ {
    const int rows;
    const int cols;
    int data[3][3];
} mx;

// for testing, create the compund statement mx filed with data
#define MX_TEST_VAL                                                            \
    (mx) {                                                                     \
        3, 3, {                                                                \
            {0, 1, 2}, {3, 4, 5}, { 6, 7, 8 }                                  \
        }                                                                      \
    }

#define MX_VAL_FMT "%2d"

// we can also imagine the whole world data will be in one "slab"
// of mx units
#define MX_SLAB_SIZE 3

// we will apstract away the actual handle type
// it must be "index" into the "slab"
typedef const int MX_HANDLE;

// on the start of the world we need some data space
// allocate slab on the stack (we could do it on heap, does not matter)
// use slab to the full capacity, at the start up
const mx slab[MX_SLAB_SIZE] = {MX_TEST_VAL, MX_TEST_VAL, MX_TEST_VAL};

// to manage the data alloc/free we need to know
// the free handles to the free slots
// on the start up there are no free slots, they are all false aka 0
// coming in C23
int slab_free_slots[MX_SLAB_SIZE] = {/* false, false, false*/};

// we do not pass the struct by value nor the pointer to it
// we pass its handle
// thus avoiding a perpetual question
static void
mx_print(MX_HANDLE mxh_) {
    dbj_err_log("\nmx:%d {", mxh_);
    for (int R = 0; R < 3; R++) {
        dbj_err_log("\n");
        for (int C = 0; C < slab[mxh_].cols; C++)
            dbj_err_log(MX_VAL_FMT, slab[mxh_].data[R][C]);
    }
    dbj_err_log("\n}");
}

static inline void
slab_print_used(void) {
    for (int k = 0; k < 3; k++)
        // do not print mx in the free slot
        if (!slab_free_slots[k])
            mx_print(k);
}

int
main(void) {
    DBJ_FX("%ld", __STDC_VERSION__);
    slab_print_used();
    return 42;
}

DBJ_EXTERN_C_END
