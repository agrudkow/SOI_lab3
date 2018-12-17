#ifndef SEM_C
#define SEM_C
#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

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
void sem_init(int semnum, int value) {
  if (semctl(semid, semnum, SETVAL, value) == -1) {
    perror("sem_init: semctl error\n");
    exit(1);
  }
}
//Destroy all semaphores from a set pointed with key
void sem_destroy_all() {
  if (semctl(semid, 0, IPC_RMID) == -1) {
    perror("sem_destroy_all: semctl error\n");
    exit(1);
  }
}
//Decrease value of semaphore
void down (int semnum, struct sembuf sem) {
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
void up (int semnum, struct sembuf sem) {
  sem.sem_num = semnum;
  sem.sem_op = 1;
  sem.sem_flg = 0;

  if (semop(semid, &sem, 1) == -1) {
    printf("Semnum: %d - ", semnum);
    perror("up: semop error\n");
    exit(1);
  }
}

int sem_get_value(int semnum) {
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
