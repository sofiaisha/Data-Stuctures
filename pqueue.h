#ifndef __PQUEUE_H
#define __PQUEUE_H

#include <stdio.h>

#include "queue.h"

typedef struct pqueue_t pqueue_t;
struct pqueue_t {
	queue_t* 	queue;
	unsigned int 	type;
	int 		(*compare)(void*,void*);
	void 		(*print)(void*, FILE*);
	void* 		(*clone)(void*);
	void 		(*destroy)(void*);
};

// Priority queue types
#define PQUEUE_MIN 0
#define PQUEUE_MAX 1

typedef struct pqnode_t pqnode_t;
struct pqnode_t {
        void*   entry;
	int	priority;
};


pqueue_t* pqueue_init(unsigned int type, 
	int (*compare)(void*,void*), void (*print)(void*, FILE*), 
	void* (*clone)(void*), void (*destroy)(void*));

unsigned int pqueue_size(pqueue_t* q);

void*	pqueue_push(pqueue_t* q, void* elem, int priority);

void*	pqueue_update(pqueue_t* q, void* elem, int* oldPriority, int* newPriority);

pqnode_t* pqueue_pop(pqueue_t* q);

pqnode_t* pqueue_peek(pqueue_t* q);

void 	pqueue_print(pqueue_t* q, FILE* fd);

int 	pqueue_destroy(pqueue_t* q); 

#endif 
