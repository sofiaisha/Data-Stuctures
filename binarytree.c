
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

#define _BT_FIND_ELEM 0
#define _BT_FIND_NODE 1

void* binarytree_find(binarytree_t* t, void* elem) {
	if ((t == NULL) || (elem == NULL)) {
		debug_print("Invalid parameter\n");
		return NULL;
	}
	return binarytree_find_internal(t, t->root,  elem, _BT_FIND_ELEM);
}


void* binarytree_find_internal(binarytree_t* t, tnode_t* n, void* elem, unsigned int mode) {
	if (n == NULL) {
		return NULL;
	}

	int comp = t->compare(elem, n->elem);
	
	if (comp == 0) {
		/* Found the element, return it */
		debug_print_simple("found\n");
		switch (mode) {
			case _BT_FIND_NODE:
				return n;
				break;
			case _BT_FIND_ELEM:
				return n->elem;
				break;
			default:
				assert(0);
				return NULL;
				break;
		}
	} 
	else if (comp < 0) {
		/* Element is smaller, search left subtree */
		debug_print_simple(" L ");
		return binarytree_find_internal(t, n->left, elem, mode);
	} 
	else if (comp > 0) {
		/* Element is larger, search right subtree */
		debug_print_simple(" R ");
		return binarytree_find_internal(t, n->right, elem, mode);
	}
	/* Dummy return to fix compiler warning */
	return NULL;
}

void* binarytree_add(binarytree_t* t, void* elem) {
	if ((t == NULL) || (elem == NULL)) {
		debug_print("Invalid parameter\n");
		return NULL;
	}
	return binarytree_add_internal(t, &(t->root), elem, NULL);
}

void* binarytree_add_internal(binarytree_t* t, tnode_t** n, void* elem, tnode_t* parent) {
	/* We've reached a leaf: add the element */
	if ((*n) == NULL) {
		(*n) = (tnode_t*)malloc(sizeof(tnode_t));
		(*n)->elem = elem;
		(*n)->left = (*n)->right = NULL;
		(*n)->parent = parent;
		t->size++;
		return (*n)->elem;
	}
	
	int comp = t->compare(elem, (*n)->elem);

	if (comp == 0) {
		/* Element is already in the tree, return it */
		return (*n)->elem;
	}
	else if (comp < 0) {
		/* Element is smaller, add to left subtree */
		return binarytree_add_internal(t, &((*n)->left), elem, *n);
	}
	else if (comp > 0) {
		/* Element is larger, add to right subtree */
		return binarytree_add_internal(t, &((*n)->right), elem, *n);
	}

	/* Dummy return to fix compiler warning */
	return NULL;
}

int binarytree_remove(binarytree_t* t, void* elem) {
	if ((t == NULL) || (elem == NULL)) {
		debug_print("Invalid parameter\n");
		return -1;
	}

	tnode_t* n = binarytree_find_internal(t, t->root, elem, _BT_FIND_NODE);
	if (n == NULL) {
		/* Element not in tree */
		return -1;
	}

	if ((n->left == NULL) && (n->right == NULL)) {
		/* The node is a leaf */
		if (n == t->root) {
			/* No parent pointers to update */
			t->root = NULL;
		}
		else {
			if (n->parent->left == n) {
				n->parent->left = NULL;
			}
			else if (n->parent->right == n) {
				n->parent->right = NULL;
			}
			else {
				/* Should never happen */
				assert(0);
			}
		}
	}

	else if ((n->left == NULL) || (n->right == NULL)) {
		/* The node has only one child */
		tnode_t* child = (n->left == NULL ? n->right:n->left);
			
		if (n == t->root) {
			/* No parent pointers to update */
			t->root = child;
		}
		else {
			child->parent = n->parent;
			if (n->parent->left == n) {
				n->parent->left = child;
			}
			else if (n->parent->right == n) {
				n->parent->right = child;
			}	
			else {
				/* Should never happen */
				assert(0);
			}
		}
	}

	else {
		/* The node has two childs */
		/* XXX not implemented yet */
		assert(0);
	}

	t->destroy(n->elem);
	free(n);
	t->size--;
	return 0;
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
