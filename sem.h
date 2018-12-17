#ifndef SEM_H
#define SEM_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define NUM_OF_SEMS 13


int sem_id(key_t key);
void sem_init(int semnum, int value));
void sem_destroy_all();
void down (int semnum, struct sembuf sem);
void up (int semnum, struct sembuf sem);
int sem_get_value(int semnum);

#endif
