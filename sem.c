#include <stdio.h>
#include <stdlib.h>
#include "sem.h"
//Get id of given semaphore
int sem_id(key_t key) {
  int sem_id;

  sem_id = semget(key, NUM_OF_SEMS + 1, 0600 | IPC_CREAT);
  if (sem_id == -1) {
    perror("semget error\n");
    exit(1);
  }

  return sem_id;
}
//Initialize semaphore with given value
void sem_init(key_t key, int semnum, int value) {
  int id = sem_id(key);
  if (semctl(id, semnum, SETVAL, value) == -1) {
    perror("sem_init: semctl error\n");
    exit(1);
  }
}
//Destroy all semaphores from a set pointed with key
void sem_destroy_all(key_t key) {
  int id = sem_id(key);
  if (semctl(id, 0, IPC_RMID) == -1) {
    perror("sem_destroy_all: semctl error\n");
    exit(1);
  }
}
//Decrease value of semaphore
void p (key_t key, int semnum) {
  struct sembuf sb;
  int id = sem_id(key);

  sb.sem_num = key;
  sb.sem_op = -1;
  sb.sem_flg = 0;

  if (semop(id, &sb, 1) == -1) {
    perror("p: semop error\n");
    exit(1);
  }
}
//Increase value of semaphore
void v (key_t key, int semnum) {
  struct sembuf sb;
  int id = sem_id(key);

  sb.sem_num = key;
  sb.sem_op = 1;
  sb.sem_flg = 0;

  if (semop(id, &sb, 1) == -1) {
    perror("v: semop error\n");
    exit(1);
  }
}

int sem_get_value(key_t key, int semnum) {
  int value;
  int id = sem_id(key);

  value = semctl(id, semnum, GETVAL);
  if (value == -1) {
    perror("sem_get_value: semctl error\n");
    exit(1);
  }

  return value;
}
