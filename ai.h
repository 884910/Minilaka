#include "moves_analysis.h"
#include <limits.h>
#include <math.h>

int evaluation (col_t* board);

int max(int n1, int n2);

int min(int n1, int n2);

int minimax(col_t* board, int depth, int player);

int macro_ai (moves_list_t* l, int depth, int player);