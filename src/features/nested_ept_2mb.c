/*
 * This file implements a test case for check the nested ept 2MB page feature.
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
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/msr.h"
#include "hyperprobe/nested.h"

uint64_t data;

// Thie function use fork to create a child process. The child process tries to read MSR_IA32_VMX_EPT_VPID_CAP.
// If MSR_IA32_VMX_EPT_VPID_CAP[16] is 1, it indicates 2MB ept page is supported. If is is 0, then 2MB ept page is not supported.
// Return: 1 if feature exist, 0 if not sure.
int test_nested_ept_2mb() {

    /* This feature only makes sense if nested vmx is enabled. */
    if (!test_nested_vmx())
        return 0;

    pid_t pid;
    int status;

    if ((pid = vfork()) < 0) {
        perror("fail to fork\n");
    }

    if (pid == 0)      // child process
    {
        DPRINTF("DEBUG: Child: %s %d \n", __FUNCTION__, __LINE__);
        data = rdmsr_on_cpu(MSR_IA32_VMX_EPT_VPID_CAP, 0);
        exit(0);
    } else    // parent process
    {
        wait(&status);
        if (data & (1ULL << 16)) {
            DPRINTF("DEBUG: Parent: Feature exists: MSR_IA32_VMX_EPT_VPID_CAP[16] is 1 and thus nested ept with 2MB page is supported!\n");
            return 1;
        } else {
            DPRINTF("DEBUG: Parent: Feature not exists: MSR_IA32_VMX_EPT_VPID_CAP[16] is 0 and thus nested ept with 2MB page is not supported!\n");
            return 0;
        }
    }
    return 0;
}
