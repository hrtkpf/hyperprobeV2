/*
 * This file implements a test case for check the msr_ia32_feature_control bug.
 * There were 2 bugs in accessing the MSR register MSR_IA32_FEATURE_CONTROL:
 * 1. When nested=0, this register is not supposed to be readable, but it is indeed readable until the fix in kernel 3.12.
 * 2. When nested=1, this register is not supposed to be writable if lock bit is set, but it is indeed writable until the fix in kernel 3.12.
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
#include "hyperprobe/cpuid.h"
#include "hyperprobe/msr.h"

int msr_ia32_feature_control_read_bug_exists();
int msr_ia32_feature_control_write_bug_exists();

int test_msr_ia32_feature_control()
{
	int a,b,c,d;
	DPRINTF("DEBUG: Passed %s %d \n",__FUNCTION__,__LINE__);
	// When we set EAX=1 and run CPUID instruction, the returning value in bit 5 of ECX indicates whether or not vmx is supported.
	cpuid(0x1,a,b,c,d);
	if(c & 1<<ECX_BIT_VMX)	// Considering nested=1
	{
		DPRINTF("DEBUG: Feature: Nested VMX exists!\n");
		return msr_ia32_feature_control_write_bug_exists();
	}else	// Considering nested=0
	{
		DPRINTF("DEBUG: Feature: Nested VMX not exists!\n");
		return msr_ia32_feature_control_read_bug_exists();
	}
}


int msr_ia32_feature_control_read_bug_exists()
{
        pid_t pid;
        int status;

        if( (pid=fork()) < 0 )
        {
                perror("fail to fork\n");
        }

        if(pid==0)      //child process
        {
                DPRINTF("DEBUG: Child: %s %d \n",__FUNCTION__,__LINE__);
                rdmsr_on_cpu(MSR_IA32_FEATURE_CONTROL,0);
                DPRINTF("DEBUG: Child: Bug Exists: MSR_IA32_FEATURE_CONTROL is readable!\n");
                exit(0);
        }else           //parent process
        {
                wait(&status);
                if(WIFEXITED(status))
                {
                        if(WEXITSTATUS(status))
                        {
                                DPRINTF("DEBUG: Parent: The return code of child process is non zero.\n");
                                DPRINTF("DEBUG: Parent: Bug Fixed: MSR_IA32_FEATURE_CONTROL is not readable!\n");
                                return 0;
                        }
                        else
                        {
                                DPRINTF("DEBUG: Parent: The return code of child process is zero.\n");
                                DPRINTF("DEBUG: Parent: Bug Exists: MSR_IA32_FEATURE_CONTROL is readable!\n");
                                return 1;       //child process exit normally with exit code 0, which means the register is readable, so the bug is existing.
                        }

                }else
                {
                        DPRINTF("DEBUG: Parent: Bug Fixed: MSR_IA32_FEATURE_CONTROL is not readable!\n");
                        return 0;       //child process exit abnormally, the register is not readable, so the bug is fixed.
                }
                DPRINTF("DEBUG: Parent: %s %d \n",__FUNCTION__,__LINE__);
        }
}

int msr_ia32_feature_control_write_bug_exists()
{
        pid_t pid;
        int status;

        if( (pid=fork()) < 0 )
        {
                perror("fail to fork\n");
        }

        if(pid==0)      //child process
        {
                DPRINTF("DEBUG: Child: %s %d \n",__FUNCTION__,__LINE__);
		wrmsr_on_cpu(MSR_IA32_FEATURE_CONTROL,0,FEATURE_CONTROL_LOCKED);
		wrmsr_on_cpu(MSR_IA32_FEATURE_CONTROL,0,0x2);
                DPRINTF("DEBUG: Child: Bug Exists: MSR_IA32_FEATURE_CONTROL is writable even after lock set!\n");
                exit(0);
        }else           //parent process
        {
                wait(&status);
                if(WIFEXITED(status))
                {
                        if(WEXITSTATUS(status))
                        {
                                DPRINTF("DEBUG: Parent: The return code of child process is non zero.\n");
                                DPRINTF("DEBUG: Parent: Bug Fixed: MSR_IA32_FEATURE_CONTROL is not writable!\n");
                                return 0;
                        }
                        else
                        {
                                DPRINTF("DEBUG: Parent: The return code of child process is zero.\n");
                                DPRINTF("DEBUG: Parent: Bug Exists: MSR_IA32_FEATURE_CONTROL is writable even after lock set!\n");
                                return 1;       //child process exit normally with exit code 0, which means the register is readable, so the bug is existing.
                        }

                }else
                {
                        DPRINTF("DEBUG: Parent: Bug Fixed: MSR_IA32_FEATURE_CONTROL is not writable!\n");
                        return 0;       //child process exit abnormally, the register is not readable, so the bug is fixed.
                }
                DPRINTF("DEBUG: Parent: %s %d \n",__FUNCTION__,__LINE__);
        }
}
