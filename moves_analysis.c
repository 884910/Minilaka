#include "moves_analysis.h"

void simple_shift(col_t* board, int x0, int y0, int xd, int yd){ 
	int i;
	for (i=0; i<board[y0 * DIM +x0].size; i++){
		copy_pawn(&(board[yd * DIM + xd].tower[i]), &(board[y0 * DIM + x0].tower[i]));
	}
	
	board[yd * DIM + xd].size = board[y0*DIM+x0].size;
	board[y0 * DIM + x0].size=0;
	free(board[y0 * DIM + x0].tower);
}

void capture(col_t* board, int x0, int y0, int x1, int y1, int xd, int yd){
	
	int sizeatt= board[y0*DIM+x0].size;
	int i;

	
	if(sizeatt!=3){
		copy_pawn(&board[y0*DIM+x0].tower[sizeatt],&board[y1*DIM+x1].tower[0]);
		board[y0*DIM+x0].size++;	
	}
	board[y1*DIM+x1].size--;
	if(board[y1*DIM+x1].size!=0){
		for(i=0;i<board[y1*DIM+x1].size;i++){
			copy_pawn(&board[y1*DIM+x1].tower[i],&board[y1*DIM+x1].tower[i+1]);
		}
	}else{
		free(board[y1*DIM+x1].tower);
	}
	simple_shift(board, x0, y0, xd, yd);
}

moves_list_t *destroy (moves_list_t* l){ 
    if(l->next!=NULL){						
		destroy(l->next);
	}
	free(l);
	l=NULL;
	return l;
}

void fill_description1 (char* desc, int x0, int y0, int xd, int yd){ 
	desc[0] = x0 + 'A';												
	desc[1] = y0 + '1'; 
	desc[2] = '-';
	desc[3] = xd + 'A';
	desc[4] = yd + '1';
}

void fill_description2 (char* desc, int x0, int y0, int x1, int y1, int xd, int yd){ 
	desc[0] = x0 + 'A';
	desc[1] = y0 + '1';
	desc[2] = '-';
	desc[3] = x1 + 'A';
	desc[4] = y1 + '1';
	desc[5] = '-';
	desc[6] = xd + 'A';
	desc[7] = yd + '1';
}

moves_list_t *union_list(moves_list_t* head, moves_list_t* new_list){ 
	moves_list_t* temp;
	if(head==NULL){
		return new_list;
	}else{
		temp=head;
		while(temp->next!=NULL) temp=temp->next;
		temp->next=new_list;
	}
	return head;
}

moves_list_t *insert_simple_shift (moves_list_t* l, col_t* board, int x0, int y0, int xd, int yd){
	moves_list_t *temp = (moves_list_t*) malloc(sizeof(moves_list_t));
	temp->board = (col_t*) malloc(sizeof(col_t) * 49);
    copy_board(board, temp->board);
	simple_shift (temp->board, x0, y0, xd, yd);
    temp->description = (char*) malloc(sizeof(char)*10 + 1);
	fill_description1(temp->description, x0, y0, xd, yd);
	temp->mandatory = 0;
	temp->next=NULL;
	l=union_list(l,temp);
	return l;
}

moves_list_t *insert_capture (moves_list_t* l, col_t* board, int x0, int y0, int x1, int y1, int xd, int yd){
	moves_list_t *temp = (moves_list_t*) malloc(sizeof(moves_list_t));
    temp->board = (col_t*) malloc(sizeof(col_t)*49);
    copy_board(board, temp->board);
    capture(temp->board, x0, y0, x1, y1, xd, yd);
    temp->description = (char*) malloc(sizeof(char)*10 + 1);
	fill_description2(temp->description, x0, y0, x1, y1, xd, yd);
    temp->mandatory = 1;
	temp->next=NULL;
    l=union_list(l,temp);
	return l;
}

moves_list_t* moves_analysis (moves_list_t* l, col_t* board, int player){ 
	int i, j;
	char color, enemy_color;

	color = player;
	enemy_color = 1 - color;
	
	for(i=0;i<DIM;i++){
		for(j=0;j<DIM;j++){
			//ciclo di scan della board

			if(board[i*DIM+j].tower != NULL){ //controllo se c'è una pedina
				
				if(board[i*DIM+j].tower[0].color == color){ //se la pedina è del giocatore del giocatore corrente

					if(board[i*DIM+j].tower[0].rank == OFFICER || (board[i*DIM+j].tower[0].rank == SOLDIER && color == WHITE)){  //set mosse in giù
						if(i+1 < DIM){ 
							//caso spostamento semplice per i soldati bianchi 
							if (j-1>=0){
								if(board[(i+1)*DIM + (j-1)].size == 0){
									l=insert_simple_shift (l, board, j, i, j-1, i+1);	
								}else if(board[(i+1)*DIM + (j-1)].tower[0].color == enemy_color){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i+2<=DIM && j-2>=0 && board[(i+2)*DIM + (j-2)].size == 0){ //se oltre il nemico è libero
										l=insert_capture (l, board, j, i, j-1, i+1, j-2, i+2);			
									}
								}
							}
							if (j+1<DIM){
								if(board[(i+1)*DIM + (j+1)].size == 0){
									l=insert_simple_shift (l, board, j, i, j+1, i+1);    
								}else if(board[(i+1)*DIM + (j+1)].tower[0].color == enemy_color){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i+2<DIM && j+2<DIM && board[(i+2)*DIM + (j+2)].size == 0){ //se oltre il nemico è libero                                        
											l=insert_capture (l, board, j, i, j+1, i+1, j+2, i+2);    												
									}
								}
							}
						}
					}
					 
					if(board[i*DIM+j].tower[0].rank==OFFICER || (board[i*DIM+j].tower[0].rank==SOLDIER && color==BLACK)){ //set di mosse in su
						if(i-1 >= 0){
							if(j-1 >= 0){
								//caso spostamento semplice per i soldati neri
								if(board[(i-1)*DIM + (j-1)].size == 0){
									l=insert_simple_shift (l, board, j, i, j-1, i-1);
								}else if(board[(i-1)*DIM + (j-1)].tower[0].color == enemy_color){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i-2 >= 0 && j-2 >= 0 && board[(i-2)*DIM + (j-2)].size == 0){ //se oltre il nemico è libero
											l=insert_capture (l, board, j, i, j-1, i-1, j-2, i-2);					
									}
								}
							}

							if(j+1 < DIM){
								if(board[(i-1)*DIM + (j+1)].size == 0){
									l=insert_simple_shift (l, board, j, i, j+1, i-1);						
								}else if(board[(i-1)*DIM + (j+1)].tower[0].color == enemy_color){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i-2 >= 0 && j+2 < DIM && board[(i-2)*DIM + (j+2)].size == 0){ //se oltre il nemico è libero
										l=insert_capture (l, board, j, i, j+1, i-1, j+2, i-2);									
		
									}
								}
							}
						}
					}
				}
			}
		}
	}
    return l;
}

moves_list_t*  filter (moves_list_t* l){
	int flag = 0; //variabile bool che diventa 1 se e solo se ci sono mosse nella lista con obbligatorietà == 1
	moves_list_t* list = l;
	moves_list_t* tmp_head = NULL;
	moves_list_t* temp = NULL;
	if(l!=NULL){
		while (list!= NULL && !flag){
			if(list->mandatory == 1){
				flag=1;
			}
			list=list->next;
		}

		if(flag == 1){
			list = l;

			while (list!= NULL){
				if(list->mandatory == 1){
					temp=list->next;
					list->next=NULL;
					tmp_head=union_list(tmp_head,list);
					list=temp;	
				}else{
					temp=list;
					list=list->next;
					free(temp);
				}
			}
			l=tmp_head;
		}
	}
	return l;
}

void print_moves(moves_list_t* l, int* n) {
	int i = 0;
	while (l != NULL ) {
        i++;
		printf("%2d) %s\n", i,l->description); //qua ho messo 2d così è tutto più ordinato
		l = l->next;
	}

	*n = i; //totale mosse
}

col_t* estract (moves_list_t* l, int n){
	int i;

	for(i=1; i<n; i++){ 
		l=l->next;
	}

	return l->board;
}

char* estract_description (moves_list_t* l, int n){
	int i;

	for(i=1; i<n; i++){ 
		l=l->next;
	}

	return l->description;
}
