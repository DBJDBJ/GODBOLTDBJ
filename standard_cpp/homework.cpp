#include <stdio.h>

#define FX(F, X) printf("\n%s : " F, (#X), (X))

int
main() {
    printf("\nStart");
    // cl Version 19.21.27702.2 for x86
    //
    constexpr auto MSCVER = _MSC_VER;            // 1921
    constexpr auto MSCFULLVER = _MSC_FULL_VER;   // 192127702
    constexpr auto MSCBUILD = _MSC_BUILD;        // 2

    FX("%d", MSCVER);
    FX("%d", MSCFULLVER);
    FX("%d", MSCBUILD);
    /*
    : error C2131:  expression did not evaluate to a constant
    : message :  failure was caused by non-constant arguments or reference to a
    non-constant symbol : message :  see usage of '__LINE__Var'
    */
    constexpr long LINE = static_cast<long>(__LINE__);

    FX("%ld", LINE);

    fflush(stdout);

    return LINE;
}