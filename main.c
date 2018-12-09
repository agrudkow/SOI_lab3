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
   SEM_FULL_C,SEM_FULL_D, SEM_FULL_AB};

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
    perror("shm_init(%d): shmget error\n", key);
    exit(1);
  }

  queue = (struct Queue*)shmat(shm_id, NULL, 0);
  if (queue == NULL) {
    perror("shm_init(%d): shmat error\n", key);
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
    perror("shm_clean(%d): shmget error\n", key);
    exit(1);
  }

  if (shmctl(shm_id, IPC_RMID, shm_desc) == -1) {
    perror("shm_clean(%d): shmctl error\n", key);
    exit(1);
  }
}

int main(int argc, char const *argv[]) {
  struct Queue *queue_A;


  queue_A = shm_init(SHM_KEY_A);
  init(queue_A, Q_SIZE);


  shm_clean(SHM_KEY_A);

  return 0;
}
