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

int main(int argc, const char *argv[])
{
    int shmid;
    int pid;
    NVRDescr * nvrAddr;
    char name[]="/home/syi/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";
    //char name[]="/scratch/syi.scratch/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";

    // create shared memory

    if((pid=fork())<0) {
        perror("Fail to fork");
        exit(EXIT_FAILURE);
    }else if(pid==0) {
        nvrAddr = NVOpenRegion(name,0,SHM_SIZE);
        shmid = nvrAddr->ID;
        printf("child shmid is %d\n",shmid);
        printf("base addr of nvr  %p\n",nvrAddr);
    }else{
        wait(NULL);
        nvrAddr = NVOpenRegion(name,0,SHM_SIZE);
        shmid = nvrAddr->ID;
        printf("parent shmid is %d\n",shmid);
        printf("base addr of nvr  %p\n",nvrAddr);
        NVRDescrDump(nvrAddr);
        DEBUG_OUTPUT("NVOpenRegion Test Pass");
        //char namee[]="/home/syi/GitRepo/nvm-simulator/nvm.daemon/NVRegion2";
        if(NVDeleteRegion(name)==-1){
            perror("Fail to shmctl");
            exit(EXIT_FAILURE);
        }
        DEBUG_OUTPUT("NVDeleteRegion Test Pass");
    }
    exit(EXIT_SUCCESS);
}
