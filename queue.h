#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"
#include "global.h"
#define QUEUE_SIZE 100


typedef struct {
    Process *processlist[QUEUE_SIZE];
    int head;
    int tail;
    int count;
} Queue;


void queue_init(Queue *q);
int queue_empty(const Queue *q);
int queue_full(const Queue *q);
void queue_push(Queue *q, Process *proc);
Process *queue_pop(Queue *q);

int queue_remove(Queue *q, Process *proc);

Process *queue_pop_shortest(Queue *q);
Process *queue_pop_highest_priority(Queue *q);

#endif