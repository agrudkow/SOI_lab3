#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define RESET "\x1B[0m"

int mod_q (int a, int b){
  int result = a % b;
  if (result >= 0) {
    return result;
  } else {
    return (b + result);
  }
}

void printq(struct Queue *queue) {
  //printf("%d %d %d \n",  queue->front, queue->back, queue->actual_size);
  if (queue->actual_size == 0) {
    for (size_t i = 0; i < 10; i++) printf("***");
    return;
  }
  for (int i = 0; i < (queue->size - queue->actual_size) ; ++i){
    printf("***");
  }
  int j = queue->back;
  j = mod_q(j - 1, queue->size);
  for (int k = 0; k < (queue->actual_size); k++) {
    if (queue->buffor[j].priority == 1)
      printf(RED "%3d" RESET, queue->buffor[j].id);
    else
      printf(GREEN "%3d" RESET, queue->buffor[j].id);
    j = mod_q(j - 1, queue->size);
  }
}

//Initializing a gueue with given adrress to buffor
void init(struct Queue *queue, int s) {
  queue->front = queue->back = queue->actual_size = 0;
  queue->size = s;
}

//Pushing applicant in to the end of the queue without priority
void push(struct Queue *queue, int id, int priority) {
  queue->buffor[queue->back].id = id;
  queue->buffor[queue->back].priority = priority;
  queue->actual_size += 1;
  queue->back = (queue->back + 1) % queue->size;
}
//Pushing applicant in to the end of the queue with priority
void push_priority(struct Queue *queue, int id, int priority) {
  if (queue->actual_size == 0) {
    push(queue, id, priority);
    return;
  }
  int j;
  j = mod_q(queue->back - 1, queue->size);

  for (int i = 0; i < queue->actual_size; i++) {
    if (queue->buffor[j].priority >= priority ) {
      queue->buffor[((j + 1) % queue->size)].id = id;
      queue->buffor[((j + 1) % queue->size)].priority = priority;
      queue->actual_size += 1;
      queue->back = (queue->back + 1) % queue->size;
      return;
    }
    queue->buffor[((j + 1) % queue->size)].id = queue->buffor[j].id;
    queue->buffor[((j + 1) % queue->size)].priority = queue->buffor[j].priority;
    j = mod_q(j - 1, queue->size);
  }
  queue->buffor[j + 1].id = id;
  queue->buffor[j + 1].priority = priority;
  queue->actual_size += 1;
  queue->back = (queue->back + 1) % queue->size;
  return;
}
//Poping applicant from the start of the queue
struct Applicant pop(struct Queue *queue){
    struct Applicant first;
    queue->actual_size -= 1;
    first = queue->buffor[queue->front];
    queue->front = ((queue->front + 1) % queue->size);
  return first;
}
