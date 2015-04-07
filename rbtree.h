#ifndef __RBTREE_
#define __RBTREE_

enum colors {RED, BLACK};

typedef struct rbnode_t rbnode_t;
struct rbnode_t {
        void*           elem;
        rbnode_t*       parent;
        rbnode_t*       left;
        rbnode_t*       right;
	enum colors	color;
};

typedef struct {
        rbnode_t*       root;
        unsigned int    size;
        int             (*compare)(void*, void*);
        void            (*print)(void*, FILE*);
        void*           (*clone)(void*);
        void            (*destroy)(void*);
	rbnode_t*	nil;
} rbtree_t;

#endif
