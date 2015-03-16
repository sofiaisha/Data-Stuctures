#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "hashtable.h"
#include "hashtable_test.h"

const unsigned int BINS = 16;

void ht_print(void* elem, FILE* fd) {
	hashtable_entry_t* hte 	= (hashtable_entry_t*)elem;
	unsigned int k 		= *(int*)(hte->key);
	char* v 		= (char*)(hte->value);

	fprintf (fd, " (k=%ud, v=%s) ", k, v);
}

unsigned int ht_hash(void * key) {
        unsigned int k = *((int*)key); 
        return k % BINS; 
}

int ht_compare(void* e1, void* e2) {
	hashtable_entry_t* hte1 = (hashtable_entry_t*)e1;
	hashtable_entry_t* hte2 = (hashtable_entry_t*)e2;
	unsigned int k1		= *(int*)(hte1->key);
	unsigned int k2 	= *(int*)(hte2->key);
	return k1-k2;
}

void ht_destroy(void* e) {
	hashtable_entry_t* hte = (hashtable_entry_t*)e;
	free(hte);
}

unsigned int ht_test_key1 = 1;
unsigned int ht_test_key2 = 2;
unsigned int ht_test_key3 = 18;
unsigned int ht_test_key4 = 42;
char ht_test_s1[] = "string1";
char ht_test_s2[] = "string2";
char ht_test_s3[] = "string18";
char ht_test_s4[] = "string42";

void ht_test_init(void) {
	CU_ASSERT (hashtable_init(0, ht_compare, ht_print, ht_destroy, ht_hash) == NULL);
	CU_ASSERT (hashtable_init(BINS, NULL, ht_print, ht_destroy, ht_hash) == NULL);
	CU_ASSERT (hashtable_init(BINS, ht_compare, ht_print, NULL, ht_hash) == NULL);
	CU_ASSERT (hashtable_init(BINS, ht_compare, ht_print, ht_destroy, NULL) == NULL);
	hashtable_t * h = hashtable_init(BINS, ht_compare, ht_print, ht_destroy, ht_hash);
	CU_ASSERT (h->bin_count == BINS);
	CU_ASSERT (h->size == 0);
	CU_ASSERT (h->hash == ht_hash);
	hashtable_destroy(&h);
}

void ht_test_empty(void) {
	hashtable_t * h = hashtable_init(BINS, ht_compare, ht_print, ht_destroy, ht_hash);

	hashtable_print(h, stdout);
	CU_ASSERT(hashtable_find(h, &ht_test_key1) == NULL);
	CU_ASSERT(hashtable_remove(h, &ht_test_key1) == -2);

	hashtable_destroy(&h);
}

void ht_test_add(void) {
	hashtable_t * h = hashtable_init(BINS, ht_compare, ht_print, ht_destroy, ht_hash);

	hashtable_entry_t* hte = 
		(hashtable_entry_t*)hashtable_add(h, &ht_test_key1, ht_test_s1);
	CU_ASSERT(strcmp((char*)(hte->value), ht_test_s1) == 0);
	CU_ASSERT(h->size == 1);

	hte = (hashtable_entry_t*)hashtable_add(h, &ht_test_key2, ht_test_s2);
	CU_ASSERT(strcmp((char*)(hte->value), ht_test_s2) == 0);
	CU_ASSERT(h->size == 2);

	hte = (hashtable_entry_t*)hashtable_add(h, &ht_test_key3, ht_test_s3);
	CU_ASSERT(strcmp((char*)(hte->value), ht_test_s3) == 0);
	CU_ASSERT(h->size == 3);

	hte = (hashtable_entry_t*)hashtable_add(h, &ht_test_key4, ht_test_s4);
	CU_ASSERT(strcmp((char*)(hte->value), ht_test_s4) == 0);
	CU_ASSERT(h->size == 4);

	hashtable_destroy(&h);
}

void ht_test_remove(void) {
	hashtable_t * h = hashtable_init(BINS, ht_compare, ht_print, ht_destroy, ht_hash);
	hashtable_add(h, &ht_test_key1, ht_test_s1);
	hashtable_add(h, &ht_test_key2, ht_test_s2);
	hashtable_add(h, &ht_test_key3, ht_test_s3);
	hashtable_add(h, &ht_test_key4, ht_test_s4);

        CU_ASSERT(hashtable_remove(NULL, &ht_test_key1) == -1);
        CU_ASSERT(hashtable_remove(h, NULL) == -1);
        CU_ASSERT (h->size == 4);

        CU_ASSERT(hashtable_remove(h, &ht_test_key1) == 0);
        CU_ASSERT (h->size == 3);
	CU_ASSERT(hashtable_find(h, &ht_test_key1) == NULL);
	hashtable_print(h, stdout);

	hashtable_destroy(&h);
}

void ht_test_find(void) {
	hashtable_t * h = hashtable_init(BINS, ht_compare, ht_print, ht_destroy, ht_hash);
	hashtable_add(h, &ht_test_key1, ht_test_s1);
	hashtable_add(h, &ht_test_key2, ht_test_s2);
	hashtable_add(h, &ht_test_key3, ht_test_s3);
	hashtable_add(h, &ht_test_key4, ht_test_s4);

	unsigned int testKey = ht_test_key2;
	char* s = (char*)hashtable_find(h, &testKey);
	CU_ASSERT (strcmp(s, ht_test_s2) == 0);
	testKey = ht_test_key3;
	s = (char*)hashtable_find(h, &testKey);
	CU_ASSERT (strcmp(s, ht_test_s3) == 0);
	testKey = ht_test_key4;
	s = (char*)hashtable_find(h, &testKey);
	CU_ASSERT (strcmp(s, ht_test_s4) == 0);

	hashtable_destroy(&h);
}

void ht_test_destroy(void) {
	hashtable_t * h = hashtable_init(BINS, ht_compare, ht_print, ht_destroy, ht_hash);
	hashtable_destroy(NULL);
	hashtable_destroy(&h);
}

int init_ht_suite(void) {
        return 0;
}

int clean_ht_suite(void) {
        return 0;
}

