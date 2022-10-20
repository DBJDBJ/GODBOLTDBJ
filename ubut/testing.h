///
/// Testing
///
#pragma region DBJ_MTX_TESTING

#include <iomanip> // setw
#include <sstream>
#include <iostream>
#include <string_view>

#if 0
// log callback signature: void (*)( std::string_view  )
inline void log_(std::string_view sv)
{
	printf("%s", sv.data());
}

// print to buffer
// NOTE: compared to printf, this is almost tragi-comical code
inline auto
	printer = [](auto matrix_, size_t width_, size_t height_)
	-> std::string
{
	using namespace std;
	std::ostringstream rez_;
	rez_ << std::boolalpha;
	for (size_t row_ = 0; row_ < width_; row_++)
	{
		rez_ << "\n";
		for (size_t col_ = 0; col_ < height_; col_++)
		{
			rez_ << setw(3) << matrix_(row_, col_) << " ";
		}
	}
	rez_ << "\n";
	return rez_.str();
};


#else
// no console -------------------------------------------------------------
inline void log_(std::string_view sv)
{
	// printf("%s", sv.data());
}

// print to buffer
// NOTE: compared to printf, this is almost tragi-comical code
inline auto
	printer = [](auto matrix_, size_t width_, size_t height_)
	-> std::string
{
	using namespace std;
	std::ostringstream rez_;
	rez_ << "";
	return rez_.str();
};

#endif


// test move in, move out and the rest
inline auto driver_printer = [&](auto matrix_, const char *prompt_)
{
	string report = prompt_ +
					printer(changer(matrix_, last_col_, last_row_, val_), width_, height_);
	log(report);
	return matrix_;
};

///
/// log callback signature:
/// void (*)( std::string_view  )
///
template <
	typename log_callback,
	typename value_type,
	size_t width_ = 3,			   // 0,1,2
	size_t height_ = 3,			   // 0,1,2
	size_t last_col_ = width_ - 1, // 0,1,2
	size_t last_row_ = height_ - 1 // 0,1,2
	>
inline void test_dbj_matrix_creation(log_callback log, value_type const &val_)
{
	using namespace dbj::mtx;
	using namespace std;

	auto stack_mx_ = driver_printer(dbj_mx_make_stack(value_type, width_, height_),
									"\n matrix_on_stack \n");

	auto heap_mx_ = driver_printer(dbj_mx_make_heap(value_type, width_, height_),
								   "\n matrix_on_heap \n");
}

template <
	typename log_callback,
	typename value_type,
	size_t width_ = 3,			   // 0,1,2
	size_t height_ = 3,			   // 0,1,2
	size_t last_col_ = width_ - 1, // 0,1,2
	size_t last_row_ = height_ - 1 // 0,1,2
	>
inline void test_dbj_matrix_creation_std(log_callback log, value_type const &val_)
{
	using namespace dbj::mtx;
	using namespace std;

	auto stack_mx_std_ = driver_printer(dbj_mx_make_stack_std(value_type, width_, height_),
										"\n matrix_on_stack STD \n");

	auto heap_mx_std_ = driver_printer(dbj_mx_make_heap_std(value_type, width_, height_),
									   "\n matrix_on_heap STD \n");
}

inline void console_hosted_tests(void)
{
	log_("\nint type 2D array ---------------------------------------------------");
	test_dbj_matrix_creation(log_, 42);
	log_("\nbool type 2D array ---------------------------------------------------");
	test_dbj_matrix_creation(log_, true);
	
	log_("\nSTD int type 2D array ---------------------------------------------------");
	test_dbj_matrix_creation_std(log_, 42);
	log_("\nSTD bool type 2D array ---------------------------------------------------");
	test_dbj_matrix_creation_std(log_, true);
}

#pragma endregion
 // DBJ_MTX_TESTING