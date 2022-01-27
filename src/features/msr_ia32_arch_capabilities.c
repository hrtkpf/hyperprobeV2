#include <stdio.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"

// 0x7 is the CPUID Leaf written to EAX
// If MSR IA32_ARCH_CAPABILITIES is supported, bit 29 in EDX should be 1
// This test only executes on Intel platforms because 4.15 introduced MSR CPUID support for Intel only
int test_msr_ia32_arch_capabilities(){
    int a, b, c, d;

    DPRINTF("Starting MSR IA32_ARCH_CAPABILITIES test...\n");

    cpuid(0x0, a, b, c, d);

    if(c != 0x6c65746e){
        // We are not on Intel
        DPRINTF("Skipping test as this is not an Intel platform\n");
        return -1;
    }

    cpuid(0x7, a, b, c, d);
    DPRINTF("CPUID executed\n");

    DPRINTF("Registers EAX, EBX, ECX, EDX are: %08x %08x %08x %08x\n", a, b, c, d);

    if(d & (1 << 29)){
        return 1;
    }else{
        return 0;
    }
}