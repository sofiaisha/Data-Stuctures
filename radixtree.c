
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include <assert.h>

#include "radixtree.h"
#include "util.h"

radixtree_t* radixtree_init(unsigned int alphabetSize) {

        radixtree_t* t = (radixtree_t*)malloc(sizeof(radixtree_t));
        if (t == NULL) { 
                perror("can't create new radix tree, errno=%d");
                return NULL;
        }       

        if (sem_init(&(t->sem), 0, 1) != 0) { 
                perror("radixtree_init: can't create semaphore");
                free(t);
                return NULL;
        }       
       
	t->root		= NULL;
	t->size		= 0;
	t->alphabetSize	= alphabetSize;

        return t;
}

unsigned int radixtree_size(radixtree_t* t) {
	if (t == NULL) {
		debug_print("Invalid parameter\n");
		return 0;
	}

	return t->size;
}

unsigned int radixtree_height(radixtree_t* t, rtnode_t* n) {
        if ((t== NULL) || (n == NULL)) {
                return 0;
        }
	unsigned int max = 0;
	for (unsigned int i=0;i<t->alphabetSize;i++) {
		unsigned int height = radixtree_height(t, n->childs[i]);
		if (height > max) {
			max = height;
		}
	}
        return max;
}

void radixtree_destroy(radixtree_t** t) {
	if ((t == NULL) || (*t == NULL)) {
		debug_print("Invalid parameter\n");
		return;
	}
	
	radixtree_t* tmp = (*t);
	(*t) = NULL;
	radixtree_destroy_internal(tmp, tmp->root);
	free (tmp);
}

void radixtree_destroy_internal(radixtree_t* t, rtnode_t* n) {
	if (n != NULL) {
		for (unsigned int i=0;i<t->alphabetSize;i++) {
			radixtree_destroy_internal(t, n->childs[i]);
		}
		free(n);
	}
}
