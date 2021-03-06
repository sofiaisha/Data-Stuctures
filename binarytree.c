
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
                perror("binarytree_init: can't create new binary tree\n");
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

int binarytree_balanceFactor(tnode_t* n) {
	if (n == NULL) {
		return 0;
	}
	return (binarytree_height(n->left)-binarytree_height(n->right));
}

int binarytree_rotateLeft(binarytree_t* t, tnode_t* n) {
	if ((t == NULL) || (n == NULL) || (n->right == NULL)) {
		return EINVAL;
	}

	// O is the right child of n
	tnode_t* o = n->right;

	// Left subtree of o becomes right subtree of n	
	n->right = o->left;
	if (o->left != NULL) {
		// Update parent pointer in subtree
		o->left->parent = n;
	}

	// Update parent pointer of o
	o->parent = n->parent;
	if (n->parent == NULL) {
		// o is the new root
		t->root = o;
	}
	// Update child pointer in the parent of n
	else if (n == n->parent->left) {
		n->parent->left = o;	
	}
	else {
		n->parent->right = o;	
	}

	// Move n to the left of o
	o->left = n;
	n->parent = o;

	return 0;
}

int binarytree_rotateRight(binarytree_t* t, tnode_t* n) {
	if ((t == NULL) || (n == NULL) || (n->left == NULL)) {
		return EINVAL;
	}

	// O is the left child of n
	tnode_t* o = n->left;

	// Right subtree of o becomes left subtree of n	
	n->left = o->right;
	if (o->right != NULL) {
		// Update parent pointer in subtree
		o->right->parent = n;
	}

	// Update parent pointer of o
	o->parent = n->parent;
	if (n->parent == NULL) {
		// o is the new root
		t->root = o;
	}
	// Update child pointer in the parent of n
	else if (n == n->parent->left) {
		n->parent->left = o;	
	}
	else {
		n->parent->right = o;	
	}

	// Move n to the right of o
	o->right = n;
	n->parent = o;

	return 0;
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
	while ((n != NULL) && (n->right != NULL)) {
		n = n->right;
	}
	return n;
}

tnode_t* binarytree_findMinNode(tnode_t* n) {
	while ((n != NULL) && (n->left != NULL)) {
		n = n->left;
	}
	return n;
}

tnode_t* binarytree_findSuccessor(tnode_t* n) {
	if (n == NULL) {
		return NULL;
	} 
	// If the right subtree exists, the successor of n is its smallest node
	if (n->right != NULL) {
		return binarytree_findMinNode(n->right);
	}
	else {
		// Find the first node whose left child is an ancestor of n
		tnode_t* p = n->parent;
		while ((p != NULL) && (p->right == n)) {
			n = p;
			p = n->parent;
		}
		return p;
	}
}

tnode_t* binarytree_findPredecessor(tnode_t* n) {
	if (n == NULL) {
		return NULL;
	} 
	// If the left subtree exists, the successor of n is its largest node
	if (n->left != NULL) {
		return binarytree_findMaxNode(n->left);
	}
	else {
		// Find the first node whose right child is an ancestor of n
		tnode_t* p = n->parent;
		while ((p != NULL) && (p->left == n)) {
			n = p;
			p = n->parent;
		}
		return p;
	}
}

tnode_t* binarytree_findGrandparent(tnode_t* n) {
	if ((n == NULL) || (n->parent == NULL)) {
		return NULL;
	} 
	return n->parent->parent;
}

tnode_t* binarytree_findUncle(tnode_t* n) {
	tnode_t* gp = binarytree_findGrandparent(n);
	if (gp == NULL) {
		return NULL;
	} 
	if (n->parent == gp->right) {
		return gp->left;
	} 
	else {
		return gp->right;
	}
}

tnode_t* binarytree_findBrother(tnode_t* n) {
	if ((n == NULL) || (n->parent == NULL)) {
		return NULL;
	} 
	if (n == n->parent->right) {
		return n->parent->left;
	} 
	else {
		return n->parent->right;
	}
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
		/* Element is already in the tree */
		return NULL;
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

	/* If the node has a single child, the child will replace it 
	 * If the node has two childs, it will be replaced by its successor
	 */
	tnode_t* repl;
	if ((n->left == NULL) || (n->right == NULL)) {
		repl = n;
	}
	else {
		repl = binarytree_findSuccessor(n);
	}
	/* If the replacement has a non-null child, update its parent pointer.	
	 *
         * Note: the replacement is guaranteed to have only have one child
	 *    - n has a single child : obvious (see above)  
	 *    - n has two childs : it's successor can only have one child 
	 */
	tnode_t* replChild = (repl->left != NULL) ? repl->left : repl->right; 
	if (replChild != NULL) {
		replChild->parent = repl->parent;
	}
	// If the parent pointer is null, the replacement is the new root
	if (repl->parent == NULL) {
		t->root = replChild;
	}
	// If the replacement is a left child, update the left pointer in the parent node to skip it
	else if (repl == repl->parent->left) {
		repl->parent->left = replChild;
	}
	else {
		// If the replacement is a right child, update the right pointer in the parent node to skip it
		repl->parent->right = replChild;
	}

	// Destroy the removed element and replace it with the element from the replacement node
	t->destroy(n->elem);
	n->elem = repl->elem;
	// The replacement node has been unlinked, destroy it	
	free(repl);

	t->size--;
	return 0;
}

void binarytree_print(binarytree_t* t, FILE* fd) {
	if ((t == NULL) || (fd == NULL)) {
		debug_print("Invalid parameter\n");
		return;
	}

	fprintf(fd, "\n( ");
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

int binarytree_destroy(binarytree_t* t) {
	if (t == NULL) {
		debug_print("Invalid parameter\n");
		return EINVAL;
	}
	
	binarytree_destroy_internal(t, t->root);
	free (t);
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
