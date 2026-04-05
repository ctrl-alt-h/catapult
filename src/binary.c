#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "binary.h"

uint8_t read_byte(uint8_t **src) {
	return *((*src)++);
}

void read_bytes(uint8_t **src, uint8_t *dest, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		dest[i] = read_byte(src);
	}
}

uint16_t read_be_uint16(uint8_t **src) {
	return read_byte(src) << 8 | read_byte(src);
}

uint32_t read_be_uint32(uint8_t **src) {
	return read_byte(src) << 24 |
	       read_byte(src) << 16 |
	       read_byte(src) << 8 |
	       read_byte(src);
}

uint32_t crc32_from_file(const char *filename) {
	FILE *file;
	size_t size;
	unsigned char *buffer;
	unsigned int result;

	file = fopen(filename, "rb");
	if (file == NULL) {
		fprintf(stderr, "Failed to open file in the crc32 function\n");
		return 0;
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	buffer = (unsigned char*)malloc(sizeof(unsigned char) * size);
	if (fread(buffer, 1, size, file) <= 0 ) {
		fprintf(stderr, "Failed to read file in the crc32 function\n");
		return 0;
	}

	result = crc32(buffer, (unsigned int)size);
	
	free(buffer);
	fclose(file);
	return result;
}
