#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]){

        struct utsname uts, uts2;

        if(argc < 2){
                fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        printf("process pid: %ld\n", (long) getpid());


        if(uname(&uts) == -1){
            perror("uname() error");
            exit(EXIT_FAILURE);
        }

        printf("initial hostname: %s \n", uts.nodename);

        sleep(20); 

        printf("\nchanging uts namespace...\n");

        if(unshare(CLONE_NEWUTS) == -1){
            perror("unshare() error");
            exit(EXIT_FAILURE);
        }
        printf("inside new uts namespace");
        if(sethostname(argv[1], strlen(argv[1])) == -1){
            perror("sethostname() error");
            exit(EXIT_FAILURE);
        }

        if(uname(&uts2) == -1){
            perror("uname() 2 error");
            exit(EXIT_FAILURE);
        }

        printf("new hostname: %s \n", uts2.nodename);

        sleep(600);

        exit(EXIT_SUCCESS);

}
