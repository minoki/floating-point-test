#include <inttypes.h>
#include <stdio.h>

#if defined(__GNUC__)
__attribute__((noinline))
#endif
double f32_to_f64(float x)
{
    return (double)x;
}

#if defined(__GNUC__)
__attribute__((noinline))
#endif
float f64_to_f32(double x)
{
    return (float)x;
}

float u32_to_f32(uint32_t i)
{
    union {
        uint32_t i;
        float f;
    } volatile u;
    u.i = i;
    return u.f;
}

double u64_to_f64(uint64_t i)
{
    union {
        uint64_t i;
        double f;
    } volatile u;
    u.i = i;
    return u.f;
}

uint32_t f32_to_u32(float f)
{
    union {
        uint32_t i;
        float f;
    } volatile u;
    u.f = f;
    return u.i;
}

uint64_t f64_to_u64(double f)
{
    union {
        uint64_t i;
        double f;
    } volatile u;
    u.f = f;
    return u.i;
}

int main(void)
{
    float sNaNf = u32_to_f32(0x7f800001);
    double sNaNd = u64_to_f64(UINT64_C(0x7ff0000000000001));
    double y = f32_to_f64(sNaNf);
    float z = f64_to_f32(y);
    float w = f64_to_f32(sNaNd);
    printf("sNaNf=%" PRIx32 " (%g)\n", f32_to_u32(sNaNf), sNaNf);
    printf("(double)sNaNf=%" PRIx64 "\n", f64_to_u64(y));
    printf("(float)(double)sNaNf=%" PRIx32 "\n", f32_to_u32(z));
    printf("sNaNd=%" PRIx64 " (%g)\n", f64_to_u64(sNaNd), sNaNd);
    printf("(float)sNaNd=%" PRIx32 "\n", f32_to_u32(w));
}

/*
Output on AArch64:

sNaNf=7f800001 (nan)
(double)sNaNf=7ff8000020000000
(float)(double)sNaNf=7fc00001
sNaNd=7ff0000000000001 (nan)
(float)sNaNd=7fc00000

Output on PPC64LE:

sNaNf=7f800001 (nan)
(double)sNaNf=7ff0000020000000
(float)(double)sNaNf=7fc00001
sNaNd=7ff0000000000001 (nan)
(float)sNaNd=7fc00000
*/
