
// for benchmarking
#include <array>
#include <vector>

// Banch MarKing == bmk
namespace dbj::bmk
{

	template <typename T, size_t N>
	inline auto simple_stack() noexcept
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
	inline auto simple_heap() noexcept
	{
		static_assert(N < dbj::mtx::MAX_HEAP_BLOCK, "max heap_block_type block is 0xFFFF");

		std::vector<T> outer_heap_instance_{N};

		return [heap_instance_ = outer_heap_instance_, size = N](size_t const &idx) mutable -> T &
		{
			assert(idx < size);
			return heap_instance_[idx];
		};
	}

#undef dbj_mx_make_std
#undef dbj_mx_make_heap_std
#undef dbj_mx_make_stack_std

#define dbj_mx_make_std(T, R, C, K) dbj::mtx::mx<T, R, C>(dbj::bmk::K<T, (R + 1) * (C + 1)>)
#define dbj_mx_make_heap_std(T, R, C) dbj_mx_make(T, R, C, simple_heap)
#define dbj_mx_make_stack_std(T, R, C) dbj_mx_make(T, R, C, simple_stack)

} // dbj::bmk