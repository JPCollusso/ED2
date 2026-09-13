#ifndef PGM_OPS_H
#define PGM_OPS_H

#include "pgm_handler.h"

#include <inttypes.h>

int pgm_p2_to_p5(char *fname_p2, char *fname_p5);

t_pgm* pgm_thresholding(uint16_t limiar, t_pgm *img_src);


#endif