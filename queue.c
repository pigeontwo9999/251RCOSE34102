#include <stdio.h>
#include "queue.h"

void queue_init(Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

int queue_empty(const Queue *q) {
    return q->count == 0;
}

int queue_full(const Queue *q) {
    return q->count == QUEUE_SIZE;
}

void queue_push(Queue *q, Process *proc) {
    if (queue_full(q)) {
        printf("Queue is full\n");
        return;
    }
    q->processlist[q->tail] = proc;
    q->tail = (q->tail + 1) % QUEUE_SIZE;
    q->count++;
}

Process *queue_pop(Queue *q) {
    if (queue_empty(q)) {
        printf("Queue has No elements\n");
        return NULL;
    }
    Process *proc = q->processlist[q->head];
    q->head = (q->head + 1) % QUEUE_SIZE;
    q->count--;
    return proc;
}

int queue_remove(Queue *q, Process *proc) {
    if (queue_empty(q)) return 0;
    int removed = 0;
    int idx = q->head;
    for (int i = 0; i < q->count; i++) {
        if (q->processlist[idx] == proc) { // proc 찾기
            removed = 1;
        }
        if (removed && i != q->count - 1) { // 당기고
            int next = (idx + 1) % QUEUE_SIZE;
            q->processlist[idx] = q->processlist[next];
        }
        idx = (idx + 1) % QUEUE_SIZE;
    }
    
    if (removed == 0) return 0;
    
    q->tail = (q->tail - 1 + QUEUE_SIZE) % QUEUE_SIZE;
    q->count--;
    
    return 1;
}



Process *queue_pop_shortest(Queue *q) {
    if (queue_empty(q)) return NULL;
    // 짧은거 찾고
    int zz = q->head;
    int idx = q->head;
    for (int i = 1; i < q->count; i++) {
        idx = (q->head + i) % QUEUE_SIZE;
        if (q->processlist[idx]->remaining_time < q->processlist[zz]->remaining_time) {
            zz = idx;
        }
    }
    
    Process *selected = q->processlist[zz];
    // Shift subsequent elements
    int cur = zz;
    for (int i = 0; i < q->count - 1; i++) {
        int next = (cur + 1) % QUEUE_SIZE;
        q->processlist[cur] = q->processlist[next];
        cur = next;
    }
    q->tail = (q->tail - 1 + QUEUE_SIZE) % QUEUE_SIZE;
    q->count--;
    return selected;
}

Process *queue_pop_highest_priority(Queue *q) {
    if (queue_empty(q)) return NULL;
    
    int zz = q->head;
    int idx = q->head;
    for (int i = 1; i < q->count; i++) {
        idx = (q->head + i) % QUEUE_SIZE;
        if (q->processlist[idx]->priority < q->processlist[zz]->priority) {
            zz = idx;
        }
    }
    // Remove the selected process
    Process *selected = q->processlist[zz];
    // Shift subsequent elements
    int cur = zz;
    for (int i = 0; i < q->count - 1; i++) {
        int next = (cur + 1) % QUEUE_SIZE;
        q->processlist[cur] = q->processlist[next];
        cur = next;
    }
    q->tail = (q->tail - 1 + QUEUE_SIZE) % QUEUE_SIZE;
    q->count--;
    return selected;
}


