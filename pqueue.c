#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "pqueue.h"
#include "queue.h"
#include "list.h"
#include "util.h"

pqueue_t* pqueue_init(unsigned int type, 
	int (*compare)(void*,void*), void (*print)(void*, FILE*), 
	void* (*clone)(void*), void (*destroy)(void*)) 
	{

	if ((type != PQUEUE_MIN) && (type != PQUEUE_MAX)) {
		debug_print("invalid parameter\n");
		return NULL;
	}
	if ((compare == NULL) || (print == NULL) || (clone == NULL) || (destroy == NULL)) {
		debug_print("invalid parameter\n");
		return NULL;
	}

	pqueue_t* pq = (pqueue_t*)malloc(sizeof(pqueue_t));
	if (pq == NULL) {
                perror("can't create new priority queue");
                return NULL;
	}

	pq->queue = queue_init(compare, print, clone, destroy);
	if (pq->queue == NULL) {
		free(pq);
                perror("can't create new priority queue");
                return NULL;
	}
		
	pq->type = type;
	pq->compare = compare;
	pq->print = print;
	pq->clone = clone;
	pq->destroy = destroy;

	return pq;
}

unsigned int pqueue_size(pqueue_t* q) {
	return queue_size(q->queue);
}

void*	pqueue_push(pqueue_t* q, void* elem, int priority) {
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
	
	// Add the new priority queue node
	void* ret;
	switch (q->type) {
		case PQUEUE_MIN:
			ret = list_addSortAsc(q->queue, n);
			break;
		case PQUEUE_MAX:
			ret = list_addSortDesc(q->queue, n);
			break;
		default:
			// Should not happen
			debug_print("Invalid queue type\n");
			assert(0);
			break;
	}
	if (ret == NULL) {
		free (n);
		return NULL;
	}
	return elem;
}

void*   pqueue_update(pqueue_t* q, void* elem, int* oldPriority, int* newPriority) {
	if ( (q == NULL) || (elem == NULL) || (newPriority == NULL) ) {
		debug_print("invalid parameter\n");
                return NULL;
	}

	list_t* l = (list_t*)(q->queue);
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
		list_remove(q->queue, n->elem);
	
		// Add the new priority queue node
		void* ret;
		switch (q->type) {
			case PQUEUE_MIN:
				ret = list_addSortAsc(q->queue, newqn);
				break;
			case PQUEUE_MAX:
				ret = list_addSortDesc(q->queue, newqn);
				break;
			default:
				// Should not happen
				debug_print("Invalid queue type\n");
				assert(0);
				break;
		}
	
		if (ret == NULL) {
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
	if (q == NULL) {
		return NULL;
	}
	return (pqnode_t*)queue_peek(q->queue);
}

pqnode_t* pqueue_pop(pqueue_t* q) {
	if (q == NULL) {
		return NULL;
	}
	return (pqnode_t*)queue_pop(q->queue);
}

void 	pqueue_print(pqueue_t* q, FILE* fd) {
	if (q == NULL) {
		return;
	}
	list_print(q->queue, fd);
}

int 	pqueue_destroy(pqueue_t** q) {
	if ((q == NULL) || (*q == NULL)) {
		return EINVAL;
	}

	list_destroy(&((*q)->queue));
	free(*q);	
	return 0;
}


