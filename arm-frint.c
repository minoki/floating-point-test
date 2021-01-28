#include <stdio.h>
#include <math.h>
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

int main(void)
{
#if defined(__ARM_FEATURE_FRINT)
  puts("__ARM_FEATURE_FRINT is defined.");
  printf("__rint32z(%g) = %a\n", INFINITY, __rint32z(INFINITY));
  printf("__rint64x(%g) = %a\n", NAN, __rint64x(NAN));
#else
  puts("__ARM_FEATURE_FRINT is not defined.");
#endif
  printf("rint32z(%g) = %a\n", INFINITY, rint32z(INFINITY));
  printf("rint64x(%g) = %a\n", NAN, rint64x(NAN));
}
