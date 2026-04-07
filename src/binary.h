#ifndef _BINARY_H_
#define _BINARY_H_

#include <stddef.h>
#include <stdint.h>

uint8_t read_byte(const uint8_t *src, int *offset);
void read_bytes(const uint8_t *src, int *offset, uint8_t *dest, size_t n);
uint16_t read_be_uint16(const uint8_t *src, int *offset);
uint32_t read_be_uint32(const uint8_t *src, int *offset);

size_t buffer_from_file(uint8_t **buffer, const char *filename);
uint32_t crc32_from_file(const char *filename);

// impl in crc32.c
unsigned int crc32(const unsigned char *message, unsigned int len);

#endif
