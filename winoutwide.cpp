// It is as simple as that. No C++ stdlib I/O, in this case, I am afraid.
// Just use Code Page Windows.1252 , output your Unicode “squiggly bits”,
// and then upon exiting the scope, revert back to whatever was the Code Page in use.

#include <windows.h>
#include <crtdbg.h>

// non destructive assert
#ifdef _DEBUG
#define assert(x_) _ASSERTE(x_)
#else
#define assert(x_) (__typeof(x_))(x_)
#endif

struct __declspec(novtable)
    WideOut
{
    HANDLE output_handle_;
    UINT previous_code_page_;

    WideOut()
    {
        this->output_handle_ = ::GetStdHandle(STD_OUTPUT_HANDLE);
        assert(INVALID_HANDLE_VALUE != this->output_handle_);
        previous_code_page_ = ::GetConsoleOutputCP();
        // CP Windows 1252
        assert(0 != ::SetConsoleOutputCP(1252));
        /* TODO: GetLastError() */
    }

    ~WideOut()
    {
        assert(0 != ::SetConsoleOutputCP(previous_code_page_));
        // TODO: should we "relase" this->output_handle_ ?
        // NO! this is stdout ::CloseHandle (this->output_handle_) ;

        /* TODO: GetLastError() */
    }
    /* wide output to Windows console */
    void operator()(const wchar_t *const wp_)
    {
        // locking here
        assert(0 != ::WriteConsoleW(
                        this->output_handle_,
                        wp_,
                        wcslen(wp_),
                        NULL, NULL));
    }

public:
    static WideOut & instance (void)
    {
        static WideOut instance_;
        return instance_ ;
    }
};

int main(void)
{
    auto helper_ = WideOut::instance() ;

    // output the Unicode extended chars to the Win Console
    helper_(L"║═╚");

    #define specimen L"кошка 日本"

    helper_(specimen);
    // destruct the helper and revert to previous Code Page
    return 42;
}