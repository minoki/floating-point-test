#include <stdio.h>
#include <arm_acle.h>
#include <arm_neon.h>

int main(void)
{
#if defined(__ARM_FEATURE_BF16)
    puts("__ARM_FEATURE_BF16 is defined");
#else
    puts("__ARM_FEATURE_BF16 is not defined");
#endif
#if defined(__ARM_BF16_FORMAT_ALTERNATIVE)
    puts("__ARM_BF16_FORMAT_ALTERNATIVE is defined");
#else
    puts("__ARM_BF16_FORMAT_ALTERNATIVE is not defined");
#endif
#if defined(__ARM_BF16_FORMAT_ALTERNATIVE)
    float32_t x = 3.14f;
    bfloat16_t y = vcvth_bf16_f32(x);
    float32_t z = vcvtah_f32_bf16(y);
    printf("%a -> %a\n", x, z);
#endif
}
