#ifndef PLAYER_H 
	#define PLAYER_H
	#include <stdbool.h>
	#include "../include/hashtable.h"
	typedef struct llist_t llist_t;
	typedef struct team_t team_t;
	typedef struct player_t player_t;
	player_t * player_create(hash_t *team_table, char *curr);
	void player_add_to_team(player_t *player, hash_t *team_table);
	bool player_insert(player_t *player, hash_t *ht);
	void print_player(char *player_arg, hash_t *player_table);
	void print_search_results(char *search_param, hash_t *player_table);
	void print_teams(hash_t *team_table);
	void print_roster (hash_t *team_table, char *key);
	void destroy_players();
	void data_destroy(hash_t *table);
	void player_destroy(player_t *player);
	void team_destroy(team_t *team);
	void player_stats(hash_t *player_table, char *name);
	void player_distance(hash_t *player_table, char *start, char *end);
	void player_oracle(hash_t *player_table);
	// void find_small_teams(hash_t *team_table);
#endif



