#include <stdio.h>

#include "stack.h"
#include "list.h"

stack_t* stack_init(int (*compare)(void*, void*), void (*print)(void*, FILE*), 
		  void* (*clone)(void*), void (*destroy)(void*)) {

	return (stack_t*)list_init(compare, print, clone, destroy);
}

unsigned int stack_size(stack_t* s) {
	return list_size((list_t*)s);
}

void*	stack_push(stack_t* s, void* elem) {
	return list_addFirst((list_t*)s, elem);	
}

void*	stack_get(stack_t* s) {
	return list_getElem((list_t*)s, 0, false);
}

void*	stack_pop(stack_t* s) {
	void* ret = list_getElem((list_t*)s, 0, true);
	list_remove((list_t*)s, ret);
	return ret;
}

void 	stack_print(stack_t* s, FILE* fd) {
	list_print((list_t*)s, fd);
}

int 	stack_destroy(stack_t** s) {
	return list_destroy((list_t**)s);
}
