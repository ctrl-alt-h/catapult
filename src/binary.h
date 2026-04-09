#ifndef _BINARY_H_
#define _BINARY_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// binary reading
uint8_t  read_byte(const uint8_t *src, int *offset);				            // read byte from buffer
void 	 read_bytes(const uint8_t *src, int *offset, uint8_t *dest, size_t n);	// read array of bytes into dest
uint16_t read_be_uint16(const uint8_t *src, int *offset);			            // read uint16_t from buffer
uint32_t read_be_uint32(const uint8_t *src, int *offset);			            // read uint32_t from buffer

// binary writing
void write_byte(FILE *stream, uint8_t x);                           // write one byte to stream
void write_be_uint16(FILE *stream, uint16_t x);                     // write uint16_t to stream
void write_be_uint32(FILE *stream, uint32_t x);                     // write uint32_t to stream

// misc binary operations
size_t buffer_from_file(uint8_t **buffer, const char *filename);	// write file bytes to buffer
uint32_t crc32_from_file(const char *filename);					    // calculate checksum from file

// impl in crc32.c
unsigned int crc32(const unsigned char *message, unsigned int len);	// crc32 calculation

#endif
