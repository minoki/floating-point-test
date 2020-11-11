#include <assert.h>
#include <float.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#if defined(__GNUC__)
#define NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE
#endif

static_assert(sizeof(double) == 8 && FLT_RADIX == 2 && DBL_MANT_DIG == 53 && DBL_MIN_EXP == -1021 && DBL_MAX_EXP == 1024, "double must be IEEE binary64 format");

double u64_to_double(uint64_t u64)
{
    union {
        uint64_t u64;
        double dbl;
    } u;
    u.u64 = u64;
    return u.dbl;
}
uint64_t double_to_u64(double x)
{
    union {
        uint64_t u64;
        double dbl;
    } u;
    u.dbl = x;
    return u.u64;
}

NOINLINE
double add(volatile double x, volatile double y)
{
    return x + y;
}

NOINLINE
double mul(volatile double x, volatile double y)
{
    return x * y;
}

NOINLINE
double fma_noinline(volatile double x, volatile double y, volatile double z)
{
    return fma(x, y, z);
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        if (strcmp(argv[1], "--default-nan") == 0) {
#if defined(__aarch64__) && defined(__GNUC__)
            unsigned int fpcr = __builtin_aarch64_get_fpcr();
            __builtin_aarch64_set_fpcr(fpcr | (1u << 25));
            fpcr = __builtin_aarch64_get_fpcr();
            if (fpcr & (1u << 25)) {
                puts("Default NaN mode set");
            } else {
                puts("Failed to set Default NaN mode");
            }
#else
            fputs("Default NaN mode is not supported on this platform\n", stderr);
#endif
        } else if (strcmp(argv[1], "--help") == 0) {
            fprintf(stderr, "Usage: %s [options]\n"
                    "Supported options:"
                    "  --help         Print this message.\n"
                    "  --default-nan  Enable Default NaN mode on AArch64\n", argv[0]);
            return 1;
        } else {
            fprintf(stderr, "Unrecognized option: %s\n", argv[1]);
        }
    }
    uint64_t nan_pattern = double_to_u64(NAN);
    if (nan_pattern & (UINT64_C(1) << 51)) {
        printf("The representation of quiet NaN follows IEEE 754-2008 recommendation (NAN=%" PRIx64 ")\n", nan_pattern);
    } else {
        printf("The representation of quiet NaN does not follow IEEE 754-2008 recommendation (NAN=%" PRIx64 ")\n", nan_pattern);
    }
    volatile double a = u64_to_double(UINT64_C(0x7ff800000000cafe));
    volatile double b = u64_to_double(UINT64_C(0x7ff800000000beef));
    volatile double c = u64_to_double(UINT64_C(0xfff800000000cafe));
    volatile double d = u64_to_double(UINT64_C(0xfff800000000beef));
    struct {
        double x, y;
    } cases[] = {
                 {a, b},
                 {a, c},
                 {a, d},
                 {b, c},
                 {b, d},
                 {c, d},
    };
    {
        puts("=== Addition ===");
        bool left = true;
        bool right = true;
        bool canonical = true;
        for (size_t i = 0; i < sizeof(cases)/sizeof(cases[0]); ++i) {
            volatile double x = cases[i].x;
            volatile double y = cases[i].y;
            volatile double z = add(x, y);
            volatile double w = add(y, x);
            printf("%" PRIx64 " + %" PRIx64 " = %" PRIx64 "\n", double_to_u64(x), double_to_u64(y), double_to_u64(z));
            printf("%" PRIx64 " + %" PRIx64 " = %" PRIx64 " %s\n", double_to_u64(y), double_to_u64(x), double_to_u64(w), double_to_u64(z) == double_to_u64(w) ? "(commutative)" : "(not commutative)");
            left = left && double_to_u64(z) == double_to_u64(x) && double_to_u64(w) == double_to_u64(y);
            right = right && double_to_u64(z) == double_to_u64(y) && double_to_u64(w) == double_to_u64(x);
            canonical = canonical && double_to_u64(z) == nan_pattern && double_to_u64(w) == nan_pattern;
        }
        if (left) {
            puts("Left operand is returned");
        }
        if (right) {
            puts("Right operand is returned");
        }
        if (canonical) {
            puts("Default NaN (or, canonical NaN) is returned");
        }
    }
    {
        puts("=== Multiplication ===");
        bool left = true;
        bool right = true;
        bool canonical = true;
        for (size_t i = 0; i < sizeof(cases)/sizeof(cases[0]); ++i) {
            volatile double x = cases[i].x;
            volatile double y = cases[i].y;
            volatile double z = mul(x, y);
            volatile double w = mul(y, x);
            printf("%" PRIx64 " * %" PRIx64 " = %" PRIx64 "\n", double_to_u64(x), double_to_u64(y), double_to_u64(z));
            printf("%" PRIx64 " * %" PRIx64 " = %" PRIx64 " %s\n", double_to_u64(y), double_to_u64(x), double_to_u64(w), double_to_u64(z) == double_to_u64(w) ? "(commutative)" : "(not commutative)");
            left = left && double_to_u64(z) == double_to_u64(x) && double_to_u64(w) == double_to_u64(y);
            right = right && double_to_u64(z) == double_to_u64(y) && double_to_u64(w) == double_to_u64(x);
            canonical = canonical && double_to_u64(z) == nan_pattern && double_to_u64(w) == nan_pattern;
        }
        if (left) {
            puts("Left operand is returned");
        }
        if (right) {
            puts("Right operand is returned");
        }
        if (canonical) {
            puts("Default NaN (or, canonical NaN) is returned");
        }
    }
    puts("=== Fused Multiply-Add ===");
#if defined(FP_FAST_FMA)
    puts("FP_FAST_FMA is defined");
#else
    puts("FP_FAST_FMA is not defined");
#endif
    for (size_t i = 0; i < sizeof(cases)/sizeof(cases[0]); ++i) {
        volatile double x = cases[i].x;
        volatile double y = cases[i].y;
        volatile double z = fma_noinline(x, y, 1.0);
        volatile double w = fma_noinline(y, x, 1.0);
        printf("fma(%" PRIx64 ", %" PRIx64 ", 1.0) = %" PRIx64 "\n", double_to_u64(x), double_to_u64(y), double_to_u64(z));
        printf("fma(%" PRIx64 ", %" PRIx64 ", 1.0) = %" PRIx64 " %s\n", double_to_u64(y), double_to_u64(x), double_to_u64(w), double_to_u64(z) == double_to_u64(w) ? "(commutative)" : "(not commutative)");
    }
    {
        volatile double u = fma_noinline(a, b, c);
        volatile double v = fma_noinline(b, c, a);
        volatile double w = fma_noinline(c, a, b);
        printf("fma(%" PRIx64 ", %" PRIx64 ", %" PRIx64 ") = %" PRIx64 "\n", double_to_u64(a), double_to_u64(b), double_to_u64(c), double_to_u64(u));
        printf("fma(%" PRIx64 ", %" PRIx64 ", %" PRIx64 ") = %" PRIx64 "\n", double_to_u64(b), double_to_u64(c), double_to_u64(a), double_to_u64(v));
        printf("fma(%" PRIx64 ", %" PRIx64 ", %" PRIx64 ") = %" PRIx64 "\n", double_to_u64(c), double_to_u64(a), double_to_u64(b), double_to_u64(w));
    }
}
