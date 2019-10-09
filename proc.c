#include "list.h"
#include "proc.h"
#include "scheduling.h"
#include <stdlib.h>
#include <stdio.h>

int last_pid;
list_t allprocs;

int new_pid(){
	last_pid = last_pid + 1;
	return last_pid;
}

void create_allprocs_queue(){

        list_initialize(&allprocs);
        last_pid = 0;

}

void create_new_process(long duration){

    struct pcb *proc = malloc(sizeof(pcb));
    proc->pid = new_pid();
    proc->cycles = duration;
    proc->executed_tick_in_a_row = 0;
    list_append(&allprocs, proc);
    printf("New process created\n");
    insert_process_ready_queue(proc);
}


void create_null_process(struct pcb **null_process){

    *null_process = malloc(sizeof(struct pcb));
    (*null_process)->pid = 0;
    (*null_process)->cycles = 0;
    (*null_process)->executed_tick_in_a_row = 0;

}


