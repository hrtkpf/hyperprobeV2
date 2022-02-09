#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/msr.h"

// Test if Hyper-V MSR HV_X64_MSR_CRASH_CTL is readable
int test_hv_x64_msr_crash_ctl(){
    DPRINTF("Starting HV_X64_MSR_CRASH_CTL test...\n");

    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        perror("failed to fork\n");
    }

    if (pid == 0) {
        // child process
        DPRINTF("DEBUG: Child: %s %d \n", __FUNCTION__, __LINE__);
        rdmsr_on_cpu(HV_X64_MSR_CRASH_CTL, 0);
        DPRINTF("DEBUG: Child: HV_X64_MSR_CRASH_CTL is readable\n");
        exit(0);
    } else {
        // parent process
        wait(&status);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            // abnormal exit status or unsuccessful register read, so feature is not present
            DPRINTF("DEBUG: Parent: The return code of child process is non-zero.\n");
            DPRINTF("DEBUG: Parent: Feature not present: Cannot read HV_X64_MSR_CRASH_CTL!\n");
            return 0;
        } else {
            // successful register read, so feature is present
            DPRINTF("DEBUG: Parent: Feature present: HV_X64_MSR_CRASH_CTL is readable!\n");
            return 1;
        }
    }
}