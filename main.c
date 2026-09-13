#include <stdio.h>
#include <inttypes.h>

#include "pgm.h"

#define MAX_NAME_FILE 20

int main(int argc, char **argv){

    t_pgm *img_read, *img_edited;
    uint16_t thresh_limiar;
    char fname_aux1[MAX_NAME_FILE];
    char fname_aux2[MAX_NAME_FILE];
    int opt;

    while(1){

        if(argc == 1){
            printf("\n\t-----PGM PROGRAM MENU-----");
            printf("\n\n\t\t1. Convert p2 to p5 format");
            printf("\n\t\t2. Thresholding image");
            printf("\n\t\t3. Exit");
            printf("\n\t\tOption: ");
            scanf("%d", &opt);
            getchar();
        }
        else{
            if(1){

            }
        }

        switch(opt){

            case 1: 
                printf("Image name to convert: ");
                scanf("%s", fname_aux1);
                printf("Converted image name: ");
                scanf("%s", fname_aux2);
                
                if(pgm_p2_to_p5(fname_aux1, fname_aux2)){
                    printf("\n\t\tERROR\n");
                }
                else printf("\n\t\tSUCCESS\n");

                break;

            case 2: 
                printf("\n\n\t\tImage name: ");
                scanf("%s", fname_aux1);
                
                if((img_read = pgm_reader(fname_aux1)) == NULL){
                    printf("\n\tERROR");
                }
                else{

                    printf("\t\tLimiar value: ");
                    scanf("%" SCNu16, &thresh_limiar);

                    if((img_edited = pgm_thresholding(thresh_limiar, img_read)) == NULL){
                        pgm_deallocator(img_read);
                        printf("\n\tERROR");
                    }
                    else{

                        if(pgm_writter("temporari.pgm", img_edited, P5) == 1){
                            printf("\n\tERROR");
                        }
                        else printf("SUCCESS");

                        pgm_deallocator(img_read);
                        pgm_deallocator(img_edited);
                    }
                }

                break;


            case 3:
                printf("\tExiting...");
                getchar();
                return 0;

            default:
                printf("\n\t\tInvalid entry, press any key to continue ");

        }

        getchar();

    }
    
}