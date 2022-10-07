
#define UBENCH_IMPLEMENTATION
#include "ubut/ubench.h"

// #define DBJ_MTX_TESTING

// (c) 2019-2021 by dbj@dbj.org
// License: CC BY SA 4.0

#undef DO_THE_BENCHMARKING

#include "mtx.h"
#ifdef DO_THE_BENCHMARKING
#include "bmk.h"
#endif // DO_THE_BENCHMARKING

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
			width_ = 10, // thus 0 .. 9
			height_ = 10, //  thus 0 .. 9
			last_col_ = width_ - 1, // yes this is the last index
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
	inline auto driver_ = [](auto  matrix_cell, auto matrix_dims, auto val_)
	{
		auto [rows_,cols_] = matrix_dims();
		// we pass the matrix by value yet it keeps its new changed state
		changer(matrix_cell, rows_, cols_, val_);
		return matrix_cell;
	};

} // dbj::bmk

using namespace dbj::bmk;

using value_type = int;
static value_type value_ = 42;

template<typename T,size_t R, size_t C >
struct stack_matrix {
   static constexpr auto F = dbj::mtx::simple_stack<T, (R /*+ 1*/) * (C /* + 1 */)> ;
   	// mtrx holds two lambdas
	static auto cell_dims () {
	/*auto [ cell_, dims_ ] = */ return  dbj::mtx::mtrx<T, R, C>(F);
	}
};

template<typename T,size_t R, size_t C >
struct heap_matrix {
   static constexpr auto F = dbj::mtx::simple_heap<T, (R /*+ 1*/) * (C /*+ 1*/)> ;
   	// mtrx holds two lambdas
	static auto cell_dims () {
	/*auto [ cell_, dims_ ] = */ return  dbj::mtx::mtrx<T, R, C>(F);
	}
};
//--------------------------------------------------------------------------
UBENCH(stack, new_stuff)
{
	using T = int; 
	constexpr size_t R = stack_setup::last_row_;
	constexpr size_t C = stack_setup::last_col_;

     auto [ cell_, dims_ ] = stack_matrix<T,R,C>::cell_dims() ;

	auto stack_nx_ = driver_(
		cell_, dims_, value_
		);
}

UBENCH(heap, new_stuff)
{
	using T = int; 
	constexpr size_t R = heap_setup::last_row_;
	constexpr size_t C = heap_setup::last_col_;

     auto [ cell_, dims_ ] = heap_matrix<T,R,C>::cell_dims() ;

	auto stack_nx_ = driver_(
		cell_, dims_, value_
		);
}

// UBENCH(stack, std_stuff)
// {
// 	auto stack_mx_std_ = driver_(dbj_mtrx_make_stack_std(value_type, stack_setup::width_, stack_setup::height_), value_);
// }

// UBENCH(heap, std_stuff)
// {
// 	auto heap_mx_std_ = driver_(dbj_mtrx_make_heap_std(value_type, heap_setup::width_, heap_setup::height_), value_);
// }

UBENCH_STATE; // note there is no ()!
int main(int argc, const char *const argv[])
{
	system(" ");
	return ubench_main(argc, argv);
}

#pragma endregion BENCHMARKING
