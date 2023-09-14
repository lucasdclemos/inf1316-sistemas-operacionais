// Criar programa que crie 2 outros processos, todos acessando a memória compartilhada
// O processo pai escreve uma mensagem na memória e os filhos exibem a mensagem


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char* argv[]) {
    int segmento;
    char *p;
    segmento = shmget(IPC_PRIVATE, sizeof(char) * 50, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    p = (char*) shmat (segmento, 0, 0);

    if (fork() == 0) {
        printf("%s\n", p);
        if (fork() == 0) {
            printf("%s\n", p);
        }
    } else {
        strcpy(p, "String para testar o codigo");
    }

    shmdt (p);
    // libera a memória compartilhada
    shmctl (segmento, IPC_RMID, 0);
    return 0;
}