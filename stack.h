#ifndef __STACK_H
#define __STACK_H

#include <stdio.h>

#include "list.h"

typedef list_t stack_t;

stack_t* stack_init(int (*compare)(void*, void*), void (*print)(void*, FILE*), 
		  void* (*clone)(void*), void (*destroy)(void*));

unsigned int stack_size(stack_t* s);

void*	stack_push(stack_t* s, void* elem);

void*	stack_get(stack_t* s);

void*	stack_pop(stack_t* s);

void 	stack_print(stack_t* s, FILE* fd);

int 	stack_destroy(stack_t* s); 

#endif 
