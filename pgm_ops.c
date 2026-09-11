#include "pgm_ops.h"

int pgm_p2_to_p5(char *fname_p2, char *fname_p5){

    t_pgm *img;

    img = pgm_reader(fname_p2);
    if(img == NULL){
        return 1;
    }

    if(img->fmt != P2){
        pgm_deallocator(img);
        return 1;
    }

    if(pgm_writter(fname_p5, img, P5)){
        pgm_deallocator(img);
        return 1;
    }

    pgm_deallocator(img);
    return 0;
}

t_pgm* pgm_thresholding(uint16_t limiar, t_pgm *img_src){

    t_pgm *img_result;
    int i, j;

    if((img_result = pgm_allocator()) == NULL){
        return NULL;
    }

    img_result->fmt = img_src->fmt;
    img_result->width = img_src->width;
    img_result->height = img_src->height;
    img_result->max_gray_level = img_src->max_gray_level;

    if(pgm_pixel_matrix_allocator(img_result)){
        pgm_deallocator(img_result);
        return NULL;
    }

    for(i = 0; i < img_src->height; i++){

        for(j = 0; j < img_src->width; j++){

            if(img_src->pixel[i][j] < limiar){
                img_result->pixel[i][j] = (uint16_t)0;
            }
            else{
                img_result->pixel[i][j] = img_src->max_gray_level;
            }
        }
    }

    return img_result;


}