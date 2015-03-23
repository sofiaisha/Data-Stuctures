#ifndef __PQUEUE_TEST_H
#define __PQUEUE_TEST_H

void pqueue_test_init(void);
void pqueue_test_empty(void);
void pqueue_test_push_min(void);
void pqueue_test_push_max(void);
void pqueue_test_peek_min(void);
void pqueue_test_peek_max(void);
void pqueue_test_pop_min(void);
void pqueue_test_pop_max(void);
void pqueue_test_update_min(void);
void pqueue_test_update_max(void);
void pqueue_test_print(void);
void pqueue_test_destroy(void);

int init_pqueue_suite(void);
int clean_pqueue_suite(void);

#endif
