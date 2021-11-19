#include <stdio.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"

// 0x7 is the CPUID Leaf written to EAX
// if MSR IA32_ARCH_CAPABILITIES is supported, bit 29 in EDX should be 1
int test_msr_ia32_arch_capabilities(){
    int a, b, c, d;

    DPRINTF("Starting MSR IA32_ARCH_CAPABILITIES test...\n");

    cpuid(0x7, a, b, c, d);
    DPRINTF("CPUID executed\n");

    DPRINTF("Registers EAX, EBX, ECX, EDX are: %08x %08x %08x %08x\n", a, b, c, d);

    if(d & (1 << 29)){
        return 1;
    }else{
        return 0;
    }
}