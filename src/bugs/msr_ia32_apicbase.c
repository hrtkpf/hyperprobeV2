/*
 * This file implements a test case for check the MSR_IA32_APICBASE bug.
 * Initial work by:
 *   (c) 2014 Jidong Xiao (jidong.xiao@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "hyperprobe/bugs.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/msr.h"

// Thie function use fork to create a child process. The child process tries to write reserve bits of MSR_IA32_APICBASE.
// According to Intel SDM, bit 36 to bit 63 in MSR_IA32_APICBASE are reserved, and also bit 0 to bit 7 is reserved.
// Also, bit 9 is reserved.
// The reserve bits should not be writable, but before kernel 3.14, it is writable, this is fixed in kernel 3.14.
// Return: 1 if bug exist, 0 if not sure.
int test_msr_ia32_apicbase() {
    pid_t pid;
    int status;

    uint64_t old_value;
    uint64_t new_value;

    if ((pid = fork()) < 0) {
        perror("fail to fork\n");
    }

    if (pid == 0)    //child process
    {
        DPRINTF("DEBUG: Child: %s %d \n", __FUNCTION__, __LINE__);
        old_value = rdmsr_on_cpu(MSR_IA32_APICBASE, 0);    //FIXME: When this is not readable, error handle needed.
        new_value = old_value | (1ULL << 0);    // We just write to bit 0
        wrmsr_on_cpu(MSR_IA32_APICBASE, 0, new_value);
        DPRINTF("DEBUG: Child: Bug Exists: The reserve bits of MSR_IA32_APICBASE is writable!\n");
        exit(0);
    } else        //parent process
    {
        wait(&status);
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status)) {
                DPRINTF("DEBUG: Parent: The return code of child process is non zero.\n");
                DPRINTF("DEBUG: Parent: Bug Fixed: The reserve bits MSR_IA32_APICBASE is not writable!\n");
                return 0;
            } else {
                DPRINTF("DEBUG: Parent: The return code of child process is zero.\n");
                DPRINTF("DEBUG: Parent: Bug Exists: The reserve bits of MSR_IA32_APICBASE is writable!\n");
                return 1;       //child process exit normally with exit code 0, which means the register is readable, so the bug is not existing.
            }
        } else {
            DPRINTF("DEBUG: Parent: Bug Fixed: The reserve bits of MSR_IA32_APICBASE is not writable!\n");
            return 0;    //child process exit abnormally, the register is not writable, so the bug is existing.
        }
        DPRINTF("DEBUG: Parent: %s %d \n", __FUNCTION__, __LINE__);
    }
    return 0;
}

