#pragma once
#include <dbj_common.h>
#include <dbj_seh.h>
#include <dbj_windows_include.h>   // also dbj_assert() non destructive

#include <io.h>
#include <stdarg.h>

// (c) 2021 by dbj@dbj.org https://dbj.org/license_dbj
//
// #define DBJ_CAPI_DEFAULT_LOG_IMPLEMENTATION , in one place exactly
//
// --------------------------------------------------------------
DBJ_EXTERN_C_BEGIN

#define DBJ_DEFAULT_LOG_BUFSIZ 1024

// poor man's logging
// LLL == Low Level Log
#ifndef DBJ_LLL
#define DBJ_LLL(...) fprintf(stderr, __VA_ARGS__)
#endif

/// --------------------------------------------------------------
/// the simplest way to use this from windows app
/// might be to redirect stderr to a file
/// https://stackoverflow.com/questions/14543443/in-c-how-do-you-redirect-stdin-stdout-stderr-to-files-when-making-an-execvp-or

void dbjcapi_default_log_function(const char *filename, long lineno,
                                  const char *format, ...);

typedef struct dbj_redirector_state dbj_redirector_state;

dbj_redirector_state *dbj_redirector_state_(void);
void dbjcapi_redirector_on(const char[static 1] /*log file name*/);
void dbjcapi_redirector_off(void);

/// --------------------------------------------------------------
#ifdef DBJ_CAPI_DEFAULT_LOG_IMPLEMENTATION

/// --------------------------------------------------------------

typedef enum dbjcapi_default_log_state_ {
    dbjcapi_default_log_noninitialized,
    dbjcapi_default_log_constructed,
    dbjcapi_default_log_destructed
} dbjcapi_default_log_state;

dbjcapi_default_log_state dbjcapi_default_log_state_ =
    dbjcapi_default_log_noninitialized;

void
dbjcapi_default_log_function(const char *filename, long lineno,
                             const char *format, ...) {
    DBJ_ASSERT(filename && lineno && format);

    if (dbjcapi_default_log_state_ != dbjcapi_default_log_constructed)
        return;

#ifdef _DEBUG
    if (_isatty(_fileno(stderr)) > 0) {
        // console found
    }
#endif
    va_list args;
    char buffer[DBJ_DEFAULT_LOG_BUFSIZ] = {0};

    va_start(args, format);
    vsnprintf(buffer, sizeof buffer, format, args);
    va_end(args);

    fprintf(stderr, "\n%s(%lu) : %s", filename, lineno, buffer);
    fflush(stderr);
}

#define DBJ_CAPI_DEFAULT_LOG_EXTENSION ".log"

struct dbj_redirector_state {
    bool redirected;
    int fd;
    fpos_t pos;
    FILE *stream;
};

__attribute__((destructor)) void
dbjcapi_default_log_destruct(void) {
    if (dbjcapi_default_log_state_ == dbjcapi_default_log_destructed)
        return;

    if (_isatty(_fileno(stderr)) != 0) {
        // we are on console
    } else {
        dbjcapi_redirector_off();
    }

    dbjcapi_default_log_state_ = dbjcapi_default_log_destructed;
}
// if no console do the stderr redirection on the startup
__attribute__((constructor)) void
dbjcapi_default_log_construct(void) {
    if (dbjcapi_default_log_state_ == dbjcapi_default_log_constructed)
        return;

    // volatile const int is_stdout = _isatty(_fileno(stdout));
    volatile const int is_stderr = _isatty(_fileno(stderr));

    if (is_stderr > 0) {
        // console found, and a shamefull hack that works to kick-start VT100
        system(" ");
    } else {
        char app_full_path[DBJ_DEFAULT_LOG_BUFSIZ] = {0};
        // Q: is __argv available for windows desktop apps?
        // A: no it is not
        // win32 required here
        int rez =
            GetModuleFileNameA(NULL, app_full_path, DBJ_DEFAULT_LOG_BUFSIZ);
        DBJ_ASSERT(rez != 0);
        char app_log_path[DBJ_DEFAULT_LOG_BUFSIZ] = {0};
        rez = _snprintf(app_log_path, 1024, "%s%s", app_full_path,
                        DBJ_CAPI_DEFAULT_LOG_EXTENSION);
        DBJ_ASSERT(rez > 0);
        dbjcapi_redirector_on(app_log_path);
    }   // else
    dbjcapi_default_log_state_ = dbjcapi_default_log_constructed;
}

/// -----------------------------------------------------
/// redirect stderr to file
/// warning: not enough error checking

dbj_redirector_state *
dbj_redirector_state_(void) {
    static dbj_redirector_state dbj_redirector_state_instance = {false};
    return &dbj_redirector_state_instance;
}

void
dbjcapi_redirector_on(const char filename[static 1]) {
    // otherwise we are on console and stderr can not be redirected
    DBJ_ASSERT(0 == _isatty(_fileno(stderr)));

    dbj_redirector_state *state = dbj_redirector_state_();

    if (state->redirected)
        return;

    fflush(stderr);
    fgetpos(stderr, &state->pos);
    state->fd = _dup(_fileno(stderr));
    errno_t err = freopen_s(&state->stream, filename, "w", stderr);

    if (err != 0)
        DBJ_FAST_FAIL;

    SYSTEMTIME lt;
    GetLocalTime(&lt);

    fprintf(stderr, "\n\n");
    fprintf(stderr, "\n*****                                                   "
                    "                  *****");
    fprintf(stderr, "\n*****  %s", filename);
    fprintf(stderr, "\n*****  LOG BEGIN                                        "
                    "                  *****");
    fprintf(stderr, "\n*****                                                   "
                    "                  *****");
    fprintf(stderr, "\n\nLocal time:%4d-%02d-%02d %02d:%02d:%02d\n\n", lt.wYear,
            lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond);
}

/// if you do this too soon printing to redirected
/// stderr might not outpout to file but to
/// non existent console
void
dbjcapi_redirector_off(void) {
    dbj_redirector_state *state = dbj_redirector_state_();

    // otherwise we are on console and stderr can not be re-redirected
    DBJ_ASSERT(0 == _isatty(_fileno(stderr)));

    fflush(stderr);
    int dup2_rezult_ = _dup2(state->fd, _fileno(stderr));
    _close(state->fd);
    clearerr(stderr);
    fsetpos(stderr, &state->pos);
}

#endif   // DBJ_CAPI_DEFAULT_LOG_IMPLEMENTATION
// -----------------------------------------------------------------------------
DBJ_EXTERN_C_END
