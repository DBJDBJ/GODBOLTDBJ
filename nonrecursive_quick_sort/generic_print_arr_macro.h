#pragma once
#ifndef GENERIC_PRINT_ARR_MACRO
#define GENERIC_PRINT_ARR_MACRO
/*
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