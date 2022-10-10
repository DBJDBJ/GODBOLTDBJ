#pragma once

#include <assert.h>

namespace dbj
{

	class global_registrar
	{
		constexpr static size_t size = 0xFF;
		global_registrar() = default;
		global_registrar(global_registrar const &) = delete;
		global_registrar &operator=(const global_registrar &) = delete;
		global_registrar(global_registrar &&) = delete;
		global_registrar &operator=(global_registrar &&) = delete;

		void *blocks[size]{};
		int last_index = 0;

	public:
		// return what was made or null if NEM
		void *next(size_t count_, size_t size_) noexcept
		{
			if (last_index == size)
				return 0;

			void *next_block_ = calloc(count_, size_);

			if (next_block_)
			{
				blocks[last_index] = next_block_;
				last_index += 1;
			}
			return next_block_;
		}

		void release() noexcept
		{
			// last_index is one beyond the last occupied slot
			while (--last_index > -1)
			{
				free(blocks[last_index]);
				blocks[last_index] = 0;
			}
		}

		~global_registrar() noexcept
		{
			release();
		}

		static global_registrar &make(void)
		{
			static global_registrar deleter_;
			return deleter_;
		}
	};

	//--------------------------------------------------------------------------------
	inline auto &global_registrar_in_the_sky_ = global_registrar::make();
	//--------------------------------------------------------------------------------

	// this is deliberate as to move the matrix in and out
	// and keep its value changed
	inline auto changer = [](auto cell_, size_t row_, size_t col_, auto value_)
	{
		// this returns reference to the value stored
		cell_(row_, col_) = value_;
		return cell_;
	};

	// here we get two lambdas matrix holds
	inline auto driver_ = [](auto matrix_cell, auto matrix_dims, auto val_)
	{
		auto [rows_, cols_] = matrix_dims();
		// we pass the matrix by value yet it keeps its new changed state
		return changer(matrix_cell, rows_, cols_, val_);
	};

	template <size_t W, size_t H>
	struct setup
	{
		static constexpr size_t
			width_ = W,				// thus 0 .. 9
			height_ = H,			//  thus 0 .. 9
			last_col_ = width_ - 1, // yes this is the last index
			last_row_ = height_ - 1;
	}; // setup

} // dbj NS

// Q: is this machinery really needed?
// A: not sure
// struct heap_block_type
// {
// 	size_t size{N};
// 	// this has to be proper copy/move able struct
// 	// seeing bellow compiler does not know the block size
// 	T(*block)
// 	[N]
// 	{ 0 };

// 	heap_block_type() noexcept : block((T(*)[N])calloc(1, sizeof(T[N]))) { assert(block); }
// 	~heap_block_type() noexcept
// 	{
// 		if (block)
// 		{
// 			free(block);
// 			block = nullptr;
// 		}
// 	}

// 	void copy(heap_block_type &left, heap_block_type const &right) noexcept
// 	{
// 		assert(left.block);
// 		assert(left.size == right.size);
// 		::memcpy(left.block, right.block, left.size);
// 	}
// 	heap_block_type(heap_block_type const &other_) noexcept : block((T(*)[N])calloc(1, sizeof(T[N])))
// 	{
// 		assert(block);
// 		copy(*this, other_);
// 	}
// 	heap_block_type &operator=(heap_block_type const &other_) noexcept
// 	{
// 		copy(*this, other_);
// 		return *this;
// 	}

// 	void swap(heap_block_type &left, heap_block_type &right) noexcept
// 	{
// 		using namespace std;
// 		left.size = right.size;
// 		left.block = right.block;
// 		right.block = nullptr;
// 	}
// 	heap_block_type(heap_block_type &&other_) noexcept
// 	{
// 		swap(*this, other_);
// 	}
// 	heap_block_type &operator=(heap_block_type &&other_) noexcept
// 	{
// 		swap(*this, other_);
// 		return *this;
// 	}
// };