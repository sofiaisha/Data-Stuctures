#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h> 

#include "hashtable.h"
#include "util.h"

void* ht_clone_INTERNAL(void* e) {
	return NULL;
}

hashtable_t* hashtable_init(unsigned int bins, int (*compare)(void*, void*), 
	void (*print)(void*, FILE*), void (*destroy)(void*),
        unsigned int (*hash)(void*)) {  

	if ((bins == 0) || (compare == NULL) || (destroy == NULL) || (hash == NULL)){
     	   	debug_print("invalid parameter\n");
		return NULL;
	}

	hashtable_t* h = (hashtable_t*)malloc(sizeof(hashtable_t));
        if (h == NULL) {
     	   	perror("hashtable_init: can't create new hashtable");
	   	return NULL;
     	}

	h->bins = (list_t**)malloc(bins*sizeof(list_t*)); 
        if (h->bins == NULL) {
     	   	perror("hashtable_init: can't create bins");
		free(h);
	   	return NULL;
     	}

	for (unsigned int i=0;i<bins;i++) {
		h->bins[i] = list_init(compare, print, ht_clone_INTERNAL, destroy);
	}

	h->hash      = hash;
	h->bin_count = bins;
	h->size      = 0;

      	return h;
}

void* hashtable_add(hashtable_t* h, void* key, void* value) {
	if ((h == NULL) || (key == NULL) || (value == NULL)) {
     	   	debug_print("invalid parameter\n");
		return NULL;
	}

	/* Create new hash table entry for key/value pair */
	hashtable_entry_t* hte = 
		(hashtable_entry_t*)malloc(sizeof(hashtable_entry_t));
	if (hte == NULL) {
		perror ("hashtable_add: can't create hash table entry\n");
		return NULL;
	}
	hte->key   = key;
	hte->value = value;

	/* Find out which bin this entry will be added to */	
	unsigned int bin = h->hash(key) % h->bin_count;
	
	/* Add hash table entry to the bin */
	if (list_addFirst(h->bins[bin], hte) != NULL) {
		h->size++;
		return hte;	
	}
	else {
		return NULL;	
	}
}

void hashtable_print(hashtable_t* h, FILE* fd) {
        if ((h == NULL) || (fd == NULL)) {
                debug_print("invalid parameter\n");
                return; 
        }       

	for (unsigned int i=0;i<h->bin_count;i++) {
		fprintf (fd, " bin %ud ", i);
		list_print(h->bins[i], fd);
	}
	fprintf (fd, "\n");
}

void* hashtable_find(hashtable_t* h, void* key) {
	if ((h == NULL) || (key == NULL)) {
     	   	debug_print("invalid parameter\n");
		return NULL;
	}

	/* Find out which bin this entry should live in */
	unsigned int bin = h->hash(key) % h->bin_count;

	hashtable_entry_t tmp;
	tmp.value = NULL;
	tmp.key = key;
	hashtable_entry_t* hte = (hashtable_entry_t*)list_find(h->bins[bin], &tmp); 
	if (hte == NULL) {
		return NULL;
	}
	else {
		return hte->value;	
	}
} 

int hashtable_remove(hashtable_t* h, void* key) {
	if ((h == NULL) || (key == NULL)) {
     	   	debug_print("invalid parameter\n");
		return EINVAL;
	}
	
	/* Find out which bin this entry should live in */
	unsigned int bin = h->hash(key) % h->bin_count;

	hashtable_entry_t tmp;
	tmp.value = NULL;
	tmp.key = key;
	int result = list_remove(h->bins[bin], &tmp);
	if (result == 0) { 
		h->size--;
	}
	return result;
}

void hashtable_destroy(hashtable_t** h) {
	if ((h == NULL) || (*h == NULL)) {
     	   	debug_print("invalid parameter\n");
		return;
	}

	hashtable_t* tmp = *h;	
	*h = NULL;

	/* Empty and free each bin */
	for (unsigned int i=0;i<tmp->bin_count;i++) {
		list_destroy(&(tmp->bins[i]));
		assert(tmp->bins[i] == NULL);
	}

	free(tmp->bins);	
	tmp->bin_count = 0;
	tmp->size = 0;
	free(tmp);
}

