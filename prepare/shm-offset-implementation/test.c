/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  11/06/2013 01:19:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "global.h"
#include <stdio.h>

#define __LowTag(p)  (*((int *)(p)-1))
#define __HiPreTag(p)  (*((int *)(p)-2))

int NVNewRoot(void *p) {
    printf("sizeof p %d\n", sizeof(p));
     printf("sizeof name in func %d\n", sizeof(((NVRDescr *)p)->name));
}

int main(int argc, const char *argv[])
{
    NVRDescr_t * t1 = (NVRDescr *)malloc(sizeof(NVRDescr));
    NVRDescr_t * t2 = (NVRDescr *)malloc(sizeof(NVRDescr));
    NVRDescr_t * t = (NVRDescr *)malloc(sizeof(NVRDescr));
    printf("sizeof nvrdescr %d\n", sizeof(NVRDescr));
    printf("sizeof name %d\n", sizeof(t->name));
    printf("addr %lx\n", t);
    t++;
    printf("addr %lx\n", t);
    NVNewRoot(t2);
    printf("sizeof name %d\n", sizeof(t1->name));
    printf("addr %p\n", t1);
    printf("addr %p\n", t2);
    char * t3="123123";
    printf("addr %p\n", t3);
    int a =100;
    printf("addr %p\n", (void *)((void *)t2+a));
   printf("addr %p\n", t3);
    printf("low tag addr is %lx\n",__LowTag(t3));
    printf("low tag addr is %lx\n",__LowTag(t3)&0xFFFFFFF8);
    printf("low tag addr is %p\n",__LowTag(t3)&0xFFFFFFF8);


    printf("low tag addr is %lx\n",(*((int *)(t3))));
    printf("low tag addr is %lx\n",(*((int *)(t3)-1)));
    printf("low tag addr is %lx\n",(*((int *)(t3)-2)));
    return 0;
}

