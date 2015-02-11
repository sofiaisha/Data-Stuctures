
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

	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);
	CU_ASSERT (t != NULL);
	CU_ASSERT (binarytree_size(t) == 0);
	CU_ASSERT (binarytree_height(t->root) == 0);

	binarytree_destroy(&t);
}

void bintree_test_empty(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);
	CU_ASSERT (binarytree_size(t) == 0);
	CU_ASSERT (binarytree_height(t->root) == 0);
	binarytree_print(t, stdout);

	binarytree_destroy(&t);
}

void bintree_test_remove1(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);

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
	CU_ASSERT (binarytree_remove(t, bintree_testKey1) == -1); 
	CU_ASSERT (binarytree_size(t) == 1);
	CU_ASSERT (binarytree_height(t->root) == 1);
	binarytree_print(t, stdout);

	binarytree_destroy(&t);
}

void bintree_test_remove2(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);

	/* Degenerate tree: s1 -> s2 -> s3 -> s4 */

	binarytree_add(t, bintree_test_s1);
	binarytree_add(t, bintree_test_s2);
	binarytree_add(t, bintree_test_s3);
	binarytree_add(t, bintree_test_s4);
	CU_ASSERT (binarytree_size(t) == 4);
	CU_ASSERT (binarytree_height(t->root) == 4);
	binarytree_print(t, stdout);

	/* Removing in addition order, i.e. treating nodes as a one-child node */ 
	CU_ASSERT (binarytree_remove(t, bintree_testKey1) == -1); 
	CU_ASSERT (binarytree_size(t) == 4);
	CU_ASSERT (binarytree_height(t->root) == 4);
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_remove(t, bintree_testKey2) == 0); 
	CU_ASSERT (binarytree_size(t) == 3);
	CU_ASSERT (binarytree_height(t->root) == 3);
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_remove(t, bintree_testKey3) == 0); 
	CU_ASSERT (binarytree_size(t) == 2);
	CU_ASSERT (binarytree_height(t->root) == 2);
	binarytree_print(t, stdout);
	CU_ASSERT (binarytree_remove(t, bintree_testKey4) == 0); 
	CU_ASSERT (binarytree_size(t) == 1);
	CU_ASSERT (binarytree_height(t->root) == 1);
	binarytree_print(t, stdout);

	binarytree_destroy(&t);
}

void bintree_test_remove3(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);

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
	CU_ASSERT (binarytree_remove(t, bintree_testKey2) == -1); 
	binarytree_print(t, stdout);
	binarytree_destroy(&t);
}

void bintree_test_add_find1(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);

	CU_ASSERT (binarytree_add(NULL, bintree_test_s1) == NULL);
	CU_ASSERT (binarytree_add(t, NULL) == NULL);
	CU_ASSERT (binarytree_size(t) == 0);
	CU_ASSERT (binarytree_find(t, bintree_testKey1) == NULL); 

	/* Degenerate tree: s1 -> s2 -> s3 -> s4 */

	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_size(t) == 1);
	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_size(t) == 2);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_size(t) == 3);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_size(t) == 4);
	binarytree_print(t, stdout);
	
	/* Finding keys in addition order */

	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey1), bintree_testKey1) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey2), bintree_testKey2) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey3), bintree_testKey3) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey4), bintree_testKey4) == 0); 
	binarytree_destroy(&t);
}

void bintree_test_add_find2(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);

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
	binarytree_destroy(&t);
}

void bintree_test_add_find3(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);

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

	/* Finding keys */

	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey1), bintree_testKey1) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey2), bintree_testKey2) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey3), bintree_testKey3) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey4), bintree_testKey4) == 0); 
	
	binarytree_destroy(&t);
}

void bintree_test_add_find4(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);

	/* Tree:            s3
         *                 /  \
         *                s1  s4
         *                  \  
         *                  s2  
	 */

	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_size(t) == 4);
	binarytree_print(t, stdout);

	/* Finding keys */

	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey1), bintree_testKey1) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey2), bintree_testKey2) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey3), bintree_testKey3) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey4), bintree_testKey4) == 0); 

	binarytree_destroy(&t);
}

void traversal_print(void* elem) {
	printf ("%s ", (char*)elem);
}

void bintree_test_traversal(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
					  bintree_clone, bintree_destroy);
	// Check invalid parameters
	CU_ASSERT (binarytree_traversal(NULL, TRAVERSAL_MODE_PREORDER, traversal_print) == -1); 
	CU_ASSERT (binarytree_traversal(t, -1, traversal_print) == -1); 
	CU_ASSERT (binarytree_traversal(t, 2, traversal_print) == -1); 
	CU_ASSERT (binarytree_traversal(t, TRAVERSAL_MODE_PREORDER, NULL) == -1); 

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
}

void bintree_test_destroy(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
					  bintree_clone, bintree_destroy);
	binarytree_destroy(NULL);
	binarytree_destroy(&t);
}


int init_bintree_suite(void) {
	return 0;
}

int clean_bintree_suite(void) {
	return 0;
}

