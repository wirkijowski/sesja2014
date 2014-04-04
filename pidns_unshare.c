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


        if (argc < 2){
            fprintf(stderr, "Usage %s: name\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        printf("starting pid: %ld\n", (long) getpid());
        printf("parent pid: %ld\n", (long) getppid());

        printf("40 secs to check...\n");
        sleep(40);
        printf("unsharing...\n");

        if (unshare(CLONE_NEWPID) == -1 )
                errExit("unshare error");
        
        printf("unshared\n");
        
        execlp("sleep", "sleep", "600", (char *) NULL);

        errExit("execvp error");
}

