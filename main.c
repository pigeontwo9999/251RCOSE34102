#include "global.h"

#include "queue.h"
#include "process.h"
#include "schedule.h"
#include "evaluate.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>



Process processes[MAX_PROCESSES];
int num_processes = 0;
int completed = 0;

Process current_processes[MAX_PROCESSES];

Queue ready_queue;
Queue waiting_queue;

Chart gantt[MAX_TIME];
int gantt_idx;

evaluation table[7];

void menu(){
    printf("CPU Scheduling Simulator\n");
    printf("========================================\n");
    printf("1. type\n");
    printf("2. Random\n");
    printf("========================================\n");
    printf("Enter your choice: ");
}

int main(){
    srand(time(0));
    pid_list_config();
    int choice, howmany;


    menu();
    scanf("%d", &choice);

    printf("Enter the number of processes: ");
    scanf("%d", &howmany);

    if (choice == 1){
        for (int i=0;i<howmany;i++){
            printf("Creating Process %d ... \n", i);
            create_process_input();
        }
    }

    else{
        for (int i=0;i<howmany;i++){
            printf("Creating Process %d ... \n", i);
            create_process_random();
        }       
    }


    for (int i=0;i<howmany;i++){
        print_process(&processes[i]);
    }

    FCFS();
    evaluate(0);
    SJF();
    evaluate(1);
    SJF_nonpreemption();
    evaluate(2);
    Priority();
    evaluate(3);
    Priority_nonpreemption();
    evaluate(4);
    RoundRobin();
    evaluate(5);
    gather();
    best();
}