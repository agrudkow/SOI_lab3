#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void printq(struct Queue *queue) {
  if (queue->actual_size == 0) {
    for (size_t i = 0; i < 10; i++) printf("***");
    return;
  }
  for (int i = queue->back; i != queue->front ; i = ((i + 1) % queue->size)) {
    //printf("%d %d %d\n",queue->back, queue-> front, i);
    printf("***");
  }
  for (int j = ((queue->back - 1) % queue->size) ; j != queue->front; j = ((j - 1) % queue->size)) {
    printf("%3d", queue->buffor[j].id);
  }
  printf("%3d", queue->buffor[queue->front].id);
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
  queue->actual_size++;
  ++(queue->back) % queue->size;
}
//Pushing applicant in to the end of the queue with priority
void push_priority(struct Queue *queue, int id, int priority) {
  if (queue->front == queue->back) {
    push(queue, id, priority);
    return;
  }

  queue->actual_size++;
  queue->back = (queue->back + 1) % queue->size;

  for (int i = ((queue->back - 2) % queue->size); i != queue->front;  i = ((i - 1) % queue->size)) {
    if (queue->buffor[i].priority >= priority) {
      queue->buffor[i + 1].id = id;
      queue->buffor[i + 1].priority = priority;
      return;
    }
    queue->buffor[i + 1].id = queue->buffor[i].id;
    queue->buffor[i + 1].priority = queue->buffor[i].priority;
  }
  if (queue->buffor[queue->front + 1].priority < priority){
    queue->buffor[queue->front + 1].id = queue->buffor[queue->front].id;
    queue->buffor[queue->front + 1].priority = queue->buffor[queue->front].priority;
    queue->buffor[queue->front].id = id;
    queue->buffor[queue->front].priority = priority;
  } else {
    queue->buffor[queue->front + 1].id = id;
    queue->buffor[queue->front + 1].priority = priority;
  }
  return;
}
//Poping applicant from the start of the queue
struct Applicant pop(struct Queue *queue){
    queue->actual_size--;
  return (queue->buffor[(queue->front)++]);
}
