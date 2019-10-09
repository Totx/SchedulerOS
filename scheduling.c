#include "list.h"
#include "scheduling.h"
#include "proc.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

list_t *ready_queue;
int top_priority_list;

void scheduler(int cpu, int core, int hthread){
    
    struct pcb *proc;

    switch(conf.scheduling_policy){

        case FIFO:
            if (process_to_schedule(0)){
                list_head(&ready_queue[0], &proc);
                computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc = proc;
                list_rem_head(&ready_queue[0]);
            } else {
                create_null_process(&computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc);
            }
            break;
        case RR:
            // If there is program executing that still hasn't finished its task but has used the quantum, it's stored once again in the ready queue
            if (!computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc->pid == 0 && computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc->cycles > 0){
                computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc->executed_tick_in_a_row = 0;
                insert_process_ready_queue(computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc);
            }
            if (process_to_schedule(0)){
                list_head(&ready_queue[0], &proc);
                computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc = proc;
                list_rem_head(&ready_queue[0]);
            } else {
                create_null_process(&computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc);
            }
            break;
        case PRIORITY:
            if (!computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc->pid == 0 && computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc->cycles > 0){
                computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc->executed_tick_in_a_row = 0;
                insert_process_ready_queue(computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc);
                if (computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc->priority > top_priority_list){
                    top_priority_list = computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc->priority;
                }
            }
            while (top_priority_list >= 0 && !process_to_schedule(top_priority_list)){
                top_priority_list--;
            }
            if (top_priority_list >= 0){
                list_head(&ready_queue[top_priority_list], &proc);
                computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc = proc;
                list_rem_head(&ready_queue[top_priority_list]);
                while (top_priority_list >= 0 && !process_to_schedule(top_priority_list)){
                top_priority_list--;
                }
            } else {
                create_null_process(&computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc);
            }
            break;
        default:
            printf("Unknown scheduling policy\n");
            exit(-1);
    }


}

void create_ready_queue(){
    
    ready_queue = malloc(conf.priority_level * sizeof(list_t));
    // Top level of priority
    top_priority_list = conf.priority_level - 1;
    for (int i = 0; i < conf.priority_level; i++)
    {
        list_initialize(&ready_queue[i]); 
    }

}

void insert_process_ready_queue(struct pcb* proc){

    list_append(&ready_queue[proc->priority], proc);

}

long process_to_schedule(int p){

    return(!list_empty(&ready_queue[p]));
}


