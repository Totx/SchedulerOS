#include "literal.h"
#include "conf.h"
#include <stdio.h>
#include <stdlib.h>

configuration_t conf;

static literal_t scheduling_policies_l[] = {
    {0, "fifo"},
    {1, "round-robin"},
    {2, "priority"}
};

static literal_t execution_policies_l[] = {
    {0, "preemptive"},
    {1, "nonpreemptive"},
};



void get_conf(int num, char *argv[]){


	if (num >= 7 && num<=9){
		if(!literal_value(scheduling_policies_l, argv[1], (int *) &conf.scheduling_policy)) {
            printf("Unknown scheduling policy\n");
            exit(-1);
        }
        if(!literal_value(execution_policies_l, argv[2], (int *) &conf.execution_policy)) {
            printf("Unknown execution policy\n");
            exit(-1);
        }

        conf.file_name = argv[3];
        conf.ncpus = atoi(argv[4]);
        conf.ncores = atoi(argv[5]);
        conf.nhthreads = atoi(argv[6]);
        conf.ngpus = atoi(argv[7]);
        // Check whether a quantum is explicitly declared in a nonpreemptive scheduling policy.
        if (conf.execution_policy == 1 && num==8){
            printf("Invalid input, can't be introduced a quantum in a nonpreemptive method\n");
            exit(-1);
        }
        if (num >= 8) conf.quantum = atoi(argv[8]);
        else conf.quantum = 0;
        if (num == 9 && atoi(argv[9]) > 0) conf.priority_level = atoi(argv[9]);
        else conf.priority_level = 1;
	} else {
        printf("The format of the input is incorrect, please follow this order:\n");
        printf("./sim (fifo|round-robin|priority) (preemptive|nonpreemptive) file_name ncpu ncores nhthreads ngpus quantum\n");
        exit(-1);
    }

}
