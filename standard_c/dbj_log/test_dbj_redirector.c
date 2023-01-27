// idea is to redirect the stderr so in case of win desktop build we
// printf to it
// dbj
#define DBJ_CAPI_DEFAULT_LOG_IMPLEMENTATION
#include "redirected_stderr_log.h"

int
main(const int argc, const char **argv) {

    dbjlowlog("Whatever");
    return 42;
}
