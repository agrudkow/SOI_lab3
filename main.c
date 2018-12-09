#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "queue.h"
#include "sem.h"

#define SHM_KEY_A   10001
#define SHM_KEY_B   10002
#define SHM_KEY_C   10003
#define SHM_KEY_D   10004
#define SEM_KEY     20001

enum { MUTEX_A, MUTEX_B, MUTEX_C, MUTEX_D,
   SEM_EMPTY_A, SEM_EMPTY_B, SEM_EMPTY_CD,
   SEM_FULL_C,SEM_FULL_D, SEM_FULL_AB,
   SEM_PRIORITY_C, SEM_PRIORITY_D};

void test() {
  /*
  printq(queue_A);
  push_priority(queue_A, 12, 0);
  printq(queue_A);
  push_priority(queue_A, 13, 0);
  printq(queue_A);
  push_priority(queue_A, 14, 1);
  printq(queue_A);
  tmp = pop(queue_A);
  printq(queue_A);
  printf("%d %d\n", tmp.id, tmp.priority);
  tmp = pop(queue_A);
  printq(queue_A);
  printf("%d %d\n", tmp.id, tmp.priority);

  //shmdt(queue_A);
  */
}

struct Queue* shm_init(key_t key){
  int shm_id;
  struct Queue *queue;
  shm_id = shmget(key, sizeof(struct Queue), 0666 | IPC_CREAT);
  if (shm_id == -1) {
    perror("shm_init: shmget error\n");
    exit(1);
  }

  queue = (struct Queue*)shmat(shm_id, NULL, 0);
  if (queue == NULL) {
    perror("shm_init: shmat error\n");
    exit(1);
  }

  return queue;
}

void shm_clean(key_t key) {
  int shm_id;
  struct Queue *queue;
  struct shmid_ds *shm_desc;
  shm_id = shmget(key, sizeof(struct Queue), 0666 | IPC_CREAT);
  if (shm_id == -1) {
    perror("shm_clean: shmget error\n");
    exit(1);
  }

  if (shmctl(shm_id, IPC_RMID, shm_desc) == -1) {
    perror("shm_clean: shmctl error\n");
    exit(1);
  }
}

void queue_init() {
  struct Queue *queue_A, *queue_B, *queue_C, *queue_D;

  queue_A = shm_init(SHM_KEY_A);
  init(queue_A, Q_SIZE);
  shmdt(queue_A);
  queue_B = shm_init(SHM_KEY_B);
  init(queue_B, Q_SIZE);
  shmdt(queue_B);
  queue_C = shm_init(SHM_KEY_C);
  init(queue_C, Q_SIZE);
  shmdt(queue_C);
  queue_D = shm_init(SHM_KEY_D);
  init(queue_D, Q_SIZE);
  shmdt(queue_D);

  sem_init(SEM_KEY, MUTEX_A, 1);
  sem_init(SEM_KEY, MUTEX_B, 1);
  sem_init(SEM_KEY, MUTEX_C, 1);
  sem_init(SEM_KEY, MUTEX_D, 1);
  sem_init(SEM_KEY, SEM_EMPTY_A, 0);
  sem_init(SEM_KEY, SEM_EMPTY_B, 0);
  sem_init(SEM_KEY, SEM_EMPTY_CD, 0);
  sem_init(SEM_KEY, SEM_FULL_C, Q_SIZE);
  sem_init(SEM_KEY, SEM_FULL_D, Q_SIZE);
  sem_init(SEM_KEY, SEM_FULL_AB, Q_SIZE);
  sem_init(SEM_KEY, SEM_PRIORITY_C, 0);
  sem_init(SEM_KEY, SEM_PRIORITY_D, 0);
}

void clean_up() {
  sem_destroy_all(SEM_KEY);

  shm_clean(SHM_KEY_A);
  shm_clean(SHM_KEY_B);
  shm_clean(SHM_KEY_C);
  shm_clean(SHM_KEY_D);
}

void process_U0(key_t key) {

}

void process_U1(key_t key_in, key_t key_out) {

}

void process_U2(key_t key) {

}


int main(int argc, char const *argv[]) {
  queue_init();

  clean_up();
  return 0;
}
