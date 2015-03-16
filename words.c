
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "radixtree.h"

int main(int argc, char** argv) {

	if (argc != 2) {
		printf("Usage: %s [word]\n", argv[0]);
		return -1;
	}

	struct timespec before, after;

       	clock_getres(CLOCK_REALTIME, &before);
	printf("Clock resolution: %ld seconds %ld nanoseconds\n", before.tv_sec, before.tv_nsec);

	radixtree_t* tree = radixtree_init(26, 'a');

	FILE* fd = fopen("wordlist.txt", "r");
	unsigned int count = 0;

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
	printf("Loaded %ud words in %ld seconds %ld nanoseconds (avg time: %.0f ns)\n", 
		count, after.tv_sec-before.tv_sec, after.tv_nsec-before.tv_nsec, 
		(float)(after.tv_nsec-before.tv_nsec)/count);

	unsigned int total =0, empty=0;
	float density = radixtree_density(tree, tree->childs, &total, &empty);
	printf("Density: %4.1f\n", density);
	


	rewind(fd);
	count = 0;

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
	printf("Found %ud words in %ld seconds %ld nanoseconds (avg time: %.0f ns)\n", 
		count, after.tv_sec-before.tv_sec, after.tv_nsec-before.tv_nsec, 
		(float)(after.tv_nsec-before.tv_nsec)/count);

	fclose(fd);

        clock_gettime(CLOCK_REALTIME, &before);
	char* s = radixtree_find(tree, argv[1]);
        clock_gettime(CLOCK_REALTIME, &after);

	char* msg = (s == NULL) ? "Did not find" : "Found";
	printf("%s %s in %ld nanoseconds\n", msg, argv[1], after.tv_nsec-before.tv_nsec);


	radixtree_destroy(&tree);
}
