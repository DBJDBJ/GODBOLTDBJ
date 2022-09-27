#include "common.h"

// officialy not until C23
// for C17 and bellow
#if __STDC_VERSION__ <= 201710L
// _memccpy in MSVC
// https://developers.redhat.com/blog/2019/08/12/efficient-string-copying-and-concatenation-in-c
void *memccpy(void *restrict dst, const void *restrict src, int c, size_t n)
{
    const char *s = src;
    for (char *ret = dst; n; ++ret, ++s, --n)
    {
        *ret = *s;
        if ((unsigned char)*ret == (unsigned char)c)
            return ret + 1;
    }
    return 0;
}
#endif

/*
-------------------------------------------------------------------------------------
 from https://news.ycombinator.com/item?id=11305190

 usage:

 _autofree_ char *s1 = strdup("foo");

without this macro:  __attribute__((__cleanup__(function_to_free)))

 */

#define _autofree_ __attribute((cleanup(dbjcapi_autofree_func_)))

static inline void dbjcapi_autofree_func_(void *ptr_ptr)
{
    assert(ptr_ptr); // dbj@dbj.org
    if (!ptr_ptr)
        return; // added

    void *ptr = *(void **)ptr_ptr;
    free(ptr);
    ptr = 0; // dbj added
}

/* --------------------------------------------------- */

#define BOOK_NAME_SIZE 0xFF

struct Book
{
    int id;
    char name[BOOK_NAME_SIZE];
};

static inline struct Book *book_populate(struct Book *bptr_, char *const new_name_)
{
    assert(bptr_);
    bptr_->id = 13;
    _memccpy(bptr_->name, new_name_, '\0', BOOK_NAME_SIZE);
    return bptr_;
}

static inline void free_the_book(struct Book **bpp)
{
    assert(bpp);
    if (*bpp)
        free(*bpp);
}

enum
{
    array_len = 3
};

int main(void)
{
    /*
                      Book 0    Book 1    Book 2
                        ^         ^         ^
                        |         |         |
                +-----------------------------------+
                |  +-----------------------------+  |
 Array          |  |    |    |    |    |    |    |  |
 of Book +-------> |    +    |    +    |    +    |  |
 pointers       |  +-----------------------------+  |
                +-----------------------------------+
                   ^
                   |
                   |
                   +
               Pointer to the array

    On the heap:
        struct Book ** array_of_book_ptrs = calloc(3, sizeof(book_arr));

    On the stack:
        struct Book *array_of_book_ptrs[3] = {};

    Now why this "pointer to the array"? Because unlike the array name decaying
    ito the first array element, pointer to array is actual handle to he array; not decaying to anything.
    Example:
    The type we are allocating:      struct Book * [3]
    The type we use as handle :   typedef struct Book * (*arr_handle_type)[3] ;
    Allocate on the heap      :   arr_handle_type bp_handle = (arr_handle_type) malloc(sizeof(struct Book * [3]));
    */

    // ok and auto free way
    {
        // allocate on heap an pointer to the array of pointers to books
        typedef struct Book *(*ptr_to_array_of_book_ptrs)[array_len];

        ptr_to_array_of_book_ptrs ptabp = (ptr_to_array_of_book_ptrs)calloc(1, sizeof(ptabp));

        // make on the heap a book to be pointed at from a slot 1
        _autofree_ struct Book *bptr_ =
            *ptabp[1] = (struct Book *)malloc(sizeof(struct Book));

        // give book a name
        book_populate(*ptabp[1], "This boat does not leak");

        FX("%s", (*ptabp[1])->name);

        // cleanup attribute is not a "destructor in C"
        // it kicks in as soon as end of block is reached
        // so: exactly here
        // follow through using debugger step-in
    }
    return 42;
}