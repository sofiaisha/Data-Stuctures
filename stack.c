#include <stdio.h>

#include "stack.h"
#include "list.h"

stack_t* stack_init(int (*compare)(void*, void*), void (*print)(void*, FILE*), 
		  void* (*clone)(void*), void (*destroy)(void*)) {

	return (stack_t*)list_init(compare, print, clone, destroy);
}

unsigned int stack_size(stack_t* s) {
	list_t* l = (list_t*)s;
	return list_size(l);
}

void*	stack_push(stack_t* s, void* elem) {
	list_t* l = (list_t*)s;
	return list_addFirst(l, elem);	
}

void*	stack_get(stack_t* s) {
	list_t* l = (list_t*)s;
	return list_getElem(l, 1, false);
}

void*	stack_pop(stack_t* s) {
	list_t* l = (list_t*)s;
	void* ret = list_getElem(l, 1, true);
	list_remove(l, ret);
	return ret;
}

void 	stack_print(stack_t* s, FILE* fd) {
	list_t* l = (list_t*)s;
	list_print(l, fd);
}

int 	stack_destroy(stack_t** s) {
	list_t** l = (list_t**)s;
	return list_destroy(l);
}
