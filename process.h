
#define MAX_PROCESSES 10
#define MAX_IO 3

#define MAX_ARRIVAL_TIME 50
#define MAX_PRIORITY 10
#define MAX_BURST_TIME 20
#define MIN_BURST_TIME 5

#define MAX_IO_BURST_TIME 5
#define MIN_IO_BURST_TIME 2

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
    int arrival_time;
    int io_burst;
    int io_time;
    int completed;
} IO;