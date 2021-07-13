#include <fenv.h>
#include <stdio.h>
struct t {
    double x, y;
};
struct t add(double a, double b)
{
    #pragma STDC FENV_ACCESS ON
    fesetround(FE_UPWARD);
    double x = a + b;
    fesetround(FE_DOWNWARD);
    double y = a + b;
    fesetround(FE_TONEAREST);
    return (struct t){.x = x, .y = y};
}

int main()
{
    struct t t = add(1.0, 0x1p-100);
    printf("x + y = %a (upward)\n", t.x);
    printf("x + y = %a (downward)\n", t.y);
}
