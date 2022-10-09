#pragma once
// for benchmarking
#include <array>
#include <vector>
#include "common.h"

// Banch MarKing == bmk
namespace dbj::mtx
{

	template <typename T, size_t N>
	inline auto std_stack() noexcept
	{
		static_assert(N < dbj::mtx::MAX_STACK_BLOCK, "max stack block is 0xFF");
		std::array<T, N> buff_{};
		return [buff_, size = N](size_t const &idx) mutable -> T &
		{
			assert(idx < size);
			return buff_[idx];
		};
	}

	template <typename T, size_t N>
	inline auto std_heap() noexcept
	{
		static_assert(N < dbj::mtx::MAX_HEAP_BLOCK, "max heap_block_type block is 0xFFFF");

		std::vector<T> outer_heap_instance_{N};

		return [heap_instance_ = outer_heap_instance_, size = N](size_t const &idx) mutable -> T &
		{
			assert(idx < size);
			return heap_instance_[idx];
		};
	}

} // dbj::bmk

