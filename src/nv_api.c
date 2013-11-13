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
#include "nv_mm.h"

#define ALIGNMENT 8
#define ALIGN(size) (((size)+(ALIGNMENT-1))& ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

extern int errno;
extern char *nv_mm_start_brk;
extern char *nv_mm_brk;
extern char *nv_mm_max_addr;
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
          }else
            e("NVOpenRegion shmget error");
    }

    // shmat

#ifdef  FIXMEMBASE
    if ((shmPtr = shmat(shmId,membase,0) )==(void *)-1)
#else      /* -----  not FIXMEMBASE  ----- */
    if ((shmPtr = shmat(shmId,startingAddr,0) )==(void *)-1)
#endif     /* -----  not FIXMEMBASE  ----- */
    {
        e("NVOpenRegion shmat error");
    }
    if ((void *)shmPtr!=membase){
        errno = EFAULT;
        e("NVOpenRegion shmat error");
    }
    // get shm_ds
    if ((shmctl(shmId,IPC_STAT,&shmDsBuf))<0) {
        e("NVOpenRegion shmctl error");
    }


    nvrAddr = (NVRDescr *)shmPtr;

/* :TODO:11/05/2013 03:24:08 PM:: warp between shmctl and NVRDescr */
    if (flag==1) {// initiate the meta data of this region
        nvrAddr->size = size;
        nvrAddr->refKey = keyNVRegion;
        nvrAddr->rootMapOffset =  size;
        nvrAddr->dataRegionOffset =  sizeof(NVRDescr);
        nvrAddr->shareFlag = SHARE;
        nvrAddr->processCnt = shmDsPtr->shm_nattch;
        nvrAddr->nvRootCnt = 0;
        nvrAddr->ID = shmId;
        nvrAddr->nameLen = nameLen;
        memset(nvrAddr->name, '\0', sizeof(nvrAddr->name));
        strcpy(nvrAddr->name,name);
/* :TODO:11/06/2013 12:53:07 PM:: initiate the rootmap */

    } else {
        // update NVRDescr
        nvrAddr->processCnt = shmDsPtr->shm_nattch; // 1 is the initial value
        nvrAddr->refKey = keyNVRegion; // it is possible that the keyNVRegion is null and needs to be updated
        // #ifdef  DEBUG
        //     printf("%d\n",nvrAddr->refKey);
        // #endif     /* -----  not DEBUG  ----- */
    }

    return nvrAddr;
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  NVDeleteRegion
 *  Description:
 * =====================================================================================
 */
int NVDeleteRegion(char * name) {
    key_t keyNVRegion;
    //struct shmid_ds shmDsBuf;
    //struct shmid_ds * shmDsPtr=&shmDsBuf;
    int shmId;
    // get refKey
    if ((keyNVRegion=ftok(name, 0))<0) {
        perror("NVDeleteRegion ftok error");
        return -1;
    }
    // get shmId,  otherwise error
    // if this shm exists, size can be 0
    if ((shmId=shmget(keyNVRegion,0,SHM_MODE))<=0) {
        perror("NVDeleteRegion shmget error");
        return -1;
    }
 /* :TODO:11/06/2013 10:58:02 PM:: return remaining processes */
    // RM
    if (shmctl(shmId,IPC_RMID,NULL)<0) {
        perror("NVDeleteRegion shmctl rmid error");
        return -1;
    }
    return 0;
}

int NVCloseRegion(NVRDescr * addr) {
    // DEBUG_OUTPUT("1");
    int shmId;
// #ifdef  DEBUG
//     printf("%d\n",addr->refKey);
// #endif     /* -----  not DEBUG  ----- */
    if ((shmId=shmget(addr->refKey,0,SHM_MODE))<=0) {
        perror("NVCloseRegion shmget error");
        return -1;
    }
    // DEBUG_OUTPUT("2");

    if (addr->processCnt>1) {
        addr->processCnt--;
        if (addr->processCnt==0) {
            addr->refKey=0;//NULL
        }
        //return 0;
    }

    if ((shmdt(addr))<0) {
        perror("NVCloseRegion shmdt error");
        return -1;
    }


    return 0;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  NVRDescrDump
 *  Description:
 * =====================================================================================
 */
void NVRDescrDump(NVRDescr *nvrAddr){
    printf("--------------- NVRDescr DUMP -----------------------\n");
    printf("              size        addr                 value  \n");
    printf("-----------------------------------------------------\n");
    printf("size          %3ld         %lx         %ld\n",sizeof(long), (unsigned long)&nvrAddr->size, nvrAddr->size);
    printf("refKey        %3ld         %lx         %ld\n",sizeof(NVKey_t), (unsigned long)&nvrAddr->refKey, nvrAddr->refKey);
    printf("rootMapOffset %3ld         %lx         %ld\n",sizeof(long), (unsigned long)&nvrAddr->rootMapOffset, nvrAddr->rootMapOffset);
    printf("shareFlag     %3ld         %lx         %d\n",sizeof(int), (unsigned long)&nvrAddr->shareFlag, nvrAddr->shareFlag);
    printf("processCnt    %3ld         %lx         %d\n",sizeof(int), (unsigned long)&nvrAddr->processCnt, nvrAddr->processCnt);
    printf("nvRootCnt     %3ld         %lx         %d\n",sizeof(int), (unsigned long)&nvrAddr->nvRootCnt, nvrAddr->nvRootCnt);
    printf("ID            %3ld         %lx         %d\n",sizeof(int), (unsigned long)&nvrAddr->ID, nvrAddr->ID);
    printf("nameLen       %3ld         %lx         %d\n",sizeof(int), (unsigned long)&nvrAddr->nameLen, nvrAddr->nameLen);
    printf("name          %3ld         %lx         %s\n",sizeof(nvrAddr->name), (unsigned long)&nvrAddr->name, nvrAddr->name);
    printf("-----------------------------------------------------\n");
}

void NVRootmapDump(NVRDescr *nvrAddr){
    NVRootmapItem_t  * nvrmPtrIdx = offset2addr(nvrAddr, nvrAddr->rootMapOffset);
    printf("--------------- NVRootmapDump DUMP ------------------\n");
    printf("                  type       location        name  \n");
    while((addr2offset(nvrAddr,nvrmPtrIdx))< nvrAddr->size){
        printf("rootmap          %3ld         %lx         %s\n",nvrmPtrIdx->type, (unsigned long)nvrmPtrIdx->location, nvrmPtrIdx->name);    
        nvrmPtrIdx++; //  move to next existed rootmapitem
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
 *  DATA REGION RELATED API
 *-----------------------------------------------------------------------------*/

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  NVFetchRoot
 *  Description:
 * =====================================================================================
 */
void * NVFetchRoot(NVRDescr * addr, char * name) {
    // check name
    NVRootmapItem_t  * nvrmPtrIdx = offset2addr(addr, addr->rootMapOffset);

    while((addr2offset(addr,nvrmPtrIdx))< addr->size){
        if((strcmp(name,nvrmPtrIdx->name))==0){
            return (void *) nvrmPtrIdx;
        }
        nvrmPtrIdx++; //  move to next existed rootmapitem
    }
    return NULL;
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  NVNewRoot
 *  Description:
 * =====================================================================================
 */
int NVNewRoot(NVRDescr * addr, void *p, char * name, size_t size) {
    // check valid addr of p
    long newRootOffset =  addr2offset(addr,p);
    void * brk = offset2addr(addr,addr->dataRegionOffset);
    // DEBUG_OUTPUT("test");
    if ( newRootOffset>=addr->rootMapOffset || newRootOffset<=sizeof(NVRDescr)) {
        errno = EINVAL;
        e("NVNewRoot fail");
    }

    NVRootmapItem_t * nvrmPtrCurr= offset2addr(addr, addr->rootMapOffset);
//    NVRootmapItem_t  * nvrmPtrIdx=nvrmPtrCurr ;
// DEBUG_OUTPUT("test");
    if (NVFetchRoot(addr,name)!=NULL) {
        errno=EEXIST;
        e("NVNewRoot fail"); // return -1
    } else {
        // DEBUG_OUTPUT("test");
        // // segment fault
        // #ifdef  DEBUG
        //     printf("%p\n",nvrmPtrCurr);
        //     printf("%p\n",brk);
        // #endif
        if ((void *)(--nvrmPtrCurr)> brk){
            if (p==NULL){
                DEBUG_OUTPUT("Error in input address");
                return -2;
            } else {
                // DEBUG_OUTPUT("test");
                nvrmPtrCurr->location = p;
                nvrmPtrCurr->type = size;
                strcpy(nvrmPtrCurr->name,name);
                // update meta data
                addr->rootMapOffset=addr2offset(membase,nvrmPtrCurr);
                addr->nvRootCnt++;
                return 0;
            }
        } else {
            errno =ENOMEM;
            e("NVNewRoot fail"); // return -1
        }
        // DEBUG_OUTPUT("test");


    }
}




/*
 * ===  FUNCTION  ======================================================================
 *         Name:  NVFree
 *  Description:
 * =====================================================================================
 */
int NVFree(void * addr) {
    // free this pointer, if this pointer is in the process' stack
    // memset(addr, '\0', sizeof(nvrAddr->name));
    // currently we do nothing
    // update NVDescr
    return 0;
}



/*-----------------------------------------------------------------------------
 *  DATA REGIOIN MEM RELATED API
 *-----------------------------------------------------------------------------*/

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  NVMalloc
 *  Description:
 * =====================================================================================
 */

void * NVMallocNaive(NVRDescr * addr, int size) {
    /*
    void * nvrmPtrBrk= offset2addr(addr, addr->dataRegionOffset);
    long newOffset = addr->dataRegionOffset+size;
    if (newOffset>=addr->rootMapOffset) {
        errno=ENOMEM;
        e("NVMalloc fail");
    }
    // do the malloc
    (addr->dataRegionOffset)+=size;
    return (void *)offset2addr(addr,newOffset);
    */
    if(addr->nvRootCnt==0) {
        nvmm_dataregion_init(addr);
    }
    else if(addr->nvRootCnt>0) {
        nvmm_dataregion_update(addr);
    }
    else {
        e("NVMalloc fail");
    }

    int newsize = ALIGN(size+SIZE_T_SIZE);
    void *p = nvmm_sbrk(newsize);
    if(p==(void *)-1) {
        return NULL;
    }
    else {
        *(size_t *)p = size;
        return (void *)((char *)p+SIZE_T_SIZE);
    }
}




