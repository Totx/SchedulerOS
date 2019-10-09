#include "list.h"
#include "scheduling.h"
#include "proc.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

list_t ready_queue;

void scheduler(int cpu, int core, int hthread){
    
    struct pcb *proc;

    switch(conf.scheduling_policy){

        case FIFO:
            if (process_to_schedule()){
                list_head(&ready_queue, &proc);
                computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc = proc;
                list_rem_head(&ready_queue);
            } else {
                create_null_process(&computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc);
            }
            break;
        case RR:
            if (!computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc->pid == 0){
                list_append(&ready_queue, &computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc);
            }
            if (process_to_schedule()){
                list_head(&ready_queue, &proc);
                computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc = proc;
                list_rem_head(&ready_queue);
            } else {
                create_null_process(&computing_engine.cpus[cpu].cores[core].hthreads[hthread].proc);
            }
            break;
        case PRIORITY:
            break;
        default:
            printf("Unknown scheduling policy\n");
            exit(-1);
    }


}

void create_ready_queue(){
    
    list_initialize(&ready_queue); 

}

void insert_process_ready_queue(struct pcb* proc){

    list_append(&ready_queue, proc);

}

long process_to_schedule(){

    return(!list_empty(&ready_queue));
}


