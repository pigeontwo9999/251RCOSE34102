
#include "queue.h"
#include "schedule.h"
#include "process.h"
#include <stdio.h>





Process *current;
int current_time;
int current_start;
int now_running;

void init(){
    for (int i=0;i<num_processes;i++){
        current_processes[i] = processes[i];
    }
    queue_init(&ready_queue);
    queue_init(&waiting_queue);
    completed = 0;

    current = NULL;
    current_time = 0;
    current_start = 0;
    now_running = 0;

    for (int i=0;i<MAX_TIME;i++){
        gantt[i].pid = -1;
        gantt[i].start = 0;
        gantt[i].end = 0;
    }
}

void save(int pid, int st, int end){
    gantt[gantt_idx].pid = pid;
    gantt[gantt_idx].start = st;
    gantt[gantt_idx].end = end;
    gantt_idx++;
}

void run(){
    current->status = RUNNING;
    if (current->response_time == -1){ // 처음 run 됐다면 기록해야 함
        current->response_time = current_time - current->arrival_time;
    }
    now_running = 1;
    current_start = current_time; // 간트땜에 적어놓기
}

void check_waitingqueue(){
    int size = waiting_queue.count;
    int head = waiting_queue.head;

    for (int i = 0; i < size; i++){
        Process* waiting = waiting_queue.processlist[(head+i) % QUEUE_SIZE];
        if (waiting->io_list[waiting->io_idx].io_time == waiting->io_list[waiting->io_idx].io_burst){
            waiting->io_list[waiting->io_idx].completed = 1;
            waiting->io_idx = -1;
            waiting->status = READY;
            queue_remove(&waiting_queue, waiting);
            head--;
            queue_push(&ready_queue, waiting);
        }
    }
}

void update_queue(){
    // 레디큐 waiting 증가
     for (int i = 0; i < ready_queue.count; i++){
        int idx = (ready_queue.head + i) % QUEUE_SIZE;
        ready_queue.processlist[idx]->waiting_time++;
    }

    // 웨이팅큐 io시간 증가
    for (int i = 0; i < waiting_queue.count; i++){
        int idx = (waiting_queue.head + i) % QUEUE_SIZE;
        int cur_io_idx = waiting_queue.processlist[idx]->io_idx;
        waiting_queue.processlist[idx]->io_list[cur_io_idx].io_time++;
    }

}

void check_io_request(){
    for (int i=0;i<current->io_count; i++){
        // 진행도
        int cur_time = current->cpu_burst - current->remaining_time;

        if (current->io_list[i].arrival_time == cur_time){
            // io처리하러 떠남
            current->status = WAITING;
            queue_push(&waiting_queue, current);
            current->io_idx = i;
            now_running = 0;
            save(current->pid, current_start, current_time);
            break;
        }
    }
}

void terminate(){
    now_running = 0;

    current->complete_time = current_time;
    current->turnaround_time = current->complete_time - current->arrival_time;
    
    current->status = TERMINATED;
    completed++;
    save(current->pid, current_start, current_time);
}

void FCFS(){
    init();

    while (completed != num_processes){
        
        // 지금 CPU 들고 있는 애 확인
        if (now_running != 0){
            if (current->remaining_time > 0){
                current->remaining_time--;
                check_io_request();
            }

            if (current->remaining_time == 0){
                terminate();
            }
        }

        // 프로세스중에 매물 탐색, readyqueue에 넣기
        for (int i=0;i<num_processes;i++){
            if (current_time == current_processes[i].arrival_time){
                queue_push(&ready_queue, &current_processes[i]);
                current_processes[i].status = READY;
            }
        }
        
        // waiting에서 기다리는 놈 찾기, 레디큐에 박기
        check_waitingqueue();
        
        // 레디큐에 있는거 running으로 전환
        if (ready_queue.count > 0 && now_running == 0){
            current = queue_pop(&ready_queue);
            run();
        }


        update_queue();

        current_time++;
    }

}

void preempt(){
    now_running = 0;
    current->status = READY;
    queue_push(&ready_queue, current);
    save(current->pid, current_start, current_time);
}

void SJF(){ // 선점형
    init();

    while (completed != num_processes){
        if (now_running != 0){
            if (current->remaining_time > 0){
                current->remaining_time--;
                check_io_request();
            }
            
            if (current->remaining_time == 0){
                terminate();
            }
        }

        // 프로세스중에 매물 탐색, readyqueue에 넣기
        for (int i=0;i<num_processes;i++){
            if (current_time == current_processes[i].arrival_time){
                queue_push(&ready_queue, &current_processes[i]);
                current_processes[i].status = READY;

                if (now_running == 1){ // 실행중
                    if (current->remaining_time > current_processes[i].remaining_time){
                        preempt();
                    } //남은게 더 짧은놈이 있다면? 레디큐로 보내버리기기
                }
            }
        }
        
        // waiting에서 기다리는 놈 찾기, 레디큐에 박기
        check_waitingqueue();
        
        // 레디큐에 있는거 running으로 전환
        if (ready_queue.count > 0 && now_running == 0){
            current = queue_pop_shortest(&ready_queue);
            run();
        }


        update_queue();

        current_time++;
    }

}



void SJF_nonpreemption(){ // 비선점형
    init();

    while (completed != num_processes){
        if (now_running != 0){
            if (current->remaining_time > 0){
                current->remaining_time--;
                check_io_request();
            }
            
            if (current->remaining_time == 0){
                terminate();
            }
        }

        // 프로세스중에 매물 탐색, readyqueue에 넣기
        for (int i=0;i<num_processes;i++){
            if (current_time == current_processes[i].arrival_time){
                queue_push(&ready_queue, &current_processes[i]);
                current_processes[i].status = READY;

                
            }
        }
        
        // waiting에서 기다리는 놈 찾기, 레디큐에 박기
        check_waitingqueue();
        
        // 레디큐에 있는거 running으로 전환
        if (ready_queue.count > 0 && now_running == 0){
            current = queue_pop_shortest(&ready_queue);
            run();
        }


        update_queue();

        current_time++;
    }

}


void Priority(){
    init();

    while (completed != num_processes){
        
        // 지금 CPU 들고 있는 애 확인
        if (now_running != 0){
            if (current->remaining_time > 0){
                current->remaining_time--;
                check_io_request();
            }

            if (current->remaining_time == 0){
                terminate();
            }
        }

        // 프로세스중에 매물 탐색, readyqueue에 넣기
        for (int i=0;i<num_processes;i++){
            if (current_time == current_processes[i].arrival_time){
                queue_push(&ready_queue, &current_processes[i]);
                current_processes[i].status = READY;

                if (now_running == 1){ // 실행중
                    if (current->remaining_time > current_processes[i].remaining_time){
                        preempt();
                    } //남은게 더 짧은놈이 있다면? 레디큐로 보내버리기기
                }
            }
        }
        
        // waiting에서 기다리는 놈 찾기, 레디큐에 박기
        check_waitingqueue();
        
        // 레디큐에 있는거 running으로 전환
        if (ready_queue.count > 0 && now_running == 0){
            current = queue_pop_highest_priority(&ready_queue);
            run();
        }


        update_queue();

        current_time++;
    }

}

void Priority_nonpreemption(){
    init();

    while (completed != num_processes){
        
        // 지금 CPU 들고 있는 애 확인
        if (now_running != 0){
            if (current->remaining_time > 0){
                current->remaining_time--;
                check_io_request();
            }

            if (current->remaining_time == 0){
                terminate();
            }
        }

        // 프로세스중에 매물 탐색, readyqueue에 넣기
        for (int i=0;i<num_processes;i++){
            if (current_time == current_processes[i].arrival_time){
                queue_push(&ready_queue, &current_processes[i]);
                current_processes[i].status = READY;

            }
        }
        
        // waiting에서 기다리는 놈 찾기, 레디큐에 박기
        check_waitingqueue();
        
        // 레디큐에 있는거 running으로 전환
        if (ready_queue.count > 0 && now_running == 0){
            current = queue_pop_highest_priority(&ready_queue);
            run();
        }


        update_queue();

        current_time++;
    }

}


void RoundRobin(){
    init();

    while (completed != num_processes){
        
        // 지금 CPU 들고 있는 애 확인
        if (now_running != 0){
            if (current->remaining_time > 0){
                current->remaining_time--;
                check_io_request();
            }

            if (current->remaining_time == 0){
                terminate();
            }

            if (now_running){
                if (current_time - current_start == RR_Q)
                    preempt();
            }
        }

        // 프로세스중에 매물 탐색, readyqueue에 넣기
        for (int i=0;i<num_processes;i++){
            if (current_time == current_processes[i].arrival_time){
                queue_push(&ready_queue, &current_processes[i]);
                current_processes[i].status = READY;
            }
        }
        
        // waiting에서 기다리는 놈 찾기, 레디큐에 박기
        check_waitingqueue();
        
        // 레디큐에 있는거 running으로 전환
        if (ready_queue.count > 0 && now_running == 0){
            current = queue_pop(&ready_queue);
            run();
        }


        update_queue();

        current_time++;
    }

}