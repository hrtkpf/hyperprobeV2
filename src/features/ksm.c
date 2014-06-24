/*
 * This file implements a test case for check ksm feature.
 * Initial work by:
 *   (c) 2014 Jidong Xiao (jidong.xiao@gmail.com)
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
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include "hyperprobe/features.h"
#include "hyperprobe/debug.h"

#define KSM_THRESHOLD 2
#define KSM_SLEEP 180

struct timespec diff(struct timespec start, struct timespec end)
{
     struct timespec temp;
     if ((end.tv_nsec-start.tv_nsec)<0) {
         temp.tv_sec = end.tv_sec-start.tv_sec-1;
         temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
     } else {
         temp.tv_sec = end.tv_sec-start.tv_sec;
         temp.tv_nsec = end.tv_nsec-start.tv_nsec;
     }
     return temp;
}

int ae_load_file_to_memory(const char *filename, char **result)
{
        int size = 0;
        FILE *f = fopen(filename, "rb");
        if (f == NULL)
        {
                *result = NULL;
                return -1; // -1 means file opening fail
        }
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fseek(f, 0, SEEK_SET);
        *result = (char *)malloc(size+1);
        if (size != fread(*result, sizeof(char), size, f))
        {
                free(*result);
                return -2; // -2 means file reading fail
        }
        fclose(f);
        (*result)[size] = 0;
        return size;
}

int test_ksm()
{
        int pages;
        int size1,size2;
        char *content0,*content1,*content2;
        double t1,t2;
        struct timespec time1, time2;

        /*Load file once and record the write access time*/
        size1 = ae_load_file_to_memory("httpd", &content0);
        if (size1 < 0)
        {
                DPRINTF("Error loading file\n");
                free(content0);
                return 0;
        }
        pages=size1/4096;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
        do{
                content0[pages*4096-1]='b';
                pages--;
        }
        while(pages > 0);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
        t1=diff(time1,time2).tv_nsec;
        printf("t1=%f\n",t1);

        /*Load file twice and record the write access time*/
        size1 = ae_load_file_to_memory("httpd", &content1);
        if (size1 < 0)
        {
                DPRINTF("Error loading file\n");
                free(content1);
                return 0;
        }
        size2 = ae_load_file_to_memory("httpd", &content2);
        if (size2 < 0)
        {
                DPRINTF("Error loading file\n");
                free(content2);
                return 0;
        }
        pages=size2/4096;
        sleep(KSM_SLEEP);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
        do{
                content2[pages*4096-1]='b';
                pages--;
        }
        while(pages > 0);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
        t2=diff(time1,time2).tv_nsec;
        DPRINTF("t2=%f\n",t2);
        DPRINTF("t1*5=%f\n",t1*5);
        free(content0);
        free(content1);
        free(content2);

	/* Normally t2 should be as 10 times large as to t1, we use a relative conservative threshold. */
        if(t2> (t1 * KSM_THRESHOLD))
        {
                DPRINTF("ksm is existing\n");
		return 1;
        }else
        {
                DPRINTF("ksm is not existing\n");
		return 0;
        }
}

