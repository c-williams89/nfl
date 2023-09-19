#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <float.h>

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
        void *parent;
}player_t;

typedef struct team_t {
        char *team_name;
        char *year;
        llist_t *roster;
        void *parent;
}team_t;

enum { NUM_COHORTS = 10};

static void bfs(player_t *player);
static bool calc_distance(player_t *start, player_t *end);
static void print_distance(player_t *start);

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
                char *key = strdup(curr_team);
                char *year = strsep(&curr_team, ",");
                char *name = curr_team;

                team_t *tmp = (team_t*)find(team_table, key);

                if (tmp) {
                        llist_enqueue(tmp->roster, player);
                        llist_enqueue(player->teams, tmp);

                        free(key);
                } else {
                        tmp = team_create(year, name, player);
                        hash_table_insert(team_table, key, tmp);
                        llist_enqueue(player->teams, tmp);
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
        // TODO: Incorporate search by name or id
        player_t *player = find_no_key(player_table, name, (comp_f)compare_player);
        // if (player) {
        //         bfs(player);
        // }
        bfs(player);
}

void player_distance(hash_t *player_table, char *start, char *end) {
        player_t *player1 = find_no_key(player_table, start, (comp_f)compare_player);
        player_t *player2 = find_no_key(player_table, end, (comp_f)compare_player);
        if (calc_distance(player1, player2)) {
                print_distance(player2);
        }
}

void player_destroy(player_t *player) {
        llist_destroy(player->teams);
        free(player->id);
}

void team_destroy(team_t *team) {
        llist_destroy(team->roster);       
}

void reset(player_t *player) {
        player->level = 0;
}

void reset_team(team_t *team) {
        team->parent = NULL;
}

static void bfs(player_t *player) {
        int cohorts[NUM_COHORTS] = { 1, 0 };
        llist_t *cohort_queue = llist_create();
        player->level = 0;
        // player_t *start = player;
        player->parent = player;
        llist_enqueue(cohort_queue, player);

        while (!llist_is_empty(cohort_queue)) {
                player_t *curr = (player_t *)llist_dequeue(cohort_queue);
                while (!llist_is_empty(curr->teams)) {
                        team_t *team = (team_t *)llist_dequeue(curr->teams);
                        while (!llist_is_empty(team->roster)) {
                                player_t *next = (player_t *)llist_dequeue(team->roster);
                                if (next->level || (next == player)) {
                                        continue;
                                } else {
                                        next->level = (curr->level + 1);
                                        cohorts[next->level] += 1;
                                        llist_enqueue(cohort_queue, next);
                                }
                        }
                }
        }   
        printf("Network stats for %s\n", player->name);
        int total_connected = 0;
        float avg_sep = 0.0;
        for (int i = 0; i < NUM_COHORTS; ++i) {
                total_connected += cohorts[i];
                avg_sep += (i * cohorts[i]);
                printf("%d -- %d cohort%s", i, cohorts[i], (i == 0)? "\n": "s\n");
        }
        avg_sep = avg_sep / total_connected;
        printf("Average separation %.6f\n", avg_sep);
}

static bool calc_distance(player_t *start, player_t *end) {
        bool b_route_found = false;
        llist_t *queue = llist_create();
        start->parent = start;
        llist_enqueue(queue, start);
        while (!llist_is_empty(queue)) {
                player_t *player = (player_t *)llist_dequeue(queue);
                while (!llist_is_empty(player->teams)) {
                        team_t *team = (team_t *)llist_dequeue(player->teams);
                        if (!team->parent) {
                                team->parent = player;
                        }
                        while (!llist_is_empty(team->roster)) {
                                player_t *next_player = (player_t *)llist_dequeue(team->roster);
                                if (!next_player->parent) {
                                        next_player->parent = team;
                                        llist_enqueue(queue, next_player);
                                        if (next_player == end) {
                                                b_route_found = true;
                                                goto EXIT;
                                        }
                                }
                        }
                }
        }
EXIT:
        return b_route_found;
}

static void print_distance(player_t *end) {
        player_t *player = end;

        while (player->parent != player) {
                team_t *team = player->parent;
                player_t *parent = team->parent;
                printf("%s played for %s in %s with %s\n", player->name, team->team_name, team->year, parent->name);
                player = parent;
        }
}


struct oracle_t {
        player_t *best;
        player_t *worst;
        float best_sep;
        float worst_sep;
};

static void oracle_search(player_t *player, struct oracle_t *or_results) {
        int cohorts[NUM_COHORTS] = { 1, 0 };
        llist_t *cohort_queue = llist_create();
        player->level = 0;
        llist_enqueue(cohort_queue, player);
        while (!llist_is_empty(cohort_queue)) {
                player_t *curr = (player_t *)llist_dequeue(cohort_queue);
                llist_iter_t teams = { 0 };
                llist_create_iter(curr->teams, &teams);
                while ((!llist_iter_is_empty(teams))) {
                        team_t *team = (team_t *)llist_iter_next(&teams);
                        if (team->parent) {
                                continue;
                        }
                        team->parent = curr;
                        llist_iter_t players = { 0 };
                        llist_create_iter(team->roster, &players);
                        while (!llist_iter_is_empty(players)) {
                                player_t *next = (player_t *)llist_iter_next(&players);
                                if (next->level || next == player) {
                                        continue;
                                } else {
                                        next->level = (curr->level + 1);
                                        cohorts[next->level] += 1;
                                        llist_enqueue(cohort_queue, next);
                                }

                                if ((2 == next->level) && (cohorts[1] < 100)) {
                                        llist_destroy(cohort_queue);
                                        return;
                                }
                        }
                }
        }
        // llist_destroy(cohort_queue);

        int total_connected = 0;
        float avg_sep = 0.0;
        for (int i = 0; i < NUM_COHORTS; ++i) {
                total_connected += cohorts[i];
                avg_sep += (i * cohorts[i]);
        }
        avg_sep = avg_sep / total_connected;
        if (avg_sep > or_results->worst_sep) {
                or_results->worst_sep = avg_sep;
                or_results->worst = player;
        }
        
        if (avg_sep < or_results->best_sep) {
                or_results->best_sep = avg_sep;
                or_results->best = player;
        }
}

void player_oracle(hash_t *player_table, hash_t *team_table) {
        struct oracle_t or_results = { 0 };
        or_results.worst_sep = 0.0;
        or_results.best_sep = FLT_MAX;
        llist_t *players = get_player(player_table);
        player_t *player;
        int count = 0;
        while (!llist_is_empty(players)) {
                player = (player_t *)llist_dequeue(players);
                // printf("Processing player: %s\n", player->name);
                oracle_search(player, &or_results);
                reset_players(player_table, (del_f)reset);
                reset_teams(team_table, (del_f)reset_team);
                ++count;
                // if (count == 5) {
                //         break;
                // }
                if (0 == (count % 500)) {
                        printf("Processed %d players\n", count);
                } 
        }
        printf("And the winners are:\n");
        printf("The center of the universe (since 1960) with a score of %.6f is %s\n", or_results.best_sep, or_results.best->name);
        printf("The least connected NFL player with at least 100 connections of the NFL ");
        printf("universe (since 1960) with a score of %.6f is %s\n", or_results.worst_sep, or_results.worst->name);
}