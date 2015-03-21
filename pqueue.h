#ifndef __PQUEUE_H
#define __PQUEUE_H

#include <stdio.h>

#include "queue.h"

typedef queue_t pqueue_t;

typedef struct pqnode_t pqnode_t;
struct pqnode_t {
        void*   entry;
	int	priority;
};


pqueue_t* pqueue_init(int (*compare)(void*,void*), void (*print)(void*, FILE*), 
	void* (*clone)(void*), void (*destroy)(void*));

unsigned int pqueue_size(pqueue_t* q);

void*	pqueue_push(queue_t* q, void* elem, int priority);

void*	pqueue_update(queue_t* q, void* elem, int* oldPriority, int* newPriority);

pqnode_t* pqueue_pop(pqueue_t* q);

pqnode_t* pqueue_peek(pqueue_t* q);

void 	pqueue_print(pqueue_t* q, FILE* fd);

int 	pqueue_destroy(pqueue_t** q); 

#endif 
