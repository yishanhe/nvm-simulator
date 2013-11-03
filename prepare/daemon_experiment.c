/*
 * =====================================================================================
 *
 *       Filename:  daemon_experiment.c
 *
 *    Description:  how to write a daemon process
 *
 *        Version:  1.0
 *        Created:  10/27/2013 11:54:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Syi
 *   Organization:  WM@CS
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>


int main(int argc, const char *argv[])
{

    pid_t pid;
    pid_t sid;

    FILE *fp = NULL;
    // create a child process
    pid = fork();

    if (pid<0) { //failed
        printf ( "fork failed ! \n" );
        exit(1);
    }

    // kill parent procee
    if (pid>0) {
        printf ( "process_id of child process %d\n", pid );// child process ID is returned at parent process
        exit(0);
    }

    umask(0); // unmask the file mode

    // create a new SID for the child process
    sid = setsid();
    if (sid<0) {
        exit(1);
    }

    // change the working directory
    if ((chdir("/scratch/syi.scratch/GitRepo/nvm-simulator/nvm.daemon/"))<0) {
        exit(1);
    }

    // close the standard file descriptors
    close(STDIN_FILENO);
    //close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // open log file
    fp = fopen("Log.txt","w+");

    fprintf(fp, "Log Info: ...\n");
    // the big loop
    while (1) {
        printf("I am running\n");
        sleep(1);
        fprintf(fp, "Logging info ...\n");
        fflush(fp);
    }

    fclose(fp);
    return;
}		/* -----  end of function create_daemon  ----- */
