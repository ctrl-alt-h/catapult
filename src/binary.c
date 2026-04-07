#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "binary.h"

uint8_t read_byte(const uint8_t *src, int *offset) {
	return src[(*offset)++];
}

void read_bytes(const uint8_t *src, int *offset, uint8_t *dest, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		dest[i] = read_byte(src, offset);
	}
}

uint16_t read_be_uint16(const uint8_t *src, int *offset) {
	return read_byte(src, offset) << 8 | read_byte(src, offset);
}

uint32_t read_be_uint32(const uint8_t *src, int *offset) {
	return read_byte(src, offset) << 24 |
	       read_byte(src, offset) << 16 |
	       read_byte(src, offset) << 8 |
	       read_byte(src, offset);
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
	if (fread(buffer, 1, size, file) != size) {
		fprintf(stderr, "Failed to read file in the crc32 function\n");
		return 0;
	}

	result = crc32(buffer, (unsigned int)size);
	
	free(buffer);
	fclose(file);
	return result;
}

size_t buffer_from_file(uint8_t **buffer, const char *filename) {
	FILE *fp = NULL;
	size_t f_size = 0;

	fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	f_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	*buffer = (uint8_t*)malloc(sizeof(uint8_t) * f_size);
	if (*buffer == NULL) {
		fprintf(stderr, "Error allocating buffer mem\n");
		fclose(fp);
		return 0;
	}

	if (fread(*buffer, 1, f_size, fp) != f_size) {
		fprintf(stderr, "Read inconsistent bytes. Something is wrong.\n");
		fclose(fp);
		return 0;
	}

	fclose(fp);
	return f_size;
}
