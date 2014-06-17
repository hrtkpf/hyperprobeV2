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

#define MSR_IA32_MC0_CTL          	0x00000400	// Clear bit 10 of this register is not allowed until 2.6.34
#define MSR_K7_HWCR                     0xc0010015	// Writing 0x100 to it is not supported until kernel 2.6.35
							// Writing 0x8 to it is not supported until kernel 3.4
#define MSR_EBC_FREQUENCY_ID            0x0000002c	// Not readable until kernel 2.6.37
#define MSR_IA32_BBL_CR_CTL3            0x0000011e	// Not readable until kernel 2.6.39

#define MSR_IA32_UCODE_REV              0x0000008b	// Not readable until kernel 3.2
#define MSR_IA32_TSC_ADJUST             0x0000003b	// Not readable until kernel 3.8
#define MSR_AMD64_BU_CFG2         	0xc001102a	// Not readable until kernel 3.9

#define MSR_IA32_VMX_ENTRY_CTLS         0x00000484	// Since kernel 3.10, when nested=1, bit 0-8, bit 12 of this register should be 1.
							// Before 3.10, this might be some other value.
#define VM_ENTRY_ALWAYSON_WITHOUT_TRUE_MSR	0x000011ff	// i.e., bit 0-8 and bit 12 is 1.

#define MSR_IA32_FEATURE_CONTROL        0x0000003a	// When nested=1, writable even if lock bit is set, fixed in kernel 3.12
							// When nested=0, readable, but is is not supposed to be readable when nested=0, fixed in kernel 3.12
#define FEATURE_CONTROL_LOCKED          (1<<0)

#define MSR_IA32_APICBASE         	0x0000001b	// Reserve bits of this register is not writable since kernel 3.14.

/* PMUv2 related MSR */
/* Intel Core-based CPU performance counters */
#define MSR_CORE_PERF_FIXED_CTR0        0x00000309
#define MSR_CORE_PERF_FIXED_CTR1        0x0000030a
#define MSR_CORE_PERF_FIXED_CTR2        0x0000030b
#define MSR_CORE_PERF_FIXED_CTR_CTRL    0x0000038d
#define MSR_CORE_PERF_GLOBAL_STATUS     0x0000038e
#define MSR_CORE_PERF_GLOBAL_CTRL       0x0000038f
#define MSR_CORE_PERF_GLOBAL_OVF_CTRL   0x00000390

/* Custom MSRs */
#define MSR_KVM_API_MAGIC		0x87655678
#define MSR_KVM_WALL_CLOCK_NEW		0x4b564d00
#define MSR_KVM_SYSTEM_TIME_NEW		0x4b564d01
#define MSR_KVM_ASYNC_PF_EN		0x4b564d02
#define MSR_KVM_STEAL_TIME		0x4b564d03
#define HV_X64_MSR_APIC_ASSIST_PAGE     0x40000073
#define MSR_P6_PERFCTR0			0x000000c1	// Used for detecting pmuv2
#define MSR_KVM_PV_EOI_EN		0x4b564d04	// Used for detecting paravirtualized EOI enable.
#define MSR_IA32_VMX_PINBASED_CTLS      0x00000481	// Used for detecting preemption timer.
#define MSR_IA32_VMX_PROCBASED_CTLS2    0x0000048b	// Used for detecting nested ept.
#define MSR_IA32_VMX_EPT_VPID_CAP       0x0000048c	// Used for detecting nested ept 2MB page.
/* MSR used to read the per-partition time reference counter */
#define HV_X64_MSR_TIME_REF_COUNT	0x40000020

uint64_t rdmsr_on_cpu(uint32_t reg, int cpu);
void wrmsr_on_cpu(uint32_t reg, int cpu, uint64_t data);

#endif /* msr.h */
