#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "../include/llist.h"

typedef struct node_t {
	struct node_t *next;
	void *data;
} node_t;

struct llist_t {
	node_t *head;
	node_t *tail;
	int size;
};

typedef struct node_t *llist_iter_t;

llist_t *llist_create()
{
	llist_t *llist = calloc(1, sizeof(*llist));
	if (!llist) {
		perror("llist_create");
		errno = 0;
	}
	return llist;
}

bool llist_enqueue(llist_t * llist, void *data)
{
	if (!llist || !data) {
		return false;
	}

	node_t *node = calloc(1, sizeof(*node));
	if (!node) {
		return false;
	}

	node->data = data;
	// node->next = NULL;

	if (llist->tail) {
		llist->tail->next = node;
	} else {
		llist->head = node;
	}

	llist->tail = node;
	++llist->size;

	return true;
}

void *llist_dequeue(llist_t * list)
{
	if (!list || !list->head) {
		return NULL;
	}
	void *data = list->head->data;
	struct node_t *tmp = list->head;
	list->head = list->head->next;
	free(tmp);

	if (!list->head) {
		list->tail = list->head;
	}
	--list->size;
	return data;
}


bool llist_is_empty(llist_t * llist)
{
	if (!llist) {
		return false;
	}
	return llist->size > 0 ? false : true;
}

void *llist_peek(llist_t *list, int idx) {
	if (!list) {
		return 0;
	}
	node_t *node = list->head;
	while (idx--) {
		node = node->next;
	}	
	return node->data;
}

void llist_destroy(llist_t * llist)
{
	if (!llist) {
		return;
	}

	struct node_t *node = llist->head;
	struct node_t *temp = NULL;
	while (node) {
		temp = node;
		node = node->next;
		free(temp);
	}
	llist->head = NULL;
	llist->tail = NULL;
	free(llist);
}

int llist_get_size(llist_t *llist) {
	return llist->size;
}

bool llist_create_iter(llist_t *llist, llist_iter_t *iter) {
	*iter = llist->head;
	return true;
}

void *llist_iter_next(llist_iter_t *iter) {
	llist_iter_t tmp = *iter;
	*iter = tmp->next;
	return tmp->data;
}

bool llist_iter_is_empty(llist_iter_t iter) {
	return (!iter) ? true : false;
}