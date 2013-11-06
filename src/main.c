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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "global.h"

int main(int argc, const char *argv[])
{
    int shmid;
    int pid;
    NVRDescr * nvrAddr;
    char name[]="/scratch/syi.scratch/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";

    // create shared memory

    if((pid=fork())<0) {
        perror("Fail to fork");
        exit(EXIT_FAILURE);
    }else if(pid==0) {
        nvrAddr = NVOpenRegion(name,0,SHM_SIZE);
    }else{
        wait(NULL);
        nvrAddr = NVOpenRegion(name,0,SHM_SIZE);

        if(shmctl(shmid,IPC_RMID,NULL)<0){
            perror("Fail to shmctl");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
