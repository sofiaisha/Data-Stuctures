
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#include "radixtree.h"
#include "util.h"

radixtree_t* radixtree_init(unsigned int alphabetSize, char alphabetStart) {

	if (alphabetSize == 0)  {
		debug_print("Invalid parameter\n");
		return NULL;
	}

        radixtree_t* t = (radixtree_t*)malloc(sizeof(radixtree_t));
        if (t == NULL) { 
                perror("radixtree_init: can't create new radix tree\n");
                return NULL;
        }       
	t->size			= 0;
	t->alphabetSize		= alphabetSize;
	t->alphabetStart	= alphabetStart;

	t->childs = (rtnode_t**)malloc((t->alphabetSize)*sizeof(rtnode_t*));
	if (t->childs == NULL) {
                perror("radixtree_init: can't create childs\n");
		free(t);
		return NULL;
	}		
	memset(t->childs, 0, t->alphabetSize * sizeof(rtnode_t*));
	
        return t;
}

int radixtree_size(radixtree_t* t) {
	if (t == NULL) {
		debug_print("Invalid parameter\n");
		return EINVAL;
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
		// Compute the index of the character in the childs array
		unsigned int childIndex = s[i] - t->alphabetStart;
		// If we're on a former leaf, add a node 
		if (currentChilds[childIndex] == NULL) {
			currentChilds[childIndex] = (rtnode_t*)malloc(sizeof(rtnode_t));
			if (currentChilds[childIndex] == NULL) {
                		perror("radixtree_add: can't create childs\n");
				return NULL;
			}
			currentChilds[childIndex]->last   = false;
			currentChilds[childIndex]->childs = NULL;
		}

		// If this is the last character, 
		// Mark the node
		// Don't allocate childs array to save memory
		// Leave the loop
		if (i == strlen(s)-1) {
			currentChilds[childIndex]->last = true;
			break;
		}
		
		// This is either a new node (allocated above) or a former leaf node
		// Allocate childs array
		if (currentChilds[childIndex]->childs == NULL) {
			currentChilds[childIndex]->childs = 
				(rtnode_t**)malloc(t->alphabetSize*sizeof(rtnode_t*));
			if (currentChilds[childIndex]->childs == NULL) {
               			perror("radixtree_add: can't create childs\n");
				free(currentChilds[childIndex]);
				return NULL;
			}
			// Zero the array (yes, it's really needed)
			memset(currentChilds[childIndex]->childs, 0, t->alphabetSize * sizeof(rtnode_t*));
		}
		// Walk to the next node
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
	rtnode_t* child = NULL;	
	for (unsigned int i=0;i<strlen(s);i++) {
		if (currentChilds == NULL) {
			// We're on a leaf and we have more characters to look for : string is not found
			return NULL;
		}
		// Compute the index of the character in the childs array
		unsigned int childIndex = s[i] - t->alphabetStart;
		child = currentChilds[childIndex];
		if (child == NULL) {
			// We're inside the tree, but no node exists for this character : string is not found
			return NULL;
		}
		// Move to the next node
		currentChilds = child->childs;
	}

	// If the node has been marked as the last one, we've found the string.
	// If not, we've only matched a substring and we return NULL.
	return (child->last) ? s : NULL;
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
	debug_print("Total: %u, empty: %u, full: %u\n", *total, *empty, full);
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
	if (c->childs) {
		for (unsigned int i=0;i<t->alphabetSize;i++) {
			radixtree_destroy_internal(t, c->childs[i]);
		}
		free(c->childs);
	}
	free(c);
}
