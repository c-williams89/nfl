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

uint64_t hash(char *key)
{
	uint64_t hash = 5381;
	int c;

	while ((c = *key++)) {
		hash = ((hash << 5) + hash) + c;

	}
	return hash;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
	     fprintf(stderr, "nfl: missing option\nTry '.nfl --help' for more information\n");
	     goto ARG_EXIT;
	}
	int c;
	int exit_status = 1;
	l_opts *my_opts = calloc(1, sizeof(l_opts));

	FILE *fp = fopen("./test/test_data/nfldata.txt", "r");
	FILE *help = NULL;

	while (1) {		//TODO: Figure out better way to write this
		int option_index = 0;
		struct option long_options[] = {
			{"player", required_argument, NULL, 'p'},
			{"search", required_argument, NULL, 's'},
			{"stats", required_argument, NULL, 'S'},
			{"roster", required_argument, NULL, 'r'},
			{"teams", no_argument, NULL, 't'},
			{"distance", required_argument, NULL, 'd'},
			{"oracle", no_argument, NULL, 'o'},
			{"help", no_argument, NULL, 'h'},
			{"ignore", required_argument, NULL, 'i'},
			{0, 0, 0, 0}
		};
		c = getopt_long(argc, argv, "f:i:", long_options,
				&option_index);
		if (-1 == c) {
			if (argv[optind]) {
				fprintf(stderr, "nfl: too many args\n");
				goto FILE_EXIT;
			}
			break;
		}

		switch (c) {
		case 1:
			break;
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
		case 'o':
			if (my_opts->option) {
				goto OPTION_EXIT;
			}
			my_opts->option = 'o';
			break;
		case 'h':
			help = fopen("./doc/help.txt", "r");
			if (!help) {
				break;
			}
			char line[255];
			while (fgets(line, 255, help)) {
				fprintf(stdout, "%s", line);
			}
			fclose(help);
			goto FILE_EXIT;
		case 'i':
			if (!my_opts->ignored) {
				my_opts->ignored = llist_create();
			}
			llist_enqueue(my_opts->ignored, optarg);
			break;
		case '?':
		// Case if required arg not found

		// printf("invalid");
			goto FILE_EXIT;
		case ':':
		printf("invalid");

			goto FILE_EXIT;
		default:
			break;
		}
	}
	if (my_opts->ignored && (my_opts->option != 'd')) {
		fprintf(stderr, "--ignore: missing --distance option\n");
		llist_destroy(my_opts->ignored);
		goto OPTION_EXIT;
	}

	if (!validate_file(fp)) {
		goto FILE_EXIT;
	}

	uint16_t num_entries = get_num_entries(fp);
	if (!num_entries) {
		goto FILE_EXIT;
	}

	hash_t *player_table =
	    hash_table_create((num_entries + (num_entries / 2)), hash);
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
 ARG_EXIT:
	return 1;
}
