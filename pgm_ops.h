#ifndef PGM_OPS_H
#define PGM_OPS_H

#include "pgm_handler.h"

#include <inttypes.h>

// Converte uma imagem P2 (texto) para P5 (binário).
// Retorna 0 em sucesso, 1 em erro.
int pgm_p2_to_p5(char *fname_p2, char *fname_p5);

// Aplica thresholding à "img_src" e devolve uma nova imagem.
// O valor "limiar" define o corte: pixels > limiar são atualizados para o nível máximo de cinza, os demais recebem 0.
t_pgm* pgm_thresholding(uint16_t limiar, t_pgm *img_src);

// Cria e devolve uma nova imagem resultante da negação dos níveis de cinza.
t_pgm* pgm_negate(t_pgm *img_src);


#endif