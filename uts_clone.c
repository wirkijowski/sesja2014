#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static int createNs(void *arg){
        struct utsname uts;
        if(sethostname(arg, strlen(arg)) == -1){
                perror("child uname");
                exit(EXIT_FAILURE);
        }

        if(uname(&uts) == -1){
                perror("child uts");
                exit(EXIT_FAILURE);
        }
        sleep(1);
        printf("child uts.nodename: %s\n", uts.nodename);

        fflush(stdout);
        sleep(600);
        return 0;
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

int main(int argc, char *argv[]){

        pid_t child_pid;
        struct utsname uts;

        if(argc < 2){
                fprintf(stderr, "Usage: %s <child-hostname>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
  
        if(uname(&uts) == -1){
            perror("uname");
            exit(EXIT_FAILURE);
        }

      
        printf("parent pid: %ld\n", (long) getpid());
        printf("parent uts.nodename: %s \n", uts.nodename);
        fflush(stdout);
        child_pid = clone(createNs,
                        child_stack + STACK_SIZE, 
                        CLONE_NEWUTS | SIGCHLD, argv[1]);
        if(child_pid == -1){
                perror("clone");
                exit(EXIT_FAILURE);
        }
        printf("child pid: %ld\n", (long) child_pid);


  
        if(waitpid(child_pid, NULL, 0) == -1){
                perror("waitpid");
                exit(EXIT_FAILURE);
        }
        printf("child terminated");

        exit(EXIT_SUCCESS);

}
