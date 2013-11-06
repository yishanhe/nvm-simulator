/*
 * =====================================================================================
 *
 *       Filename:  nv_api.c
 *
 *    Description:  provide api for nvregion operation
 *
 *        Version:  1.0
 *        Created:  10/31/2013 06:14:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Syi
 *   Organization:  CS@WM
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#include "nv_api.h"

extern int errno;

/*-----------------------------------------------------------------------------
 *
 *  this is reasonable since if we are using a share memory as nvregion,
 *  we can return this address and actually we will put the NVRDescr structure
 *  at this position.  the offset + base address if we put all nvregion in a big
 *  chunk of shared memory. So let's do  nvregion=shm first.
 *-----------------------------------------------------------------------------*/

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  NVOpenRegion
 *  Description:
 * =====================================================================================
 */
NVRDescr * NVOpenRegion(char * name,            /* region name */
                        void * startingAddr,    /* starting addr */
                        int size)               /* region size. The real size is rounded up */
{
 /* :TODO:11/05/2013 10:48:43 AM:: implement hash(name)->key, not rely on the file index any more */
 /* :WARNING:11/05/2013 03:36:26 PM:: the size maybe different  */
    int nameLen;
    // check name lenght
    if ((nameLen=strlen(name))>NV_MAXPATH){
        printf("NVOpenRegion name length is longer than NV_MAXPATH\n");
        exit(1);
    }

    key_t keyNVRegion;
    int shmId;
    struct shmid_ds shmDsBuf,* shmDsPtr;
    shmDsPtr = &shmDsBuf;
    int flag = 0;
    NVRDescr *nvrAddr;
    char *shmPtr;

    if ((keyNVRegion=ftok(name, 0))<0) {
 /* :TODO:11/05/2013 10:52:10 AM:: reminder of creating backing file */
        e("NVOpenRegion ftok error");
    }
    // get shmId, if not exist create
    if ((shmId=shmget(keyNVRegion,size,SHM_MODE))<=0) {
        if (errno==ENOENT){// this region is not exists, create a new one
            if ((shmId=shmget(keyNVRegion,size,IPC_CREAT|SHM_MODE))<=0){
                e("NVOpenRegion shmget error");
            }
            // successfully create this shm
            flag = 1;
            // get stat here
            if ((shmctl(shmId,IPC_STAT,&shmDsBuf))<0) {
                e("NVOpenRegion shmctl error");
            }
        }else
            e("NVOpenRegion shmget error");
    }

    // shmat
    if ((shmPtr = shmat(shmId,startingAddr,0) )==(void *)-1) {
        e("NVOpenRegion shmat error");
    }

    nvrAddr = (NVRDescr *)shmPtr;

/* :TODO:11/05/2013 03:24:08 PM:: warp between shmctl and NVRDescr */
    if (flag==1) {// initiate the meta data of this region
        nvrAddr->size = size;
        nvrAddr->refKey = keyNVRegion;
        nvrAddr->rootMapOffset =  size; // the end of region, should be a relative address
        nvrAddr->shareFlag = SHARE;
        nvrAddr->processCnt = shmDsPtr->shm_nattch; // 1 is the initial value
        nvrAddr->nvRootCnt = 0; // 0 is the initial value
        nvrAddr->ID = shmId;
        memset(nvrAddr->name, '\0', sizeof(nvrAddr->name));
        strcpy(nvrAddr->name,name);
        nvrAddr->nameLen = nameLen;
/* :TODO:11/06/2013 12:53:07 PM:: initiate the rootmap */
    } else {
        // update NVRDescr
        nvrAddr->processCnt = shmDsPtr->shm_nattch; // 1 is the initial value
    }

    return nvrAddr;
}



void NVRDescrDump(NVRDescr *nvrAddr){
    printf("--------------- NVRDescr DUMP -----------------------\n");
    printf("name        size        addr        value       \n");
    printf("size       %3d        %lx        %ld       \n",sizeof(long), (unsigned long)nvrAddr->size, nvrAddr->size);
}







