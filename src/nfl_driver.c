#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/hashtable.h"
#include "../include/io_helper.h"
#include "../include/player.h"
#include "../include/llist.h"

uint64_t hash(char *key) {
        uint64_t hash = 5381;
        int c;

        while (c = *key++) {
                hash = ((hash << 5) + hash) + c;

        }
        return hash;
}

int main (void) {
        FILE *fp = fopen("./test/test_data/data_4_entries.txt", "r");
        if (!validate_file(fp)) {
                goto FILE_EXIT;
        }

        uint16_t num_entries = get_num_entries(fp);
        if (!num_entries) {
                goto FILE_EXIT;
        }

        hash_t *player_table = hash_table_create(num_entries * 2, hash);

        for (uint16_t entry = 0; entry < num_entries; ++entry) {
                char *curr_entry = NULL;
                size_t len = 0;
                getline(&curr_entry, &len, fp);
                curr_entry[strcspn(curr_entry, "\n")] = '\0';
                player_t *player = player_create(curr_entry);
                player_insert(player, player_table);
        }
        hash_table_print(player_table);

// Create player hashtable with known number of entries * 2
// parse input and populate player struct
// Use player.team to populate team struct
// Add team struct to 

FILE_EXIT:
        fclose(fp);
        return 1;
}