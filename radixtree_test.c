
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "radixtree.h"

char radixtree_test_s1[] = "string";
char radixtree_test_s2[] = "strings";
char radixtree_test_s3[] = "stretch";
char radixtree_test_s4[] = "hello";
char radixtree_test_s5[] = "help";

char radixtree_test_s6[] = "morning";
char radixtree_test_s7[] = "stringz";
char radixtree_test_s8[] = "helpful";

char radixtree_test_s9[] = "str";


#define ALPHABET_SIZE 	26
#define ALPHABET_START 	'a'	

void radixtree_test_init(void) {
	CU_ASSERT (radixtree_init(0, ALPHABET_START) == NULL);

	radixtree_t* t = radixtree_init(ALPHABET_SIZE, ALPHABET_START);
	CU_ASSERT (t != NULL);
	CU_ASSERT (radixtree_size(t) == 0);

	radixtree_destroy(&t);
}

void radixtree_test_destroy(void) {
	radixtree_t* t = radixtree_init(ALPHABET_SIZE, ALPHABET_START);
	radixtree_destroy(NULL);
	radixtree_destroy(&t);
}

void radixtree_test_add(void) {
	radixtree_t* t = radixtree_init(ALPHABET_SIZE, ALPHABET_START);
	CU_ASSERT (strcmp(radixtree_add(t, radixtree_test_s1), radixtree_test_s1) == 0);
	CU_ASSERT (strcmp(radixtree_add(t, radixtree_test_s2), radixtree_test_s2) == 0);
	CU_ASSERT (strcmp(radixtree_add(t, radixtree_test_s3), radixtree_test_s3) == 0);
	CU_ASSERT (strcmp(radixtree_add(t, radixtree_test_s4), radixtree_test_s4) == 0);
	CU_ASSERT (strcmp(radixtree_add(t, radixtree_test_s5), radixtree_test_s5) == 0);
	radixtree_destroy(&t);
}


void radixtree_test_find(void) {
	radixtree_t* t = radixtree_init(ALPHABET_SIZE, ALPHABET_START);
	radixtree_add(t, radixtree_test_s1);
	radixtree_add(t, radixtree_test_s2);
	radixtree_add(t, radixtree_test_s3);
	radixtree_add(t, radixtree_test_s4);
	radixtree_add(t, radixtree_test_s5);

	CU_ASSERT (strcmp(radixtree_find(t, radixtree_test_s1), radixtree_test_s1) == 0);
	CU_ASSERT (strcmp(radixtree_find(t, radixtree_test_s2), radixtree_test_s2) == 0);
	CU_ASSERT (strcmp(radixtree_find(t, radixtree_test_s3), radixtree_test_s3) == 0);
	CU_ASSERT (strcmp(radixtree_find(t, radixtree_test_s4), radixtree_test_s4) == 0);
	CU_ASSERT (strcmp(radixtree_find(t, radixtree_test_s5), radixtree_test_s5) == 0);

	CU_ASSERT (radixtree_find(t, radixtree_test_s6) == NULL);
	CU_ASSERT (radixtree_find(t, radixtree_test_s7) == NULL);
	CU_ASSERT (radixtree_find(t, radixtree_test_s8) == NULL);

	radixtree_destroy(&t);
}

int init_radixtree_suite(void) {
	return 0;
}

int clean_radixtree_suite(void) {
	return 0;
}

