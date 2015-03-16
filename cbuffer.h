#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdio.h>

typedef struct cbuffer_t cbuffer_t;
struct cbuffer_t {
        void**  	entries;
	unsigned int	head;
	unsigned int 	size;
	void 		(*print)(void*, FILE*);
	void* 		(*clone)(void*);
	void 		(*destroy)(void*);
};

cbuffer_t* cbuffer_init(unsigned int size,
	void (*print)(void*, FILE*), void* (*clone)(void*), void (*destroy)(void*));

int cbuffer_size(cbuffer_t* c);

void*	cbuffer_write(cbuffer_t* c, void* elem);

void*	cbuffer_read(cbuffer_t* q, unsigned int index);

void 	cbuffer_print(cbuffer_t* q, FILE* fd);

int 	cbuffer_destroy(cbuffer_t** q); 

#endif 
