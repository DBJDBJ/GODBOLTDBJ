/* (c) 2018-2021-2022 by dbj@dbj.org -- licence is at eof
        usage until 2022:
    auto demangled_name = dbj::demang< int (&) [42] >() ;

       usage from 2022 Nov, please see the main bellow.

    This is both windows and linux C++ code.

    https://godbolt.org/z/9KWo8Tcvx
*/

#if __cplusplus >= 202002L
// C++20 (and later) code
#define CONSTEXPR constexpr
#else // lower than C++20
#define CONSTEXPR
#endif

#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <cxxabi.h>
#else
#include <typeinfo>
#endif
namespace dbj
{
#ifdef _WIN32
    template <typename T_>
    CONSTEXPR const char *demang() noexcept
    {
        return typeid(T_).name();
    }
#else  // __linux__
    template <typename T>
    CONSTEXPR const char *demang() noexcept
    {
        // once per one type
        auto demangled_name = []()
        {
            int error = 0;
            // std::string retval;
            char *name = abi::__cxa_demangle(typeid(T).name(), 0, 0, &error);

            switch (error)
            {
            case 0:
                // retval = name;
                break;
            case -1:
                name = strdup("memory allocation failed");
                break;
            case -2:
                name = strdup("not a valid mangled name");
                break;
            default:
                name = strdup("__cxa_demangle failed");
                break;
            }
            // std::free(name);
            return name;
        }();
        // std::free(name) ;
        return demangled_name; // .data();
    }
#endif // __linux__

    // calling with instances
    template <typename T>
    const char *demang(T const &) noexcept
    {
        return demang<T>();
    }

} // namespace dbj

#include <stdio.h>
#include <string>

template <typename T_>
__inline CONSTEXPR
    std::string
    display_type(void)
{
    return dbj::demang<T_>();
}

template <typename T_>
__inline CONSTEXPR
    std::string
    display_type(const T_ &t_)
{
    return dbj::demang<T_>(t_);
}

#define SX(F_, X_) printf("\n%s : " F_, (#X_), (X_))

int main(void)
{
    printf("\n%s", display_type<int>().data());
    printf("\n%s", display_type(42).data());
   return EXIT_SUCCESS;
}