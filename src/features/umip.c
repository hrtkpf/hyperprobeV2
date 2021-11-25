#include <stdio.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"

// 0x7 is the CPUID Leaf written to EAX
// if UMIP (User-Mode Instruction Prevention) is supported, bit 2 in ECX should be 1
int test_umip(){
    int a, b, c, d;

    DPRINTF("Starting UMIP test...\n");

    cpuid(0x7, a, b, c, d);
    DPRINTF("CPUID executed\n");

    DPRINTF("Registers EAX, EBX, ECX, EDX are: %08x %08x %08x %08x\n", a, b, c, d);

    if(c & (1 << 2)){
        return 1;
    }else{
        return 0;
    }
}