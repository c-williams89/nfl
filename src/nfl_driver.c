#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/hashtable.h"
#include "../include/io_helper.h"

int main (void) {
        FILE *fp = fopen("./test/test_data/data_4_entries.txt", "r");
        if (!validate_file(fp)) {
                goto FILE_EXIT;
        }

        uint16_t num_entries = get_num_entries(fp);
        if (!num_entries) {
                goto FILE_EXIT;
        }

// Create player hashtable with known number of entries * 2
// parse input and populate player struct
// Use player.team to populate team struct
// Add team struct to 

FILE_EXIT:
        fclose(fp);
        return 1;
}