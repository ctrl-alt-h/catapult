#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "pul.h"
#include "binary.h"

int main(int argc, char **argv) {
	FILE 	*fp;
	size_t 	f_size;
	uint8_t *buffer;

	if (argc < 2) {
		fprintf(stderr, "Usage: catapult /path/to/config.pul\n");
		return 1;
	}

	fp = fopen(argv[1], "rb");

	fseek(fp, 0, SEEK_END);	
	f_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	buffer = (uint8_t*)malloc(f_size * sizeof(uint8_t));
	if (buffer == NULL) {
		fprintf(stderr, "Error allocating buffer mem\n");
		return 1;
	}

	if (fread(buffer, 1, f_size, fp) != f_size) {
		fprintf(stderr, "Read 0 bytes. Something is wrong.\n");
		return 1;
	}

#ifdef DEBUG
	printf("file length: %zu\n", f_size);
	printf("memory successfully read into mem\n");

	struct pul_file file;
	uint8_t *current_ptr = buffer;

	read_file(&file, &current_ptr);
	free(buffer);

	print_file(file);

	if (argc == 3) {
		uint32_t test_crc = crc32_from_file(argv[2]);
		printf("crc32: %d\n", test_crc);
	}
#endif

	fclose(fp);
	return 0;
}
