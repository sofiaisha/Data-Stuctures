
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

char cbuffer_test_s1[] = "string1";
char cbuffer_test_s2[] = "string2";
char cbuffer_test_s3[] = "string3";
char cbuffer_test_s4[] = "string4";
char cbuffer_test_s5[] = "string5";
char cbuffer_test_s6[] = "string6";

unsigned int cb_size = 10;

void cbuffer_test_init(void) {
	cb_size = 0;
	CU_ASSERT (cbuffer_init(cb_size, cb_print, cb_clone, NULL) == NULL);
	cb_size = 10;
	CU_ASSERT (cbuffer_init(cb_size, NULL, cb_clone, NULL) == NULL);
	CU_ASSERT (cbuffer_init(cb_size, cb_print, NULL, NULL) == NULL);

	cbuffer_t* cb = cbuffer_init(cb_size, cb_print, cb_clone, NULL);
	CU_ASSERT (cb != NULL);

	CU_ASSERT (cb->size == cb_size);
	CU_ASSERT (cb->head == 0);
	CU_ASSERT (cb->print == cb_print);
	CU_ASSERT (cb->clone == cb_clone);
	CU_ASSERT (cb->destroy == NULL);
	CU_ASSERT (cb->entries != NULL);

	CU_ASSERT (cbuffer_destroy(cb) == 0);
}

void cbuffer_test_empty(void) {
	cbuffer_t* cb = cbuffer_init(cb_size, cb_print, cb_clone, NULL);
	CU_ASSERT (cb != NULL);

	CU_ASSERT (cbuffer_size(NULL) == EINVAL);
	CU_ASSERT (cbuffer_size(cb) == cb_size);

	CU_ASSERT (cbuffer_destroy(cb) == 0);
}

void cbuffer_test_destroy(void) {
	cbuffer_t* cb = cbuffer_init(cb_size, cb_print, cb_clone, NULL);
	CU_ASSERT (cb != NULL);
	CU_ASSERT (cbuffer_destroy(NULL) == EINVAL);
	CU_ASSERT (cbuffer_destroy(cb) == 0);
}

void cbuffer_test_read(void) {
	cbuffer_t* cb = cbuffer_init(cb_size, cb_print, cb_clone, NULL);
	CU_ASSERT (cb != NULL);
	char* s = NULL;
	
	CU_ASSERT (cbuffer_read(NULL, 0, (void**)&s) == EINVAL);
	CU_ASSERT (cbuffer_read(cb, 0, NULL) == EINVAL);

	CU_ASSERT (cbuffer_destroy(cb) == 0);
}

void cbuffer_test_write(void) {
	cbuffer_t* cb = cbuffer_init(4, cb_print, cb_clone, NULL);
	char* s = NULL;
	
	CU_ASSERT (cbuffer_write(NULL, s) == EINVAL);
	CU_ASSERT (cbuffer_write(cb, s) == EINVAL);

	CU_ASSERT (cbuffer_write(cb, cbuffer_test_s1) == 0);
	CU_ASSERT (cbuffer_write(cb, cbuffer_test_s2) == 0);
	CU_ASSERT (cbuffer_write(cb, cbuffer_test_s3) == 0);
	CU_ASSERT (cbuffer_write(cb, cbuffer_test_s4) == 0);
	CU_ASSERT (cbuffer_write(cb, cbuffer_test_s5) == 0);
	CU_ASSERT (cbuffer_write(cb, cbuffer_test_s6) == 0);

	CU_ASSERT (cbuffer_read(cb, 0, (void**)&s) == 0);
	CU_ASSERT (strcmp(cbuffer_test_s6, s) == 0);
	CU_ASSERT (cbuffer_read(cb, -1, (void**)&s) == 0);
	CU_ASSERT (strcmp(cbuffer_test_s5, s) == 0);
	CU_ASSERT (cbuffer_read(cb, -2, (void**)&s) == 0);
	CU_ASSERT (strcmp(cbuffer_test_s4, s) == 0);
	CU_ASSERT (cbuffer_read(cb, -3, (void**)&s) == 0);
	CU_ASSERT (strcmp(cbuffer_test_s3, s) == 0);
	CU_ASSERT (cbuffer_read(cb, -4, (void**)&s) == 0);
	CU_ASSERT (strcmp(cbuffer_test_s6, s) == 0);
	CU_ASSERT (cbuffer_read(cb, -5, (void**)&s) == 0);
	CU_ASSERT (strcmp(cbuffer_test_s5, s) == 0);

	CU_ASSERT (cbuffer_read(cb, 1, (void**)&s) == 0);
	CU_ASSERT (strcmp(cbuffer_test_s3, s) == 0);
	CU_ASSERT (cbuffer_read(cb, 2, (void**)&s) == 0);
	CU_ASSERT (strcmp(cbuffer_test_s4, s) == 0);
	CU_ASSERT (cbuffer_read(cb, 3, (void**)&s) == 0);
	CU_ASSERT (strcmp(cbuffer_test_s5, s) == 0);
	CU_ASSERT (cbuffer_read(cb, 4, (void**)&s) == 0);
	CU_ASSERT (strcmp(cbuffer_test_s6, s) == 0);
	CU_ASSERT (cbuffer_read(cb, 5, (void**)&s) == 0);
	CU_ASSERT (strcmp(cbuffer_test_s3, s) == 0);

	CU_ASSERT (cbuffer_destroy(cb) == 0);
}

int init_cbuffer_suite(void) {
	return 0;
}

int clean_cbuffer_suite(void) {
	return 0;
}

