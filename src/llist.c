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

int llist_enqueue(llist_t * llist, void *data)
{
	int exit_status = 0;
	if (!llist || !data) {
		goto EXIT;
	}

	node_t *node = calloc(1, sizeof(*node));
	if (!node) {
		goto EXIT;
	}

	node->data = data;

	if (llist->tail) {
		llist->tail->next = node;
	} else {
		llist->head = node;
	}

	llist->tail = node;
	++llist->size;

	exit_status = 1;

EXIT:
	return exit_status;
}

void *llist_dequeue(llist_t * list)
{
	void *data = NULL;
	if (!list || !list->head) {
		goto EXIT;
	}

	data = list->head->data;
	struct node_t *tmp = list->head;
	list->head = list->head->next;
	free(tmp);

	if (!list->head) {
		list->tail = list->head;
	}

	--list->size;

EXIT:
	return data;
}

bool llist_is_empty(llist_t * llist)
{
	if (!llist) {
		return false;
	}
	return llist->size > 0 ? false : true;
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

int llist_create_iter(llist_t * llist, llist_iter_t * iter)
{
	int exit_status = 0;
	if (!llist || !llist->head || !iter) {
		goto EXIT;
	}
	*iter = llist->head;
	exit_status = 1;
	
EXIT:
	return exit_status;
}

void *llist_iter_next(llist_iter_t * iter)
{
	if (!iter) {
		return NULL;
	}
	
	llist_iter_t tmp = *iter;
	*iter = tmp->next;
	return tmp->data;
}

bool llist_iter_is_empty(llist_iter_t iter)
{
	return (!iter) ? true : false;
}
