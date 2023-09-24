#ifndef LLIST_H
#define LLIST_H

#include <stdbool.h>

// Opaque data type for users to use with API
typedef struct llist_t llist_t;

/**
 * Data type to represent linked list node as an iterable without modifying the 
 * linked list or head pointer.
 */
typedef struct node_t *llist_iter_t;

/**
 * @brief Create an empty list
 * 
 * @return llist_t* Pointer to linked list or NULL on failure. 
 */
llist_t *llist_create(void);

/**
 * @brief free() memory back to the OS.
 * 
 * @param list Pointer to linked list to free(). 
 */
void llist_destroy(llist_t * list);

/**
 * @brief Puts heap allocated data at back of queue
 * 
 * @param list Pointer to linked list to store the data.
 * @param data Pointer to data user wishes to store.
 * 
 * @return 1 on success, 0 on failure. 
 */
int llist_enqueue(llist_t * list, void *data);

/**
 * @brief Extracts the oldest element from the queue.
 * 
 * @param list Pointer to linked list to remove from. 
 */
void *llist_dequeue(llist_t * list);

/**
 * @brief Determine if there are items in the linked list.
 * 
 * @param llist Pointer to linked list to check
 * 
 * @return true if empty, else false. 
 */
bool llist_is_empty(llist_t * llist);

/**
 * @brief Create an iterable of a linked list node.
 * 
 * @param llist Pointer to linked list to create iterable from.
 * @param iter Pointer to iter to point to node address.
 * 
 * @return 1 on sucess, else 0.
 */
int llist_create_iter(llist_t * llist, llist_iter_t * iter);

/**
 * @brief Get the next item in a linked list.
 * 
 * @param iter Pointer to linked list node to get next item. 
 */
void *llist_iter_next(llist_iter_t * iter);

/**
 * Determine if iter is NULL.
 * 
 * @param iter Pointer to node to check.
 * 
 * @return true if NULL, else false. 
 */
bool llist_iter_is_empty(llist_iter_t iter);
#endif
