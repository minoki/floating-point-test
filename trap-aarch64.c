#include <stdio.h>
#include <fenv.h>
#include <stdint.h>

#pragma STDC FENV_ACCESS ON

#if defined(__GNUC__) && !defined(__clang__)
// __has_builtin: GCC 10 or later
// __builtin_aarch64_{get,set}_fpcr64: GCC 11 or later
// __builtin_aarch64_{get,set}_fpcr: GCC 5 or later
#define get_fpcr()  __builtin_aarch64_get_fpcr64()
#define set_fpcr(x) __builtin_aarch64_set_fpcr64(x)
#else
#include <arm_acle.h>
#define get_fpcr() __arm_rsr64("fpcr")
#define set_fpcr(x) __arm_wsr64("fpcr", x)
#endif

#if 0
__attribute__((always_inline))
uint64_t get_fpcr()
{
    uint64_t fpcr;
    asm volatile("mrs %0, fpcr" : "=r"(fpcr));
    return fpcr;
}
__attribute__((always_inline))
void set_fpcr(uint64_t x)
{
    asm volatile("msr fpcr, %0" : : "r"(x));
}
#endif

int main(void)
{
    uint64_t fpcr = get_fpcr();
    set_fpcr(fpcr | 0x9f00u); // IDE + IXE + UFE + OFE + DZE + IOE
    fpcr = get_fpcr();
    if ((fpcr & 0x9f00u) == 0x9f00u) {
        puts("Traps are supported:");
    } else if ((fpcr & 0x9f00u) != 0) {
        puts("Traps are partially supported:");
    } else {
        puts("Traps are not supported:");
    }
    printf("  Input Denormal floating-point exception (IDE) trap %s.\n", (fpcr & (1 << 15)) ? "enabled" : "disabled");
    printf("  Inexact floating-point exception (IXE) trap %s.\n", (fpcr & (1 << 12)) ? "enabled" : "disabled");
    printf("  Underflow floating-point exception (UFE) trap %s.\n", (fpcr & (1 << 11)) ? "enabled" : "disabled");
    printf("  Overflow floating-point exception (OFE) trap %s.\n", (fpcr & (1 << 10)) ? "enabled" : "disabled");
    printf("  Divide by Zero floating-point exception (DZE) trap %s.\n", (fpcr & (1 << 9)) ? "enabled" : "disabled");
    printf("  Invalid Operation floating-point exception (IOE) trap %s.\n", (fpcr & (1 << 8)) ? "enabled" : "disabled");
    volatile double zero = 0.0, one = 1.0;
    printf("1.0 / 0.0 = %g\n", one / zero);
}
