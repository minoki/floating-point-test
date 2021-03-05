#include <stdio.h>
#include <quadmath.h>

#define TYPENAME(t) _Generic((t *)NULL, long double *: "long double", _Float128 *: "_Float128", default: "unknown")

__attribute__((noinline))
__float128 f128_add(__float128 x, __float128 y)
{
    return x + y;
}

int main(void)
{
    printf("__float128 is %s.\n", TYPENAME(__float128));
    __float128 x = 1.0Q;
    __float128 y = 0x1p-113Q;
    __float128 sqrt2 = sqrtq(2.0Q);
    char buf[1024];
    quadmath_snprintf(buf, sizeof(buf), "%Qa", f128_add(x, y));
    printf("1.0Q + 0x1p-113Q = %s\n", buf);
    quadmath_snprintf(buf, sizeof(buf), "%Qa", f128_add(x, y + y));
    printf("1.0Q + (0x1p-113Q + 0x1p-113Q) = %s\n", buf);
    quadmath_snprintf(buf, sizeof(buf), "%Qa", sqrt2);
    printf("sqrtq(2.0Q) = %s\n", buf);
    quadmath_snprintf(buf, sizeof(buf), "%.40Qg", sqrt2);
    printf("sqrtq(2.0Q) = %s\n", buf);
}
