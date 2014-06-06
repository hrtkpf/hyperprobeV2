/*
 * msr.h:  This file defines msr read and write functions.
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
 *
 */
#ifndef __MSR_H
#define __MSR_H

#include <inttypes.h>

#define MSR_IA32_MCG_STATUS             0x0000017a
#define MSR_IA32_EBL_CR_POWERON         0x0000002a 
#define MSR_IA32_MCG_CTL		0x0000017b
#define MSR_IA32_PERF_STATUS		0x00000198
#define MSR_IA32_MC0_MISC		0x00000403
#define MSR_VM_HSAVE_PA			0xc0010117
#define MSR_K7_EVNTSEL0			0xc0010000

uint64_t rdmsr_on_cpu(uint32_t reg, int cpu);
void wrmsr_on_cpu(uint32_t reg, int cpu, int valcnt, char *regvals[]);

#endif /* msr.h */
