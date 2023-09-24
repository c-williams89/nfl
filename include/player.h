#ifndef PLAYER_H
#define PLAYER_H
#include "../include/hashtable.h"
typedef struct llist_t llist_t;

// Struct to hold information about a single player.
typedef struct team_t team_t;

// Struct to hold information about a single team.
typedef struct player_t player_t;

/**
 * @brief Create a player from an entry from a TSV.
 * 
 * @param team_table Pointer to team hashtable to compare player to existing teams. 
 * @param curr String containing single entry from TSV.
 * 
 * @return player_t Pointer to created player, NULL on failure.
 */
player_t *player_create(hash_t * team_table, char *curr);

/**
 * @brief Add player struct to player hashtable.
 * 
 * @param player Pointer to player struct to add.
 * @param ht Pointer to hashtable to insert into.
 * 
 * @return 1 on success, 0 on failure. 
 */
int player_insert(player_t * player, hash_t * ht);

/**
 * @brief Print single player statistics.
 * 
 * @param player_arg String containing player name or id.
 * @param player_table Pointer to player hashtable.
 * 
 * @return 1 on success, 0 if player not found. 
 */
int print_player(char *player_arg, hash_t * player_table);

/**
 * @brief Print list of players that match provided search term.
 * 
 * @param search_param String containing value to compare against.
 * @param player_table Pointer to player hashtable.
 * 
 * @return 1 on success, 0 on failure. 
 */
int print_search_results(char *search_param, hash_t * player_table);

/**
 * @brief Prints list of unique team names.
 * 
 * @param team_table Pointer to hashtable containing teams. 
 */
void print_teams(hash_t * team_table);

/**
 * @brief Print the roster of provided team.
 * 
 * @param team_table Pointer to hashtable containing teams.
 * @param key String containing team name and year. 
 */
void print_roster(hash_t * team_table, char *key);


// void destroy_players();
// void data_destroy(hash_t * table);
/**
 * @brief free() player specific data fields, without freeing the player struct.
 * 
 * @param player Pointer to player struct to free().
 */
void player_destroy(player_t * player);

/**
 * @brief free() roster linked list associated with a team.
 * 
 * @param team Pointer to team struct to free().
 */
void team_destroy(team_t * team);

/**
 * @brief Print separation and cohort numbers of provided player.
 * 
 * @param player_table Pointer to player hashtable.
 * @param name String containing player to conduct BFS against.
 */
void player_stats(hash_t * player_table, char *name);

/**
 * @brief Print separation between two provided players.
 * 
 * @param player_table Pointer to player hashtable.
 * @param start String containing name of player to start at.
 * @param end String containing name of player to end at.
 * @param ignored Optional linked list of players to ignore from consideration. 
 */
void player_distance(hash_t * player_table, char *start, char *end,
		     llist_t * ignored);

/**
 * @brief Find and print the most and least connected players.
 * 
 * @param player_table Pointer to player hashtable.
 * @param team_table Pointer to team hashtable. 
 */
void player_oracle(hash_t * player_table, hash_t * team_table);
#endif
