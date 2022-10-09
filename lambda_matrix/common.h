#pragma once

namespace dbj
{

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