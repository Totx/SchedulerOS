#include "globals.h"
#include "scheduling.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int current_tick = 0;

void tick(){
    current_tick++;
    printf(">>>>>>%5d\n", current_tick);
}

void start_simulation(){

    int i, j, k;
    int active;

    while(1){
        tick();
        active = 0;
        for(i = 0; i < computing_engine.ncpus; i++){
            for(j = 0; j <  computing_engine.cpus[i].ncores; j++){
                for(k = 0; k < computing_engine.cpus[i].cores[j].nhthreads; k++){
                    if (conf.execution_policy == NONPREEMPTIVE){
                        if (conf.scheduling_policy == FIFO){
                            if(computing_engine.cpus[i].cores[j].hthreads[k].proc->pid == 0){
                                scheduler(i, j, k);
                            }else{
                                computing_engine.cpus[i].cores[j].hthreads[k].proc->cycles--;
                                if(computing_engine.cpus[i].cores[j].hthreads[k].proc->cycles == 0) scheduler(i, j, k);
                            }
                        }
                    } else {
                        if(computing_engine.cpus[i].cores[j].hthreads[k].proc->pid == 0){
                            scheduler(i, j, k);
                        } else {
                            computing_engine.cpus[i].cores[j].hthreads[k].proc->cycles--;
                            computing_engine.cpus[i].cores[j].hthreads[k].proc->executed_tick_in_a_row ++;
                            if(computing_engine.cpus[i].cores[j].hthreads[k].proc->cycles == 0 || computing_engine.cpus[i].cores[j].hthreads[k].proc->executed_tick_in_a_row == conf.quantum) scheduler(i, j, k);
                        }
                    }
                    if(!computing_engine.cpus[i].cores[j].hthreads[k].proc->pid == 0) active++;
                    printf("TICK: %d, CPU %d, CORE: %d, THREAD: %d, PID: %d \n", current_tick, i, j, k, computing_engine.cpus[i].cores[j].hthreads[k].proc->pid);
                }
            }
        }
        if(active == 0) {
            printf("All processes have been handled by the system\n");
            exit(0);
        }
        sleep(1);
    }   
}
