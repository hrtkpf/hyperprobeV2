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

#define BUG_MSR_IA32_MCG_STATUS		0
#define BUG_MSR_IA32_EBL_CR_POWERON	1
#define BUG_MSR_IA32_MCG_CTL		2
#define BUG_MSR_IA32_PERF_STATUS	3
#define BUG_MSR_IA32_MC0_MISC5		4
#define BUG_MSR_VM_HSAVE_PA		5
#define BUG_MSR_K7_EVNTSEL0		6
#define BUG_DR_UNCHECK			7
//#define BUG_DR_NULL			8
#define BUG_MSR_IA32_UCODE_REV		8
#define BUG_MSR_IA32_TSC_ADJUST		9
#define BUG_VNC_CRASH			10
#define BUG_UNUSABLE_PRESENT 		11

extern int test_msr_ia32_mcg_status();
extern int test_msr_ia32_ebl_cr_poweron();
extern int test_msr_ia32_mcg_ctl();
extern int test_msr_ia32_perf_status();
extern int test_msr_ia32_mc0_misc5();
extern int test_msr_vm_hsave_pa();
extern int test_msr_k7_evntsel0();
extern int test_dr_uncheck();
//extern int test_dr_null();
extern int test_msr_ia32_ucode_rev();
extern int test_msr_ia32_tsc_adjust();
extern int test_vnc_crash();
extern int test_unusable_present();

#endif /* bugs.h */
