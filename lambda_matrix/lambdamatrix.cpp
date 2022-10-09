// (c) 2019-2021 by dbj@dbj.org
// License: CC BY SA 4.0

#define UBENCH_IMPLEMENTATION
#include "ubut/ubench.h"

#undef DO_THE_BENCHMARKING

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
}

//--------------------------------------------------------------------------
UBENCH(stack, new_stuff)
{
	using namespace dbj::mtx;
	using T = value_type;
	constexpr size_t R = dbj::mtx::stack_setup::last_row_;
	constexpr size_t C = dbj::mtx::stack_setup::last_col_;

	auto [cell_, dims_] = stack_matrix<T, R, C>::cell_dims();

	auto stack_nx_ = dbj::driver_(
		cell_, dims_, value_);
}

UBENCH(heap, new_stuff)
{
	using namespace dbj::mtx;
	using T = value_type;
	constexpr size_t R = dbj::mtx::heap_setup::last_row_;
	constexpr size_t C = dbj::mtx::heap_setup::last_col_;

	auto [cell_, dims_] = heap_matrix<T, R, C>::cell_dims();

	auto stack_nx_ = dbj::driver_(
		cell_, dims_, value_);
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
