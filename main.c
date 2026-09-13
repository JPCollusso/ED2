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

    clean_stdin();
    clear_terminal();

    while(1){

        printf("\n\t-----PGM PROGRAM MENU-----");
        printf("\n\n\t\t1. Convert p2 to p5 format");
        printf("\n\t\t2. Thresholding image");
        printf("\n\t\t3. Create a PGM dataset");
        printf("\n\t\t4. Exit");
        printf("\n\t\tOption: ");
        scanf("%d", &opt);
        clean_stdin();

        switch(opt){

            case 1: 
                printf("\n\t\tImage name to convert: ");
                scanf("%s", fname_aux1);
                printf("\t\tConverted image name: ");
                scanf("%s", fname_aux2);
                
                if(pgm_p2_to_p5(fname_aux1, fname_aux2) == 1){
                    printf("\n\tAn unexpected error has occurred! Press any key to continue");
                }
                else printf("\n\tSuccess! Press any key to continue ");

                break;

            case 2: 
                printf("\n\t\tImage name to thresholding: ");
                scanf("%s", fname_aux1);
                
                if((img_read = pgm_reader(fname_aux1)) == NULL){
                    printf("\n\tAn unexpected error has occurred! Press any key to continue");
                }
                else{

                    printf("\t\tLimiar value: ");
                    scanf("%" SCNu16, &thresh_limiar);

                    if((img_edited = pgm_thresholding(thresh_limiar, img_read)) == NULL){
                        pgm_deallocator(img_read);
                        printf("\n\tAn unexpected error has occurred! Press any key to continue");
                    }
                    else{

                        printf("\t\tThresholded image name: ");
                        scanf("%s", fname_aux2);

                        if(pgm_writter(fname_aux2, img_edited, P5) == 1){
                            printf("\n\tAn unexpected error has occurred! Press any key to continue");
                        }
                        else printf("\n\tSuccess! Press any key to continue ");

                        pgm_deallocator(img_read);
                        pgm_deallocator(img_edited);
                    }
                }
                break;

            case 3:
                printf("\n\t\tGive a name to the PGM dataset: ");
                scanf("%s", fname_aux1);

                if(pgm_dataset_creator(fname_aux1) == 1){
                    printf("\n\tAn unexpected error has occurred! Press any key to continue");
                }
                else printf("\n\tSuccess! Press any key to continue ");
                break;

            case 4:
                printf("\n\tExiting...");
                getchar();
                return 0;

            default:
                printf("\n\tInvalid entry! Press any key to continue ");

        }
        pause_terminal();
        clean_stdin();
        clear_terminal();
        printf("\n");

    }
    
}