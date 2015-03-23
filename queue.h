#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdio.h>

#include "list.h"

typedef list_t queue_t;

queue_t* queue_init(int (*compare)(void*, void*), void (*print)(void*, FILE*), 
		  void* (*clone)(void*), void (*destroy)(void*));

unsigned int queue_size(queue_t* q);

void*	queue_push(queue_t* q, void* elem);

void*	queue_pop(queue_t* q);

void*	queue_peek(queue_t* q);

void 	queue_print(queue_t* q, FILE* fd);

int 	queue_destroy(queue_t* q); 

#endif 
