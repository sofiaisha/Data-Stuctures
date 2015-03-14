
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "radixtree.h"

char radixtree_test_s1[] = "string1";
char radixtree_test_s2[] = "string2";
char radixtree_test_s3[] = "string3";
char radixtree_test_s4[] = "string4";
char radixtree_test_s5[] = "string5";
char radixtree_test_s6[] = "string6";

#define ALPHABET_SIZE 26

void radixtree_test_init(void) {
	radixtree_t* t = radixtree_init(ALPHABET_SIZE);
	CU_ASSERT (t != NULL);
	CU_ASSERT (radixtree_size(t) == 0);
	CU_ASSERT (radixtree_height(t, t->root) == 0);

	radixtree_destroy(&t);
}

void radixtree_test_destroy(void) {
	radixtree_t* t = radixtree_init(ALPHABET_SIZE);
	radixtree_destroy(NULL);
	radixtree_destroy(&t);
}


int init_radixtree_suite(void) {
	return 0;
}

int clean_radixtree_suite(void) {
	return 0;
}

