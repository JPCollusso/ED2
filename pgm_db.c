#include "pgm_db.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>


typedef struct db_img_header_s{
    uint8_t fmt;
    uint32_t width;
    uint32_t height;
    uint16_t max_gray;
    uint64_t pixel_bytes;
} db_img_header;

int db_add_image(const char *db_fname, const char *key_fname, const char *pgm_fname, const char *image_name){

    FILE *dbf = NULL;
    FILE *keyf = NULL;
    t_pgm *img = NULL;
    db_img_header hdr;
    uint64_t offset;
    int i, j;

    if(!db_fname || !key_fname || !pgm_fname || !image_name) return 1;

    img = pgm_reader((char *)pgm_fname);
    if(img == NULL) return 1;

    
    dbf = fopen(db_fname, "ab+");
    if(dbf == NULL){
        pgm_deallocator(img);
        return 1;
    }

    if(fseek(dbf, 0, SEEK_END) != 0){
        fclose(dbf);
        pgm_deallocator(img);
        return 1;
    }

    offset = (uint64_t)ftell(dbf);

    hdr.fmt = (uint8_t)(img->fmt == P2 ? 2 : 5);
    hdr.width = (uint32_t)img->width;
    hdr.height = (uint32_t)img->height;
    hdr.max_gray = img->max_gray_level;

    if(img->max_gray_level <= 255){
        hdr.pixel_bytes = (uint64_t)img->width * img->height * sizeof(uint8_t);
    } else {
        hdr.pixel_bytes = (uint64_t)img->width * img->height * sizeof(uint16_t);
    }

    if(fwrite(&hdr, sizeof(db_img_header), 1, dbf) != 1){
        fclose(dbf);
        pgm_deallocator(img);
        return 1;
    }

    
    if(img->max_gray_level <= 255){
        for(i = 0; i < img->height; i++){
            for(j = 0; j < img->width; j++){
                uint8_t p = (uint8_t)img->pixel[i][j];
                if(fwrite(&p, sizeof(uint8_t), 1, dbf) != 1){
                    fclose(dbf);
                    pgm_deallocator(img);
                    return 1;
                }
            }
        }
    } else {
        for(i = 0; i < img->height; i++){
            for(j = 0; j < img->width; j++){
                uint16_t p = img->pixel[i][j];
                if(fwrite(&p, sizeof(uint16_t), 1, dbf) != 1){
                    fclose(dbf);
                    pgm_deallocator(img);
                    return 1;
                }
            }
        }
    }

    keyf = fopen(key_fname, "a");
    if(keyf == NULL){
        fclose(dbf);
        pgm_deallocator(img);
        return 1;
    }

    fprintf(keyf, "%s %" PRIu64 "\n", image_name, offset);
    fclose(keyf);
    fclose(dbf);
    pgm_deallocator(img);
    return 0;
}

int db_find_offset_by_name(const char *key_fname, const char *image_name, uint64_t *offset_out){

    FILE *keyf;
    char name[256];
    uint64_t offset;

    if(!key_fname || !image_name || !offset_out) return 1;

    keyf = fopen(key_fname, "r");
    if(keyf == NULL) return 1;

    while(fscanf(keyf, "%255s %" SCNu64, name, &offset) == 2){
        if(strcmp(name, image_name) == 0){
            *offset_out = offset;
            fclose(keyf);
            return 0;
        }
    }

    fclose(keyf);
    return 1;
}

t_pgm* db_read_image_by_offset(const char *db_fname, uint64_t offset){

    FILE *dbf;
    db_img_header hdr;
    t_pgm *img = NULL;
    int i, j;

    if(!db_fname) return NULL;

    dbf = fopen(db_fname, "rb");
    if(dbf == NULL) return NULL;

    if(fseek(dbf, (long)offset, SEEK_SET) != 0){
        fclose(dbf);
        return NULL;
    }

    if(fread(&hdr, sizeof(db_img_header), 1, dbf) != 1){
        fclose(dbf);
        return NULL;
    }

    if((img = pgm_allocator()) == NULL){
        fclose(dbf);
        return NULL;
    }

    img->fmt = (hdr.fmt == 2) ? P2 : P5;
    img->width = (int)hdr.width;
    img->height = (int)hdr.height;
    img->max_gray_level = hdr.max_gray;

    if(pgm_pixel_matrix_allocator(img)){
        pgm_deallocator(img);
        fclose(dbf);
        return NULL;
    }

    if(img->max_gray_level <= 255){
        for(i = 0; i < img->height; i++){
            for(j = 0; j < img->width; j++){
                uint8_t p8;
                if(fread(&p8, sizeof(uint8_t), 1, dbf) != 1){
                    pgm_deallocator(img);
                    fclose(dbf);
                    return NULL;
                }
                img->pixel[i][j] = (uint16_t)p8;
            }
        }
    } else {
        for(i = 0; i < img->height; i++){
            for(j = 0; j < img->width; j++){
                uint16_t p16;
                if(fread(&p16, sizeof(uint16_t), 1, dbf) != 1){
                    pgm_deallocator(img);
                    fclose(dbf);
                    return NULL;
                }
                img->pixel[i][j] = p16;
            }
        }
    }

    fclose(dbf);
    return img;
}

int db_list_images(const char *key_fname){

    FILE *keyf;
    char name[256];
    uint64_t offset;

    if(!key_fname) return 1;

    keyf = fopen(key_fname, "r");
    if(keyf == NULL) return 1;

    printf("Stored images:\n");
    while(fscanf(keyf, "%255s %" SCNu64, name, &offset) == 2){
        printf("- %s (offset=%" PRIu64 ")\n", name, offset);
    }

    fclose(keyf);
    return 0;
}
