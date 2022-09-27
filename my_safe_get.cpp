#include <assert.h>
#include <cstdlib>

template <std::size_t Idx, typename T, std::size_t N>
constexpr T& get(T (&arr)[N]) noexcept {
    static_assert(Idx < N, "Index out of bounds");
    return arr[Idx];
}

int main(void) {
    constexpr int arr[99] = {1, 2, 3, 4};
    static_assert(get<2>(arr) == 3);

    typedef decltype(&get<99>(arr)) time_bomb;
    time_bomb p = &get<2>(arr);  // works
}