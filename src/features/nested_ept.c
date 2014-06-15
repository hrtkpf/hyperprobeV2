/*
 * This file implements a test case for check the nested ept feature.
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
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/msr.h"

extern int test_nested_vmx();
uint64_t data;

// Thie function use fork to create a child process. The child process tries to read MSR_IA32_VMX_PROCBASED_CTLS2.
// If MSR_IA32_VMX_PROCBASED_CTLS2[33] is 1, it indicates ept is supported. If is is 0, then ept is not supported.
// Return: 1 if feature exist, 0 if not sure.
int test_nested_ept()
{

        /* This feature only makes sense if nested vmx is enabled. */
        if(!test_nested_vmx())
                return 0;

        pid_t pid;
        int status;

        if( (pid=vfork()) < 0 )
        {
                perror("fail to fork\n");
        }

        if(pid==0)      // child process
        {
                DPRINTF("DEBUG: Child: %s %d \n",__FUNCTION__,__LINE__);
		data=rdmsr_on_cpu(MSR_IA32_VMX_PROCBASED_CTLS2,0);
		exit(0);
	}else	// parent process
        {
                wait(&status);
		if(data & (1ULL<<33))
		{
			DPRINTF("DEBUG: Parent: Feature exists: MSR_IA32_VMX_PROCBASED_CTLS2[33] is 1 and thus nested ept is supported!\n");
			return 1;
		}
		else
		{
			DPRINTF("DEBUG: Parent: Feature not exists: MSR_IA32_VMX_PROCBASED_CTLS2[33] is 0 and thus nested ept is not supported!\n");
			return 0;
		}
	}
	return 0;
}
