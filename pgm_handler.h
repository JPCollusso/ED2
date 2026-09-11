#ifndef PGM_HANDLER_H
#define PGM_HANDLER_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FORMAT_NAME_SIZE 5

/**
 * @brief Supported PGM formats.
 * 
 * @note The 'Unspecified' field is used as default format in pgm_allocator function.
*/
typedef enum pgm_format_s { 
    Unspecified,    
    P2,            
    P5              
} t_pgm_format;


/** 
 * @brief Stores a PGM image in primary memory. 
 * 
 * @warning t_pgm variables must be pointers and should be initialized to NULL.
 * 
 * @note Not directly modify a t_pgm variable to evite inconsistencies.
 * 
 * Comments aren't supported.
 * 
 * The pixel matrix is indexed like pixel[height][widght].
 */
typedef struct pgm_s {
    t_pgm_format fmt;           
    int width;                  
    int height;                 
    uint16_t max_gray_level;    
    uint16_t **pixel;           
} t_pgm;


/**
 * @brief Dynamically allocates a contiguous memory block for t_pgm structure.
 * 
 * @return A t_pgm pointer to the allocated memory block, or NULL in failure. 
 * 
 * @warning The return must be stored after the function call to future deallocation and should have be done in a NULL t_pgm pointer.
 * 
 * @note All fields will be correctly initialized with default values, such that not represent any pgm image. 
 * 
 * t_pgm structure must be deallocated later and should be done with pgm_deallocator function. 
 */
t_pgm* pgm_allocator();


/**
 * @brief Frees the memory allocated to a t_pgm structure and its internal pixel matrix.
 * 
 * @param img Pointer to the t_pgm structure that will be deallocated. No action is performed for NULL values.
 * 
 * @warning Both the img pointer and its internal pixel matrix pointer must point to valid addresses or NULL. 
 * Otherwise, undefined behavior will be caused. 
 * 
 * @note After the function call, is recommended to set NULL to the parameter pointer. 
 */
void pgm_deallocator(t_pgm *img);


/**
 * @brief Dynamically allocates a pixel matrix for t_pgm structure.
 * 
 * @param img Pointer to the t_pgm structure that its internal pixel matrix will be dynamically allocated.
 * NULL value will be treated and cause failure.
 * 
 * @return 0, or 1 in failure.
 * 
 * @warning The img pointer must point to valid adresse and its internal pixel matrix must be NULL. 
 * Otherwise, undefined behavior will be caused. 
 * 
 * @note Pixel matrix must be deallocated later and should be done with pgm_deallocator or pgm_pixel_matrix_deallocator functions.
 */
int pgm_pixel_matrix_allocator(t_pgm *img);


/** 
 * @brief Frees the memory allocated to a pixel matrix in a t_pgm structure.
 * 
 * @param img Pointer to the t_pgm structure that stores the pixel matrix that will be deallocated. 
 * No action is performed for NULL values, both for the t_pgm structure and pixel matrix.
 * 
 * @warning Both the img pointer and its internal pixel matrix pointer must point to valid addresses or NULL. 
 * Otherwise, undefined behavior will be caused. 
 * 
 * @note After the function call, is recommended to set NULL to the parameter pointer. 
 */
void pgm_pixel_matrix_deallocator(t_pgm *img);


/** 
 * @brief Reads a PGM image from a file and stored it in a t_pgm structure.
 * 
 * @param fname Pointer to an array that stores the name of the file to be read from.
 * NULL value will be treated and cause failure.
 * 
 * @return Pointer to the t_pgm structure that stores the reading, or NULL in failure.
 * 
 * @warning fname must point to a valid adresses or NULL, otherwise, undefined behaviour will be caused.
 *  
 * The return must be stored after the function call to future deallocation and should be done in a NULL t_pgm pointer.
*/
t_pgm* pgm_reader(char *fname);


/**
 * @brief Writtes a t_pgm structure content into a PGM file, based on a specified format.
 * 
 * @param fname Pointer to an array that stores the name that will be given to the PGM file that will be where the writtens occours. 
 * NULL value will be treated and cause failure.
 * 
 * @param img Pointer to a t_pgm structure that will be writted.
 * NULL values will be treated and cause failure.
 * 
 * @param fmt t_pgm_format value that specifies the format that the t_pgm structure will be writted. 
 * 'Unspecified' value will be treated and cause failure. 
 * 
 * @return 0, or 1 in failure.
 * 
 * @warning img pointer and its internal pixel matrix pointer, in adittion to fname, must point to valid addresses or NULL. 
 * Otherwise, undefined behavior will be caused. 
 * 
 * @note Previous file called 'fname' will might be overwritten, losing all their content.
 */
int pgm_writter(char *fname, t_pgm *img, t_pgm_format fmt);


#endif /* PGM_HANDLER_H */

