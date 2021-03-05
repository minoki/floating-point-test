#include <stdio.h>
#include <math.h>
#include <float.h>

#if defined(__GNUC__)
__attribute__((noinline))
#endif
long double ld_add(long double x, long double y)
{
    return x + y;
}

int main(void)
{
    if (LDBL_MANT_DIG == 64 && LDBL_MIN_EXP == -16381 && LDBL_MAX_EXP == 16384) {
        puts("long double is probably x87 80-bit extended double format");
    } else if (LDBL_MANT_DIG == 113 && LDBL_MIN_EXP == -16381 && LDBL_MAX_EXP == 16384) {
        puts("long double is probably IEEE binary128 format");
    } else if (LDBL_MANT_DIG == 53 && LDBL_MIN_EXP == -1021 && LDBL_MAX_EXP == 1024) {
        puts("long double is probably IEEE binary64 format");
    } else {
        printf("Unknown long double format (MANT_DIG=%d, MIN_EXP=%d, MAX_EXP=%d)\n", LDBL_MANT_DIG, LDBL_MIN_EXP, LDBL_MAX_EXP);
    }
#if defined(__riscv_flen)
    printf("__riscv_flen = %d\n", __riscv_flen);
#endif
    long double x = 1.0L;
    long double y = 0x1p-113L;
    long double sqrt2 = sqrtl(2.0L);
    printf("1.0L + 0x1p-113L = %La\n", ld_add(x, y));
    printf("1.0L + (0x1p-113L + 0x1p-113L) = %La\n", ld_add(x, y + y));
    printf("1.0L + (0x1p-113L + 0x1p-113L) = %.40Lg\n", ld_add(x, y + y));
    printf("sqrtl(2.0L) = %La\n", sqrt2);
    printf("sqrtl(2.0L) = %.40Lg\n", sqrt2);
}
