#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pul.h"
#include "binary.h"

#define PUL_TRUE 1
#define PUL_FALSE 0

struct app_state {
	int show_tracks; // boolean
	int show_alphabet_table; // boolean
};

//-----------------------//
//----- CAT COMMAND -----//
//-----------------------//

void command_cat(struct app_state state, const char *filename) {
	uint8_t *buffer = NULL;
	size_t f_size = buffer_from_file(&buffer, filename);
	if (f_size <= 0) {
		return;
	}

	int offset = 0;
	struct pul_file pf;
	read_file(&pf, buffer, &offset);
	free(buffer);

	print_file(pf);

	if (state.show_tracks) {
		print_track_entries(pf);
	}

	if (state.show_alphabet_table) {
		print_alphabet_table(pf);
	}
}

//----------------------------------------------------//

void handle_cat(int argc, char **argv, struct app_state state) {
	if (argc < 3) {
		fprintf(stderr, "Usage: catapult CAT /path/to/Config.pul [options]...\n");
		return;
	}

	char *file = argv[2];
	optind = 3;

	struct option long_options[] = {
		{"tracks",         no_argument, 0, 't'},
		{"alphabet-table", no_argument, 0, 'a'},
		{0,                0,           0,  0 },
	};

	int opt;
	while ((opt = getopt_long(argc, argv, "", long_options, NULL)) != -1) {
		switch (opt) {
			case 't':
				state.show_tracks = PUL_TRUE;
				break;
			case 'a':
				state.show_alphabet_table = PUL_TRUE;
				break;
			default:
				abort();
		}
	}

	command_cat(state, file);
}

//-----------------------//
//----- Entry Point -----//
//-----------------------//

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: catapult <command> [options]...\n");
		return 1;
	}

	char *command = argv[1];
	struct app_state state = {
		.show_tracks 		= PUL_FALSE,
		.show_alphabet_table 	= PUL_FALSE,
	};

	for (int i = 0; command[i]; ++i) {
		command[i] = toupper(command[i]);
	}

	if (strncmp(command, "CAT", 3) == 0) {
		handle_cat(argc, argv, state);
	}

	return 0;
}
