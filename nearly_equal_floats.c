/*
 * Note: this function attempts a "one size fits all" solution. There may be
 * some edge cases for which it still produces unexpected results, and some of
 * the tests it was developed to pass probably specify behaviour that is not
 * appropriate for some applications, especially concerning very small values
 * with differing signs.
 *
 * Before using it, make sure it's appropriate for your application!
 *
 * From http://floating-point-gui.de, @author Michael Borgwardt
 *
 *  2022 NOV 16 dbj@dbj.org -- Refactoring https://floating-point-gui.de/errors/comparison/
 * 
 */
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <float.h>

#define FX_WIDTH "32"
#define FX(F,X) printf("\n%"FX_WIDTH"s : " F,(#X),(X))
#define FXB(X) printf("\n%"FX_WIDTH"s : %s",(#X),(X)? "true" : "false")

/*
***********************************************************************************

***********************************************************************************
*/
 __inline bool nearly_equal(float a, float b, float epsilon) {
		const float absA = fabs(a);
		const float absB = fabs(b);
		const float diff = fabs(a - b);

		if (a == b) { // shortcut, handles infinities
			return true;
    } else if (a == 0 || b == 0 || (absA + absB < FLT_MIN)) {
			// a or b is zero or both are extremely close to it
			// relative error is less meaningful here
			return diff < (epsilon * FLT_MIN);
		} else { // use relative error
			return diff / fmin((absA + absB), FLT_MAX ) < epsilon;
		}
	}
#define UTEST_IMPLEMENTATION
#include "ubut/utest.h"
/*
***********************************************************************************

***********************************************************************************
*/

    __inline bool nearly_equal_proxy(float a, float b) 
    {
             // this makes many tests fail
             // return a == b ;
             // and this is almost perfect
             return nearly_equal(a,b, 0.001f);
             // return nearly_equal(a,b, FLT_EPSILON);
    }

    /** Regular large numbers - generally not problematic */
    UTEST( nearly_equal, TEST_0) { 
        ASSERT_TRUE(nearly_equal_proxy(1000000.0f, 1000001.0f));
        ASSERT_TRUE(nearly_equal_proxy(1000001.0f, 1000000.0f));
        ASSERT_FALSE(nearly_equal_proxy(10000.0f, 10001.0f));
        ASSERT_FALSE(nearly_equal_proxy(10001.0f, 10000.0f));
    }

    /** Negative large numbers */
    UTEST( nearly_equal, TEST_1) { 
        ASSERT_TRUE(nearly_equal_proxy(-1000000.0f, -1000001.0f));
        ASSERT_TRUE(nearly_equal_proxy(-1000001.0f, -1000000.0f));
        ASSERT_FALSE(nearly_equal_proxy(-10000.0f, -10001.0f));
        ASSERT_FALSE(nearly_equal_proxy(-10001.0f, -10000.0f));
    }

    /** Numbers around 1 */
    UTEST( nearly_equal, TEST_2) { 
        ASSERT_TRUE(nearly_equal_proxy(1.0000001f, 1.0000002f));
        ASSERT_TRUE(nearly_equal_proxy(1.0000002f, 1.0000001f));
        ASSERT_FALSE(nearly_equal_proxy(1.0002f, 1.0001f));
        ASSERT_FALSE(nearly_equal_proxy(1.0001f, 1.0002f));
    }

    /** Numbers around -1 */
    UTEST( nearly_equal, TEST_3) { 
        ASSERT_TRUE(nearly_equal_proxy(-1.000001f, -1.000002f));
        ASSERT_TRUE(nearly_equal_proxy(-1.000002f, -1.000001f));
        ASSERT_FALSE(nearly_equal_proxy(-1.0001f, -1.0002f));
        ASSERT_FALSE(nearly_equal_proxy(-1.0002f, -1.0001f));
    }

    /** Numbers between 1 and 0 */
    UTEST( nearly_equal, TEST_4) { 
        ASSERT_TRUE(nearly_equal_proxy(0.000000001000001f, 0.000000001000002f));
        ASSERT_TRUE(nearly_equal_proxy(0.000000001000002f, 0.000000001000001f));
        ASSERT_FALSE(nearly_equal_proxy(0.000000000001002f, 0.000000000001001f));
        ASSERT_FALSE(nearly_equal_proxy(0.000000000001001f, 0.000000000001002f));
    }

    /** Numbers between -1 and 0 */
    UTEST( nearly_equal, TEST_5) { 
        ASSERT_TRUE(nearly_equal_proxy(-0.000000001000001f, -0.000000001000002f));
        ASSERT_TRUE(nearly_equal_proxy(-0.000000001000002f, -0.000000001000001f));
        ASSERT_FALSE(nearly_equal_proxy(-0.000000000001002f, -0.000000000001001f));
        ASSERT_FALSE(nearly_equal_proxy(-0.000000000001001f, -0.000000000001002f));
    }

    /** Small differences away from zero */
    UTEST( nearly_equal, TEST_6) { 
        ASSERT_TRUE(nearly_equal_proxy(0.3f, 0.30000003f));
        ASSERT_TRUE(nearly_equal_proxy(-0.3f, -0.30000003f));
    }

    /** Comparisons involving zero */
    UTEST( nearly_equal, TEST_7) { 
        ASSERT_TRUE(nearly_equal_proxy(0.0f, 0.0f));
        ASSERT_TRUE(nearly_equal_proxy(0.0f, -0.0f));
        ASSERT_TRUE(nearly_equal_proxy(-0.0f, -0.0f));
        ASSERT_FALSE(nearly_equal_proxy(0.00000001f, 0.0f));
        ASSERT_FALSE(nearly_equal_proxy(0.0f, 0.00000001f));
        ASSERT_FALSE(nearly_equal_proxy(-0.00000001f, 0.0f));
        ASSERT_FALSE(nearly_equal_proxy(0.0f, -0.00000001f));

        ASSERT_TRUE(nearly_equal(0.0f, 1e-40f, FLT_EPSILON));
        ASSERT_TRUE(nearly_equal(1e-40f, 0.0f, FLT_EPSILON));
        ASSERT_FALSE(nearly_equal(1e-40f, 0.0f, FLT_EPSILON));
        ASSERT_FALSE(nearly_equal(0.0f, 1e-40f, FLT_EPSILON));

        ASSERT_TRUE(nearly_equal(0.0f, -1e-40f, FLT_EPSILON));
        ASSERT_TRUE(nearly_equal(-1e-40f, 0.0f, FLT_EPSILON));
        ASSERT_FALSE(nearly_equal(-1e-40f, 0.0f, FLT_EPSILON));
        ASSERT_FALSE(nearly_equal(0.0f, -1e-40f, FLT_EPSILON));
    }

    /**
     * Comparisons involving extreme values (overflow potential)
     */
    UTEST( nearly_equal, TEST_8) { 
        ASSERT_TRUE(nearly_equal_proxy(FLT_MAX, FLT_MAX));
        ASSERT_FALSE(nearly_equal_proxy(FLT_MAX, -FLT_MAX));
        ASSERT_FALSE(nearly_equal_proxy(-FLT_MAX, FLT_MAX));
        ASSERT_FALSE(nearly_equal_proxy(FLT_MAX, FLT_MAX / 2));
        ASSERT_FALSE(nearly_equal_proxy(FLT_MAX, -FLT_MAX / 2));
        ASSERT_FALSE(nearly_equal_proxy(-FLT_MAX, FLT_MAX / 2));
    }

    /**
     * Comparisons involving infinities
     */
    UTEST( nearly_equal, TEST_9) { 
        ASSERT_TRUE(nearly_equal_proxy(INFINITY, INFINITY));
        ASSERT_FALSE(nearly_equal_proxy(INFINITY, FLT_MAX));
    }

    /**
     * Comparisons involving NaN values
     */
    UTEST( nearly_equal, TEST_10) { 
        ASSERT_FALSE(nearly_equal_proxy(NAN, NAN));
        ASSERT_FALSE(nearly_equal_proxy(NAN, 0.0f));
        ASSERT_FALSE(nearly_equal_proxy(-0.0f, NAN));
        ASSERT_FALSE(nearly_equal_proxy(NAN, -0.0f));
        ASSERT_FALSE(nearly_equal_proxy(0.0f, NAN));
        ASSERT_FALSE(nearly_equal_proxy(NAN, INFINITY));
        ASSERT_FALSE(nearly_equal_proxy(INFINITY, NAN));
        ASSERT_FALSE(nearly_equal_proxy(NAN, FLT_MAX));
        ASSERT_FALSE(nearly_equal_proxy(FLT_MAX, NAN));
        ASSERT_FALSE(nearly_equal_proxy(NAN, -FLT_MAX));
        ASSERT_FALSE(nearly_equal_proxy(-FLT_MAX, NAN));
        ASSERT_FALSE(nearly_equal_proxy(NAN, FLT_MIN));
        ASSERT_FALSE(nearly_equal_proxy(FLT_MIN, NAN));
        ASSERT_FALSE(nearly_equal_proxy(NAN, -FLT_MIN));
        ASSERT_FALSE(nearly_equal_proxy(-FLT_MIN, NAN));
    }

    /** Comparisons of numbers on opposite sides of 0 */
    UTEST( nearly_equal, TEST_11) { 
        ASSERT_FALSE(nearly_equal_proxy(1.000000001f, -1.0f));
        ASSERT_FALSE(nearly_equal_proxy(-1.0f, 1.000000001f));
        ASSERT_FALSE(nearly_equal_proxy(-1.000000001f, 1.0f));
        ASSERT_FALSE(nearly_equal_proxy(1.0f, -1.000000001f));
        ASSERT_TRUE(nearly_equal_proxy(10 * FLT_MIN, 10 * -FLT_MIN));
        ASSERT_FALSE(nearly_equal_proxy(10000 * FLT_MIN, 10000 * -FLT_MIN));
    }

    /**
     * The really tricky part - comparisons of numbers very close to zero.
     */
    UTEST( nearly_equal, TEST_12) { 
        ASSERT_TRUE(nearly_equal_proxy(FLT_MIN, FLT_MIN));
        ASSERT_TRUE(nearly_equal_proxy(FLT_MIN, -FLT_MIN));
        ASSERT_TRUE(nearly_equal_proxy(-FLT_MIN, FLT_MIN));
        ASSERT_TRUE(nearly_equal_proxy(FLT_MIN, 0));
        ASSERT_TRUE(nearly_equal_proxy(0, FLT_MIN));
        ASSERT_TRUE(nearly_equal_proxy(-FLT_MIN, 0));
        ASSERT_TRUE(nearly_equal_proxy(0, -FLT_MIN));

        ASSERT_FALSE(nearly_equal_proxy(0.000000001f, -FLT_MIN));
        ASSERT_FALSE(nearly_equal_proxy(0.000000001f, FLT_MIN));
        ASSERT_FALSE(nearly_equal_proxy(FLT_MIN, 0.000000001f));
        ASSERT_FALSE(nearly_equal_proxy(-FLT_MIN, 0.000000001f));
    }

/*
***********************************************************************************

***********************************************************************************
*/
  UTEST_STATE;   // <-- note there is no ()! 
  int main(int argc, const char *const argv[]) {
	return utest_main(argc, argv);          
  }