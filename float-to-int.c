#include <stdio.h>
#include <math.h>
#include <fenv.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

#if defined(__GNUC__)
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif

#pragma STDC FENV_ACCESS ON

NOINLINE
double my_ceil(double x)
{
    return ceil(x);
}
NOINLINE
double my_floor(double x)
{
    return floor(x);
}
NOINLINE
double my_trunc(double x)
{
    return trunc(x);
}
NOINLINE
double my_round(double x)
{
    return round(x);
}
NOINLINE
double my_nearbyint(double x)
{
    return nearbyint(x);
}
NOINLINE
double my_rint(double x)
{
    return rint(x);
}

NOINLINE
long cast_double_to_long(double x)
{
    return (long)x;
}
NOINLINE
unsigned long cast_double_to_ulong(double x)
{
    return (unsigned long)x;
}
NOINLINE
long my_lrint(double x)
{
    return lrint(x);
}
NOINLINE
long my_lround(double x)
{
    return lround(x);
}
NOINLINE
long ceil_as_long(double x)
{
    return (long)ceil(x);
}
NOINLINE
long trunc_as_long(double x)
{
    return (long)trunc(x);
}
NOINLINE
unsigned long ceil_as_ulong(double x)
{
    return (unsigned long)ceil(x);
}
NOINLINE
unsigned long trunc_as_ulong(double x)
{
    return (unsigned long)trunc(x);
}

static bool dumpexcepts(char *buf)
{
    int e = fetestexcept(FE_ALL_EXCEPT);
    bool f = false;
    buf[0] = '\0';
    if (e & FE_INVALID) {
        strcat(buf, "invalid");
        f = true;
    }
    if (e & FE_DIVBYZERO) {
        if (f) {
            strcat(buf, ", divbyzero");
        } else {
            strcat(buf, "divbyzero");
        }
        f = true;
    }
    if (e & FE_OVERFLOW) {
        if (f) {
            strcat(buf, ", overflow");
        } else {
            strcat(buf, "overflow");
        }
        f = true;
    }
    if (e & FE_UNDERFLOW) {
        if (f) {
            strcat(buf, ", underflow");
        } else {
            strcat(buf, "underflow");
        }
        f = true;
    }
    if (e & FE_INEXACT) {
        if (f) {
            strcat(buf, ", inexact");
        } else {
            strcat(buf, "inexact");
        }
        f = true;
    }
    return f;
}

int main(int argc, char *argv[])
{
    int r = fegetround();
    if (argc > 1) {
        if (strcmp(argv[1], "--tonearest") == 0) {
            r = FE_TONEAREST;
        } else if (strcmp(argv[1], "--upward") == 0) {
            r = FE_UPWARD;
        } else if (strcmp(argv[1], "--downward") == 0) {
            r = FE_DOWNWARD;
        } else if (strcmp(argv[1], "--towardzero") == 0) {
            r = FE_TOWARDZERO;
        } else {
            fprintf(stderr, "Usage: %s [--tonearest|--upward|--downward|--towardzero]\n", argv[0]);
        }
    }
    fesetround(r);
    r = fegetround();
    printf("fegetround() = %s\n", r == FE_TONEAREST ? "FE_TONEAREST"
           : r == FE_UPWARD ? "FE_UPWARD"
           : r == FE_DOWNWARD ? "FE_DOWNWARD"
           : r == FE_TOWARDZERO ? "FE_TOWARDZERO"
           : "unknown");
    const double values[] = {-1.5, -0.5, -0.25, -0.0, 0.25, 0.5, 1.5, 0x1p100, INFINITY, NAN};
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        double y = my_ceil(x);
        char s[128];
        dumpexcepts(s);
        printf("ceil(%g) = %g %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        double y = my_floor(x);
        char s[128];
        dumpexcepts(s);
        printf("floor(%g) = %g %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        double y = my_trunc(x);
        char s[128];
        dumpexcepts(s);
        printf("trunc(%g) = %g %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        double y = my_round(x);
        char s[128];
        dumpexcepts(s);
        printf("round(%g) = %g %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        double y = my_nearbyint(x);
        char s[128];
        dumpexcepts(s);
        printf("nearbyint(%g) = %g %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        double y = my_rint(x);
        char s[128];
        dumpexcepts(s);
        printf("rint(%g) = %g %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        long y = my_lrint(x);
        char s[128];
        dumpexcepts(s);
        printf("lrint(%g) = %ld %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        long y = my_lround(x);
        char s[128];
        dumpexcepts(s);
        printf("lround(%g) = %ld %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        long y = cast_double_to_long(x);
        char s[128];
        dumpexcepts(s);
        printf("(long)(%g) = %ld %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        unsigned long y = cast_double_to_ulong(x);
        char s[128];
        dumpexcepts(s);
        printf("(unsigned long)(%g) = %lu %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        long y = ceil_as_long(x);
        char s[128];
        dumpexcepts(s);
        printf("(long)ceil(%g) = %ld %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        long y = trunc_as_long(x);
        char s[128];
        dumpexcepts(s);
        printf("(long)trunc(%g) = %ld %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        unsigned long y = ceil_as_ulong(x);
        char s[128];
        dumpexcepts(s);
        printf("(unsigned long)ceil(%g) = %lu %s\n", x, y, s);
    }
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        double x = values[i];
        feclearexcept(FE_ALL_EXCEPT);
        unsigned long y = trunc_as_ulong(x);
        char s[128];
        dumpexcepts(s);
        printf("(unsigned long)trunc(%g) = %lu %s\n", x, y, s);
    }
}
