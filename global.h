#ifndef GLOBAL_H
#define GLOBAL_H

#include "queue.h"
#include "process.h"

extern Process processes[MAX_PROCESSES];
extern int num_processes = 0;
extern int completed_processes = 0;
extern Process current_processes[MAX_PROCESSES];
extern Queue ready_queue;
extern Queue waiting_queue;

#endif