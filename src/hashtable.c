#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "hashtable.h"
#include "player.h"
#include "llist.h"
#include "long_opts_helper.h"

#define LOAD(x) ((x * 3) / 4)

enum { PTR_SIZE = 8 };

typedef struct entry_t {
        void *data;
        bool b_to_delete;
        char *key;
} entry_t;

// HACK: Re-write entries to be a single pointer, each index is an entry, rather
//  than a pointer to an entry
typedef struct hash_t {
        entry_t **entries;
        hash_f hash_function;
        uint32_t max_cap;
        uint32_t curr_size;
}hash_t;

hash_t *hash_table_create(uint32_t size, hash_f hf) {
        hash_t *table = NULL;
        
        if (!hf) {
                fprintf(stderr, "hash_table_create: Invalid hash_f - NULL\n");
                goto EXIT;
        }
        
        if (size < 1) {
                fprintf(stderr, "hash_table_create: Invalid size - Must be positive integer\n");
                goto EXIT;
        }

        table = calloc(1, sizeof(*table));
        if (!table) {
                perror("hash_table_create");
                errno = 0;
                goto EXIT;
        }

        table->max_cap = size;
        table->hash_function = hf;
        table->entries = calloc(table->max_cap, sizeof(entry_t *));
        if (!table->entries) {
                perror("hash_table_create");
                errno = 0;
                free(table);
                table = NULL;
        }

EXIT:
        return table;
}

static entry_t *create_entry(void *data, char *key) {
        entry_t *entry = calloc(1, sizeof(*entry));
        // TODO: ABC
        entry->data = data;
        entry->key = key;
        return entry;
}

bool hash_table_insert(hash_t *ht, char *key, void *data) {
        if (!ht || !key || !data) {
                fprintf(stderr, "hash_table_insert\n");
                return false;
        }

        if (ht->curr_size >= LOAD(ht->max_cap)) {
                // TODO: RESIZE
        }

        entry_t *entry = create_entry(data, key);

        uint64_t index = (ht->hash_function(key) % ht->max_cap);
        for (uint32_t i = 0; i < ht->max_cap; ++i) {
                uint32_t try = (i + index) % ht->max_cap;
                
                if (!ht->entries[try] || ht->entries[try]->b_to_delete) {
                        ht->entries[try] = entry;
                        ht->curr_size += 1;
                        break;
                } 
        }
        return true;
}

void *find(hash_t * table, char *key) {
        uint64_t index = (table->hash_function(key) % table->max_cap);
        for (uint32_t i = 0; i < table->max_cap; ++i) {
                int try = (i + index) % table->max_cap;
                if (table->entries[try]) {
                        if (0 == (strncmp(table->entries[try]->key, key, strlen(key)))) {
                                return table->entries[try]->data;
                        }
                }
        }

        return NULL;
}

void *find_no_key(hash_t *table, char *val, comp_f compare) {
        void *data = NULL;
        for (uint32_t i = 0; i < table->max_cap; ++i) {
                if (table->entries[i]) {
                        if (compare(table->entries[i]->data, val)) {
                                data = table->entries[i]->data;
                        }
                }
        }

        return data;
}

llist_t *find_matches(hash_t *table, char *val, comp_f compare) {
        llist_t *search_results = llist_create();
        for (uint32_t i = 0; i < table->max_cap; ++i) {
                if (table->entries[i]) {
                        if (compare(table->entries[i]->data, val)) {
                                llist_enqueue(search_results, table->entries[i]->data);
                        }
                }
        }
        return search_results;
}

llist_t *find_teams(hash_t *table) {
        llist_t *team_results = llist_create();
        for (uint32_t i = 0; i < table->max_cap; ++i) {
                if (table->entries[i]) {
                        llist_enqueue(team_results, table->entries[i]->data);
                }
        }

        return team_results;
}

void hashtable_destroy(l_opts *my_opts) {
        if (!my_opts) {
                return;
        }

        hash_t *teams = my_opts->team_table;
        for (uint32_t i = 0; i < teams->max_cap; ++i) {
                if (teams->entries[i]) {
                        team_destroy(teams->entries[i]->data);
                        free(teams->entries[i]->key);
                }
        }

        hash_t *players = my_opts->player_table;
        for (uint32_t i = 0; i < players->max_cap; ++i) {
                if (players->entries[i]) {
                        player_destroy(players->entries[i]->data);
                        free(players->entries[i]->data);
                }
                free(players->entries[i]);
        }
        free(players->entries);
        free(players);

        for (uint32_t i = 0; i < teams->max_cap; ++i) {
                if (teams->entries[i]) {
                        free(teams->entries[i]->data);
                }
                free(teams->entries[i]);
        }
        free(teams->entries);
        free(teams);

}

llist_t *get_player(hash_t *player_table) {
        llist_t *players = llist_create();
        for (uint32_t i = 0; i < player_table->max_cap; ++i) {
                if (player_table->entries[i]) {
                        llist_enqueue(players, player_table->entries[i]->data);
                }
        }
        return players;
}

void reset_players(hash_t *player_table, del_f reset) {
        for (uint32_t i = 0; i < player_table->max_cap; ++i) {
                if (player_table->entries[i]) {
                        reset(player_table->entries[i]->data);
                }
        }

}