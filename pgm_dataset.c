#include "pgm_dataset.h"


/* Components functions of pgm_dataset_creator(), each dedicated for create a specific file model (PGM dataset 
and it's respective index file). */
static int pgm_dataset_file_creator(const char *basename, char *fname_dataset);
static int pgm_dataset_index_file_creator(const char *basename, char *fname_index);


/* Component function of pgm_dataset_reader(), dedicated only to read a PGM image content from the PGM dataset. */
static int pgm_dataset_image_reader(FILE *in_stream, t_pgm *img);


/* Component function of pgm_dataset_writer(), dedicated only to write a PGM image content in the PGM dataset. */
static int pgm_dataset_file_writer(FILE *out_stream, const t_pgm *img);


int pgm_dataset_basename_validator(const char *basename){

    int basename_len, prefix_len;
    int i;

    if(basename == NULL){
        return 1;
    }

    if((basename_len = strlen(basename)) > MAX_PGM_DATASET_BASENAME_SIZE){
        return 1;
    }

    /* checks if the passed basename have the prefix "PGM_DATASET_INDEX_FILE_PREFIX" */
    prefix_len = PGM_DATASET_INDEX_FILE_PREFIX_SIZE;

    if(basename_len >= prefix_len){
        if(strncmp(basename, PGM_DATASET_INDEX_FILE_PREFIX, prefix_len) == 0){
             return 1;
        }
    }

    /* checks if the passed basename have invalids characters */
    for(i = 0; i < basename_len; i++){

        /* '.' indicates the probability of a extension in basename or that is hidden */
        if(basename[i] == '.'){
            return 1;
        }

        /* '/' indicates the probability of a path in basename */
        if(basename[i] == '/'){
            return 1;
        }

        /* '|' is used as delimitator in index file */
        if(basename[i] == '|'){
            return 1;
        }
    }

    return 0;
}


int pgm_dataset_creator(const char *basename){

    char fname_dataset[MAX_PGM_DATASET_BASENAME_SIZE + PGM_DATASET_EXTENSION_SIZE + 1];
    char fname_index[MAX_PGM_DATASET_BASENAME_SIZE + PGM_DATASET_EXTENSION_SIZE + PGM_DATASET_INDEX_FILE_PREFIX_SIZE + 1];

    if(pgm_dataset_basename_validator(basename) == 1){
        return 1;
    }

    if(pgm_dataset_file_creator(basename, fname_dataset) == 1){
        return 1;
    }

    if(pgm_dataset_index_file_creator(basename, fname_index) == 1){
        return 1;
    }

    return 0;
}


static int pgm_dataset_file_creator(const char *basename, char *fname_dataset){

    FILE *out_stream;
    size_t fname_max_len;
    int ch;

    fname_max_len = MAX_PGM_DATASET_BASENAME_SIZE + PGM_DATASET_EXTENSION_SIZE;
    
    /* concatenates the passed basename with "PGM_DATASET_EXTENSION" and checks it was succesfull */
    if(snprintf(fname_dataset, fname_max_len + 1, "%s.dat", basename) < 0){
        return 1;
    }

    out_stream = fopen(fname_dataset, "ab+");
    if(out_stream == NULL){
        return 1;
    }

    /* checks if the opened file is empty */
    fseek(out_stream, 0, SEEK_SET);
    ch = fgetc(out_stream);
    if(ch != EOF){
        fclose(out_stream);
        return 1;
    } 

    fclose(out_stream);

    return 0;
}


static int pgm_dataset_index_file_creator(const char *basename, char *fname_index){

    FILE *out_stream;
    size_t fname_max_len;
    int ch;

    fname_max_len = MAX_PGM_DATASET_BASENAME_SIZE + PGM_DATASET_EXTENSION_SIZE + PGM_DATASET_INDEX_FILE_PREFIX_SIZE;

    /* concatenates the passed basename with "PGM_DATASET_INDEX_FILE_PREFIX" and "PGM_DATASET_EXTENSION" and checks it was succesfull */
    if(snprintf(fname_index, fname_max_len + 1, "idx_%s.dat", basename) < 0){
        return 1;
    }

    out_stream = fopen(fname_index, "ab+");
    if(out_stream == NULL){
        return 1;
    }

    /* checks if the opened file is empty */
    fseek(out_stream, 0, SEEK_SET);
    ch = fgetc(out_stream);
    if(ch != EOF){
        fclose(out_stream);
        return 1;
    } 

    fclose(out_stream);

    return 0;
}


t_pgm* pgm_dataset_reader(const char *basename_dataset, const char *basename_img){

    FILE *in_stream_dataset;
    FILE *in_stream_index;
    size_t max_name_len;
    char img_founded;

    t_pgm *img_read;
    t_pgm_dataset_fidx_record record;

    max_name_len = MAX_PGM_DATASET_BASENAME_SIZE + PGM_DATASET_EXTENSION_SIZE + PGM_DATASET_INDEX_FILE_PREFIX_SIZE;
    char fname_dataset[max_name_len + 1];
    char fname_index[max_name_len + 1];

     /* Before attempt to read any file, it's necessary to conclude all parameters checks and allocates the t_pgm structure.
        Furthermore, it's necessary to verify if both PGM dataset and it's respective index file are succesffuly opened */

    if(pgm_dataset_basename_validator(basename_dataset) == 1){
        return NULL;
    }

    /* concatenates the passed basename with "PGM_DATASET_EXTENSION" and checks if it was succesfull */
    if(snprintf(fname_dataset, max_name_len + 1, "%s.dat", basename_dataset) < 0){
        return NULL;
    }

    /* concatenates the passed basename with "PGM_DATASET_INDEX_FILE_PREFIX" and "PGM_DATASET_EXTENSION" and checks if it was succesfull */
    if(snprintf(fname_index, max_name_len + 1, "idx_%s.dat", basename_dataset) < 0){
        return NULL;
    }

    in_stream_dataset = fopen(fname_dataset, "rb+");
    if(in_stream_dataset == NULL){
        return NULL;
    }

    in_stream_index = fopen(fname_index, "rb+");
    if(in_stream_index == NULL){
        return NULL;
    }

    img_founded = '0';
    while(fscanf(in_stream_index, "%ld|%[^|]|%c|%ld\n", &record.img_offset, record.img_basename, &record.is_excluded, &record.img_size) == 4){

        if(strcmp(basename_img, record.img_basename) == 0){
            img_founded = '1';
            break;
        }
    }

    fclose(in_stream_index);

    if(img_founded == '0'){
        fclose(in_stream_dataset);
        return NULL;
    }

    if(record.is_excluded == '1'){
        fclose(in_stream_dataset);
        return NULL;
    }

    img_read = pgm_allocator();
    if(img_read == NULL){
        return NULL;
    }

    fseek(in_stream_dataset, record.img_offset, SEEK_SET);

    if(pgm_dataset_image_reader(in_stream_dataset, img_read) == 1){
        fclose(in_stream_dataset);
        pgm_deallocator(img_read);
        return NULL;
    }

    fclose(in_stream_dataset);

    return img_read;
}


static int pgm_dataset_image_reader(FILE *in_stream, t_pgm *img){

    char fmt_aux[MAX_FORMAT_NAME_SIZE];
    size_t n_read;
    uint8_t p8;
    uint16_t p16;
    int i, j;


    /* Try to read the PGM image format from the dataset, EOF indicates emptiness file. */
    if(fscanf(in_stream, "%s", fmt_aux) == EOF){
        return 1;
    }

    /* Checks if the read PGM format is supportted. In afirmative case stores it and continue. */
    if(strcmp(fmt_aux, "P2") == 0){
        img->fmt = P2;
    }
    else if(strcmp(fmt_aux, "P5") == 0){
            img->fmt = P5;
        }
        else{
            return 1;
        }

    fscanf(in_stream, "%d", &(img->width));
    fscanf(in_stream, "%d", &(img->height));
    fscanf(in_stream, "%" SCNu16, &(img->max_gray_level));

    fgetc(in_stream);

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


int pgm_dataset_writer(const char *basename_dataset, const char *basename_img, const t_pgm *img){

    FILE *out_stream_dataset;
    FILE *out_stream_index;
    size_t max_name_len;
    long int start_pos, final_pos, n_writted;
    char must_be_excluded;

    max_name_len = MAX_PGM_DATASET_BASENAME_SIZE + PGM_DATASET_EXTENSION_SIZE + PGM_DATASET_INDEX_FILE_PREFIX_SIZE;
    char fname_dataset[max_name_len + 1];
    char fname_index[max_name_len + 1];

    /*  Before writing in any file, it's necessary to conclude all parameters checks.
        Furthermore, it's necessary to verify if both PGM dataset and it's respective index file are succesffuly opened */

    if(pgm_dataset_basename_validator(basename_dataset) == 1){
        return 1;
    }

    /* concatenates the passed basename with "PGM_DATASET_EXTENSION" and checks it was succesfull */
    if(snprintf(fname_dataset, max_name_len + 1, "%s.dat", basename_dataset) < 0){
        return 1;
    }

    /* concatenates the passed basename with "PGM_DATASET_INDEX_FILE_PREFIX" and "PGM_DATASET_EXTENSION" and checks it was succesfull */
    if(snprintf(fname_index, max_name_len + 1, "idx_%s.dat", basename_dataset) < 0){
        return 1;
    }

    out_stream_dataset = fopen(fname_dataset, "rb+");
    if(out_stream_dataset == NULL){
        return 1;
    }

    out_stream_index = fopen(fname_index, "rb+");
    if(out_stream_index == NULL){
        return 1;
    }

    /* Writes the PGM image in the dataset */

    fseek(out_stream_dataset, 0, SEEK_END);
    start_pos = ftell(out_stream_dataset);

    must_be_excluded = '0';
    if(pgm_dataset_file_writer(out_stream_dataset, img) == 1){
        must_be_excluded = '1';
    }

    final_pos = ftell(out_stream_dataset);
    n_writted = final_pos - start_pos;
    fclose(out_stream_dataset);

    /* Writes in the index file the informations about the image write in the dataset */

    fseek(out_stream_index, 0, SEEK_END);
    fprintf(out_stream_index, "%ld|%s|%c|%ld\n", start_pos, basename_img, must_be_excluded, n_writted);

    fclose(out_stream_index);

    return 0;

}


static int pgm_dataset_file_writer(FILE *out_stream, const t_pgm *img){

    uint8_t p8;
    uint16_t p16;
    int n_written;
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