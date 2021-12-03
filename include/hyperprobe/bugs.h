/*
 * bugs.h: bugs related definitions
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
#ifndef __BUGS_H
#define __BUGS_H

#define BUG_MSR_IA32_MCG_STATUS        0
#define BUG_MSR_IA32_EBL_CR_POWERON    1
#define BUG_MSR_IA32_MCG_CTL        2
#define BUG_MSR_IA32_PERF_STATUS    3
#define BUG_MSR_IA32_MC0_MISC5        4
#define BUG_MSR_VM_HSAVE_PA        5
#define BUG_MSR_K7_EVNTSEL0        6
#define BUG_DR_UNCHECK            7
#define BUG_MSR_IA32_MC0_CTL        8
//#define BUG_DR_NULL			8
#define BUG_MSR_K7_HWCR_0x100        9
#define BUG_MSR_EBC_FREQUENCY_ID    10
#define BUG_MSR_IA32_BBL_CR_CTL3    11
#define BUG_MSR_IA32_UCODE_REV        12
#define BUG_CPUID_SIGNATURE        13
#define BUG_MSR_K7_HWCR_0x8        14
#define BUG_MSR_IA32_TSC_ADJUST        15
#define BUG_MSR_AMD64_BU_CFG2        16
//#define BUG_VNC_CRASH			13
//#define BUG_UNUSABLE_PRESENT 		14
#define BUG_MSR_IA32_VMX_ENTRY_CTLS    17
#define BUG_MSR_IA32_FEATURE_CONTROL    18
#define BUG_MSR_IA32_APICBASE        19

extern int test_msr_ia32_mcg_status();
extern int test_msr_ia32_ebl_cr_poweron();
extern int test_msr_ia32_mcg_ctl();
extern int test_msr_ia32_perf_status();
extern int test_msr_ia32_mc0_misc5();
extern int test_msr_vm_hsave_pa();
extern int test_msr_k7_evntsel0();
extern int test_dr_uncheck();
extern int test_msr_ia32_mc0_ctl();
//extern int test_dr_null();
extern int test_msr_k7_hwcr_0x100();
extern int test_msr_ebc_frequency_id();
extern int test_msr_ia32_bbl_cr_ctl3();
extern int test_msr_ia32_ucode_rev();
extern int test_msr_k7_hwcr_0x8();
extern int test_cpuid_signature();
extern int test_msr_ia32_tsc_adjust();
extern int test_msr_amd64_bu_cfg2();
//extern int test_vnc_crash();
extern int test_msr_ia32_vmx_entry_ctls();
//extern int test_unusable_present();
extern int test_msr_ia32_feature_control();
extern int test_msr_ia32_apicbase();

#endif /* bugs.h */
