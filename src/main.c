/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  11/02/2013 09:12:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "global.h"
#include "nv_api.h"
 #define ALIGNMENT 8
#define ALIGN(size) (((size)+(ALIGNMENT-1))& ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

int main(int argc, const char *argv[])
{
    int shmid;
    int pid;
    // int child_status;
    NVRDescr * nvrAddr;
    char name[]="/home/syi/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";
    char rootname[]="root1";
    //char name[]="/scratch/syi.scratch/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";

    // create shared memory

    if((pid=fork())<0) {
        perror("Fail to fork");
        exit(EXIT_FAILURE);
    }else if(pid==0) {
        printf("Child's turn!\n");
        nvrAddr = NVOpenRegion(name,0,SHM_SIZE);
        shmid = nvrAddr->ID;
        printf("child shmid is %d\n",shmid);
        printf("base addr of nvr  %p\n",nvrAddr);
        DEBUG_OUTPUT("NVOpenRegion 1st Test Pass");
        NVRDescrDump(nvrAddr);
        // add data here
        int size=100;
        int * array=(int *)NVMallocNaive(nvrAddr, size);
        int i;
        for (i = 0; i < 100; ++i)
        {
            *(array+i)=i;
        }
        printf("malloced array addr of root1 is  %p\n",array);
        if((NVNewRoot(nvrAddr, (void *)array, rootname, sizeof(int)))!=0){
            perror("Fail to NVNewRoot");
            exit(EXIT_FAILURE);
        }
        DEBUG_OUTPUT("NVNewRoot Test Pass");
        NVRootmapDump(nvrAddr);
        // #ifdef  DEBUG
        //     printf("%d\n",NVCloseRegion(nvrAddr));
        // #endif   
        sleep(5);
        if(NVCloseRegion(nvrAddr)!=0){
            perror("Fail to close region");
            exit(EXIT_FAILURE);
        }
        DEBUG_OUTPUT("NVCloseRegion Test Pass");
        //kill(pid,SIGUSR1)
    }else{
        sleep(2);
        printf("Parent's turn!\n");
        
        nvrAddr = NVOpenRegion(name,0,SHM_SIZE);
        NVRDescrDump(nvrAddr);
        sleep(8);
        NVRDescrDump(nvrAddr);
        DEBUG_OUTPUT("ProcessCnt Test Pass");
        // if(NVCloseRegion(nvrAddr)!=0){
        //     perror("Fail to close region");
        //     exit(EXIT_FAILURE);
        // }
        // DEBUG_OUTPUT("NVCloseRegion Test Pass");
       
        // nvrAddr = NVOpenRegion(name,0,SHM_SIZE);
        shmid = nvrAddr->ID;
        printf("parent shmid is %d\n",shmid);
        printf("base addr of nvr  %p\n",nvrAddr);
        NVRDescrDump(nvrAddr);
        DEBUG_OUTPUT("NVOpenRegion 2nd Test Pass");
        //char namee[]="/home/syi/GitRepo/nvm-simulator/nvm.daemon/NVRegion2";
        
        
        NVRootmapItem_t  * nvrmPtrIdx=(NVRootmapItem_t  *)NVFetchRoot(nvrAddr,rootname);
        printf("the nvrmPtrIdx is %p\n", nvrmPtrIdx);
        if (nvrmPtrIdx==NULL){
            perror("Fail to fetch root");
            exit(EXIT_FAILURE);   
        }
        void *p = nvrmPtrIdx->location;
        size_t type = nvrmPtrIdx->type;
        printf("type size is %d\n", type); // from this type we know it is int.
    
        printf("the size of array is %d\n", *((size_t *)(p-SIZE_T_SIZE) ) );
        int arraylen = *((size_t *)(p-SIZE_T_SIZE) );
        int j=0;
        for (j = 0; j < arraylen; ++j)
        {
            printf("the %d of array is %d\n", j,*((int *)p+j));
        }

        //
        //
        if(NVDeleteRegion(name)==-1){
            perror("Fail to delete region");
            exit(EXIT_FAILURE);
        }
        DEBUG_OUTPUT("NVDeleteRegion Test Pass");
    }


    exit(EXIT_SUCCESS);
}
