#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

enum { max_args = 255 } ;  // never without max args

typedef void myvararg_callback ( void *  ) ; 

static myvararg_callback my_print_ ;

static void my_print_ ( void * p ) 
{
    assert(p);
    static unsigned cnt_ = 0;
        printf("Argument %d %8s[%p] \"%s\"\n", ++cnt_, " ", p,(char *)p);
}

// this is not generic
// this works only if second argument is void *
// AND if last argument in NULL
// can do max max_args arguments
static void myvararg( myvararg_callback callback_fptr , void *ptr, ...) {
    assert(callback_fptr);
    unsigned cnt_ = 0;
    va_list va;
    va_start(va, ptr);
    for (void * p = ptr; p != NULL && cnt_ < max_args ; p = va_arg(va, void *)) {
        callback_fptr(p);
    }
        assert(max_args > cnt_ ) ;
    va_end(va);
}

#define MULTIFREE(...) myvararg((void)__VA_ARGS__, 0)

enum { newlen = 1024, ARRSZ = 9 } ; // global space
typedef char  * arr_type[ARRSZ] ;

static void my_free_ ( void * p ) 
{
    static unsigned cnt_ = 0;
    if (p){
    printf("Going to free %d [%p] \"%s\"\n", ++cnt_, p,(char *)p);
    free(p);
    p = 0 ;
    }
}


int main() {

    // last argument must be NULL
    myvararg( my_print_, "A", "B", "C", 0);

    // example from the post
    // note: this is C
// in some function
// static array of 9 pointers to the heap space
arr_type slave = {
	calloc( newlen, sizeof(char)), /* 0 */
	calloc( newlen, sizeof(char)),
	calloc( newlen, sizeof(char)),
	calloc( newlen, sizeof(char)),
	calloc( newlen, sizeof(char)),
	calloc( newlen, sizeof(char)),
	calloc( newlen, sizeof(char)),
	calloc( newlen, sizeof(char)),
	calloc( newlen, sizeof(char)) /* 8 */
};

    return 42;
}