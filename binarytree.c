
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

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

#define MAX(a,b) ( (a) > (b) ? (a) : (b) ) 

unsigned int binarytree_height(tnode_t* n) {
	if (n == NULL) {
		return 0;
	}
	return 1 + MAX(binarytree_height(n->left), binarytree_height(n->right));
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

int binarytree_traversal(binarytree_t* t, unsigned int mode, void(*f)(void*)) {
	if ((t == NULL) || (mode > TRAVERSAL_MODE_POSTORDER) || (f == NULL)) {
		debug_print("Invalid parameter\n");
		return EINVAL;
	}
	binarytree_traversal_internal(t->root, mode, f);
	return 0;

}

void binarytree_traversal_internal(tnode_t* t, unsigned int mode, void(*f)(void*)) {
	if (t==NULL) {
		return;
	}
	if (mode == TRAVERSAL_MODE_PREORDER) {
		f(t->elem);
	}
	binarytree_traversal_internal(t->left, mode, f);
	if (mode == TRAVERSAL_MODE_INORDER) {
		f(t->elem);
	}
	binarytree_traversal_internal(t->right, mode, f);
	if (mode == TRAVERSAL_MODE_POSTORDER) {
		f(t->elem);
	}
}

tnode_t* binarytree_findMaxNode(tnode_t* n) {
	while ((n != NULL) && (n->left != NULL)) {
		n = n->left;
	}
	return n;
}

void* binarytree_add(binarytree_t* t, void* elem) {
	if ((t == NULL) || (elem == NULL)) {
		debug_print("Invalid parameter\n");
		return NULL;
	}
	return binarytree_add_internal(t, &(t->root), elem, NULL);
}

void binarytree_skipNode_internal(tnode_t* old, tnode_t* new) {
	/* Old node is a single child. It will be skipped, 
	 * i.e. its parent and the new node are linked both ways 
	 */
	if (old == old->parent->left) {
		old->parent->left = new;
	}
	else if (old == old->parent->right) {
		old->parent->right = new;
	}
	if (new != NULL) {
		new->parent = old->parent;
	}
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
		return EINVAL;
	}

	tnode_t* n = binarytree_find_internal(t, t->root, elem, _BT_FIND_NODE);
	if (n == NULL) {
		/* Element not in tree */
		return ENOENT;
	}

	if (n == t->root) {
		/* Not implemented yet :) */
		return EPERM;
	}

	/* Count children of node to remove */
	unsigned int children = ((n->left != NULL)?1:0) + ((n->right != NULL)?2:0); 

	tnode_t* tmp = NULL;

	switch (children) {
		case 0: /* The node is a leaf */
			binarytree_skipNode_internal(n, NULL);
			t->destroy(n->elem);
			free(n);
			break;

		case 1: /* The node has only one child (left) */
			binarytree_skipNode_internal(n, n->left);
			t->destroy(n->elem);
			free(n);
			break;

		case 2: /* The node has only one child (right) */
			binarytree_skipNode_internal(n, n->right);
			t->destroy(n->elem);
			free(n);
			break;

		case 3: /* The node has two children */
			/* Find max node in right subtree */
			tmp = binarytree_findMaxNode(n->right);
			/* Replace n element with min element */
			t->destroy(n->elem);
			n->elem = tmp->elem;
			/* Unlink max node */
			binarytree_skipNode_internal(tmp, NULL);
			/* Delete max node */
			free(tmp);
			break;

		default: /* should never happen */
			assert(0);
			break;
	}

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

int binarytree_destroy(binarytree_t** t) {
	if ((t == NULL) || (*t == NULL)) {
		debug_print("Invalid parameter\n");
		return EINVAL;
	}
	
	binarytree_t* tmp = (*t);
	(*t) = NULL;
	binarytree_destroy_internal(tmp, tmp->root);
	free (tmp);
	return 0;
}

void binarytree_destroy_internal(binarytree_t* t, tnode_t* n) {
	if (n != NULL) {
		binarytree_destroy_internal(t, n->left);
		binarytree_destroy_internal(t, n->right);
		t->destroy(n->elem);
		free(n);
	}
}
