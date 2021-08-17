#include "board.h"

void simple_shift(col_t* board, int x0, int y0, int xd, int yd);

void capture(col_t* board, int x0, int y0, int x1, int y1, int xd, int yd);

moves_list_t *destroy (moves_list_t* l);

void fill_description1 (char* desc, int x0, int y0, int xd, int yd);

void fill_description2 (char* desc, int x0, int y0, int x1, int y1, int xd, int yd);

moves_list_t *union_list(moves_list_t* head, moves_list_t* new_list);

moves_list_t *insert_simple_shift (moves_list_t* l, col_t* board, int x0, int y0, int xd, int yd);

moves_list_t *insert_capture (moves_list_t* l, col_t* board, int x0, int y0, int x1, int y1, int xd, int yd);

moves_list_t* moves_analysis (moves_list_t* l, col_t* board, int player);

moves_list_t*  filter (moves_list_t* l);

void print_moves(moves_list_t* l, int* n);

col_t* estract (moves_list_t* l, int n);

char* estract_description (moves_list_t* l, int n);