
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
char list_test_s5[] = "string5";

void list_test_init(void) {
	CU_ASSERT (list_init(NULL, lst_print, lst_clone, NULL) == NULL);
	CU_ASSERT (list_init(lst_compare, NULL, lst_clone, lst_destroy) == NULL);
	CU_ASSERT (list_init(lst_compare, lst_print, NULL, lst_destroy) == NULL);

	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);

	CU_ASSERT (list_size(NULL) == EINVAL);
	CU_ASSERT (list_size(l) == 0);

	CU_ASSERT (list_destroy(l) == 0);
}

void list_test_empty(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);

	list_print(l, stderr);
	CU_ASSERT (list_getElem(l, 0, false) == NULL);
	CU_ASSERT (list_find(l, list_test_s1) == NULL);
	CU_ASSERT (list_remove(l, list_test_s1) == ENOENT);

	CU_ASSERT (list_destroy(l) == 0);
}

void list_test_addFirst(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);

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

	CU_ASSERT (list_destroy(l) == 0);
}

void list_test_addLast(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);

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

	CU_ASSERT (list_destroy(l) == 0);
}

void list_test_addSort(void) {

	/* Sort ascending */ 
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);

	CU_ASSERT( list_addSortAsc(NULL, list_test_s2) == NULL);
	CU_ASSERT( list_addSortAsc(l, NULL) == NULL);

	char*s;
	s = list_addSortAsc(l, list_test_s2);
	CU_ASSERT(strcmp(s, list_test_s2) == 0);	// Add first
	s = list_addSortAsc(l, list_test_s1);		// Add new head
	CU_ASSERT(strcmp(s, list_test_s1) == 0);
	s = list_addSortAsc(l, list_test_s4);		// Add new tail
	CU_ASSERT(strcmp(s, list_test_s4) == 0);
	s = list_addSortAsc(l, list_test_s3);		// Add in the middle
	CU_ASSERT(strcmp(s, list_test_s3) == 0);

	s = list_getElem(l, 0, false);
	CU_ASSERT(strcmp(s, list_test_s1) == 0);
	s = list_getElem(l, 1, false);
	CU_ASSERT(strcmp(s, list_test_s2) == 0);
	s = list_getElem(l, 2, false);
	CU_ASSERT(strcmp(s, list_test_s3) == 0);
	s = list_getElem(l, 3, false);
	CU_ASSERT(strcmp(s, list_test_s4) == 0);

	char list_test_s31[] = "string31";
	CU_ASSERT (strcmp((char*)list_findSortedAsc(l, list_test_s1), list_test_s1) == 0);
	CU_ASSERT (strcmp((char*)list_findSortedAsc(l, list_test_s2), list_test_s2) == 0);
	CU_ASSERT (strcmp((char*)list_findSortedAsc(l, list_test_s3), list_test_s3) == 0);
	CU_ASSERT (strcmp((char*)list_findSortedAsc(l, list_test_s4), list_test_s4) == 0);
	CU_ASSERT (list_findSortedAsc(l, list_test_s31) == NULL);

	CU_ASSERT (list_destroy(l) == 0);

	/* Sort descending */ 
	l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);

	s = list_addSortDesc(l, list_test_s2);
	CU_ASSERT(strcmp(s, list_test_s2) == 0);	// Add first
	s = list_addSortDesc(l, list_test_s1);		// Add new tail
	CU_ASSERT(strcmp(s, list_test_s1) == 0);
	s = list_addSortDesc(l, list_test_s4);		// Add new head
	CU_ASSERT(strcmp(s, list_test_s4) == 0);
	s = list_addSortDesc(l, list_test_s3);		// Add in the middle
	CU_ASSERT(strcmp(s, list_test_s3) == 0);

	s = list_getElem(l, 0, false);
	CU_ASSERT(strcmp(s, list_test_s4) == 0);
	s = list_getElem(l, 1, false);
	CU_ASSERT(strcmp(s, list_test_s3) == 0);
	s = list_getElem(l, 2, false);
	CU_ASSERT(strcmp(s, list_test_s2) == 0);
	s = list_getElem(l, 3, false);
	CU_ASSERT(strcmp(s, list_test_s1) == 0);

	CU_ASSERT (strcmp((char*)list_findSortedDesc(l, list_test_s1), list_test_s1) == 0);
	CU_ASSERT (strcmp((char*)list_findSortedDesc(l, list_test_s2), list_test_s2) == 0);
	CU_ASSERT (strcmp((char*)list_findSortedDesc(l, list_test_s3), list_test_s3) == 0);
	CU_ASSERT (strcmp((char*)list_findSortedDesc(l, list_test_s4), list_test_s4) == 0);
	CU_ASSERT (list_findSortedDesc(l, list_test_s31) == NULL);

	CU_ASSERT (list_destroy(l) == 0);
}

void list_test_getElem(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);

	CU_ASSERT(list_getElem(NULL, 0, false) == NULL); 
	CU_ASSERT(list_getElem(l, 0, false) == NULL); 

	CU_ASSERT(list_addFirst(l, list_test_s1) != NULL);
	CU_ASSERT(list_addFirst(l, list_test_s2) != NULL);
	CU_ASSERT(list_addFirst(l, list_test_s3) != NULL);

	CU_ASSERT(list_getElem(l, -1, false) == NULL); 
	CU_ASSERT(list_getElem(l, 4, false) == NULL); 

	char* s;
	s = list_getElem(l, 0, false);
	CU_ASSERT (s == list_test_s3);
	s = list_getElem(l, 0, true);
	CU_ASSERT(s != NULL);
	CU_ASSERT(s != list_test_s3);
	CU_ASSERT (strcmp(s, list_test_s3) == 0);
	free(s);

	s = list_getElem(l, 1, false);
	CU_ASSERT (s == list_test_s2);
	s = list_getElem(l, 1, true);
	CU_ASSERT (s != list_test_s2);
	CU_ASSERT(s != NULL);
	CU_ASSERT (strcmp(s, list_test_s2) == 0);
	free(s);

	s = list_getElem(l, 2, false);
	CU_ASSERT (s == list_test_s1);
	s = list_getElem(l, 2, true);
	CU_ASSERT(s != NULL);
	CU_ASSERT (s != list_test_s1);
	CU_ASSERT (strcmp(s, list_test_s1) == 0);
	free(s);

	CU_ASSERT (list_destroy(l) == 0);
}

void list_test_walk(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT(l != NULL);

	list_addLast(l, list_test_s1);
	list_addLast(l, list_test_s2);
	list_addLast(l, list_test_s3);
	list_addLast(l, list_test_s4);

	node_t* tmp = l->head;
	CU_ASSERT (strcmp(tmp->elem, list_test_s1) == 0);
	tmp = tmp->next;
	CU_ASSERT (strcmp(tmp->elem, list_test_s2) == 0);
	tmp = tmp->next;
	CU_ASSERT (strcmp(tmp->elem, list_test_s3) == 0);
	tmp = tmp->next;
	CU_ASSERT (strcmp(tmp->elem, list_test_s4) == 0);
	tmp = tmp->next;
	CU_ASSERT (tmp == NULL);

	tmp = l->tail;
	CU_ASSERT (strcmp(tmp->elem, list_test_s4) == 0);
	tmp = tmp->prev;
	CU_ASSERT (strcmp(tmp->elem, list_test_s3) == 0);
	tmp = tmp->prev;
	CU_ASSERT (strcmp(tmp->elem, list_test_s2) == 0);
	tmp = tmp->prev;
	CU_ASSERT (strcmp(tmp->elem, list_test_s1) == 0);
	tmp = tmp->prev;
	CU_ASSERT (tmp == NULL);

	CU_ASSERT (list_destroy(l) == 0);
}

void list_test_print(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);

	list_print(NULL, stderr);
	list_print(l, NULL);
	CU_ASSERT (list_addFirst(l, list_test_s1) != NULL);
	CU_ASSERT (list_addFirst(l, list_test_s2) != NULL);
	CU_ASSERT (list_addFirst(l, list_test_s3) != NULL);
	list_print(l, stderr);

	CU_ASSERT (list_destroy(l) == 0);
}

void list_test_find(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);

	CU_ASSERT (list_find(NULL, list_test_s1) == NULL);
	CU_ASSERT (list_find(l, NULL) == NULL);

	CU_ASSERT(list_addFirst(l, list_test_s1) != NULL);
	CU_ASSERT(list_addFirst(l, list_test_s2) != NULL);
	CU_ASSERT(list_addFirst(l, list_test_s3) != NULL);

	CU_ASSERT (strcmp((char*)list_find(l, list_test_s1), list_test_s1) == 0);
	CU_ASSERT (strcmp((char*)list_find(l, list_test_s2), list_test_s2) == 0);
	CU_ASSERT (strcmp((char*)list_find(l, list_test_s3), list_test_s3) == 0);
	CU_ASSERT (list_find(l, list_test_s4) == NULL);

	CU_ASSERT (list_destroy(l) == 0);
}

void list_test_remove(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);

	CU_ASSERT(list_remove(NULL, list_test_s1) == EINVAL);
	CU_ASSERT(list_remove(l, NULL) == EINVAL);

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

	CU_ASSERT (list_destroy(l) == 0);
}

void list_test_save(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);

	CU_ASSERT(list_addLast(l, list_test_s1) != NULL);
	CU_ASSERT(list_addLast(l, list_test_s2) != NULL);
	CU_ASSERT(list_addLast(l, list_test_s3) != NULL);

	FILE* f = fopen("file2.bin", "w+");
	CU_ASSERT (f != NULL);
	l->elemSize = 7;

	CU_ASSERT (list_save(NULL, f) == EINVAL);
	CU_ASSERT (list_save(l, NULL) == EINVAL);
	CU_ASSERT (list_save(l, f) == list_size(l));

	fclose(f);
	CU_ASSERT (list_destroy(l) == 0);

	l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);
	l->elemSize = 7;
	f = fopen("file2.bin", "r");
	CU_ASSERT (f != NULL);

	CU_ASSERT (list_load(NULL, f) == EINVAL);
	CU_ASSERT (list_load(l, NULL) == EINVAL);
	/*CU_ASSERT (list_load(l, f) == 3);
	CU_ASSERT (list_size(l) == 3);
	list_print(l, stdout);
	CU_ASSERT (list_find(l, list_test_s1) != NULL);
	CU_ASSERT (strncmp((char*)list_find(l, list_test_s1), list_test_s1, l->elemSize) == 0);
	printf("%s\n", l->head->next->elem);
	CU_ASSERT (list_find(l, list_test_s2) != NULL);
	CU_ASSERT (list_find(l, list_test_s4) == NULL);*/

	fclose(f);
	CU_ASSERT (list_destroy(l) == 0);
}

void list_test_destroy(void) {
	list_t* l = list_init(lst_compare, lst_print, lst_clone, lst_destroy);
	CU_ASSERT (l != NULL);
	CU_ASSERT (list_destroy(NULL) == EINVAL);
	CU_ASSERT (list_destroy(l) == 0);
}

int init_list_suite(void) {
	return 0;
}

int clean_list_suite(void) {
	return 0;
}

