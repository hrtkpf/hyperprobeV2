#include <stdio.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"

// 0x40000001 is the CPUID Leaf written to EAX
// if Sched Yield Hypercall is supported, bit 13 in EAX should be 1
// https://linux.die.net/man/2/sched_yield
int test_sched_yield_hypercall(){
    int a, b, c, d;

    DPRINTF("Starting Sched Yield Hypercall test...\n");

    cpuid(0x40000001, a, b, c, d);
    DPRINTF("CPUID executed\n");

    DPRINTF("Registers EAX, EBX, ECX, EDX are: %08x %08x %08x %08x\n", a, b, c, d);

    if(a & (1 << 13)){
        return 1;
    }else{
        return 0;
    }
}