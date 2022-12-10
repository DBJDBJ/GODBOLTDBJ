#ifndef NONGENERIC_RECURSIVE_QUICK_SORT
#define NONGENERIC_RECURSIVE_QUICK_SORT
/*
dbj@dbj.org  2022DEC07

instead of writing non generic comparison func. 
write (very short) non generic qsort func.
actualy generate one with a macro (on the roadmap)
*/

// Function prototypes for intrinsic types
static inline void quick_sort_int(int *array, int left, int right);

#ifdef IMPLEMENT_NONGENERIC_RECURSIVE_QUICK_SORT

#if 0
// in here we do not use SWAP and PARTITION macros
#undef PRIVATE_SWAP_
#define PRIVATE_SWAP(x, y) \
    do { \
        __typeof(x) tmp = x; \
        x = y; \
        y = tmp; \
    } while (0)

// right keeps the result
#undef PRIVATE_PARTITION
#define PRIVATE_PARTITION(array, left, right) \
    do { \
        __typeof(*(array)) pivot = (array)[(left + right) / 2]; \
        int splitIndex = left; \
        for (int i = left; i < right; i++) \
        { \
            if ((array)[i] < pivot) \
            { \
                PRIVATE_SWAP((array)[i], (array)[splitIndex]); \
                splitIndex++; \
            } \
        } \
        PRIVATE_SWAP((array)[right], (array)[splitIndex]); \
        right = splitIndex; \
    } while (0)
#endif // 0
/*
in here we qsort array of ints
The partition function is not needed because the partitioning is done within the quick_sort_int   function.
*/
static inline void quick_sort_int  (int *array, int left, int right) {
    if (left >= right) {
        return;
    }

    __typeof(array[0]) pivot = array[(left + right) / 2];
    int i = left;
    int j = right;

    while (i <= j) {
        while (array[i] < pivot) {
            i++;
        }

        while (array[j] > pivot) {
            j--;
        }

        if (i <= j) {
            __typeof(array[0]) temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }

    quick_sort_int(array, left, j);
    quick_sort_int(array, i, right);
}


#endif // IMPLEMENT_NONGENERIC_RECURSIVE_QUICK_SORT

#endif // NONGENERIC_RECURSIVE_QUICK_SORT