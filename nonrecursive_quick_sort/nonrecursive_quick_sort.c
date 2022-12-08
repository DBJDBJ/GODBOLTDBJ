#include <stdio.h>
#include <stdlib.h>

/*
non recursive quick sort as a macro
*/

#undef QUICK_SORT
#define QUICK_SORT(arr, size) \
  do { \
    int stack[size]; \
    int stack_size = 0; \
    stack[stack_size++] = 0; \
    stack[stack_size++] = size - 1; \
    while (stack_size > 0) { \
      int right = stack[--stack_size]; \
      int left = stack[--stack_size]; \
      if (left < right) { \
        int pivot = arr[(left + right) / 2]; \
        int i = left; \
        int j = right; \
        while (i <= j) { \
          while (arr[i] < pivot) i++; \
          while (arr[j] > pivot) j--; \
          if (i <= j) { \
            __typeof(arr[0]) tmp = arr[i]; \
            arr[i] = arr[j]; \
            arr[j] = tmp; \
            i++; \
            j--; \
          } \
        } \
        stack[stack_size++] = left; \
        stack[stack_size++] = j; \
        stack[stack_size++] = i; \
        stack[stack_size++] = right; \
      } \
    } \
  } while (0)


#define PRINT_ARRAY(F_, arr, size) \
    for (int i = 0; i < size; i++) \
        printf(" " F_ " ", arr[i]); \
    printf("\n");


// Function prototypes
static void quickSort(int *array, int left, int right);

// Main function
int main(void)
{
    // Sample array to sort
    double array[] = {5.1, 7., .2, 4.3, 9.9, 1.2};
    int n = sizeof(array) / sizeof(double);

    // Print unsorted array
    printf("Original array:");
    PRINT_ARRAY("%1.1f", array, n);

    // Sort array
    // quickSort(array, 0, n - 1);
    QUICK_SORT(array,n);

    printf("Sorted array:");
     PRINT_ARRAY("%1.1f", array, n);

    return 0;
}

#define SWAP(x, y) \
    do { \
        __typeof(x) tmp = x; \
        x = y; \
        y = tmp; \
    } while (0)

// right keeps the result
#define PARTITION(array, left, right) \
    do { \
        __typeof(*(array)) pivot = (array)[right]; \
        int splitIndex = left; \
        for (int i = left; i < right; i++) \
        { \
            if ((array)[i] < pivot) \
            { \
                SWAP((array)[i], (array)[splitIndex]); \
                splitIndex++; \
            } \
        } \
        SWAP((array)[right], (array)[splitIndex]); \
        right = splitIndex; \
    } while (0)

/*
dbj@dbj.org  2022DEC07

non generic qsort using generic SWAP and PARTITION

instead of writing non generic comparison func. 
write (very short) non generic qsort func.

in here we qsort array of ints

*/
void quickSort(int *array, int left, int right)
{
    // Stop recursion when left index is greater than or equal to right index
    if (left >= right)
        return;

    // Partition the array and get the pivot index
    // int pivotIndex = partition(array, left, right);
    PARTITION(array, left, right);
    int pivotIndex = right;

    // Recursively sort the left and right halves of the array
    quickSort(array, left, pivotIndex - 1);
    quickSort(array, pivotIndex + 1, right);
}
