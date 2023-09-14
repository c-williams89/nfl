#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

// #include "../include/player.h"
#include "../include/hashtable.h"
#include "../include/llist.h"

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

static team_t * team_create(char * curr_team) {
        team_t *team = calloc(1, sizeof(*team));
        team->year = strsep(&curr_team, ",");
        team->team_name = strsep(&curr_team, "\t");
        return team; 
}

player_t * player_create(char *current) {
        
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
                team_t *team = team_create(curr_team);
                llist_enqueue(player->teams, team);
                player->num_teams += 1;
                curr_team = strsep(&current, "\t");
        }

        // Debugging print
        // while (!llist_is_empty(player->teams)) {
        //         team_t *curr_team = (team_t *)llist_dequeue(player->teams);
        //         printf("%s, %s\n", curr_team->year, curr_team->team_name);
        // }
        // printf("\n");
        
EXIT:
        return player;
}

bool player_insert(player_t *player, hash_t *ht) {
        char *key = player->id;
        hash_table_insert(ht, key, player);
        return true;
}

static bool team_insert(team_t *team, hash_t *ht, char *key) {
        hash_table_insert(ht, key, team);
        return true;
}

void player_update_team(player_t *player, hash_t *team_table) {
        team_t *tmp = NULL;
        printf("For player: %s\n", player->name);
        for (int i = 0; i < player->num_teams; ++i) {
                tmp = (team_t *)llist_peek(player->teams, i);
                size_t len = strlen(tmp->team_name);
                char *key = calloc(len + 4, sizeof(char));
                memcpy(key, tmp->year, 4);
                strncat(key, tmp->team_name, len);
                printf("Key: %s\n", key);
                printf("Team: %s\n", tmp->team_name);
                team_insert(tmp, team_table, key);
                if (!tmp->roster) {
                        tmp->roster = llist_create();
                        if (!tmp->roster) {
                                return;
                        }
                }
                llist_enqueue(tmp->roster, player);
        }
}