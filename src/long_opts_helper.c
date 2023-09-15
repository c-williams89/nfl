#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/player.h"
#include "../include/hashtable.h"

typedef struct l_opts {
	char option;
	char *search_param1;
	char *search_param2;
	hash_t *player_table;
	hash_t *team_table;
} l_opts;

void print_helper (l_opts *my_opts) {
	size_t len = 0;
        switch (my_opts->option) {
                case 'r':
		 	len = strlen(my_opts->search_param1);
                	char *key = calloc(len + 5, sizeof(char));
                	memcpy(key, my_opts->search_param2, 4);
                	strncat(key, my_opts->search_param1, len);
			print_roster(my_opts->team_table, key);
			free(key);
			break;
		default:
			break;
        }
}