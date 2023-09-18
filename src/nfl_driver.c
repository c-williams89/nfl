#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>

#include "../include/hashtable.h"
#include "../include/io_helper.h"
#include "../include/player.h"
#include "../include/llist.h"
#include "../include/long_opts_helper.h"

uint64_t hash(char *key) {
        uint64_t hash = 5381;
        int c;

        while ((c = *key++)) {
                hash = ((hash << 5) + hash) + c;

        }
        return hash;
}

int main (int argc, char **argv) {
        int c;
        l_opts *my_opts = calloc(1, sizeof(l_opts));
        FILE *fp = fopen("./test/test_data/nfldata.txt", "r");
        // char *file = "./test/test_data/nfldata.txt";

        while (1) { //TODO: Figure out better way to write this
                int option_index = 0;
                struct option long_options[] = {
                        {"player", required_argument, NULL, 'p'},
                        {"search", required_argument, NULL, 's'},
                        {"stats", required_argument, NULL, 'S'},
                        {"roster", required_argument, NULL, 'r'},
                        {"teams", no_argument, NULL, 't'},
                        {"distance", required_argument, NULL, 'd'},
                        {0, 0, 0, 0}
                };
                c = getopt_long(argc, argv, "f:", long_options, &option_index);
                if (-1 == c) {
                        break;
                }

                switch (c) {
                        case 'p':
                                if (my_opts->option) {
                                        goto OPTION_EXIT;
                                }
                                my_opts->option = 'p';
                                my_opts->search_param1 = optarg;                                
                                break;
                        case 's':
                                if (my_opts->option) {
                                        goto OPTION_EXIT;
                                }
                                my_opts->option = 's';
                                my_opts->search_param1 = optarg;
                                break;
                        case 'S':
                                my_opts->option = 'S';
                                my_opts->search_param1 = optarg;
                                break;
                        case 'r':
                                if (my_opts->option) {
                                        goto OPTION_EXIT;
                                }
                                my_opts->option = 'r';
                                my_opts->search_param1 = optarg;
                                if ((optind < argc) && (*argv[optind] != '-')) {
                                        my_opts->search_param2 = argv[optind];
                                }
                                ++optind;
                                break;
                        case 't':
                                if (my_opts->option) {
                                        goto OPTION_EXIT;
                                }
                                my_opts->option = 't';
                                break;
                        case 'd':
                                my_opts->option = 'd';
                                my_opts->search_param1 = optarg;
                                if ((optind < argc) && (*argv[optind] != '-')) {
                                        my_opts->search_param2 = argv[optind];
                                }
                                ++optind;
                                break;
                        case 'f':
                                fclose(fp);
                                fp = fopen(optarg, "r");
                                if (!fp) {
                                        goto OPTION_EXIT;
                                }
                                break;
                        case '?':
                                goto FILE_EXIT;
                        case ':':
                                goto FILE_EXIT;
                        default:
                                break;
                }  
        }

        if (!validate_file(fp)) {
                goto FILE_EXIT;
        }

        uint16_t num_entries = get_num_entries(fp);
        if (!num_entries) {
                goto FILE_EXIT;
        }

        hash_t *player_table = hash_table_create(num_entries * 2, hash);
        hash_t *team_table = hash_table_create(2000, hash);
        for (uint16_t entry = 0; entry < num_entries; ++entry) {
                char *curr_entry = NULL;
                size_t len = 0;
                getline(&curr_entry, &len, fp);
                curr_entry[strcspn(curr_entry, "\n")] = '\0';
                player_t *player = player_create(team_table, curr_entry);
                player_insert(player, player_table);
        }
        my_opts->team_table = team_table;
        my_opts->player_table = player_table;

        print_helper(my_opts);
        hashtable_destroy(my_opts);

FILE_EXIT:
        fclose(fp);
OPTION_EXIT:
        free(my_opts);
        return 1;
}