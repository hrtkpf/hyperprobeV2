/*
 * This file implements a test case for check ksm feature.
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
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"

int test_ksm()
{
	DPRINTF("DEBUG: Passed %s %d \n",__FUNCTION__,__LINE__);
//	printf("test ksm.\n");
	return 0;
}
