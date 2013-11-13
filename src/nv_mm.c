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
#include <errno.h>
#include "global.h"
#include "nv_mm.h"


#define ALIGNMENT 8
#define ALIGN(size) (((size)+(ALIGNMENT-1))& ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

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
    nvmm_brk = nvmm_start_brk;
    return nvrmPtr;
}

// run this if rootmap count is bigger than 0
void nvmm_dataregion_init(NVRDescr *nvrAddr) {
	nvmm_max_addr = offset2addr(nvrAddr,nvrAddr->rootMapOffset);
    nvmm_start_brk = offset2addr(nvrAddr,sizeof(NVRDescr));// start address of dataregion
    nvmm_brk = offset2addr(nvrAddr, nvrAddr->dataRegionOffset);
}

void nvmm_dataregion_update(NVRDescr *nvrAddr) {
	nvrAddr->rootMapOffset = addr2offset(membase,nvmm_max_addr);
	nvrAddr->dataRegionOffset = addr2offset(membase,nvmm_brk);
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
