#include <stdio.h>
#include <time.h>
#include <math.h>

#if defined(__GNUC__)
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif

NOINLINE
double fma_wrapped(double x, double y, double z)
{
    return fma(x, y, z);
}
NOINLINE
double multiply_add(double x, double y, double z)
{
    return x * y + z;
}

int main(void)
{
    const unsigned long int N = 100000000;
    for (int k = 0; k < 3; ++k) {
        {
            double x = 1.234;
            clock_t start_time = clock();
            for (unsigned long int i = 0; i < N; ++i) {
                x = fma_wrapped(x, x, x);
            }
            printf("FMA: result = %a, time = %g[ns]\n", x, (clock() - start_time) * 1.0 / CLOCKS_PER_SEC / N * 1e9);
        }
        {
            double x = 1.234;
            clock_t start_time = clock();
            for (unsigned long int i = 0; i < N; ++i) {
                x = multiply_add(x, x, x);
            }
            printf("multiply add: result = %a, time = %g[ns]\n", x, (clock() - start_time) * 1.0 / CLOCKS_PER_SEC / N * 1e9);
        }
    }
}
