
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "pqueue.h"
#include "pqueue_test.h"

void pq_print(void* elem, FILE* fd) {
	pqnode_t* node = (pqnode_t*)elem;
	fprintf (fd, "(%d,%s) ", node->priority, (char*)node->entry);
}

void* pq_clone(void* s) {
	pqnode_t* node    = (pqnode_t*)s;
	pqnode_t* newnode = (pqnode_t*)malloc(sizeof(pqnode_t));
	newnode->priority = node->priority;
	newnode->entry    = strdup(node->entry);
	return newnode;
}

void pq_destroy(void* s) {
	pqnode_t* node = (pqnode_t*)s;
	free(node);
}

char pqueue_test_s1[] = "string1";
char pqueue_test_s2[] = "string2";
char pqueue_test_s3[] = "string3";
char pqueue_test_s4[] = "string4";

void pqueue_test_init(void) {
	CU_ASSERT (pqueue_init(NULL, pq_clone, pq_destroy) == NULL);
	CU_ASSERT (pqueue_init(pq_print, NULL, pq_destroy) == NULL);

	pqueue_t* pq = pqueue_init(pq_print, pq_clone, pq_destroy);
	CU_ASSERT (pq != NULL);
	CU_ASSERT (pqueue_destroy(&pq) == 0);
}

void pqueue_test_empty(void) {
	pqueue_t* pq = pqueue_init(pq_print, pq_clone, pq_destroy);
	CU_ASSERT (pq != NULL);

	pqueue_print(pq, stderr);
	CU_ASSERT (pqueue_peek(pq) == NULL);
	CU_ASSERT (pqueue_pop(pq) == NULL);

	CU_ASSERT (pqueue_destroy(&pq) == 0);
}

void pqueue_test_push(void) {
	pqueue_t* pq = pqueue_init(pq_print, pq_clone, pq_destroy);
	CU_ASSERT (pq != NULL);
	
	CU_ASSERT (pqueue_push(NULL, pqueue_test_s2, 2) == NULL);
	CU_ASSERT (pqueue_push(pq, NULL, 2) == NULL);

	CU_ASSERT (pqueue_push(pq, pqueue_test_s2, 2) == pqueue_test_s2);
	CU_ASSERT (pqueue_push(pq, pqueue_test_s1, 1) == pqueue_test_s1);
	CU_ASSERT (pqueue_push(pq, pqueue_test_s4, 4) == pqueue_test_s4);
	CU_ASSERT (pqueue_push(pq, pqueue_test_s3, 3) == pqueue_test_s3);
	CU_ASSERT (pqueue_size(pq) == 4);

	node_t* tmp = pq->head;
	char*s = ((pqnode_t*)tmp->elem)->entry;
	CU_ASSERT (strcmp(s, pqueue_test_s1) == 0);
	tmp = tmp->next;
	s = ((pqnode_t*)tmp->elem)->entry;
	CU_ASSERT (strcmp(s, pqueue_test_s2) == 0);
	tmp = tmp->next;
	s = ((pqnode_t*)tmp->elem)->entry;
	CU_ASSERT (strcmp(s, pqueue_test_s3) == 0);
	tmp = tmp->next;
	s = ((pqnode_t*)tmp->elem)->entry;
	CU_ASSERT (strcmp(s, pqueue_test_s4) == 0);

	CU_ASSERT (pqueue_destroy(&pq) == 0);
}

void pqueue_test_peek(void) {
	pqueue_t* pq = pqueue_init(pq_print, pq_clone, pq_destroy);
	CU_ASSERT (pq != NULL);
	
	CU_ASSERT (pqueue_peek(NULL) == NULL);

	pqueue_push(pq, pqueue_test_s2, 2);
	pqueue_push(pq, pqueue_test_s1, 1);
	pqueue_push(pq, pqueue_test_s4, 4);
	pqueue_push(pq, pqueue_test_s3, 3);

	pqnode_t* n = pqueue_peek(pq);
	CU_ASSERT(n->entry == pqueue_test_s1);
	CU_ASSERT (strcmp(n->entry, pqueue_test_s1) == 0);

	n = pqueue_pop(pq);
	CU_ASSERT(n->entry != pqueue_test_s1);
	CU_ASSERT (strcmp(n->entry, pqueue_test_s1) == 0);
	free(n->entry);
	free(n);

	n = pqueue_peek(pq);
	CU_ASSERT(n->entry == pqueue_test_s2);
	CU_ASSERT (strcmp(n->entry, pqueue_test_s2) == 0);

	CU_ASSERT (pqueue_destroy(&pq) == 0);
}
	
void pqueue_test_pop(void) {
	pqueue_t* pq = pqueue_init(pq_print, pq_clone, pq_destroy);
	
	CU_ASSERT (pqueue_pop(NULL) == NULL);

	pqueue_push(pq, pqueue_test_s3, 3);
	pqueue_push(pq, pqueue_test_s4, 4);
	pqueue_push(pq, pqueue_test_s1, 1);
	pqueue_push(pq, pqueue_test_s2, 2);

	pqnode_t* n = pqueue_pop(pq);
	CU_ASSERT(n->entry != pqueue_test_s1);
	CU_ASSERT(n->priority == 1);
	CU_ASSERT (strcmp(n->entry, pqueue_test_s1) == 0);
	CU_ASSERT (pqueue_size(pq) == 3);
	free(n->entry);
	free(n);

	n = queue_pop(pq);
	CU_ASSERT(n->priority == 2);
	CU_ASSERT(n->entry != pqueue_test_s2);
	CU_ASSERT (strcmp(n->entry, pqueue_test_s2) == 0);
	CU_ASSERT (pqueue_size(pq) == 2);
	free(n->entry);
	free(n);

	n = queue_pop(pq);
	CU_ASSERT(n->priority == 3);
	CU_ASSERT(n->entry != pqueue_test_s3);
	CU_ASSERT (strcmp(n->entry, pqueue_test_s3) == 0);
	CU_ASSERT (pqueue_size(pq) == 1);
	free(n->entry);
	free(n);

	n = queue_pop(pq);
	CU_ASSERT(n->priority == 4);
	CU_ASSERT(n->entry != pqueue_test_s4);
	CU_ASSERT (strcmp(n->entry, pqueue_test_s4) == 0);
	CU_ASSERT (pqueue_size(pq) == 0);
	free(n->entry);
	free(n);

	n = pqueue_pop(pq);
	CU_ASSERT(n == NULL);
	CU_ASSERT(pqueue_size(pq) == 0);
	
	CU_ASSERT (pqueue_destroy(&pq) == 0);
}
	
void pqueue_test_print(void) {
	pqueue_t* pq = pqueue_init(pq_print, pq_clone, pq_destroy);
	pqueue_push(pq, pqueue_test_s4, 4);
	pqueue_push(pq, pqueue_test_s3, 3);
	pqueue_push(pq, pqueue_test_s2, 2);
	pqueue_push(pq, pqueue_test_s1, 1);

	pqueue_print(NULL, stdout);

	pqueue_print(pq, stdout);

	CU_ASSERT (pqueue_destroy(&pq) == 0);
}

void pqueue_test_destroy(void) {
	pqueue_t* pq = pqueue_init(pq_print, pq_clone, pq_destroy);

	CU_ASSERT (pqueue_destroy(NULL) == EINVAL);
	CU_ASSERT (pqueue_destroy(&pq) == 0);
}

int init_pqueue_suite(void) {
	return 0;
}

int clean_pqueue_suite(void) {
	return 0;
}

