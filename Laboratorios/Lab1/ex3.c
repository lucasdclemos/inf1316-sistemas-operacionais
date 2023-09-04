// Lucas Demarco Cambraia Lemos - 2110013
// Natalia Passini Grossmann - 2110456

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void type_prompt();
void read_command(char* comando, char** parametros);
void inicializa_array(char** parametros);

int main (void)  {

    char input[100];
    char* comando;
    char* parametros[100];

    while (1) {
        type_prompt();
        
        fgets(input, sizeof(input), stdin);

        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }

        char* p = strtok(input, " ");         
        comando = p;
        int posicao = 1;

        parametros[0] = comando;

        inicializa_array(parametros);

        while(p) {
            p = strtok(NULL, " ");
            if (p != NULL) {
                parametros[posicao] = p;
            }
            posicao++;
        }
        
        parametros[posicao] = NULL;

        read_command(comando, parametros);
    }
}

void inicializa_array (char** parametros) {
    for (int i = 1; i < 100; i++) {
        parametros[i] = NULL;
    } 
}

void read_command (char* comando, char** parametros) {
    int status;

    if (fork() != 0 ) {
        waitpid(-1, &status, 0);
    } else {
        if (strcmp(comando, "meuecho") == 0 || strcmp(comando, "meucat") == 0) {
            execve(comando, parametros, 0);
            exit(0);
        } else {
            char path[100] = "/bin/";
            strcat(path, comando);
            execve(path, parametros, 0);
            exit(0);
        }        
    }
}

void type_prompt () {
    printf("\n");
    printf("$ ");
    return;
}