#ifndef GLOBAL_H
#define GLOBAL_H


#define MAX_PROCESSES 10
#define MAX_IO 3

#define MAX_ARRIVAL_TIME 50
#define MAX_PRIORITY 10
#define MAX_BURST_TIME 20
#define MIN_BURST_TIME 5

#define MAX_IO_BURST_TIME 5
#define MIN_IO_BURST_TIME 2

#define QUEUE_SIZE 100

typedef struct {
    int arrival_time;
    int io_burst;
    int io_time;
    int completed;
} IO;

typedef struct {
    int pid;
    int arrival_time;
    int cpu_burst;
    int remaining_time;
    int priority;
    
    int io_count;
    IO io_list[MAX_IO];
    int io_idx;

    int start_time;
    int finish_time;

    int waiting_time;
    int turnaround_time;
    int response_time;
    int complete_time;

} Process;


typedef struct {
    Process *processlist[QUEUE_SIZE];
    int head;
    int tail;
    int count;
} Queue;



extern Process processes[MAX_PROCESSES];
extern int num_processes;
extern int completed_processes;
extern Process current_processes[MAX_PROCESSES];
extern Queue ready_queue;
extern Queue waiting_queue;

#endif