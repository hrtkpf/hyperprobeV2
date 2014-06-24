/*
 * This file implements a test case for check ept/npt and shadow page table.
 * Initial work by:
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

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"

/* define constant numbers */

#define PAGESIZE 4096
#define PROCESS_NUM 40000
#define LOOP_NUMBER 3
#define EPT_THRESHOLD 150000

/* global variables */

double fork_wait_one_process_avg = 0.0;
double access_one_byte_avg = 0.0;

uint64_t rdtsc()
{
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}


/* calculate average page access time */
void access_one_byte (long iterations)
{
    unsigned long long int total = 0;
    unsigned long long int counter1 = 0;
    unsigned long long int counter2 = 0;
    long i = 0;
    int offset = 0;

    /* allocate one page memory */
    char* page = (char*) malloc(PAGESIZE * sizeof(char));
    if (page != NULL){
        memset(page, 0, PAGESIZE);
    }

    counter1 = rdtsc();
    for (i=0; i<iterations; i++){
        offset = (offset + 1) % PAGESIZE;
        page[offset] = ( page[offset] + 1 ) % 256;
    }
    counter2 = rdtsc();

    total = counter2 - counter1;
    access_one_byte_avg =  (double)total / (double)iterations;

    /* release page*/
    free(page);
}

/* calculate average process fork wait time */
void fork_wait_one_process(){
    int i;
    unsigned long long int cycle1;
    unsigned long long int cycle2;
    unsigned long long int total;
    pid_t pid;
    int status;

    cycle1 = rdtsc();
    for (i=0; i<PROCESS_NUM; i++){
        pid = fork();
        if (pid < 0) {
            exit(0);
        }
        if (pid == 0) {
            exit(0);
        }
        wait(&status);
    }
    cycle2 = rdtsc();

    total = cycle2 - cycle1;
    fork_wait_one_process_avg = (double)total / (double)PROCESS_NUM;
}

int one_time_result()
{
    long iterations = 2000000000;
    printf("Detecting ... \n\n");

    access_one_byte (iterations);
    printf("Accessing one byte...  \navg. time = %f\n\n", access_one_byte_avg );

    fork_wait_one_process();
    printf("Fork and wait one process...  \navg. time = %f\n\n", fork_wait_one_process_avg );

    double ratio = fork_wait_one_process_avg / access_one_byte_avg;
    printf("Ratio = %f\n\n", ratio);

    if (ratio < EPT_THRESHOLD)
        return 1;
    else
        return 0;
}

int test_ept(){
    int i = 0;
    for (i=0; i<LOOP_NUMBER; i++){
        if (one_time_result())
            return 1;
    }
    return 0;
}
