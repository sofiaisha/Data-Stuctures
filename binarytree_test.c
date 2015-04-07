
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "binarytree.h"

void bintree_print(void* elem, FILE* fd) {
	fprintf (fd, "%s ", (char*)elem);
}

int bintree_compare(void* s1, void* s2) {
	return strcmp ((char*)s1, (char*)s2);
}

void* bintree_clone(void* s) {
	return strdup((char*)s);
}

void bintree_destroy(void* s) {
	(void)s;
	return;
}

char bintree_test_s1[] = "string1";
char bintree_test_s2[] = "string2";
char bintree_test_s3[] = "string3";
char bintree_test_s4[] = "string4";
char bintree_test_s5[] = "string5";
char bintree_test_s6[] = "string6";

char bintree_testKey1[] = "string1";
char bintree_testKey2[] = "string2";
char bintree_testKey3[] = "string3";
char bintree_testKey4[] = "string4";
char bintree_testKey5[] = "string5";
char bintree_testKey6[] = "string6";

void bintree_test_init(void) {
	CU_ASSERT (binarytree_init(NULL, bintree_print, 
			bintree_clone, NULL) == NULL);
	CU_ASSERT (binarytree_init(bintree_compare, NULL, 
			bintree_clone, bintree_destroy) == NULL);
	CU_ASSERT (binarytree_init(bintree_compare, bintree_print, 
			NULL, bintree_destroy) == NULL);
	CU_ASSERT (binarytree_init(bintree_compare, bintree_print, 
			bintree_clone, NULL) == NULL);

	CU_ASSERT (binarytree_size(NULL) == 0);
	CU_ASSERT (binarytree_height(NULL) == 0);
	CU_ASSERT (binarytree_balanceFactor(NULL) == 0);
	CU_ASSERT (binarytree_findMinNode(NULL) == NULL);
	CU_ASSERT (binarytree_findMaxNode(NULL) == NULL);
	CU_ASSERT (binarytree_findSuccessor(NULL) == NULL);
	CU_ASSERT (binarytree_findPredecessor(NULL) == NULL);
	CU_ASSERT (binarytree_findGrandparent(NULL) == NULL);
	CU_ASSERT (binarytree_findBrother(NULL) == NULL);
	CU_ASSERT (binarytree_findUncle(NULL) == NULL);

	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	CU_ASSERT(binarytree_destroy(t) == 0);
}

void bintree_test_empty(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);
	
	CU_ASSERT (binarytree_size(t) == 0);
	CU_ASSERT (binarytree_height(t->root) == 0);
	CU_ASSERT (binarytree_balanceFactor(t->root) == 0);
	CU_ASSERT (binarytree_findMinNode(t->root) == NULL);
	CU_ASSERT (binarytree_findMaxNode(t->root) == NULL);
	CU_ASSERT (binarytree_findSuccessor(t->root) == NULL);
	CU_ASSERT (binarytree_findPredecessor(t->root) == NULL);
	CU_ASSERT (binarytree_findGrandparent(t->root) == NULL);
	CU_ASSERT (binarytree_findBrother(t->root) == NULL);
	CU_ASSERT (binarytree_findUncle(t->root) == NULL);
	binarytree_print(t, stdout);

	CU_ASSERT(binarytree_destroy(t) == 0);
}

void bintree_test_remove1(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	CU_ASSERT (binarytree_remove(NULL, bintree_testKey4) == EINVAL); 
	CU_ASSERT (binarytree_remove(t, NULL) == EINVAL); 
	CU_ASSERT (binarytree_remove(t, bintree_testKey4) == ENOENT); 

	/* Degenerate tree: s1 -> s2 -> s3 -> s4 */
	binarytree_add(t, bintree_test_s1);
	CU_ASSERT (binarytree_size(t) == 1);
	CU_ASSERT (binarytree_height(t->root) == 1);
	binarytree_add(t, bintree_test_s2);
	CU_ASSERT (binarytree_height(t->root) == 2);
	CU_ASSERT (binarytree_size(t) == 2);
	binarytree_add(t, bintree_test_s3);
	CU_ASSERT (binarytree_height(t->root) == 3);
	CU_ASSERT (binarytree_size(t) == 3);
	binarytree_add(t, bintree_test_s4);
	CU_ASSERT (binarytree_size(t) == 4);
	CU_ASSERT (binarytree_height(t->root) == 4);
	binarytree_print(t, stdout);

	/* Removing in reverse order, i.e. treating each node as a leaf */ 
	CU_ASSERT (binarytree_remove(t, bintree_testKey4) == 0); 
	CU_ASSERT (binarytree_size(t) == 3);
	CU_ASSERT (binarytree_height(t->root) == 3);
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_remove(t, bintree_testKey3) == 0); 
	CU_ASSERT (binarytree_size(t) == 2);
	CU_ASSERT (binarytree_height(t->root) == 2);
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_remove(t, bintree_testKey2) == 0); 
	CU_ASSERT (binarytree_size(t) == 1);
	CU_ASSERT (binarytree_height(t->root) == 1);
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_remove(t, bintree_testKey1) == 0); 
	CU_ASSERT (binarytree_size(t) == 0);
	CU_ASSERT (binarytree_height(t->root) == 0);
	binarytree_print(t, stdout);

	CU_ASSERT(binarytree_destroy(t) == 0);
}

void bintree_test_remove2(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	/* Degenerate tree: s1 -> s2 -> s3 -> s4 */

	binarytree_add(t, bintree_test_s1);
	binarytree_add(t, bintree_test_s2);
	binarytree_add(t, bintree_test_s3);
	binarytree_add(t, bintree_test_s4);
	CU_ASSERT (binarytree_size(t) == 4);
	CU_ASSERT (binarytree_height(t->root) == 4);
	binarytree_print(t, stdout);

	/* Removing in addition order, i.e. treating nodes as a one-child node */ 
	CU_ASSERT (binarytree_remove(t, bintree_testKey1) == 0); 
	CU_ASSERT (binarytree_size(t) == 3);
	CU_ASSERT (binarytree_height(t->root) == 3);
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_remove(t, bintree_testKey2) == 0); 
	CU_ASSERT (binarytree_size(t) == 2);
	CU_ASSERT (binarytree_height(t->root) == 2);
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_remove(t, bintree_testKey3) == 0); 
	CU_ASSERT (binarytree_size(t) == 1);
	CU_ASSERT (binarytree_height(t->root) == 1);
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_remove(t, bintree_testKey4) == 0); 
	CU_ASSERT (binarytree_size(t) == 0);
	CU_ASSERT (binarytree_height(t->root) == 0);
	binarytree_print(t, stdout);

	CU_ASSERT(binarytree_destroy(t) == 0);
}

void bintree_test_remove3(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	/* Tree:            s2
         *                 /  \
         *                s1  s5
         *                   /  \ 
         *                  s3  s6 
         *                   \     
         *                   s4     
	 */

	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_add(t, bintree_test_s5) == bintree_test_s5);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_add(t, bintree_test_s6) == bintree_test_s6);
	CU_ASSERT (binarytree_size(t) == 6);
	CU_ASSERT (binarytree_height(t->root) == 4);
	binarytree_print(t, stdout);

	CU_ASSERT (binarytree_remove(t, bintree_testKey5) == 0); 
	CU_ASSERT (binarytree_height(t->root) == 4);
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_remove(t, bintree_testKey6) == 0); 
	CU_ASSERT (binarytree_height(t->root) == 3);
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_remove(t, bintree_testKey4) == 0); 
	CU_ASSERT (binarytree_height(t->root) == 2);
	CU_ASSERT (binarytree_remove(t, bintree_testKey3) == 0); 
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_height(t->root) == 2);
	CU_ASSERT (binarytree_remove(t, bintree_testKey1) == 0); 
	CU_ASSERT (binarytree_height(t->root) == 1);
	CU_ASSERT (binarytree_remove(t, bintree_testKey2) == 0); 
	binarytree_print(t, stdout);

	CU_ASSERT(binarytree_destroy(t) == 0);
}

void bintree_test_add_find1(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	CU_ASSERT (binarytree_add(NULL, bintree_test_s1) == NULL);
	CU_ASSERT (binarytree_add(t, NULL) == NULL);
	CU_ASSERT (binarytree_size(t) == 0);

	CU_ASSERT (binarytree_find(NULL, bintree_testKey1) == NULL); 
	CU_ASSERT (binarytree_find(t, NULL) == NULL); 

	CU_ASSERT (binarytree_find(t, bintree_testKey1) == NULL); 

	/* Degenerate tree: s1 -> s2 -> s3 -> s4 */

	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_size(t) == 1);
	CU_ASSERT (binarytree_add(t, bintree_test_s1) == NULL);
	CU_ASSERT (binarytree_size(t) == 1);
	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_size(t) == 2);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_size(t) == 3);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_size(t) == 4);
	binarytree_print(t, stdout);
	
	/* Checking balance factor for each node */

	CU_ASSERT (binarytree_balanceFactor(t->root) == -3);
	CU_ASSERT (binarytree_balanceFactor(t->root->right) == -2);
	CU_ASSERT (binarytree_balanceFactor(t->root->right->right) == -1);
	CU_ASSERT (binarytree_balanceFactor(t->root->right->right->right) == 0);

	/* Finding keys in addition order */

	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey1), bintree_testKey1) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey2), bintree_testKey2) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey3), bintree_testKey3) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey4), bintree_testKey4) == 0); 

	CU_ASSERT(binarytree_destroy(t) == 0);
}

void bintree_test_add_find2(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	/* Degenerate tree: s1 -> s2 -> s3 -> s4 */

	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_size(t) == 4);
	binarytree_print(t, stdout);

	/* Finding keys in reverse addition order */

	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey4), bintree_testKey4) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey3), bintree_testKey3) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey2), bintree_testKey2) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey1), bintree_testKey1) == 0); 

	tnode_t* n = binarytree_findMaxNode(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey4) == 0); 
	n = binarytree_findMinNode(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey1) == 0); 

	n = binarytree_findSuccessor(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey2) == 0); 
	n = binarytree_findSuccessor(t->root->right);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey3) == 0); 
	n = binarytree_findSuccessor(t->root->right->right);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey4) == 0); 
	n = binarytree_findSuccessor(t->root->right->right->right);
	CU_ASSERT (n == NULL);

	n = binarytree_findPredecessor(t->root);
	CU_ASSERT (n == NULL);
	n = binarytree_findPredecessor(t->root->right);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey1) == 0); 
	n = binarytree_findPredecessor(t->root->right->right);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey2) == 0); 
	n = binarytree_findPredecessor(t->root->right->right->right);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey3) == 0); 

	CU_ASSERT(binarytree_destroy(t) == 0);
}

void bintree_test_add_find3(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	/* Tree:            s2
         *                 /  \
         *                s1  s4
         *                   / 
         *                  s3  
	 */

	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_size(t) == 4);
	binarytree_print(t, stdout);

	/* Checking balance factor for each node */

	CU_ASSERT (binarytree_balanceFactor(t->root) == -1);
	CU_ASSERT (binarytree_balanceFactor(t->root->left) == 0);
	CU_ASSERT (binarytree_balanceFactor(t->root->right) == 1);
	CU_ASSERT (binarytree_balanceFactor(t->root->right->left) == 0);

	/* Find grandfather, uncle and brother */

	CU_ASSERT (binarytree_findGrandparent(t->root) == NULL);
	CU_ASSERT (binarytree_findGrandparent(t->root->left) == NULL);
	CU_ASSERT (binarytree_findGrandparent(t->root->right) == NULL);
	CU_ASSERT (binarytree_findGrandparent(t->root->right->left) == t->root);

	CU_ASSERT (binarytree_findUncle(t->root) == NULL);
	CU_ASSERT (binarytree_findUncle(t->root->left) == NULL);
	CU_ASSERT (binarytree_findUncle(t->root->right) == NULL);
	CU_ASSERT (binarytree_findUncle(t->root->right->left) == t->root->left);

	CU_ASSERT (binarytree_findBrother(t->root) == NULL);
	CU_ASSERT (binarytree_findBrother(t->root->left) == t->root->right);
	CU_ASSERT (binarytree_findBrother(t->root->right) == t->root->left);
	CU_ASSERT (binarytree_findBrother(t->root->right->left) == NULL);

	/* Finding keys */

	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey1), bintree_testKey1) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey2), bintree_testKey2) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey3), bintree_testKey3) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey4), bintree_testKey4) == 0); 
	
	tnode_t* n = binarytree_findMaxNode(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey4) == 0); 
	n = binarytree_findMinNode(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey1) == 0); 

	n = binarytree_findSuccessor(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey3) == 0); 
	n = binarytree_findSuccessor(t->root->left);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey2) == 0); 
	n = binarytree_findSuccessor(t->root->right);
	CU_ASSERT (n == NULL);
	n = binarytree_findSuccessor(t->root->right->left);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey4) == 0); 

	n = binarytree_findPredecessor(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey1) == 0); 
	n = binarytree_findPredecessor(t->root->left);
	CU_ASSERT (n == NULL);
	n = binarytree_findPredecessor(t->root->right);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey3) == 0); 
	n = binarytree_findPredecessor(t->root->right->left);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey2) == 0); 

	CU_ASSERT(binarytree_destroy(t) == 0);
}

void bintree_test_add_find4(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	/* Tree:            s3
         *                 / \
         *                s1  s4 
         *                  \
         *                  s2  
	 */

	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_size(t) == 4);
	binarytree_print(t, stdout);

	/* Checking balance factor for each node */

	CU_ASSERT (binarytree_balanceFactor(t->root) == 1);
	CU_ASSERT (binarytree_balanceFactor(t->root->left) == -1);
	CU_ASSERT (binarytree_balanceFactor(t->root->left->right) == 0);
	CU_ASSERT (binarytree_balanceFactor(t->root->right) == 0);

	/* Find grandfather, uncle and brother */

	CU_ASSERT (binarytree_findGrandparent(t->root) == NULL);
	CU_ASSERT (binarytree_findGrandparent(t->root->left) == NULL);
	CU_ASSERT (binarytree_findGrandparent(t->root->right) == NULL);
	CU_ASSERT (binarytree_findGrandparent(t->root->left->right) == t->root);

	CU_ASSERT (binarytree_findUncle(t->root) == NULL);
	CU_ASSERT (binarytree_findUncle(t->root->left) == NULL);
	CU_ASSERT (binarytree_findUncle(t->root->right) == NULL);
	CU_ASSERT (binarytree_findUncle(t->root->left->right) == t->root->right);

	CU_ASSERT (binarytree_findBrother(t->root) == NULL);
	CU_ASSERT (binarytree_findBrother(t->root->left) == t->root->right);
	CU_ASSERT (binarytree_findBrother(t->root->right) == t->root->left);
	CU_ASSERT (binarytree_findBrother(t->root->right->left) == NULL);

	/* Finding keys */

	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey1), bintree_testKey1) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey2), bintree_testKey2) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey3), bintree_testKey3) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey4), bintree_testKey4) == 0); 

	tnode_t* n = binarytree_findMaxNode(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey4) == 0); 
	n = binarytree_findMinNode(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey1) == 0); 

	n = binarytree_findSuccessor(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey4) == 0); 
	n = binarytree_findSuccessor(t->root->right);
	CU_ASSERT (n == NULL);
	n = binarytree_findSuccessor(t->root->left);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey2) == 0); 
	n = binarytree_findSuccessor(t->root->left->right);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey3) == 0); 

	n = binarytree_findPredecessor(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey2) == 0); 
	n = binarytree_findPredecessor(t->root->right);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey3) == 0); 
	n = binarytree_findPredecessor(t->root->left);
	CU_ASSERT (n == NULL);
	n = binarytree_findPredecessor(t->root->left->right);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey1) == 0); 

	CU_ASSERT(binarytree_destroy(t) == 0);
}

void traversal_print(void* elem) {
	printf ("%s ", (char*)elem);
}

void bintree_test_traversal(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
					  bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	// Check invalid parameters
	CU_ASSERT (binarytree_traversal(NULL, TRAVERSAL_MODE_PREORDER, traversal_print) == EINVAL); 
	CU_ASSERT (binarytree_traversal(t, -1, traversal_print) == EINVAL); 
	CU_ASSERT (binarytree_traversal(t, 4, traversal_print) == EINVAL); 
	CU_ASSERT (binarytree_traversal(t, TRAVERSAL_MODE_PREORDER, NULL) == EINVAL); 

	/* Tree:            s2
         *                 /  \
         *                s1  s4
         *                   / 
         *                  s3  
	 */

	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);

	printf("\n");
	printf("Preorder: ");
	binarytree_traversal(t, TRAVERSAL_MODE_PREORDER, traversal_print); 
	printf("\n");
	printf("Inorder: ");
	binarytree_traversal(t, TRAVERSAL_MODE_INORDER, traversal_print); 
	printf("\n");
	printf("Postorder: ");
	binarytree_traversal(t, TRAVERSAL_MODE_POSTORDER, traversal_print); 
	printf("\n");

	CU_ASSERT(binarytree_destroy(t) == 0);
}

void bintree_test_walk(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
					  bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	/* Tree:            s2
         *                 /  \
         *                s1  s4
         *                   / \
         *                  s3 s5 
	 *                      \
 	 *			s6
	 */

	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_add(t, bintree_test_s5) == bintree_test_s5);
	CU_ASSERT (binarytree_add(t, bintree_test_s6) == bintree_test_s6);

	/* Checking balance factor for each node */

	CU_ASSERT (binarytree_balanceFactor(t->root) == -2);
	CU_ASSERT (binarytree_balanceFactor(t->root->left) == 0);
	CU_ASSERT (binarytree_balanceFactor(t->root->right) == -1);
	CU_ASSERT (binarytree_balanceFactor(t->root->right->left) == 0);
	CU_ASSERT (binarytree_balanceFactor(t->root->right->right) == -1);
	CU_ASSERT (binarytree_balanceFactor(t->root->right->right->right) == 0);

	// s2 to s1 and back
	tnode_t* tmp = t->root;
	CU_ASSERT(strcmp(tmp->left->elem, bintree_test_s1) == 0);
	tmp = tmp->left;
	CU_ASSERT(tmp->left == NULL);
	CU_ASSERT(tmp->right == NULL);
	CU_ASSERT(strcmp(tmp->parent->elem, bintree_test_s2) == 0);
	tmp = tmp->parent;

	// s2 to s4 and back
	CU_ASSERT(strcmp(tmp->right->elem, bintree_test_s4) == 0);
	tmp = tmp->right;
	CU_ASSERT(strcmp(tmp->parent->elem, bintree_test_s2) == 0);
	tmp = tmp->parent;
	
	// s2 to s3 and back
	CU_ASSERT(strcmp(tmp->right->elem, bintree_test_s4) == 0);
	tmp = tmp->right;
	CU_ASSERT(strcmp(tmp->left->elem, bintree_test_s3) == 0);
	tmp = tmp->left;
	CU_ASSERT(tmp->left == NULL);
	CU_ASSERT(tmp->right == NULL);
	CU_ASSERT(strcmp(tmp->parent->elem, bintree_test_s4) == 0);
	tmp = tmp->parent;
	CU_ASSERT(strcmp(tmp->parent->elem, bintree_test_s2) == 0);
	tmp = tmp->parent;

	//s2 to s5 and back	
	CU_ASSERT(strcmp(tmp->right->elem, bintree_test_s4) == 0);
	tmp = tmp->right;
	CU_ASSERT(strcmp(tmp->right->elem, bintree_test_s5) == 0);
	tmp = tmp->right;
	CU_ASSERT(strcmp(tmp->parent->elem, bintree_test_s4) == 0);
	tmp = tmp->parent;
	CU_ASSERT(strcmp(tmp->parent->elem, bintree_test_s2) == 0);
	tmp = tmp->parent;

	//s2 to s6 and back	
	CU_ASSERT(strcmp(tmp->right->elem, bintree_test_s4) == 0);
	tmp = tmp->right;
	CU_ASSERT(strcmp(tmp->right->elem, bintree_test_s5) == 0);
	tmp = tmp->right;
	CU_ASSERT(strcmp(tmp->right->elem, bintree_test_s6) == 0);
	tmp = tmp->right;
	CU_ASSERT(tmp->left == NULL);
	CU_ASSERT(tmp->right == NULL);
	CU_ASSERT(strcmp(tmp->parent->elem, bintree_test_s5) == 0);
	tmp = tmp->parent;
	CU_ASSERT(strcmp(tmp->parent->elem, bintree_test_s4) == 0);
	tmp = tmp->parent;
	CU_ASSERT(strcmp(tmp->parent->elem, bintree_test_s2) == 0);
	tmp = tmp->parent;

	tnode_t* n = binarytree_findMaxNode(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey6) == 0); 
	n = binarytree_findMinNode(t->root);
	CU_ASSERT (strcmp((char*)n->elem, bintree_testKey1) == 0); 

	CU_ASSERT(binarytree_destroy(t) == 0);
}

void bintree_test_rotateLeft(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
					  bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	/* Tree:            s2
         *                 /  \
         *                s1  s4
         *                   / \
         *                  s3 s5 
	 *                      \
 	 *			s6
	 */

	/* Rotated tree:    s4
         *                 /  \
         *                s2  s5
         *               / \   \
         *              s1 s3  s6 
	 */

	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_add(t, bintree_test_s5) == bintree_test_s5);
	CU_ASSERT (binarytree_add(t, bintree_test_s6) == bintree_test_s6);

	CU_ASSERT (binarytree_rotateLeft(NULL, t->root) == EINVAL);
	CU_ASSERT (binarytree_rotateLeft(t, NULL) == EINVAL);
	CU_ASSERT (binarytree_rotateLeft(t, t->root->left) == EINVAL);

	tnode_t* n = t->root;
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_rotateLeft(t, n) == 0);
	binarytree_print(t, stdout);

	/* Checking balance factor for each node */

	CU_ASSERT (binarytree_balanceFactor(t->root) == 0);
	CU_ASSERT (binarytree_balanceFactor(t->root->left) == 0);
	CU_ASSERT (binarytree_balanceFactor(t->root->left->left) == 0);
	CU_ASSERT (binarytree_balanceFactor(t->root->left->right) == 0);
	CU_ASSERT (binarytree_balanceFactor(t->root->right) == -1);
	CU_ASSERT (binarytree_balanceFactor(t->root->right->right) == 0);

	// s4
	CU_ASSERT(strcmp(t->root->elem, bintree_test_s4) == 0);
	CU_ASSERT(t->root->parent == NULL);
	// s2
	CU_ASSERT(strcmp(t->root->left->elem, bintree_test_s2) == 0);
	CU_ASSERT(strcmp(t->root->left->parent->elem, bintree_test_s4) == 0);
	// s1	
	CU_ASSERT(strcmp(t->root->left->left->elem, bintree_test_s1) == 0);
	CU_ASSERT(strcmp(t->root->left->left->parent->elem, bintree_test_s2) == 0);
	CU_ASSERT(t->root->left->left->left == NULL);
	CU_ASSERT(t->root->left->left->right == NULL);
	// s3
	CU_ASSERT(strcmp(t->root->left->right->elem, bintree_test_s3) == 0);
	CU_ASSERT(strcmp(t->root->left->right->parent->elem, bintree_test_s2) == 0);
	CU_ASSERT(t->root->left->right->left == NULL);
	CU_ASSERT(t->root->left->right->right == NULL);
	// s5
	CU_ASSERT(strcmp(t->root->right->elem, bintree_test_s5) == 0);
	CU_ASSERT(strcmp(t->root->right->parent->elem, bintree_test_s4) == 0);
	CU_ASSERT(t->root->right->left == NULL);
	// s6
	CU_ASSERT(strcmp(t->root->right->right->elem, bintree_test_s6) == 0);
	CU_ASSERT(strcmp(t->root->right->right->parent->elem, bintree_test_s5) == 0);

	CU_ASSERT(binarytree_destroy(t) == 0);
}

void bintree_test_rotateRight(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
					  bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	/* Tree:            s2
         *                 /  \
         *                s1  s4
         *                   / \
         *                  s3 s5 
	 *                      \
 	 *			s6
	 */

	/* Rotated tree:    s2
         *                 /  \
         *                s1  s3
         *                     \
         *                     s4 
	 *                      \
	 *                      s5
	 *                       \
	 *                       s6
	 */

	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_add(t, bintree_test_s5) == bintree_test_s5);
	CU_ASSERT (binarytree_add(t, bintree_test_s6) == bintree_test_s6);

	CU_ASSERT (binarytree_rotateRight(NULL, t->root) == EINVAL);
	CU_ASSERT (binarytree_rotateRight(t, NULL) == EINVAL);
	CU_ASSERT (binarytree_rotateRight(t, t->root->left) == EINVAL);

	tnode_t* n = t->root->right;
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_rotateRight(t, n) == 0);
	binarytree_print(t, stdout);

	/* Checking balance factor for each node */

	CU_ASSERT (binarytree_balanceFactor(t->root) == -3);
	CU_ASSERT (binarytree_balanceFactor(t->root->left) == 0);
	CU_ASSERT (binarytree_balanceFactor(t->root->right) == -3);
	CU_ASSERT (binarytree_balanceFactor(t->root->right->right) == -2);
	CU_ASSERT (binarytree_balanceFactor(t->root->right->right->right) == -1);
	CU_ASSERT (binarytree_balanceFactor(t->root->right->right->right->right) == 0);

	// s2
	CU_ASSERT(strcmp(t->root->elem, bintree_test_s2) == 0);
	CU_ASSERT(t->root->parent == NULL);
	// s1
	CU_ASSERT(strcmp(t->root->left->elem, bintree_test_s1) == 0);
	CU_ASSERT(strcmp(t->root->left->parent->elem, bintree_test_s2) == 0);
	CU_ASSERT(t->root->left->left == NULL);
	CU_ASSERT(t->root->left->right == NULL);
	// s3	
	CU_ASSERT(strcmp(t->root->right->elem, bintree_test_s3) == 0);
	CU_ASSERT(strcmp(t->root->right->parent->elem, bintree_test_s2) == 0);
	CU_ASSERT(t->root->right->left == NULL);
	CU_ASSERT(strcmp(t->root->right->right->elem, bintree_test_s4) == 0);
	// s4
	CU_ASSERT(strcmp(t->root->right->right->elem, bintree_test_s4) == 0);
	CU_ASSERT(strcmp(t->root->right->right->parent->elem, bintree_test_s3) == 0);
	CU_ASSERT(t->root->right->right->left == NULL);
	CU_ASSERT(strcmp(t->root->right->right->right->elem, bintree_test_s5) == 0);
	// s5
	CU_ASSERT(strcmp(t->root->right->right->right->elem, bintree_test_s5) == 0);
	CU_ASSERT(strcmp(t->root->right->right->right->parent->elem, bintree_test_s4) == 0);
	CU_ASSERT(t->root->right->right->right->left == NULL);
	CU_ASSERT(strcmp(t->root->right->right->right->right->elem, bintree_test_s6) == 0);
	// s6
	CU_ASSERT(strcmp(t->root->right->right->right->right->elem, bintree_test_s6) == 0);
	CU_ASSERT(strcmp(t->root->right->right->right->right->parent->elem, bintree_test_s5) == 0);
	CU_ASSERT(t->root->right->right->right->right->left == NULL);
	CU_ASSERT(t->root->right->right->right->right->right == NULL);

	CU_ASSERT(binarytree_destroy(t) == 0);
}
void bintree_test_destroy(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
					  bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);

	CU_ASSERT(binarytree_destroy(NULL) == EINVAL);
	CU_ASSERT(binarytree_destroy(t) == 0);
}


int init_bintree_suite(void) {
	return 0;
}

int clean_bintree_suite(void) {
	return 0;
}

