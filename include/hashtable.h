#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stdint.h>
#include <stdbool.h>
#include "../include/llist.h"

typedef uint64_t(*hash_f) (char *);
typedef int (*comp_f)(void *, char *);
typedef void (*del_f)(void *);

// Opaque data type for users to use with API
typedef struct hash_t hash_t;

typedef struct l_opts l_opts;

/**
 * @brief Create an empty hashtable.
 * 
 * @param size Number of entries to initialize hashtable with. 
 * @param hash_f Custom hashing functions.
 * 
 * @return Pointer to hashtable on success, else NULL.
 */
hash_t *hash_table_create(uint32_t size, hash_f);

/**
 * @brief Insert data into hashtable.
 * 
 * @param ht Pointer to hashtable to insert into.
 * @param key Unique key to hash.
 * @param data Pointer to data to store in hashtable.
 * 
 * @return 1 on success, else 0.
 */
int hash_table_insert(hash_t * ht, char *key, void *data);

/**
 * @brief Find an entry in hashtable.
 * 
 * @param table Pointer to hashtable to search.
 * @param key Unique key associated with entry.
 * 
 * @return void * to data if found, else NULL. 
 */
void *find(hash_t * table, char *key);

/**
 * @brief Find entry in hashtable without key.
 * 
 * @param table Pointer to hashtable to search.
 * @param val Value of entry to find.
 * @param comp_f User defined custom compare function.
 * 
 * @return void * to data if found, else NULL.
 */
void *find_no_key(hash_t * table, char *val, comp_f);

/**
 * @brief Find all entries in hashtable matching value.
 * 
 * @param table Pointer to hashtable to search.
 * @param val Value to compare entries against.
 * @param comp_f User defined custom compare function. 
 * 
 * @return llist of matching entries based on comp_f, else NULL.
 */
llist_t *find_matches(hash_t * table, char *val, comp_f compare);

/**
 * @brief Find all teams in the hashtable.
 * 
 * @param table Pointer to the teams hashtable.
 * 
 * @return llist of teams if found, else NULL.
 */
llist_t *find_teams(hash_t * table);

/**
 * @brief free() both hashtables to os.
 * 
 * @param my_opts Pointer to l_opts struct containing player and team hashtables 
 */
void hashtable_destroy(l_opts * my_opts);

/**
 * @brief Find all players in the hashtable.
 * 
 * @param player_table Pointer to the players hashtable.
 * 
 * @return llist of players if found, else NULL. 
 */
llist_t *get_player(hash_t * player_table);

/**
 * @brief Reset entry values after conducting BFS.
 * 
 * @param player_table Pointer to players hashtable to reset.
 * @param reset Custom function to reset data. 
 */
void reset_players(hash_t * player_table, del_f reset);

/**
 * @brief Reset entry values after conducting BFS.
 *
 * @param team_table Pointer to teams hashtable to reset.
 * @param reset Custom function to reset data.
 */
void reset_teams(hash_t * team_table, del_f reset);

/**
 * @brief Get number of entries in hashtable.
 * 
 * @param table Pointer to hashtable.
 * 
 * @return uint32_t number of entries, NULL on error. 
 */
uint32_t hashtable_get_size(hash_t * table);
#endif
