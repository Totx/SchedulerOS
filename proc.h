#ifndef _proc
#define _proc

typedef struct pcb {

	int pid;
	long cycles;
	int executed_tick_in_a_row;
	int priority;

} pcb;

void create_allprocs_queue();

void create_new_process(long duration, int priority);

void create_null_process(struct pcb **null_process);

#endif
