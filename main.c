#include <stdio.h>
#include <inttypes.h>

#include "pgm.h"

#define MAX_NAME_FILE 20

// Programa de exemplo que fornece um menu interativo para operar
// sobre imagens PGM: conversão de formatos, limiar (thresholding),
// armazenamento em banco interno e exportação.

int main(){

    // Ponteiros para a imagem lida e para imagens resultantes de operações
    t_pgm *img_read, *img_edited;
    // Valor do limiar usado na operação de thresholding
    uint16_t thresh_limiar;
    // Buffers para nomes de arquivos de entrada/saida
    char fname_aux1[MAX_NAME_FILE];
    char fname_aux2[MAX_NAME_FILE];
    // Opção selecionada no menu
    int opt;

    while(1){

            printf("\n\t-----PGM PROGRAM MENU-----");
            printf("\n\n\t\t1. Convert p2 to p5 format");
            printf("\n\t\t2. Thresholding image");
            printf("\n\t\t3. Add image to DB");
            printf("\n\t\t4. List images in DB");
            printf("\n\t\t5. Export image from DB");
            printf("\n\t\t-1. Exit");
            printf("\n\t\tOption: ");
            scanf("%d", &opt);
            getchar();

        // Processa a opção escolhida pelo utilizador
        switch(opt){

            // Caso 1: converter imagem do formato P2 para P5
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

            // Caso 2: aplicar thresholding à imagem
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

            // Caso 3: adicionar imagem ao banco de imagens (arquivo images.db)
            case 3:
                {
                    char dbname[64] = "images.db";
                    char keyname[64] = "images.keys";
                    char imgname[MAX_NAME_FILE];
                    char srcfname[MAX_NAME_FILE];

                    printf("Image file to add: ");
                    scanf("%19s", srcfname);
                    printf("Name to store: ");
                    scanf("%19s", imgname);

                    if(db_add_image(dbname, keyname, srcfname, imgname) == 0){
                        printf("\n\tStored successfully\n");
                    } else {
                        printf("\n\tError storing image\n");
                    }
                }
                break;

            // Caso 4: listar imagens armazenadas no arquivo de chaves
            case 4:
                db_list_images("images.keys");
                break;

            // Caso 5: exportar uma imagem do DB com opções de pós-processamento
            case 5:
                {
                    char keyname[64] = "images.keys";
                    char dbname[64] = "images.db";
                    char imgname[MAX_NAME_FILE];
                    uint64_t offset;
                    t_pgm *img = NULL;
                    t_pgm *edited = NULL;
                    int choice;
                    uint16_t limiar;
                    char outfname[64];

                    printf("Image name to export: ");
                    scanf("%19s", imgname);

                    if(db_find_offset_by_name(keyname, imgname, &offset) != 0){
                        printf("Image not found in DB\n");
                        break;
                    }

                    img = db_read_image_by_offset(dbname, offset);
                    if(img == NULL){
                        printf("Error reading image from DB\n");
                        break;
                    }

                    printf("Export options:\n1. Unmodified\n2. Thresholding\n3. Negate\nChoice: ");
                    scanf("%d", &choice);

                    if(choice == 1){
                        printf("Output filename: ");
                        scanf("%63s", outfname);
                        if(pgm_writter(outfname, img, P5) == 0)
                            printf("Exported\n"); 
                        else 
                            printf("Error exporting\n");
                    } else if(choice == 2){
                        printf("Limiar value: ");
                        scanf("%" SCNu16, &limiar);
                        edited = pgm_thresholding(limiar, img);
                        if(edited){
                            printf("Output filename: ");
                            scanf("%63s", outfname);
                            if(pgm_writter(outfname, edited, P5) == 0) 
                                printf("Exported\n"); 
                            else   
                                printf("Error exporting\n");
                            pgm_deallocator(edited);
                        } 
                        else {
                            printf("Error applying threshold\n");
                        }
                    } else if(choice == 3){
                        edited = pgm_negate(img);
                        if(edited){
                            printf("Output filename: ");
                            scanf("%63s", outfname);
                            if(pgm_writter(outfname, edited, P5) == 0) 
                                printf("Exported\n"); 
                            else 
                                printf("Error exporting\n");
                            pgm_deallocator(edited);
                        } else {
                            printf("Error applying negate\n");
                        }
                    } else {
                        printf("Invalid choice\n");
                    }

                    pgm_deallocator(img);
                }
                break;


            // Opção para sair do programa
            case -1:
                printf("\tExiting...");
                getchar();
                return 0;

            // Entrada inválida
            default:
                printf("\n\t\tInvalid entry, press any key to continue ");

        }

        getchar();

    }
    
}