#ifndef TRIE_H
	#define TRIE_H
	typedef struct trie_t trie_t;
	trie_t *trie_create(void);
	void trie_print(trie_t * trie);
	int trie_insert(trie_t ** trie, const char *target);
#endif



