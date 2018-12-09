#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
   SEM_FULL_A, SEM_FULL_B, SEM_EMPTY_AB,
   SEM_EMPTY_C,SEM_EMPTY_D, SEM_FULL_CD,
   SEM_PRIORITY_C, SEM_PRIORITY_D,
   MUTEX_PRINT};

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

  struct Queue *queue_A;
  queue_A = shm_init(SHM_KEY_A);
  push_priority(queue_A, 12, 0);
  push_priority(queue_A, 13, 0);
  push_priority(queue_A, 14, 0);
  push_priority(queue_A, 15, 1);
  pop(queue_A);
  push_priority(queue_A, 16, 0);
  push_priority(queue_A, 17, 1);
  push_priority(queue_A, 18, 1);
  pop(queue_A);

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

void print_all() {
  struct Queue *queue;

  down(SEM_KEY, MUTEX_PRINT);
  printf("\033c");
  printf("      ");
  queue = shm_init(SHM_KEY_A);
  printq(queue);
  shmdt(queue);
  printf("[U1_1]");
  queue = shm_init(SHM_KEY_C);
  printq(queue);
  shmdt(queue);
  printf("\n");
  printf("     /");
  for (size_t i = 0; i < 66; i++) printf(" ");
  printf("\\\n");
  printf("    /");
  for (size_t i = 0; i < 68; i++) printf(" ");
  printf("\\\n");
  printf("[U0]");
  for (size_t i = 0; i < 70; i++) printf(" ");
  printf("[U2]\n");
  printf("    \\");
  for (size_t i = 0; i < 68; i++) printf(" ");
  printf("/\n");
  printf("     \\");
  for (size_t i = 0; i < 66; i++) printf(" ");
  printf("/\n");
  printf("      ");
  queue = shm_init(SHM_KEY_B);
  printq(queue);
  shmdt(queue);
  printf("[U1_2]");
  queue = shm_init(SHM_KEY_D);
  printq(queue);
  shmdt(queue);
  printf("\n");
  up(SEM_KEY, MUTEX_PRINT);
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

void init_all() {
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
  sem_init(SEM_KEY, SEM_FULL_A, 0);
  sem_init(SEM_KEY, SEM_FULL_B, 0);
  sem_init(SEM_KEY, SEM_EMPTY_AB, 2 * Q_SIZE);
  sem_init(SEM_KEY, SEM_EMPTY_C, Q_SIZE);
  sem_init(SEM_KEY, SEM_EMPTY_D, Q_SIZE);
  sem_init(SEM_KEY, SEM_FULL_CD, 0);
  sem_init(SEM_KEY, SEM_PRIORITY_C, 0);
  sem_init(SEM_KEY, SEM_PRIORITY_D, 0);
  sem_init(SEM_KEY, MUTEX_PRINT, 1);
}
// Clean up all the allocated memory from shared memory and semaphores
void clean_up() {
  sem_destroy_all(SEM_KEY);

  shm_clean(SHM_KEY_A);
  shm_clean(SHM_KEY_B);
  shm_clean(SHM_KEY_C);
  shm_clean(SHM_KEY_D);
}

void process_U0() {
  struct Queue *queue;
  int applicant_id = 0, applicant_priority, which_queue;
  srand(time(NULL));

  for (;;) {
    down(SEM_KEY, SEM_EMPTY_AB);
    which_queue = (int)(rand()) % 2; // 2 - number of queues betwen U0 and U1
    if (which_queue == 0 && sem_get_value(SEM_KEY, SEM_FULL_A) != Q_SIZE) {
      applicant_priority = rand() % 2;
      down(SEM_KEY, MUTEX_A);
      queue = shm_init(SHM_KEY_A);
      push(queue, applicant_id, applicant_priority);
      shmdt(queue);
      up(SEM_KEY, MUTEX_A);
      up(SEM_KEY, SEM_FULL_A);
      applicant_id++;
    } else if (which_queue == 1 && sem_get_value(SEM_KEY, SEM_FULL_B) != Q_SIZE) {
      applicant_priority = rand() % 2;
      down(SEM_KEY, MUTEX_B);
      queue = shm_init(SHM_KEY_B);
      push(queue, applicant_id, applicant_priority);
      shmdt(queue);
      up(SEM_KEY, MUTEX_B);
      up(SEM_KEY, SEM_FULL_B);
      applicant_id++;
    } else
      up(SEM_KEY, SEM_EMPTY_AB);

    print_all();
  }
}

void process_U1_1() {
  struct Queue *queue;
  struct Applicant applicant;
  int which_queue;

  for (;;) {
    down(SEM_KEY, SEM_FULL_A);
    down(SEM_KEY, MUTEX_A);
    queue = shm_init(SHM_KEY_A);
    applicant = pop(queue);
    shmdt(queue);
    up(SEM_KEY, MUTEX_A);
    up(SEM_KEY, SEM_EMPTY_AB);

    print_all();
    sleep(1000);

    down(SEM_KEY, SEM_EMPTY_C);
    down(SEM_KEY, MUTEX_C);
    queue = shm_init(SHM_KEY_C);
    push_priority(queue, applicant.id, applicant.priority);
    shmdt(queue);
    if (applicant.priority == 1)
      up(SEM_KEY, SEM_PRIORITY_C);
    up(SEM_KEY, MUTEX_C);
    up(SEM_KEY, SEM_FULL_CD);

    print_all();
  }
}

void process_U1_2() {
  struct Queue *queue;
  struct Applicant applicant;

  for (;;) {
    down(SEM_KEY, SEM_FULL_B);
    down(SEM_KEY, MUTEX_B);
    queue = shm_init(SHM_KEY_B);
    applicant = pop(queue);
    shmdt(queue);
    up(SEM_KEY, MUTEX_B);
    up(SEM_KEY, SEM_EMPTY_AB);

    print_all();
    sleep(1000);

    down(SEM_KEY, SEM_EMPTY_D);
    down(SEM_KEY, MUTEX_D);
    queue = shm_init(SHM_KEY_D);
    push_priority(queue, applicant.id, applicant.priority);
    shmdt(queue);
    if (applicant.priority == 1)
      up(SEM_KEY, SEM_PRIORITY_D);
    up(SEM_KEY, MUTEX_D);
    up(SEM_KEY, SEM_FULL_CD);

    print_all();
  }
}

void process_U2() {
  struct Queue *queue;
  struct Applicant applicant;
  int priority_num;

  for (;;) {
    down(SEM_KEY, SEM_FULL_CD);
    if (sem_get_value(SEM_KEY, SEM_PRIORITY_C) >= sem_get_value(SEM_KEY, SEM_PRIORITY_D)
    && sem_get_value(SEM_KEY, SEM_EMPTY_C) != Q_SIZE) {
      down(SEM_KEY, MUTEX_C);
      queue = shm_init(SHM_KEY_C);
      applicant = pop(queue);
      shmdt(queue);
      if (applicant.priority == 1)
        down(SEM_KEY, SEM_PRIORITY_C);
      up(SEM_KEY, MUTEX_C);
      up(SEM_KEY, SEM_EMPTY_C);
    } else {
      down(SEM_KEY, MUTEX_D);
      queue = shm_init(SHM_KEY_D);
      applicant = pop(queue);
      shmdt(queue);
      if (applicant.priority == 1)
        down(SEM_KEY, SEM_PRIORITY_D);
      up(SEM_KEY, MUTEX_D);
      up(SEM_KEY, SEM_EMPTY_D);
    }
    print_all();
    sleep(2000);
  }
}

int run_tasks() {
  int pid;

  pid = fork();
  if (pid == 1) {
    process_U0();
    return 1;
  }

  pid = fork();
  if (pid == 1) {
    process_U1_1();
    return 1;
  }
  pid = fork();
  if (pid == 0) {
    process_U1_2();
    return 1;
  }
  
  pid = fork();
  if (pid == 0) {
    process_U2();
    return 1;
  }

  return 0;
}


int main(int argc, char const *argv[]) {
  init_all();

  if (run_tasks())
    return 0;

  clean_up();
  return 0;
}
