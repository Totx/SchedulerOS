#include "proc.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

void read_file(char *file_name) {

	FILE *fd;
	long ticks;
	int priority;


	if((fd = fopen(file_name, "r")) == NULL){
		printf("Error while opening file.\n");
		exit(-1);
	}
	
	if (conf.scheduling_policy == PRIORITY){
		while (fscanf(fd, "%ld %d", &ticks, &priority) != EOF){
                create_new_process(ticks, priority);
			printf("Ticks %ld, Priority: %d\n", ticks, priority);
		}
	} else {
		while (fscanf(fd, "%ld", &ticks) != EOF){
                create_new_process(ticks, 0);
			printf("Ticks %ld\n", ticks);
		}
	}
	

	fclose(fd);
}
