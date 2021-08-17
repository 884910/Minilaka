#include "struct.h"
#include <stdio.h>
#include <stdlib.h>

void copy_pawn (pawn_t* p_dst, pawn_t* p_src);

void copy_board(col_t* src, col_t* dst);

void check_and_do_promotion (col_t* board, int to_print);

/*funzione che crea la matrix di struct colonne*/
void matrix(col_t* board);

void field(col_t* board);

