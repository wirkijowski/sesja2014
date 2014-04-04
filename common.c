#define _GNU_SOURCE
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>



void err(char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}



