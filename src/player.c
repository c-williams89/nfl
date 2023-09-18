#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include "../include/hashtable.h"
#include "../include/llist.h"
#include "../include/trie.h"

typedef struct player_t {
        char *id;
        char *name;
        char *position;
        char *birthday;
        char *college;
        llist_t *teams;
        int num_teams;
        int level;
}player_t;

typedef struct team_t {
        char *team_name;
        char *year;
        llist_t *roster;
}team_t;

enum { NUM_COHORTS = 10};

static void bfs(player_t *player);

static team_t * team_create(char *year, char *name, player_t *player) {
        team_t *team = calloc(1, sizeof(*team));
        team->year = year;
        team->team_name = name;
        team->roster = llist_create();
        llist_enqueue(team->roster, player);
        return team; 
}

player_t * player_create(hash_t *team_table, char *current) {
        
        player_t *player = calloc(1, sizeof(*player));
        if (!player) {
                perror("player_create");
                errno = 0;
                goto EXIT;
        }

        player->id = strsep(&current, "\t");
        player->name = strsep(&current, "\t");
        player->position = strsep(&current, "\t");
        player->birthday = strsep(&current, "\t");
        player->college = strsep(&current, "\t");

        char *curr_team = NULL;
        curr_team = strsep(&current, "\t");
        player->teams = llist_create();
        while (curr_team) {
                char *year = strsep(&curr_team, ",");
                char *name = curr_team;
                size_t len = strlen(name);
                // TODO: Don't Calloc key, just grab whole string '1970,Miami Dolphins'
                //  And use as the key.
                char *key = calloc(len + 5, sizeof(char));
                memcpy(key, year, 4);
                strncat(key, name, len);
                team_t *tmp = find(team_table, key);
                if (tmp) {
                        llist_enqueue(player->teams, tmp);
                        llist_enqueue(tmp->roster, player);
                        free(key);
                } else {
                        team_t *team = team_create(year, name, player);
                        hash_table_insert(team_table, key, team);
                }
                player->num_teams += 1;
                curr_team = strsep(&current, "\t");
        }        
EXIT:
        return player;
}

bool player_insert(player_t *player, hash_t *ht) {
        char *key = player->id;
        hash_table_insert(ht, key, player);
        return true;
}

int compare_player (player_t *player, char *val){
        if (0 == strncmp(player->name, val, strlen(player->name))) {
                return 1;
        }
        return 0;
}

int compare_fields (player_t *player, char *val) {
        // Will hand a player and compare val against player name or college
        if (strstr(player->name, val) || strstr(player->college, val)) {
                return 1;
        }

        return 0;
}

void print_player(char *player_arg, hash_t *player_table) {
        player_t *player;

        if (strpbrk(player_arg, "0123456789")) {
                char *key = player_arg;
                player = (player_t *)find(player_table, key);
        } else {
                player = (player_t *)find_no_key(player_table, player_arg, (comp_f)compare_player);
        }

        if (player) {
                printf("%s\t%s\t%s\t%s\n", player->id, player->name, player->position, player->college);
                while (!llist_is_empty(player->teams)) {
                        team_t *team = (team_t *)llist_dequeue(player->teams);
                        printf("\t%s - %s\n", team->year, team->team_name);
                }
        }
}

void print_search_results(char *search_param, hash_t *player_table) {
        llist_t *search_results = find_matches(player_table, search_param, (comp_f)compare_fields);
        while (!llist_is_empty(search_results)) {
                player_t *player = (player_t *)llist_dequeue(search_results);
                printf("%s\t%s\t%s\n", player->id, player->name, player->college);
        } 
}

void print_teams(hash_t *team_table) {
        llist_t *team_results = find_teams(team_table);
        trie_t *team_trie = trie_create();
        while (!llist_is_empty(team_results)) {
                team_t *team = (team_t*)llist_dequeue(team_results);
                trie_insert(&team_trie, team->team_name);
        }

        trie_print(team_trie);
}

void print_roster (hash_t *team_table, char *key) {
        team_t *team = find(team_table, key);
        while (!llist_is_empty(team->roster)) {
                player_t *player = (player_t *)llist_dequeue(team->roster);
                printf("\t%s\t%s\t%s\t%s\n", player->id, player->name, player->position, player->college);
        }
}

void player_stats(hash_t *player_table, char *name) {
        player_t *player = find_no_key(player_table, name, (comp_f)compare_player);
        // if (player) {
        //         bfs(player);
        // }
        bfs(player);
}


void player_destroy(player_t *player) {
        llist_destroy(player->teams);
        free(player->id);
}

void team_destroy(team_t *team) {
        llist_destroy(team->roster);       
}

static void bfs(player_t *player) {
        //PSEUDO: enqueue the starting player
        // While llist isn't empty
        // Dequeue from llist
        // while player.team list isnt empt
        // dequeue player from player.teams
        // enqueue to llinked list
        // increment cohort index

        int cohorts[NUM_COHORTS] = { 1, 0 };
        // int cohorts[NUM_COHORTS] = { 0 };

        llist_t *cohort_queue = llist_create();
        player->level = 1;
        llist_enqueue(cohort_queue, player);
        uint16_t level = 1;
        player_t *next_player = NULL;
        // player_t *new_level = NULL;

        while (!llist_is_empty(player->teams)) {
                team_t *curr_team = (team_t*)llist_dequeue(player->teams);
                while (!llist_is_empty(curr_team->roster)) {
                        next_player = (player_t *)llist_dequeue(curr_team->roster);
                        if (!next_player->level) {
                                next_player->level = level;
                                llist_enqueue(cohort_queue, next_player);
                                cohorts[level] += 1;
                        }
                }
        }

        // while (!llist_is_empty(cohort_queue)) {
        for (int i = 2; i < NUM_COHORTS; ++i) {
                for (int j = 0; j < cohorts[i - 1]; ++j) {
                        player_t *curr_player = (player_t *)llist_dequeue(cohort_queue);
                        while (!llist_is_empty(curr_player->teams)) {
                                team_t *curr_team = (team_t *)llist_dequeue(curr_player->teams);

                                while (!llist_is_empty(curr_team->roster)) {
                                        next_player = (player_t *)llist_dequeue(curr_team->roster);
                                        if (!next_player->level) {
                                                llist_enqueue(cohort_queue, next_player);
                                                next_player->level = level;
                                                cohorts[i] += 1;
                                        }
                                }
                        }
                }
        }

        for (int i = 0; i < NUM_COHORTS; ++i) {
                printf("%d -- %d cohorts\n", i, cohorts[i]);
        }
}