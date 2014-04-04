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



static void sigchldHandler(int signal){

    pid_t pid;
    int status;

    while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) != 0){
        if(pid == -1){
            break;
        }
    }

}


static int simpleInit(void *arg){
    int i;
    pid_t child;
    struct sigaction sigact;
    char **exec_argv;
    char **exec_envp;
    sigact.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigact.sa_handler = sigchldHandler;
    sigemptyset(&sigact.sa_mask);
    
    if(sigaction(SIGCHLD, &sigact, NULL) == -1)
        err("sigaction() error");

    signal(SIGTTOU, SIG_IGN);

    printf("Init started with pid %ld\n", (long) getpid());
    if(setpgid(0, 0) == -1)
        err("setpgid()");

    if(tcsetpgrp(STDIN_FILENO, getpgrp()) == -1)
        err("tcsetpgrpchild");

    switch(child = fork()){
        case -1:
            err("fork() in init failed");
        case 0:
            printf("\tinit's child pid %ld\n", (long) getpid());
            //printf("pid %ld in the foreground\n", (long) getpid());
            execve("/bin/bash", exec_argv, exec_envp);
            err("ececve() error");
        default:
           while(1){
               pause();
           }
            
    }

    
    /*
    for(i=0; i<100; i++){
        sleep(5);
    }
    */
    exit(EXIT_SUCCESS);
}
#define STACK_SIZE (1024 * 1024)

static char init_stack[STACK_SIZE];


int main(int argc, char *argv[]){
    int flags;
    int verbose = 1;
    pid_t init_pid;
    argv[0] = "pidns: start";
    flags = CLONE_NEWPID | CLONE_NEWNS ;
    if(verbose)
        printf("pidns_init has pid: %ld\n", (long) getpid());

    init_pid = clone(simpleInit, init_stack + STACK_SIZE,
            flags | SIGCHLD, 0);
    if(verbose)
        printf("init process has %ld pid in parent ns\n", (long) init_pid);

    if(init_pid == -1)
        err("init not cloned");

    if(waitpid(init_pid, NULL, 0) == -1)
        err("waitpid for init");
    
    if(verbose){
       printf("init exited\n");
       printf("exiting...");
    }
    exit(EXIT_SUCCESS);
}


