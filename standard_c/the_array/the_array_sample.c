
/*

THE_ARRAY sampling 
*/
#include "the_array.h"
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

// FRIST the declares and defines --------------------------------------------

// float_array
// new_float_array( unsigned );
// copy_float_array( unsigned );
THE_ARRAY(float);

// char_array
// new_char_array( unsigned );
// copy_char_array( unsigned );
THE_ARRAY(char);

// array of UDT's
typedef struct { char name [0xf]; } my_type ;
// my_type_array
// new_my_type_array( unsigned );
// copy_my_type_array( unsigned );
THE_ARRAY(my_type);

int main(void) {
    // array size is given at runtime
    float_array* _auto_cleanup_ f16 = new_float_array(0xF);
    f16->data[0] = 3.14;
    PRINT_ARRAY("%2.2f", f16->data, f16->size );

    float_array* _auto_cleanup_ f16copy = copy_float_array(f16);
    assert(f16->data[0] == f16copy->data[0]);
    PRINT_ARRAY("%2.2f", f16copy->data, f16copy->size );

    //----------------
    char_array* _auto_cleanup_ c16 = new_char_array(0xF);
    c16->data[0] = '!';
    PRINT_ARRAY("%c", c16->data, c16->size );

    char_array* _auto_cleanup_ c16copy = copy_char_array(c16);
    assert(c16->data[0] == c16copy->data[0]);
    PRINT_ARRAY("%c", c16copy->data, c16copy->size );
    //----------------
    my_type_array _auto_cleanup_ * mta = new_my_type_array(0xF);

    return 42;
}
