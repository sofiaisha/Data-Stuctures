
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "cbuffer.h"
#include "cbuffer_test.h"

void cb_print(void* elem, FILE* fd) {
	fprintf (fd, "%s ", (char*)elem);
}

void* cb_clone(void* s) {
	return strdup((char*)s);
}

void cb_destroy(void* s) {
	return;
}

char cbuffer_test_s1[] = "string1";
char cbuffer_test_s2[] = "string2";
char cbuffer_test_s3[] = "string3";
char cbuffer_test_s4[] = "string4";

unsigned int cb_size = 10;

void cbuffer_test_init(void) {
	CU_ASSERT (cbuffer_init(0, cb_print, cb_clone, cb_destroy) == NULL);
	CU_ASSERT (cbuffer_init(cb_size, NULL, cb_clone, cb_destroy) == NULL);
	CU_ASSERT (cbuffer_init(cb_size, cb_print, NULL, cb_destroy) == NULL);
	CU_ASSERT (cbuffer_init(cb_size, cb_print, cb_clone, NULL) == NULL);

	cbuffer_t* cb = cbuffer_init(cb_size, cb_print, cb_clone, cb_destroy);
	CU_ASSERT (cb != NULL);

	CU_ASSERT (cb->size == 0);
	CU_ASSERT (cb->head == 0);
	CU_ASSERT (cb->print == cb_print);
	CU_ASSERT (cb->clone == cb_clone);
	CU_ASSERT (cb->destroy == cb_destroy);
	CU_ASSERT (cb->entries != NULL);

	CU_ASSERT (cbuffer_destroy(&cb) == 0);
}

void cbuffer_test_empty(void) {
	cbuffer_t* cb = cbuffer_init(cb_size, cb_print, cb_clone, cb_destroy);
	CU_ASSERT (cb != NULL);

	CU_ASSERT (cbuffer_size(NULL) == EINVAL);
	CU_ASSERT (cbuffer_size(cb) == 0);

	CU_ASSERT (cbuffer_destroy(&cb) == 0);
}

void cbuffer_test_destroy(void) {
	cbuffer_t* cb = cbuffer_init(cb_size, cb_print, cb_clone, cb_destroy);
	CU_ASSERT (cb != NULL);
	CU_ASSERT (cbuffer_destroy(NULL) == EINVAL);
	CU_ASSERT (cbuffer_destroy(&cb) == 0);
}

int init_cbuffer_suite(void) {
	return 0;
}

int clean_cbuffer_suite(void) {
	return 0;
}

