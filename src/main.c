/*
 * This file includes the main function of the hyperprobe project.
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
#include "hyperprobe/features.h"
#include "hyperprobe/bugs.h"
#include "hyperprobe/lib.h"
#include "hyperprobe/debug.h"

#ifndef DEBUG
#define DEBUG 0
#endif

static int (*const kvm_feature_testers[])() = {
        [FEATURE_KVM_MAGIC]             = test_kvm_magic,
        [FEATURE_SMP]                   = test_smp,
        [FEATURE_KVM_SIGNATURE]         = test_kvm_signature,
        [FEATURE_EPT]                   = test_ept,
        [FEATURE_MTRR]                  = test_mtrr,
        [FEATURE_POPCNT]                = test_popcnt,
        [FEATURE_KSM]                   = test_ksm,
        [FEATURE_PLE]                   = test_ple,
        [FEATURE_RDTSCP]                = test_rdtscp,
        [FEATURE_KVM_CLOCK_NEW]         = test_kvm_clock_new,
        [FEATURE_ASYNC_PF]		= test_kvm_async_pf,
        [FEATURE_STEAL_TIME]		= test_kvm_steal_time,
//        [FEATURE_NESTED]                = test_nested_vmx,
        [FEATURE_PMU_V2]		= test_pmu_v2,
        [FEATURE_PV_EOI_EN]		= test_pv_eoi_en,
        [FEATURE_X2APIC]            	= test_x2apic,
        [FEATURE_NESTED_EPT]            = test_nested_ept,
};

static const int kvm_max_feature_testers =
        ARRAY_SIZE(kvm_feature_testers);

static int const kvm_feature_start[] = {
        [FEATURE_KVM_MAGIC]             = 21,
        [FEATURE_SMP]                   = 23,
        [FEATURE_KVM_SIGNATURE]         = 25,
        [FEATURE_EPT]                   = 26,
        [FEATURE_MTRR]                  = 27,
        [FEATURE_POPCNT]                = 31,
        [FEATURE_KSM]                   = 32,
        [FEATURE_PLE]                   = 33,
        [FEATURE_RDTSCP]                = 34,
        [FEATURE_KVM_CLOCK_NEW]         = 35,
        [FEATURE_ASYNC_PF]		= 38,
        [FEATURE_STEAL_TIME]		= 301,
//        [FEATURE_NESTED_VMX]            = 301,
        [FEATURE_PMU_V2]		= 303,
        [FEATURE_PV_EOI_EN]		= 306,
        [FEATURE_X2APIC]            	= 309,
        [FEATURE_NESTED_EPT]            = 312,
};

static int (*const kvm_bug_testers[])() = {
        [BUG_MSR_IA32_MCG_STATUS]	= test_msr_ia32_mcg_status,
        [BUG_MSR_IA32_EBL_CR_POWERON]	= test_msr_ia32_ebl_cr_poweron,
        [BUG_MSR_IA32_MCG_CTL]		= test_msr_ia32_mcg_ctl,
        [BUG_MSR_IA32_PERF_STATUS]	= test_msr_ia32_perf_status,
        [BUG_MSR_IA32_MC0_MISC5]	= test_msr_ia32_mc0_misc5,
        [BUG_MSR_VM_HSAVE_PA]		= test_msr_vm_hsave_pa,
        [BUG_MSR_K7_EVNTSEL0]		= test_msr_k7_evntsel0,
        [BUG_DR_UNCHECK]		= test_dr_uncheck,
        [BUG_MSR_IA32_MC0_CTL]		= test_msr_ia32_mc0_ctl,
//        [BUG_DR_NULL]			= test_dr_null,
        [BUG_MSR_K7_HWCR_0x100]		= test_msr_k7_hwcr_0x100,
        [BUG_MSR_EBC_FREQUENCY_ID]	= test_msr_ebc_frequency_id,
        [BUG_MSR_IA32_BBL_CR_CTL3]	= test_msr_ia32_bbl_cr_ctl3,
//        [BUG_VNC_CRASH]		= test_vnc_crash,	// Have not verified this.
        [BUG_MSR_IA32_UCODE_REV]	= test_msr_ia32_ucode_rev,
        [BUG_MSR_K7_HWCR_0x8]		= test_msr_k7_hwcr_0x8,
        [BUG_CPUID_SIGNATURE]		= test_cpuid_signature,
        [BUG_MSR_IA32_TSC_ADJUST]	= test_msr_ia32_tsc_adjust,
        [BUG_MSR_AMD64_BU_CFG2]		= test_msr_amd64_bu_cfg2,
//        [BUG_UNUSABLE_PRESENT]		= test_unusable_present,
        [BUG_MSR_IA32_FEATURE_CONTROL]	= test_msr_ia32_feature_control,
        [BUG_MSR_IA32_APICBASE]		= test_msr_ia32_apicbase,
};

static const int kvm_max_bug_testers =
        ARRAY_SIZE(kvm_bug_testers);

static int const kvm_bug_start[] = {
        [BUG_MSR_IA32_MCG_STATUS]	= 20,
        [BUG_MSR_IA32_EBL_CR_POWERON]	= 20,
        [BUG_MSR_IA32_MCG_CTL]		= 20,
        [BUG_MSR_IA32_PERF_STATUS]	= 20,
        [BUG_MSR_IA32_MC0_MISC5]	= 20,
        [BUG_MSR_VM_HSAVE_PA]		= 20,
        [BUG_MSR_K7_EVNTSEL0]		= 20,
        [BUG_DR_UNCHECK]		= 20,
        [BUG_MSR_IA32_MC0_CTL]		= 20,
//        [BUG_DR_NULL]			= 20,
        [BUG_MSR_K7_HWCR_0x100]		= 20,
        [BUG_MSR_EBC_FREQUENCY_ID]	= 20,
        [BUG_MSR_IA32_BBL_CR_CTL3]	= 20,
//        [BUG_VNC_CRASH]			= 39,
        [BUG_MSR_IA32_UCODE_REV]	= 20,
        [BUG_MSR_K7_HWCR_0x8]		= 20,
        [BUG_CPUID_SIGNATURE]		= 25,
        [BUG_MSR_IA32_TSC_ADJUST]	= 20,
        [BUG_MSR_AMD64_BU_CFG2]		= 20,
//        [BUG_UNUSABLE_PRESENT]		= 20,
        [BUG_MSR_IA32_FEATURE_CONTROL]	= 301,
        [BUG_MSR_IA32_APICBASE]		= 32,
};

static int const kvm_bug_end[] = {
        [BUG_MSR_IA32_MCG_STATUS]	= 22,
        [BUG_MSR_IA32_EBL_CR_POWERON]	= 23,
        [BUG_MSR_IA32_MCG_CTL]		= 25,
        [BUG_MSR_IA32_PERF_STATUS]	= 26,
        [BUG_MSR_IA32_MC0_MISC5]	= 28,
        [BUG_MSR_VM_HSAVE_PA]		= 30,
        [BUG_MSR_K7_EVNTSEL0]		= 31,
        [BUG_DR_UNCHECK]		= 32,
        [BUG_MSR_IA32_MC0_CTL]		= 34,
//        [BUG_DR_NULL]			= 35,
        [BUG_MSR_K7_HWCR_0x100]		= 35,
        [BUG_MSR_EBC_FREQUENCY_ID]	= 37,
        [BUG_MSR_IA32_BBL_CR_CTL3]	= 39,
//        [BUG_VNC_CRASH]			= 300,
        [BUG_MSR_IA32_UCODE_REV]	= 302,
        [BUG_MSR_K7_HWCR_0x8]		= 304,
        [BUG_CPUID_SIGNATURE]		= 305,
        [BUG_MSR_IA32_TSC_ADJUST]	= 308,
        [BUG_MSR_AMD64_BU_CFG2]		= 309,
//        [BUG_UNUSABLE_PRESENT]		= 311,
        [BUG_MSR_IA32_FEATURE_CONTROL]	= 312,
        [BUG_MSR_IA32_APICBASE]		= 314,
};

int main()
{
        int ifeature;
        int ibug;

	int vmax=VERSION_MAX;
	int vmin=VERSION_MIN;	
	int vmax_print;
	int vmin_print;

        DPRINTF("project kick-off!\n");
	int i=0;

	DPRINTF("===Starting Feature Test!===\n");
	for(i=kvm_max_feature_testers;i>0;i--)
	{
        	ifeature=kvm_feature_testers[i-1]();
		DPRINTF("ifeature is %d\n",ifeature);
		if(ifeature==1)
		{
			DPRINTF("ifeature is 1 while i=%d\n",i);
			vmin=kvm_feature_start[i-1];
			break;
		}
	}

	DPRINTF("===Starting Bug Test!===\n");
	for(i=0;i<kvm_max_bug_testers;i++)
	{
        	ibug=kvm_bug_testers[i]();
		if(ibug==1)
		{
			vmax=kvm_bug_end[i]-1;
			break;
		}
	}
	if(vmin<40)
	{
		if(vmax<40)
       			printf("\n\nThe underlying hypervisor kernel version should be between 2.6.%d and 2.6.%d.\n",vmin,vmax);
		else
		{
			vmax_print=vmax-300;
       			printf("\n\nThe underlying hypervisor kernel version should be between 2.6.%d and 3.%d.\n",vmin,vmax_print);
		}
	}else
	{
		if(vmax<40)
       			printf("\n\nSomething must be wrong, since vmin=%d is greater than vmax=%d.\n",vmin,vmax);
		else
		{
			vmin_print=vmin-300;
			vmax_print=vmax-300;
       			printf("\n\nThe underlying hypervisor kernel version should be between 3.%d and 3.%d.\n",vmin_print,vmax_print);
		}
	}
        return 0;
}

