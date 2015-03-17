
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "queue.h"
#include "queue_test.h"

void q_print(void* elem, FILE* fd) {
	fprintf (fd, "%s ", (char*)elem);
}

int q_compare(void* s1, void* s2) {
	return strcmp ( (char*)s1, (char*)s2 );
}

void* q_clone(void* s) {
	return strdup((char*)s);
}

void q_destroy(void* s) {
	return;
}

char queue_test_s1[] = "string1";
char queue_test_s2[] = "string2";
char queue_test_s3[] = "string3";
char queue_test_s4[] = "string4";

void queue_test_init(void) {
	CU_ASSERT (queue_init(NULL, q_print, q_clone, q_destroy) == NULL);
	CU_ASSERT (queue_init(q_compare, NULL, q_clone, q_destroy) == NULL);
	CU_ASSERT (queue_init(q_compare, q_print, NULL, q_destroy) == NULL);

	queue_t* q = queue_init(q_compare, q_print, q_clone, q_destroy);
	queue_destroy(&q);
}

void queue_test_empty(void) {
	queue_t* q = queue_init(q_compare, q_print, q_clone, q_destroy);

	q_print(q, stderr);
	CU_ASSERT (queue_peek(q) == NULL);
	CU_ASSERT (queue_pop(q) == NULL);

	queue_destroy(&q);
}

void queue_test_push(void) {
	queue_t* q = queue_init(q_compare, q_print, q_clone, q_destroy);
	
	CU_ASSERT (queue_push(NULL, queue_test_s1) == NULL);
	CU_ASSERT (queue_push(q, NULL) == NULL);

	CU_ASSERT (queue_push(q, queue_test_s1) == queue_test_s1);
	CU_ASSERT (queue_push(q, queue_test_s2) == queue_test_s2);
	CU_ASSERT (queue_push(q, queue_test_s3) == queue_test_s3);
	CU_ASSERT (queue_push(q, queue_test_s4) == queue_test_s4);

	queue_destroy(&q);
}

void queue_test_peek(void) {
	queue_t* q = queue_init(q_compare, q_print, q_clone, q_destroy);
	
	queue_push(q, queue_test_s1);
	queue_push(q, queue_test_s2);
	queue_push(q, queue_test_s3);
	queue_push(q, queue_test_s4);

	char* str = queue_peek(q);
	CU_ASSERT(str == queue_test_s1);
	CU_ASSERT (strcmp(str, queue_test_s1) == 0);

	str = queue_pop(q);
	CU_ASSERT(str != queue_test_s1);
	CU_ASSERT (strcmp(str, queue_test_s1) == 0);
	free(str);

	str = queue_peek(q);
	CU_ASSERT(str == queue_test_s2);
	CU_ASSERT (strcmp(str, queue_test_s2) == 0);

	queue_destroy(&q);
}
	
void queue_test_pop(void) {
	queue_t* q = queue_init(q_compare, q_print, q_clone, q_destroy);
	
	queue_push(q, queue_test_s1);
	queue_push(q, queue_test_s2);
	queue_push(q, queue_test_s3);
	queue_push(q, queue_test_s4);

	char* str = queue_pop(q);
	CU_ASSERT(str != queue_test_s1);
	CU_ASSERT (strcmp(str, queue_test_s1) == 0);
	free(str);

	str = queue_pop(q);
	CU_ASSERT(str != queue_test_s2);
	CU_ASSERT (strcmp(str, queue_test_s2) == 0);
	free(str);

	str = queue_pop(q);
	CU_ASSERT(str != queue_test_s3);
	CU_ASSERT (strcmp(str, queue_test_s3) == 0);
	free(str);

	str = queue_pop(q);
	CU_ASSERT(str != queue_test_s4);
	CU_ASSERT (strcmp(str, queue_test_s4) == 0);
	free(str);

	str = queue_pop(q);
	CU_ASSERT(str == NULL);
	CU_ASSERT(queue_size(q) == 0);
	
	queue_destroy(&q);
}
	
void queue_test_print(void) {
	queue_t* q = queue_init(q_compare, q_print, q_clone, q_destroy);
	queue_push(q, queue_test_s1);
	queue_push(q, queue_test_s2);
	queue_push(q, queue_test_s3);
	queue_push(q, queue_test_s4);

	queue_print(q, stdout);

	queue_destroy(&q);
}

void queue_test_destroy(void) {
	queue_t* q = queue_init(q_compare, q_print, q_clone, q_destroy);
	queue_destroy(NULL);
	queue_destroy(&q);
}

int init_queue_suite(void) {
	return 0;
}

int clean_queue_suite(void) {
	return 0;
}

