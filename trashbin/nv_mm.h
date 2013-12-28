/*
 * =====================================================================================
 *
 *       Filename:  nv_mm.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  11/07/2013 10:47:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Syi,
 *   Organization:  CS@WM
 *
 * =====================================================================================
 */



#ifndef  nv_mm_INC
#define  nv_mm_INC

#include "global.h"


//NVRootmapItem_t * nvmm_rootmap_init(NVRDescr * nvrAddr); // called in the new nvregion
void nvmm_dataregion_init(NVRDescr *nvrAddr); // everytime open a new nvregion
void nvmm_dataregion_update(NVRDescr *nvrAddr);  // everytime close a nvregion.
void nvmm_dataregion_deinit(NVRDescr * nvrAddr); // memset maybe
void *nvmm_sbrk(int incr);
void nvmm_reset_brk(void);
void *nvmm_dataregion_lo(void);
void *nvmm_dataregion_hi(void);

size_t nvmm_dataregion_size(void); // kinda a heap
size_t nvmm_pagesize(void);
#endif   /* ----- #ifndef nv_mm_INC  ----- */

/*

void mem_init(void);               
void mem_deinit(void);
void *mem_sbrk(int incr);
void mem_reset_brk(void); 
void *mem_heap_lo(void);
void *mem_heap_hi(void);
size_t mem_heapsize(void);
size_t mem_pagesize(void);

*/
