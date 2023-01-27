#pragma once
// for benchmarking
#include "lambdamatrix.h"
#include <array>
#include <vector>

// Banch MarKing == bmk
namespace dbj::mtx {

template <typename T, size_t N>
inline auto
std_stack() noexcept {
    static_assert(N > 0);
    static_assert(N < dbj::mtx::MAX_STACK_BLOCK, "max stack block is 0xFF");

    return [](size_t const &idx) mutable -> T & {
        static std::array<T, N + 1> outer_buff_{};
#ifdef _DEBUG
        auto size_ = N;
        (void) size_;
#endif
        assert(idx <= outer_buff_.size());
        return outer_buff_[idx];
    };
}

template <typename T, size_t N>
inline auto
std_heap() noexcept {
    static_assert(N > 0);
    static_assert(N < dbj::mtx::MAX_HEAP_BLOCK,
                  "max heap_block_type block is 0xFFFF");

    return [](size_t const &idx) mutable -> T & {
        static std::vector<T> outer_heap_instance_(N + 1);
        assert(N < outer_heap_instance_.size());

#ifdef _DEBUG
        auto size_ = N;
        (void) size_;
#endif
        assert(idx <= outer_heap_instance_.size());
        return outer_heap_instance_[idx];
    };
}

}   // namespace dbj::mtx
