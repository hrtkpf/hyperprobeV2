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
        [FEATURE_VIRTUAL_DR]            = test_virtual_dr,
        [FEATURE_POPCNT]                = test_popcnt,
        [FEATURE_KSM]                   = test_ksm,
        [FEATURE_PLE]                   = test_ple,
        [FEATURE_RDTSCP]                = test_rdtscp,
        [FEATURE_KVM_CLOCK_NEW]         = test_kvm_clock_new,
        [FEATURE_ASYNC_PF]        = test_kvm_async_pf,
        [FEATURE_STEAL_TIME]        = test_kvm_steal_time,
//        [FEATURE_NESTED]                = test_nested_vmx,
        [FEATURE_HV_APIC_ASSIST_PAGE]    = test_hv_apic_assist_page,
        [FEATURE_PMU_V2]        = test_pmu_v2,
        [FEATURE_PV_EOI_EN]        = test_pv_eoi_en,
        [FEATURE_X2APIC]                = test_x2apic,
        [FEATURE_NESTED_PREEMPTION_TIMER] = test_nested_preemption_timer,
        [FEATURE_NESTED_EPT]            = test_nested_ept,
        [FEATURE_NESTED_EPT_2MB]        = test_nested_ept_2mb,
        [FEATURE_HV_TIME_REF_COUNT]     = test_hv_time_ref_count,
        [FEATURE_RDSEED_INSTRUCTION]    = test_rdseed_instruction,
        [FEATURE_ARAT]                  = test_arat,
        [FEATURE_HV_X64_MSR_CRASH_CTL]  = test_hv_x64_msr_crash_ctl,
        [FEATURE_HV_X64_MSR_RESET]      = test_hv_x64_msr_reset,
        [FEATURE_HV_X64_MSR_TSC_FREQUENCY] = test_hv_x64_msr_tsc_frequency,
        [FEATURE_UMIP]                  = test_umip,
        [FEATURE_MSR_IA32_ARCH_CAPABILITIES] = test_msr_ia32_arch_capabilities,
        [FEATURE_HV_X64_MSR_REENLIGHTENMENT_CONTROL] = test_hv_x64_msr_reenlightenment_control,
        [FEATURE_IPI_HYPERCALL]         = test_ipi_hypercall,
        [FEATURE_MSR_IA32_POWER_CTL_INTEL] = test_msr_ia32_power_ctl_intel,
        [FEATURE_MSR_IA32_POWER_CTL]    = test_msr_ia32_power_ctl,
        [FEATURE_SCHED_YIELD_HYPERCALL] = test_sched_yield_hypercall,
        [FEATURE_ASYNC_PF_INT]          = test_async_pf_int,
};

static const int kvm_max_feature_testers =
        ARRAY_SIZE(kvm_feature_testers);

static int const kvm_feature_start[] = {
        [FEATURE_KVM_MAGIC]             = 21,
        [FEATURE_SMP]                   = 23,
        [FEATURE_KVM_SIGNATURE]         = 25,
        [FEATURE_EPT]                   = 26,
        [FEATURE_MTRR]                  = 27,
        [FEATURE_VIRTUAL_DR]            = 30,
        [FEATURE_POPCNT]                = 31,
        [FEATURE_KSM]                   = 32,
        [FEATURE_PLE]                   = 33,
        [FEATURE_RDTSCP]                = 34,
        [FEATURE_KVM_CLOCK_NEW]         = 35,
        [FEATURE_ASYNC_PF]        = 38,
        [FEATURE_STEAL_TIME]        = 301,
//        [FEATURE_NESTED_VMX]            = 301,
        [FEATURE_HV_APIC_ASSIST_PAGE]    = 302,
        [FEATURE_PMU_V2]        = 303,
        [FEATURE_PV_EOI_EN]        = 306,
        [FEATURE_X2APIC]                = 309,
        [FEATURE_NESTED_PREEMPTION_TIMER] = 310,
        [FEATURE_NESTED_EPT]            = 312,
        [FEATURE_NESTED_EPT_2MB]        = 313,
        [FEATURE_HV_TIME_REF_COUNT]     = 314,
        [FEATURE_RDSEED_INSTRUCTION]    = 315,
        [FEATURE_ARAT]                  = 402,
        [FEATURE_HV_X64_MSR_CRASH_CTL]  = 403,
        [FEATURE_HV_X64_MSR_RESET]      = 404,
        [FEATURE_HV_X64_MSR_TSC_FREQUENCY] = 414,
        [FEATURE_MSR_IA32_ARCH_CAPABILITIES] = 415,  // 4.15.2
        [FEATURE_UMIP]                  = 416,
        [FEATURE_HV_X64_MSR_REENLIGHTENMENT_CONTROL] = 417,
        [FEATURE_IPI_HYPERCALL]         = 419,
        [FEATURE_MSR_IA32_POWER_CTL_INTEL] = 502,
        [FEATURE_MSR_IA32_POWER_CTL]    = 503,
        [FEATURE_SCHED_YIELD_HYPERCALL] = 503,
        [FEATURE_ASYNC_PF_INT]          = 508,
};

// Non-configurable features are always present if implemented and cannot be deactivated by the user
static int const kvm_features_non_configurable[] = {
        FEATURE_MSR_IA32_POWER_CTL_INTEL,
        FEATURE_MSR_IA32_POWER_CTL,
};

static int const kvm_features_non_configurable_count = ARRAY_SIZE(kvm_features_non_configurable);

static int (*const kvm_bug_testers[])() = {
        [BUG_MSR_IA32_MCG_STATUS]    = test_msr_ia32_mcg_status,
        [BUG_MSR_IA32_EBL_CR_POWERON]    = test_msr_ia32_ebl_cr_poweron,
        [BUG_MSR_IA32_MCG_CTL]        = test_msr_ia32_mcg_ctl,
        [BUG_MSR_IA32_PERF_STATUS]    = test_msr_ia32_perf_status,
        [BUG_MSR_IA32_MC0_MISC5]    = test_msr_ia32_mc0_misc5,
        [BUG_MSR_VM_HSAVE_PA]        = test_msr_vm_hsave_pa,
        [BUG_MSR_K7_EVNTSEL0]        = test_msr_k7_evntsel0,
        [BUG_DR_UNCHECK]        = test_dr_uncheck,
        [BUG_MSR_IA32_MC0_CTL]        = test_msr_ia32_mc0_ctl,
//        [BUG_DR_NULL]			= test_dr_null,
        [BUG_MSR_K7_HWCR_0x100]        = test_msr_k7_hwcr_0x100,
        [BUG_MSR_EBC_FREQUENCY_ID]    = test_msr_ebc_frequency_id,
        [BUG_MSR_IA32_BBL_CR_CTL3]    = test_msr_ia32_bbl_cr_ctl3,
//        [BUG_VNC_CRASH]		= test_vnc_crash,	// Have not verified this.
        [BUG_MSR_IA32_UCODE_REV]    = test_msr_ia32_ucode_rev,
        [BUG_MSR_K7_HWCR_0x8]        = test_msr_k7_hwcr_0x8,
        [BUG_CPUID_SIGNATURE]        = test_cpuid_signature,
        [BUG_MSR_IA32_TSC_ADJUST]    = test_msr_ia32_tsc_adjust,
        [BUG_MSR_AMD64_BU_CFG2]        = test_msr_amd64_bu_cfg2,
//        [BUG_UNUSABLE_PRESENT]		= test_unusable_present,
        [BUG_MSR_IA32_VMX_ENTRY_CTLS]    = test_msr_ia32_vmx_entry_ctls,
        [BUG_MSR_IA32_FEATURE_CONTROL]    = test_msr_ia32_feature_control,
        [BUG_MSR_IA32_APICBASE]        = test_msr_ia32_apicbase,
        [BUG_MSR_K8_TSEG_ADDR] = test_msr_k8_tseg_addr,
        [BUG_MSR_IA32_PERF_CTL] = test_msr_ia32_perf_ctl,
        [BUG_MSR_AMD64_DC_CFG] = test_msr_amd64_dc_cfg,
        [BUG_MSR_IA32_ARCH_CAPABILITIES_AMD] = test_msr_ia32_arch_capabilities_amd,
        [BUG_HV_X64_MSR_TSC_EMULATION_STATUS] = test_hv_x64_msr_tsc_emulation_status,
};

static const int kvm_max_bug_testers =
        ARRAY_SIZE(kvm_bug_testers);

static int const kvm_bug_start[] = {
        [BUG_MSR_IA32_MCG_STATUS]    = 20,
        [BUG_MSR_IA32_EBL_CR_POWERON]    = 20,
        [BUG_MSR_IA32_MCG_CTL]        = 20,
        [BUG_MSR_IA32_PERF_STATUS]    = 20,
        [BUG_MSR_IA32_MC0_MISC5]    = 20,
        [BUG_MSR_VM_HSAVE_PA]        = 20,
        [BUG_MSR_K7_EVNTSEL0]        = 20,
        [BUG_DR_UNCHECK]        = 20,
        [BUG_MSR_IA32_MC0_CTL]        = 20,
//        [BUG_DR_NULL]			= 20,
        [BUG_MSR_K7_HWCR_0x100]        = 20,
        [BUG_MSR_EBC_FREQUENCY_ID]    = 20,
        [BUG_MSR_IA32_BBL_CR_CTL3]    = 20,
//        [BUG_VNC_CRASH]			= 39,
        [BUG_MSR_IA32_UCODE_REV]    = 20,
        [BUG_MSR_K7_HWCR_0x8]        = 20,
        [BUG_CPUID_SIGNATURE]        = 25,
        [BUG_MSR_IA32_TSC_ADJUST]    = 20,
        [BUG_MSR_AMD64_BU_CFG2]        = 20,
//        [BUG_UNUSABLE_PRESENT]		= 20,
        [BUG_MSR_IA32_VMX_ENTRY_CTLS]    = 301,
        [BUG_MSR_IA32_FEATURE_CONTROL]    = 301,
        [BUG_MSR_IA32_APICBASE]        = 32,
        [BUG_MSR_K8_TSEG_ADDR] = 20,
        [BUG_MSR_IA32_PERF_CTL] = 20,
        [BUG_MSR_AMD64_DC_CFG] = 20,
        [BUG_MSR_IA32_ARCH_CAPABILITIES_AMD] = 417,
        [BUG_HV_X64_MSR_TSC_EMULATION_STATUS] = 417,
};

static int const kvm_bug_end[] = {
        [BUG_MSR_IA32_MCG_STATUS]    = 22,
        [BUG_MSR_IA32_EBL_CR_POWERON]    = 23,
        [BUG_MSR_IA32_MCG_CTL]        = 25,
        [BUG_MSR_IA32_PERF_STATUS]    = 26,
        [BUG_MSR_IA32_MC0_MISC5]    = 28,
        [BUG_MSR_VM_HSAVE_PA]        = 30,
        [BUG_MSR_K7_EVNTSEL0]        = 31,
        [BUG_DR_UNCHECK]        = 32,
        [BUG_MSR_IA32_MC0_CTL]        = 34,
//        [BUG_DR_NULL]			= 35,
        [BUG_MSR_K7_HWCR_0x100]        = 35,
        [BUG_MSR_EBC_FREQUENCY_ID]    = 37,
        [BUG_MSR_IA32_BBL_CR_CTL3]    = 39,
//        [BUG_VNC_CRASH]			= 300,
        [BUG_MSR_IA32_UCODE_REV]    = 302,
        [BUG_MSR_K7_HWCR_0x8]        = 304,
        [BUG_CPUID_SIGNATURE]        = 305,
        [BUG_MSR_IA32_TSC_ADJUST]    = 308,
        [BUG_MSR_AMD64_BU_CFG2]        = 309,
//        [BUG_UNUSABLE_PRESENT]		= 311,
        [BUG_MSR_IA32_VMX_ENTRY_CTLS]    = 310,
        [BUG_MSR_IA32_FEATURE_CONTROL]    = 312,
        [BUG_MSR_IA32_APICBASE]        = 314,
        [BUG_MSR_K8_TSEG_ADDR] = 403,  // 4.2.4
        [BUG_MSR_IA32_PERF_CTL] = 407,
        [BUG_MSR_AMD64_DC_CFG] = 412,
        [BUG_MSR_IA32_ARCH_CAPABILITIES_AMD] = 501,
        [BUG_HV_X64_MSR_TSC_EMULATION_STATUS] = 513,
};

int main() {
    int ifeature;
    int ibug;

    int vmax = VERSION_MAX;
    int vmin = VERSION_MIN;
    int vmax_print;

    DPRINTF("project kick-off!\n");
    int i = 0;

    DPRINTF("===Starting Feature Test!===\n");
    for (i = kvm_max_feature_testers; i > 0; i--) {
        DPRINTF("==============================\n");
        int current_feature = i - 1;
        ifeature = kvm_feature_testers[current_feature]();
        DPRINTF("ifeature is %d\n", ifeature);

        if (ifeature == 1) {
            DPRINTF("ifeature is 1 while i=%d\n", i);

            int version = kvm_feature_start[current_feature];

            // only update vmin if it actually narrows down the possible versions
            if (version > vmin) vmin = version;

            break;
        } else if(ifeature == 0) {
            // If a non-configurable feature is checked and not detected, it is not implemented.
            // From that, we can assume the maximal hypervisor version.
            //
            // Check if array of non-configurable features contains the currently checked feature
            for(int j = 0; j < kvm_features_non_configurable_count; j++) {
                if(kvm_features_non_configurable[j] == current_feature) {
                    DPRINTF("Non-configurable feature is not present, assuming max version\n");
                    vmax = kvm_feature_start[current_feature] - 1;
                }
            }

            // Special case for synthetic Hyper-V MSRs:
            // Since 5.12, Hyper-V MSRs cannot be read/written unless Hyper-V Enlightenments are activated in KVM,
            // decreasing the chance to detect such MSRs. Therefore, we cannot treat those features as non-configurable
            // in general because they could be implemented, although not detected.
            // However, if we know that we are running on KVM < 5.12, that restriction is not present.
            // In that case, we can treat features based on Hyper-V MSRs as non-configurable.
            if(vmax < 512 && (current_feature == FEATURE_HV_X64_MSR_REENLIGHTENMENT_CONTROL ||
                               current_feature == FEATURE_HV_X64_MSR_TSC_FREQUENCY ||
                               current_feature == FEATURE_HV_X64_MSR_RESET ||
                               current_feature == FEATURE_HV_X64_MSR_CRASH_CTL)){
                DPRINTF("Non-configurable feature based on Hyper-V MSR is not present while vmax < 5.12, assuming max version\n");
                vmax = kvm_feature_start[current_feature] - 1;
            }
        }
    }

    DPRINTF("===Starting Bug Test!===\n");
    for (i = 0; i < kvm_max_bug_testers; i++) {
        DPRINTF("==============================\n");
        ibug = kvm_bug_testers[i]();

        if (ibug == 1) {
            int version = kvm_bug_end[i] - 1;
            if (version < vmax) vmax = version;
//			break;
        }
    }

    if (vmax < 40 || vmax < vmin)
        printf("\n\nSomething must be wrong, since vmin=%d is greater than vmax=%d.\n", vmin, vmax);

    if (vmin < 40) {
        // it's a really old version (2.6.11 - 3.20)
        if (vmax < 40)
            printf("\n\nThe underlying hypervisor kernel version should be between 2.6.%d and 2.6.%d.\n", vmin, vmax);
        else {
            vmax_print = vmax - 300;
            printf("\n\nThe underlying hypervisor kernel version should be between 2.6.%d and 3.%d.\n", vmin,
                   vmax_print);
        }
    // version 4.1 or above
    } else if (vmin != vmax) {
        double vmin_print = (double)vmin / 100;
        double vmax_print = (double)vmax / 100;
        printf("\n\nThe underlying hypervisor kernel version should be between %.2f and %.2f.\n", vmin_print, vmax_print);
    } else if (vmin == vmax) { // check for equality just to be sure...
        double version = (double)vmin / 100;
        printf("\n\nThe underlying hypervisor kernel version should be %.2f.\n", version);
    }

    if(vmax == VERSION_MAX){
        printf("\nHint: The detected maximum version of the underlying hypervisor kernel equals the maximum supported "
               "version by Hyperprobe. The actual hypervisor kernel version may be higher.\n");
    }

    return 0;
}

