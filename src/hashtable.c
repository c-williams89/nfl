#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "hashtable.h"
#include "player.h"
#include "llist.h"

#define LOAD(x) ((x * 3) / 4)

enum { PTR_SIZE = 8 };

typedef struct entry_t {
        void *data;
        bool b_to_delete;
        char *key;
} entry_t;

typedef struct hash_t {
        entry_t **entries;
        hash_f hash_function;
        uint32_t max_cap;
        uint32_t curr_size;
}hash_t;

// static uint64_t hash(char *key) {
//         uint64_t hash = 5381;
//         int c;
//         c = *key++;

//         while (key) {
//                 hash = ((hash << 5) + hash) + c;
//                 c = *key++;
//         }
//         return hash;
// }

// entry_t *find(hash_t * table, char *key);

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

        entry_t *entry = create_entry(data, key);// TODO: write create_entry function

        uint64_t index = (ht->hash_function(key) % ht->max_cap);
        for (int i = 0; i < ht->max_cap; ++i) {
                uint32_t try = (i + index) % ht->max_cap;
                
                if (!ht->entries[try] || ht->entries[try]->b_to_delete) {
                        ht->entries[try] = entry;
                        ht->curr_size += 1;
                        break;
                }
        }
        return true;
        printf("Index: %ld\n", index);
}

void hash_table_print(hash_t *ht) {
        for (int i = 0; i < ht->max_cap; ++i) {
                printf("index %d:\t", i);
                if (ht->entries[i] && (!ht->entries[i]->b_to_delete)) {
                        player_t *player = (player_t *)ht->entries[i]->data;
                        printf("%s %s %s\n", player->id, player->name, player->college);
                        for (int i = 0; i < player->num_teams; ++i) {
                                team_t *team = llist_peek(player->teams, i);
                                printf("\t%s, %s\n", team->year, team->team_name);
                        }
                } else {
                        printf("\t-------------------\n");
                }
        }
}

void hash_table_print_team(hash_t *ht) {
        for (int i = 0; i < ht->max_cap; ++i) {
                if (ht->entries[i] && (!ht->entries[i]->b_to_delete)) {
                printf("index %d:\t", i);
                        team_t *team = (team_t *)ht->entries[i]->data;
                        printf("%s %s\n", team->year, team->team_name);
                        while (!llist_is_empty(team->roster)) {
                                player_t *curr_player = (player_t *)llist_dequeue(team->roster);
                                printf("\t\t%s, %s\n", curr_player->id, curr_player->name);
                        }
                printf("\n");
                } else {
                        // printf("\t-------------------\n");
                }
        }
}

void *find(hash_t * table, char *key) {
        uint64_t index = (table->hash_function(key) % table->max_cap);
        for (int i = 0; i < table->max_cap; ++i) {
                int try = (i + index) % table->max_cap;
                if (table->entries[try]) {
                        printf("try: %d\n", try);
                        if (0 == (strncmp(table->entries[try]->key, key, strlen(key)))) {
                                return table->entries[try]->data;
                        }
                } else {
                        break;
                }
        }
        return NULL;
}