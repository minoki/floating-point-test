#include <stdio.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>

#if defined(__GNUC__)
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif

#if defined(__aarch64__) && defined(USE_FRINTTS)
#include <arm_acle.h>

double rint32z(double x)
{
    double result;
    asm("frint32z %d0, %d1" : "=w"(result) : "w"(x));
    return result;
}
double rint64x(double x)
{
    double result;
    asm("frint64x %d0, %d1" : "=w"(result) : "w"(x));
    return result;
}
NOINLINE
int64_t rint64z_as_i64(double x)
{
    double result;
    asm("frint64z %d0, %d1" : "=w"(result) : "w"(x));
    return (int64_t)result;
}
#endif

NOINLINE
int64_t cast_double_to_i64(double x)
{
    return (int64_t)x;
}

NOINLINE
uint64_t cast_double_to_u64(double x)
{
    return (uint64_t)x;
}

int main(void)
{
    volatile double x = INFINITY;
    unsigned long int N = 100000000;
    for (int k = 0; k < 3; ++k) {
        {
            int64_t y = 0;
            clock_t start_time = clock();
            for (unsigned long int i = 0; i < N; ++i) {
                y |= cast_double_to_i64(x);
            }
            printf("cast to i64: result = %" PRId64 ", time = %g[ns]\n", y, (clock() - start_time) * 1.0 / CLOCKS_PER_SEC / N * 1e9);
        }
        {
            uint64_t y = 0;
            clock_t start_time = clock();
            for (unsigned long int i = 0; i < N; ++i) {
                y |= cast_double_to_u64(x);
            }
            printf("cast to u64: result = %" PRIu64 ", time = %g[ns]\n", y, (clock() - start_time) * 1.0 / CLOCKS_PER_SEC / N * 1e9);
        }
#if defined(__aarch64__) && defined(USE_FRINTTS)
        {
            int64_t y = 0;
            clock_t start_time = clock();
            for (unsigned long int i = 0; i < N; ++i) {
                y |= rint64z_as_i64(x);
            }
            printf("FRINT64Z: result = %" PRId64 ", time = %g[ns]\n", y, (clock() - start_time) * 1.0 / CLOCKS_PER_SEC / N * 1e9);
        }
#endif
    }
}
