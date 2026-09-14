#ifndef PGM_DATASET_H
#define PGM_DATASET_H

#include <stdio.h>
#include <string.h>

#include "pgm_handler.h"


/**
 * @brief PGM dataset file extension.
 * 
 * @note A PGM dataset name consists of (basename)(extension).
 */
#define PGM_DATASET_EXTENSION ".dat"


/**
 * @brief Index file prefix of a respective PGM dataset.
 * 
 * @note A index file name consists of (prefix)(basename)(extension), such that (besaname) is the same of it's
 * respective PGM dataset.
 */
#define PGM_DATASET_INDEX_FILE_PREFIX "idx_"


/**
 * @brief Maximum number of characters that will be added on the basename of a PGM dataset as it's extension.
 * 
 * @note Not includes the null terminator ('\0'). 
 */
#define PGM_DATASET_EXTENSION_SIZE (sizeof(PGM_DATASET_EXTENSION) - 1)


/**
 * @brief Maximum number of characters that will be added on the basename of a index file of a respective PGM 
 * dataset as it's prefix.
 * 
 * @note Not includes the null terminator ('\0'). 
 */
#define PGM_DATASET_INDEX_FILE_PREFIX_SIZE (sizeof(PGM_DATASET_INDEX_FILE_PREFIX) - 1)


/**
 * @brief Maximum number of characters to a PGM dataset basename.
 * 
 * @note Not includes the null terminator ('\0').
 */
#define MAX_PGM_DATASET_BASENAME_SIZE (30 - PGM_DATASET_EXTENSION_SIZE - PGM_DATASET_INDEX_FILE_PREFIX_SIZE)


/**
 * @brief Stores a record of the PGM dataset index file.
 * 
 * @note The field position in the declaration order is equivalent to it's importance as a key.
 * 
 * Thus, primary key: img_offset, secundary key: img_basename, tertiary key: is_excluded, fourth key: img_size.
 */
typedef struct pgm_dataset_fidx_record_s{
    long int img_offset;
    char img_basename[27];
    char is_excluded;
    long int img_size;
} t_pgm_dataset_fidx_record;


/**
 * @brief Validates a PGM dataset basename.
 * 
 * @param basename The basename that will be verified.
 * 
 * @return 0 if basename is valid, 1 otherwise.
 * 
 * @note A valid PGM dataset basename is the one that don't have any path, invalid prefix, extension, invalids 
 * characters and is not hidden.
 * 
 * An invalid prefix is: PGM_DATASET_EXTENSION.
 * 
 * Invalid characters are: '.'  '\'  '|'
 */
int pgm_dataset_basename_validator(const char *basename);


/**
 * @brief Creates a PGM dataset file and it's respective index file.
 * 
 * @param basename The basename of the files that will be created. 
 * 
 * @return 0 if both successfully created, 1 otherwise.
 * 
 * @note The basename will be verified.
 * 
 * A PGM dataset name consists of: (basename)(extension), such that extension is defined as PGM_DATASET_EXTENSION.
 * 
 * Therefore, it's respectve index file name consists of: (prefix)(basename)(sufix), such that the prefix is defined 
 * as PGM_DATASET_INDEX_FILE_PREFIX and the extension as PGM_DATASET_EXTENSION.
 */
int pgm_dataset_creator(const char *basename);


/**
 * @brief Reads a PGM image from a PGM dataset.
 * 
 * @param basename_dataset The basename of the PGM dataset.
 * 
 * @param basename_img The search criterion for the PGM image in the PGM dataset (corresponds to the secondary 
 * key in index file).
 * 
 * @return Pointer to the t_pgm structure that stores the PGM image read, or NULL in failure.
 */
t_pgm* pgm_dataset_reader(const char *basename_dataset, const char *basename_img);


/**
 * @brief Writes a PGM image in a PGM dataset.
 * 
 * @param basename_dataset The basename of the PGM dataset.
 * 
 * @param basename_img The secondary key that is writed in the index file for the PGM image.
 * 
 * @param img The PGM image that will be writted.
 * 
 * @return 0, 1 in failure.
 */
int pgm_dataset_writer(const char *basename_dataset, const char *basename_img, const t_pgm *img);


#endif /* PGM_DATASET_H */
