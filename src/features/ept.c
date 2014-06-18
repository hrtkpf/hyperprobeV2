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

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"

/* define constant numbers */
#define PAGESIZE 4096
#define NUM_OF_PAGES 1024*16
#define SEED 123456789
#define STEP 33
#define EPT_THRESHOLD 800
#define LOOP_NUMBER 3
/* global variables */
unsigned long long int alloc_one_page_avg = 0;
unsigned long long int access_one_byte_avg = 0;

uint64_t rdtsc()
{
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

/* allocate one page and initialize it to 0 */
char* alloc_one_page()
{
    char* page = (char*) malloc(PAGESIZE * sizeof(char));
    if (page != NULL){
        memset(page, 0, PAGESIZE);
    }
    return page;
}

/* allocate a number of pages */
char** alloc_all_pages(long num_of_pages)
{
    unsigned long long int cycles_before = 0;
    unsigned long long int cycles_after = 0;
    unsigned long long int total = 0;
    long i = 0;

    char **pages = (char**) malloc (num_of_pages * sizeof(char*));

    cycles_before = rdtsc();
    for (i=0; i<num_of_pages; i++){
        pages[i] = alloc_one_page();
    }
    cycles_after = rdtsc();

    total += cycles_after - cycles_before;
    alloc_one_page_avg = total/num_of_pages;
    return pages;
}

/* release all allocated pages */
void release_all_pages (char** pages, long num_of_pages)
{
    long i = 0;
    for (i=0; i<num_of_pages; i++){
        free(pages[i]);
    }
    free(pages);
}

/* calculate average page access time */
void access_one_page (char ** pages, long num_of_pages, unsigned long long int iterations)
{
    unsigned long long int total = 0;
    unsigned long long int cycles_before = 0;
    unsigned long long int cycles_after = 0;
    unsigned long long int i = 0;
    int offset = 0;
    cycles_before = rdtsc();
    for (i=0; i<iterations; i++){
        offset = (offset + STEP) % PAGESIZE;
        pages[0][offset] = ( pages[0][offset] + 1 ) % 256;
    }
    cycles_after = rdtsc();
    total += cycles_after - cycles_before;
    access_one_byte_avg =  total/iterations;
}


int one_time_result()
{
    long iterations = 2000000;
    DPRINTF("Detecting ... \n");

    char** p = alloc_all_pages(NUM_OF_PAGES);
    DPRINTF("Allocating one page...  \navg. time = %lld\n", alloc_one_page_avg );

    access_one_page (p, NUM_OF_PAGES, PAGESIZE*iterations);
    DPRINTF("Allocating one byte...  \navg. time = %lld\n", access_one_byte_avg );
    
    release_all_pages(p, NUM_OF_PAGES);

    double ratio = (double)alloc_one_page_avg / (double) access_one_byte_avg;
    if (ratio < EPT_THRESHOLD)
	return 1;
    else
        return 0;
}

int test_ept(){
    int i = 0;
    int result = 0;
    for (i=0; i<LOOP_NUMBER; i++){
        result = one_time_result();
        if (result == 1){
            break;
        }
    }
    return result;
}

