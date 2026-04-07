#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "pul.h"
#include "binary.h"

static void _read_file_header(struct file_header *hdr, const uint8_t *buffer, int *offset) {
	hdr->magic 		= read_be_uint32(buffer, offset);

	if (hdr->magic != FILE_MAGIC) {
		fprintf(stderr, "Invalid data: PULS\n");
		return;
	}

	hdr->version 		= read_be_uint32(buffer, offset);
	hdr->info_offset 	= read_be_uint32(buffer, offset);
	hdr->cups_offset 	= read_be_uint32(buffer, offset);
	hdr->bmg_offset 	= read_be_uint32(buffer, offset);
	read_bytes(buffer, offset, (uint8_t*)hdr->mod_folder_name, MOD_NAME_MAX);
}

static void _read_file_info(struct info_header *hdr, struct info_fields *fld, const uint8_t *buffer, int *offset) {
	hdr->magic = read_be_uint32(buffer, offset);

	if (hdr->magic != INFO_MAGIC) {
		fprintf(stderr, "Invalid data: INFO\n");
		return;
	}

	hdr->version 	= read_be_uint32(buffer, offset);
	hdr->size 	= read_be_uint32(buffer, offset);

	fld->room_key 		= read_be_uint32(buffer, offset);
	fld->prob_100cc 	= read_be_uint32(buffer, offset);
	fld->prob_150cc 	= read_be_uint32(buffer, offset);
	fld->wiimmfi_region 	= read_be_uint32(buffer, offset);
	fld->track_blocking 	= read_be_uint32(buffer, offset);

	fld->tt_trophies 	= read_byte(buffer, offset);
	fld->enable_200cc 	= read_byte(buffer, offset);
	fld->umts 		= read_byte(buffer, offset);
	fld->feather 		= read_byte(buffer, offset);
	fld->mega_tc 		= read_byte(buffer, offset);
	fld->n_cup_icons 	= read_be_uint16(buffer, offset);
	fld->track_timer 	= read_byte(buffer, offset);
	read_bytes(buffer, offset, fld->padding, 40);
}

static void _read_cups_header(struct cups_header *hdr, const uint8_t *buffer, int *offset) {
	hdr->magic = read_be_uint32(buffer, offset);

	if (hdr->magic != CUPS_MAGIC) {
		fprintf(stderr, "Invalid data: CUPS\n");
		return;
	}

	hdr->version 		= read_be_uint32(buffer, offset);
	hdr->size 		= read_be_uint32(buffer, offset);
	hdr->n_ct_cups 		= read_be_uint16(buffer, offset);
	hdr->nin_track_mode 	= read_byte(buffer, offset);
	hdr->padding 		= read_byte(buffer, offset);

	// u16[4]
	hdr->n_trophies[0] = read_be_uint16(buffer, offset);
	hdr->n_trophies[1] = read_be_uint16(buffer, offset);
	hdr->n_trophies[2] = read_be_uint16(buffer, offset);
	hdr->n_trophies[3] = read_be_uint16(buffer, offset);

	hdr->n_variants = read_be_uint32(buffer, offset);
}

static void _read_track_entry(struct track_entry *track, const uint8_t *buffer, int *offset) {
	track->track_slot 	= read_byte(buffer, offset);
	track->music_slot 	= read_byte(buffer, offset);
	track->n_variants 	= read_be_uint16(buffer, offset);
	track->crc32 		= read_be_uint32(buffer, offset);
}

void read_file(struct pul_file *file, const uint8_t *buffer, int *offset) {
	_read_file_header(&file->f_hdr, buffer, offset);
	_read_file_info(&file->i_hdr, &file->i_fld, buffer, offset);
	_read_cups_header(&file->c_hdr, buffer, offset);

	int n_cups = file->c_hdr.n_ct_cups;
	int n_tracks = 4 * (n_cups + (n_cups % 2));

	file->t_arr = (struct track_entry*)malloc(sizeof(struct track_entry) * n_tracks);
	for (int i = 0; i < n_tracks; ++i) {
		_read_track_entry(&file->t_arr[i], buffer, offset);
	}

	file->alphabet_table = (uint16_t*)malloc(sizeof(uint16_t) * n_tracks);
	for (int i = 0; i < n_tracks; ++i) {
		file->alphabet_table[i] = read_be_uint16(buffer, offset);
	}
}

static void _print_track_entry(const struct track_entry track, const int index) {
	printf("TRACK ENTRY %d\n", index);
	printf("------------\n");
	printf("Track slot:\t%d\n", track.track_slot);
	printf("Music slot:\t%d\n", track.music_slot);
	printf("Variant count:\t%d\n", track.n_variants);
	printf("CRC32:\t\t%u\n\n", track.crc32);
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

	for (int i = 0; i < n_tracks; ++i) {
		printf("%d\n", file.alphabet_table[i]);
	}
}

void export_bmg(const struct pul_file file, const uint8_t *buffer) {
	int start = file.f_hdr.bmg_offset;
	int offset = start;
	
	if (read_be_uint32(buffer, &offset) != BMG_MAGIC_1) {
		fprintf(stderr, "BMG magic invalid\n");
		return;
	}

	if (read_be_uint32(buffer, &offset) != BMG_MAGIC_2) {
		fprintf(stderr, "BMG magic invalid\n");
		return;
	}

	size_t f_size = read_be_uint32(buffer, &offset);	

	FILE *fp = fopen("./config.bmg", "wb");
	fwrite(&buffer[start], 1, f_size, fp);
	fclose(fp);
}
