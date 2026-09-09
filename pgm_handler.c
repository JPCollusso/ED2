#include "pgm_handler.h"

t_pgm* pgm_allocator(){

    t_pgm *img;

    if((img = (t_pgm *)malloc(sizeof(t_pgm))) == NULL){
        return NULL;
    }

    img->format;
    img->width = 0;
    img->height = 0;
    img->max_gray_level = 0;
    img->pixel = NULL;

    return img;
}

void pgm_deallocator(t_pgm *img){
    
    pgm_pixelMatrix_deallocator(img);
    free(img);
    img = NULL;
}

int pgm_pixe_matrix_allocator(t_pgm *img){

    int i;

    img->pixel = (uint16_t *)malloc(img->height * sizeof(uint16_t *));

    if(!img->pixel){
        return 1;
    }

    for(i = 0; i < img->height; i++){

        img->pixel[i] = (uint16_t *)malloc(img->width * sizeof(uint16_t));
        if(img->pixel[i] == NULL){
            return 1;
        }
    }
    return 0;
}

void pgm_pixel_matrix_deallocator(t_pgm *img){
    
    int i;

    if(img->pixel == NULL){
        return;
    }

    for(i = 0; i < img->height; i++){
        free(((*img).height) + i);
    }

    free(img->pixel);
    img->pixel = NULL;
}

t_pgm* pgm_reader(char *name){

    FILE *in_stream;
    t_pgm *img;
    int i, j;

    if ((img = pgm_allocator()) == NULL){
        return NULL;
    }

    if((in_stream = fopen(name, "rb")) == NULL){
        pgm_deallocator(img);
        return NULL;
    }

    if(fscanf(in_stream, "%s", img->format) == EOF){
        pgm_deallocator(img);
        return NULL;
    }

    if(strcmp(img->format, "P2") == 0){

        fscanf(in_stream, "%d", img->width);
        fscanf(in_stream, "%d", img->height);
        fscanf(in_stream, "%d", img->max_gray_level);

        if(pgm_pixel_matrix_allocator(img) == 1){
            pgmDeallocator(img);
            return NULL;
        }
        
        for(i = 0; i < img->height; i++){

            for(j = 0; i < img->width; j++){

                if(fscanf(in_stream, "%d", img->pixel[i][j]) == EOF){
                    pgm_deallocator(img);
                    return NULL;
                }
            }
        }

    }
    else if((strcmp(img->format, "P5") == 0)){
            return NULL;
        }
        else{
            return NULL;
        }

    fclose(in_stream);
     return img;
}

int pgm_writter(t_pgm *img){

}

int pgm_writter_p5(t_pgm *img){

    FILE *out_stream;

    if(img == NULL){
        return 1;
    }

    out_stream = fopen("nome", );
}
