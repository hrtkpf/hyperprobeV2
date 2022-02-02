#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "hyperprobe/bugs.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/msr.h"

// Test if MSR_K8_TSEG_ADDR is readable
int test_msr_k8_tseg_addr(){
    DPRINTF("Starting MSR_K8_TSEG_ADDR test...\n");

    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        perror("failed to fork\n");
    }

    if (pid == 0) {
        // child process
        DPRINTF("DEBUG: Child: %s %d \n", __FUNCTION__, __LINE__);
        rdmsr_on_cpu(MSR_K8_TSEG_ADDR, 0);
        DPRINTF("DEBUG: Child: MSR_K8_TSEG_ADDR is readable\n");
        exit(0);
    } else {
        // parent process
        wait(&status);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            // abnormal exit status or unsuccessful register read, so bug is present
            DPRINTF("DEBUG: Parent: The return code of child process is non-zero.\n");
            DPRINTF("DEBUG: Parent: Bug present: Cannot read MSR_K8_TSEG_ADDR!\n");
            return 1;
        } else {
            // successful register read, so bug is not present
            DPRINTF("DEBUG: Parent: Bug not present: MSR_K8_TSEG_ADDR is readable!\n");
            return 0;
        }
    }
}