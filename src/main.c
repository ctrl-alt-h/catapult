#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "pul.h"
#include "binary.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: catapult /path/to/config.pul\n");
		return 1;
	}

#ifdef DEBUG
	uint8_t *buffer = NULL;
	size_t f_size = buffer_from_file(&buffer, argv[1]);

	if (f_size == 0) { return 1; }

	struct pul_file file;
	int 	offset = 0;

	read_file(&file, buffer, &offset);
	export_bmg(file, buffer, "./out.bmg");
	export_txt(file, buffer, f_size, "./out.txt");

	free(buffer);

	print_file(file);
#endif

	return 0;
}
