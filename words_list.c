
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "list.h"

void lst_print(void* elem, FILE* fd) {
        fprintf (fd, "%s ", (char*)elem);
}

int lst_compare(void* s1, void* s2) {
        return strcmp ((char*)s1, (char*)s2);
}

void* lst_clone(void* s) {
        return strdup((char*)s);
}

void lst_destroy(void* s) {
	free(s);
}

void processList(FILE * fd, char* argString) {
	struct timespec before, after;
	unsigned int count = 0;

	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);

	// Add all words to the tree
        clock_gettime(CLOCK_REALTIME, &before);

	rewind(fd);

	while (!feof(fd)) {
		char s[32];
		unsigned int ret = fscanf(fd, "%s\n", s);
		if (list_addLast(l, strdup(s)) != NULL) {
			count+=ret;	
		}
	}

        clock_gettime(CLOCK_REALTIME, &after);
	printf("List: loaded %u words in %ld seconds %ld nanoseconds (avg time: %.0f ns)\n", 
		count, after.tv_sec-before.tv_sec, after.tv_nsec-before.tv_nsec, 
		(float)(after.tv_nsec-before.tv_nsec)/count);

	// Find all words in the list
	count = 0;
	rewind(fd);
        clock_gettime(CLOCK_REALTIME, &before);
	while (!feof(fd)) {
		char s[32];
		unsigned int ret = fscanf(fd, "%s\n", s);
		if (list_findSortedAsc(l, s) != NULL) {
			count+=ret;
		}
		
	}
        clock_gettime(CLOCK_REALTIME, &after);
	printf("List: found %u words in %ld seconds %ld nanoseconds (avg time: %.0f ns)\n", 
		count, after.tv_sec-before.tv_sec, after.tv_nsec-before.tv_nsec, 
		(float)(after.tv_nsec-before.tv_nsec)/count);


        clock_gettime(CLOCK_REALTIME, &before);
	char* s = list_find(l, argString);
        clock_gettime(CLOCK_REALTIME, &after);

	char* msg = (s == NULL) ? "did not find" : "found";
	printf("List: %s %s in %ld nanoseconds\n", msg, argString, after.tv_nsec-before.tv_nsec);

	list_destroy(l);
}
