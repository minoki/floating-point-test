#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <arm_acle.h>

// Prototype:
//   int32_t __jcvt(double);

int main(void)
{
    printf("(int32_t)1.99 = %" PRId32 "\n", (int32_t)1.99); // -> 1
    printf("(int32_t)(-Infinity) = %" PRId32 "\n", (int32_t)(-INFINITY)); // -> 0
    printf("(int32_t)Infinity = %" PRId32 "\n", (int32_t)INFINITY); // -> 0
    printf("(int32_t)(0x1p50 + 123.0) = %" PRId32 "\n", (int32_t)(0x1p50+123.0)); // -> 123
#if defined(__ARM_FEATURE_JCVT)
    printf("__jcvt(1.99) = %" PRId32 "\n", __jcvt(1.99)); // -> 1
    printf("__jcvt(-Infinity) = %" PRId32 "\n", __jcvt(-INFINITY)); // -> 0
    printf("__jcvt(Infinity) = %" PRId32 "\n", __jcvt(INFINITY)); // -> 0
    printf("__jcvt(0x1p50 + 123.0) = %" PRId32 "\n", __jcvt(0x1p50+123.0)); // -> 123
#else
    puts("__jcvt is not available");
#endif
}
