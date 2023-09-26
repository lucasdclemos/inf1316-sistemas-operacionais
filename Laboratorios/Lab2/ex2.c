// Lucas Demarco Cambraia Lemos 2110013
// Natalia Passini Grossmann 2110456

// Exercicio 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>   
#include <unistd.h>

int main(int argc, char *argv[]) {
    
    int segmento_a, segmento_parcela, status, id;
    segmento_a = shmget(IPC_PRIVATE, sizeof(int) * 10000, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    segmento_parcela = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    int *a;
    int *parcela;

    a = (int *)shmat(segmento_a, 0, 0);
    parcela = (int *)shmat(segmento_parcela, 0, 0);

    for (int i = 0; i < 10000; i++) {
        a[i] = 5;
    }

    for ((*parcela) = 0; (*parcela) < 2; (*parcela)++) {
        id = fork();

        if (id == 0) {

            for (int i = 0; i < 10000; i++) {
                a[i] *= 2;
                a[i] += 2;
            }
            exit(0);
        }
    }
    
    if (id != 0) {
        for (int i = 0; i < 2; i++) {
            waitpid(-1, &status, 0);
        }
        
        int verificador = 1;

        for (int i = 0; i < 9999; i++) {
            if (a[i] != a[i + 1]) {
                verificador = 0;
            }
        }

        if (verificador == 1) {
            printf("Todas as posicoes do vetor sao iguais. O valor e %d.\n", a[100]);
        } else {
            printf("As posicoes do vetor nao sao iguais.\n");
        }
    }

    shmdt(a);
    shmdt(parcela);
    shmctl(segmento_a, IPC_RMID, NULL);
    shmctl(segmento_parcela, IPC_RMID, NULL);

    return 0;
}