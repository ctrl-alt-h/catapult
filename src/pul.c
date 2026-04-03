#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "pul.h"
#include "binary.h"

struct pul_file* read_file(uint8_t **buffer) {
	struct pul_file *file = (struct pul_file*)malloc(sizeof(struct pul_file));

	_read_file_header(&file->f_hdr, buffer);
	_read_file_info(&file->i_hdr, &file->i_fld, buffer);
	_read_cups_header(&file->c_hdr, buffer);

	int n_cups = file->c_hdr.n_ct_cups;
	int n_tracks = 4 * (n_cups + (n_cups % 2));

	file->t_arr = (struct track_entry*)malloc(sizeof(struct track_entry) * n_tracks);
	for (int i = 0; i < n_tracks; ++i) {
		_read_track_entry(&file->t_arr[i], buffer);
	}

	return file;
}

void _read_file_header(struct file_header *hdr, uint8_t **buffer) {
	hdr->magic 		= read_be_uint32(buffer);

	if (hdr->magic != FILE_MAGIC) {
		fprintf(stderr, "Invalid data: PULS\n");
		return;
	}

	hdr->version 		= read_be_uint32(buffer);
	hdr->info_offset 	= read_be_uint32(buffer);
	hdr->cups_offset 	= read_be_uint32(buffer);
	hdr->bmg_offset 	= read_be_uint32(buffer);
	read_string(buffer, hdr->mod_folder_name, 16);
}

void _read_file_info(struct info_header *hdr, struct info_fields *fld, uint8_t **buffer) {
	hdr->magic = read_be_uint32(buffer);

	if (hdr->magic != INFO_MAGIC) {
		fprintf(stderr, "Invalid data: INFO\n");
		return;
	}

	hdr->version 	= read_be_uint32(buffer);
	hdr->size 	= read_be_uint32(buffer);

	fld->room_key 		= read_be_uint32(buffer);
	fld->prob_100cc 	= read_be_uint32(buffer);
	fld->prob_150cc 	= read_be_uint32(buffer);
	fld->wiimmfi_region 	= read_be_uint32(buffer);
	fld->track_blocking 	= read_be_uint32(buffer);

	fld->tt_trophies 	= read_byte(buffer);
	fld->enable_200cc 	= read_byte(buffer);
	fld->umts 		= read_byte(buffer);
	fld->feather 		= read_byte(buffer);
	fld->mega_tc 		= read_byte(buffer);
	fld->n_cup_icons 	= read_be_uint16(buffer);
	fld->track_timer 	= read_byte(buffer);
	read_bytes(buffer, fld->padding, 40);
}

void _read_cups_header(struct cups_header *hdr, uint8_t **buffer) {
	hdr->magic = read_be_uint32(buffer);

	if (hdr->magic != CUPS_MAGIC) {
		fprintf(stderr, "Invalid data: CUPS\n");
		return;
	}

	hdr->version 		= read_be_uint32(buffer);
	hdr->size 		= read_be_uint32(buffer);
	hdr->n_ct_cups 		= read_be_uint16(buffer);
	hdr->nin_track_mode 	= read_byte(buffer);
	hdr->padding 		= read_byte(buffer);

	// u16[4]
	hdr->n_trophies[0] = read_be_uint16(buffer);
	hdr->n_trophies[1] = read_be_uint16(buffer);
	hdr->n_trophies[2] = read_be_uint16(buffer);
	hdr->n_trophies[3] = read_be_uint16(buffer);

	hdr->n_variants = read_be_uint32(buffer);
}

void _read_track_entry(struct track_entry *track, uint8_t **buffer) {
	track->track_slot 	= read_byte(buffer);
	track->music_slot 	= read_byte(buffer);
	track->n_variants 	= read_be_uint16(buffer);
	track->crc32 		= read_be_uint32(buffer);
}

void print_file(const struct pul_file file) {
	printf("FILE HEADER\n");
	printf("------------\n");
	printf("Magic:\t\t%x\n", file.f_hdr.magic);
	printf("Version:\t%d\n", file.f_hdr.version);
	printf("Info offset:\t%d\n", file.f_hdr.info_offset);
	printf("Cups offset:\t%d\n", file.f_hdr.cups_offset);
	printf("BMG offset:\t%d\n", file.f_hdr.bmg_offset);
	printf("Mod folder:\t%.16s\n\n", file.f_hdr.mod_folder_name);

	printf("INFO HEADER\n");
	printf("------------\n");
	printf("Magic:\t\t%x\n", file.i_hdr.magic);
	printf("Version:\t%d\n", file.i_hdr.version);
	printf("Size:\t\t%d\n\n", file.i_hdr.size);

	printf("INFO FIELDS\n");
	printf("------------\n");
	printf("Room key:\t%d\n", file.i_fld.room_key);
	printf("100cc chance:\t%d\n", file.i_fld.prob_100cc);
	printf("150cc chance:\t%d\n", file.i_fld.prob_150cc);
	printf("Region:\t\t%d\n", file.i_fld.wiimmfi_region);
	printf("Blocking:\t%d\n", file.i_fld.track_blocking);
	printf("TT trophies:\t%d\n", file.i_fld.tt_trophies);
	printf("200cc:\t\t%d\n", file.i_fld.enable_200cc);
	printf("UMTs:\t\t%d\n", file.i_fld.umts);
	printf("Feather:\t%d\n", file.i_fld.feather);
	printf("Mega TC:\t%d\n", file.i_fld.mega_tc);
	printf("Cup icon count:\t%d\n", file.i_fld.n_cup_icons);
	printf("Track timer:\t%d\n\n", file.i_fld.track_timer);

	printf("CUPS HEADER\n");
	printf("------------\n");
	printf("Magic:\t\t%x\n", file.c_hdr.magic);
	printf("Version:\t%d\n", file.c_hdr.version);
	printf("Size:\t\t%d\n", file.c_hdr.size);
	printf("Cup count:\t%d\n", file.c_hdr.n_ct_cups);
	printf("Track mode:\t%d\n", file.c_hdr.nin_track_mode);
	printf("Trophy count 1:\t%d\n", file.c_hdr.n_trophies[0]);
	printf("Trophy count 2:\t%d\n", file.c_hdr.n_trophies[1]);
	printf("Trophy count 3:\t%d\n", file.c_hdr.n_trophies[2]);
	printf("Trophy count 4:\t%d\n", file.c_hdr.n_trophies[3]);
	printf("Variant count:\t%d\n\n", file.c_hdr.n_variants);

	int n_cups = file.c_hdr.n_ct_cups;
	int n_tracks = 4 * (n_cups + (n_cups % 2));

	for (int i = 0; i < n_tracks; ++i) {
		_print_track_entry(file.t_arr[i], i);
	}
}

void _print_track_entry(const struct track_entry track, const int index) {
	printf("TRACK ENTRY %d\n", index);
	printf("------------\n");
	printf("Track slot:\t%d\n", track.track_slot);
	printf("Music slot:\t%d\n", track.music_slot);
	printf("Variant count:\t%d\n", track.n_variants);
	printf("CRC32:\t\t%u\n\n", track.crc32);
}
