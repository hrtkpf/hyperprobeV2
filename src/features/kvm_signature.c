/*
 * This file implements a test case for check kvm signature feature.
 * Initial work by:
 *   (c) 2014 Jidong Xiao (lulei.wm@gmail.com)
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
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"

int test_kvm_signature()
{
	int a,b,c,d;
	DPRINTF("DEBUG: Passed %s %d \n",__FUNCTION__,__LINE__);
	// When we set EAX=0x40000000 and run CPUID instruction, the returning value in EDX should be 0x4d, which is the ascii code of letter "M".
	cpuid(0x40000000,a,b,c,d);
	if(d==EDX_SIGNATURE)
	{
		DPRINTF("DEBUG: Feature: KVM signature exists!\n");
		return 1;
	}else
	{
		DPRINTF("DEBUG: Feature: KVM signature does not exist!\n");
		return 0;
	}
	return 0;
}
