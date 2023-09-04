// Lucas Demarco Cambraia Lemos - 2110013
// Natalia Passini Grossmann - 2110456

#include <stdio.h>

int main (int argc, char *argv[]) {
    
    for (int i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }

    printf("\n");

    return 0;
}