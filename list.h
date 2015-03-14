#ifndef __LIST_H
#define __LIST_H

#include <stdio.h>
#include <stdbool.h>

typedef struct node_t node_t;
struct node_t {
	void*	elem;
	node_t*	next;
	node_t*	prev;
}; 

typedef struct {
	node_t*		head;
	node_t*		tail;
	unsigned int 	size;
	int		(*compare)(void*, void*);
	void		(*print)(void*, FILE*);
	void*		(*clone)(void*);
	void		(*destroy)(void*);
} list_t;

#define ADD_FIRST 0
#define ADD_LAST  1

list_t*	list_init(int (*compare)(void*, void*), void (*print)(void*, FILE*), 
		  void* (*clone)(void*), void (*destroy)(void*));

unsigned int list_size(list_t* l);

void* list_add(list_t* l, void* elem, unsigned int mode); 
void* list_addFirst(list_t* l, void* elem);
void* list_addLast(list_t* l, void* elem);

void*	list_getElem(list_t*l, unsigned int index, bool clone);

int 	list_remove(list_t* l, void* elem);

void* 	list_find(list_t* l, void* elem);

void 	list_print(list_t* l, FILE* fd);

int 	list_destroy(list_t** l); 

#endif 
