#define DBJ_CAPI_DEFAULT_LOG_IMPLEMENTATION
#include "../dbj_log/redirected_stderr_log.h"
#include <common/dbj_seh.h>

void
strswap(char **name1, char **name2) {
    char *tempo_1 = _strdup(*name1);
    char *tempo_2 = _strdup(*name2);

    assert(tempo_1 && tempo_2);

    *name1 = tempo_2;
    *name2 = tempo_1;
}

int
main(const int argc, const char **argv) {
    __try {
        // SEH raised
    } __except (dbj_filter_seh(GetExceptionCode(), GetExceptionInformation())) {
        DBJ_LLL("SEH caught");
    }
    return 42;
}