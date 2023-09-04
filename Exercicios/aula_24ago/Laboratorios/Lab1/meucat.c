// Lucas Demarco Cambraia Lemos - 2110013
// Natalia Passini Grossmann - 2110456

#include <stdio.h>
#include <stdlib.h>

void escaneia_arquivo (char *nome_arquivo);

int main (int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        escaneia_arquivo(argv[i]);
    }

    return 0;
}

void escaneia_arquivo (char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");

    if (f == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo);
    }

    char c;

    while ((c = fgetc(f)) != EOF) {
        printf("%c", c);
    }


    fclose(f);
}