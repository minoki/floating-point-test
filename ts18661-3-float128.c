#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#if defined(__GNUC__)
__attribute__((noinline))
#endif
_Float128 f128_add(_Float128 x, _Float128 y)
{
    return x + y;
}

int main(void)
{
    _Float128 x = 1.0f128;
    _Float128 y = 0x1p-113f128;
    _Float128 sqrt2 = sqrtf128(2.0f128);
    char buf[1024];
    strfromf128(buf, sizeof(buf), "%a", f128_add(x, y));
    printf("1.0f128 + 0x1p-113f128 = %s\n", buf);
    strfromf128(buf, sizeof(buf), "%a", f128_add(x, y + y));
    printf("1.0f128 + (0x1p-113f128 + 0x1p-113f128) = %s\n", buf);
    strfromf128(buf, sizeof(buf), "%a", sqrt2);
    printf("sqrtf128(2.0f128) = %s\n", buf);
    strfromf128(buf, sizeof(buf), "%.40g", sqrt2);
    printf("sqrtf128(2.0f128) = %s\n", buf);
}
