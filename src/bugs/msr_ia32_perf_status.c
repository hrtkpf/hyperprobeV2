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
int test_msr_ia32_perf_status()
{
	uint64_t data;

	data=rdmsr_on_cpu(MSR_IA32_PERF_STATUS,0);
	if(data==0)
	{
		DPRINTF("DEBUG: Child: Bug Exists: MSR_IA32_PERF_STATUS returns 0 upon read!\n");
		return 1;
	}
	else
	{
		DPRINTF("DEBUG: Child: Bug Fixed: MSR_IA32_PERF_STATUS returns non-zero upon read!\n");
		DPRINTF("DEBUG: Child: Bug Fixed: MSR_IA32_PERF_STATUS returns %llx\n",data);
		return 0;
	}
}
