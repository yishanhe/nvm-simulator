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


#define SHARE 0
#define EXCLUSIVE 1
#define MAXREGION 32  //define the maximum number of regions
#define DEBUG

#define NV_MAXPATH  128// maximum file name length
#define NVROOT_MAXPATH  128// maximum file name length
#define offset2addr(base,off) ((void *)((base)+(off)))
#define addr2offset(base,addr) ((void *)((addr)-(base)))
#define e(msg) {perror(msg);exit(EXIT_FAILURE);}


#define SHM_MODE 0660 // user and same group user read and write
#define SHM_SIZE 100000

#ifdef  DEBUG
    #define DEBUG_OUTPUT(string) (printf("[DEBUG] %s\n",string))
#else      /* -----  not DEBUG  ----- */
    #define DEBUG_OUTPUT(string)
#endif     /* -----  not DEBUG  ----- */

typedef long NVKey_t;

typedef struct NVRDescr_s {
    long size;
    NVKey_t refKey;
    long rootMapOffset;
    int shareFlag;
    int processCnt;
    int nvRootCnt;
    int ID;
    int nameLen;
    char name[NV_MAXPATH];
    // extra meta data for implementation
} NVRDescr,NVRDescr_t;

typedef struct NVRootmapItem_s {
    long location;
    char type;
    char name[NVROOT_MAXPATH];
} NVRootmapItem_t;

extern int errno;
#endif
