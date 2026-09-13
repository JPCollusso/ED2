#include "pgm_dataset.h"


static int pgm_dataset_file_creator(const char *basename, char *fname_dataset);
static int pgm_dataset_index_file_creator(const char *basename, char *fname_index);


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

    /* checks if the passed basename probably have an extension or if is hidden */
    for(i = 0; i < basename_len; i++){

        if(basename[i] == '.'){
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
    size_t max_name_len;
    int n_concatenated;
    int ch;

    max_name_len = MAX_PGM_DATASET_BASENAME_SIZE + PGM_DATASET_EXTENSION_SIZE;
    
    /* concatenates the passed basename with "PGM_DATASET_EXTENSION" */
    n_concatenated = snprintf(fname_dataset, max_name_len + 1, "%s.dat", basename);

    /* checks if the concatenation occurred corectaly */
    if((n_concatenated < 0)){
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
    size_t max_name_len;
    int n_concatenated;
    int ch;

    max_name_len = MAX_PGM_DATASET_BASENAME_SIZE + PGM_DATASET_EXTENSION_SIZE + PGM_DATASET_INDEX_FILE_PREFIX_SIZE;

    /* concatenates the passed basename with "PGM_DATASET_INDEX_FILE_PREFIX" and "PGM_DATASET_EXTENSION" */
    n_concatenated = snprintf(fname_index, max_name_len, "idx_%s.dat", basename);

    /* checks if the concatenation occurred corectaly */
    if((n_concatenated < 0)){
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

