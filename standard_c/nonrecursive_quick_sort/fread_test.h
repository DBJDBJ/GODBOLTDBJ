#pragma once
#ifndef FREAD_TEST
#define FREAD_TEST
// https://en.cppreference.com/w/c/io/fread
#include <stdio.h>
 
#define FREAD_TEST_SIZE 5
static inline void fread_test(void)
{
    double a[FREAD_TEST_SIZE] = {1.,2.,3.,4.,5.};
    FILE *fp = fopen("test.bin", "wb"); // must use binary mode
    fwrite(a, sizeof *a, FREAD_TEST_SIZE, fp); // writes an array of doubles
    fclose(fp);
 
    double b[FREAD_TEST_SIZE];
    fp = fopen("FREAD_TEST.bin","rb");
    size_t ret_code = fread(b, sizeof *b, FREAD_TEST_SIZE, fp); // reads an array of doubles
    if(ret_code == FREAD_TEST_SIZE) {
        puts("Array read successfully, contents: ");
        for(int n = 0; n < FREAD_TEST_SIZE; ++n) printf("%f ", b[n]);
        putchar('\n');
    } else { // error handling
       if (feof(fp))
          perror("Error reading test.bin: unexpected end of file");
       else if (ferror(fp)) {
           perror("Error reading test.bin");
       }
    }
 
    fclose(fp);
}
#undef FREAD_TEST_SIZE

#endif FREAD_TEST