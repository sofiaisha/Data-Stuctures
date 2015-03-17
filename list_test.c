
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "list.h"

void lst_print(void* elem, FILE* fd) {
	fprintf (fd, "%s ", (char*)elem);
}

int lst_compare(void* s1, void* s2) {
	return strcmp ((char*)s1, (char*)s2);
}

void* lst_clone(void* s) {
	return strdup((char*)s);
}

void lst_destroy(void* s) {
	return;
}

char list_test_s1[] = "string1";
char list_test_s2[] = "string2";
char list_test_s3[] = "string3";
char list_test_s4[] = "string4";

void list_test_init(void) {
	CU_ASSERT (list_init(NULL, lst_print, lst_clone, NULL) == NULL);
	CU_ASSERT (list_init(lst_compare, NULL, lst_clone, lst_destroy) == NULL);
	CU_ASSERT (list_init(lst_compare, lst_print, NULL, lst_destroy) == NULL);

	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);
	CU_ASSERT (list_size(l) == 0);

	CU_ASSERT (list_destroy(&l) == 0);
}

void list_test_empty(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);

	list_print(l, stderr);
	CU_ASSERT (list_getElem(l, 0, false) == NULL);
	CU_ASSERT (list_find(l, list_test_s1) == NULL);
	CU_ASSERT (list_remove(l, list_test_s1) == -2);

	CU_ASSERT (list_destroy(&l) == 0);
}

void list_test_addFirst(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);

	CU_ASSERT (list_add(NULL, list_test_s1, ADD_FIRST) == NULL);
	CU_ASSERT (list_add(l, NULL, ADD_FIRST) == NULL);
	CU_ASSERT (list_add(l, list_test_s1, -1) == NULL);
	CU_ASSERT (list_size(l) == 0);

	char* s = (char*)list_addFirst(l, list_test_s1);
	CU_ASSERT(s != NULL);
	CU_ASSERT(strcmp(s, list_test_s1) == 0);
	CU_ASSERT(strcmp((char*)(l->head->elem), list_test_s1) == 0);
	CU_ASSERT(strcmp((char*)(l->tail->elem), list_test_s1) == 0);
	CU_ASSERT (list_size(l) == 1);

	s = (char*)list_addFirst(l, list_test_s2);
	CU_ASSERT(s != NULL);
	CU_ASSERT(strcmp(s, list_test_s2) == 0);
	CU_ASSERT(strcmp((char*)(l->head->elem), list_test_s2) == 0);
	CU_ASSERT(strcmp((char*)(l->tail->elem), list_test_s1) == 0);
	CU_ASSERT (list_size(l) == 2);

	s = list_addFirst(l, list_test_s3);
	CU_ASSERT(s != NULL);
	CU_ASSERT(strcmp(s, list_test_s3) == 0);
	CU_ASSERT(strcmp((char*)(l->head->elem), list_test_s3) == 0);
	CU_ASSERT(strcmp((char*)(l->tail->elem), list_test_s1) == 0);
	CU_ASSERT (list_size(l) == 3);

	CU_ASSERT (list_destroy(&l) == 0);
}

void list_test_addLast(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);

	CU_ASSERT (list_add(NULL, list_test_s1, ADD_LAST) == NULL);
	CU_ASSERT (list_add(l, NULL, ADD_LAST) == NULL);
	CU_ASSERT (list_add(l, list_test_s1, -1) == NULL);

	CU_ASSERT (list_size(l) == 0);

	char* s = (char*)list_addLast(l, list_test_s1);
	CU_ASSERT(s != NULL);
	CU_ASSERT(strcmp(s, list_test_s1) == 0);
	CU_ASSERT(strcmp((char*)(l->head->elem), list_test_s1) == 0);
	CU_ASSERT(strcmp((char*)(l->tail->elem), list_test_s1) == 0);
	CU_ASSERT (list_size(l) == 1);

	s = list_addLast(l, list_test_s2);
	CU_ASSERT(s != NULL);
	CU_ASSERT(strcmp(s, list_test_s2) == 0);
	CU_ASSERT(strcmp((char*)(l->tail->elem), list_test_s2) == 0);
	CU_ASSERT(strcmp((char*)(l->head->elem), list_test_s1) == 0);
	CU_ASSERT (list_size(l) == 2);

	s = list_addLast(l, list_test_s3);
	CU_ASSERT(s != NULL);
	CU_ASSERT(strcmp(s, list_test_s3) == 0);
	CU_ASSERT(strcmp((char*)(l->tail->elem), list_test_s3) == 0);
	CU_ASSERT(strcmp((char*)(l->head->elem), list_test_s1) == 0);
	CU_ASSERT (list_size(l) == 3);

	CU_ASSERT (list_destroy(&l) == 0);
}

void list_test_getElem(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);

	CU_ASSERT(list_getElem(NULL, 0, false) == NULL); 
	CU_ASSERT(list_getElem(l, 0, false) == NULL); 

	CU_ASSERT(list_addFirst(l, list_test_s1) != NULL);
	CU_ASSERT(list_addFirst(l, list_test_s2) != NULL);
	CU_ASSERT(list_addFirst(l, list_test_s3) != NULL);

	CU_ASSERT(list_getElem(l, 0, false) == NULL); 
	CU_ASSERT(list_getElem(l, 4, false) == NULL); 

	char* s;
	s = list_getElem(l, 1, false);
	CU_ASSERT (s == list_test_s3);
	s = list_getElem(l, 1, true);
	CU_ASSERT(s != NULL);
	CU_ASSERT (strcmp(s, list_test_s3) == 0);
	free(s);

	s = list_getElem(l, 2, false);
	CU_ASSERT (s == list_test_s2);
	s = list_getElem(l, 2, true);
	CU_ASSERT (s != list_test_s2);
	CU_ASSERT(s != NULL);
	CU_ASSERT (strcmp(s, list_test_s2) == 0);
	free(s);

	s = list_getElem(l, 3, false);
	CU_ASSERT (s == list_test_s1);
	s = list_getElem(l, 3, true);
	CU_ASSERT(s != NULL);
	CU_ASSERT (s != list_test_s1);
	CU_ASSERT (strcmp(s, list_test_s1) == 0);
	free(s);

	CU_ASSERT (list_destroy(&l) == 0);
}

void list_test_print(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);

	list_print(NULL, stderr);
	list_print(l, NULL);
	list_addFirst(l, list_test_s1);
	list_addFirst(l, list_test_s2);
	list_addFirst(l, list_test_s3);
	list_print(l, stderr);

	CU_ASSERT (list_destroy(&l) == 0);
}

void list_test_find(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);

	CU_ASSERT (list_find(NULL, list_test_s1) == NULL);
	CU_ASSERT (list_find(l, NULL) == NULL);

	CU_ASSERT(list_addFirst(l, list_test_s1) != NULL);
	CU_ASSERT(list_addFirst(l, list_test_s2) != NULL);
	CU_ASSERT(list_addFirst(l, list_test_s3) != NULL);

	CU_ASSERT (strcmp((char*)list_find(l, list_test_s1), list_test_s1) == 0);
	CU_ASSERT (strcmp((char*)list_find(l, list_test_s2), list_test_s2) == 0);
	CU_ASSERT (strcmp((char*)list_find(l, list_test_s3), list_test_s3) == 0);
	CU_ASSERT (list_find(l, list_test_s4) == NULL);

	CU_ASSERT (list_destroy(&l) == 0);
}

void list_test_remove(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);

	CU_ASSERT(list_remove(NULL, list_test_s1) == -1);
	CU_ASSERT(list_remove(l, NULL) == -1);

	CU_ASSERT(list_addFirst(l, list_test_s1) != NULL);
	CU_ASSERT(list_addFirst(l, list_test_s2) != NULL);
	CU_ASSERT(list_addFirst(l, list_test_s3) != NULL);

	CU_ASSERT(list_remove(l, list_test_s1) == 0);
	CU_ASSERT(list_find(l, list_test_s1) == NULL);
	CU_ASSERT (list_size(l) == 2);

	CU_ASSERT(list_remove(l, list_test_s2) == 0);
	CU_ASSERT(list_find(l, list_test_s2) == NULL);
	CU_ASSERT (list_size(l) == 1);

	CU_ASSERT(list_remove(l, list_test_s3) == 0);
	CU_ASSERT(list_find(l, list_test_s3) == NULL);
	CU_ASSERT (list_size(l) == 0);

	CU_ASSERT (list_destroy(&l) == 0);
}

void list_test_destroy(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (list_destroy(NULL) == EINVAL);
	CU_ASSERT (list_destroy(&l) == 0);
}

int init_list_suite(void) {
	return 0;
}

int clean_list_suite(void) {
	return 0;
}

