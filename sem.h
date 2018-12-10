#ifndef SEM_H
#define SEM_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define NUM_OF_SEMS 13


int sem_id(int id);
void sem_init(int id, int semnum, union semun sem_info);
void sem_destroy_all(int id);
void down(int semid, int semnum, struct sembuf sb);
void up(int semid, int semnum, struct sembuf sb);
int sem_get_value(int semid, int semnum);

#endif
