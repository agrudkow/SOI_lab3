#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "queue.h"

#define SHM_KEY_A 11111
#define SHM_KEY_B 11112
#define SHM_KEY_C 11113
#define SHM_KEY_D 11114

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
    perror("shmget error\n");
    exit(1);
  }

  queue = (struct Queue*)shmat(shm_id, NULL, 0);
  if (queue == NULL) {
    perror("shmat error\n");
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
    perror("shmget error\n");
    exit(1);
  }

  if (shmctl(shm_id, IPC_RMID, shm_desc) == -1) {
    perror("shmat error\n");
    exit(1);
  }
}


int main(int argc, char const *argv[]) {
  struct Applicant tmp;
  struct Queue *queue_A;


  queue_A = shm_init(SHM_KEY_A);
  init(queue_A, Q_SIZE);


  shm_clean(SHM_KEY_A);

  return 0;
}
