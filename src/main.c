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
        [FEATURE_EPT]                   = test_ept,
        [FEATURE_NESTED]                = test_nested,
        [FEATURE_KSM]                   = test_ksm,
        [FEATURE_NESTED_EPT]            = test_nested_ept,
};

static const int kvm_max_feature_testers =
        ARRAY_SIZE(kvm_feature_testers);

static int const kvm_feature_start[] = {
        [FEATURE_KVM_MAGIC]             = 21,
        [FEATURE_SMP]                   = 23,
        [FEATURE_EPT]                   = 26,
        [FEATURE_NESTED]                = 31,
        [FEATURE_KSM]                   = 32,
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
        [BUG_DR_NULL]			= test_dr_null,
        [BUG_VNC_CRASH]			= test_vnc_crash,
        [BUG_UNUSABLE_PRESENT]		= test_unusable_present,
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
        [BUG_DR_NULL]			= 20,
        [BUG_VNC_CRASH]			= 39,
        [BUG_UNUSABLE_PRESENT]		= 20,
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
        [BUG_DR_NULL]			= 35,
        [BUG_VNC_CRASH]			= 300,
        [BUG_UNUSABLE_PRESENT]		= 311,
};

int main()
{
        int ifeature;
        int ibug;

	int vmax=VERSION_MAX;
	int vmin=VERSION_MIN;	

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
       	printf("\n\nThe underlying hypervisor kernel version should be between %d and %d\n",vmin,vmax);
        return 0;
}

