
#define UBENCH_IMPLEMENTATION
#include "ubut/ubench.h"

// #define DBJ_MTX_TESTING

// (c) 2019-2021 by dbj@dbj.org
// License: CC BY SA 4.0

#include "mtx.h"
#include "bmk.h"

// this is deliberate as to move the matrix in and out
// and keep its value changed
inline auto changer = [](auto matrix_, size_t row_, size_t col_, auto value_)
{
	matrix_(row_, col_) = value_;
	return matrix_;
};

#pragma region BENCHMARKING

namespace dbj::bmk
{

	namespace stack_setup
	{
		constexpr size_t
			width_ = 15,
			height_ = 15,
			last_col_ = width_ - 1,
			last_row_ = height_ - 1;
	} // stack_setup

	namespace heap_setup
	{
		constexpr size_t
			width_ = 175,
			height_ = 175,
			last_col_ = width_ - 1,
			last_row_ = height_ - 1;
	}

		// here we get two lambdas matrix holds
	inline auto driver_changer = [](auto  matrix_cell, auto matrix_dims, auto val_)
	{
		auto [last_row_,last_col_] = matrix_dims();
		// we pass the matrix by value yet it keeps its new changed state
		changer(matrix_, last_col_, last_row_, val_);
		return matrix_;
	};

} // dbj::bmk

using namespace dbj::bmk;

using value_type = int;
static value_type value_ = 42;

template<typename F>
struct TYPER{
   using TYPE_ = F;
};

UBENCH(stack, new_stuff)
{
	using T = int; 
	constexpr size_t R = stack_setup::width_;
	constexpr size_t C = stack_setup::height_;
	auto F = dbj::mtx::simple_stack<T, (R + 1) * (C + 1)> ;
	// mtrx holds two lambdas
	auto [ cell_, dims_ ] = dbj::mtx::mtrx<T, R, C>(F);
	// dbj_mtrx_make_stack(value_type, stack_setup::width_, stack_setup::height_), value_

	auto stack_nx_ = driver_changer(
		cell_, dims_, value_
		// dbj_mtrx_make_stack(value_type, stack_setup::width_, stack_setup::height_), value_
		);
}

UBENCH(heap, new_stuff)
{
	// auto heap_mx_ = driver_changer(
	// 	dbj_mtrx_make_heap(value_type, heap_setup::width_, heap_setup::height_), value_
	// 	);
}

// UBENCH(stack, std_stuff)
// {
// 	auto stack_mx_std_ = driver_changer(dbj_mtrx_make_stack_std(value_type, stack_setup::width_, stack_setup::height_), value_);
// }

// UBENCH(heap, std_stuff)
// {
// 	auto heap_mx_std_ = driver_changer(dbj_mtrx_make_heap_std(value_type, heap_setup::width_, heap_setup::height_), value_);
// }

UBENCH_STATE; // note there is no ()!
int main(int argc, const char *const argv[])
{
	system(" ");
	return ubench_main(argc, argv);
}

#pragma endregion BENCHMARKING
