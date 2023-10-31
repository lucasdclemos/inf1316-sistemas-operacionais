// Lucas Demarco Cambraia Lemos - 2110013
// Natalia Passini Grossmann - 2110456

#include "data_structures.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void start_queue(Queue *q);
void add_to_queue(Queue *q, char *program_name, int start_time, int duration,
                  int pid);
void print_queue(Queue *q);
Job *remove_first_job(Queue *q);
int empty_list(Queue *q);
void free_memory(Queue *q);

int main() {
  char name[10];
  int status;
  int read, s, d, line = 0;
  FILE *file = fopen("exec.txt", "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.");
    exit(1);
  }

  int segmento_nome, segmento_start_time, segmento_duration, segmento_semaphore,
      segmento_reading;
  segmento_nome = shmget(IPC_PRIVATE, sizeof(char) * 20, IPC_CREAT | 0666);
  segmento_start_time = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
  segmento_duration = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
  segmento_semaphore = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
  segmento_reading = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);

  char *nome;
  int *start_time;
  int *duration;
  int *semaphore;
  int *finished_reading;

  nome = (char *)shmat(segmento_nome, 0, 0);
  start_time = (int *)shmat(segmento_start_time, 0, 0);
  duration = (int *)shmat(segmento_duration, 0, 0);
  semaphore = (int *)shmat(segmento_semaphore, 0, 0);
  finished_reading = (int *)shmat(segmento_reading, 0, 0);

  *semaphore = 0;
  *finished_reading = 0;

  Queue *interpreter_queue;
  interpreter_queue = (Queue *)malloc(sizeof(Queue));
  start_queue(interpreter_queue);

  pid_t interpretador = fork();

  if (interpretador != 0) {
    while (!feof(file)) {
      read = fscanf(file, "Run %s I=%d D=%d\n", name, &s, &d);
      if (read == 3) {
        int hasConflict = 0;
        Job *current = interpreter_queue->first;
        while (current != NULL) {
          if (s < current->start_time + current->duration &&
              s + d > current->start_time) {
            hasConflict = 1;
            break;
          }
          current = current->next;
        }
        if (s + d > 60) {
          printf("Duracao do processo %s maior que 60 segundos.\n", name);
        } else if (hasConflict) {
          printf("%s descartado. Conflito de tempo com o processo %s.\n", name,
                 current->name);
        } else { // REAL-TIME
          strcpy(nome, name);
          *start_time = s;
          *duration = d;
          add_to_queue(interpreter_queue, nome, *start_time, *duration, 0);
          *semaphore = 1;
        }
      } else if (read == 1) { // ROUND-ROBIN
        strcpy(nome, name);
        *start_time = -1;
        *duration = -1;
        *semaphore = 1;
      } else {
        fprintf(stderr, "Erro na linha %d\n", line);
        exit(-3);
      }
      sleep(1);
    }
    fclose(file);
    *finished_reading = 1;
  } else {
    Queue *real_time_queue;
    Queue *round_robin_queue;
    char escalonator_job_name[20];
    int escalonator_start_time, escalonator_duration;
    struct timeval current_time;

    real_time_queue = (Queue *)malloc(sizeof(Queue));
    round_robin_queue = (Queue *)malloc(sizeof(Queue));
    start_queue(real_time_queue);
    start_queue(round_robin_queue);

    while (1) {
      if (*semaphore == 1) {
        if (*start_time < 0 && *duration < 0) {
          escalonator_duration = *duration;
          escalonator_start_time = *start_time;
          strcpy(escalonator_job_name, nome);
          add_to_queue(round_robin_queue, escalonator_job_name,
                       escalonator_start_time, escalonator_duration, 0);
        } else {
          escalonator_duration = *duration;
          escalonator_start_time = *start_time;
          strcpy(escalonator_job_name, nome);
          add_to_queue(real_time_queue, escalonator_job_name,
                       escalonator_start_time, escalonator_duration, 0);
        }
        *semaphore = 0;
      }
      if (*finished_reading == 1) {
        break;
      }
    }

    if (empty_list(real_time_queue) == 0) {
      printf("Real Time Queue: \n");
      print_queue(real_time_queue);
    }
    if (empty_list(round_robin_queue) == 0) {
      printf("Round Robin Queue: \n");
      print_queue(round_robin_queue);
    }
    printf("\n");

    gettimeofday(&current_time, NULL);
    int escalonator_start = current_time.tv_sec;

    while (1) {
      gettimeofday(&current_time, NULL);
      if (empty_list(real_time_queue) == 0) {
        if ((current_time.tv_sec - escalonator_start) % 60 ==
            real_time_queue->first->start_time) {
          Job *job = remove_first_job(real_time_queue);
          gettimeofday(&current_time, NULL);
          int start_real_time = current_time.tv_sec;
          printf("%ld s - ", (current_time.tv_sec - escalonator_start));
          printf("Executando programa %s - Real Time\n", job->name);
          while (1) {
            if (job->pid == 0) {
              job->pid = fork();
              if (job->pid == 0) {
                char path[10] = "./";
                strcat(path, job->name);
                char *argv[] = {NULL};
                execvp(path, argv);
                sleep(job->duration);
              } else {
                while (1) {
                  gettimeofday(&current_time, NULL);
                  if (current_time.tv_sec >= start_real_time + job->duration) {
                    printf("Finalizando programa %s\n", job->name);
                    kill(job->pid, SIGSTOP);
                    add_to_queue(real_time_queue, job->name, job->start_time,
                                 job->duration, job->pid);
                    printf("Real Time Queue: \n");
                    print_queue(real_time_queue);
                    start_real_time = 10000000;
                    break;
                  }
                }
                break;
              }
            } else {
              kill(job->pid, SIGCONT);
              while (1) {
                gettimeofday(&current_time, NULL);
                if (current_time.tv_sec >= start_real_time + job->duration) {
                  printf("Finalizando programa %s\n", job->name);
                  kill(job->pid, SIGSTOP);
                  add_to_queue(real_time_queue, job->name, job->start_time,
                               job->duration, job->pid);
                  printf("Real Time Queue: \n");
                  print_queue(real_time_queue);
                  start_real_time = 10000000;
                  break;
                }
              }
              break;
            }
          }
        }
        if (empty_list(round_robin_queue) == 0) {
          Job *job = remove_first_job(round_robin_queue);
          printf("%ld s - ", current_time.tv_sec - escalonator_start);
          printf("Executando programa %s - Round Robin\n", job->name);
          gettimeofday(&current_time, NULL);
          int start_round_robin = current_time.tv_sec;
          if (job->pid == 0) {
            job->pid = fork();
            if (job->pid == 0) {
              char path[10] = "./";
              strcat(path, job->name);
              char *argv[] = {NULL};
              execvp(path, argv);
              sleep(job->duration);
            } else {
              while (1) {
                gettimeofday(&current_time, NULL);
                int end_time_robin = current_time.tv_sec;
                if (end_time_robin - start_round_robin >= 1) {
                  printf("Finalizando o programa %s\n", job->name);
                  kill(job->pid, SIGSTOP);
                  add_to_queue(round_robin_queue, job->name, -1, -1, job->pid);
                  printf("Round Robin Queue\n");
                  print_queue(round_robin_queue);
                  break;
                }
                usleep(1000);
              }
            }
          } else {
            kill(job->pid, SIGCONT);
            while (1) {
              gettimeofday(&current_time, NULL);
              int end_time_robin = current_time.tv_sec;
              if (end_time_robin - start_round_robin >= 1) {
                printf("Finalizando o programa %s\n", job->name);
                kill(job->pid, SIGSTOP);
                add_to_queue(round_robin_queue, job->name, -1, -1, job->pid);
                printf("Round Robin Queue\n");
                print_queue(round_robin_queue);
                break;
              }
              usleep(1000);
            }
          }
        }
        if (current_time.tv_sec - escalonator_start >= 120) {
          printf("Escalonador atingiu 2 minutos de execucao.\n");
          free_memory(round_robin_queue);
          free_memory(real_time_queue);
          exit(0);
        }
      }
      usleep(1000);
    }
  }
  wait(&status);
  shmdt(nome);
  shmdt(start_time);
  shmdt(duration);
  shmdt(semaphore);
  shmdt(finished_reading);
  shmctl(segmento_nome, IPC_RMID, 0);
  shmctl(segmento_start_time, IPC_RMID, 0);
  shmctl(segmento_duration, IPC_RMID, 0);
  shmctl(segmento_semaphore, IPC_RMID, 0);
  shmctl(segmento_reading, IPC_RMID, 0);
  return 0;
}

int empty_list(Queue *q) {
  if (q->first == NULL) {
    return 1;
  } else {
    return 0;
  }
}

void print_queue(Queue *q) {
  if (q->first == NULL) {
    return;
  }
  Job *current = q->first;
  int contador = 1;
  while (current != NULL) {
    printf("%d) ", contador);
    printf("%s ", current->name);
    if (current->start_time != -1) {
      printf("- Start Time: %d ", current->start_time);
    }
    if (current->duration != -1) {
      printf("- Duration: %d ", current->duration);
    }
    printf("\n");
    current = current->next;
    contador++;
  }
  printf("\n");
}

void start_queue(Queue *q) {
  q->first = NULL;
  q->last = NULL;
}

void add_to_queue(Queue *q, char *program_name, int start_time, int duration,
                  int pid) {
  Job *node = (Job *)malloc(sizeof(Job));
  strcpy(node->name, program_name);
  node->start_time = start_time;
  node->duration = duration;
  node->pid = pid;
  node->next = NULL;
  if (q->first == NULL) {
    q->first = node;
    q->last = node;
  } else {
    q->last->next = node;
    q->last = node;
  }
}

Job *remove_first_job(Queue *q) {
  Job *j = q->first;
  q->first = q->first->next;
  return j;
}

void free_memory(Queue *q) {
  Job *current = q->first;
  Job *next;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
}
