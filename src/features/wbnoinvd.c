#include <stdio.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"

// 0x80000008 is the CPUID Leaf written to EAX
// if WBNOINVD instruction is supported, bit 9 in EBX should be 1
int test_wbnoinvd_instruction(){
    int a, b, c, d;

    DPRINTF("Starting WBNOINVD instruction test...\n");

    cpuid(0x80000008, a, b, c, d);
    DPRINTF("CPUID executed\n");

    DPRINTF("Registers EAX, EBX, ECX, EDX are: %08x %08x %08x %08x\n", a, b, c, d);

    if(b & (1 << 9)){
        return 1;
    }else{
        return 0;
    }
}