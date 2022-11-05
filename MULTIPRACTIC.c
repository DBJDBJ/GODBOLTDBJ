#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define MULTIPRACTIC(...) myvararg((void*)__VA_ARGS__, 0)

enum { multipractic_max_args = 255 } ;  // never without max args

typedef void multipractic_callback ( void *  ) ; 

static multipractic_callback my_print_ ;

static void my_print_ ( void * p ) 
{
    assert(p);
    printf("Argument %8s[%p] \"%s\"\n", " ", p,(char *)p);
}

static multipractic_callback my_free_ ;

static void my_free_ ( void * p ) 
{
    if (p != NULL){
    printf("Going to free [%p] \"%s\"\n", p,(char *)p);
    free(p);
    p = NULL ;
    }
}

// this is not generic
// this works only if second argument is void *
// AND if last argument is NULL
// can do max multipractic_max_args arguments
static void myvararg( multipractic_callback callback_fptr , void *ptr, ...) {
    assert(callback_fptr);
    unsigned cnt_ = 0;
    va_list va;
    va_start(va, ptr);
    for (void * p = ptr; p != NULL && cnt_ < multipractic_max_args ; p = va_arg(va, void *)) {
        callback_fptr(p);
    }
    va_end(va);
}



int main() {

    // last argument must be NULL
    MULTIPRACTIC( my_print_, "A", "B", "C");
    MULTIPRACTIC( my_print_, "D", "E", "F");
    MULTIPRACTIC( my_print_, "G", "H", "L");

enum { newlen = 1024, ARRSZ = 9 } ; 
typedef char  * arr_type[ARRSZ] ;

// static array of 3 pointers to the allocated heap space
arr_type slave = {
	calloc( newlen, sizeof(char)), /* 0 */
	calloc( newlen, sizeof(char)),
	calloc( newlen, sizeof(char))  /* 2 */
};

MULTIPRACTIC(my_free_, slave[0],slave[1],slave[2]);
MULTIPRACTIC(my_free_, slave[0],slave[1],slave[2]);

    return 42;
}