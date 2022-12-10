#pragma once
#ifndef QUICK_SORT_NON_RECURSIVE_MACRO
#define QUICK_SORT_NON_RECURSIVE_MACRO

/*
non recursive quick sort as a macro
*/

// right now has a bug, so ...
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

#endif // QUICK_SORT_NON_RECURSIVE_MACRO