
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include <assert.h>

#include "binarytree.h"
#include "util.h"

binarytree_t*	binarytree_init(int (*compare)(void*, void*), 
			void (*print)(void*, FILE*), 
			void* (*clone)(void*), void (*destroy)(void*)) {

	if (     (compare == NULL) || (print == NULL) 
		|| (clone == NULL) || (destroy == NULL)) {
		debug_print("Invalid parameter\n");
		return NULL;
	}

        binarytree_t* t = (binarytree_t*)malloc(sizeof(binarytree_t));
        if (t == NULL) { 
                perror("can't create new binary tree, errno=%d");
                return NULL;
        }       

        if (sem_init(&(t->sem), 0, 1) != 0) { 
                perror("binarytree_init: can't create semaphore");
                free(t);
                return NULL;
        }       
       
	t->root		= NULL;
	t->size		= 0;
        t->compare      = compare;
        t->print        = print;
        t->destroy      = destroy;
        t->clone        = clone;

        return t;
}

unsigned int binarytree_size(binarytree_t* t) {
	if (t == NULL) {
		debug_print("Invalid parameter\n");
		return 0;
	}

	return t->size;
}

void* binarytree_find(binarytree_t* t, void* elem) {
	if ((t == NULL) || (elem == NULL)) {
		debug_print("Invalid parameter\n");
		return NULL;
	}
	return binarytree_find_internal(t, t->root,  elem);
}

void* binarytree_find_internal(binarytree_t* t, tnode_t* n, void* elem) {
	if (n == NULL) {
		return NULL;
	}
	
	if (t->compare(elem, n->elem) == 0) {
		/* Found the element, return it */
		debug_print_simple("found\n");
		return n->elem;
	} 

	if (t->compare(elem, n->elem) < 0) {
		/* Element is smaller, search left subtree */
		debug_print_simple(" L ");
		return binarytree_find_internal(t, n->left, elem);
	} 

	if (t->compare(elem, n->elem) > 0) {
		/* Element is larger, search right subtree */
		debug_print_simple(" R ");
		return binarytree_find_internal(t, n->right, elem);
	}

	/* Dummy return to fix compiler warning */
	return NULL;
}

void* binarytree_add(binarytree_t* t, void* elem) {
	if ((t == NULL) || (elem == NULL)) {
		debug_print("Invalid parameter\n");
		return NULL;
	}
	return binarytree_add_internal(t, &(t->root), elem);
}

void* binarytree_add_internal(binarytree_t* t, tnode_t** n, void* elem) {
	/* We've reached a leaf: add the element */
	if ((*n) == NULL) {
		(*n) = (tnode_t*)malloc(sizeof(tnode_t));
		(*n)->elem = elem;
		(*n)->left = (*n)->right = NULL;
		t->size++;
		return (*n)->elem;
	}
	
	if (t->compare(elem, (*n)->elem) == 0) {
		/* Element is already in the tree, return it */
		return (*n)->elem;
	}
 
	if (t->compare(elem, (*n)->elem) < 0) {
		/* Element is smaller, add to left subtree */
		return binarytree_add_internal(t, &((*n)->left), elem);
	}
	 
	if (t->compare(elem, (*n)->elem) > 0) {
		/* Element is larger, add to right subtree */
		return binarytree_add_internal(t, &((*n)->right), elem);
	}

	/* Dummy return to fix compiler warning */
	return NULL;
}

void binarytree_print(binarytree_t* t, FILE* fd) {
	if ((t == NULL) || (fd == NULL)) {
		debug_print("Invalid parameter\n");
		return;
	}

	fprintf(fd, " ( ");
	binarytree_print_internal(t, t->root, fd);
	fprintf(fd, " )\n");
}

void binarytree_print_internal(binarytree_t* t, tnode_t* n, FILE* fd) {
	if (n != NULL) {
		t->print(n->elem, fd);
		fprintf(fd, " L ");
		binarytree_print_internal(t, n->left, fd);
		fprintf(fd, " R ");
		binarytree_print_internal(t, n->right,fd);
	}
}

void binarytree_destroy(binarytree_t** t) {
	if ((t == NULL) || (*t == NULL)) {
		debug_print("Invalid parameter\n");
		return;
	}
	
	binarytree_t* tmp = (*t);
	(*t) = NULL;
	binarytree_destroy_internal(tmp, tmp->root);
	free (tmp);
}

void binarytree_destroy_internal(binarytree_t* t, tnode_t* n) {
	if (n != NULL) {
		binarytree_destroy_internal(t, n->left);
		binarytree_destroy_internal(t, n->right);
		t->destroy(n->elem);
		free(n);
	}
}
