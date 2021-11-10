/*
 * virtual_dr.c: this test case is a check against the feature: debug register virtualization.
 * This feature is only supported since kernel 2.6.30.
 * Before kernel 2.6.30, debug inside guest is not possible.
 * e.g., write to debug register would be ignored.
 * Since kernel 2.6.30, write to debug registers would not be ignore, as least some bits will be kept.
 * Therefore, to test this bit, we write a predefined value into debug register 6: dr6
 * And read it back, in kernel newer than 2.6.30, the kept bits should be the same as what we have written.
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
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"

static unsigned long get_dr6(void) {
    unsigned long value;

    asm volatile("mov %%dr6,%0" : "=r" (value));
    return value;
}

static void set_dr6(unsigned long value) {
    asm volatile("mov %0,%%dr6" : : "r" (value));
}


// This function forks a child process, the child process attempts to write and then read dr register 6, 
// if some bits are kept, we assume the feature is exists, otherwise, we think the feature might not be existing.
// Return: 1 if feature exist, 0 if feature not exist.
#define WRITE_TO_DR    0x123
#define DR6_FIXED_1    0xffff0ff0
#define DR6_VOLATILE    0x0000e00f
unsigned long dr;

int test_virtual_dr() {
    return 0;
    pid_t pid;
    int status;

    if ((pid = vfork()) < 0) {
        perror("fail to fork\n");
    }

    if (pid == 0)    //child process
    {
        DPRINTF("DEBUG: Child: %s %d \n", __FUNCTION__, __LINE__);
        set_dr6(WRITE_TO_DR);
        dr = get_dr6();
        DPRINTF("DEBUG: Child: DR register is accessible by ring3 program!\n");
        exit(0);
    } else        //parent process
    {
        wait(&status);
        if (dr == ((WRITE_TO_DR & DR6_VOLATILE) | DR6_FIXED_1)) {
            DPRINTF("DEBUG: Parent: Feature Exists: we can read from and write to DR correctly.\n");
            return 1;    //child process exit normally, which means dr register is accessible, so the bug is existing.
        } else {
            DPRINTF("DEBUG: Parent: We are not sure, either DR is not readable/writable, or the feature is not supported.\n");
            return 0; //child process is terminated by a segmentation fault signal, which means the bug does not exist.
        }
//		printf("DEBUG: Parent: %s %d \n",__FUNCTION__,__LINE__);
    }
    return 0;
}
