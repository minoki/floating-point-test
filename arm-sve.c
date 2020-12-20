#include <stdio.h>
#include <arm_sve.h>

#if defined(__GNUC__)
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif

NOINLINE
void add(size_t n, double result[restrict n], const double x[restrict n], const double y[restrict n])
{
    for (size_t i = 0; i < n; ++i) {
        result[i] = x[i] + y[i];
    }
}

NOINLINE
void add_sve(size_t n, double result[restrict n], const double x[restrict n], const double y[restrict n])
{
    for (size_t i = 0; i < n; i += svcntd()) {
        svbool_t p = svwhilelt_b64((uint64_t)i, (uint64_t)n);
        svfloat64_t xx = svld1(p, &x[i]);
        svfloat64_t yy = svld1(p, &y[i]);
        svfloat64_t zz = svadd_z(p, xx, yy);
        svst1(p, &result[i], zz);
    }
}

int main()
{
    double x[] = {1.0, 2.0, 3.0};
    double y[] = {-0.5, 2.1, 3.14};
    double result[3];
    add(3, result, x, y);
    for (size_t i = 0; i < 3; ++i) {
        printf("result[%zu]=%g\n", i, result[i]);
        result[i] = 0.0;
    }
    puts("==SVE==");
    add_sve(3, result, x, y);
    for (size_t i = 0; i < 3; ++i) {
        printf("result[%zu]=%g\n", i, result[i]);
    }
}
