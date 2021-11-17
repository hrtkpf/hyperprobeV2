#include <stdio.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"

// 0x80000007 is the CPUID Leaf written to EAX
// if Invariant TSC is supported, bit 8 in EDX should be 1
// https://en.wikipedia.org/wiki/Time_Stamp_Counter
int test_invariant_tsc(){
    int a, b, c, d;

    DPRINTF("Starting Invariant TSC test...\n");

    cpuid(0x80000007, a, b, c, d);
    DPRINTF("CPUID executed\n");

    DPRINTF("Registers EAX, EBX, ECX, EDX are: %08x %08x %08x %08x\n", a, b, c, d);

    // if bit 8 of EDX is 1, Invariant TSC is available
    if(d & (1 << 8)){
        return 1;
    }else{
        return 0;
    }
}