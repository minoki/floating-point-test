#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(__clang__)
#include <arm_acle.h>
#define get_fpcr() __arm_rsr64("fpcr")
#define set_fpcr(x) __arm_wsr64("fpcr", x)
#elif __GNUC__ >= 11
#define get_fpcr() __builtin_aarch64_get_fpcr64()
#define set_fpcr(x) __builtin_aarch64_set_fpcr64(x)
#else
#error unsupported compiler
#endif

uint64_t double_to_u64(double x)
{
    uint64_t y;
    static_assert(sizeof(x) == sizeof(y), "double and uint64_t must have the same size");
    memcpy(&y, &x, sizeof(x));
    return y;
}
double u64_to_double(uint64_t x)
{
    double y;
    static_assert(sizeof(x) == sizeof(y), "double and uint64_t must have the same size");
    memcpy(&y, &x, sizeof(x));
    return y;
}

__attribute__((noinline))
double negative_nan(void)
{
    double x = u64_to_double(UINT64_C(0xfff8000000000000));
    assert(isnan(x));
    assert(signbit(x) == 1);
    return x;
}

double my_fabs(double x)
{
    asm volatile("fabs %d0, %d1" : "=w"(x) : "w"(x));
    return x;
}

int main(void)
{
    printf("signbit(-NaN) = %d\n", signbit(negative_nan()));
    printf("signbit(fabs(-NaN)) = %d\n", signbit(fabs(negative_nan())));
    printf("signbit(my_fabs(-NaN)) = %d\n", signbit(my_fabs(negative_nan())));
    uint64_t fpcr = get_fpcr();
    set_fpcr(fpcr | 0x02);
    fpcr = get_fpcr();
    puts((fpcr & 0x02) ? "FPCR.AH = 1" : "FPCR.AH = 0");
    printf("signbit(-NaN) = %d\n", signbit(negative_nan()));
    printf("signbit(fabs(-NaN)) = %d\n", signbit(fabs(negative_nan())));
    printf("signbit(my_fabs(-NaN)) = %d\n", signbit(my_fabs(negative_nan())));
}
