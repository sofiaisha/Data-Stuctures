#ifndef __RBTREE_
#define __RBTREE_

enum colors {RED, BLACK};

typedef struct rbnode_t rbnode_t;
struct rbnode_t {
        void*           elem;
	enum colors	color;
        rbnode_t*       parent;
        rbnode_t*       left;
        rbnode_t*       right;
};

typedef struct {
        rbnode_t*       root;
	rbnode_t*	nil;
        unsigned int    size;
        int             (*compare)(void*, void*);
        void            (*print)(void*, FILE*);
        void*           (*clone)(void*);
        void            (*destroy)(void*);
} rbtree_t;

#endif
