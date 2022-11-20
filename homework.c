// refactoring https://www.reddit.com/r/C_Programming/comments/yzxz2k/something_is_wrong_with_my_code_and_my_hair_is/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CAPACITY 0xFF
#define MAX_STRINGS 5

int main()
{
    size_t max_len = 0;
    char *max_string = 0;

    printf("\n\nPlease enter %d strings\n", MAX_STRINGS );

    for (unsigned i = 0; i < MAX_STRINGS; ++i)
    {
        char userString[MAX_CAPACITY] = {0};
        printf("%3d: ", i);
        fflush(stdout);
        if (0 != fgets(userString, MAX_CAPACITY, stdin))
        {
            size_t valid_len = strcspn(userString, "\n");

            if (valid_len > max_len)
            {
                max_len = valid_len;
                if (max_string != 0)
                    free(max_string);
                userString[valid_len] = '\x0';
                max_string = _strdup(userString);
            }
        }
    }
    fflush(stdout);
    printf("\nThe first longest string (length: %llu) was: '%s' ", max_len, max_string);
    if (max_string != 0)
        free(max_string);
    return 0;
}