// (c) 2019-2021 by dbj@dbj.org
// License: CC BY SA 4.0

#define UBENCH_IMPLEMENTATION
#include "ubut/ubench.h"

#define DO_THE_BENCHMARKING

#include "common.h"
#include "mtx.h"
#ifdef DO_THE_BENCHMARKING
#include "bmk.h"
#endif // DO_THE_BENCHMARKING

#pragma region BENCHMARKING

namespace dbj::mtx
{
	using stack_setup = setup<10, 10>;
	using heap_setup = setup<175, 175>;

	using value_type = int;
	static value_type value_ = value_type{42};

	using T = value_type;
	constexpr size_t RS = dbj::mtx::stack_setup::last_row_;
	constexpr size_t CS = dbj::mtx::stack_setup::last_col_;

	constexpr size_t RH = dbj::mtx::heap_setup::last_row_;
	constexpr size_t CH = dbj::mtx::heap_setup::last_col_;

	// struct matrix_simple_stack
	MATRIX(int, RS, CS, simple_stack);

	// struct matrix_simple_heap
	MATRIX(int, RH, CH, simple_heap);

	// struct matrix_std_stack
	MATRIX(int, RS, CS, std_stack);

	// struct matrix_std_heap
	MATRIX(int, RH, CH, std_heap);
}

//--------------------------------------------------------------------------
UBENCH(stack, new_stuff)
{
	auto [cell_, dims_] = dbj::mtx::matrix_simple_stack::cell_dim();

	auto stack_nx_ = dbj::driver_(cell_, dims_, dbj::mtx::value_);
}

UBENCH(heap, new_stuff)
{
	auto [cell_, dims_] = dbj::mtx::matrix_simple_heap::cell_dim();

	auto stack_nx_ = dbj::driver_(cell_, dims_, dbj::mtx::value_);
}

UBENCH(stack, std_stuff)
{
	auto [cell_, dims_] = dbj::mtx::matrix_std_stack::cell_dim();

	auto stack_nx_ = dbj::driver_(cell_, dims_, dbj::mtx::value_);
}

UBENCH(heap, std_stuff)
{
	auto [cell_, dims_] = dbj::mtx::matrix_std_heap::cell_dim();

	auto stack_nx_ = dbj::driver_(cell_, dims_, dbj::mtx::value_);
}

UBENCH_STATE; // note there is no ()!
int main(int argc, const char *const argv[])
{
	system(" ");
	return ubench_main(argc, argv);
}

#pragma endregion BENCHMARKING
