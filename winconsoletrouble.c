#include <stdio.h>
#include <io.h>
#include <fcntl.h>

// #define _O_U16TEXT 0x00020000
// #define _O_U8TEXT 0x00040000

#define specimen L"кошка 日本"

void test_crash_console_output( void )
{
    // Visual Studio 2022 version 17.3.4
    // 
       printf("\n_MSC_FULL_VER: %d\n", _MSC_FULL_VER);

        fflush(stdout);
        _setmode(_fileno(stdout), _O_U16TEXT);
        // printf("CRASH BANG!");
        wprintf(L"%s", specimen);

        fflush(stdout);
        _setmode(_fileno(stdout), _O_TEXT);
        // If printf is used after _setmode(_fileno(stdout), _O_U16TEXT); 
        // this line is NEVER REACHED :( ucrtbased.dll stops the show...
}

int main (void){
    test_crash_console_output();
    return 42;
}