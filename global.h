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

#define MAX_TIME MAX_ARRIVAL_TIME + MAX_PROCESSES * (MAX_BURST_TIME + MAX_IO * MAX_IO_BURST_TIME)

#define QUEUE_SIZE 100

#define RR_Q 4

typedef struct {
    int arrival_time;
    int io_burst;
    int io_time;
    int completed;
} IO;

typedef enum {
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} State;


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

    State status;

} Process;


typedef struct {
    Process *processlist[QUEUE_SIZE];
    int head;
    int tail;
    int count;
} Queue;

typedef struct {
    int pid;
    int start;
    int end;
} Chart;

typedef struct {
    char *algorithm;
    float average_waiting_time;
    float average_turnaround_time;
    float cpu_utilization;

    int max_wait;
    int min_wait;
    int max_turn;
    int min_turn;
} evaluation;



extern Process processes[MAX_PROCESSES];
extern int num_processes;
extern int completed;
extern Process current_processes[MAX_PROCESSES];
extern Queue ready_queue;
extern Queue waiting_queue;
extern Chart gantt[MAX_TIME];
extern int gantt_idx;

extern evaluation table[7];
#endif