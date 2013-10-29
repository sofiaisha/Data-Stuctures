#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#include "list.h"
#include "util.h"

list_t* list_init(int (*compare)(void*, void*), void (*print)(void*, FILE*),
		  void* (*clone)(void*), void (*destroy)(void*)) {
	if ((compare == NULL) || (print == NULL) ||
            (clone == NULL) || (destroy == NULL))  {
		debug_print("invalid parameter\n");
		return NULL;
	}

	list_t* l = (list_t*)malloc(sizeof(list_t));
	if (l == NULL) {
		perror("can't create new list, errno=%d");
		return NULL;
	}

	if (sem_init(&(l->sem), 0, 1) != 0) {
		perror("list_init: can't create semaphore");
		free(l);
		return NULL;
	}
	
	l->head 	= NULL;
	l->tail 	= NULL;
	l->size 	= 0;
	l->compare 	= compare;
	l->print 	= print;
	l->destroy 	= destroy;
	l->clone 	= clone;

	return l;
}

unsigned int list_size(list_t* l) {
	if (l == NULL) {
		return 0;
	}
	else {
		return l->size;
	}
}

void* list_add(list_t* l, void* elem, unsigned int mode) {
	if ((l == NULL) || (elem == NULL) || 
            ((mode != ADD_FIRST) && (mode != ADD_LAST))){
		debug_print("invalid parameter\n");
		return NULL;
	}

	node_t* n = (node_t*)malloc(sizeof(node_t));
	if (n == NULL) {
		perror("list_add: can't create node");
		return NULL;
	}

	if (sem_wait(&(l->sem)) != 0) {
		perror("list_add: can't wait semaphore");
		free(n);
		return NULL;
	}

	if (l == NULL) {
		debug_print("oops, list has been destroyed\n");
		free(n);
		return NULL;
	}

	n->elem = elem;

	/* If the list is empty, this is the first and only element */
	if (l->head == NULL) {
		l->head = l->tail = n;
		n->prev = n->next = NULL;
	}
	else {
		switch (mode) {
			/* New node becomes the head */
			case ADD_FIRST:	
				l->head->prev = n;
				n->next = l->head;
				n->prev = NULL;
				l->head = n;
				break;

			/* New node becomes the tail */
			case ADD_LAST:
				l->tail->next = n;
				n->next = NULL;
				l->tail = n;
				break; 

			/* Should never happen */
			default:
				debug_print("invalid mode, %d", mode);
				assert(0);
				break;
		}
	}
	
	l->size++;	

	if (sem_post(&(l->sem)) != 0) {
		perror("list_init: can't post semaphore");
	}

	return n->elem;
}

void* list_addFirst(list_t* l, void* elem) {
	return list_add(l, elem, ADD_FIRST);
}

void* list_addLast(list_t* l, void* elem) {
	return list_add(l, elem, ADD_LAST);
}

void* list_getElem(list_t* l, unsigned int index, bool clone) {
	if ((l == NULL) || (index == 0) || (index > l->size)) {
		debug_print("invalid parameter\n");
		return NULL;
	}

	node_t* tmp = l->head;

	if (tmp == NULL) {
		return NULL;
	}
	for (unsigned int i=1;i<index;i++) {
		tmp = tmp->next;
	}
	if (clone) {
		return l->clone(tmp->elem);
	}
	else {
		return tmp->elem;
	}
}

int list_remove(list_t* l, void* elem) {
	if ((l == NULL) || (elem == NULL)) {
		debug_print("invalid parameter\n");
		return -1;
	}

	/* Find the element to remove */
	node_t* n = l->head;
	while ((n != NULL) && (l->compare(n->elem,elem) != 0)) {
		n = n->next;
	} 

	if (n == NULL) {
		return -2;
	}

	/* XXX: thread safeness */

	/* If this is the first node, the head must be updated */
	if (n == l->head) {
		l->head = n->next;
	}
	/* n->prev is not NULL, skip node (forward) */
	else {
		n->prev->next = n->next;
	}

	/* If this is the last node, the tail must be updated */
	if (n == l->tail) {
		l->tail = n->prev;
	}
	/* n->next is not NULL, skip node (backward) */
	else {	
		n->next->prev = n->prev;;
	}

	/* Update list size */
	l->size--;
	n->prev = n->next = NULL;

	/* If element has been dynamically allocated, free it */
	if (l->destroy != NULL) {
		l->destroy(n->elem);
	}
	/* Free node */
	free(n);

	return 0;
}

void* list_find(list_t* l, void* elem) {
	if ((l == NULL) || (elem == NULL)) {
		debug_print("invalid parameter\n");
		return NULL;
	}

	/* This is checked by init(), but better safe than sorry */
	assert(l->compare != NULL);

	node_t* tmp = l->head;
	while (tmp != NULL) {
		if (l->compare(tmp->elem,elem) == 0) {
			return tmp->elem;
		}
		else {
			tmp = tmp->next;
		}
	} 
	return NULL;
}

void list_print(list_t* l, FILE* fd) {
	if ((l == NULL) || (l->print == NULL) || (fd == NULL)) {
		debug_print("invalid parameter\n");
		return;
	}

	fprintf(fd, "(");
	node_t* head = l->head;
	if (head == NULL) {
		fprintf(fd, "empty");
	}
	while (head) {
		l->print(head->elem, fd);
		head = head->next;
	} 
	fprintf(fd, ") ");
}

int list_destroy(list_t** l) {
	if ((l == NULL) || (*l == NULL)) {
		debug_print("invalid parameter\n");
		return EINVAL;
	}
	
	if (sem_wait(&((*l)->sem)) != 0) {
		perror("list_destroy: can't wait semaphore");
		return errno;
	}

	list_t* tmpl = *l;
	*l = NULL;
	while (tmpl->head) {
		node_t* tmpn = tmpl->head;
		tmpl->head = tmpl->head->next;
		tmpl->size--;
		if (tmpl->destroy != NULL) {
			tmpl->destroy(tmpn->elem);
		}
		free(tmpn);
	}

	assert(tmpl->head == NULL);
	assert(tmpl->size == 0);

	if (sem_post(&(tmpl->sem)) != 0) {
		perror("list_destroy: can't post semaphore");
	}

	free(tmpl);

	return 0;
}
