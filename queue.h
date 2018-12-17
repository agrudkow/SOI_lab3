#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#define Q_SIZE    10

struct Applicant {
  int id, priority;
};

struct Queue {
  struct Applicant buffor[Q_SIZE];
  // back - points on a place ready to be filled, except the feckt when queue is empty
  // front - points on a first element in a queue
  int front, back, size, actual_size;
};

int mod_q(int a, int b);                                     //Custom modulo function
void printq(struct Queue *queue);                            //Print selected queue
void init(struct Queue *queue, int s);                       //Initializing a gueue with given adrress to buffor
void push(struct Queue *queue, int id, int priority);        //Pushing applicant in to the end of the queue without priority
void push_priority(struct Queue *queue,int id, int priority);//Pushing applicant in to the end of the queue with priority
struct Applicant pop(struct Queue *queue);                   //Poping applicant from the start of the queue

#endif
