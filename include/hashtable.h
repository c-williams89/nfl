#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stdint.h>
#include <stdbool.h>
#include "../include/llist.h"


typedef uint64_t (*hash_f)(char *);
typedef int (*comp_f)(void *, char *);
typedef void (*del_f)(void *);
typedef struct hash_t hash_t;
typedef struct l_opts l_opts;
hash_t *hash_table_create(uint32_t size, hash_f);
bool hash_table_insert(hash_t *ht, char *key, void *data);
void hash_table_print(hash_t *ht);
void hash_table_print_team(hash_t *ht);
void *find(hash_t * table, char *key);
void *find_no_key(hash_t *table, char *val, comp_f);
llist_t *find_matches(hash_t *table, char *val, comp_f compare);
llist_t *find_teams(hash_t *table);
// void hashtable_destroy(hash_t *table);
void hashtable_destroy(l_opts *my_opts);
llist_t * find_smaller_teams(hash_t *team_table);
llist_t *get_player(hash_t *player_table);
void reset_players(hash_t *player_table, del_f reset);
void reset_teams(hash_t *team_table, del_f reset);
#endif



