/*
 * This file implements a test case for check nested virtualization feature.
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
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"
#include "hyperprobe/cpuid.h"

int test_x2apic() {
    /*We cannot detect virtual x2apic now, as we don't have the hardware, so let's just return 0 now.*/
    return 0;
    int a, b, c, d;
    DPRINTF("DEBUG: Passed %s %d \n", __FUNCTION__, __LINE__);
    // When we set EAX=1 and run CPUID instruction, the returning value in bit 21 of ECX indicates whether or not x2apic is supported.
    cpuid(0x1, a, b, c, d);
    if (c & (1 << ECX_BIT_X2APIC)) {
        DPRINTF("DEBUG: Feature: x2apic exists!\n");
        return 1;
    } else {
        DPRINTF("DEBUG: Feature: x2apic not exists!\n");
        return 0;
    }
    return 0;
}
