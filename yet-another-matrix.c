#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#undef FX_WIDTH
#undef FX
#undef FXB

#define FX_WIDTH "32"
#define FX(F,X) printf("\n%"FX_WIDTH"s : " F,(#X),(X))
#define FXB(X) printf("\n%"FX_WIDTH"s : %s",(#X),(X)? "true" : "false")

#define MATRIX_HANDLE void*

// usage: typedef MATRIX_STRUCT(char) char_matrix ;
#define MATRIX_STRUCT(T_) \
  struct {                \
    unsigned rows;        \
    unsigned cols;        \
    T_ data[];            \
  }

#define MATRIX_ALIAS(N_,T_,C_) \
  typedef T_(*N_)[C_]

#define MATRIX_CAST(N_,A_, M_) \
A_ N_ = (A_)&(M_->data)

typedef MATRIX_STRUCT(char) str_mx_type;

static str_mx_type * new_matrix(const unsigned R_, const unsigned C_) {
  str_mx_type* retval 
    = calloc(1, sizeof(str_mx_type) + sizeof(char[R_*C_]));
  if (retval) {
    retval->rows = R_;
    retval->cols = C_;
  }
  return retval ;
}

__attribute__((always_inline)) int is_empty ( const char * s_ )
{
   return (s_ && *s_ == 0);
}

__attribute__((always_inline)) int is_null ( const void * s_ )
{
   return (s_ == 0);
}

int main(void) {

   FX("%lu", strlen(""));
   FXB(""[0] == '\0');
   FXB(is_empty("") );
   FXB(is_null("") );
   FXB(is_empty(0) );
   FXB(is_null(0) );

   // array of chars 
   /*
warning: use of GNU empty initializer extension [-Wgnu-empty-initializer]
warning: zero size arrays are an extension [-Wzero-length-array]
   */
   const char charr[] = {};

  str_mx_type* proto = new_matrix(3,4);

  MATRIX_ALIAS( matrix, char, proto->cols) ;

  MATRIX_CAST(mx,matrix,proto);

  mx[1][1] = 'x';
}
