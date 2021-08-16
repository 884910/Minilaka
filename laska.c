
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#define DIM 7
#define DEPTH 5
#define PLAYER_1 0
#define PLAYER_2 1
#define WHITE 0
#define BLACK 1
#define SOLDIER 0
#define OFFICER 1


/*struct pawns*/
typedef
struct pawns {
	int color;
	char name;
	char rank;
} pawns_t;

/*struct cols*/
typedef
struct cols {
	int size;
	//array of struct pawns
	struct pawns* tower;
}cols_t;

typedef
struct list_of_moves{
    char mandatory; //0=no, 1=yes
    struct cols* board; //qui inseriremo il new possibile rank della board
    char* description; //ex. A5 - C7 
    struct list_of_moves* next;
}list_of_moves_t;

void copy_pawns (pawns_t* p_dst, pawns_t* p_src){  
	p_dst->name = p_src->name;
	p_dst->rank = p_src->rank;
	p_dst->color = p_src->color;
}

void copy_board(cols_t* src, cols_t* dst){ 
	int i,j,k;											
	for(i=0;i<DIM;i++){									
		for(j=0;j<DIM;j++){
			if(src[i*DIM+j].tower!=NULL){	
				//if(dst[i*DIM+j].tower == NULL){
					dst[i*DIM+j].tower = (pawns_t*) malloc(sizeof(pawns_t) * 3);
				//}
				for(k=0;k<3;k++){
					copy_pawns(&(dst[i*DIM+j].tower[k]),&(src[i*DIM+j].tower[k]));
				}
				dst[i*DIM+j].size = src[i*DIM+j].size;
			}
		}
	}
}

void simple_shift(struct cols* board, int x0, int y0, int xd, int yd){ 
	int i;
	for (i=0; i<board[y0 * DIM +x0].size; i++){
		copy_pawns(&(board[yd * DIM + xd].tower[i]), &(board[y0 * DIM + x0].tower[i]));
	}
	
	board[yd * DIM + xd].size = board[y0*DIM+x0].size;
	board[y0 * DIM + x0].size=0;
	free(board[y0 * DIM + x0].tower);
}

void capture(struct cols* board, int x0, int y0, int x1, int y1, int xd, int yd) {
	int sizeatt = board[y0 * DIM + x0].size;
	int i;


	if (sizeatt != 3) {
		copy_pawns(&board[y0 * DIM + x0].tower[sizeatt], &board[y1 * DIM + x1].tower[0]);
		board[y0 * 7 + x0].size++;
	}

	/*se ci sono 4 pedine?
	copio fino alla terza pedina e poi elimino l'altra struct e la sostituisco con quella creata*/

	if (sizeatt > 3) {
		/*mi basta l'array, perchè con la free libero tower e non l'intera struct colonna*/
		pawns_t* tower = (pawns_t*)malloc(sizeof(pawns_t) * 3);
		for (int i = 0; i < 3; i++) {
			copy_pawns(&board[y0 * DIM + x0].tower[i], &tower[i]);
		}
		free(board[y0 * DIM + x0].tower);
		board[y0 * DIM + x0].size--;
		board[y0 * DIM + x0].tower = (pawns_t*)malloc(sizeof(pawns_t) * 3);
		//board[y0*7+x0].tower=tower;
		for (int i = 0; i < 3; i++) {
			copy_pawns(&tower[i], &board[y0 * DIM + x0].tower[i]);
		}
		free(tower);
	}


	board[y1 * DIM + x1].size--;
	if (board[y1 * DIM + x1].size != 0) {
		for (i = 0; i < board[y1 * DIM + x1].size; i++) {
			copy_pawns(&board[y1 * DIM + x1].tower[i], &board[y1 * DIM + x1].tower[i + 1]);
		}
	}
	else {
		free(board[y1 * DIM + x1].tower);
	}
	simple_shift(board, x0, y0, xd, yd);
	/*int sizeatt = board[y0 * DIM + x0].size;
	
	int i;

	if (sizeatt < 3) {
		copy_pawns(&board[y0 * DIM + x0].tower[sizeatt], &board[y1 * DIM + x1].tower[0]);
		board[y0 * DIM + x0].size++;
	}
	
	board[y1 * 7 + x1].size--;
	if (board[y1 * 7 + x1].size > 0) {
		for (i = 0; i < board[y1 * 7 + x1].size; i++) {
			copy_pawns(&board[y1 * 7 + x1].tower[i], &board[y1 * 7 + x1].tower[i + 1]); //la testa è andata via
		}
	}
	else {
		free(board[y1 * 7 + x1].tower);
	}
	simple_shift(board, x0, y0, xd, yd);*/
}

list_of_moves_t *destroy (list_of_moves_t* l){ 
	if(l->next!=NULL){						
		destroy(l->next);
	}
	if(l){free(l);}
	l=NULL;
	return l;
    /*if(l == NULL){	
		return l;					
	}

	destroy(l->next);
	free(l);*/	
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

list_of_moves_t *concat(list_of_moves_t* head, list_of_moves_t* new){ 
	list_of_moves_t* temp;
	if(head==NULL){
		return new;
	}else{
		temp=head;
		while(temp->next!=NULL) temp=temp->next;
		temp->next=new;
	}
	return head;
}

list_of_moves_t *insertion_simple_shift (list_of_moves_t* l, cols_t* board, int x0, int y0, int xd, int yd){
	list_of_moves_t *temp = (struct list_of_moves*) malloc(sizeof(struct list_of_moves));
	temp->board = (cols_t*) malloc(sizeof(cols_t) * 49);
    copy_board(board, temp->board);
	simple_shift (temp->board, x0, y0, xd, yd);
    temp->description = (char*) malloc(sizeof(char)*10 + 1);
	fill_description1(temp->description, x0, y0, xd, yd);
	temp->mandatory = 0;
	temp->next=NULL;
	l=concat(l,temp);
	return l;
}

list_of_moves_t *insertion_capture (list_of_moves_t* l, cols_t* board, int x0, int y0, int x1, int y1, int xd, int yd){
	list_of_moves_t *temp = (struct list_of_moves*) malloc(sizeof(struct list_of_moves));
    temp->board = (struct cols*) malloc(sizeof(struct cols)*49);
    copy_board(board, temp->board);
    capture(temp->board, x0, y0, x1, y1, xd, yd);
    temp->description = (char*) malloc(sizeof(char)*10 + 1);
	fill_description2(temp->description, x0, y0, x1, y1, xd, yd);
    temp->mandatory = 1;
	temp->next=NULL;
    l=concat(l,temp);
	return l;
}


list_of_moves_t* moves_analysis (list_of_moves_t* l, struct cols* board, int player){ 
	int i, j;
	char color, enemy_color;

	color = player;
	enemy_color = 1 - color;
	
	for(i = 0; i < DIM; i++){
		for(j = 0; j < DIM; j++){
			//ciclo di scan della board

			if(board[i*DIM+j].tower != NULL){ //controllo se c'è una pawns
				
				if(board[i*DIM+j].tower[0].color == color){ //se la pawns è del giocatore del turno corrente

					if(board[i*DIM+j].tower[0].rank == OFFICER || (board[i*DIM+j].tower[0].rank == SOLDIER && color == WHITE)){  //set mosse in giù
						if(i+1 < DIM){ 
							//caso spostamento semplice per i bianchi soldati
							if (j-1 >= 0){
								if(board[(i+1)*DIM + (j-1)].size == 0){
									l=insertion_simple_shift (l, board, j, i, j-1, i+1);	
								}else if(board[(i+1)*DIM + (j-1)].tower[0].color == enemy_color){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i+2 <= DIM && j-2 >= 0 && board[(i+2)*DIM + (j-2)].size == 0){ //se oltre il nemico è libero
										l=insertion_capture (l, board, j, i, j-1, i+1, j-2, i+2);			
									}
								}
							}
							if (j+1 < DIM){
								if(board[(i+1)*DIM + (j+1)].size == 0){
									l=insertion_simple_shift (l, board, j, i, j+1, i+1);    
								}else if(board[(i+1)*DIM + (j+1)].tower[0].color == enemy_color){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i+2 < DIM && j+2 < DIM && board[(i+2) * DIM + (j+2)].size == 0){ //se oltre il nemico è libero                                        
											l=insertion_capture (l, board, j, i, j+1, i+1, j+2, i+2);    												
									}
								}
							}
						}
					}
					 
					if(board[i*DIM+j].tower[0].rank==OFFICER || (board[i*DIM+j].tower[0].rank==SOLDIER && color==BLACK)){ //set di mosse in su
						if(i-1 >= 0){
							if(j-1 >= 0){
								//caso spostamento semplice per i neri soldati
								if(board[(i-1)*DIM + (j-1)].size == 0){
									l=insertion_simple_shift (l, board, j, i, j-1, i-1);
								}else if(board[(i-1)*DIM + (j-1)].tower[0].color == enemy_color){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i-2 >= 0 && j-2 >= 0 && board[(i-2)*DIM + (j-2)].size == 0){ //se oltre il nemico è libero
											l=insertion_capture (l, board, j, i, j-1, i-1, j-2, i-2);					
									}
								}
							}

							if(j+1 < DIM){
								if(board[(i-1)*DIM + (j+1)].size == 0){
									l=insertion_simple_shift (l, board, j, i, j+1, i-1);						
								}else if(board[(i-1)*DIM + (j+1)].tower[0].color == enemy_color){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i-2 >= 0 && j+2 < DIM && board[(i-2)*DIM + (j+2)].size == 0){ //se oltre il nemico è libero
										l=insertion_capture (l, board, j, i, j+1, i-1, j+2, i-2);									
		
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



list_of_moves_t* create(list_of_moves_t* l){
	list_of_moves_t* supp= (struct list_of_moves*) malloc(sizeof(struct list_of_moves));
	supp->description= l->description;
	supp->mandatory= l->mandatory;
	supp->board= l->board;
	supp->next= NULL;
	return supp;
}

list_of_moves_t* append(list_of_moves_t* l, list_of_moves_t* cell){
	list_of_moves_t* supp= create(cell);
	list_of_moves_t* n= l;
	if(l== NULL){
		return supp;
	}
	else{
		while(n->next!= NULL){
			n=n->next;
		}
		n->next= supp;
	}
	return l;
}

list_of_moves_t*  filter (list_of_moves_t* l){
	int flag = 0; //variabile bool che diventa 1 se e solo se ci sono mosse nella lista con obbligatorietà == 1
	list_of_moves_t* list = l;
	list_of_moves_t* tmp_head = NULL;
	list_of_moves_t* temp = NULL;
	if(l!=NULL){
		while (list!= NULL && !flag){
			if(list->mandatory == 1){
				flag=1;
			}
			list = list->next;
		}

		if(flag == 1){
			list = l;

			while (list!= NULL){
				if(list->mandatory == 1){
					temp=list->next;
					list->next=NULL;
					tmp_head=concat(tmp_head,list);
					list=temp;	
				}else{
					temp=list;
					list = list->next;
					free(temp);
				}
			}
			l=tmp_head;
		}
	}
	return l;






	/*int flag = 0; //variabile bool che diventa 1 se e solo se ci sono mosse nella list con obbligatorietà == 1
	list_of_moves_t* list = l;
	list_of_moves_t* supp= NULL;
	if(l!= NULL){
		while(list!= NULL){
			if(list->mandatory== 1){
				flag = 1;
				supp = append(supp, list);
			}
			list = list->next;
		}
	}
	if(flag == 1){
		l = destroy(l);
		return supp;
	} 
	return l;*/
}

void print_moves (list_of_moves_t* l, int* n) {
	int i = 1;
	while (l != NULL ) {
		printf("%2d) %s\n", i, l->description); //qua ho messo 2d così è tutto più ordinato
		i++;
		l = l->next;
	}

	*n = i - 1; //totale mosse
	/*i - 1 perchè abbiamo incrementato all'ultima iterazione senza sapere che era l'ultima*/
}

cols_t* extract (list_of_moves_t* l, int n){
	int i;

	for(i = 1; i < n; i++){ 
		l = l->next;
	}

	return l->board;
}



void check_and_do_promotion (struct cols* board, int to_print){ //possibilmente da scartare e da implementare in spostamento semplice
   	int j;													
	for (j = 0; j < DIM; j++){
    	if(board[6 * DIM + j].tower != NULL){
        
            if(board[6 * DIM + j].tower[0].color == WHITE && board[6 * DIM + j].tower[0].rank == SOLDIER){ //è bianco ed è soldato
                board[6 * DIM + j].tower[0].rank = OFFICER;
                board[6 * DIM + j].tower[0].name -= 32;
				if(to_print){
					printf("%c7 è stato promosso", j+65);
				}
                //to_print == 0 solo in minimax() che è ricorsiva
            }
        }
	}    

    
	for (j = 0; j < DIM; j++){
    	if(board[0+j].tower != NULL){   
            if(board[0+j].tower[0].color == 1  && board[0 * DIM + j].tower[0].rank == 0){ //è nero
                board[0+j].tower[0].rank = 1;
                board[0+j].tower[0].name -= 32;
				if(to_print){
					printf("%c1 è stato promosso", j+65);
				}
                //to_print == 0 solo in minimax() che è ricorsiva
            }
        }
	}    
}

/*funzione che crea la matrix di struct colonne*/
void matrix(cols_t* board) {
	int i, j;
	for(i = 0; i < DIM; i++){
		for(j = 0; j < DIM; j++){
			if(i <= 2){
				if((i * DIM+j) % 2 == 0){
					board[i*DIM+j].tower = (pawns_t*) malloc(sizeof(pawns_t) * 3);

					if (!board[i * DIM + j].tower) {
						printf("Errore di allocazione di memoria");
						exit(EXIT_FAILURE);
					}

					board[i*DIM+j].tower[0].color = WHITE;
					board[i*DIM+j].tower[0].name = 'w';
					board[i*DIM+j].tower[0].rank = SOLDIER;
					board[i*DIM+j].size = 1;
					      
				}else{
					board[i*DIM+j].tower = NULL;
					board[i*DIM+j].size = -1;
				}
			}else if(i == 3){                                                                     
				if((i*DIM+j) % 2 == 0){
					board[i*DIM+j].tower = (pawns_t*) malloc(sizeof(pawns_t) * 3);

					if (!board[i * DIM + j].tower) {
						printf("Errore di allocazione di memoria");
						exit(EXIT_FAILURE);
					}
					
					board[i*DIM+j].size = 0;
					
				}else{
					board[i*DIM+j].tower = NULL;
					board[i*DIM+j].size = -1;
					
				}
			}else{
				if((i*DIM+j) % 2 == 0){
					board[i*DIM+j].tower = (pawns_t*) malloc(sizeof(pawns_t) * 3);

					if (!board[i * DIM + j].tower) {
						printf("Errore di allocazione di memoria");
						exit(EXIT_FAILURE);
					}
					
					
					board[i*DIM+j].tower[0].color = BLACK;
					board[i*DIM+j].tower[0].name = 'b';
					board[i*DIM+j].tower[0].rank = SOLDIER;
					board[i*DIM+j].size = 1;
					
				}else{
					board[i*DIM+j].tower = NULL;
					board[i*DIM+j].size = -1;
					
				}
			}
		}
	}
}


void field(cols_t* board) {

	int numbers[7] = { 1,2,3,4,5,6,7 };
	char letters[9] = { '/','a','b','c','d','e','f','g','/' };
	int row, cell_row, cols, cell_cols, t;

    printf("\n");

	for(row = 0; row < 9; row++){
        for(t = 0; t < 55 ; t++) printf("-");
        printf("\n");
        if(row == 0 || row == 8){            
            for(cell_row = 0; cell_row < 5; cell_row++){
                for(cols = 0; cols < 9; cols++){
                    printf("|");
                    if(cell_row == 2){
                        for(cell_cols = 0; cell_cols < 5; cell_cols++){
                            printf("%c",letters[cols]);
                        }
                    }else{
                        for(cell_cols = 0; cell_cols < 5; cell_cols++){
                            printf(" ");
                        }
                    }

                }
                printf("|\n");
            }
        }else{
            for(cell_row = 0; cell_row < 5; cell_row++){
                printf("|  %d  |",numbers[row-1]);
                for(cols = 0; cols < DIM; cols++){
                    for(cell_cols = 0; cell_cols < 5; cell_cols++){
						if((row + cols) % 2 == 0){
                            printf("°");
                        }else{
							t = board[(row-1)*DIM+(cols)].size;
							if(cell_cols == 2 && cell_row > 0 && cell_row < 4 && t+cell_row > 3){
								printf("%c",board[(row-1)*DIM+(cols)].tower[t+cell_row-4].name);
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
	for(t = 0; t < 55; t++) printf("-");
	printf("\n");
}

int evaluation (cols_t* board){
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

int minimax(cols_t* board, int depth, int player){
	//i bianchi (0) massimizzano
	
	struct list_of_moves* l_og = NULL;
	struct list_of_moves* l = NULL;
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

char* extract_description (list_of_moves_t* l, int n){
	int i;

	for(i=1; i<n; i++){ 
		l=l->next;
	}

	return l->description;
}

int macro_ai (struct list_of_moves* l, int depth, int player){
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
			}

			pos++;
			l = l->next;
		}

		return res;
	}else{
		return 0;
	}

	
}

int main (){  
			
	cols_t* board = (cols_t*) malloc(sizeof(cols_t) * 49);
	struct list_of_moves* l = NULL;
	
	int n=0, choice=0;
	int player = PLAYER_1;
	int game_mode = 0;

	matrix(board);

	while(game_mode < 1 || game_mode > 2){
		printf("Vuoi giocare contro il COMPUTER (1) o contro un altro giocatore(2)? \n");
		printf("Digita la tua risposta. \n");
		scanf("%d", &game_mode);
	}
	

	if(game_mode == 1){//modalità COMPUTER
		while(player == PLAYER_1 || player == PLAYER_2){
			field(board);
			printf("PLAYER 1: AI, PLAYER 2: utente \n");
			printf("player = %d \n", player);
			printf("player %c\n",87-(player*21));

			l = moves_analysis(l, board, player);
			if(l == NULL){
				printf("GAME OVER: ha vinto %c\n",87-(!player*21));
				exit(EXIT_FAILURE);
			}
				
			l = filter(l);
				
			if(player == PLAYER_1){
				choice = macro_ai(l, DEPTH, player);
				copy_board(extract(l,choice),board);
				//to_print description mossa choice
				printf("W ha giocato %s\n",extract_description(l, choice));
			}else{ //se gioca l'utente
				/*to_print list mosse*/
				print_moves (l, &n);
				
				/*acquisisci ed esegui la mossa choice*/
				do{
					printf("Digita il numero della mossa che desideri eseguire.\n");
					scanf("%d", &choice);
				}while(choice > n || choice<1);

				copy_board(extract(l,choice),board);

			}
		
			l=destroy(l);
					
			/*eventuale promozione*/
			check_and_do_promotion (board, 1);

			/*cambio giocatore*/
			player= !player;
		}
		
	}else{ //per la condizione del while siamo sicuri che o è 1 o è 2
		while(player == PLAYER_1 || player == PLAYER_2){
			field(board);
			printf("player %c\n",87-(player*21));
			l = moves_analysis(l, board, player);
			if(l == NULL){
				printf("GAME OVER: ha vinto %c\n",87-(!player*21));
				exit(EXIT_FAILURE);
			}

			l = filter(l);

			/*to_print list mosse*/
			print_moves (l, &n);
				

			/*acquisisci ed esegui la mossa choice*/
			do{
				printf("Digita il numero della mossa che desideri eseguire.\n");
				scanf("%d", &choice);
			}while(choice > n || choice<1);

			copy_board(extract(l,choice),board);

			l=destroy(l);
			/*eventuale promozione*/
			check_and_do_promotion (board, 1);

					
			/*cambio giocatore*/
			player= !player;
		}
		
	}

	return 0;
}
