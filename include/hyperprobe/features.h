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
#define FEATURE_EPT		2
#define FEATURE_NESTED		3
#define FEATURE_KSM		4
#define FEATURE_NESTED_EPT 	5

extern int test_kvm_magic();
extern int test_smp();
extern int test_ept();
extern int test_nested();
extern int test_ksm();
extern int test_nested_ept();

#endif /* features.h */
