#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <arm_acle.h>

// Prototype:
//   int32_t __jcvt(double);

#if defined(__GNUC__)
__attribute__((noinline))
#endif
int32_t cast_double_to_i32(double x)
{
    return (int32_t)x;
}

int main(void)
{
    printf("(int32_t)(-2.8) = %" PRId32 "\n", cast_double_to_i32(-2.8));
    printf("(int32_t)1.99 = %" PRId32 "\n", cast_double_to_i32(1.99));
    printf("(int32_t)(-Infinity) = %" PRId32 "\n", cast_double_to_i32(-INFINITY));
    printf("(int32_t)Infinity = %" PRId32 "\n", cast_double_to_i32(INFINITY));
    printf("(int32_t)NaN = %" PRId32 "\n", cast_double_to_i32(NAN));
    printf("(int32_t)(0x1p50 + 123.0) = %" PRId32 "\n", cast_double_to_i32(0x1p50+123.0));
#if defined(__ARM_FEATURE_JCVT)
    printf("__jcvt(-2.8) = %" PRId32 "\n", __jcvt(-2.8)); // -> -2
    printf("__jcvt(1.99) = %" PRId32 "\n", __jcvt(1.99)); // -> 1
    printf("__jcvt(-Infinity) = %" PRId32 "\n", __jcvt(-INFINITY)); // -> 0
    printf("__jcvt(Infinity) = %" PRId32 "\n", __jcvt(INFINITY)); // -> 0
    printf("__jcvt(NaN) = %" PRId32 "\n", __jcvt(NAN)); // -> 0
    printf("__jcvt(0x1p50 + 123.0) = %" PRId32 "\n", __jcvt(0x1p50+123.0)); // -> 123
#else
    puts("__jcvt is not available");
#endif
}
