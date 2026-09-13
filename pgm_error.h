#ifndef PGM_ERROR_H
#define PGM_ERROR_H

typedef enum _error_group_ {
    NONE,
    MEMORY,
    READING_WRITING,
    OPERATION,
    UNKNOWN
} t_error_group;


typedef enum _error_memory{
    BAD_ALLOCATION,
} t_error_memory;

typedef enum _error_rw_{

} t_error_rw;

typedef enum _error_operation_{

} t_error_operation;

typedef union _error_occurred_{
    t_error_memory memory;
    t_error_rw reading_writing;
    t_error_operation operation;
} t_error_occurred;


typedef struct _pgm_error_ {

    t_error_group group;
    t_error_occurred occ;

} t_pgm_error;

#endif