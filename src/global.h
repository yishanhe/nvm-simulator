/*
 * =====================================================================================
 *
 *       Filename:  global.h
 *
 *    Description:  define global var
 *
 *        Version:  1.0
 *        Created:  10/31/2013 10:26:18 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Syi
 *   Organization:  CS@WM
 *
 * =====================================================================================
 */

#ifndef  global_h_INC
#define global_h_INC

#include <stddef.h>

/*-----------------------------------------------------------------------------
 *  Global define parameters
 *-----------------------------------------------------------------------------*/
#define SHM
//#define MMAP
#define FIXMEMBASE
#define DEBUG



/*-----------------------------------------------------------------------------
 *  NVRegion
 *-----------------------------------------------------------------------------*/
#define SHARE 0
#define EXCLUSIVE 1
#define MAXREGION 32  //define the maximum number of regions
#define NV_MAXPATH  128// maximum file name length
#define NVROOT_MAXPATH  32// maximum file name length
#define SHM_MODE 0660 // user and same group user read and write
#define SHM_SIZE 100000 // (20*(1<<20)) //20MB
#define offset2addr(base,off) ((void *)((void *)(base)+(long)(off)))
#define addr2offset(base,addr) ((long)((void *)(addr)-(void *)(base)))
#define PAGESIZE 4096 //4k

#define membase (void *)0x80000000
//#define membase 0x1000000000L

typedef long NVKey_t;

typedef struct NVRDescr_s {
    long size;
    NVKey_t refKey;
    long rootMapOffset;
    long dataRegionOffset; // brk
    int shareFlag;
    int processCnt;
    int nvRootCnt;
    int ID;
    int nameLen;
    char name[NV_MAXPATH];
    // extra meta data for implementation
} NVRDescr,NVRDescr_t;

typedef struct NVRootmapItem_s {
    void * location;
    // design tag bit
    size_t type;
    char name[NVROOT_MAXPATH];
} NVRootmapItem_t;


/*-----------------------------------------------------------------------------
 *  DEBUG
 *-----------------------------------------------------------------------------*/
#define e(msg) {perror(msg);exit(EXIT_FAILURE);}

#ifdef  DEBUG
    #define DEBUG_OUTPUT(string) (printf("[DEBUG] %s\n",string))
#else      /* -----  not DEBUG  ----- */
    #define DEBUG_OUTPUT(string)
#endif     /* -----  not DEBUG  ----- */


extern int errno;







/*-----------------------------------------------------------------------------
 *  NVRegionMeM
 *-----------------------------------------------------------------------------*/
#define ALIGNMENT 8
// you can define MAX_HEAP here, according to the SHM_SIZE
// #define MAX_HEAP (20*(1<<20)) //20MB

#endif
