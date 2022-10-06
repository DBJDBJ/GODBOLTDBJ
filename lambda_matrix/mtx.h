#include <assert.h>
#include <stdlib.h>
#include <string.h> // memcpy

namespace dbj::mtx
{

	// enum class version { major = 6, minor = 1, patch = 0 };

	// 2022-09-29 -- banchmarking added -- comparig to std:: abstractions usage
	enum class version
	{
		major = 7,
		minor = 0,
		patch = 0
	};

	// the limits
	constexpr size_t MAX_STACK_BLOCK = 0xFF;
	constexpr size_t MAX_HEAP_BLOCK = 0xFFFF;

	// we do not need std::array
	template <typename T, size_t N>
	inline auto simple_stack() noexcept
	{
		static_assert(mtx::MAX_STACK_BLOCK > N, "max R * C for a stack block is 0xFF");
		T buff_[N]{};
		return [buff_, size = N](size_t const &idx) mutable -> T &
		{
			assert(idx < size);
			return buff_[idx];
		};
	}

	// and we do not need std::vector too
	template <typename T, size_t N>
	inline auto simple_heap() noexcept
	{
		static_assert(mtx::MAX_HEAP_BLOCK > N, "max R * C for heap_block_type block is 0xFFFF");

		// Q: is this machinery really needed?
		// A: yes it is, proper copy and move struct with pointer member is the only way
		//    also the destructor in here is the only way this is free'd
		struct heap_block_type
		{
			size_t size{N};
			// this has to be proper copy/move able struct
			// seeing bellow compiler does not know the block size
			T *block{};

			heap_block_type() noexcept : block((T *)calloc(N, sizeof(T))) { assert(block); }
			~heap_block_type() noexcept
			{
				if (block)
				{
					free(block);
					block = nullptr;
				}
			}

			void copy(heap_block_type &left, heap_block_type const &right) noexcept
			{
				assert(left.block);
				assert(left.size == right.size);
				::memcpy(left.block, right.block, left.size);
			}
			heap_block_type(heap_block_type const &other_) noexcept : block((T *)calloc(N, sizeof(T)))
			{
				copy(*this, other_);
			}
			heap_block_type &operator=(heap_block_type const &other_) noexcept
			{
				copy(*this, other_);
				return *this;
			}

			void swap(heap_block_type &left, heap_block_type &right) noexcept
			{
				using namespace std;
				left.size = right.size;
				left.block = right.block;
				right.block = nullptr;
			}
			heap_block_type(heap_block_type &&other_) noexcept
			{
				swap(*this, other_);
			}
			heap_block_type &operator=(heap_block_type &&other_) noexcept
			{
				swap(*this, other_);
				return *this;
			}
		};

		return [heap_instance_ = heap_block_type{}, size = N](size_t const &idx) mutable -> T &
		{
			assert(idx < size);
			return heap_instance_.block[idx];
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
			[arry = source_()]
			// and this will be a function call operator on that class
			(size_t row_, size_t col_) mutable -> T &
			{
				assert(row_ < rows); // from here we reach the template args
				assert(col_ < cols); // of the immediate closure
				return arry(row_ * rows + col_);
			},
			// second is for reaching to matrix dimenzions
			[=]()
			{ return std::make_piar(rows, cols); }

		);

	} // mx()

#undef dbj_mx_make
#undef dbj_mx_make_heap
#undef dbj_mx_make_stack

#define dbj_mx_make(T, R, C, K) dbj::mtx::mx<T, R, C>(dbj::mtx::K<T, (R + 1) * (C + 1)>)
#define dbj_mx_make_heap(T, R, C) dbj_mx_make(T, R, C, simple_heap)
#define dbj_mx_make_stack(T, R, C) dbj_mx_make(T, R, C, simple_stack)

// use the better version returning two lambdas
#undef dbj_mtrx_make
#undef dbj_mtrx_make_heap
#undef dbj_mtrx_make_stack

// notice here we actually call 
#define dbj_mtrx_make(T, R, C, K) dbj::mtx::mtrx<T, R, C>(dbj::mtx::K<T, (R + 1) * (C + 1)>)
#define dbj_mtrx_make_heap(T, R, C) dbj_mtrx_make(T, R, C, simple_heap)
#define dbj_mtrx_make_stack(T, R, C) dbj_mtrx_make(T, R, C, simple_stack)

} // dbj::mtx
