#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                } while (0)



int main(int argc, char *argv[]) {
        pid_t  childPid;

        printf("starting pid: %ld\n", (long) getpid());

        printf("40 secs to check...\n");
        sleep(40);
        printf("unsharing...\n");

        if (unshare(CLONE_NEWPID) == -1 )
                errExit("unshare error");
        
        printf("pid namespace for children has been prepared\n");
        
        switch(childPid = fork()){
        case -1:
            errExit("fork failed");
        case 0:
            printf("entered new PID namespace\n");
            printf("child pid inside pidns: %ld\n", (long) getpid());
            printf("parent pid inside pidns: %ld\n", (long) getppid());
            fflush(stdout);
            sleep(600);
            break;
        default:
            printf("child pid in main namespace: %ld\n", (long) childPid);
            if(waitpid(childPid, NULL, 0) == -1)
                errExit("waitpid error");
            printf("child terminated\n");

        }
        exit(EXIT_SUCCESS);
        
        
}

