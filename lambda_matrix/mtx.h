#include <assert.h>
#include <stdlib.h>
#include <string.h> // memcpy
#include <utility>	// std::pair

namespace dbj::mtx
{

	// enum class version { major = 6, minor = 1, patch = 0 };

	// 2022-09-29 -- banchmarking added -- comparig to std:: abstractions usage
	enum class version
	{
		major = 7,
		minor = 3,
		patch = 0
	};

	// the limits
	constexpr size_t MAX_STACK_BLOCK = 0xFF;
	constexpr size_t MAX_HEAP_BLOCK = 0xFFFF;

	// we do not need std::array, and you?
	template <typename T, size_t N>
	inline auto simple_stack() noexcept
	{
		static_assert(mtx::MAX_STACK_BLOCK > N, "max size for a stack block is 0xFF");
		T buff_[N]{};

		// notice we return the mutable reference
		return [buff_, size = N](size_t idx) mutable -> T &
		{
#ifdef _DEBUG
			size_t dumsy_ = size;
			(void)dumsy_;
#endif
			// idx is 0 .. size
			assert(idx <= size);
			return buff_[idx];
		};
	}

	// and we do not need std::vector too
	template <typename T, size_t N>
	inline auto simple_heap() noexcept
	{
		static_assert(mtx::MAX_HEAP_BLOCK > N, "max R * C for heap_block_type block is 0xFFFF");

		return [size = N](size_t const &idx) mutable -> T &
		{
			static T(*block)[N] = []()
			{
				void *block_ = calloc(1, sizeof(T[N]));
				assert(0 != global_deleter_in_the_sky_.add(block_));
				return (T(*)[N])block_;
			}();

#ifdef _DEBUG
			size_t dumsy_ = size;
			(void)dumsy_;
#endif
			// idx is 0 .. size
			assert(idx <= size);
			return (*block)[idx];
		};
	} // simple_heap()

	// -----------------------------------------------------------------------
	// so this is the matrix as lambda
	// the issue is how to get to its size, not just the cell value?
	//
	template <typename T, size_t rows, size_t cols, typename F>
	inline constexpr auto mx(F source_)
	{
		// source_ has to be callable
		// lambda is local class
		// arry will be a member of that class
		return [arry = source_()]
			   // and this will be a function call operator on that class
			   (size_t row_, size_t col_) mutable -> T &
		{
			assert(row_ < rows); // from here we reach the template args
			assert(col_ < cols); // of the immediate closure
			return arry(row_ * rows + col_);
		};
	} // mx()

	// here is how, return pair of lambadas
	template <typename T, size_t rows, size_t cols, typename F>
	inline constexpr auto mtrx(F source_)
	{
		return std::make_pair(
			// first lambada is for matrix cell value reference
			// source_ has to be callable
			// lambda is local class
			// arry will be a member of that class
			[lambda_block_ = source_()]
			// and this will be a function call operator on that class
			(size_t row_, size_t col_) mutable -> T &
			{
#ifdef _DEBUG
				size_t dumsy_ = rows;
				(void)dumsy_;
				size_t dumsy2_ = cols;
				(void)dumsy2_;
#endif
				// rows and cols are last index
				// e.g. row_ 0..9 means there are 10 rows
				// from here we reach the template args
				// of the immediate closure
				assert(row_ <= rows);
				assert(col_ <= cols);
				return lambda_block_(row_ * (cols - 1) + col_);
			},
			// second is for reaching to matrix dimenzions
			[=](void)
			{ return std::make_pair(rows, cols); }

		);

	} // mtrx()

#undef XPND
#undef CTCAT
#undef MATRIX

#define XPND(A) A
// One level of macro indirection is required in order to resolve __COUNTER__,
// and get varname1 instead of varname__COUNTER__.
#define CTCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a##b

	// type F must be in the dbj::mtx space
	// and the mtrx template

#define MATRIX(T, R, C, F)                           \
	struct CTCAT(matrix_, F)                         \
	{                                                \
		static constexpr auto FUN = F<T, (R) * (C)>; \
		static auto cell_dim()                       \
		{                                            \
			return mtrx<T, R, C>(FUN);               \
		}                                            \
	}

	// usage

	// generate: struct matrix_simple_stack
	// MATRIX(int, 3, 3, simple_stack);

	// generate: struct matrix_simple_heap
	// MATRIX(int, 3, 3, simple_heap);

	// must avoid
	// error: static data member 'FUN' not allowed in anonymous struct
	// __COUNTER__ may have portability issues. If this is a problem, 
	// you can use __LINE__ instead and as long as you aren't calling the macro 
	// more than once per line or sharing the names across compilation units, you will be just fine.

#define MTRX(T, R, C, F)                             \
	struct CTCAT(MTX_, __COUNTER__)                  \
	{                                                \
		static constexpr auto FUN = F<T, (R) * (C)>; \
		static auto cell_dim()                       \
		{                                            \
			return dbj::mtx::mtrx<T, R, C>(FUN);     \
		}                                            \
	}

	// usage

	// using mtrx_ss = MTRX(int, 3, 3, simple_stack);
	// using mtrx_sh = MTRX(int, 3, 3, dbj::mtx::simple_heap);

} // dbj::mtx
