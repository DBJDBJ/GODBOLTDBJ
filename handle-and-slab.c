
#include "common.h"

#ifdef __cpluplus
extern "C"
{
#endif

    typedef struct mx_
    {
        const int rows;
        const int cols;
        int data[3][3];
    } mx;

#define MX_VAL_FMT "%2d"

#define MX_SLAB_SIZE 3

    typedef const int MX_HANDLE;

    // on the start
    // allocate slab on the stack
    // use slab to the full capacity
    const mx slab[MX_SLAB_SIZE] = {
        (mx){3, 3, {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}}},
        (mx){3, 3, {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}}},
        (mx){3, 3, {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}}}};

    // no free slots
    int slab_free_slots[MX_SLAB_SIZE] = {/* false, false, false*/};

    static void elementary(mx matrix_);

    static void mx_print(MX_HANDLE mxh_)
    {
        printf("\nmx:%d {", mxh_);
        for (int R = 0; R < 3; R++)
        {
            printf("\n");
            for (int C = 0; C < slab[mxh_].cols; C++)
                printf(MX_VAL_FMT, slab[mxh_].data[R][C]);
        }
        printf("\n}");
    }

    static void slab_print_used()
    {
        for (int k = 0; k < 3; k++)
            // do not print mx in the free slot
            if (!slab_free_slots[k])
                mx_print(k);
    }

    int main(void)
    {
        FX("%ld",__STDC_VERSION__ );
        slab_print_used();
        return 42;
    }

    static void elementary(mx matrix_)
    {
        //       // declare and initialize an array
        //   int arr[3][2] = {{50,60},{70,80},{90,100}};

        //   // display 2D array using for loop
        //   printf("The Array elements are:\n");

        // outer loop for row
        for (int i = 0; i < matrix_.rows; i++)
        {
            // inner loop for column
            for (int j = 0; j < matrix_.cols; j++)
            {
                printf("%d ", matrix_.data[i][j]);
            }
            printf("\n"); // new line
        }
    }

#ifdef __cpluplus
} // extern "C" {
#endif
