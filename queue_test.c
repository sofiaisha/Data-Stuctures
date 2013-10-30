
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "queue.h"
#include "queue_test.h"

void que_print(void* elem, FILE* fd) {
	fprintf (fd, "%s ", (char*)elem);
}

int que_compare(void* s1, void* s2) {
	return strcmp ( (char*)s1, (char*)s2 );
}

void* que_clone(void* s) {
	return strdup((char*)s);
}

void que_destroy(void* s) {
	return;
}

char que_test_s1[] = "string1";
char que_test_s2[] = "string2";
char que_test_s3[] = "string3";
char que_test_s4[] = "string4";

void que_test_init(void) {
	CU_ASSERT (queue_init(NULL, que_print, que_clone, que_destroy) == NULL);
	CU_ASSERT (queue_init(que_compare, NULL, que_clone, que_destroy) == NULL);
	CU_ASSERT (queue_init(que_compare, que_print, NULL, que_destroy) == NULL);
	CU_ASSERT (queue_init(que_compare, que_print, que_clone, NULL) == NULL);

	queue_t* q = queue_init(que_compare, que_print, que_clone, que_destroy);
	queue_destroy(&q);
}

void que_test_empty(void) {
	queue_t* s = queue_init(que_compare, que_print, que_clone, que_destroy);

	queue_print(s, stderr);
	CU_ASSERT (queue_get(s) == NULL);
	CU_ASSERT (queue_dequeue(s) == NULL);

	queue_destroy(&s);
}

void que_test_enqueue(void) {
	queue_t* q = queue_init(que_compare, que_print, que_clone, que_destroy);
	
	CU_ASSERT (queue_enqueue(NULL, que_test_s1) == NULL);
	CU_ASSERT (queue_enqueue(q, NULL) == NULL);

	CU_ASSERT (queue_enqueue(q, que_test_s1) == que_test_s1);
	CU_ASSERT (queue_enqueue(q, que_test_s2) == que_test_s2);
	CU_ASSERT (queue_enqueue(q, que_test_s3) == que_test_s3);
	CU_ASSERT (queue_enqueue(q, que_test_s4) == que_test_s4);

	queue_destroy(&q);
}

void que_test_get(void) {
	queue_t* q = queue_init(que_compare, que_print, que_clone, que_destroy);
	
	queue_enqueue(q, que_test_s1);
	queue_enqueue(q, que_test_s2);
	queue_enqueue(q, que_test_s3);
	queue_enqueue(q, que_test_s4);

	char* str = queue_get(q);
	CU_ASSERT(str == que_test_s1);
	CU_ASSERT (strcmp(str, que_test_s1) == 0);

	str = queue_dequeue(q);
	CU_ASSERT(str != que_test_s1);
	CU_ASSERT (strcmp(str, que_test_s1) == 0);
	free(str);

	str = queue_get(q);
	CU_ASSERT(str == que_test_s2);
	CU_ASSERT (strcmp(str, que_test_s2) == 0);

	queue_destroy(&q);
}
	
void que_test_dequeue(void) {
	queue_t* q = queue_init(que_compare, que_print, que_clone, que_destroy);
	
	queue_enqueue(q, que_test_s1);
	queue_enqueue(q, que_test_s2);
	queue_enqueue(q, que_test_s3);
	queue_enqueue(q, que_test_s4);

	char* str = queue_dequeue(q);
	CU_ASSERT(str != que_test_s1);
	CU_ASSERT (strcmp(str, que_test_s1) == 0);
	free(str);

	str = queue_dequeue(q);
	CU_ASSERT(str != que_test_s2);
	CU_ASSERT (strcmp(str, que_test_s2) == 0);
	free(str);

	str = queue_dequeue(q);
	CU_ASSERT(str != que_test_s3);
	CU_ASSERT (strcmp(str, que_test_s3) == 0);
	free(str);

	str = queue_dequeue(q);
	CU_ASSERT(str != que_test_s4);
	CU_ASSERT (strcmp(str, que_test_s4) == 0);
	free(str);

	str = queue_dequeue(q);
	CU_ASSERT(str == NULL);
	CU_ASSERT(queue_size(q) == 0);
	
	queue_destroy(&q);
}
	
void que_test_print(void) {
	queue_t* q = queue_init(que_compare, que_print, que_clone, que_destroy);
	queue_enqueue(q, que_test_s1);
	queue_enqueue(q, que_test_s2);
	queue_enqueue(q, que_test_s3);
	queue_enqueue(q, que_test_s4);

	queue_print(q, stdout);

	queue_destroy(&q);
}

void que_test_destroy(void) {
	queue_t* q = queue_init(que_compare, que_print, que_clone, que_destroy);
	queue_destroy(NULL);
	queue_destroy(&q);
}

int init_que_suite(void) {
	return 0;
}

int clean_que_suite(void) {
	return 0;
}

