#include "board.h"

void copy_pawn (pawn_t* p_dst, pawn_t* p_src){  
	p_dst->name = p_src->name;
	p_dst->rank = p_src->rank;
	p_dst->color = p_src->color;
}

void copy_board(col_t* src, col_t* dst){ 
	int i,j,k;											
	for(i=0;i<DIM;i++){									
		for(j=0;j<DIM;j++){
			if(src[i*DIM+j].tower!=NULL){
				if(dst[i*DIM+j].tower == NULL){
					dst[i*DIM+j].tower = (pawn_t*) malloc(sizeof(pawn_t) * 3);
				}
				for(k=0;k<3;k++){
					copy_pawn(&(dst[i*DIM+j].tower[k]),&(src[i*DIM+j].tower[k]));
				}
				dst[i*DIM+j].size = src[i*DIM+j].size;
			}
		}
	}
}

void check_and_do_promotion (col_t* board, int to_print){ 
   	int j;													
	for (j=0; j<DIM; j++){
    	if(board[6*DIM + j].tower != NULL){
        
            if(board[6*DIM + j].tower[0].color == WHITE && board[6*DIM + j].tower[0].rank == SOLDIER){ //è bianco ed è soldato
                board[6*DIM + j].tower[0].rank = OFFICER;
                board[6*DIM + j].tower[0].name -= 32;
				if(to_print){
					printf("%c7 è stato promosso", j+65);
				}
                //to_print == 0 solo in minimax() che è ricorsiva
            }
        }
	}    

    
	for (j=0; j<DIM; j++){
    	if(board[0+j].tower != NULL){   
            if(board[0+j].tower[0].color == BLACK  && board[0*DIM + j].tower[0].rank == SOLDIER){ //è nero
                board[0+j].tower[0].rank = OFFICER;
                board[0+j].tower[0].name -= 32;
				if(to_print){
					printf("%c1 è rank promosso", j+65);
				}
                //to_print == 0 solo in minimax() che è ricorsiva
            }
        }
	}    
}

/*funzione che crea la matrice di struct colonne*/
void matrix(col_t* board) {
	int i, j;
	for(i=0; i<DIM; i++){
		for(j=0; j<DIM; j++){
			if(i<=2){
				if((i*DIM+j) % 2 == 0){
					board[i*DIM+j].tower = (pawn_t*) malloc(sizeof(pawn_t) * 3);

					if (!board[i * DIM + j].tower) {
						printf("Errore di allocazione di memoria");
						exit(EXIT_FAILURE);
					}

					board[i*DIM+j].tower[0].color = WHITE;
					board[i*DIM+j].tower[0].name = 'w';
					board[i*DIM+j].tower[0].rank = SOLDIER;
					board[i*DIM+j].size = 1;
					board[i*DIM+j].capacity = 3;     
				}else{
					board[i*DIM+j].tower = NULL;
					board[i*DIM+j].size = -1;
					board[i*DIM+j].capacity = 0;
				}
			}else if(i == 3){                                                                     
				if((i*DIM+j) % 2 == 0){
					board[i*DIM+j].tower = (pawn_t*) malloc(sizeof(pawn_t) * 3);

					if (!board[i * DIM + j].tower) {
						printf("Errore di allocazione di memoria");
						exit(EXIT_FAILURE);
					}
					
					board[i*DIM+j].size = 0;
					board[i*DIM+j].capacity = 3;
				}else{
					board[i*DIM+j].tower = NULL;
					board[i*DIM+j].size = -1;
					board[i*DIM+j].capacity = 0;
				}
			}else{
				if((i*DIM+j) % 2 == 0){
					board[i*DIM+j].tower = (pawn_t*) malloc(sizeof(pawn_t) * 3);

					if (!board[i * DIM + j].tower) {
						printf("Errore di allocazione di memoria");
						exit(EXIT_FAILURE);
					}
					
					
					board[i*DIM+j].tower[0].color = BLACK;
					board[i*DIM+j].tower[0].name = 'b';
					board[i*DIM+j].tower[0].rank = SOLDIER;
					board[i*DIM+j].size = 1;
					board[i*DIM+j].capacity = 3;
				}else{
					board[i*DIM+j].tower = NULL;
					board[i*DIM+j].size = -1;
					board[i*DIM+j].capacity = 0;
				}
			}
		}
	}
}

void field(col_t* board) {

	int numbers[7] = { 1,2,3,4,5,6,7 };
	char letters[9] = { '/','a','b','c','d','e','f','g','/' };
	int row,row_cell,col,col_cell,t;
    printf("\n");
	for(row=0;row<9;row++){
        for(t=0;t<55;t++)printf("-");
        printf("\n");
        if(row==0 || row==8){            
            for(row_cell=0;row_cell<5;row_cell++){
                for(col=0;col<9;col++){
                    printf("|");
                    if(row_cell==2){
                        for(col_cell=0;col_cell<5;col_cell++){
                            printf("%c",letters[col]);
                        }
                    }else{
                        for(col_cell=0;col_cell<5;col_cell++){
                            printf(" ");
                        }
                    }

                }
                printf("|\n");
            }
        }else{
            for(row_cell=0;row_cell<5;row_cell++){
                printf("|  %d  |",numbers[row-1]);
                for(col=0;col<7;col++){
                    for(col_cell=0;col_cell<5;col_cell++){
						if((row+col)%2==0){
                            printf("°");
                        }else{
							t=board[(row-1)*7+(col)].size;
							if(col_cell==2 && row_cell>0 && row_cell<4 && t+row_cell>3){
								printf("%c",board[(row-1)*DIM+(col)].tower[t+row_cell-4].name);
							}else{
								printf(" ");
							}
						}
                    }
                    printf("|");
                }
                printf("  %d  |\n",numbers[row-1]);
            }
        }
	}
	for(t=0;t<55;t++)printf("-");
	printf("\n");
}

