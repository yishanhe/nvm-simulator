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
#define BUFF_SIZE 32

int main(int argc, const char *argv[])
{
    int shmid;
    int pid;
    void * shmaddr;

    // create shared memory
    if((shmid=shmget(IPC_PRIVATE,BUFF_SIZE,0666|IPC_CREAT))<0) {
        perror("Faile to shmget");
        exit(EXIT_FAILURE);
    }

    if((pid=fork())<0) {
        perror("Fail to fork");
        exit(EXIT_FAILURE);
    }else if(pid==0) {
    }else{
        if(shmctl(shmid,IPC_RMID,NULL)<0){
            perror("Fail to shmctl");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
