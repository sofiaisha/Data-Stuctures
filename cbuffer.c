#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#include "cbuffer.h"
#include "util.h"

cbuffer_t* cbuffer_init(unsigned int size, 
	void (*print)(void*, FILE*), void* (*clone)(void*), void (*destroy)(void*)) {

	if ( (size <=0) || (print == NULL) || (clone == NULL) || (destroy == NULL))  {
		debug_print("invalid parameter\n");
		return NULL;
	}

	cbuffer_t* cb = (cbuffer_t*)malloc(sizeof(cbuffer_t));
	if (cb == NULL) {
		perror("can't create new circular buffer, errno=%d");
		return NULL;
	}
	cb->entries = (void*)malloc(size*sizeof(void*));
	if (cb->entries == NULL) {
		perror("can't create new circular buffer, errno=%d");
		free(cb);
		return NULL;
	}

	cb->size 	= 0;
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

int cbuffer_destroy(cbuffer_t** cb) {
	if ((cb == NULL) || (*cb == NULL)) {
		debug_print("invalid parameter\n");
		return EINVAL;
	}

	for (unsigned int i=0;i<(*cb)->size;i++) {
		if ((*cb)->entries[i]) {
			free((*cb)->entries[i]);
		}
	}  	
	free((*cb)->entries);
	free(*cb);

	return 0;
}
