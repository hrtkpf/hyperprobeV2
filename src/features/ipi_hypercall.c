#include <stdio.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"

// 0x40000001 is the CPUID Leaf written to EAX
// if IPI Hypercall is supported, bit 11 in EAX should be 1
// https://git.kernel.org/pub/scm/virt/kvm/kvm.git/tree/Documentation/virtual/kvm/cpuid.txt?h=tags/kvm-4.19-1#n65
// https://en.wikipedia.org/wiki/Inter-processor_interrupt
int test_ipi_hypercall(){
    int a, b, c, d;

    DPRINTF("Starting IPI Hypercall test...\n");

    cpuid(0x40000001, a, b, c, d);
    DPRINTF("CPUID executed\n");

    DPRINTF("Registers EAX, EBX, ECX, EDX are: %08x %08x %08x %08x\n", a, b, c, d);

    if(a & (1 << 11)){
        return 1;
    }else{
        return 0;
    }
}