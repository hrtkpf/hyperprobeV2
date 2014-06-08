/*
 * features.h: features related definitions
 * Copyright (c) 2014, Jidong Xiao (jidong.xiao@gmail.com).
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

#define FEATURE_KVM_MAGIC	0
#define FEATURE_SMP		1
#define FEATURE_KVM_SIGNATURE	2
#define FEATURE_EPT		3
#define FEATURE_MTRR		4
#define FEATURE_POPCNT		5
#define FEATURE_KSM		6
#define FEATURE_PLE		7
#define FEATURE_RDTSCP		8
#define FEATURE_KVM_CLOCK_NEW	9
#define FEATURE_ASYNC_PF	10
#define FEATURE_STEAL_TIME	11
#define FEATURE_PMU_V2		12
//#define FEATURE_NESTED_VMX		3
#define FEATURE_X2APIC	 	13
#define FEATURE_NESTED_EPT 	14

extern int test_kvm_magic();
extern int test_smp();
extern int test_kvm_signature();
extern int test_ept();
extern int test_mtrr();
extern int test_popcnt();
extern int test_ksm();
extern int test_ple();
extern int test_rdtscp();
extern int test_kvm_clock_new();
extern int test_kvm_async_pf();
extern int test_kvm_steal_time();
extern int test_pmu_v2();
//extern int test_nested_vmx();
extern int test_x2apic();
extern int test_nested_ept();

#endif /* features.h */
