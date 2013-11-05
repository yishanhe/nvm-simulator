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
#include <error.h>


#define ARRAY_SIZE 40000
#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE 0600 // user read and write

char array[ARRAY_SIZE];

int main(int argc, const char *argv[])
{
    int shmid;
    char * ptr,* shmptr;

    printf("array[] from %lx to %lx\n",(unsigned long)&array[0],(unsigned long)&array[ARRAY_SIZE]);
    printf("stack around %lx\n", (unsigned long)&shmid);


    if ((ptr=malloc(MALLOC_SIZE))==NULL) {
        perror("malloc error");
        exit(1);
    }

    printf("malloced from %lx to %lx\n",(unsigned long)ptr,(unsigned long)ptr+MALLOC_SIZE);

    if ((shmid=shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE))<=0) {
        perror("shmget error");
        exit(1);
    }
    printf("the shmid is %d\n",shmid);
    if ((shmptr=shmat(shmid,0,0))==(void *)-1) {
        perror("shmat error");
        exit(1);
    }
    printf("shared memory attached from %lx to %lx\n",(unsigned long)shmptr,(unsigned long)shmptr+SHM_SIZE);
    printf("shared memory attached distance from stack %lx \n",(-(unsigned long)shmptr+(unsigned long)&shmid));

    //system("ipcs -m>1");
    if(shmctl(shmid,IPC_RMID,0)<0) {
        perror("shmctl error");
        exit(1);
    }

    // test ftok and shmget
    key_t shm_key = ftok("/scratch/syi.scratch/GitRepo/nvm-simulator/prepare/shm/test.nvregion",1);

    int shmid1;
    printf("the key is %lx\n",(int)shm_key);
    if ((shmid1=shmget(shm_key, SHM_SIZE, SHM_MODE|IPC_CREAT))<=0) {
        perror("shmget error");
        exit(1);
    }
    system("ipcs -m>1");
    printf("the shmid with key is %d\n",shmid1);
    if(shmctl(shmid1,IPC_RMID,0)<0) {
        perror("shmctl error");
        exit(1);
    }



    // the file is existed
//    system("ipcs -m>2");
    return 0;
}
