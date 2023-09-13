#ifndef LLIST_H
	#define LLIST_H
	#include <stdbool.h>
	typedef struct llist_t llist_t;
	llist_t *llist_create(void);
	bool llist_enqueue(llist_t * list, void *data);
	void *llist_dequeue(llist_t * list);
	bool llist_push(llist_t * list, void *data);
	void *llist_pop(llist_t * list);
	bool llist_is_empty(llist_t * llist);
#endif



