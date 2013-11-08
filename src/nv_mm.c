/*
 * =====================================================================================
 *
 *       Filename:  nv_mm.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  11/07/2013 01:21:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Syi,
 *   Organization:  CS@WM
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "global.h"
#include "nv_mm.h"


static char *nv_mm_start_brk;
static char *nv_mm_brk;
static char *nv_mm_max_addr;


NVRootmapItem_t * nv_dataregion_init(NVRDescr * nvrAddr){
    NVRootmapItem_t * nvrmPtr;
    nvrmPtr = nvrAddr+nvrAddr->rootMapOffset;
//    nvrmPtr = offset2addr(nvrAddr,nvAddr->rootMapOffset);
//    memset();
    nv_mm_max_addr = nvrAddr+nvrAddr->size;
    nv_mm_start = nvrAddr+sizeof(NVRDescr);
    nv_mm_brk = nv_mm_start;
    return nvrmPtr;
}

void nv_dataregion_deinit(NVRDescr * nvrAddr)

void *nv_mm_sbrk(int incr){
    char *old_brk = nv_mm_brk;

    if ((inc<0)||((nv_mm_brk+inc)>nv_mm_max_addr)) {
        errno = ENOMEM;
        e("nv_mm_brk fail");
    }
    // success
}

void nv_mm_reset_brk(){
    nv_mm_brk = nv_mm_start_brk;
}


size_t nv_dataregion_size(void) {

}
size_t nv_pagesize(void) {
    return (size_t) getpagesize();
}
