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

// void llist_destroy(llist_t ** list)
// {
// 	if (!list || !*list) {
// 		return;
// 	}
// 	llist_t *copy = *list;
// 	node_t *head = copy->head;
// 	while (head) {
// 		node_t *tmp = head;
// 		head = head->next;
// 		// delete(tmp->data);
// 		free(tmp);
// 	}
// 	free(copy);
// 	*list = NULL;
// }

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
