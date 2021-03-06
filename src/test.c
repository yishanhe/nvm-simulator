/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  Create a NVRegion1
 *                  Malloc an int array(szie=100)(array)
 *                  NVNewRoot sets the array address as the NVRoot root1.
 *                  NVMalloc an int array(size=100)(array_shm)
 *                  NVNewRoot sets the array_shm address as the NVRoot root2.
 *                  Assignment value to this array
 *                  Print out the array value
 *                  NVFree the array_shm
 *                  Free the array
 *                  Close this region
 *                  Delete this region
 *                  Quit.
 *
 *        Version:  1.0
 *        Created:  12/05/2013 01:45:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Syi,
 *   Organization:  CS@WM
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
#include "global.h"
#include "nv_api.h"
#include "memlib.h"
#include "mm.h"
#include <unistd.h>



int main(int argc, char const *argv[])
{
    int shmid;
    int pid;
    // int child_status;
    NVRDescr * nvrAddr;
#if defined(SHM)
    //char name[]="/scratch/syi.scratch/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";
    char name[]="/home/syi/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";
#elif defined(MMAP)
    char name[]="/home/syi/GitRepo/nvm-simulator/nvm.daemon/NVRegion4";
#endif
    char rootname1[]="root1";
    char rootname2[]="root2";
    //char name[]="/scratch/syi.scratch/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";


    nvrAddr = NVOpenRegion(name,0,SHM_SIZE);
    shmid = nvrAddr->ID;

    printf("The shmid is %d\n",shmid);
    printf("base addr of nvr  %p\n",nvrAddr);
    DEBUG_OUTPUT("NVOpenRegion 1st Test Pass");
    NVRDescrDump(nvrAddr);
    int array_size = 100;
    int * array = (int*)malloc(sizeof(int)*array_size);

    // if (mm_init()<0)
    // {
    //     e("MVMalloc initialization error.");
    // }

    int * array_shm = (int*)NVMalloc(nvrAddr,sizeof(int)*array_size);
    for (int i = 0; i < array_size; ++i)
    {
        *(array_shm+i)=i;
    }
    for (int j = 0; j < array_size; ++j)
    {
        printf("%d\n", *(array_shm+j));
    }

    if((NVNewRoot(nvrAddr, (void *)array, rootname1, sizeof(int)))!=0){
        perror("Fail to NVNewRoot");
        exit(EXIT_FAILURE);
    }
    if((NVNewRoot(nvrAddr, (void *)array_shm, rootname2, sizeof(int)))!=0){
        perror("Fail to NVNewRoot");
        exit(EXIT_FAILURE);
    }

    int * new_array_shm = (int*)NVRealloc(nvrAddr,array_shm,2*sizeof(int)*array_size);
    printf("new_array_shm is %p\n", new_array_shm);
    for (int j = 0; j < array_size; ++j)
    {
        printf("%d\n", *(new_array_shm+j));
    }
    NVFree(new_array_shm);
    free(array);

    DEBUG_OUTPUT("NVNewRoot Test Pass");
    NVRDescrDump(nvrAddr);
    NVRootmapDump(nvrAddr);


    if(NVCloseRegion(nvrAddr)!=0){
        perror("Fail to close region");
        exit(EXIT_FAILURE);
    }
    DEBUG_OUTPUT("NVCloseRegion Test Pass");
    //kill(pid,SIGUSR1)


    if(NVDeleteRegion(name)==-1){
        perror("Fail to delete region");
        exit(EXIT_FAILURE);
    }
    DEBUG_OUTPUT("NVDeleteRegion Test Pass");


    exit(EXIT_SUCCESS);
}
