// Lucas Demarco Cambraia Lemos 2110013
// Natalia Passini Grossmann 2110456

// Exercicio 1 - SEQUENCIAL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    clock_t inicio_programa = clock();
    clock_t final_programa;

    int segmento_a, segmento_soma, segmento_parcela, status, id;
    segmento_a = shmget(IPC_PRIVATE, sizeof(int) * 10000, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    segmento_soma = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    segmento_parcela = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    int *a;
    int *soma;
    int *parcela;

    a = (int *)shmat(segmento_a, 0, 0);
    soma = (int *)shmat(segmento_soma, 0, 0);
    parcela = (int *)shmat(segmento_parcela, 0, 0);

    *soma = 0;

    for (int i = 0; i < 10000; i++) {
        a[i] = 5;
    }

    for (*parcela = 0; (*parcela) < 10; (*parcela)++) {
        id = fork();
        if (id == 0) {

            int inicio = (*parcela) * (1000);
            int fim = ((*parcela) + 1) * (1000);

            int auxiliar = 0;
            for (int i = inicio; i < fim; i++) {
                a[i] *= 2;    
                auxiliar += a[i];
            }

            *soma += auxiliar;
            exit(0);
        } else {
            waitpid(-1, &status, 0);
        }
    }

    if (id != 0) {
        printf("%d\n", *soma);
        final_programa = clock();
        printf("%f\n", (float)(final_programa - inicio_programa)/CLOCKS_PER_SEC);
    }

    shmdt(a);
    shmdt(parcela);
    shmdt(soma);
    shmctl(segmento_a, IPC_RMID, NULL);
    shmctl(segmento_parcela, IPC_RMID, NULL);
    shmctl(segmento_soma, IPC_RMID, NULL);
    return 0;
}