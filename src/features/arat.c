#include <stdio.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"

// 0x6 is the CPUID Leaf written to EAX
// if ARAT (Always Running APIC Timer) is supported, bit 2 in EAX should be 1
int test_arat(){
    int a, b, c, d;

    DPRINTF("Starting ARAT test...\n");

    cpuid(0x6, a, b, c, d);
    DPRINTF("CPUID executed\n");

    DPRINTF("Registers EAX, EBX, ECX, EDX are: %08x %08x %08x %08x\n", a, b, c, d);

    if(a & (1 << 2)){
        return 1;
    }else{
        return 0;
    }
}