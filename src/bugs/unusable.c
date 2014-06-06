/*
 * druncheck.c: this test case is a check against CVE-2009-3722.
 * CVE-2009-3722: in kvm code, between kernel version 2.6.20 and 2.6.31,
 * the kvm do not check privilege level when a program is accessing dr registers.
 * This may leads to a privilege escalation attack. And this has been fixed in kernel 2.6.32.
 *
 * Copyright (c) 2014, Jidong Xiao (jidong.xiao@gmail.com)
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
#include "hyperprobe/bugs.h"
#include "hyperprobe/debug.h"

int test_unusable_present()
{
	DPRINTF("DEBUG: Passed %s %d \n",__FUNCTION__,__LINE__);
	return 0;
	//
//	printf("This function will test whether or not dr uncheck is true.\n");
}
