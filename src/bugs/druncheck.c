/*
 * druncheck.c: this test case is a check against CVE-2009-3722.
 * CVE-2009-3722: in kvm code, between kernel version 2.6.20 and 2.6.31,
 * the kvm do not check privilege level when a program is accessing dr registers.
 * This may leads to a privilege escalation attack. And this has been fixed in kernel 2.6.32.
 * Copyright (c) 2014, Jidong Xiao (jidong.xiao@gmail.com).
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "hyperprobe/bugs.h"
#include "hyperprobe/debug.h"

static unsigned long get_dr6(void)
{
        unsigned long value;

        asm volatile("mov %%dr6,%0" : "=r" (value));
        return value;
}

// This function forks a child process, if the child process successfully accessed dr register, 
// it implies that the dr uncheck access bug is existing. Otherwise, if the child process is terminated
// by a segmemtation fault signal, it suggests the bug is fixed.
// Return: 1 if bug exist, 0 if bug not exist.
int test_dr_uncheck()
{
	pid_t pid;
	unsigned long dr;
	int status;

	if( (pid=fork()) < 0 )
	{
		perror("fail to fork\n");
	}

	if(pid==0)	//child process
	{
		DPRINTF("DEBUG: Child: %s %d \n",__FUNCTION__,__LINE__);
		dr=get_dr6();
		DPRINTF("DEBUG: Child: Bug Exists: dr register is accessible by ring3 program!\n");
		exit(0);
	}else		//parent process
	{
		wait(&status);
		if(WIFEXITED(status))
		{
			return 1;	//child process exit normally, which means dr register is accessible, so the bug is existing.
		}else if(WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGSEGV)
			{
				DPRINTF("DEBUG: Parent: the child process is terminated by a segmentation fault signal\n");
				DPRINTF("DEBUG: Parent: Bug Fixed: dr register is not accessible by ring3 program!\n");
				return 0; //child process is terminated by a segmentation fault signal, which means the bug does not exist.
			}
			else
				DPRINTF("This is funny. The child process is terminated by some other signals?\n");

		}
//		printf("DEBUG: Parent: %s %d \n",__FUNCTION__,__LINE__);
	}
	return 0;
	//
}
