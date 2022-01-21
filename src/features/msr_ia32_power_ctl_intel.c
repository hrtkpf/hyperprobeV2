#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"
#include "hyperprobe/msr.h"

// MSR_IA32_POWER_CTL is present since KVM 5.2 for Intel CPUs (VMX) only.
// This is a non-configurable feature as it cannot be deactivated, so this will also assume max. version if not present.
// Although this is a complete feature test, it is only intended to be run when the feature is not present to
// narrow down max. version to 5.2 instead of 5.3. This results in a more precise estimation.
// All other cases will be covered by another test for 5.3.
// If we are on Intel and the MSR is readable, assume min. version 5.2.
// If we are on Intel and the MSR is not readable, assume max. ver 5.1.
int test_msr_ia32_power_ctl_intel(){
    int a, b, c, d;

    DPRINTF("Starting Intel MSR_IA32_POWER_CTL test...\n");

    cpuid(0x0, a, b, c, d);

    if(c != 0x6c65746e){  // check for Intel's vendor ID
        // We are not on Intel
        DPRINTF("Skipping test as this is not an Intel platform\n");
        return -1;
    }

    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        perror("failed to fork\n");
    }

    if (pid == 0) {
        // child process
        DPRINTF("DEBUG: Child: %s %d \n", __FUNCTION__, __LINE__);
        rdmsr_on_cpu(MSR_IA32_POWER_CTL, 0);
        DPRINTF("DEBUG: Child: MSR_IA32_POWER_CTL is readable\n");
        exit(0);
    } else {
        // parent process
        wait(&status);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            // abnormal exit status or unsuccessful register read, so feature is not present
            DPRINTF("DEBUG: Parent: The return code of child process is non-zero.\n");
            DPRINTF("DEBUG: Parent: Feature not present: Cannot read MSR_IA32_POWER_CTL!\n");
            return 0;
        } else {
            // successful register read, so feature is present
            DPRINTF("DEBUG: Parent: Feature present: MSR_IA32_POWER_CTL is readable!\n");
            return 1;
        }
    }
}