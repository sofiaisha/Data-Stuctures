#ifndef __BINARYTREE_H
#define __BINARYTREE_H

#include <stdio.h>
#include <stdbool.h>

typedef struct tnode_t tnode_t;
struct tnode_t {
	void*		elem;
	tnode_t*	parent;
	tnode_t*	left;
	tnode_t*	right;
}; 

typedef struct {
	tnode_t*	root;
	unsigned int 	size;
	int		(*compare)(void*, void*);
	void		(*print)(void*, FILE*);
	void*		(*clone)(void*);
	void		(*destroy)(void*);
} binarytree_t;

binarytree_t*	binarytree_init(int (*compare)(void*, void*), 
			void (*print)(void*, FILE*), 
			void* (*clone)(void*), void (*destroy)(void*));

unsigned int	binarytree_size(binarytree_t* t);

unsigned int	binarytree_height(tnode_t* n);

int		binarytree_balanceFactor(tnode_t* n);

void* 		binarytree_add(binarytree_t* t, void* elem);

int 		binarytree_remove(binarytree_t* t, void* elem);

void* 		binarytree_find(binarytree_t* t, void* elem);

tnode_t*	binarytree_findMaxNode(tnode_t* n);

tnode_t*	binarytree_findMinNode(tnode_t* n);

tnode_t*	binarytree_findSuccessor(tnode_t* n);

tnode_t*	binarytree_findPredecessor(tnode_t* n);

tnode_t*	binarytree_findGrandparent(tnode_t* n);

tnode_t*	binarytree_findUncle(tnode_t* n);

tnode_t*	binarytree_findBrother(tnode_t* n);

int		binarytree_rotateLeft(binarytree_t* t, tnode_t* n);

int		binarytree_rotateRight(binarytree_t* t, tnode_t* n);

#define 	TRAVERSAL_MODE_PREORDER 	1
#define 	TRAVERSAL_MODE_INORDER 		2
#define 	TRAVERSAL_MODE_POSTORDER 	3
int		binarytree_traversal(binarytree_t* t, unsigned int mode, void(*f)(void*));

void 		binarytree_print(binarytree_t* t, FILE* fd);

int		binarytree_destroy(binarytree_t* t);

/* Internal functions, do not use! */

void* binarytree_add_internal(binarytree_t* t, tnode_t** n, void* elem, tnode_t* parent);
void* binarytree_find_internal(binarytree_t* t, tnode_t* n, void* elem, unsigned int mode);
void  binarytree_traversal_internal(tnode_t* t, unsigned int mode, void(*f)(void*));
void  binarytree_print_internal(binarytree_t* t, tnode_t* n, FILE* fd);
void  binarytree_destroy_internal(binarytree_t* t, tnode_t* n);
#endif
