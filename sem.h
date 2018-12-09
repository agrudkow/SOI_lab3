#ifndef SEM_H
#define SEM_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define NUM_OF_SEMS 13

int sem_id(key_t key);
void sem_init(key_t key, int semnum, int value);
void sem_destroy_all(key_t key);
void down(key_t key, int semnum);
void up(key_t key, int semnum);
int sem_get_value(key_t key, int semnum);

#endif
