#ifndef PGM_OPS_H
#define PGM_OPS_H

#include "pgm_handler.h"

#include <inttypes.h>

/**
 * @brief Reads a PGM image from a file and if their format is P2 rewritte the content in another with P5 format.
 * 
 * @param fname_p2 Name of the file that the PGM image will be readed. 
 * 
 * @param fname_p5 Name of the file that the PGM image will be writted in P5 format.
 * 
 * @return 0, or 1 in failure.
 * 
 * @note NULL pointers as argument will be treated and cause failure.
 * 
 * Failure will be accurate if the read PGM image isn't P2.
 */
int pgm_p2_to_p5(char *fname_p2, char *fname_p5);


/**
 * @brief Thresholds an PGM image.
 * 
 * @param thresh_value Threshold value. It's an uint16_t because max_gray_level it's also too.
 * 
 * @param img_src PGM image that will be thresholded.
 * 
 * @return Pointer to the t_pgm structure that stores the thresholding PGM image, or NULL in failure.
 * 
 * @warning The return must be stored after the function call to future deallocation and should have be done in a NULL t_pgm pointer.
 */
t_pgm* pgm_thresholding(uint16_t thresh_value, t_pgm *img_src);


#endif