
#include <stdio.h>
#include <stdlib.h>

#include "process.h"

int pid_list[MAX_PROCESSES];

void pid_list_config(){
    for (int i = 0; i < MAX_PROCESSES; i++) {
        pid_list[i] = i + 1;
    }

    for (int i = MAX_PROCESSES; i > 1; i--) {
        int j = rand() % i;           
        int tmp = pid_list[i-1];
        pid_list[i-1] = pid_list[j];
        pid_list[j] = tmp;
    }
}


void create_process(int pid, int arrival_time, int cpu_burst, int priority, int io_count) {
    if (num_processes >= MAX_PROCESSES) {
        printf("PROCESSES are full\n");
        return;
    }
    Process *p = &processes[num_processes];


    p->pid = pid;
    

    p->arrival_time     = arrival_time;
    p->cpu_burst        = cpu_burst;
    p->remaining_time   = cpu_burst;
    p->priority         = priority;

    p->io_count = io_count;
    p->start_time       = -1;
    p->finish_time      = 0;

    p->waiting_time     = 0;
    p->turnaround_time  = 0;
    p->response_time    = -1;
    p->complete_time    = 0;
    num_processes++;
}

void create_io(Process *p, int *arrival_time, int *io_burst){


    for (int i=0;i<p->io_count; i++){
        p->io_list[i].arrival_time = arrival_time[i];
        p->io_list[i].io_burst = io_burst[i];
        p->io_list[i].io_time = 0;
        p->io_list[i].completed = 0;
    }
    
}

void create_process_random(){

    Process *p =  &processes[num_processes];

    int pid = pid_list[num_processes];
    int arrival_time = rand() % MAX_ARRIVAL_TIME;
    int burst_time = (rand() % (MAX_BURST_TIME - MIN_BURST_TIME)) + MIN_BURST_TIME;
    int priority = rand() % MAX_PRIORITY + 1;
    int io_count = rand() % MAX_IO;

    create_process(pid, arrival_time, burst_time, priority, io_count);

    int io_arrive[MAX_IO] = {0};
    int io_burst[MAX_IO] = {0};

    int zz = burst_time / io_count;

    for (int i=0;i<io_count;i++){

        // io 요청 시간이 안겹치려면? 구간마다 1개씩 집어넣자
        int io_random_arrive;
        while (1){
            io_random_arrive = (rand() % zz) + (zz * i);
            if (io_arrive == 0) continue;
            break;
        }
        
        io_arrive[i] = io_random_arrive;
        io_burst[i] = (rand() % (MAX_IO_BURST_TIME  - MIN_IO_BURST_TIME)) + MIN_IO_BURST_TIME;
    }

    create_io(p, io_arrive, io_burst);
    p->io_idx = 0;

}


void create_process_input(){
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int io_count;

    Process *p =  &processes[num_processes];
    
    printf("Enter PID: ");
    scanf("%d", &pid);

    
    printf("Enter arrival time (0 – %d): ", MAX_ARRIVAL_TIME);
    scanf("%d", &arrival_time);
    while (arrival_time < 0 || arrival_time > MAX_ARRIVAL_TIME) {
        printf("Invalid. Enter arrival time (0 – %d): ", MAX_ARRIVAL_TIME);
        scanf("%d", &arrival_time);
    }

    
    printf("Enter CPU burst time (1 – %d): ", MAX_BURST_TIME);
    scanf("%d", &burst_time);
    while (burst_time < 1 || burst_time > MAX_BURST_TIME) {
        printf("Invalid. Enter CPU burst time (1 – %d): ", MAX_BURST_TIME);
        scanf("%d", &burst_time);
    }

    
    printf("Enter priority (1 – %d): ", MAX_PRIORITY);
    scanf("%d", &priority);
    while (priority < 1 || priority > MAX_PRIORITY) {
        printf("Invalid. Enter priority (1 – %d): ", MAX_PRIORITY);
        scanf("%d", &priority);
    }

    
    printf("Enter IO count (0 – %d): ", MAX_IO);
    scanf("%d", &io_count);
    while (io_count < 0 || io_count > MAX_IO) {
        printf("Invalid. Enter IO count (0 – %d): ", MAX_IO);
        scanf("%d", &io_count);
    }

    create_process(pid, arrival_time, burst_time, priority, io_count);

    int io_arrive[MAX_IO];
    int io_burst[MAX_IO];

    for (int i = 0; i < io_count; i++) {
        printf("Enter IO %d request time (0 – %d): ", i,
               burst_time);
        scanf("%d", &io_arrive[i]);
        while (io_arrive[i] < 0 || io_arrive[i] > burst_time) {
            printf(" Invalid. Enter IO %d request time (0 – %d): ",
                   i, burst_time);
            scanf("%d", &io_arrive[i]);
        }

        printf("Enter IO %d burst time (1 – %d): ", i,
               MAX_IO_BURST_TIME);
        scanf("%d", &io_burst[i]);
        while (io_burst[i] < 1 || io_burst[i] > MAX_IO_BURST_TIME) {
            printf(" Invalid. Enter IO %d burst time (1 – %d): ",
                   i, MAX_IO_BURST_TIME);
            scanf("%d", &io_burst[i]);
        }
    }

    create_io(p, io_arrive, io_burst);
}


void print_process(Process *p){
    
    printf("\n=============================================\n");
    printf("PID %d\n", p->pid);
    printf("Arrival Time: %d\n", p->arrival_time);
    printf("CPU burst: %d\n", p->cpu_burst);
    printf("Priority: %d\n", p->priority);
    printf("IO Count: %d\n", p->io_count);
    for (int i = 0; i < p->io_count; i++){
        printf("IO %d Request Time: %d\n", i, p->io_list[i].arrival_time);
        printf("IO %d Burst Time: %d\n", i, p->io_list[i].io_burst);
    }
    printf("==============================================\n");

}