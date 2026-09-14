/*
    Developed by:
        João Pedro Meneghetti Collusso;
        Vitor Barreto Carvalho.

    To compile and execute:
        First, access by the terminal the src directory (where main.c and Makefile are).
        Them, type "make" and enter it.
        Finally, type "./pgm" and enter it. 
*/

#include <stdio.h>
#include <inttypes.h>

#include "pgm_handler.h"
#include "pgm_dataset.h"
#include "pgm_ops.h"

#include "utils.h"

#define MAX_NAME_FILE 20

int main(){

    t_pgm *img_read, *img_edited;
    uint16_t thresh_limiar;
    char fname_aux1[MAX_NAME_FILE];
    char fname_aux2[MAX_NAME_FILE];
    int opt;

    clear_terminal();

    while(1){

        printf("\n\t-----PGM PROGRAM MENU-----");
        printf("\n\n\t\t1. Editation module");
        printf("\n\t\t2. Image file manipulation module");
        printf("\n\t\t3. Dataset manipulation module");
        printf("\n\t\t4. Exit");

        printf("\n\n\tOption: ");
        scanf("%d", &opt);

        clean_stdin();
        clear_terminal();

        switch(opt){

            case 1: 

                while(1){

                    printf("\n\t-----EDITATION MODULE-----");
                    printf("\n\n\t\t1. Thresholding image");
                    printf("\n\t\t2. Back");

                    printf("\n\n\tOption: ");
                    scanf("%d", &opt);
                    
                    if(opt == 2){
                        break;
                    }

                    clean_stdin();

                    switch(opt){

                        case 1:
                            printf("\n\t\tImage name: ");
                            scanf("%s", fname_aux1);
                            clean_stdin();

                            img_read = pgm_reader(fname_aux1);
                            if(img_read == NULL){
                                printf("\n\tAn unexpected error has occurred! Press any key to continue ");
                                break;
                            }

                            printf("\t\tLimiar value: ");
                            scanf("%" SCNu16, &thresh_limiar);
                            clean_stdin();

                            img_edited = pgm_thresholding(thresh_limiar, img_read);

                            if(img_edited == NULL){
                                printf("\n\tAn unexpected error has occurred! Press any key to continue ");
                                pgm_deallocator(img_read);
                                img_read = NULL;
                                break;
                            }

                            printf("\t\tGive a name to the edited image: ");
                            scanf("%s", fname_aux2);
                            clean_stdin();

                            if(pgm_writer(fname_aux2, img_edited, P5) == 1){
                                printf("\n\tAn unexpected error has occurred! Press any key to continue ");
                                pgm_deallocator(img_read);
                                pgm_deallocator(img_edited);
                                img_read = NULL;
                                img_edited = NULL;
                                break;
                            }

                            printf("\n\tSuccess! Press any key to continue ");
                            pgm_deallocator(img_read);
                            pgm_deallocator(img_edited);
                            img_read = NULL;
                            img_read = NULL;
        
                            break;

                        case 2:
                            break;

                        default:
                            printf("\n\tInvalid entry! Press any key to continue ");
                    } 

                    pause_terminal();
                    clear_terminal();
                }

                break;

            case 2:

                while(1){
                    printf("\n\t-----IMAGE FILE MANIPULATION MODULE-----");
                    printf("\n\n\t\t1. Convert p2 to p5 format");
                    printf("\n\t\t2. Back");

                    printf("\n\n\tOption: ");
                    scanf("%d", &opt);

                    if(opt == 2){
                        break;
                    }

                    clean_stdin();

                    switch(opt){

                        case 1:
                            printf("\n\t\tImage name to convert: ");
                            scanf("%s", fname_aux1);
                            clean_stdin();

                            printf("\t\tConverted image name: ");
                            scanf("%s", fname_aux2);
                            clean_stdin();

                            if(pgm_p2_to_p5(fname_aux1, fname_aux2) == 1){
                                printf("\n\tAn unexpected error has occurred! Press any key to continue");
                                break;
                            }
                                
                            printf("\n\tSuccess! Press any key to continue ");
                            break;

                        default:
                            printf("\n\tInvalid entry! Press any key to continue ");
                    }

                    pause_terminal();
                    clear_terminal();

                }
                
                break;

            case 3:

                while(1){
                    printf("\n\t-----DATASET MANIPULATION MODULE-----");
                    printf("\n\n\t\t1. Create a PGM dataset");
                    printf("\n\t\t2. Add a PGM image to a PGM dataset");
                    printf("\n\t\t3. Read a PGM image from a PGM dataset");
                    printf("\n\t\t4. Back");

                    printf("\n\n\tOption: ");
                    scanf("%d", &opt);
                   
                    if(opt == 4){
                        break;
                    }

                    clean_stdin();

                    switch(opt){

                        case 1:
                            printf("\n\t\tGive a name to the PGM dataset: ");
                            scanf("%s", fname_aux1);
                            clean_stdin();

                            if(pgm_dataset_creator(fname_aux1) == 1){
                                printf("\n\tAn unexpected error has occurred! Press any key to continue");
                                break;
                            }

                            printf("\n\tSuccess! Press any key to continue ");
                            break;

                        case 2:
                            printf("\n\t\tPGM image name: ");
                            scanf("%s", fname_aux1);
                            clean_stdin();

                            img_read = pgm_reader(fname_aux1);
                            if(img_read == NULL){
                                printf("\n\tAn unexpected error has occurred! Press any key to continue");
                                break;
                            }

                            printf("\t\tPGM dataset basename [without extension]: ");
                            scanf("%s", fname_aux2);
                            clean_stdin();

                            if(pgm_dataset_writer(fname_aux2, fname_aux1, img_read) == 1){
                                printf("\n\tAn unexpected error has occurred! Press any key to continue");
                                pgm_deallocator(img_read);
                                break;
                            }

                            printf("\n\tSuccess! Press any key to continue ");
                            pgm_deallocator(img_read);
                            break;

                        case 3:
                            printf("\n\t\tPGM image name: ");
                            scanf("%s", fname_aux1);
                            clean_stdin();

                            printf("\t\tPGM dataset basename [without extension]: ");
                            scanf("%s", fname_aux2);
                            clean_stdin();

                            img_read = pgm_dataset_reader(fname_aux2, fname_aux1);
                            if(img_read == NULL){
                                printf("\n\tAn unexpected error has occurred! Press any key to continue");
                                break;
                            }

                            printf("\n\tSuccess, what you wanna do with the readed image? ");
                            printf("\n\n\t\t1. Save it");
                            printf("\n\t\t2. Threshold and save it");
                            printf("\n\t\t3. Get it negative and save it");

                            printf("\n\n\tOption: ");
                            scanf("%d", &opt);
                            clean_stdin();

                            switch(opt){
                                        
                                case 1:
                                    if(pgm_writer(fname_aux1, img_read, P5) == 1){
                                        printf("\n\tAn unexpected error has occurred! Press any key to continue");
                                        break;
                                    }
                                        
                                    printf("\n\tSuccess! Press any key to continue ");
                                    break;

                                case 2:
                                    printf("\n\t\tLimiar value: ");
                                    scanf("%" SCNu16, &thresh_limiar);
                                    clean_stdin();

                                    img_edited = pgm_thresholding(thresh_limiar, img_read);
                                    if(img_edited == NULL){
                                        printf("\n\tAn unexpected error has occurred! Press any key to continue");
                                        break;
                                    }

                                    if(pgm_writer(fname_aux1, img_edited, P5) == 1){
                                        printf("\n\tAn unexpected error has occurred! Press any key to continue");
                                        pgm_deallocator(img_edited);
                                    }
                                    
                                    printf("\n\tSuccess! Press any key to continue ");
                                    pgm_deallocator(img_edited);

                                    break;

                                case 3:
                                    img_edited = pgm_get_negative(img_read);
                                    if(img_edited == NULL){
                                        printf("\n\tAn unexpected error has occurred! Press any key to continue");
                                        break;
                                    }

                                    if(pgm_writer(fname_aux1, img_edited, P5) == 1){
                                        printf("\n\tAn unexpected error has occurred! Press any key to continue");
                                        pgm_deallocator(img_edited);
                                        break;
                                    }

                                    printf("\n\tSuccess! Press any key to continue ");
                                    pgm_deallocator(img_edited);
                                    break;

                                default:
                                    printf("\n\tInvalid entry! Press any key to continue ");

                                }

                                pgm_deallocator(img_read);
                                img_read = NULL;

                                break;

                        case 4:
                            break;

                        default:
                            printf("\n\tInvalid entry! Press any key to continue ");

                    }

                    pause_terminal();
                    clear_terminal();

                }

                break;

            case 4:
                return 0;

            default: 
                printf("\n\tInvalid entry! Press any key to continue ");

        }   

        pause_terminal();
        clear_terminal();

    }

}