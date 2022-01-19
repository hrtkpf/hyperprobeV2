#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"
#include "hyperprobe/msr.h"

// Test for interrupt based asynchronous page fault (APF) mechanism by checking whether the corresponding MSR is present
// https://git.kernel.org/pub/scm/virt/kvm/kvm.git/commit/?id=2635b5c4a0e407b84f68e188c719f28ba0e9ae1b
int test_async_pf_int(){
    DPRINTF("Starting ASYNC_PF_INT (interrupt-based APF) test...\n");

    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        perror("failed to fork\n");
    }

    if (pid == 0) {
        // child process
        DPRINTF("DEBUG: Child: %s %d \n", __FUNCTION__, __LINE__);
        rdmsr_on_cpu(MSR_KVM_ASYNC_PF_INT, 0);
        DPRINTF("DEBUG: Child: MSR_KVM_ASYNC_PF_INT is readable\n");
        exit(0);
    } else {
        // parent process
        wait(&status);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            // abnormal exit status or unsuccessful register read, so feature is not present
            DPRINTF("DEBUG: Parent: The return code of child process is non-zero.\n");
            DPRINTF("DEBUG: Parent: Feature not present: Cannot read MSR_KVM_ASYNC_PF_INT!\n");
            return 0;
        } else {
            // successful register read, so feature is present
            DPRINTF("DEBUG: Parent: Feature present: MSR_KVM_ASYNC_PF_INT is readable!\n");
            return 1;
        }
    }
}