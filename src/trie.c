#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#define LOWER_TO_INDEX(c) ((int)c - (int)'a')
#define UPPER_TO_INDEX(c) ((int)c - 39)

#define LOWER_TO_CHAR(i) ((char)i + (char)'a')
#define UPPER_TO_CHAR(i) ((char)i + 39)

#define NUM_CHARS 54

typedef struct trie_t {
	struct trie_t *children[NUM_CHARS];
	bool terminal;
} trie_t;

static trie_t *trie_create_node();
static void trie_print_rec(trie_t * node, char *prefix, int len);

trie_t *trie_create(void)
{
	return calloc(1, sizeof(trie_t));
}

void trie_destroy(trie_t **trie)
{
	if (!trie || !*trie) {
		return;
	}
	for (int i = 0; i < NUM_CHARS; ++i) {
		trie_destroy(&((*trie)->children[i]));
	}
	free(*trie);
}


int trie_insert(trie_t ** trie, const char *target)
{
	int exit_status = 0;
	if (!trie || !target) {
		fprintf(stderr, "trie_insert: Invalid argument");
		goto EXIT;
	}
	if (!*trie) {
		*trie = trie_create_node();
		if (!(*trie)) {
			goto EXIT;
		}
	}

	trie_t *tmp = *trie;
	int len = strlen(target);
	for (int i = 0; i < len; ++i) {
		int c;
		if (' ' == target[i]) {
			c = 52;
		} else if ('.' == target[i]) {
			c = 53;
		} else if (isupper(target[i])) {
			c = UPPER_TO_INDEX(target[i]);
		} else {
			c = LOWER_TO_INDEX(target[i]);
		}
		if (!(tmp->children[c])) {
			tmp->children[c] = trie_create_node();
		}
		tmp = tmp->children[c];
	}
	exit_status = 1;
	if (!tmp->terminal) {
		tmp->terminal = true;
	}
EXIT:
	return exit_status;
}

void trie_print(trie_t * trie)
{
	if (!trie) {
		return;
	}
	trie_print_rec(trie, NULL, 0);
}

static trie_t *trie_create_node()
{
	trie_t *node = calloc(1, sizeof(*node));
	if (!node) {
		perror("calloc");
		errno = 0;
	}
	return node;
}

static void trie_print_rec(trie_t * node, char *prefix, int len)
{
	char *new_prefix = calloc(len + 2, sizeof(char));
	memcpy(new_prefix, prefix, len);
	new_prefix[len + 1] = '\0';

	if (node->terminal) {
		printf("%s\n", prefix);
	}
	for (int i = 0; i < NUM_CHARS; ++i) {
		if (node->children[i]) {
			if (52 == i) {
				new_prefix[len] = 32;
			} else if (53 == i) {
				new_prefix[len] = 46;
			} else if (i < 26) {
				new_prefix[len] = LOWER_TO_CHAR(i);
			} else {
				new_prefix[len] = UPPER_TO_CHAR(i);
			}
			trie_print_rec(node->children[i], new_prefix, len + 1);
		}
	}
	free(new_prefix);
}
