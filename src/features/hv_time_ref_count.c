/*
 * This file implements a test case for check Hyper-V HV_X64_MSR_TIME_REF_COUNT feature.
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

// Thie function use fork to create a child process. The child process tries to read HV_X64_MSR_TIME_REF_COUNT.
// If the register exists, it is readable. Otherwise, it is not readable.
// Return: 1 if feature exist, 0 if not sure.
int test_hv_time_ref_count()
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
		rdmsr_on_cpu(HV_X64_MSR_TIME_REF_COUNT,0);
		DPRINTF("DEBUG: Child: Feature Exists: HV_X64_MSR_TIME_REF_COUNT is readable!\n");
		exit(0);
	}else		//parent process
	{
		wait(&status);
                if(WIFEXITED(status))	// WIFEXITED(status) returns true if the child terminated normally
                {
                        if(WEXITSTATUS(status))	// WEXITSTATUS(status) returns  the  exit  status  of the child.
                        {
                                DPRINTF("DEBUG: Parent: The return code of child process is non zero.\n");
                                DPRINTF("DEBUG: Parent: Feature not Exists: HV_X64_MSR_TIME_REF_COUNT is not readable!\n");
                                return 0;
                        }
                        else
                        {
                                DPRINTF("DEBUG: Parent: The return code of child process is zero.\n");
                                DPRINTF("DEBUG: Parent: Feature Exists: HV_X64_MSR_TIME_REF_COUNT is readable!\n");
                                return 1;       //child process exit normally with exit code 0, which means the register is readable, so the feature is existing.
                        }
                }else
                {
                        DPRINTF("DEBUG: Parent: Feature not Exists: HV_X64_MSR_TIME_REF_COUNT is not readable!\n");
                        return 0;       //child process exit abnormally, the register is not readable, so the feature is not existing.
                }
                DPRINTF("DEBUG: Parent: %s %d \n",__FUNCTION__,__LINE__);
	}
	return 0;
}
