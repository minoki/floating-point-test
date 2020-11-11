#include <stdio.h>
#include <math.h>
#include <float.h>

#if defined(__cplusplus)
#define FLOATING_TYPE_NAME(T) (typeid(T) == typeid(float) ? "float" : typeid(T) == typeid(double) ? "double" : typeid(T) == typeid(long double) ? "long double" : "unknown")
#else
#define FLOATING_TYPE_NAME(T) _Generic((T *)NULL, float *: "float", double *: "double", long double *: "long double", default: "unknown")
#endif

int main(void)
{
    printf("FLT_RADIX = %d\n", FLT_RADIX);
    printf("FLT_ROUNDS = %d; %s\n", FLT_ROUNDS,
           FLT_ROUNDS == -1 ? "indeterminable" :
           FLT_ROUNDS == 0 ? "toward zero" :
           FLT_ROUNDS == 1 ? "to nearest" :
           FLT_ROUNDS == 2 ? "toward positive infinity" :
           FLT_ROUNDS == 3 ? "toward negative infinity" :
           "unknown");
    printf("FLT_EVAL_METHOD = %d; %s\n", FLT_EVAL_METHOD,
           FLT_EVAL_METHOD == -1 ? "indeterminable" :
           FLT_EVAL_METHOD == 0 ? "just to the range and precision of the type" :
           FLT_EVAL_METHOD == 1 ? "double" :
           FLT_EVAL_METHOD == 2 ? "long double" :
           "unknown");
#if defined(__STDC_IEC_559__)
    puts("__STDC_IEC_559__ is defined");
#else
    puts("__STDC_IEC_559__ is not defined");
#endif
    puts("---");
    printf("sizeof(float) = %zu\n", sizeof(float));
    printf("FLT_MANT_DIG = %d\n", FLT_MANT_DIG);
    printf("FLT_MIN_EXP = %d\n", FLT_MIN_EXP);
    printf("FLT_MAX_EXP = %d\n", FLT_MAX_EXP);
    printf("FLT_MAX = %a\n", FLT_MAX);
    printf("FLT_MIN = %a\n", FLT_MIN);
    printf("FLT_TRUE_MIN = %a\n", FLT_TRUE_MIN);
    printf("float_t is %s\n", FLOATING_TYPE_NAME(float_t));
    if (FLT_MANT_DIG == 24 && FLT_MIN_EXP == -126 && FLT_MAX_EXP == 128) {
        puts("float is probably IEEE binary32 format");
    }
    puts("---");
    printf("sizeof(double) = %zu\n", sizeof(double));
    printf("DBL_MANT_DIG = %d\n", DBL_MANT_DIG);
    printf("DBL_MIN_EXP = %d\n", DBL_MIN_EXP);
    printf("DBL_MAX_EXP = %d\n", DBL_MAX_EXP);
    printf("DBL_MAX = %a\n", DBL_MAX);
    printf("DBL_MIN = %a\n", DBL_MIN);
    printf("DBL_TRUE_MIN = %a\n", DBL_TRUE_MIN);
    printf("double_t is %s\n", FLOATING_TYPE_NAME(double_t));
    if (DBL_MANT_DIG == 53 && DBL_MIN_EXP == -1021 && DBL_MAX_EXP == 1024) {
        puts("double is probably IEEE binary64 format");
    }
    puts("---");
    printf("sizeof(long double) = %zu\n", sizeof(long double));
    printf("LDBL_MANT_DIG = %d\n", LDBL_MANT_DIG);
    printf("LDBL_MIN_EXP = %d\n", LDBL_MIN_EXP);
    printf("LDBL_MAX_EXP = %d\n", LDBL_MAX_EXP);
    printf("LDBL_MAX = %La\n", LDBL_MAX);
    printf("LDBL_MIN = %La\n", LDBL_MIN);
    printf("LDBL_TRUE_MIN = %La\n", LDBL_TRUE_MIN);
    if (LDBL_MANT_DIG == 64 && LDBL_MIN_EXP == -16381 && LDBL_MAX_EXP == 16384) {
        puts("long double is probably x87 80-bit extended double format");
    } else if (LDBL_MANT_DIG == 113 && LDBL_MIN_EXP == -16381 && LDBL_MAX_EXP == 16384) {
        puts("long double is probably IEEE binary128 format");
    } else if (LDBL_MANT_DIG == 53 && LDBL_MIN_EXP == -1021 && LDBL_MAX_EXP == 1024) {
        puts("long double is probably IEEE binary64 format");
    }
    puts("---");
    {
        volatile double x = 0x1.00002fff0p0, y = 0x1.000000008p0;
        volatile double z = x * y;
        printf("%a * %a = %a; %s\n", x, y, z, z == 0x1.00002fff80001p0 ? "correctly rounded" : z == 0x1.00002fff80002p0 ? "x87 FPU with PC=64" : "unexpected result");
    }
    {
        volatile double x = 0x1.fffe0effffffep-51, y = 0x1.0000000000001p-1000;
        volatile double z = x * y;
        printf("%a * %a = %a; %s\n", x, y, z, z == 0x1.fffe0ep-1051 ? "correctly rounded" : z == 0x1.fffe1p-1051 ? "incorrect rounding with x87 FPU" : "unexpected result");
    }
}
