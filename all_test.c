
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "list_test.h"
#include "stack_test.h"
#include "queue_test.h"
#include "hashtable_test.h"
#include "binarytree_test.h"
#include "radixtree_test.h"
#include "cbuffer_test.h"

int main(void) {
	CU_pSuite pSuite = NULL;

	if (CU_initialize_registry() != CUE_SUCCESS) {
		return CU_get_error();	
	}

	/* List test suite */ 

	pSuite = CU_add_suite("list_suite", init_list_suite, clean_list_suite); 
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();	
	}

	if (
	(CU_add_test(pSuite, "testing list_init()", list_test_init) == NULL) ||
	(CU_add_test(pSuite, "testing empty list", list_test_empty) == NULL) ||
	(CU_add_test(pSuite, "testing list_addFirst()", list_test_addFirst) == NULL) ||
	(CU_add_test(pSuite, "testing list_addLast()", list_test_addLast) == NULL) ||
	(CU_add_test(pSuite, "testing list traversal", list_test_walk) == NULL) ||
	(CU_add_test(pSuite, "testing list_getElem()", list_test_getElem) == NULL) ||
	(CU_add_test(pSuite, "testing list_print()", list_test_print) == NULL) ||
	(CU_add_test(pSuite, "testing list_find()", list_test_find) == NULL) ||
	(CU_add_test(pSuite, "testing list_remove()", list_test_remove) == NULL) ||
	(CU_add_test(pSuite, "testing list_destroy()", list_test_destroy) == NULL) 
	) {
		CU_cleanup_registry();
		return CU_get_error();	
	}

	/* Stack test suite */ 

	pSuite = CU_add_suite("stack_suite", init_stk_suite, clean_stk_suite); 
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();	
	}

	if (
	(CU_add_test(pSuite, "testing stack_init()", stk_test_init) == NULL) ||
	(CU_add_test(pSuite, "testing empty stack", stk_test_empty) == NULL) ||
	(CU_add_test(pSuite, "testing stack_push()", stk_test_push) == NULL) ||
	(CU_add_test(pSuite, "testing stack_get()", stk_test_get) == NULL) ||
	(CU_add_test(pSuite, "testing stack_pop()", stk_test_pop) == NULL) || 
	(CU_add_test(pSuite, "testing stack_print()", stk_test_print) == NULL) ||
	(CU_add_test(pSuite, "testing stack_destroy()", stk_test_destroy) == NULL) 
	) {
		CU_cleanup_registry();
		return CU_get_error();	
	}

	/* Queue test suite */ 

	pSuite = CU_add_suite("queue_suite", init_queue_suite, clean_queue_suite); 
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();	
	}

	if (
	(CU_add_test(pSuite, "testing queue_init()", queue_test_init) == NULL) ||
	(CU_add_test(pSuite, "testing queue_empty()", queue_test_empty) == NULL) ||
	(CU_add_test(pSuite, "testing queue_push()", queue_test_push) == NULL) || 
	(CU_add_test(pSuite, "testing queue_peek()", queue_test_peek) == NULL) ||
	(CU_add_test(pSuite, "testing queue_pop()", queue_test_pop) == NULL) ||
	(CU_add_test(pSuite, "testing queue_print()", queue_test_print) == NULL) || 
	(CU_add_test(pSuite, "testing queue_destroy()", queue_test_destroy) == NULL) 
	) {
		CU_cleanup_registry();
		return CU_get_error();	
	}

	/* Hashtable test suite */ 

	pSuite = CU_add_suite("hashtable_suite", init_ht_suite, clean_ht_suite); 
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();	
	}

	if (
	(CU_add_test(pSuite, "testing hashtable_init()", ht_test_init) == NULL) ||
	(CU_add_test(pSuite, "testing empty hashtable", ht_test_empty) == NULL) ||
	(CU_add_test(pSuite, "testing hashtable_add()", ht_test_add) == NULL) ||
	(CU_add_test(pSuite, "testing hashtable_remove()", ht_test_remove) == NULL) || 
	(CU_add_test(pSuite, "testing hashtable_find()", ht_test_find) == NULL) || 
	(CU_add_test(pSuite, "testing hashtable_destroy()", ht_test_destroy) == NULL)  
	) {
		CU_cleanup_registry();
		return CU_get_error();	
	}

	/* Binary tree test suite */ 

	pSuite = CU_add_suite("binarytree_suite", 
			init_bintree_suite, clean_bintree_suite); 
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();	
	}

	if (
	(CU_add_test(pSuite, "testing binarytree_init()", bintree_test_init) == NULL) ||
	(CU_add_test(pSuite, "testing binarytree_destroy()", bintree_test_destroy) == NULL) ||
	(CU_add_test(pSuite, "testing empty binarytree", bintree_test_empty) == NULL) ||
	(CU_add_test(pSuite, "testing binarytree_add() and find() #1", bintree_test_add_find1) == NULL) ||
	(CU_add_test(pSuite, "testing binarytree_add() and find() #2", bintree_test_add_find2) == NULL) ||
	(CU_add_test(pSuite, "testing binarytree_add() and find() #3", bintree_test_add_find3) == NULL) ||
	(CU_add_test(pSuite, "testing binarytree_add() and find() #4", bintree_test_add_find4) == NULL) ||
	(CU_add_test(pSuite, "testing binarytree_traversal()", bintree_test_traversal) == NULL) ||
	(CU_add_test(pSuite, "testing binarytree_walk()", bintree_test_walk) == NULL) ||
	(CU_add_test(pSuite, "testing binarytree_remove() #1", bintree_test_remove1) == NULL) ||
	(CU_add_test(pSuite, "testing binarytree_remove() #2", bintree_test_remove2) == NULL) ||
	(CU_add_test(pSuite, "testing binarytree_remove() #3", bintree_test_remove3) == NULL) 
	) {
		CU_cleanup_registry();
		return CU_get_error();	
	}

	/* Radix tree test suite */ 

	pSuite = CU_add_suite("radixtree_suite", 
			init_radixtree_suite, clean_radixtree_suite); 
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();	
	}

	if (
	(CU_add_test(pSuite, "testing radixtree_init()", radixtree_test_init) == NULL) ||
	(CU_add_test(pSuite, "testing radixtree_destroy()", radixtree_test_destroy) == NULL) || 
	(CU_add_test(pSuite, "testing radixtree_density()", radixtree_test_density) == NULL) ||
	(CU_add_test(pSuite, "testing radixtree_add()", radixtree_test_add) == NULL) || 
	(CU_add_test(pSuite, "testing radixtree_find()", radixtree_test_find) == NULL) 
	) {
		CU_cleanup_registry();
		return CU_get_error();	
	}
 
	/* Circular buffer test suite */ 

	pSuite = CU_add_suite("cbuffer_suite", 
			init_cbuffer_suite, clean_cbuffer_suite); 
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();	
	}

	if (
	(CU_add_test(pSuite, "testing cbuffer_init()", cbuffer_test_init) == NULL) ||
	(CU_add_test(pSuite, "testing cbuffer_size()", cbuffer_test_empty) == NULL) ||
	(CU_add_test(pSuite, "testing cbuffer_read()", cbuffer_test_read) == NULL) ||
	(CU_add_test(pSuite, "testing cbuffer_write()", cbuffer_test_write) == NULL) ||
	(CU_add_test(pSuite, "testing cbuffer_destroy()", cbuffer_test_destroy) == NULL) 
	) {
		CU_cleanup_registry();
		return CU_get_error();	
	}
 
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();	
}
