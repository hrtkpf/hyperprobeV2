#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "hyperprobe/bugs.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/msr.h"

// Test if MSR_AMD64_DC_CFG is readable
int test_msr_amd64_dc_cfg(){
    DPRINTF("Starting MSR_AMD64_DC_CFG test...\n");

    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        perror("failed to fork\n");
    }

    if (pid == 0) {
        // child process
        DPRINTF("DEBUG: Child: %s %d \n", __FUNCTION__, __LINE__);
        rdmsr_on_cpu(MSR_AMD64_DC_CFG, 0);
        DPRINTF("DEBUG: Child: MSR_AMD64_DC_CFG is readable\n");
        exit(0);
    } else {
        // parent process
        wait(&status);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            // abnormal exit status or unsuccessful register read, so bug is present
            DPRINTF("DEBUG: Parent: The return code of child process is non-zero.\n");
            DPRINTF("DEBUG: Parent: Bug present: Cannot read MSR_AMD64_DC_CFG!\n");
            return 1;
        } else {
            // successful register read, so bug is not present
            DPRINTF("DEBUG: Parent: Bug not present: MSR_AMD64_DC_CFG is readable!\n");
            return 0;
        }
    }
}