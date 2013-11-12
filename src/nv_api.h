/*
 * =====================================================================================
 *
 *       Filename:  nv_api.h
 *
 *    Description:  header file
 *
 *        Version:  1.0
 *        Created:  10/31/2013 06:15:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Syi
 *   Organization:  CS@WM
 *
 * =====================================================================================
 */

#ifndef  nv_api_INC
#define  nv_api_INC

#include "global.h"


//  NVRegion APIs
NVRDescr* NVOpenRegion(char * name, void *startingAddr, int size);
int NVDeleteRegion(char * name);
int NVCloseRegion(NVRDescr * addr);
void *NVFetchRoot(NVRDescr * addr, char *name);
void *NVMalloc(NVRDescr * addr, int size);
void *NVMallocNaive(NVRDescr * addr, int size);
int NVNewRoot(NVRDescr * addr, void * p, char * name,size_t size);
int NVFree(void * addr);
void NVRDescrDump(NVRDescr *nvrAddr);

#endif   /* ----- #ifndef nv_api_INC  ----- */
