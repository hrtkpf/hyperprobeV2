#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "hyperprobe/bugs.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"
#include "hyperprobe/msr.h"

// MSR IA32_ARCH_CAPABILITIES is not readable until 4.19
// if MSR is present but not readable, bug is existent
int test_msr_ia32_arch_capabilities_readable() {
    int a, b, c, d;

    DPRINTF("Starting MSR IA32_ARCH_CAPABILITIES read test...\n");
    cpuid(0x7, a, b, c, d);

    if(!(d & (1 << 29))) {
        // MSR is not supported, so bug cannot be present
        // see MSR_IA32_ARCH_CAPABILITIES feature test for details
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
        uint64_t data = rdmsr_on_cpu(MSR_IA32_ARCH_CAPABILITIES, 0);
        DPRINTF("DEBUG: Child: Bug fixed: MSR_IA32_ARCH_CAPABILITIES is readable!\n");
        DPRINTF("DEBUG: Child: MSR_IA32_ARCH_CAPABILITIES value is: %08x\n", (uint32_t)data);
        exit(0);
    } else {
        // parent process
        wait(&status);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            // abnormal exit status or unsuccessful MSR read, so bug is present
            DPRINTF("DEBUG: Parent: The return code of child process is non-zero.\n");
            DPRINTF("DEBUG: Parent: Bug exists: MSR_IA32_ARCH_CAPABILITIES is not readable!\n");
            return 1;
        } else {
            // successful MSR read, bug is fixed
            DPRINTF("DEBUG: Parent: Bug fixed: MSR_IA32_ARCH_CAPABILITIES is readable!\n");
            return 0;
        }
        DPRINTF("DEBUG: Parent: %s %d \n", __FUNCTION__, __LINE__);
    }
}