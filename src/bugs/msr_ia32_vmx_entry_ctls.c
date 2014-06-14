/*
 * This file implements a test case for check the MSR_IA32_VMX_ENTRY_CTLS bug.
 * This bug can only be detected when nested=1, because it is only readable when nested=1.
 * According to Intel SDM, bit 0-8 and bit 12 of this register (in KVM case) should be 1.
 * But in kernel version before 3.10, these bits might be some other/random values.
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

uint64_t data;
int msr_ia32_vmx_entry_ctls_read_bug_exists();

int test_msr_ia32_vmx_entry_ctls()
{
	int a,b,c,d;
	DPRINTF("DEBUG: Passed %s %d \n",__FUNCTION__,__LINE__);
	// When we set EAX=1 and run CPUID instruction, the returning value in bit 5 of ECX indicates whether or not vmx is supported.
	cpuid(0x1,a,b,c,d);
	if(c & 1<<ECX_BIT_VMX)	// Considering nested=1
	{
		DPRINTF("DEBUG: Feature: Nested VMX exists!\n");
		return msr_ia32_vmx_entry_ctls_read_bug_exists();
	}else	// Considering nested=0
	{
		DPRINTF("DEBUG: Bug: When Nested=0, we are not sure whether or not this bug exists, so just return 0!\n");
		return 0;
	}
}


int msr_ia32_vmx_entry_ctls_read_bug_exists()
{
        pid_t pid;
        int status;

        if( (pid=vfork()) < 0 )
        {
                perror("fail to fork\n");
        }

        if(pid==0)      // child process
        {
                DPRINTF("DEBUG: Child: %s %d \n",__FUNCTION__,__LINE__);
                data=rdmsr_on_cpu(MSR_IA32_VMX_ENTRY_CTLS,0);
                exit(0);
        }else   // parent process
        {
                wait(&status);
                if((data & VM_ENTRY_ALWAYSON_WITHOUT_TRUE_MSR) == VM_ENTRY_ALWAYSON_WITHOUT_TRUE_MSR)
                {
                        DPRINTF("DEBUG: Parent: Bug Fixed: MSR_IA32_VMX_ENTRY_CTLS[0-8,12] is 1 as per spec demands!\n");
                        return 0;
                }
                else
                {
                        DPRINTF("DEBUG: Parent: Bug Exists: MSR_IA32_VMX_ENTRY_CTLS[0-8,12] are not all 1 as per spec demands!\n");
                        return 1;
                }
        }
        DPRINTF("DEBUG: Parent: Please fix me, we are not sure why this MSR is not readable. \n");
        return 0;

}

