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
		static_assert(N > 0);
		static_assert(N < dbj::mtx::MAX_STACK_BLOCK, "max stack block is 0xFF");

		std::array<T, N + 1> outer_buff_{};

		return [=](size_t const &idx) mutable -> T &
		{
#ifdef _DEBUG
			auto size_ = N;
			(void)size_;
#endif
			assert(idx <= outer_buff_.size());
			return outer_buff_[idx];
		};
	}

	template <typename T, size_t N>
	inline auto std_heap() noexcept
	{
		static_assert(N > 0);
		static_assert(N < dbj::mtx::MAX_HEAP_BLOCK, "max heap_block_type block is 0xFFFF");

		std::vector<T> outer_heap_instance_(N + 1);

		assert(N < outer_heap_instance_.size());

		return [=](size_t const &idx) mutable -> T &
		{
#ifdef _DEBUG
			auto size_ = N;
			(void)size_;
#endif
			assert(idx <= outer_heap_instance_.size());
			return outer_heap_instance_[idx];
		};
	}

} // dbj::bmk
