#include <stdio.h>
#include <math.h>
#include <fenv.h>
#include <stdbool.h>

#pragma STDC FENV_ACCESS ON

void raise_invalid_on_fma_with_nan(void)
{
    volatile double x = 0.0;
    volatile double y = INFINITY;
    volatile double z = NAN;
    feclearexcept(FE_INVALID);
    volatile double w = fma(x, y, z);
    bool invalid = fetestexcept(FE_INVALID) != 0;
    printf("fma(%g, %g, %g), which yields %g, %s\n", x, y, z, w, invalid ? "raises INVALID" : "does not raise INVALID");
}

void underflow_after_rounding(void)
{
    volatile double x = 0x1.0000001p-1022;
    volatile double y = 0x0.fffffffp0;
    // x * y = 0x0.ffffffffffffffp-1022 -(rounding)-> 0x1.0000000000000p-1022
    feclearexcept(FE_UNDERFLOW);
    volatile double z = x * y;
    bool underflow = fetestexcept(FE_UNDERFLOW) != 0;
    printf("%a * %a, which yields %a, %s\n", x, y, z, underflow ? "raises UNDERFLOW; underflow is detected before rounding" : "does not raise UNDERFLOW; underflow is detected after rounding");
}

int main(void)
{
#if defined(FP_FAST_FMA)
    puts("FP_FAST_FMA is defined");
#else
    puts("FP_FAST_FMA is not defined");
#endif
    raise_invalid_on_fma_with_nan();
    underflow_after_rounding();
}
