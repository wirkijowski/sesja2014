#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    pid_t pid;

    switch(pid = fork()){
        case -1:
            exit(EXIT_FAILURE);

        case 0:
            printf("child, PID: %ld, PPID: %ld\n", (long) getpid(), (long) getppid());
            do {
                sleep(2);
            } while(getppid() != 1);

            printf("child -> orphan, PID: %ld, PPID %ld\n", (long) getpid(), (long) getppid());
            sleep(100);
            printf("exiting...\n");
            exit(EXIT_SUCCESS);

    }         
    sleep(30);
    printf("parent, PID: %ld\n", (long) getpid());
    exit(EXIT_SUCCESS);
}
