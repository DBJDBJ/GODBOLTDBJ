/**
 *  Refactoring https://floating-point-gui.de/errors/comparison/
 * 
 * 	public static boolean nearlyEqual(float a, float b, float epsilon) {
		final float absA = Math.abs(a);
		final float absB = Math.abs(b);
		final float diff = Math.abs(a - b);

		if (a == b) { // shortcut, handles infinities
			return true;
    } else if (a == 0 || b == 0 || (absA + absB < Float.MIN_NORMAL)) {
			// a or b is zero or both are extremely close to it
			// relative error is less meaningful here
			return diff < (epsilon * Float.MIN_NORMAL);
		} else { // use relative error
			return diff / Math.min((absA + absB), Float.MAX_VALUE) < epsilon;
		}
	}
*/
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <float.h>

#define FX_WIDTH "32"
#define FX(F,X) printf("\n%"FX_WIDTH"s : " F,(#X),(X))
#define FXB(X) printf("\n%"FX_WIDTH"s : %s",(#X),(X)? "true" : "false")

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

int main(void)    
{
    float epsilong = 0.00001f;

    FXB( nearly_equal( FLT_MIN, FLT_MIN, epsilong) );
}