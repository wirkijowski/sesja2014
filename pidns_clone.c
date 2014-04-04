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



static int newPidNs(void *arg) {

        printf("child PID in %s ns: %ld\n", (char *) arg, (long) getpid());
        printf("parent PID in %s ns: %ld\n", (char *) arg, (long) getppid());
        
        char *root_dir =  "/var/opt/namespaces/proc";
        char mount_point[sizeof(root_dir) + sizeof(arg)];
        strcat(mount_point, root_dir);
        strcat(mount_point, arg);
        
        if(mount_point != NULL){
            mkdir(mount_point, 0555);
            if(mount("proc", mount_point, "proc", 0, NULL) == -1)
                errExit("mount error");
            printf("procfs mounted at %s\n", mount_point);
        }



        fflush(stdout);
        execlp("sleep", "sleep", "600", (char *) NULL);
        errExit("execlp has failed");
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

int main(int argc, char *argv[]) {
        pid_t child_pid;
       
        if (argc < 2){
            fprintf(stderr, "Usage %s: name\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        child_pid = clone(newPidNs,
                child_stack + STACK_SIZE,
                CLONE_NEWPID | SIGCHLD, argv[1]);

        if (child_pid == -1)
            errExit("clone error");
        
        printf("child's PID: %ld\n", (long) child_pid);
      
        if (waitpid(child_pid, NULL, 0) == -1)
            errExit("waitpid error");

        exit(EXIT_SUCCESS);

}

