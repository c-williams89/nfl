#ifndef PLAYER_H 
	#define PLAYER_H
	#include <stdbool.h>
	typedef struct llist_t llist_t;
	typedef struct player_t {
        	char *id;
        	char *name;
        	char *position;
        	char *birthday;
        	char *college;
        	llist_t *teams;
		int num_teams;
	}player_t;

	typedef struct team_t {
        	char *team_name;
        	char *year;
        	llist_t *roster;
	}team_t;
	player_t * player_create(char *curr);
	void player_add_to_team(player_t *player, hash_t *team_table);
	bool player_insert(player_t *player, hash_t *ht);
	void print_player(char *player_arg, hash_t *player_table);
#endif



