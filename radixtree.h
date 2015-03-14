#ifndef __RADIXTREE_STRING_H
#define __RADIXTREE_STRING_H

#include <stdio.h>
#include <semaphore.h>
#include <stdbool.h>

typedef struct rtnode_t rtnode_t;
struct rtnode_t {
	char		c;
	rtnode_t*	parent;
	rtnode_t*	childs[];
}; 

typedef struct {
	rtnode_t*	root;
	unsigned int 	alphabetSize;
	unsigned int 	size;
	sem_t		sem;
} radixtree_t;

radixtree_t*	radixtree_init(unsigned int alphabetSize);

unsigned int	radixtree_size(radixtree_t* t);

unsigned int 	radixtree_height(radixtree_t* t, rtnode_t* n);

char* 		radixtree_add(radixtree_t* t, char* s);

char*		radixtree_find(radixtree_t* t, char* s);

void 		radixtree_print(radixtree_t* t, FILE* fd);

void		radixtree_destroy(radixtree_t** t);

/* Internal functions, do not use! */

void radixtree_destroy_internal(radixtree_t* t, rtnode_t* n);

/*
void* binarytree_add_internal(binarytree_t* t, tnode_t** n, void* elem, tnode_t* parent);
void* binarytree_find_internal(binarytree_t* t, tnode_t* n, void* elem, unsigned int mode);
void  binarytree_traversal_internal(tnode_t* t, unsigned int mode, void(*f)(void*));
void  binarytree_print_internal(binarytree_t* t, tnode_t* n, FILE* fd);
*/
#endif
