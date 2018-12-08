#include <stdio.h>
#include "queue.h"

//extern struct Applicant;

//Initializing a gueue with given adrress to buffor
void init(struct Queue *queue, struct Applicant *buff, int s) {
  queue->buffor = buff;
  queue->front = queue->back = 0;
  queue->size = s;
}

//Pushing applicant in to the end of the queue without priority
void push(struct Queue *queue, int id, int priority) {
  queue->buffor[queue->back].id = id;
  queue->buffor[queue->back].priority = priority;
  ++(queue->back) % queue->size;
}
//Pushing applicant in to the end of the queue with priority
void push_priority(struct Queue *queue,int id, int priority) {
  for (int i = (queue->back - 1) % queue->size; i != queue->front; --i % queue->size) {
    if (queue->buffor[i].priority == priority) {
      queue->buffor[i + 1].id = id;
      queue->buffor[i + 1].priority = priority;
      ++(queue->back) % queue->size;
      return;
    }
    queue->buffor[i + 1].id = queue->buffor[i].id;
    queue->buffor[i + 1].priority = queue->buffor[i].priority;
  }
  queue->buffor[queue->front + 1].id = queue->buffor[queue->front].id;
  queue->buffor[queue->front + 1].priority = queue->buffor[queue->front].priority;
  queue->buffor[queue->front].id = id;
  queue->buffor[queue->front].priority = priority;
  ++(queue->back) % queue->size;

  return;
}
//Poping applicant from the start of the queue
struct Applicant* pop(struct Queue *queue){
  return &(queue->buffor[(queue->front)--]);
}
