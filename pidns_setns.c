#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[]){
        pid_t child;
        int fd;

        if(argc < 2){
                fprintf(stderr, "Usage %s: /proc/PID/ns/FILE\n", argv[0]);
                exit(EXIT_FAILURE);
        }
        printf("Parent started with PID: %ld\n", (long) getpid());
        fd = open(argv[1], O_RDONLY);

        if(fd == -1)
                errExit("open");

        if(setns(fd, CLONE_NEWPID) == -1)
                errExit("setNs");

        switch( child =  fork()){
            case -1:
                perror("fork()");
                exit(EXIT_FAILURE);
            case 0:
                printf("joined PID snamespace: %s PID: %ld\n", argv[1], (long) getpid());
                fflush(stdout);
                sleep(100);
            default:
                printf("forked child PID: %ld\n", (long) child);
                if(waitpid(child, NULL, 0) == -1){
                    perror("waitpid()");
                    exit(EXIT_FAILURE);
                }
        }

        exit(EXIT_SUCCESS);
}
