#include <stdio.h>
#include <stdlib.h>
	typedef struct l_opts {
		char option;
		char *search_param1;
		char *search_param2;
		FILE *fp;
	} l_opts;

static void print_player(char *player);

void print_helper (l_opts *my_opts) {
        switch (my_opts->option) {
                case 'p':
                        print_player(my_opts->search_param1);
        }
}

static void print_player(char *player) {
        
}