// Elaborar um programa que crie, inicialize e imprime um vetor de 20 posições com números inteiros [1, 100] aleatoriamente
// e que crie dois outros processos: um que ordene e exiba o vetor ordenado e outro que calcule a média dos valores do vetor

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

void ordena_exibe (int* vetor);
void calcula_media (int* vetor);

int main () {
    srand(time(NULL));
    int vetor[20];
    
    printf("----------Vetor----------\n");
    for (int i = 0; i < 20; i++) {
        int r = (rand() % 100) + 1;
        vetor[i] = r;
        printf("%d\n", vetor[i]);
    }

    if (fork() == 0) {
        ordena_exibe(vetor);
    } else {
        if (fork() == 0) {
            calcula_media(vetor);
        }
    }

}

void calcula_media (int* vetor) {
    int soma = 0;

    for (int i = 0; i < 20; i++) {
        soma += vetor[i];
    }

    printf("\nMédia calculada: %d\n", soma / 20);
}

void ordena_exibe (int* vetor) {

    int auxiliar = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (vetor[i] < vetor[j]) {
                auxiliar = vetor[j];
                vetor[j] = vetor[i];
                vetor[i] = auxiliar;
            }
        }
    }

    printf("\n----------Vetor ordenado----------\n");
    for (int i = 0; i < 20; i++) {
        printf("%d\n", vetor[i]);
    }

}
