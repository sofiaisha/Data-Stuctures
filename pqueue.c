#include <stdio.h>
#include <stdlib.h>

#include "pqueue.h"
#include "queue.h"
#include "list.h"
#include "util.h"

int pq_compare_internal(void* e1, void* e2) {
	int p1 = ((pqnode_t*)e1)->priority;
	int p2 = ((pqnode_t*)e2)->priority;
	if (p1 == p2) return 0; 
	return (p1<p2? -1:1);
}

pqueue_t* pqueue_init(void (*print)(void*, FILE*), void* (*clone)(void*), void (*destroy)(void*)) {
	return (pqueue_t*)queue_init(pq_compare_internal, print, clone, destroy);
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


