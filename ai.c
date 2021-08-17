#include "ai.h"

int evaluation (col_t* board){
	int i = 0;
	int j = 0;
	int counter = 0;
	
	for(i = 0; i < DIM; i++){
		for(j = 0; j < DIM; j++){
			if(board[i*DIM + j].size){
				if(board[i*DIM + j].tower[0].color == BLACK){
					counter --;
				}else{
					counter ++;
				}
			}
		}
	}

	return counter;
}

int max(int n1, int n2){
	if(n1 > n2){
		return n1;
	}
	
	return n2;
}

int min(int n1, int n2){
	if(n1 < n2){
		return n1;
	}
	
	return n2;
}

int minimax(col_t* board, int depth, int player){
	//i bianchi (0) massimizzano
	
	moves_list_t* l_og = NULL;
	moves_list_t* l = NULL;
    
	l_og = moves_analysis(l_og, board, player);
	
	l_og = filter(l_og);
	
	if(depth == 0 || l_og == NULL){
		return evaluation(board);
	}

	if(player == WHITE){
		int maxEval = INT_MIN;
		int eval = 0;
		l = l_og;
		while(l){
			check_and_do_promotion (l->board, 0);
			eval = minimax(l->board, depth - 1, !player);
			maxEval = max(maxEval, eval);
			l = l->next;
		}
		l_og = destroy(l_og);
		return maxEval;

	}else{
		int minEval = INT_MAX;
		int eval = 0;
		l = l_og;
		while(l){
			check_and_do_promotion (l->board, 0);
			eval = minimax(l->board, depth -1, !player);
			minEval = min(minEval, eval);
			l = l->next;
		}

		l_og = destroy(l_og);
		return minEval;
	}

}

int macro_ai (moves_list_t* l, int depth, int player){
	//i neri minimizzano e i bianchi massimizzano
	int max = INT_MIN; 
	int pos = 0;
	int  res = 0;
	if(l){
		while(l){
			int new = minimax(l->board, depth, player);
			//printf("prova %d \n", pos);
			if(max < new){
				max = new;
				res = pos + 1;
				//printf("%d valutazione board %d  description mossa %s \n", count, new, l->description);
			}

			pos++;
			l = l->next;
		}

		return res;
	}else{
		return 0;
	}

	
}