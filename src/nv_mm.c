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


static char *nvmm_start_brk;
static char *nvmm_brk;
static char *nvmm_max_addr;

// run this if rootmap count is 0
NVRootmapItem_t * nv_rootmap_init(NVRDescr * nvrAddr){
    NVRootmapItem_t * nvrmPtr;
    nvrmPtr = offset2addr(nvrAddr,nvrAddr->rootMapOffset);
//    memset();
    nvmm_max_addr = offset2addr(nvrAddr,nvrAddr->size);
    nvmm_start_brk = offset2addr(nvrAddr,sizeof(NVRDescr));// start address of dataregion
    nvmm_brk = nvmm_start;
    return nvrmPtr;
}

void nv_dataregion_init(NVRDescr *nvrAddr) {

}

void nv_dataregion_deinit(NVRDescr * nvrAddr){
    // do nothing
    // update related data structure
}

void *nvmm_sbrk(int incr){
    char *old_brk = nvmm_brk;

    if ((incr<0)||((nvmm_brk+incr)>nvmm_max_addr)) {
        errno = ENOMEM;
        e("nvmm_brk fail");
    }
    // success
    return (void *) old_brk;
}

void nvmm_reset_brk(){
    nvmm_brk = nvmm_start_brk;
}


size_t nv_dataregion_size(void) {
    return (size_t) (nvmm_brk-nvmm_start_brk);
}
size_t nv_pagesize(void) {
    return (size_t) getpagesize();
}

void *nv_dataregion_lo() {
    return (void *) nvmm_start_brk;
}

void *nv_dataregion_hi() {
    return (void *) (nvmm_brk -1);
}
