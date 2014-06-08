/*
 * This file implements a test case for check the clearing bit 10 of MSR_IA32_MC0_CTL bug.
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

// Thie function use fork to create a child process. The child process tries to clear bit 10 of MSR_IA32_MC0_CTL.
// If the register exists, it is readable. Otherwise, it is not readable.
// Return: 1 if bug exist, 0 if not sure.
int test_msr_ia32_mc0_ctl()
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
		wrmsr_on_cpu(MSR_IA32_MC0_CTL,0,0x0);	//FIXME: Maybe we should read the original value and just clear bit 10, rather than clear every bit.
		DPRINTF("DEBUG: Child: Bug Fixed: bit 10 of MSR_IA32_MC0_CTL is clearable!\n");
		exit(0);
	}else		//parent process
	{
		wait(&status);
		if(WIFEXITED(status))
		{
                        if(WEXITSTATUS(status))
                        {
                                DPRINTF("DEBUG: Parent: The return code of child process is non zero.\n");
                                DPRINTF("DEBUG: Parent: Bug Exists: clearing bit 10 of MSR_IA32_MC0_CTL is not allowed!\n");
                                return 1;
                        }
                        else
                        {
                                DPRINTF("DEBUG: Parent: The return code of child process is zero.\n");
                                DPRINTF("DEBUG: Parent: Bug Fixed: clearing bit 10 of MSR_IA32_MC0_CTL is allowed!\n");
                                return 0;       //child process exit normally with exit code 0, which means the register is readable, so the bug is not existing.
                        }
		}else
		{
			DPRINTF("DEBUG: Parent: Bug Exists: clearing bit 10 of MSR_IA32_MC0_CTL is not allowed!\n");
			return 1;	//child process exit abnormally, the register is not writable, so the bug is existing.
		}
		DPRINTF("DEBUG: Parent: %s %d \n",__FUNCTION__,__LINE__);
	}
	return 0;
}

