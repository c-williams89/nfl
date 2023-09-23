#ifndef LONG_OPTS_HELPER_H
#define LONG_OPTS_HELPER_H
#include <stdio.h>
#include <stdbool.h>
typedef struct l_opts {
	char option;
	char *search_param1;
	char *search_param2;
	hash_t *player_table;
	hash_t *team_table;
	llist_t *ignored;
} l_opts;
int print_helper(l_opts * my_opts);
#endif
