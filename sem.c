#ifndef SEM_C
#define SEM_C
#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

struct sembuf sem = {0, 0, 0};
//Get id of given semaphore
int sem_id(key_t key) {
  int sem_id;

  sem_id = semget(key, NUM_OF_SEMS, 0666 | IPC_CREAT);
  if (sem_id == -1) {
    perror("semget error\n");
    exit(1);
  }

  return sem_id;
}
//Initialize semaphore with given value
void sem_init(int id, int semnum, union semun sem_info) {
  if (semctl(id, semnum, SETVAL, sem_info) == -1) {
    perror("sem_init: semctl error\n");
    exit(1);
  }
}
//Destroy all semaphores from a set pointed with key
void sem_destroy_all(int id) {
  if (semctl(id, 0, IPC_RMID) == -1) {
    perror("sem_destroy_all: semctl error\n");
    exit(1);
  }
}
//Decrease value of semaphore
void down (int semid, int semnum, struct sembuf sem) {
  sem.sem_num = semnum;
  sem.sem_op = -1;
  sem.sem_flg = 0;

  if (semop(semid, &sem, 1) == -1) {
    printf("Semnum: %d - ", semnum);
    perror("down: semop error\n");
    exit(1);
  }
}
//Increase value of semaphore
void up (int semid, int semnum, struct sembuf sem) {
  sem.sem_num = semnum;
  sem.sem_op = 1;
  sem.sem_flg = 0;

  if (semop(semid, &sem, 1) == -1) {
    printf("Semnum: %d - ", semnum);
    perror("up: semop error\n");
    exit(1);
  }
}

int sem_get_value(int semid, int semnum) {
  int value;

  value = semctl(semid, semnum, GETVAL);
  if (value == -1) {
    printf("Semnum: %d - ", semnum);
    perror("sem_get_value: semctl error\n");
    exit(1);
  }

  return value;
}
#endif
