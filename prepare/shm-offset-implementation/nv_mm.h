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

//typedef struct NVDataRegionMM_s {
//	// offset
//    long nv_mm_start_brk;
//    long nv_mm_brk;
//    long nv_mm_max_addr;
//} NVDRmm_t;

typedef struct nv_mm_chunk_s {
	// offset
    long preCOffset;
    long currCOffset;
    long nextCOffset;
} nv_mm_chunk_t;

// design tag bit


NVRootmapItem_t * nv_dataregion_init(NVRDescr * nvrAddr);
void nv_dataregion_deinit(NVRDescr * nvrAddr); // memset maybe
void *nv_mm_sbrk(int incr);
void nv_mm_reset_brk(void);
void *nv_dataregion_lo(void);
void *nv_dataregion_hi(void);

size_t nv_dataregion_size(void); // kinda a heap
size_t nv_pagesize(void);
#endif   /* ----- #ifndef nv_mm_INC  ----- */