#include <stdio.h>
#include <stdlib.h>

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif // _countof

#include "generic_print_arr_macro.h"

#define IMPLEMENT_NONGENERIC_RECURSIVE_QUICK_SORT
#include "nongeneric_recursive_quick_sort.h"

static void test_sort_int_arr(const int n, int *array)
{
  printf("\nOriginal array:\n");
  PRINT_ARRAY("%2d", array, n);

  // spot the n-1 !
  quick_sort_int(array, 0, n - 1);

  printf("\nSorted array:\n");
  PRINT_ARRAY("%2d", array, n);
}

// Main function
int main(void)
{
  {
    int arr[] = {5, 7, 2, 4, 9, 1};
    test_sort_int_arr(_countof(arr), arr);
  }
  {
    int arr[] = {4, 5, 7, 1, 3, 9, 2, 8, 6};
    test_sort_int_arr(_countof(arr), arr);
  }
  return 0;
}
