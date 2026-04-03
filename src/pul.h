#ifndef _PUL_H_
#define _PUL_H_

#include <stdint.h>

#define FILE_MAGIC 0x50554C53
#define INFO_MAGIC 0x494E464F
#define CUPS_MAGIC 0x43555053

struct file_header {
	uint32_t magic;
	uint32_t version;
	uint32_t info_offset;
	uint32_t cups_offset;
	uint32_t bmg_offset;
	char mod_folder_name[16];
};

struct info_header {
	uint32_t magic;
	uint32_t version;
	uint32_t size;
};

struct info_fields {
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

struct cups_header {
	uint32_t magic;
	uint32_t version;
	uint32_t size;
	uint16_t n_ct_cups;
	uint8_t nin_track_mode;
	uint8_t padding;
	uint16_t n_trophies[4];
	uint32_t n_variants;
};

struct track_entry {
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

struct pul_file* read_file(uint8_t **buffer);
void _read_file_header(struct file_header *hdr, uint8_t **buffer);
void _read_file_info(struct info_header *hdr, struct info_fields *fld, uint8_t **buffer);
void _read_cups_header(struct cups_header *hdr, uint8_t **buffer);
void _read_track_entry(struct track_entry *track, uint8_t **buffer);

void print_file(const struct pul_file file);
void _print_track_entry(const struct track_entry track, const int index);

#endif
