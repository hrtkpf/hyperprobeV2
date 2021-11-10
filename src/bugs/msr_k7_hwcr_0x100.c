/*
 * This file implements a test case for check the MSR_K7_HWCR writing 0x100 bug.
 * Initial work by:
 *   (c) 2014 Lei Lu (lulei.wm@gmail.com)
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

// Thie function use fork to create a child process. The child process tries to read MSR_K7_HWCR.
// If the register exists, it is readable. Otherwise, it is not readable.
// Return: 1 if bug exist, 0 if not sure.
int test_msr_k7_hwcr_0x100() {
    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        perror("fail to fork\n");
    }

    if (pid == 0)    //child process
    {
        DPRINTF("DEBUG: Child: %s %d \n", __FUNCTION__, __LINE__);
        wrmsr_on_cpu(MSR_K7_HWCR, 0, 0x100);
        DPRINTF("DEBUG: Child: Bug Fixed: MSR_K7_HWCR 0x100 is writable!\n");
        exit(0);
    } else        //parent process
    {
        wait(&status);
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status)) {
                DPRINTF("DEBUG: Parent: The return code of child process is non zero.\n");
                DPRINTF("DEBUG: Parent: Bug Exists: writing 0x100 to MSR_K7_HWCR is not allowed!\n");
                return 1;
            } else {
                DPRINTF("DEBUG: Parent: The return code of child process is zero.\n");
                DPRINTF("DEBUG: Parent: Bug Fixed: writing 0x100 to MSR_K7_HWCR is allowed!\n");
                return 0;       //child process exit normally with exit code 0, which means the register is readable, so the bug is not existing.
            }
        } else {
            DPRINTF("DEBUG: Parent: Bug Exists: writing 0x100 to MSR_K7_HWCR is not allowed!\n");
            return 1;    //child process exit abnormally, the register is not writable, so the bug is existing.
        }
        DPRINTF("DEBUG: Parent: %s %d \n", __FUNCTION__, __LINE__);
    }
    return 0;
}

