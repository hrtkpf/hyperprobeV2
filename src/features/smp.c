/*
 * This file implements a test case for check smp feature.
 * Initial work by:
 *   (c) 2014 Lei Lu (lulei.wm@gmail.com)
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
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"

// Thie function use the sysconf function to get the number of processors,
// if it is more than 1, then we assume Guest smp is supported.
// Otherwise, we are not sure smp is supported or not.
// Return: 1 if feature exist, 0 if not sure.
int test_smp()
{
	int smp;
	smp=sysconf( _SC_NPROCESSORS_ONLN );
	DPRINTF("smp is %d\n",smp);
	if(smp>1)
		return 1;
	else
		return 0;
}
