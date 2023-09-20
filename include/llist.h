#ifndef LLIST_H
#define LLIST_H
#include <stdbool.h>
typedef struct llist_t llist_t;
typedef struct node_t *llist_iter_t;
llist_t *llist_create(void);
bool llist_enqueue(llist_t * list, void *data);
void *llist_dequeue(llist_t * list);
bool llist_push(llist_t * list, void *data);
void *llist_pop(llist_t * list);
bool llist_is_empty(llist_t * llist);
void *llist_peek(llist_t * list, int index);
void llist_destroy(llist_t * list);
int llist_get_size(llist_t * llist);
bool llist_create_iter(llist_t * llist, llist_iter_t * iter);
void *llist_iter_next(llist_iter_t * iter);
bool llist_iter_is_empty(llist_iter_t iter);
#endif
