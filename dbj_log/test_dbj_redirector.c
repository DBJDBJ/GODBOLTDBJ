// dbj
#define DBJ_CAPI_DEFAULT_LOG_IMPLEMENTATION
#include "redirected_stderr_log.h"
#include <dbj_seh.h>

int
main(const int argc, const char argv[argc]) {
    __try {
        // SEH raised
    } __except (dbj_filter_seh(GetExceptionCode(), GetExceptionInformation())) {
        DBJ_LLL("SEH caught");
    }
    return 42;
}