// Lucas Demarco Cambraia Lemos 2110013
// Natalia Passini Grossmann 2110456

// Exercicio 1 - SEQUENCIAL

#include <stdio.h>
#include <time.h>
#define NUM_TRABALHADORES 10
#define TAM_VETOR 10000

int a[TAM_VETOR];
int soma = 0;

void multiplica_vetor (int id) {

    int inicio = ((int)id) * 1000;
    int final = ((int)id + 1) * 1000;

    for (int i = inicio; i < final; i++) {
        a[i] *= 2;
        soma += a[i];
    }
}

int main (void) {
    clock_t inicio;
    int trabalhador;

    for (int i = 0; i < TAM_VETOR; i++) {
        a[i] = 5;
    }

    for (trabalhador = 0; trabalhador < NUM_TRABALHADORES; trabalhador++) {
        inicio = clock();
        multiplica_vetor(trabalhador);
    }

    clock_t final = clock();

    printf("Soma de todas as posicoes do vetor: %d\n", soma);
    printf("Tempo de execucao: %f\n", (float)(final - inicio)/CLOCKS_PER_SEC);

    return 0;
}