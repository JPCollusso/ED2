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
 * @brief Maximum number of characters that will be added on the basename of a index file of a respective PGM dataset as it's prefix.
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


int pgm_dataset_basename_validator(const char *basename);


int pgm_dataset_creator(const char *basename);


int pgm_dataset_writter(const char *basename, t_pgm *img);


#endif /* PGM_DATASET_H */
