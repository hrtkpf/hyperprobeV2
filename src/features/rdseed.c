#include <stdio.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"

int test_rdseed_instruction(){
    int a, b, c, d;

    // 0x07 is the standard function written to EAX
    // if RDSEED is supported, bit 18 in EBX should be 1
    // https://en.wikipedia.org/wiki/RDRAND
    DPRINTF("Starting RDSEED test...\n");

    cpuid(0x7, a, b, c, d);
    DPRINTF("CPUID executed\n");

    DPRINTF("Registers EAX, EBX, ECX, EDX are: %08x %08x %08x %08x\n", a, b, c, d);

    // if bit 18 of EBX is 1, RDSEED is available
    if(b & (1 << 18)){
        return 1;
    }else{
        return 0;
    }
}