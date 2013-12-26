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
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "nv_api.h"
#include "mm.h"
#include "memlib.h"
#include "global.h"
#include "GeneralHashFunctions.h"

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

#if defined(SHM)

    if ((keyNVRegion=ftok(name, 0))<0) {
 /* :TODO:11/05/2013 10:52:10 AM:: reminder of creating backing file */
        DEBUG_OUTPUT("Please create this file to back the memory.");
        e("NVOpenRegion ftok error");
    }

    // unsigned int RSHash  (char* str, unsigned int len);


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
#elif defined(MMAP)
#ifdef  FIXMEMBASE
    shmPtr = membase;
#else      /* -----  not FIXMEMBASE  ----- */
    shmPtr = startingAddr;
#endif     /* -----  not FIXMEMBASE  ----- */
    // open the file
    int fd;
    if ( (fd = open(name, O_RDWR, S_IRWXU)) < 0){
        if (errno==ENOENT){// this region is not exists, create a new one
            if ((fd = open(name, O_RDWR|O_CREAT|O_TRUNC, S_IRWXU)) < 0){
                e("NVOpenRegion fd open error");
            }
            // successfully create this file
            flag = 1;
            // make it becomes certain size
            int result = lseek(fd, SHM_SIZE-1, SEEK_SET);
            if (result == -1) {
                close(fd);
                e("NVOpenRegion lseek error");
            }
            result = write(fd, "", 1);
            if (result < 0) {
                close(fd);
                e("NVOpenRegion write at the end of file error");
            }
          }else
            e("NVOpenRegion fd open error");
    }
    // get file stat
    struct stat file_stat;
    if ( fstat( fd, &file_stat) < 0 )
    {
        // printf(" fstat wrong");
        // exit(1);
        e("NVOpenRegion fstat error");
    }
    keyNVRegion = file_stat.st_ino;
   // ftruncate(fd, file_stat.st_size+size);
    printf("filein size = %ld\n",file_stat.st_size);
    void *start_fp;
    // check size here.
    //
    if( ( start_fp = mmap(shmPtr, file_stat.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 )) == MAP_FAILED)
    {
        // printf("mmap wrong");
        // exit(0);
        e("NVOpenRegion mmap error");
    }
    // now you can close
    close(fd);
    shmId = RSHash(name, nameLen); // provide shmid using hash

    // as long as the file exists, the shared memory exists.

#endif

    nvrAddr = (NVRDescr *)shmPtr;

/* :TODO:11/05/2013 03:24:08 PM:: warp between shmctl and NVRDescr */
    if (flag==1) {// initiate the meta data of this region
        nvrAddr->size = size;
        nvrAddr->refKey = keyNVRegion;
        nvrAddr->rootMapOffset =  size;
        nvrAddr->dataRegionOffset =  sizeof(NVRDescr);
        nvrAddr->shareFlag = SHARE;
        #if defined(SHM)
            nvrAddr->processCnt = shmDsPtr->shm_nattch; // better processCnt
        #elif defined(MMAP)
            nvrAddr->processCnt = 1; // the process creates this shm, the processCnt will always be 1
        #endif
        nvrAddr->nvRootCnt = 0;
        nvrAddr->ID = shmId;
        nvrAddr->nameLen = nameLen;
        memset(nvrAddr->name, '\0', sizeof(nvrAddr->name));
        strcpy(nvrAddr->name,name);
/* :TODO:11/06/2013 12:53:07 PM:: initiate the rootmap */
        mem_init();
        if (mm_init()<0)
        {
            e("MVMalloc initialization error.");
        }

    } else {
        // update NVRDescr
        nvrAddr->processCnt = shmDsPtr->shm_nattch; // 1 is the initial value
        nvrAddr->refKey = keyNVRegion; // it is possible that the keyNVRegion is null and needs to be update
        // #ifdef  DEBUG
        //     printf("%d\n",nvrAddr->refKey);
        // #endif     /* -----  not DEBUG  ----- */
        mem_init();

        // mem_init();
        // if (mm_init()<0)
        // {
        //     e("MVMalloc initialization error.");
        // }
    }
    return nvrAddr;
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  NVDeleteRegion
 *  Description:  For shm, it is shmctl with IPC_RMID. For munmap, it is msync and munmap delete
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
    #if defined(SHM)
    // get shmId,  otherwise error
    // if this shm exists, size can be 0
    if ((shmId=shmget(keyNVRegion,0,SHM_MODE))<=0) {
        perror("NVDeleteRegion shmget error");
        return -1;
    }
 /* :TODO:11/06/2013 10:58:02 PM:: return remaining processes */
    if (shmctl(shmId,IPC_RMID,NULL)<0) {
        perror("NVDeleteRegion shmctl rmid error");
        return -1;
    }
    #elif defined(MMAP)
    // get shmdid first by hashing  
    // int nameLen = strlen(name);
    // shmId = RSHash  (name, nameLen); 

    // just delete the file. msync and munmap is ensured by NVCloseRegion
    if(remove(name)<0) {
    	perror("NVDeleteRegion unlink file error");
    	return -1;
    }
    #endif
    return 0;
}



int NVCloseRegion(NVRDescr * addr) {
    int shmId;
// #ifdef  DEBUG
//     printf("%d\n",addr->refKey);
// #endif     /* -----  not DEBUG  ----- */
	if (addr->processCnt>1) {
        addr->processCnt--;
        if (addr->processCnt==0) {
            addr->refKey=0;//NULL
        }
    }

    #if defined(SHM)
	    if ((shmId=shmget(addr->refKey,0,SHM_MODE))<=0) {
	        perror("NVCloseRegion shmget error");
	        return -1;
	    }


	    if ((shmdt(addr))<0) {
	        perror("NVCloseRegion shmdt error");
	        // add the processCnt back by 1
	        return -1;
	    }
    #elif defined(MMAP)

	    if (munmap(addr, (size_t)addr->size)<0) { // it will call msync implicite
	    	perror("NVCloseRegion munmap error");
    		return -1; 
	    }
        
    #endif


    // if ((shmId=shmget(addr->refKey,0,SHM_MODE))<=0) {
    //     perror("NVCloseRegion shmget error");
    //     return -1;
    // }

    // if (addr->processCnt>1) {
    //     addr->processCnt--;
    //     if (addr->processCnt==0) {
    //         addr->refKey=0;//NULL
    //     }
    //     //return 0;
    // }

    // if ((shmdt(addr))<0) {
    //     perror("NVCloseRegion shmdt error");
    //     // add the processCnt back by 1
    //     return -1;
    // }


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
                // update dataregion mem
                return 0;
            }
        } else {
            errno =ENOMEM;
            e("NVNewRoot fail"); // return -1
        }
        // DEBUG_OUTPUT("test");


    }
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


void * NVMalloc(NVRDescr * addr, int size) {

    void * newMemPtr = mm_malloc((size_t)size);
    // printf("newMemPtr is %p\n", newMemPtr);
    return newMemPtr;

}



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  NVFree
 *  Description:
 * =====================================================================================
 */
int NVFree(void * addr) {
    // update NVDescr
    // @TODO What if you are freeing a NVRoot, design NVRootMap valid tag
    mm_free(addr);
    return 0;
}



// void * NVMallocNaive(NVRDescr * addr, int size) {
//     /*
//     void * nvrmPtrBrk= offset2addr(addr, addr->dataRegionOffset);
//     long newOffset = addr->dataRegionOffset+size;
//     if (newOffset>=addr->rootMapOffset) {
//         errno=ENOMEM;
//         e("NVMalloc fail");
//     }
//     // do the malloc
//     (addr->dataRegionOffset)+=size;
//     return (void *)offset2addr(addr,newOffset);
//     */
//     if(addr->nvRootCnt==0) {
//         nvmm_dataregion_init(addr);
//     }
//     else if(addr->nvRootCnt>0) {
//         nvmm_dataregion_update(addr);
//     }
//     else {
//         e("NVMalloc fail");
//     }

//     int newsize = ALIGN(size+SIZE_T_SIZE);
//     void *p = nvmm_sbrk(newsize);
//     if(p==(void *)-1) {
//         return NULL;
//     }
//     else {
//         *(size_t *)p = size;
//         return (void *)((char *)p+SIZE_T_SIZE);
//     }
// }



/*-----------------------------------------------------------------------------
 * EXTRA API
 *-----------------------------------------------------------------------------*/

NVRootmapItem_t * getNVRootMapPtr(NVRDescr * nvrAddr){
    NVRootmapItem_t * nvrmPtr;
    nvrmPtr = offset2addr(nvrAddr,nvrAddr->rootMapOffset);
//    memset();
   // nvmm_max_addr = offset2addr(nvrAddr,nvrAddr->size);
   // nvmm_start_brk = offset2addr(nvrAddr,sizeof(NVRDescr));// start address of dataregion
  //  nvmm_brk = nvmm_start_brk;
    return nvrmPtr;
}


// isNVRoot
