// Lucas Demarco Cambraia Lemos - 2110013
// Natalia Passini Grossmann - 2110456

#include <stdio.h>
#include <unistd.h>

int main (void) {
    fprintf(stdout, "indo dormir...\n");
    sleep(5);
    fprintf(stdout, "Acordei!\n");
    return 0;
}