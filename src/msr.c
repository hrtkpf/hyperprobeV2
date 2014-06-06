/*
 * msr.c: this file implements msr read and write operations.
 * We borrowed the code from msr-tools.
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

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#include "hyperprobe/msr.h"

uint64_t rdmsr_on_cpu(uint32_t reg, int cpu)
{
        uint64_t data;
        int fd;
        char msr_file_name[64];

        sprintf(msr_file_name, "/dev/cpu/%d/msr", cpu);
        fd = open(msr_file_name, O_RDONLY);
        if (fd < 0) {
                if (errno == ENXIO) {
                        fprintf(stderr, "rdmsr: No CPU %d\n", cpu);
                        exit(2);
                } else if (errno == EIO) {
                        fprintf(stderr, "rdmsr: CPU %d doesn't support MSRs\n",
                                cpu);
                        exit(3);
                } else {
                        perror("rdmsr: open");
                        exit(127);
                }
        }

        if (pread(fd, &data, sizeof data, reg) != sizeof data) {
                if (errno == EIO) {
                        fprintf(stderr, "rdmsr: CPU %d cannot read "
                                "MSR 0x%08"PRIx32"\n",
                                cpu, reg);
                        exit(4);
                } else {
                        perror("rdmsr: pread");
                        exit(127);
                }
        }

        close(fd);

        return data;
}

void wrmsr_on_cpu(uint32_t reg, int cpu, uint64_t data)
{
        int fd;
        char msr_file_name[64];

        sprintf(msr_file_name, "/dev/cpu/%d/msr", cpu);
        fd = open(msr_file_name, O_WRONLY);
        if (fd < 0) {
                if (errno == ENXIO) {
                        fprintf(stderr, "wrmsr: No CPU %d\n", cpu);
                        exit(2);
                } else if (errno == EIO) {
                        fprintf(stderr, "wrmsr: CPU %d doesn't support MSRs\n",
                                cpu);
                        exit(3);
                } else {
                        perror("wrmsr: open");
                        exit(127);
                }
        }

	if (pwrite(fd, &data, sizeof data, reg) != sizeof data) {
		if (errno == EIO) {
			fprintf(stderr,
				"wrmsr: CPU %d cannot set MSR "
				"0x%08"PRIx32" to 0x%016"PRIx64"\n",
				cpu, reg, data);
			exit(4);
		} else {
			perror("wrmsr: pwrite");
			exit(127);
		}
	}

        close(fd);

        return;
}

