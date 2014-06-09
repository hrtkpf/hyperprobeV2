/*
 * This file implements a test case for check pmu v2 feature.
 * Note: To enable pmu v2, -cpu host should be passed to qemu command line, 
 * otherwise, pmu->version will always be zero and cpuid leaf 0xa won't return
 * pmu related information. When using -cpu host, pmu->version would be 2.
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


//To support vPMU, KVM exposes several MSRs to Guest, technically, the following registers should be accessible by the Guest.
//#define MSR_CORE_PERF_FIXED_CTR0        0x00000309
//#define MSR_CORE_PERF_FIXED_CTR1        0x0000030a
//#define MSR_CORE_PERF_FIXED_CTR2        0x0000030b
//#define MSR_CORE_PERF_FIXED_CTR_CTRL    0x0000038d
//#define MSR_CORE_PERF_GLOBAL_STATUS     0x0000038e
//#define MSR_CORE_PERF_GLOBAL_CTRL       0x0000038f
//#define MSR_CORE_PERF_GLOBAL_OVF_CTRL   0x00000390

// Thie function use fork to create a child process. The child process tries to read MSR_CORE_PERF_GLOBAL_CTRL.
// If the register exists, it is readable. Otherwise, it is not readable.
// Return: 1 if feature exist, 0 if not sure.
int test_pmu_v2()
{
	pid_t pid;
	int status;

	if( (pid=fork()) < 0 )
	{
		perror("fail to fork\n");
	}

	if(pid==0)	//child process
	{
		DPRINTF("DEBUG: Child: %s %d \n",__FUNCTION__,__LINE__);
		rdmsr_on_cpu(MSR_CORE_PERF_GLOBAL_CTRL,0);
		DPRINTF("DEBUG: Child: Feature Exists: MSR_CORE_PERF_GLOBAL_CTRL is readable!\n");
		exit(0);
	}else		//parent process
	{
		wait(&status);
                if(WIFEXITED(status))	// WIFEXITED(status) returns true if the child terminated normally
                {
                        if(WEXITSTATUS(status))	// WEXITSTATUS(status) returns  the  exit  status  of the child.
                        {
                                DPRINTF("DEBUG: Parent: The return code of child process is non zero.\n");
                                DPRINTF("DEBUG: Parent: Feature not Exists: MSR_CORE_PERF_GLOBAL_CTRL is not readable!\n");
                                return 0;
                        }
                        else
                        {
                                DPRINTF("DEBUG: Parent: The return code of child process is zero.\n");
                                DPRINTF("DEBUG: Parent: Feature Exists: MSR_CORE_PERF_GLOBAL_CTRL is readable!\n");
                                return 1;       //child process exit normally with exit code 0, which means the register is readable, so the bug is not existing.
                        }
                }else
                {
                        DPRINTF("DEBUG: Parent: Feature not Exists: MSR_CORE_PERF_GLOBAL_CTRL is not readable!\n");
                        return 0;       //child process exit abnormally, the register is not readable, so the bug is existing.
                }
                DPRINTF("DEBUG: Parent: %s %d \n",__FUNCTION__,__LINE__);
	}
	return 0;
}

