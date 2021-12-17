#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "hyperprobe/bugs.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/msr.h"

// HV_X64_MSR_TSC_EMULATION_STATUS is writable with 1 until 5.13
// If 1 can be written to register, bug is present
int test_hv_x64_msr_tsc_emulation_status() {
    DPRINTF("Starting HV_X64_MSR_TSC_EMULATION_STATUS test...\n");

    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        perror("failed to fork\n");
    }

    if (pid == 0) {
        // child process
        DPRINTF("DEBUG: Child: %s %d \n", __FUNCTION__, __LINE__);
        uint64_t old_value = rdmsr_on_cpu(HV_X64_MSR_TSC_EMULATION_STATUS, 0);
        wrmsr_on_cpu(HV_X64_MSR_TSC_EMULATION_STATUS, 0, 1);
        uint64_t new_value = rdmsr_on_cpu(HV_X64_MSR_TSC_EMULATION_STATUS, 0);
        DPRINTF("DEBUG: Child: HV_X64_MSR_TSC_EMULATION_STATUS value was %08d and is now: %08x\n", (uint32_t)old_value, (uint32_t)new_value);
        exit(0);
    } else {
        // parent process
        wait(&status);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            // abnormal exit status or unsuccessful register write, so bug is fixed
            DPRINTF("DEBUG: Parent: The return code of child process is non-zero.\n");
            DPRINTF("DEBUG: Parent: Bug not present: Cannot write 1 to HV_X64_MSR_TSC_EMULATION_STATUS!\n");
            return 0;
        } else {
            // successful register write, so bug is present
            DPRINTF("DEBUG: Parent: Bug present: HV_X64_MSR_TSC_EMULATION_STATUS is writable with 1!\n");
            return 1;
        }
    }
}