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

int NVNewRoot(void *p) {
    printf("sizeof p %d\n", sizeof(p));
     printf("sizeof name in func %d\n", sizeof(((NVRDescr *)p)->name));
}

int main(int argc, const char *argv[])
{
    NVRDescr_t * t1 = (NVRDescr *)malloc(sizeof(NVRDescr));
    NVRDescr_t * t2 = (NVRDescr *)malloc(sizeof(NVRDescr));
    printf("sizeof nvrdescr %d\n", sizeof(NVRDescr));
    NVNewRoot(t2);
    printf("sizeof name %d\n", sizeof(t1->name));
    printf("addr %p\n", t1);
    printf("addr %p\n", t2);
    char * t3="123123";
    printf("addr %p\n", t3);
    int a =100;
    printf("addr %p\n", (void *)((void *)t2+a));

    return 0;
}

