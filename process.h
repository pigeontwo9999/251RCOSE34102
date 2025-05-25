#ifndef PROCESS_H
#define PROCESS_H

#include "global.h"

void pid_list_config(void);
void create_process(int pid,int arrival_time,int cpu_burst,int priority,int io_count);
void create_io(Process*p,int*arrival_time,int*io_burst);
void create_process_random(void);
void create_process_input(void);
void print_process(Process*p);

#endif