
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "radixtree.h"
#include "util.h"

radixtree_t* radixtree_init(unsigned int alphabetSize, char alphabetStart) {

	if (alphabetSize == 0)  {
		debug_print("Invalid parameter\n");
		return NULL;
	}

        radixtree_t* t = (radixtree_t*)malloc(sizeof(radixtree_t));
        if (t == NULL) { 
                perror("radixtree_init: can't create new radix tree, errno=%d");
                return NULL;
        }       
	t->size			= 0;
	t->alphabetSize		= alphabetSize;
	t->alphabetStart	= alphabetStart;

	t->childs = (rtnode_t**)malloc((t->alphabetSize)*sizeof(rtnode_t*));
	if (t->childs == NULL) {
                perror("radixtree_init: can't create childs, errno=%d");
		free(t);
		return NULL;
	}		
	memset(t->childs, 0, t->alphabetSize * sizeof(rtnode_t*));
	
        return t;
}

unsigned int radixtree_size(radixtree_t* t) {
	if (t == NULL) {
		debug_print("Invalid parameter\n");
		return 0;
	}

	return t->size;
}

char* radixtree_add(radixtree_t* t, char* s) {
	if ((t == NULL) || (s == NULL)) {
		debug_print("Invalid parameter\n");
		return NULL;
	}
	assert (t->childs != NULL);
	
	rtnode_t** currentChilds = t->childs;

	for (unsigned int i=0;i<strlen(s);i++) {
		unsigned int childIndex = s[i] - t->alphabetStart;

		if (currentChilds[childIndex] == NULL) {
			currentChilds[childIndex] = (rtnode_t*)malloc(sizeof(rtnode_t));
			if (currentChilds[childIndex] == NULL) {
                		perror("radixtree_add: can't create childs, errno=%d");
				return NULL;
			}

			currentChilds[childIndex]->childs = (rtnode_t**)malloc(t->alphabetSize*sizeof(rtnode_t*));
			if (currentChilds[childIndex]->childs == NULL) {
                		perror("radixtree_add: can't create childs, errno=%d");
				free(currentChilds[childIndex]);
				return NULL;
			}
			memset(currentChilds[childIndex]->childs, 0, t->alphabetSize * sizeof(rtnode_t*));
		}
		currentChilds = currentChilds[childIndex]->childs;
	}

	t->size++;
	return s;
}

char* radixtree_find(radixtree_t* t, char* s) {
	if ((t == NULL) || (s == NULL)) {
		debug_print("Invalid parameter\n");
		return NULL;
	}
	assert (t->childs != NULL);

	rtnode_t** currentChilds = t->childs;

	for (unsigned int i=0;i<strlen(s);i++) {
		unsigned int childIndex = s[i] - t->alphabetStart;
		rtnode_t* child = currentChilds[childIndex];
		if (child == NULL) {
			return NULL;
		}
		currentChilds = child->childs;
	}
	return s;
}

float radixtree_density(radixtree_t* t, rtnode_t** childs, unsigned int* total, unsigned int* empty) {
	if ( (t == NULL) || (childs == NULL) || (total == NULL) || (empty == NULL)) {
		debug_print("Invalid parameter\n");
		return -1; 
	}
	
	for (unsigned int i =0; i<t->alphabetSize; i++) {
		(*total)++;
		if (childs[i] == NULL) {
			(*empty)++;
		}
		else {
			radixtree_density(t, childs[i]->childs, total, empty);
		}
	}
	unsigned int full = (*total)-(*empty);
	debug_print("Total: %d, empty: %d, full: %d\n", *total, *empty, full);
	return ((float)full)/(*total)*100;
}

void radixtree_destroy(radixtree_t** t) {
	if ((t == NULL) || (*t == NULL)) {
		debug_print("Invalid parameter\n");
		return;
	}

	radixtree_t* tmp = (*t);
	(*t) = NULL;
	for (unsigned int i=0;i<tmp->alphabetSize;i++) {
		radixtree_destroy_internal(tmp, tmp->childs[i]);
	}
	free(tmp->childs);
	free (tmp);
}

void radixtree_destroy_internal(radixtree_t* t, rtnode_t* c) {
	if ( (t == NULL) || (c == NULL) ) {
		return;
	}
	for (unsigned int i=0;i<t->alphabetSize;i++) {
		radixtree_destroy_internal(t, c->childs[i]);
	}
	free(c->childs);
	free(c);
}
