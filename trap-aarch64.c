#include <stdio.h>
#include <fenv.h>
#include <stdint.h>

#pragma STDC FENV_ACCESS ON

// #if defined(__clang__)
// __builtin_aarch64_get_fpcr and __builtin_aarch64_set_fpcr are not available on clang
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
// #endif

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
