#ifndef PGM_HANDLER_H
#define PGM_HANDLER_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FORMAT_NAME_SIZE 5

// Formatos PGM suportados pelo programa.
// "Unspecified" é usado como valor inicial quando ainda não se conhece o formato.
typedef enum pgm_format_s { 
    Unspecified,    
    P2,            
    P5              
} t_pgm_format;



// Estrutura que representa uma imagem PGM em memória.
// - fmt: formato (P2/P5)
// - width/height: dimensões da imagem
// - max_gray_level: nível máximo de cinza
// - pixel: matriz dinâmica de pixels indexada como pixel[linha][coluna]
typedef struct pgm_s {
    t_pgm_format fmt;           
    int width;                  
    int height;                 
    uint16_t max_gray_level;    
    uint16_t **pixel;           
} t_pgm;



// Aloca e inicializa uma estrutura t_pgm vazia.
// Retorna um ponteiro para a estrutura, ou NULL em caso de falha.
t_pgm* pgm_allocator();



// Libera a memória associada a img, incluindo a matriz de pixels.
// Não faz nada se img for NULL.
void pgm_deallocator(t_pgm *img);



// Aloca a matriz de pixels com dimensões definidas em img->width e img->height.
// Retorna 0 em sucesso, 1 em erro de alocação.
int pgm_pixel_matrix_allocator(t_pgm *img);



// Libera apenas a matriz de pixels de img, sem libertar a própria estrutura.
void pgm_pixel_matrix_deallocator(t_pgm *img);



// Lê uma imagem PGM (P2 ou P5) de "fname" e devolve uma t_pgm preenchida.
// O arquivo é aberto em modo binário para permitir leitura de ambos os formatos.
t_pgm* pgm_reader(char *fname);



// Escreve a imagem "img" para o arquivo "fname" no formato especificado por "fmt".
// Se ocorrer algum erro durante a escrita, o arquivo de destino pode ser removido.
int pgm_writter(char *fname, t_pgm *img, t_pgm_format fmt);


#endif /* PGM_HANDLER_H */

