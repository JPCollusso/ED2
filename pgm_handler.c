#include "pgm_handler.h"

static int pgm_reader_p2(FILE *in_stream, t_pgm *img);
static int pgm_reader_p5(FILE *in_stream, t_pgm *img);
static int pgm_writter_p2(FILE *out_stream, t_pgm *img);
static int pgm_writter_p5(FILE *out_stream, t_pgm *img);

t_pgm* pgm_allocator(){

    t_pgm *img;

    if((img = (t_pgm *)malloc(sizeof(t_pgm))) == NULL){
        return NULL;
    }

    img->fmt = Unspecified;
    img->width = 0;
    img->height = 0;
    img->max_gray_level = 0;
    img->pixel = NULL;

    return img;
}

void pgm_deallocator(t_pgm *img){
    
    if(img == NULL){
        return;
    }

    pgm_pixel_matrix_deallocator(img);
    free(img);
}

int pgm_pixel_matrix_allocator(t_pgm *img){

    int i;

    img->pixel = (uint16_t **)calloc((size_t)img->height, sizeof(uint16_t *));

    if(!img->pixel){
        return 1;
    }

    for(i = 0; i < img->height; i++){

        img->pixel[i] = (uint16_t *)calloc((size_t)img->width, sizeof(uint16_t));
        if(img->pixel[i] == NULL){
            return 1;
        }
    }
    return 0;
}

void pgm_pixel_matrix_deallocator(t_pgm *img){
    
    int i;

    if(img == NULL){
        return;
    }

    if(img->pixel == NULL){
        return;
    }

    for(i = 0; i < img->height; i++){
        free(img->pixel[i]);
    }

    free(img->pixel);
    img->pixel = NULL;
}

t_pgm* pgm_reader(char *fname){

    FILE *in_stream;
    t_pgm *img;
    char fmt_aux[MAX_FORMAT_NAME_SIZE];

    if(fname == NULL){
        return NULL;
    }

    if ((img = pgm_allocator()) == NULL){
        return NULL;
    }

    if((in_stream = fopen(fname, "rb")) == NULL){
        pgm_deallocator(img);
        return NULL;
    }

    if(fscanf(in_stream, "%s", fmt_aux) == EOF){
        pgm_deallocator(img);
        fclose(in_stream);
        return NULL;
    }

    if(strcmp(fmt_aux, "P2") == 0){
        img->fmt = P2;
    }
    else if(strcmp(fmt_aux, "P5") == 0){
            img->fmt = P5;
        }
        else{
            pgm_deallocator(img);
            fclose(in_stream);
            return NULL;
        }
    
    switch(img->fmt){

        case P2:
            if(pgm_reader_p2(in_stream, img)){
                pgm_deallocator(img);
                fclose(in_stream);
                return NULL;
            }
            break;

        case P5:
            if(pgm_reader_p5(in_stream, img)){
                pgm_deallocator(img);
                fclose(in_stream);
                return NULL;
            }
            break;

        default:
            pgm_deallocator(img);
            fclose(in_stream);
            return NULL;
    }

    fclose(in_stream);
    return img;
}

static int pgm_reader_p2(FILE *in_stream, t_pgm *img){

    int i, j;

    fscanf(in_stream, "%d", &(img->width));
    fscanf(in_stream, "%d", &(img->height));
    fscanf(in_stream, "%" SCNu16, &(img->max_gray_level));

    if(pgm_pixel_matrix_allocator(img) == 1){
        return 1;
    }
        
    for(i = 0; i < img->height; i++){

        for(j = 0; j < img->width; j++){

            if(fscanf(in_stream, "%" SCNu16, &(img->pixel[i][j])) != 1){
                return 1;
            }
        }
    }
    
    return 0;
}

static int pgm_reader_p5(FILE *in_stream, t_pgm *img){

    size_t n_read;
    uint8_t p8;
    uint16_t p16;
    int i, j;

    fscanf(in_stream, "%d", &(img->width));
    fscanf(in_stream, "%d", &(img->height));
    fscanf(in_stream, "%" SCNu16, &(img->max_gray_level));

    if(pgm_pixel_matrix_allocator(img) == 1){
        return 1;
    }

    if(img->max_gray_level <= 255){
        
        for(i = 0; i < img->height; i++){
            
            n_read = 0;

            for(j = 0; j < img->width; j++){

                n_read += fread(&p8, sizeof(uint8_t), 1, in_stream);
                img->pixel[i][j] = (uint16_t)p8;
            }

            if(n_read != img->width){;
                return 1;
            }
        }
    }
    else if(img->max_gray_level <= 65535){

        for(i = 0; i < img->height; i++){
            
            n_read = 0;

            for(j = 0; j < img->width; j++){

                n_read += fread(&p16, sizeof(uint16_t), 1, in_stream);
                img->pixel[i][j] = p16;
            }

            if(n_read != img->width){;
                return 1;
            }
        }
    }
    
    return 0;
}

int pgm_writter(char *fname, t_pgm *img, t_pgm_format fmt){

    FILE *out_stream;

    if(fname == NULL){
        return 1;
    }

    if(img == NULL){
        return 1;
    }

    out_stream = fopen(fname, "wb");
    if(out_stream == NULL){
        return 1;
    }

    switch(fmt){

        case Unspecified:
            if(1){
                fclose(out_stream);
                return 1;
            }
            break;

        case P2:
            if(pgm_writter_p2(out_stream, img)){
                fclose(out_stream);
                remove(fname);
                return 1;
            }
            break;

        case P5:
            if(pgm_writter_p5(out_stream, img)){
                remove(fname);
                fclose(out_stream);
                return 1;
            }
            break;

        default:
            return 1;
    }
   
    fclose(out_stream);
    return 0;
}

static int pgm_writter_p2(FILE *out_stream, t_pgm *img){

    int8_t n_written;
    int i, j;

    fprintf(out_stream, "P2\n%d %d\n%" PRIu16 "\n", img->width, img->height, img->max_gray_level);

    for(i = 0; i < img->height; i++){

        for(j = 0; j < img->width; j++){
            
            if(j == img->width - 1){
                if((n_written = fprintf(out_stream, "%" PRIu16 "\n", img->pixel[i][j])) < 0){
                    return 1;
                }
            }
            else{
                if((n_written = fprintf(out_stream, "%" PRIu16 " ", img->pixel[i][j])) < 0){
                    return 1;
                }
            }
        }
    }
    return 0;
}

static int pgm_writter_p5(FILE *out_stream, t_pgm *img){

    size_t n_written;
    uint8_t p8;
    uint16_t p16;
    int i, j;

    fprintf(out_stream, "P5\n%d %d\n" "%" PRIu16 "\n", img->width, img->height, img->max_gray_level);

    if(img->max_gray_level <= 255){
        
        for(i = 0; i < img->height; i++){
            
            n_written = 0;

            for(j = 0; j < img->width; j++){

                p8 = (uint8_t)img->pixel[i][j];
                n_written += fwrite(&p8, sizeof(uint8_t), 1, out_stream);
            }

            if(n_written != img->width){;
                return 1;
            }
        }
    }
    else if(img->max_gray_level <= 65535){

        for(i = 0; i < img->height; i++){
            
            n_written = 0;

            for(j = 0; j < img->width; j++){

                p16 = img->pixel[i][j];
                n_written += fwrite(&p16, sizeof(uint16_t), 1, out_stream);
            }

            if(n_written != img->width){;
                return 1;
            }
        }
    }

    return 0;

}
