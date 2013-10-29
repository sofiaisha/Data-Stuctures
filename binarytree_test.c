
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
	return strdup(s);
}

void bintree_destroy(void* s) {
	return;
}

char bintree_test_s1[] = "string1";
char bintree_test_s2[] = "string2";
char bintree_test_s3[] = "string3";
char bintree_test_s4[] = "string4";

char bintree_testKey1[] = "string1";
char bintree_testKey2[] = "string2";
char bintree_testKey3[] = "string3";
char bintree_testKey4[] = "string4";

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

	binarytree_destroy(&t);
}

void bintree_test_empty(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);
	binarytree_print(t, stdout);

	binarytree_destroy(&t);
}

void bintree_test_add_find(void) {
	binarytree_t* t = binarytree_init(bintree_compare, bintree_print, 
                                          bintree_clone, bintree_destroy);

	CU_ASSERT (binarytree_add(NULL, bintree_test_s1) == NULL);
	CU_ASSERT (binarytree_add(t, NULL) == NULL);
	CU_ASSERT (binarytree_size(t) == 0);
	CU_ASSERT (binarytree_find(t, bintree_testKey1) == NULL); 

	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_size(t) == 1);
	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_size(t) == 2);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_size(t) == 3);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_size(t) == 4);
	fprintf(stdout, "\n");
	binarytree_print(t, stdout);
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey1), bintree_testKey1) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey2), bintree_testKey2) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey3), bintree_testKey3) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey4), bintree_testKey4) == 0); 
	binarytree_destroy(&t);

	t = binarytree_init(bintree_compare, bintree_print, 
		bintree_clone, bintree_destroy);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_size(t) == 4);
	binarytree_print(t, stdout);
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey1), bintree_testKey1) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey2), bintree_testKey2) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey3), bintree_testKey3) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey4), bintree_testKey4) == 0); 
	binarytree_destroy(&t);

	t = binarytree_init(bintree_compare, bintree_print, 
		bintree_clone, bintree_destroy);
	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_size(t) == 4);
	binarytree_print(t, stdout);
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey1), bintree_testKey1) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey2), bintree_testKey2) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey3), bintree_testKey3) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey4), bintree_testKey4) == 0); 
	binarytree_destroy(&t);

	t = binarytree_init(bintree_compare, bintree_print, 
		bintree_clone, bintree_destroy);
	CU_ASSERT (binarytree_add(t, bintree_test_s2) == bintree_test_s2);
	CU_ASSERT (binarytree_add(t, bintree_test_s4) == bintree_test_s4);
	CU_ASSERT (binarytree_add(t, bintree_test_s3) == bintree_test_s3);
	CU_ASSERT (binarytree_add(t, bintree_test_s1) == bintree_test_s1);
	CU_ASSERT (binarytree_size(t) == 4);
	binarytree_print(t, stdout);
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey1), bintree_testKey1) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey2), bintree_testKey2) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey3), bintree_testKey3) == 0); 
	CU_ASSERT (strcmp(binarytree_find(t, bintree_testKey4), bintree_testKey4) == 0); 
	binarytree_destroy(&t);
}


/*

void list_test_find(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);

	CU_ASSERT (list_find(NULL, list_test_s1) == NULL);
	CU_ASSERT (list_find(l, NULL) == NULL);

	list_addFirst(l, list_test_s1);
	list_addFirst(l, list_test_s2);
	list_addFirst(l, list_test_s3);

	CU_ASSERT (strcmp((char*)list_find(l, list_test_s1), list_test_s1) == 0);
	CU_ASSERT (strcmp((char*)list_find(l, list_test_s2), list_test_s2) == 0);
	CU_ASSERT (strcmp((char*)list_find(l, list_test_s3), list_test_s3) == 0);
	CU_ASSERT (list_find(l, list_test_s4) == NULL);

	list_destroy(&l);
}

void list_test_remove(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);

	CU_ASSERT(list_remove(NULL, list_test_s1) == -1);
	CU_ASSERT(list_remove(l, NULL) == -1);

	list_addFirst(l, list_test_s1);
	list_addFirst(l, list_test_s2);
	list_addFirst(l, list_test_s3);

	CU_ASSERT(list_remove(l, list_test_s1) == 0);
	CU_ASSERT(list_find(l, list_test_s1) == NULL);
	CU_ASSERT (list_size(l) == 2);

	CU_ASSERT(list_remove(l, list_test_s2) == 0);
	CU_ASSERT(list_find(l, list_test_s2) == NULL);
	CU_ASSERT (list_size(l) == 1);

	CU_ASSERT(list_remove(l, list_test_s3) == 0);
	CU_ASSERT(list_find(l, list_test_s3) == NULL);
	CU_ASSERT (list_size(l) == 0);

	list_destroy(&l);
}

*/

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

