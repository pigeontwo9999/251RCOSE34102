#include "evaluate.h"
#include "process.h"
#include "schedule.h"
#include <stdio.h>

const char *name_table[] = {
        "FCFS",
        "SJF",
        "SJF_nonpreemption",
        "Priority",
        "Priority_nonpreemption",
        "RoundRobin"
    };
int get_min_waiting_time(){
    int min_waiting_time = MAX_TIME;
    for (int i = 0; i< num_processes; i++){
        if (current_processes[i].waiting_time < min_waiting_time){
            min_waiting_time = current_processes[i].waiting_time;
        }
    }
    return min_waiting_time;
}
int get_min_turnaround_time(){
    int min_turnaround_time = MAX_TIME;
    for (int i = 0; i < num_processes; i++){
        if (current_processes[i].turnaround_time < min_turnaround_time){
            min_turnaround_time = current_processes[i].turnaround_time;
        }
    }
    return min_turnaround_time;
}    

float get_average_waiting_time(){
    float total_waiting_time = 0;
    for (int i = 0; i < num_processes; i++)
        total_waiting_time += current_processes[i].waiting_time;    
    return total_waiting_time / num_processes;
}
float get_average_turnaround_time(){
    float total_turnaround_time = 0;
    for (int i = 0; i < num_processes; i++)
        total_turnaround_time += current_processes[i].turnaround_time;
    return total_turnaround_time / num_processes;
}

int get_max_waiting_time(){
    int max_waiting_time = 0;
    for (int i = 0; i < num_processes; i++){
        if (current_processes[i].waiting_time > max_waiting_time){
            max_waiting_time = current_processes[i].waiting_time;
        }
    }
    return max_waiting_time;
}
int get_max_turnaround_time(){
    int max_turnaround_time = 0;
    for (int i = 0; i < num_processes; i++){
        if (current_processes[i].turnaround_time > max_turnaround_time){
            max_turnaround_time = current_processes[i].turnaround_time;
        }
    }
    return max_turnaround_time;
}

int get_sum_burst_time(){
    int sum = 0;
    for (int i = 0; i < num_processes; i++){
        sum += current_processes[i].cpu_burst;
    }
    return sum;
}
int get_max_completion_time(){
    int max_completion_time = 0;
    for (int i = 0; i < num_processes; i++){
        if (current_processes[i].complete_time > max_completion_time){
            max_completion_time = current_processes[i].complete_time;
        }
    }
    return max_completion_time;
}
int get_first_arrival_time(){
    int first_arrival = 9999;
    for (int i = 0; i < num_processes; i++){
        if (current_processes[i].arrival_time < first_arrival){
            first_arrival= current_processes[i].arrival_time;
        }
    }
    return first_arrival;
}

float get_cpu_utilization(){
    return (float)get_sum_burst_time() / (get_max_completion_time() - get_first_arrival_time());
}

void print_gantt(){
    printf("\nGantt chart\n");
    printf("========================================\n\n");
    if (gantt[0].start != 0){
        printf("|  idle  |");
    } else{
        printf("|");
    }
    for (int i = 0; i < gantt_idx ; i++){
        printf("   P%-3d |", gantt[i].pid);
        if(gantt[i].end != gantt[i+1].start){
            printf("  idle  |");
        }
    }
    if (gantt[0].start != 0){
        printf("\n0\t ");
    } else{
        printf("\n");
    }
    for (int i = 0; i < gantt_idx ; i++){
        if(gantt[i].end != gantt[i+1].start){
            printf("%-5d%5d", gantt[i].start, gantt[i].end);
            printf("        ");
        }
        else{
            printf("%-9d", gantt[i].start);
        }
    }
    printf("fin\n\n");
}

void evaluate(int idx){
    print_gantt();

    char *name = name_table[idx];
    float average_waiting_time = get_average_waiting_time();
    float average_turnaround_time = get_average_turnaround_time();
    float cpu_utilization = get_cpu_utilization();
    int max_wait = get_max_waiting_time();
    int max_turn = get_max_turnaround_time();
    int min_wait = get_min_waiting_time();
    int min_turn = get_min_turnaround_time();

    table[idx].algorithm = name;
    table[idx].average_waiting_time = average_waiting_time;
    table[idx].average_turnaround_time = average_turnaround_time;
    table[idx].cpu_utilization = cpu_utilization;
    
    table[idx].max_wait = max_wait;
    table[idx].min_wait = min_wait;
    table[idx].max_turn = max_turn;
    table[idx].min_turn = min_turn;




    printf("| Algorithm: %s \n| Average Waiting Time: %.2f,   MAX: %d, MIN: %d \n| Average Turnaround Time: %.2f,   MAX: %d, MIN: %d\n| CPU Utilization: %.2f%%\n",
       table[idx].algorithm,
       table[idx].average_waiting_time,max_wait,min_wait,
       table[idx].average_turnaround_time,max_turn,min_turn,
       table[idx].cpu_utilization * 100.0f);

}

void gather(){
    printf("\n===Result===\n");
    for(int idx=0;idx<6;idx++){
       printf("** Algorithm: %s **\n\t| Average Waiting Time: %.2f,   MAX: %d, MIN: %d | Average Turnaround Time: %.2f,   MAX: %d, MIN: %d| CPU Utilization: %.2f%%\n",
       table[idx].algorithm,
       table[idx].average_waiting_time,table[idx].max_wait,table[idx].min_wait,
       table[idx].average_turnaround_time,table[idx].max_turn,table[idx].min_turn,
       table[idx].cpu_utilization * 100.0f);
    }
}

void best(){
    
    int best_wait_idx = 0;
    int best_turn_idx = 0;
    int best_cpu_idx  = 0;

    for (int i = 0; i < 6; i++) {
        
        if (table[i].average_waiting_time < table[best_wait_idx].average_waiting_time) {
            best_wait_idx = i;
        }
        
        if (table[i].average_turnaround_time < table[best_turn_idx].average_turnaround_time) {
            best_turn_idx = i;
        }
        
        if (table[i].cpu_utilization > table[best_cpu_idx].cpu_utilization) {
            best_cpu_idx = i;
        }
    }

    printf("\n=== Best Algorithms ===\n");
    printf("Shortest Average Waiting Time   : %s (%.2f units)\n",
           table[best_wait_idx].algorithm,
           table[best_wait_idx].average_waiting_time);
    printf("Shortest Average Turnaround Time: %s (%.2f units)\n",
           table[best_turn_idx].algorithm,
           table[best_turn_idx].average_turnaround_time);
    
    printf("Highest CPU Utilization         : %s (%.2f%%)\n",
           table[best_cpu_idx].algorithm,
           table[best_cpu_idx].cpu_utilization * 100.0f);
}