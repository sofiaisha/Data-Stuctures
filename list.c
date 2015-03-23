#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#include "list.h"
#include "util.h"

list_t* list_init(int (*compare)(void*, void*), void (*print)(void*, FILE*),
		  void* (*clone)(void*), void (*destroy)(void*)) {
	if ((compare == NULL) || (print == NULL) || (clone == NULL))  {
		debug_print("invalid parameter\n");
		return NULL;
	}

	list_t* l = (list_t*)malloc(sizeof(list_t));
	if (l == NULL) {
		perror("list_init: can't create new list\n");
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
		debug_print("invalid parameter\n");
		return EINVAL;
	}
	else {
		return l->size;
	}
}

void* list_add(list_t* l, void* elem, unsigned int mode) {
	if ((l == NULL) || (elem == NULL) || ((mode != ADD_FIRST) && (mode != ADD_LAST))) {
		debug_print("invalid mode\n");
		return NULL;
	}

	node_t* n = (node_t*)malloc(sizeof(node_t));
	if (n == NULL) {
		perror("list_add: can't create node\n");
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
				n->prev = l->tail;
				n->next = NULL;
				l->tail = n;
				break; 

			/* Should never happen */
			default:
				debug_print("invalid mode, %ud", mode);
				assert(0);
				break;
		}
	}
	
	l->size++;	

	return n->elem;
}

void* list_addSort(list_t* l, void* elem, unsigned int mode) {
	if ((l == NULL) || (elem == NULL) || ((mode != ADD_SORT_ASC) && (mode != ADD_SORT_DESC))) {
		debug_print("invalid parameter\n");
		return NULL;
	}

	if (l->compare == NULL) {
		debug_print("no compare function\n");
		return NULL;
	}

	/* If list is empty, add element */
	if (l->head == NULL) {
		list_add(l, elem, ADD_FIRST);
		return elem;
	}

	/* Move to the right position */
	node_t* tmp = l->head;
	switch (mode) {
		case ADD_SORT_ASC:
			/* Stop at the first larger element */
			while ( (tmp != NULL) && (l->compare)(elem, tmp->elem) > 0) {
				tmp = tmp->next;
			}
			break;

		case ADD_SORT_DESC:
			/* Stop at the first smaller element */
			while ( (tmp != NULL) && (l->compare)(elem, tmp->elem) < 0) {
				tmp = tmp->next;
			}
			break;

		default:
			/* Should never happen */
			debug_print("invalid mode, %ud", mode);
			assert(0);
	}

	if (tmp == l->head) {
		/* Insert new element at the beginning of the list */
		list_add(l, elem, ADD_FIRST);
		return elem;
	}

	if (tmp == NULL) {
		/* Insert new element at the end of the list */
		list_add(l, elem, ADD_LAST);
		return elem;
	}

	/* Insert new element in the middle of the list */
	node_t* n = (node_t*)malloc(sizeof(node_t));
	if (n == NULL) {
		perror("list_addSorted: can't create node\n");
		return NULL;
	}
	n->elem = elem;

	n->next 	= tmp;
	n->prev 	= tmp->prev;
	tmp->prev->next = n;
	tmp->prev 	= n;

	l->size++;

	return elem;	
}

void* list_addFirst(list_t* l, void* elem) {
	return list_add(l, elem, ADD_FIRST);
}

void* list_addLast(list_t* l, void* elem) {
	return list_add(l, elem, ADD_LAST);
}

void* list_addSortAsc(list_t* l, void* elem) {
	return list_addSort(l, elem, ADD_SORT_ASC);
}

void* list_addSortDesc(list_t* l, void* elem) {
	return list_addSort(l, elem, ADD_SORT_DESC);
}

void* list_getElem(list_t* l, unsigned int index, bool clone) {
	if ((l == NULL) || (index >= l->size)) {
		debug_print("invalid parameter\n");
		return NULL;
	}

	node_t* tmp = l->head;

	if (tmp == NULL) {
		return NULL;
	}
	for (unsigned int i=0;i<index;i++) {
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
	if ((l == NULL) || (elem == NULL) || (l->compare == NULL)) {
		debug_print("invalid parameter\n");
		return EINVAL;
	}

	/* Find the element to remove */
	node_t* n = l->head;
        while (n != NULL) {
                if (l->compare(n->elem,elem) == 0) {
                        break;
                }
                else {
                        n = n->next;
                }
        }
	if (n == NULL) {
		return ENOENT;
	}

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

int list_destroy(list_t* l) {
	if (l == NULL) {
		debug_print("invalid parameter\n");
		return EINVAL;
	}
	
	list_t* tmpl = l;
	while (tmpl->head) {
		node_t* tmpn = tmpl->head;
		tmpl->head = tmpl->head->next;
		tmpl->size--;
		if (tmpl->destroy != NULL) {
			tmpl->destroy(tmpn->elem);
		}
		free(tmpn);
	}

	assert(l->head == NULL);
	assert(l->size == 0);

	free(l);

	return 0;
}
