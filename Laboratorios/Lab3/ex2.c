// Lucas Demarco Cambraia Lemos 2110013
// Natalia Passini Grossmann 2110456

// Exercicio 2

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#define NUM_TRABALHADORES 2
#define TAM_VETOR 10000

int a[TAM_VETOR];

void *multiplica_vetor (void *id) {

    for (int i = 0; i < 10000; i++) {
        a[i] *= 2;
        a[i] += 2;
    }

    pthread_exit(NULL);
}

int compara_vetor () {

    for (int i = 0; i < 9999; i++) {
        if (a[i] != a[i + 1]) {
            return 0;
        }
    }

    return a[0];
}


int main (void) {
    pthread_t trabalhadores[NUM_TRABALHADORES];
    int trabalhador;

    for (int i = 0; i < TAM_VETOR; i++) {
        a[i] = 5;
    }

    for (trabalhador = 0; trabalhador < NUM_TRABALHADORES; trabalhador++) {
        pthread_create(&trabalhadores[trabalhador], NULL, multiplica_vetor, (void *)trabalhador);
    }

    for (trabalhador = 0; trabalhador < NUM_TRABALHADORES; trabalhador++) {
        pthread_join(trabalhadores[trabalhador], NULL);
    }

    int resposta = compara_vetor();

    if (resposta == 0) {
        printf("As posicoes do vetor nao sao iguais.\n");
    } else {
        printf("As posicoes do vetor sao iguais e tem valor %d.\n", resposta);
    }

    return 0;
}