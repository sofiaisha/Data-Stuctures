
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "radixtree.h"

void processRadixtree(FILE * fd, char* argString) {
	struct timespec before, after;
	unsigned int count = 0;

	radixtree_t* tree = radixtree_init(26, 'a');
	rewind(fd);

	// Add all words to the tree
        clock_gettime(CLOCK_REALTIME, &before);
	while (!feof(fd)) {
		char s[32];
		unsigned int ret = fscanf(fd, "%s\n", s);

		if (radixtree_add(tree, s) != NULL) {
			count+=ret;	
		}
	}
        clock_gettime(CLOCK_REALTIME, &after);
	printf("Radixtree: loaded %ud words in %ld seconds %ld nanoseconds (avg time: %.0f ns)\n", 
		count, after.tv_sec-before.tv_sec, after.tv_nsec-before.tv_nsec, 
		(float)(after.tv_nsec-before.tv_nsec)/count);

	unsigned int total =0, empty=0;
	float density = radixtree_density(tree, tree->childs, &total, &empty);
	printf("Radixtree: density=%4.1f\n", density);
	
	rewind(fd);

	// Find all words in the tree
        clock_gettime(CLOCK_REALTIME, &before);
	while (!feof(fd)) {
		char s[32];
		unsigned int ret = fscanf(fd, "%s\n", s);

		if (radixtree_find(tree, s) != NULL) {
			count+=ret;
		}
		
	}
        clock_gettime(CLOCK_REALTIME, &after);
	printf("Radixtree: found %ud words in %ld seconds %ld nanoseconds (avg time: %.0f ns)\n", 
		count, after.tv_sec-before.tv_sec, after.tv_nsec-before.tv_nsec, 
		(float)(after.tv_nsec-before.tv_nsec)/count);

	fclose(fd);

        clock_gettime(CLOCK_REALTIME, &before);
	char* s = radixtree_find(tree, argString);
        clock_gettime(CLOCK_REALTIME, &after);

	char* msg = (s == NULL) ? "did not find" : "found";
	printf("Radixtree: %s %s in %ld nanoseconds\n", msg, argString, after.tv_nsec-before.tv_nsec);

	radixtree_destroy(&tree);
}
