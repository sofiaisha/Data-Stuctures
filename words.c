
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "radixtree.h"

void processRadixtree(FILE * fd, char* s); 
void processList(FILE * fd, char* s); 

int main(int argc, char** argv) {

	if (argc != 2) {
		printf("Usage: %s [word]\n", argv[0]);
		return -1;
	}

	struct timespec ts;
       	clock_getres(CLOCK_REALTIME, &ts);
	printf("Clock resolution: %ld seconds %ld nanoseconds\n", ts.tv_sec, ts.tv_nsec);


	FILE* fd = fopen("wordlist.txt", "r");

	processRadixtree(fd, argv[1]);
	processList(fd, argv[1]);

	fclose(fd);
}

