#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#define Q_SIZE    10

struct Applicant {
  int id, priority;
};

struct Queue {
  struct Applicant buffor[Q_SIZE];
  // back points on a place ready to be filled, except the feckt when queue is empty
  // front points on a first element in a queue
  int front, back, size;
};

void printq(struct Queue *queue);
void init(struct Queue *queue, int s);
void push(struct Queue *queue, int id, int priority);
void push_priority(struct Queue *queue,int id, int priority);
struct Applicant pop(struct Queue *queue);

#endif
