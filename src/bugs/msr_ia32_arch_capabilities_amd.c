#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "hyperprobe/bugs.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"
#include "hyperprobe/msr.h"

// MSR_IA32_ARCH_CAPABILITIES is not emulated on AMD until 5.1 despite being advertised via CPUID since 4.17.
// If CPUID flag is set but we cannot read the MSR, the bug is present. Only works on AMD.
int test_msr_ia32_arch_capabilities_amd() {
    int a, b, c, d;
    DPRINTF("Starting MSR_IA32_ARCH_CAPABILITIES (AMD) test...\n");

    cpuid(0x0, a, b, c, d);

    // Skip test if we are not on AMD
    if(c == 0x6c65746e){
        // We are on Intel
        DPRINTF("Skipping test as this is an Intel platform\n");
        return -1;
    }

    // Check CPUID flag
    cpuid(0x7, a, b, c, d);
    DPRINTF("CPUID executed\n");

    DPRINTF("Registers EAX, EBX, ECX, EDX are: %08x %08x %08x %08x\n", a, b, c, d);

    // If CPUID flag is not present, we cannot know if the bug is present
    if(!(d & (1 << 29))){
        return 0;
    }

    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        perror("failed to fork\n");
    }

    if (pid == 0) {
        // child process
        DPRINTF("DEBUG: Child: %s %d \n", __FUNCTION__, __LINE__);
        rdmsr_on_cpu(MSR_IA32_ARCH_CAPABILITIES, 0);
        DPRINTF("DEBUG: Child: MSR_IA32_ARCH_CAPABILITIES is readable\n");
        exit(0);
    } else {
        // parent process
        wait(&status);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            // abnormal exit status or unsuccessful register read, so bug is present
            DPRINTF("DEBUG: Parent: The return code of child process is non-zero.\n");
            DPRINTF("DEBUG: Parent: Bug present: Cannot read MSR_IA32_ARCH_CAPABILITIES!\n");
            return 1;
        } else {
            // successful register read, so bug is fixed
            DPRINTF("DEBUG: Parent: Bug not present: MSR_IA32_ARCH_CAPABILITIES is readable!\n");
            return 0;
        }
    }
}