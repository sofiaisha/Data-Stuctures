#include <stdio.h>

#include "queue.h"
#include "list.h"

queue_t* queue_init(int (*compare)(void*, void*), void (*print)(void*, FILE*), 
		  void* (*clone)(void*), void (*destroy)(void*)) {

	return (queue_t*)list_init(compare, print, clone, destroy);
}

unsigned int queue_size(queue_t* q) {
	return list_size((list_t*)q);
}

void*	queue_push(queue_t* q, void* elem) {
	return list_addLast((list_t*)q, elem);	
}

void*	queue_peek(queue_t* q) {
	return list_getElem((list_t*)q, 0, false);
}

void*	queue_pop(queue_t* q) {
	list_t* l = (list_t*)q;
	void* ret = list_getElem(l, 0, true);
	list_remove(l, ret);
	return ret;
}

void 	queue_print(queue_t* q, FILE* fd) {
	list_print((list_t*)q, fd);
}

int 	queue_destroy(queue_t** q) {
	return list_destroy((list_t**)q);
}
