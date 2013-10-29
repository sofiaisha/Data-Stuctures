#ifndef __BINARYTREE_H
#define __BINARYTREE_H

#include <stdio.h>
#include <semaphore.h>
#include <stdbool.h>

typedef struct tnode_t tnode_t;
struct tnode_t {
	void*		elem;
	tnode_t*	left;
	tnode_t*	right;
}; 

typedef struct {
	tnode_t*	root;
	unsigned int 	size;
	sem_t		sem;
	int		(*compare)(void*, void*);
	void		(*print)(void*, FILE*);
	void*		(*clone)(void*);
	void		(*destroy)(void*);
} binarytree_t;

binarytree_t*	binarytree_init(int (*compare)(void*, void*), 
			void (*print)(void*, FILE*), 
			void* (*clone)(void*), void (*destroy)(void*));

unsigned int	binarytree_size(binarytree_t* t);

void* 		binarytree_add(binarytree_t* t, void* elem);

void* 		binarytree_find(binarytree_t* t, void* elem);

void 		binarytree_print(binarytree_t* t, FILE* fd);

void		binarytree_destroy(binarytree_t** t);

/* Internal functions used recursion, do not use! */

void* 		binarytree_add_internal(binarytree_t* t, tnode_t** n, void* elem);
void* 		binarytree_find_internal(binarytree_t* t, tnode_t* n, void* elem);
void 		binarytree_print_internal(binarytree_t* t, tnode_t* n, FILE* fd);
void 		binarytree_destroy_internal(binarytree_t* t, tnode_t* n);
#endif
