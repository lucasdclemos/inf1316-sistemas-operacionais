#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct queue {
  struct job *first;
  struct job *last;
} Queue;

typedef struct job {
  char name[20];
  int start_time;
  int duration;
  int pid;
  struct job *next;
} Job;
