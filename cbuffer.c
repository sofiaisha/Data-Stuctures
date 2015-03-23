#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#include "cbuffer.h"
#include "util.h"

cbuffer_t* cbuffer_init(unsigned int size, 
	void (*print)(void*, FILE*), void* (*clone)(void*), void (*destroy)(void*)) {
	if ( (size == 0) || (print == NULL) || (clone == NULL) )  {
		debug_print("invalid parameter\n");
		return NULL;
	}

	cbuffer_t* cb = (cbuffer_t*)malloc(sizeof(cbuffer_t));
	if (cb == NULL) {
		perror("cbuffer_init: can't create new circular buffer\n");
		return NULL;
	}
	cb->entries = (void*)malloc(size*sizeof(void*));
	if (cb->entries == NULL) {
		perror("cbuffer_init: can't create circular buffer entries\n");
		free(cb);
		return NULL;
	}

	cb->size 	= size;
	cb->head	= 0;
	cb->print 	= print;
	cb->destroy 	= destroy;
	cb->clone 	= clone;

	return cb;
}

int cbuffer_size(cbuffer_t* cb) {
	if (cb == NULL) {
		debug_print("invalid parameter\n");
		return EINVAL;
	}
	else {
		return cb->size;
	}
}

int cbuffer_write(cbuffer_t* cb, void* elem) {
	if ((cb == NULL) || (elem == NULL)) {
		debug_print("invalid parameter\n");
		return EINVAL;
	}
	(cb->head)++;
	if (cb->head == cb->size) {
		cb->head = 0;
	}
	if (cb->destroy != NULL) {
		cb->destroy(cb->entries[cb->head]);
	}
	cb->entries[cb->head] = elem;
	return 0;
}

int cbuffer_read(cbuffer_t* cb, int offset, void** element) {
	if ( (cb == NULL) || (cb->size ==0) || (element == NULL)) {
		debug_print("invalid parameter\n");
		return EINVAL;
	}

	offset = cb->head + offset;			
	offset = offset % cb->size;
	if (offset < 0) {
		offset += cb->size;	
	}
	*element = cb->entries[offset];
	return 0;
}
	
int cbuffer_destroy(cbuffer_t** cb) {
	if ((cb == NULL) || (*cb == NULL)) {
		debug_print("invalid parameter\n");
		return EINVAL;
	}

	if ((*cb)->destroy != NULL) {
		for (unsigned int i=0;i<(*cb)->size;i++) {
			if ((*cb)->entries[i] != NULL) {
				((*cb)->destroy)((*cb)->entries[i]);
			}
		}
	}  	
	if ((*cb)->entries != NULL) {
		free((*cb)->entries);
	}
	free(*cb);

	return 0;
}
