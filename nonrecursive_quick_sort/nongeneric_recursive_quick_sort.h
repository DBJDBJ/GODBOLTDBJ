#ifndef NONGENERIC_RECURSIVE_QUICK_SORT
#define NONGENERIC_RECURSIVE_QUICK_SORT
/*
dbj@dbj.org  2022DEC07

non generic qsort using generic PRIVATE_SWAP and PRIVATE_PARTITION_

instead of writing non generic comparison func. 
write (very short) non generic qsort func.
actualy generate one with a macro (on the roadmap)
*/

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif // _countof

// Function prototypes for intrinsic types
static inline void quick_sort_int(int *array, int left, int right);

#ifdef IMPLEMENT_NONGENERIC_RECURSIVE_QUICK_SORT

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

/*
in here we qsort array of ints
*/
// void quick_sort_int(int *array, int left, int right)
// {
//     // Stop recursion when left index is greater than or equal to right index
//     if (left >= right)
//         return;

//     // Partition the array and get the pivot index
//     PRIVATE_PARTITION(array, left, right);
//     int pivotIndex = right;

//     // Recursively sort the left and right halves of the array
//     quick_sort_int(array, left, pivotIndex - 1);
//     quick_sort_int(array, pivotIndex , right);
// }
/*
The partition function is not needed because the partitioning is done within the quick_sort_int   function.
*/
void quick_sort_int  (int *array, int left, int right) {
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