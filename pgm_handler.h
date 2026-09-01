#ifndef PGM_HANDLER.H
#define PGM_HANDLER.H

#define MAX_FORMAT_NAME_SIZE 5

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*  PGM file type definition
    Note:   no support for comments */
typedef struct _pgm_ {
    char format[MAX_FORMAT_NAME_SIZE];
    int width;
    int height;
    uint16_t max_gray_level; // equivalent to C (or Maxval) value
    uint16_t **pixel;        
} t_pgm;

t_pgm* pgm_allocator(t_pgm *img);

void pgm_deallocator(t_pgm *img);

int pgm_pixel_matrix_allocator(t_pgm *img);

void pgm_pixel_matrix_deallocator(t_pgm *img);

t_pgm* pgm_reader(char *name, t_pgm *img);




#endif

