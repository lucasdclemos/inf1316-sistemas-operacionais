// Lucas Demarco Cambraia Lemos - 2110013
// Natalia Passini Grossmann - 2110456

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define EVER ;;

void childhandler(int signo);
int delay;

int main (int argc, char *argv[]) {
    pid_t pid;
    signal(SIGCHLD, childhandler);
    char* parametros[] = {"sleep15", NULL};
    if ((pid = fork()) < 0) {
        fprintf(stderr, "Erro ao criar filho\n");
        exit(-1);
    }
    if (pid == 0)        
        execve("sleep15", parametros, 0);
    else{
        sscanf(argv[1], "%d", &delay); 
        sleep(delay);
        printf("Program exceeded limit of %d seconds!\n", delay);
        kill(pid, SIGKILL);
    }
    return 0;
}

void childhandler(int signo) {
    int status;
    pid_t pid = wait(&status);
    printf("Child %d terminated within %d seconds com estado %d.\n", pid, delay, status);
    exit(0);
}