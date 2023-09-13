#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/hashtable.h"
#include "../include/io_helper.h"
#include "../include/player.h"

// typedef struct player_t {
//         char *id;
//         char *name;
//         char *position;
//         char *birthday;
//         char *college;
//         llist_t *teams;
// } player_t;

// typedef struct team_t {
//         char *team_name;
//         char *year;
//         llist_t *roster;
// }team_t;

int main (void) {
        FILE *fp = fopen("./test/test_data/data_4_entries.txt", "r");
        if (!validate_file(fp)) {
                goto FILE_EXIT;
        }

        uint16_t num_entries = get_num_entries(fp);
        if (!num_entries) {
                goto FILE_EXIT;
        }

        for (uint16_t entry = 0; entry < num_entries; ++entry) {
                char *curr_entry = NULL;
                size_t len = 0;
                getline(&curr_entry, &len, fp);
                player_create(curr_entry);
        }

        // char *curr_entry = NULL;
        // size_t len = 0;

        // while (getline(&curr_entry, &len, fp) != -1) {
        //         player_create(curr_entry);
        // }


// Create player hashtable with known number of entries * 2
// parse input and populate player struct
// Use player.team to populate team struct
// Add team struct to 

FILE_EXIT:
        fclose(fp);
        return 1;
}