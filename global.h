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


#ifdef  DEBUG
    #define DEBUG_OUTPUT(string) (printf("[DEBUG] %s\n",string))
#else      /* -----  not DEBUG  ----- */
    #define DEBUG_OUTPUT(string)
#endif     /* -----  not DEBUG  ----- */

typedef long NVKey_t;

typedef struct {
    long size; // the size of NVRegion
    int shareFlag;
    NVKey_t RefKey;
    int processCnt;
    int NVRootCnt;
    long rootMapOffset;
    void * rootMap;
    int ID;
    int nameLen;
    char* name;
    // extra meta data for implementation
    //
} NVRDescr;

#endif
