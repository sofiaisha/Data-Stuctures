#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#include <stdio.h>

#include "list.h"

typedef struct {
	list_t**	bins;
	unsigned int 	bin_count;
	unsigned int 	size;
	unsigned int 	(*hash)(void * key);
} hashtable_t;

typedef struct {
	void* key;
	void* value;
} hashtable_entry_t;	

hashtable_t* hashtable_init(unsigned int bins, int (*compare)(void*, void*),
              void (*print)(void*, FILE*), void (*destroy)(void*),
              unsigned int (*hash)(void*));

void* 	hashtable_add(hashtable_t* h, void* key, void* value); 

int 	hashtable_remove(hashtable_t* h, void* key);

void* 	hashtable_find(hashtable_t* h, void* key);

void 	hashtable_print(hashtable_t* h, FILE* fd);

void 	hashtable_destroy(hashtable_t** h); 

#endif 
