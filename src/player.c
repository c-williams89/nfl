#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include "../include/llist.h"

typedef struct player_t {
        char *id;
        char *name;
        char *position;
        char *birthday;
        char *college;
        llist_t *teams;
} player_t;

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
        printf("%s, %s, %s, %s, %s ", player->id, player->name, player->position, player->birthday, player->college);

        char *curr_team = NULL;
        curr_team = strsep(&current, "\t");
        player->teams = llist_create();
        while (curr_team) {
                team_t *team = team_create(curr_team);
                llist_enqueue(player->teams, team);
                curr_team = strsep(&current, "\t");
        }
        while (!llist_is_empty(player->teams)) {
                team_t *curr_team = (team_t *)llist_dequeue(player->teams);
                printf("%s, %s\n", curr_team->year, curr_team->team_name);
        }
        // Will need peek function to add teams to team hashtable
        printf("\n");
        
EXIT:
        return player;
}