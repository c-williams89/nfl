#ifndef TRIE_H
#define TRIE_H

// Opaque data type for users to use with API
typedef struct trie_t trie_t;

/**
 * @brief Create an empty trie.
 * 
 * @return trie_t Pointer to the trie or NULL on failure.  
 */
trie_t *trie_create(void);

/**
 * @brief Frees memory back to OS, sets trie to null.
 * 
 * @param trie Address of where user stores trie. 
 */
void trie_destroy(trie_t ** trie);

/**
 * @brief Insert a word into the trie.
 * 
 * @param trie User address of trie to insert into.
 * @param target String to insert into trie.
 * 
 * @return 1 on success, 0 on failure. 
 */
int trie_insert(trie_t ** trie, const char *target);

/**
 * @brief Print list of words in the trie.
 * 
 * @param trie Pointer to trie to print. 
 */
void trie_print(trie_t * trie);
#endif
