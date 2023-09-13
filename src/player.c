#include <stdio.h>
#include <stdint.h>
#include "../include/llist.h"

typedef struct player_t {
        char *id;
        char *name;
        char *birthday;
        char *college;
        llist_t *team;
} player_t;

int player_create(char *current) {
        return -1;
}