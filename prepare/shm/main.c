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
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#define BUFF_SIZE 1024


int father_do_work(int shmid){
    char *buf;
    void *shmaddr;
    sem_t *prsem;
    sem_t *pwsem;

    if ((prsem = sem_open("rsem", O_CREAT, 0666,0))== SEM_FAILED) {
        perror("Fail to open sem");
        return -1;
    }
    if ((pwsem = sem_open("wsem", O_CREAT, 0666,0))== SEM_FAILED) {
        perror("Fail to open sem");
        return -1;
    }

    if( (shmaddr = shmat(shmid,NULL,0)) == (void *) -1  )  {
        perror("Fail to shmat");
        exit(EXIT_FAILURE);
    }

    buf = (char *) shmaddr;

    while(1)
    {
        if (sem_wait(pwsem)<0) {
            perror("Fail to sem wait");
            break;
        }

        printf(">");
        fgets(buf,BUFF_SIZE,stdin);
        buf[strlen(buf)-1]='\0';

        if (sem_post(prsem)<0) {
            perror("Failed to sem post");
            break;
        }

        if(strncmp(buf,"quit",4)==0) {
            if(shmdt(shmaddr)<0){
                perror("Fail to shmaddr");
                exit(EXIT_FAILURE);
            }
            break;
        }
        usleep(500);

    }
    return 0;
}

int child_do_work(int shmid) {
    char *buf;
    void *shmaddr;
    sem_t *prsem;
    sem_t *pwsem;

    if ((prsem = sem_open("rsem", O_CREAT, 0666,0))== SEM_FAILED) {
        perror("Fail to open sem");
        return -1;
    }
    if ((pwsem = sem_open("wsem", O_CREAT, 0666,0))== SEM_FAILED) {
        perror("Fail to open sem");
        return -1;
    }

    if( (shmaddr = shmat(shmid,NULL,0)) == (void *) -1  )  {
        perror("Fail to shmat");
        exit(EXIT_FAILURE);
    }

    buf = (char *) shmaddr;

    while(1) {
        if(sem_wait(prsem)<0){
            perror("Fail to prsem");
            break;
        }
        printf("read buf: %s.\n",buf);

        if(sem_post(pwsem)<0) {
            perror("Fail to pwsem");
            break;
        }

        if(strncmp(buf,"quit",4)==0){
            if(shmdt(shmaddr)<0){
                perror("Fail to shmaddr");
                exit(EXIT_FAILURE);
            }
            break;

        }
    }
    return 0;

}



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
        child_do_work(shmid);
    }else{
        father_do_work(shmid);
        wait(NULL);
        if(shmctl(shmid,IPC_RMID,NULL)<0){
            perror("Fail to shmctl");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
