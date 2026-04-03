#ifndef _BINARY_H_
#define _BINARY_H_

#include <stddef.h>
#include <stdint.h>

uint8_t read_byte(uint8_t **src);
void read_string(uint8_t **src, char *dest, size_t n);
void read_bytes(uint8_t **src, uint8_t *dest, size_t n);
uint16_t read_be_uint16(uint8_t **src);
uint32_t read_be_uint32(uint8_t **src);
uint32_t crc32_from_file(const char *filename);

unsigned int crc32(const unsigned char *message, unsigned int len);

#endif
