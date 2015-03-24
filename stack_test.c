
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "stack.h"
#include "stack_test.h"

void stk_print(void* elem, FILE* fd) {
	fprintf (fd, "%s ", (char*)elem);
}

int stk_compare(void* s1, void* s2) {
	return strcmp ( (char*)s1, (char*)s2 );
}

void* stk_clone(void* s) {
	return strdup((char*)s);
}

void stk_destroy(void* s) {
	return;
}

char stk_test_s1[] = "string1";
char stk_test_s2[] = "string2";
char stk_test_s3[] = "string3";
char stk_test_s4[] = "string4";

void stk_test_init(void) {
	CU_ASSERT (stack_init(NULL, stk_print, stk_clone, stk_destroy) == NULL);
	CU_ASSERT (stack_init(stk_compare, NULL, stk_clone, stk_destroy) == NULL);
	CU_ASSERT (stack_init(stk_compare, stk_print, NULL, stk_destroy) == NULL);

	stack_t* s = stack_init(stk_compare, stk_print, stk_clone, stk_destroy);
	CU_ASSERT (s != NULL);
	CU_ASSERT (stack_destroy(s) == 0);
}

void stk_test_empty(void) {
	stack_t* s = stack_init(stk_compare, stk_print, stk_clone, stk_destroy);

	stack_print(s, stderr);
	CU_ASSERT (stack_get(s) == NULL);
	CU_ASSERT (stack_pop(s) == NULL);

	CU_ASSERT (stack_destroy(s) == 0);
}

void stk_test_push(void) {
	stack_t* s = stack_init(stk_compare, stk_print, stk_clone, stk_destroy);
	
	CU_ASSERT (stack_push(NULL, stk_test_s1) == NULL);
	CU_ASSERT (stack_push(s, NULL) == NULL);

	CU_ASSERT (stack_push(s, stk_test_s1) == stk_test_s1);
	CU_ASSERT (stack_push(s, stk_test_s2) == stk_test_s2);
	CU_ASSERT (stack_push(s, stk_test_s3) == stk_test_s3);
	CU_ASSERT (stack_push(s, stk_test_s4) == stk_test_s4);

	CU_ASSERT (stack_destroy(s) == 0);
}

void stk_test_get(void) {
	stack_t* s = stack_init(stk_compare, stk_print, stk_clone, stk_destroy);
	
	CU_ASSERT (stack_push(s, stk_test_s1) != NULL);;
	CU_ASSERT (stack_push(s, stk_test_s2) != NULL);;
	CU_ASSERT (stack_push(s, stk_test_s3) != NULL);;
	CU_ASSERT (stack_push(s, stk_test_s4) != NULL);;

	char* str = stack_get(s);
	CU_ASSERT(str == stk_test_s4);
	CU_ASSERT (strcmp(str, stk_test_s4) == 0);

	str = stack_pop(s);
	CU_ASSERT(str != stk_test_s4);
	CU_ASSERT (strcmp(str, stk_test_s4) == 0);
	free(str);

	str = stack_get(s);
	CU_ASSERT(str == stk_test_s3);
	CU_ASSERT (strcmp(str, stk_test_s3) == 0);

	CU_ASSERT (stack_destroy(s) == 0);
}
	
void stk_test_pop(void) {
	stack_t* s = stack_init(stk_compare, stk_print, stk_clone, stk_destroy);
	CU_ASSERT (s != NULL);
	
	CU_ASSERT (stack_push(s, stk_test_s1) != NULL);;
	CU_ASSERT (stack_push(s, stk_test_s2) != NULL);;
	CU_ASSERT (stack_push(s, stk_test_s3) != NULL);;
	CU_ASSERT (stack_push(s, stk_test_s4) != NULL);;

	char* str = stack_pop(s);
	CU_ASSERT(str != stk_test_s4);
	CU_ASSERT (strcmp(str, stk_test_s4) == 0);
	free(str);

	str = stack_pop(s);
	CU_ASSERT(str != stk_test_s3);
	CU_ASSERT (strcmp(str, stk_test_s3) == 0);
	free(str);

	str = stack_pop(s);
	CU_ASSERT(str != stk_test_s2);
	CU_ASSERT (strcmp(str, stk_test_s2) == 0);
	free(str);

	str = stack_pop(s);
	CU_ASSERT(str != stk_test_s1);
	CU_ASSERT (strcmp(str, stk_test_s1) == 0);
	free(str);

	str = stack_pop(s);
	CU_ASSERT(str == NULL);
	CU_ASSERT(stack_size(s) == 0);
	
	CU_ASSERT (stack_destroy(s) == 0);
}
	
void stk_test_print(void) {
	stack_t* s = stack_init(stk_compare, stk_print, stk_clone, stk_destroy);
	CU_ASSERT (s != NULL);
	
	CU_ASSERT (stack_push(s, stk_test_s1) != NULL);;
	CU_ASSERT (stack_push(s, stk_test_s2) != NULL);;
	CU_ASSERT (stack_push(s, stk_test_s3) != NULL);;
	CU_ASSERT (stack_push(s, stk_test_s4) != NULL);;

	stack_print(s, stdout);

	CU_ASSERT (stack_destroy(s) == 0);
}

void stk_test_destroy(void) {
	stack_t* s = stack_init(stk_compare, stk_print, stk_clone, stk_destroy);
	CU_ASSERT (s != NULL);
	CU_ASSERT (stack_destroy(NULL) == EINVAL);
	CU_ASSERT (stack_destroy(s) == 0);
}

int init_stk_suite(void) {
	return 0;
}

int clean_stk_suite(void) {
	return 0;
}

