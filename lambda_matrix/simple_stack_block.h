
#include <assert.h>
#include <utility>

using namespace std;

// the limits
constexpr size_t MAX_STACK_BLOCK = 0xFF;
// constexpr size_t MAX_HEAP_BLOCK = 0xFFFF;

// we do not need std::array
template <typename T, size_t WDT, size_t HGT>
inline auto simple_stack_block() noexcept
{

    constexpr size_t N = WDT * HGT;
    static_assert(MAX_STACK_BLOCK > N, "max R * C for a stack block is 0xFF");

    T buff_[N]{};                      // guaranteed as a single block
    typedef T(*buff_2d_ptr)[WDT][HGT]; // pointer to 2d matrix

    return std::make_pair(
        // array is copied and carried inside lambda instance
        [buff_, size = N](size_t const row_idx_,
                          size_t const col_idx_) mutable -> T &
        {
            // T(*buff_2d_)[HGT] = (T(*)[HGT]) & (buff_);
            buff_2d_ptr buff_2d_ = (buff_2d_ptr) & (buff_);
#ifndef NDEBUG
            size_t dumsy_ = size;
            (void)dumsy_;
#endif
            // idx is 0 .. size
            assert(row_idx_ < HGT);
            assert(col_idx_ < WDT);
            return *buff_2d_[row_idx_][col_idx_];
        },
        []()
        { return std::make_pair(WDT, HGT); }); // eof make pair
}

#ifdef SIMPLE_STACK_BLOCK_TEST

#include <stdio.h>

#define FX(F, X) printf("\n%s: " F, (#X), (X))

int main(void)
{
    // test_lambda_pair<int, 3, 3>();

    auto [mtx, dim] = simple_stack<int, 3, 3>();

    FX("%lu", dim().first);
    FX("%lu", dim().second);

    FX("%d", mtx(0, 0) = 1);

    FX("%d", mtx(0, 1) = 2);
    FX("%d", mtx(0, 2) = 3);

    FX("%d", mtx(1, 0) = 4);
    FX("%d", mtx(1, 1) = 5);
    FX("%d", mtx(1, 2) = 6);

    FX("%d", mtx(1, 3) = 6); // assertion failed

    FX("%d", mtx(0, 0) == 1);

    return 42;
}

#endif // SIMPLE_STACK_BLOCK_TEST
