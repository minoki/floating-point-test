#include <stdio.h>
#include <xmmintrin.h>

int main(void)
{
    float a = 0x1p100f;
    __m128 aa = _mm_set_ss(a);
    int i = _mm_cvt_ss2si(aa);
    printf("cvtss2si(%a) = %d (0x%08x)\n", a, i, (unsigned int)i);
}
