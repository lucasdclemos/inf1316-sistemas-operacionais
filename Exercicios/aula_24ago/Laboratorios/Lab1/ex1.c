// Lucas Demarco Cambraia Lemos - 2110013
// Natalia Passini Grossmann - 2110456

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main (void)  {

    int n = 3;
    int status;

    if (fork() != 0) {
        printf("\n---------- Processo Pai ----------\n\n");

        for (int i = 1; i <= 10000; i++) {
            n++;
        }
        
        printf("Processo pai, pid = %4d, n = %d\n", getpid(), n);
        waitpid(-1, &status, 0);
    } else {
        printf("\n---------- Processo Filho ----------\n\n");

        for (int i = 1; i <= 10000; i++) {
            n += 10;
        }

        printf("Processo filho, pid = %4d, n = %d\n", getpid(), n);
    }

    return 0;
}