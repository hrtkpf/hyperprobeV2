/*
 * features.h: features related definitions
 * Copyright (c) 2014, Jidong Xiao (jidong.xiao@gmail.com).
 * Copyright (c) 2014, Lei Lu (lulei.wm@gmail.com).
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

#ifndef __FEATURES_H
#define __FEATURES_H

#define FEATURE_KVM_MAGIC    0
#define FEATURE_SMP        1
#define FEATURE_KVM_SIGNATURE    2
#define FEATURE_EPT        3
#define FEATURE_MTRR        4
#define FEATURE_VIRTUAL_DR    5
#define FEATURE_POPCNT        6
#define FEATURE_KSM        7
#define FEATURE_PLE        8
#define FEATURE_RDTSCP        9
#define FEATURE_KVM_CLOCK_NEW    10
#define FEATURE_ASYNC_PF    11
#define FEATURE_STEAL_TIME    12
//#define FEATURE_NESTED_VMX		3
#define FEATURE_HV_APIC_ASSIST_PAGE    13
#define FEATURE_PMU_V2        14
#define FEATURE_PV_EOI_EN    15
#define FEATURE_X2APIC        16
#define FEATURE_NESTED_PREEMPTION_TIMER    17
#define FEATURE_NESTED_EPT    18
#define FEATURE_NESTED_EPT_2MB    19
#define FEATURE_HV_TIME_REF_COUNT    20
#define FEATURE_RDSEED_INSTRUCTION 21
#define FEATURE_INVARIANT_TSC 22
#define FEATURE_ARAT 23
#define FEATURE_HV_X64_MSR_RESET 24
#define FEATURE_HV_X64_MSR_TSC_FREQUENCY 25
#define FEATURE_MSR_IA32_ARCH_CAPABILITIES 26
#define FEATURE_UMIP 27
#define FEATURE_HV_X64_MSR_REENLIGHTENMENT_CONTROL 28
#define FEATURE_IPI_HYPERCALL 29
#define FEATURE_WBNOINVD_INSTRUCTION 30
#define FEATURE_MSR_IA32_POWER_CTL_INTEL 31
#define FEATURE_MSR_IA32_POWER_CTL 32
#define FEATURE_SCHED_YIELD_HYPERCALL 33
#define FEATURE_ASYNC_PF_INT 34

extern int test_kvm_magic();
extern int test_smp();
extern int test_kvm_signature();
extern int test_ept();
extern int test_mtrr();
extern int test_virtual_dr();
extern int test_popcnt();
extern int test_ksm();
extern int test_ple();
extern int test_rdtscp();
extern int test_kvm_clock_new();
extern int test_kvm_async_pf();
extern int test_kvm_steal_time();
//extern int test_nested_vmx();
extern int test_hv_apic_assist_page();
extern int test_pmu_v2();
extern int test_pv_eoi_en();
extern int test_x2apic();
extern int test_nested_preemption_timer();
extern int test_nested_ept();
extern int test_nested_ept_2mb();
extern int test_hv_time_ref_count();
extern int test_rdseed_instruction();
extern int test_invariant_tsc();
extern int test_arat();
extern int test_hv_x64_msr_reset();
extern int test_hv_x64_msr_tsc_frequency();
extern int test_msr_ia32_arch_capabilities();
extern int test_umip();
extern int test_hv_x64_msr_reenlightenment_control();
extern int test_ipi_hypercall();
extern int test_wbnoinvd_instruction();
extern int test_msr_ia32_power_ctl_intel();
extern int test_msr_ia32_power_ctl();
extern int test_sched_yield_hypercall();
extern int test_async_pf_int();

#endif /* features.h */
