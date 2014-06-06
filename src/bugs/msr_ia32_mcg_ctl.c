/*
 * This file implements a test case for check the MSR_MCG_STATUS bug.
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

// Thie function use fork to create a child process. The child process tries to read MSR_KVM_API_MAGIC.
// If the register exists, it is readable. Otherwise, it is not readable.
// Return: 1 if feature exist, 0 if not sure.
int test_msr_ia32_mcg_ctl()
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
		rdmsr_on_cpu(MSR_IA32_MCG_CTL,0);
		DPRINTF("DEBUG: Child: Bug Fixed: MSR_IA32_MCG_CTL is readable!\n");
		exit(0);
	}else		//parent process
	{
		wait(&status);
		if(WIFEXITED(status))
		{
                        if(WEXITSTATUS(status))
                        {
                                DPRINTF("DEBUG: Parent: The return code of child process is non zero.\n");
                                DPRINTF("DEBUG: Parent: Bug Exists: MSR_IA32_MCG_CTL is not readable!\n");
                                return 1;
                        }
                        else
                        {
                                DPRINTF("DEBUG: Parent: The return code of child process is zero.\n");
                                DPRINTF("DEBUG: Parent: Bug Fixed: MSR_IA32_MCG_CTL is readable!\n");
                                return 0;       //child process exit normally with exit code 0, which means the register is readable, so the bug is not existing.
                        }

		}else
		{
			DPRINTF("DEBUG: Parent: Bug Exists: MSR_IA32_MCG_CTL is not readable!\n");
			return 1;	//child process exit abnormally, the register is not writable, so the bug is existing.
		}
		DPRINTF("DEBUG: Parent: %s %d \n",__FUNCTION__,__LINE__);
	}
	return 0;
}
