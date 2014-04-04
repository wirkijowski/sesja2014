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

        int fd;
        struct utsname uts;

        if(argc < 2){
                fprintf(stderr, "Usage %s: /proc/PID/ns/FILE\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        fd = open(argv[1], O_RDONLY);

        if(fd == -1)
                errExit("open");

        if(setns(fd, 0) == -1)
                errExit("setNs");

        if(uname(&uts) == -1)
                errExit("uname");

        printf("joined uts namespace: %s \n", uts.nodename);
        fflush(stdout);

        exit(EXIT_SUCCESS);
}
