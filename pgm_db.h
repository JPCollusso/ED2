#ifndef PGM_DB_H
#define PGM_DB_H

#include <stdint.h>
#include "pgm_handler.h"

// Adiciona uma imagem PGM ao arquivo de base de dados (images.db).
// Regista também o par (nome lógico -> offset) em images.key.
int db_add_image(const char *db_fname, const char *key_fname, const char *pgm_fname, const char *image_name);

// Procura no arquivo de chaves o offset associado ao nome de uma imagem (ex: barbara.pgm).
// Retorna 0 em sucesso e preenche offset_out.
int db_find_offset_by_name(const char *key_fname, const char *image_name, uint64_t *offset_out);

// Lê a imagem armazenada em images.db no offset indicado e devolve um ponteiro para t_pgm.
t_pgm* db_read_image_by_offset(const char *db_fname, uint64_t offset);

// Lista todas as entradas presentes no arquivo de chaves e exibe na tela.
int db_list_images(const char *key_fname);

#endif
