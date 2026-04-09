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

// PUL file header structure
struct file_header {
	uint32_t magic;				        // file magic (always PULS)
	uint32_t version;			        // version currently 3
	uint32_t info_offset;			    // offset to info section header
	uint32_t cups_offset;			    // offset to cups section header
	uint32_t bmg_offset;			    // offset to bmg file
	char mod_folder_name[MOD_NAME_MAX];	// name of mod folder
};

// INFO section header
struct info_header {
	uint32_t magic;		            // section magic (always INFO)
	uint32_t version;	            // version currently 1
	uint32_t size;		            // size of info section in bytes
};

// INFO section configuration fields
struct info_fields {
	uint32_t room_key;			    // wiimmfi room key
	uint32_t prob_100cc;			// probability 100cc is chosen online
	uint32_t prob_150cc;			// probability 150cc is chosen online
	uint32_t wiimmfi_region;		// wiimmfi region
	uint32_t track_blocking;		// number of tracks to block in rooms
	uint8_t tt_trophies;			// time trial trophies enabled
	uint8_t enable_200cc;			// 200cc mode enabled
	uint8_t umts;				    // ultra-miniturbos enabled
	uint8_t feather;			    // feather enabled
	uint8_t mega_tc;			    // mega thundercloud enabled
	uint16_t n_cup_icons;			// number of cup icons
	uint8_t track_timer;			// length of track voting timer online
	uint8_t padding[40];
};

// CUPS section header
struct cups_header {
	uint32_t magic;				    // section magic (always CUPS)
	uint32_t version;			    // version currently 3
	uint32_t size;				    // size of CUPS section
	uint16_t n_ct_cups;			    // number of custom cups
	uint8_t nin_track_mode;			// nintendo track mode
	uint8_t padding;
	uint16_t n_trophies[4];			// number of tt trophies for 150cc, 200cc, 150ccF, 200ccF
	uint32_t n_variants;			// number of variant tracks
};

// Track data field. Position in track array corresponds to indices in bmg and files
struct track_entry {
	uint8_t track_slot;			    // track slot value
	uint8_t music_slot;			    // music slot value
	uint16_t n_variants;		    // number of variants for this track
	uint32_t crc32;				    // checksum for track file
};

// Overall file structure
struct pul_file {
	struct file_header f_hdr;
	struct info_header i_hdr;
	struct info_fields i_fld;
	struct cups_header c_hdr;
	struct track_entry *t_arr;
	uint16_t *alphabet_table;
};

// File IO operations
void read_file(struct pul_file *file, const uint8_t *buffer, int *offset);
void write_file(const struct pul_file file, const char *filename, const char *bmg_path, const char *txt_path);

// Printing
void print_file(const struct pul_file file);
void print_track_entries(const struct pul_file file);
void print_alphabet_table(const struct pul_file file);

// Export sub-files
void export_bmg(const struct pul_file file, const uint8_t *buffer, const char *path);
void export_txt(const struct pul_file file, const uint8_t *buffer, const size_t size, const char *path);

#endif
