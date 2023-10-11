// Lucas Demarco Cambraia Lemos 2110013
// Natalia Passini Grossmann 2110456

// Exercicio 1 - PARALELISMO

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#define NUM_TRABALHADORES 10
#define TAM_VETOR 10000

int a[TAM_VETOR];
int soma = 0;

void *multiplica_vetor (void *id) {

    int inicio = ((int)id) * 1000;
    int final = ((int)id + 1) * 1000;

    for (int i = inicio; i < final; i++) {
        a[i] *= 2;
        soma += a[i];
    }

    pthread_exit(NULL);
}


int main (void) {
    pthread_t trabalhadores[NUM_TRABALHADORES];
    int trabalhador;
    clock_t inicio;

    for (int i = 0; i < TAM_VETOR; i++) {
        a[i] = 5;
    }

    for (trabalhador = 0; trabalhador < NUM_TRABALHADORES; trabalhador++) {
        inicio = clock();
        pthread_create(&trabalhadores[trabalhador], NULL, multiplica_vetor, (void *)trabalhador);
    }

    for (trabalhador = 0; trabalhador < NUM_TRABALHADORES; trabalhador++) {
        pthread_join(trabalhadores[trabalhador], NULL);
    }

    clock_t final = clock();

    printf("Soma de todas as posicoes do vetor: %d\n", soma);
    printf("Tempo de execucao: %f\n", (float)(final - inicio)/CLOCKS_PER_SEC);

    return 0;
}