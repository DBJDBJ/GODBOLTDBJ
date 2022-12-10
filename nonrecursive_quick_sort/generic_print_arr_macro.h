#pragma once
#ifndef GENERIC_PRINT_ARR_MACRO
#define GENERIC_PRINT_ARR_MACRO
/*
in C there is no safe array count macro or function
the safest array in C must be a struct with flexible array member

typedef struct {
   const unsigned int size;
   int    data[] ;
} array_int ;

array_int * make_int_arr(size_t count) {
array_int * aint_ptr = calloc(1,sizeof(array_int) + sizeof(int[count])); 
    int * hack = (int*)aint_ptr ;
        *hack = count;
    return aint_ptr; 
}

int is intentionaly used for size parameter type bellow
*/

#include <stdio.h>

#undef PRINT_ARRAY_STRM
#undef PRINT_ARRAY

#define PRINT_ARRAY_STRM stdout
#define PRINT_ARRAY( F_, arr, size ) \
do { \
    for (int i = 0; i < size; i++) \
        fprintf( PRINT_ARRAY_STRM, " " F_ " ", arr[i]); \
    fprintf( PRINT_ARRAY_STRM, "\n");\
} while(0)


#endif // GENERIC_PRINT_ARR_MACRO