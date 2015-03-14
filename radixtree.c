
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
	rtnode_t** currentChilds = t->childs;
	assert (currentChilds != NULL);

	for (unsigned int i=0;i<strlen(s);i++) {
		unsigned int childIndex = s[i] - t->alphabetStart;
		if (currentChilds[childIndex] == NULL) {
			currentChilds[childIndex] = (rtnode_t*)malloc(sizeof(rtnode_t));
			if (currentChilds[childIndex] == NULL) {
                		perror("radixtree_add: can't create childs, errno=%d");
				return NULL;
			}
			currentChilds[childIndex]->childs = (rtnode_t**)malloc(t->alphabetSize*sizeof(rtnode_t*));
			memset(currentChilds[childIndex]->childs, 0, t->alphabetSize * sizeof(rtnode_t*));
			if (currentChilds[childIndex]->childs == NULL) {
                		perror("radixtree_add: can't create childs, errno=%d");
				free(currentChilds[childIndex]);
				return NULL;
			}
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

	rtnode_t** currentChilds = t->childs;
	assert (currentChilds != NULL);

	for (unsigned int i=0;i<strlen(s);i++) {
		unsigned int childIndex = s[i] - t->alphabetStart;
		if (currentChilds[childIndex] == NULL) {
			return NULL;
		}
		currentChilds = currentChilds[childIndex]->childs;
	}
	return s;
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
