#ifndef _PUL_H_
#define _PUL_H_

#include <stdint.h>

#define FILE_MAGIC 0x50554C53
#define INFO_MAGIC 0x494E464F
#define CUPS_MAGIC 0x43555053
#define BMG_MAGIC_1 0x4D455347
#define BMG_MAGIC_2 0x626D6731
#define TEXT_MAGIC 0x46494C45

#define MOD_NAME_MAX 16

struct __attribute__((__packed__)) file_header {
	uint32_t magic;
	uint32_t version;
	uint32_t info_offset;
	uint32_t cups_offset;
	uint32_t bmg_offset;
	char mod_folder_name[MOD_NAME_MAX];
};

struct __attribute__((__packed__)) info_header {
	uint32_t magic;
	uint32_t version;
	uint32_t size;
};

struct __attribute__((__packed__)) info_fields {
	uint32_t room_key;
	uint32_t prob_100cc;
	uint32_t prob_150cc;
	uint32_t wiimmfi_region;
	uint32_t track_blocking;
	uint8_t tt_trophies;
	uint8_t enable_200cc;
	uint8_t umts;
	uint8_t feather;
	uint8_t mega_tc;
	uint16_t n_cup_icons;
	uint8_t track_timer;
	uint8_t padding[40];
};

struct __attribute__((__packed__)) cups_header {
	uint32_t magic;
	uint32_t version;
	uint32_t size;
	uint16_t n_ct_cups;
	uint8_t nin_track_mode;
	uint8_t padding;
	uint16_t n_trophies[4];
	uint32_t n_variants;
};

struct __attribute__((__packed__)) track_entry {
	uint8_t track_slot;
	uint8_t music_slot;
	uint16_t n_variants;
	uint32_t crc32;
};

struct pul_file {
	struct file_header f_hdr;
	struct info_header i_hdr;
	struct info_fields i_fld;
	struct cups_header c_hdr;
	struct track_entry *t_arr;
	uint16_t *alphabet_table;
};

void read_file(struct pul_file *file, const uint8_t *buffer, int *offset);
void write_file(const struct pul_file file, const char *filename, const char *bmg_path, const char *txt_path);

void print_file(const struct pul_file file);
void print_track_entries(const struct pul_file file);
void print_alphabet_table(const struct pul_file file);

void export_bmg(const struct pul_file file, const uint8_t *buffer, const char *path);
void export_txt(const struct pul_file file, const uint8_t *buffer, const size_t size, const char *path);

#endif
