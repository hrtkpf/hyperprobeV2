/*
 * This file implements a test case for check ple feature.
 * Initial work by:
 *   (c) 2014 Jidong Xiao (jidong.xiao@gmail.com)
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
#include <inttypes.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"

/*
uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}
*/

// Thie function use the sysconf function to get the number of processors,
// if it is more than 1, then we assume Guest smp is supported.
// Otherwise, we are not sure smp is supported or not.
// Return: 1 if feature exist, 0 if not sure.
int test_ple()
{
/*We are not able to detect PLE at this moment, so let's just return 0 and assume it is not existing.*/
	return 0;
/*
	int i;
	unsigned long long int cycles_1 = rdtsc();	
	for(i=0;i<50;i++)
		__asm__ __volatile__("pause\n");
	unsigned long long int cycles_2 = rdtsc();
	for(i=0;i<200;i++)
		__asm__ __volatile__("pause\n");
	unsigned long long int cycles_3 = rdtsc();

	unsigned long long int t200 = cycles_2 - cycles_1;
        unsigned long long int t50 = cycles_3 - cycles_2;

        double ratio = (double)t200/(double)t50;
	printf("ratio is %f\n");
	
	if(ratio>5)
		return 1;
	else
		return 0;
*/
}
