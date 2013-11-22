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



int main(int argc, const char *argv[])
{
    int shmid;
    int pid;
    // int child_status;
    NVRDescr * nvrAddr;
    char name[]="/home/syi/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";
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
    
    mem_init();
    if (mm_init()<0)
    {
        e("MVMalloc initialization error.");
    }
    
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

    DEBUG_OUTPUT("NVNewRoot Test Pass");
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
