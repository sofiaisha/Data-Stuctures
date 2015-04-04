#ifndef __CBUFFER_H
#define __CBUFFER_H

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

int 	cbuffer_size(cbuffer_t* cb);

int	cbuffer_write(cbuffer_t* cb, void* elem);

int 	cbuffer_read(cbuffer_t* cb, int offset, void** elem);

void 	cbuffer_print(cbuffer_t* cb, FILE* fd);

int 	cbuffer_destroy(cbuffer_t* cb); 

#endif 
