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
EXIT:
        return player;
}

bool player_insert(player_t *player, hash_t *ht) {
        char *key = player->id;
        hash_table_insert(ht, key, player);
        return true;
}

static bool team_insert(team_t *team, hash_t *ht, char *key, player_t *player) {
        team_t *tmp = (team_t *)find(ht, key);

        if (tmp) {
                llist_enqueue(tmp->roster, player);
        } else {
                team->roster = llist_create();
                llist_enqueue(team->roster, player);
                hash_table_insert(ht, key, team);
        }
        return true;
}

void player_add_to_team(player_t *player, hash_t *team_table) {
        team_t *tmp = NULL;
        for (int i = 0; i < player->num_teams; ++i) {
                tmp = (team_t *)llist_peek(player->teams, i);
                
                size_t len = strlen(tmp->team_name);
                char *key = calloc(len + 5, sizeof(char));
                memcpy(key, tmp->year, 4);
                strncat(key, tmp->team_name, len);

                team_insert(tmp, team_table, key, player);
        }
}

void print_options() {

}

int compare (player_t *player, char *val){
        if (0 == strncmp(player->name, val, strlen(player->name))) {
                return 1;
        }
        return 0;
}

void print_player(char *player_arg, hash_t *player_table) {
        // Prints Id, name, college, position, and teams played for
        player_t *player;
        if (strpbrk(player_arg, "0123456789")) {
                char *key = player_arg;
                player = (player_t *)find(player_table, key);
        } else {
                player = (player_t *)find_no_key(player_table, player_arg, compare);
        }
        printf("%s\t%s\t%s\t%s\n", player->id, player->name, player->college, player->position);
}
