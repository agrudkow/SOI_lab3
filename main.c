#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include "queue.h"
//#include "sem.h"

#define SHM_KEY_A   10001
#define SHM_KEY_B   10002
#define SHM_KEY_C   10003
#define SHM_KEY_D   10004
#define SEM_KEY     20001

#define NUM_OF_SEMS 13
#define NUM_OF_APPLICANTS 50

pthread_t U0_thread, U1_1_thread, U1_2_thread, U2_thread;

int semid;
struct sembuf sb = {0, 0, 0};

enum { MUTEX_A, MUTEX_B, MUTEX_C, MUTEX_D,
   SEM_FULL_A, SEM_FULL_B, SEM_EMPTY_AB,
   SEM_EMPTY_C,SEM_EMPTY_D, SEM_FULL_CD,
   SEM_PRIORITY_C, SEM_PRIORITY_D,
   MUTEX_PRINT};

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

  down(MUTEX_PRINT, sb);
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
  up( MUTEX_PRINT, sb);
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

  semid = sem_id(SEM_KEY);

  sem_init(MUTEX_A, 1);
  sem_init(MUTEX_B, 1);
  sem_init(MUTEX_C, 1);
  sem_init(MUTEX_D, 1);
  sem_init(SEM_FULL_A, 0);
  sem_init(SEM_FULL_B, 0);
  sem_init(SEM_EMPTY_AB,2 * Q_SIZE);
  sem_init(SEM_EMPTY_C, Q_SIZE);
  sem_init(SEM_EMPTY_D, Q_SIZE);
  sem_init(SEM_FULL_CD, 0);
  sem_init(SEM_PRIORITY_C, 0);
  sem_init(SEM_PRIORITY_D, 0);
  sem_init(MUTEX_PRINT, 1);
}
// Clean up all the allocated memory from shared memory and semaphores
void clean_up() {

  sem_destroy_all();

  shm_clean(SHM_KEY_A);
  shm_clean(SHM_KEY_B);
  shm_clean(SHM_KEY_C);
  shm_clean(SHM_KEY_D);

  pthread_kill(U0_thread, 9);
  pthread_kill(U1_1_thread, 9);
  pthread_kill(U1_2_thread, 9);
  pthread_kill(U2_thread, 9);
}

void* process_U0() {
  struct Queue *queue;
  int applicant_id = 1, applicant_priority, which_queue;
  srand(time(NULL));

  while (applicant_id <= NUM_OF_APPLICANTS) {
    down(SEM_EMPTY_AB, sb);
    which_queue = (int)(rand()) % 2; // 2 - number of queues betwen U0 and U1
    if (which_queue == 0 && sem_get_value(SEM_FULL_A) != Q_SIZE) {
      applicant_priority = rand() % 2;
      down(MUTEX_A, sb);
      queue = shm_init(SHM_KEY_A);
      push(queue, applicant_id, applicant_priority);
      shmdt(queue);
      up(MUTEX_A, sb);
      up(SEM_FULL_A, sb);
      applicant_id++;
    } else if (which_queue == 1 && sem_get_value(SEM_FULL_B) != Q_SIZE) {
      applicant_priority = rand() % 2;
      down(MUTEX_B, sb);
      queue = shm_init(SHM_KEY_B);
      push(queue, applicant_id, applicant_priority);
      shmdt(queue);
      up(MUTEX_B, sb);
      up(SEM_FULL_B, sb);
      applicant_id++;
    } else
      up(SEM_EMPTY_AB, sb);

    print_all();
    //sleep(1);
  }
}

void* process_U1_1() {
  sleep(5);
  struct Queue *queue;
  struct Applicant applicant;
  int which_queue;

  for (;;) {
    down(SEM_FULL_A, sb);
    down(MUTEX_A, sb);
    queue = shm_init(SHM_KEY_A);
    applicant = pop(queue);
    shmdt(queue);
    up(MUTEX_A, sb);
    up(SEM_EMPTY_AB, sb);

    print_all();
    sleep(1);

    down(SEM_EMPTY_C, sb);
    down(MUTEX_C, sb);
    queue = shm_init(SHM_KEY_C);
    push_priority(queue, applicant.id, applicant.priority);
    shmdt(queue);
    if (applicant.priority == 1)
      up(SEM_PRIORITY_C, sb);
    up(MUTEX_C, sb);
    up(SEM_FULL_CD, sb);

    print_all();
  }
}

void* process_U1_2() {
  sleep(5);
  struct Queue *queue;
  struct Applicant applicant;

  for (;;) {
    down(SEM_FULL_B, sb);
    down(MUTEX_B, sb);
    queue = shm_init(SHM_KEY_B);
    applicant = pop(queue);
    shmdt(queue);
    up(MUTEX_B, sb);
    up(SEM_EMPTY_AB, sb);

    print_all();
    sleep(1);

    down(SEM_EMPTY_D, sb);
    down(MUTEX_D, sb);
    queue = shm_init(SHM_KEY_D);
    push_priority(queue, applicant.id, applicant.priority);
    shmdt(queue);
    if (applicant.priority == 1)
      up(SEM_PRIORITY_D, sb);
    up(MUTEX_D, sb);
    up(SEM_FULL_CD, sb);

    print_all();
  }
}

void* process_U2() {
  sleep(15);
  struct Queue *queue;
  struct Applicant applicant;
  int priority_num, applicant_num = 0;

  for (int i = 0; i < 100; i++) {
    down(SEM_FULL_CD, sb);
    if (sem_get_value(SEM_PRIORITY_C) > sem_get_value(SEM_PRIORITY_D)
    && sem_get_value(SEM_PRIORITY_C) > 0) {
      down(MUTEX_C, sb);
      queue = shm_init(SHM_KEY_C);
      applicant = pop(queue);
      shmdt(queue);
      if (applicant.priority == 1)
        down(SEM_PRIORITY_C, sb);
      up(MUTEX_C, sb);
      up(SEM_EMPTY_C, sb);
      applicant_num++;
    } else if (sem_get_value(SEM_PRIORITY_D) > sem_get_value(SEM_PRIORITY_C)
    && sem_get_value(SEM_PRIORITY_D) > 0) {
      down(MUTEX_D, sb);
      queue = shm_init(SHM_KEY_D);
      applicant = pop(queue);
      shmdt(queue);
      if (applicant.priority == 1)
        down(SEM_PRIORITY_D, sb);
      up(MUTEX_D, sb);
      up(SEM_EMPTY_D, sb);
      applicant_num++;
    } else if (sem_get_value(SEM_EMPTY_C) > sem_get_value(SEM_EMPTY_D)) {
      down(MUTEX_D, sb);
      queue = shm_init(SHM_KEY_D);
      applicant = pop(queue);
      shmdt(queue);
      if (applicant.priority == 1)
        down(SEM_PRIORITY_D, sb);
      up(MUTEX_D, sb);
      up(SEM_EMPTY_D, sb);
      applicant_num++;
    } else {
      down(MUTEX_C, sb);
      queue = shm_init(SHM_KEY_C);
      applicant = pop(queue);
      shmdt(queue);
      if (applicant.priority == 1)
        down(SEM_PRIORITY_C, sb);
      up(MUTEX_C, sb);
      up(SEM_EMPTY_C, sb);
      applicant_num++;
    }
    print_all();
    sleep(1);
    if (applicant_num == NUM_OF_APPLICANTS)
      clean_up();
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

void simulate() {
  pthread_create(&U0_thread, NULL, &process_U0, NULL);
  pthread_create(&U1_1_thread, NULL, &process_U1_1, NULL);
  pthread_create(&U1_2_thread, NULL, &process_U1_2, NULL);
  pthread_create(&U2_thread, NULL, &process_U2, NULL);

  pthread_join(U0_thread, NULL);
  pthread_join(U1_1_thread, NULL);
  pthread_join(U1_2_thread, NULL);
  pthread_join(U2_thread, NULL);
}


int main(int argc, char const *argv[]) {
  init_all();
  simulate();
  clean_up();
  return 0;
}
