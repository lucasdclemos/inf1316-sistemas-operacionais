// Lucas Demarco Cambraia Lemos - 2110013
// Natalia Passini Grossmann - 2110456

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

void tratamento_erro(int signo);

int main(void) {
    float x, y;
    signal(SIGFPE, tratamento_erro);
    printf("Digite dois numeros reais: ");
    scanf("%f %f", &x, &y);
    
    printf("%.2f + %.2f = %.2f\n", x, y, (x + y));
    printf("%.2f - %.2f = %.2f\n", x, y, (x - y));
    printf("%.2f * %.2f = %.2f\n", x, y, (x * y));
    printf("%.2f / %.2f = %.2f\n", x, y, (x / y));

    return 0;
}

void tratamento_erro(int signo) {
    printf("erro de floating point\n");
}