// Lucas Demarco Cambraia Lemos - 2110013
// Natalia Passini Grossmann - 2110456

#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_MSG_LEN 15
#define BUFFER_SIZE 8

int main() {
  int segment1, segment2, segment3;
  char **message_array;
  int *contador;
  pid_t p1, p2;
  sem_t *mutex, *buffer, *semaphore;
  int status;

  segment1 = shmget(IPC_PRIVATE, sizeof(char *) * BUFFER_SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
  segment2 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
  segment3 = shmget(IPC_PRIVATE, sizeof(char *), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
  message_array = (char **)shmat(segment1, 0, 0);
  contador = (int *)shmat(segment2, 0, 0);
  mutex = sem_open("mutex", O_CREAT, 0666, 0);
  buffer = sem_open("buffer", O_CREAT, 0666, 0);
  semaphore = sem_open("semaphore", O_CREAT, 0666, 0);
  sem_init(mutex, 1, 1);
  sem_init(buffer, 1, BUFFER_SIZE);
  sem_init(semaphore, 1, 0);

  *contador = 0;

  for(int i = 0; i < BUFFER_SIZE; i++) {
    message_array[i] = (char *)malloc(MAX_MSG_LEN * sizeof(char));
    message_array[i] = (char *)shmat(segment3, 0, 0);
  }
  
  p1 = fork();
  if (p1 < 0) {
    printf("Erro na criacao do processo 1\n");
    exit(-1);
  }
  if (p1 == 0) {
    // PRODUTOR
    char aux[MAX_MSG_LEN];
    for(int i = 0; i < 128; i++) {
      sem_wait(buffer);
      sem_wait(mutex);
      sprintf(aux, "mensagem %d", i + 1);
      strcpy(message_array[*contador], aux);
      printf("Mensagem produzida: %s\n", message_array[*contador]);
      // printf("%s\n", message_array[*contador]);
      *contador += 1;
      sem_post(mutex);
      sem_post(semaphore);
      // sleep(1);
    }
    exit(1);
  }

  p2 = fork();
  if (p2 < 0) {
    printf("Erro na criacao do processo 2\n");
    exit(-1);
  }
  if (p2 == 0) {
    // CONSUMIDOR
    // sleep(1);
    for(int j = 0; j < 128; j++) {
      sem_wait(semaphore);
      sem_wait(mutex);
      *contador -= 1;
      // printf("%s\n", message_array[0]);
      printf("Mensagem consumida: %s\n", message_array[*contador]);
      sem_post(mutex);
      sem_post(buffer);
      // sleep(1);
    }
    exit(1);
  }

  for(int i = 0; i < 2; i++) {
    wait(&status);
  }

  for(int i = 0; i < BUFFER_SIZE; i++) {
    shmdt(message_array[i]);
    free(message_array[i]);
  }
  
  shmdt(message_array);
  shmdt(contador);
  sem_destroy(semaphore);
  sem_destroy(mutex);
  sem_destroy(buffer);
  shmctl(segment1, IPC_RMID, NULL);
  shmctl(segment2, IPC_RMID, NULL);
  shmctl(segment3, IPC_RMID, NULL);

  return 0;
}
