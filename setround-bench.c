// setround1.c
#include <assert.h>
#include <fenv.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(__SSE2__)
#include <immintrin.h>
#elif defined(__aarch64__)
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
#endif

#if defined(__x86_64__)
#define FORCE_REREAD(x) asm volatile("" : "+x"(x)) // x: any SSE register
#define FORCE_EVAL(x) asm volatile("" : : "x"(x)) // x: any SSE register
#elif defined(__aarch64__)
#define FORCE_REREAD(x) asm volatile("" : "+w"(x)) // w: Floating point register
#define FORCE_EVAL(x) asm volatile("" : : "w"(x)) // w: Floating point register
#else
#error not supported
#endif

#pragma STDC FENV_ACCESS ON

__attribute__((noinline))
double add_up_c99(double x, double y) {
    int r = fesetround(FE_UPWARD);
    FORCE_REREAD(x);
    FORCE_REREAD(y);
    double z = x + y;
    FORCE_EVAL(z);
    fesetround(FE_TONEAREST);
    return z;
}

__attribute__((noinline))
double add_up_c99_volatile(volatile double x, volatile double y) {
    int r = fesetround(FE_UPWARD);
    volatile double z = x + y;
    fesetround(FE_TONEAREST);
    return z;
}

__attribute__((noinline))
double add_up_native(double x, double y) {
#if defined(__SSE2__)
    unsigned int csr = _mm_getcsr();
    _mm_setcsr((csr & ~(3u << 13)) | (2u << 13)); // Set RC=upward
    FORCE_REREAD(x);
    FORCE_REREAD(y);
    double z = x + y;
    FORCE_EVAL(z);
    _mm_setcsr(csr); // Restore
    return z;
#elif defined(__aarch64__)
    uint64_t fpcr = get_fpcr();
    set_fpcr((fpcr & ~(3u << 22)) | (1u << 22)); // Set RMode=RP (upward)
    FORCE_REREAD(x);
    FORCE_REREAD(y);
    double z = x + y;
    FORCE_EVAL(z);
    set_fpcr(fpcr);
    return z;
#else
#error not supported
#endif
}

__attribute__((noinline))
double add_up_native_volatile(volatile double x, volatile double y) {
#if defined(__SSE2__)
    unsigned int csr = _mm_getcsr();
    _mm_setcsr((csr & ~(3u << 13)) | (2u << 13)); // Set RC=upward
    volatile double z = x + y;
    _mm_setcsr(csr); // Restore
    return z;
#elif defined(__aarch64__)
    uint64_t fpcr = get_fpcr();
    set_fpcr((fpcr & ~(3u << 22)) | (1u << 22)); // Set RMode=RP (upward)
    volatile double z = x + y;
    set_fpcr(fpcr);
    return z;
#else
#error not supported
#endif
}

#if defined(__AVX512F__)
__attribute__((noinline))
double add_up_avx512(double x, double y) {
    __m128d xx = _mm_set_sd(x), yy = _mm_set_sd(y);
    __m128d up = _mm_add_round_sd(xx, yy, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
    double result;
    _mm_store_sd(&result, up);
    return result;
}
#endif

int main(int argc, char *argv[]) {
    const int N = 100 * 1000 * 1000;
    if (argc > 1 && strcmp(argv[1], "avx512") == 0) {
#if defined(__AVX512F__)
        double a = 1.0;
        for (int i = 0; i < N; ++i) {
            a = add_up_avx512(a, 0x1p-1022);
        }
        printf("%a\n", a);
#else
        puts("AVX512 not enabled");
#endif
    } else if (argc > 1 && strcmp(argv[1], "native") == 0) {
        double a = 1.0;
        for (int i = 0; i < N; ++i) {
            a = add_up_native(a, 0x1p-1022);
        }
        printf("%a\n", a);
    } else if (argc > 1 && strcmp(argv[1], "native-volatile") == 0) {
        double a = 1.0;
        for (int i = 0; i < N; ++i) {
            a = add_up_native_volatile(a, 0x1p-1022);
        }
        printf("%a\n", a);
    } else if (argc > 1 && strcmp(argv[1], "c99") == 0) {
        double a = 1.0;
        for (int i = 0; i < N; ++i) {
            a = add_up_c99(a, 0x1p-1022);
        }
        printf("%a\n", a);
    } else if (argc > 1 && strcmp(argv[1], "c99-volatile") == 0) {
        double a = 1.0;
        for (int i = 0; i < N; ++i) {
            a = add_up_c99_volatile(a, 0x1p-1022);
        }
        printf("%a\n", a);
    } else {
        puts("Argument must be one of c99, c99-volatile, native, native-volatile, avx512");
    }
}
