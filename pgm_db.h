#ifndef PGM_DB_H
#define PGM_DB_H

#include <stdint.h>
#include "pgm_handler.h"

int db_add_image(const char *db_fname, const char *key_fname, const char *pgm_fname, const char *image_name);
int db_find_offset_by_name(const char *key_fname, const char *image_name, uint64_t *offset_out);
t_pgm* db_read_image_by_offset(const char *db_fname, uint64_t offset);
int db_list_images(const char *key_fname);

#endif
