#ifndef __RADIXTREE_STRING_H
#define __RADIXTREE_STRING_H

#include <stdio.h>
#include <semaphore.h>
#include <stdbool.h>

typedef struct rtnode_t rtnode_t;
struct rtnode_t {
	rtnode_t**	childs;
}; 

typedef struct {
	rtnode_t**	childs;
	unsigned int 	alphabetSize;
	char		alphabetStart;
	unsigned int 	size;
	sem_t		sem;
} radixtree_t;

radixtree_t*	radixtree_init(unsigned int alphabetSize, char alphabetStart);

unsigned int	radixtree_size(radixtree_t* t);

unsigned int 	radixtree_height(radixtree_t* t, rtnode_t* n);

char* 		radixtree_add(radixtree_t* t, char* s);

char*		radixtree_find(radixtree_t* t, char* s);

void 		radixtree_print(radixtree_t* t, FILE* fd);

void		radixtree_destroy(radixtree_t** t);

/* Internal functions, do not use! */

void 		radixtree_destroy_internal(radixtree_t* t, rtnode_t* c);

#endif
