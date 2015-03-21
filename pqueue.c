#include <stdio.h>
#include <stdlib.h>

#include "pqueue.h"
#include "queue.h"
#include "list.h"
#include "util.h"

pqueue_t* pqueue_init(int (*compare)(void*,void*), void (*print)(void*, FILE*), 
	void* (*clone)(void*), void (*destroy)(void*)) {
	return (pqueue_t*)queue_init(compare, print, clone, destroy);
}

unsigned int pqueue_size(pqueue_t* q) {
	return queue_size(q);
}

void*	pqueue_push(queue_t* q, void* elem, int priority) {
	if ( (q == NULL) || (elem == NULL) ) {
		debug_print("invalid parameter\n");
                return NULL;
	}

	pqnode_t* n = (pqnode_t*)malloc(sizeof(pqnode_t));
	if (n == NULL) {
                perror("can't create new node, errno=%d");
                return NULL;
        }
	n->entry 	= elem;
	n->priority 	= priority;
	
	if (list_addSortAsc(q, n) == NULL) {
		free (n);
		return NULL;
	}
	return elem;
}

void*   pqueue_update(queue_t* q, void* elem, int* oldPriority, int* newPriority) {
	if ( (q == NULL) || (elem == NULL) || (newPriority == NULL) ) {
		debug_print("invalid parameter\n");
                return NULL;
	}

	list_t* l = (list_t*)q;
	node_t* n = l->head;
	pqnode_t* qn;

	// Find the node to be updated
	while (n != NULL) {
		qn = ((pqnode_t*)n->elem);
		if (qn->entry == elem) {
			break;
		}	
		n = n->next;
	}
	// Node hasn't been found
	if (n == NULL) {
		return NULL;
	}

	// If priorities are different,
	if (qn->priority != *newPriority) {
		// Allocate and initialize a new priority queue node
		pqnode_t* newqn = (pqnode_t*)malloc(sizeof(pqnode_t));
		if (newqn == NULL) {
                	perror("can't create new node, errno=%d");
                	return NULL;
        	}
		newqn->entry 	 = elem;
		newqn->priority  = *newPriority;

		// Remove the list node
		list_remove(q, n->elem);
	
		// Add the new priority queue node
		if (list_addSortAsc(q, newqn) == NULL) {
			debug_print("lost a node!\n");
			// This isn't great, but the only reason this would fail
			// is a malloc error. Not much we can do anyway,
			// especially not trying to add the node back...
			free (newqn);
			return NULL;
		}
	}
	// Save the old priority
	if (oldPriority != NULL) {
		*oldPriority = qn->priority;
	}

	return elem;
}

pqnode_t* pqueue_peek(pqueue_t* q) {
	return (pqnode_t*)queue_peek(q);
}

pqnode_t* pqueue_pop(pqueue_t* q) {
	return (pqnode_t*)queue_pop(q);
}

void 	pqueue_print(pqueue_t* q, FILE* fd) {
	list_print(q, fd);
}

int 	pqueue_destroy(pqueue_t** q) {
	return list_destroy((list_t**)q);
}


