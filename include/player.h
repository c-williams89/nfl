#ifndef PLAYER_H 
	#define PLAYER_H
	#include <stdbool.h>
	typedef struct player_t player_t;
	player_t * player_create(char *curr);
	bool player_insert(player_t *player, hash_t *ht);
#endif



