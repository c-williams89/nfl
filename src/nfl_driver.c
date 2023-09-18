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
                c = getopt_long(argc, argv, "", long_options, &option_index);
                if (-1 == c) {
                        break;
                }

                switch (c) {
                        case 0:
                                break;
                        case 1:
                                break;
                        case 'p':
                                if (my_opts->option) {
                                        goto OPTION_EXIT;
                                }
                                my_opts->option = 'p';
                                my_opts->search_param1 = optarg;
                                printf("player case\n");
                                
                                break;
                        case 's':
                                if (my_opts->option) {
                                        goto OPTION_EXIT;
                                }
                                my_opts->option = 's';
                                my_opts->search_param1 = optarg;
                                printf("search case\n");
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
                                printf("teams case\n");
                                break;
                        case 'd':
                                printf("distance case\n");
                                break;
                        case '?':
                                goto FILE_EXIT;
                        case ':':
                                goto FILE_EXIT;
                        default:
                                break;

                }
                
        }

        // FILE *fp = fopen("./test/test_data/data_4_entries.txt", "r");
        // FILE *fp = fopen("./test/test_data/nfldata.txt", "r");
        FILE *fp = fopen("./test/test_data/nfldata.tsv", "r");

        if (!validate_file(fp)) {
                goto FILE_EXIT;
        }

        uint16_t num_entries = get_num_entries(fp);
        if (!num_entries) {
                goto FILE_EXIT;
        }

        hash_t *player_table = hash_table_create(num_entries * 2, hash);
        hash_t *team_table = hash_table_create(2000, hash);
        // hash_t *player_table = hash_table_create(2, hash);
        // hash_t *team_table = hash_table_create(2, hash);
        for (uint16_t entry = 0; entry < num_entries; ++entry) {
                char *curr_entry = NULL;
                size_t len = 0;
                getline(&curr_entry, &len, fp);
                curr_entry[strcspn(curr_entry, "\n")] = '\0';
                player_t *player = player_create(team_table, curr_entry);
                // player_add_to_team(player, team_table);
                player_insert(player, player_table);
        }
        my_opts->team_table = team_table;
        my_opts->player_table = player_table;


        print_helper(my_opts);
        // print_roster(team_table, "1987Indianapolis Colts");
        // print_player(my_opts->search_param1, player_table);
        // print_search_results(my_opts->search_param1, player_table);
        // print_teams(team_table);
OPTION_EXIT:
        // fprintf(stderr, "./nfl: too many arguments\n");
FILE_EXIT:
        hashtable_destroy(my_opts);
        free(my_opts);
        fclose(fp);
        return 1;
}