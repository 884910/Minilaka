
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
				if(dst[i*DIM+j].tower == NULL){
					dst[i*DIM+j].tower = (pawns_t*) malloc(sizeof(pawns_t) * 3);
				}
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
	
	board[yd * DIM + xd].size = board[y0*7+x0].size;
	board[y0 * DIM + x0].size=0;
	free(board[y0 * DIM + x0].tower);
}

void capture(struct cols* board, int x0, int y0, int x1, int y1, int xd, int yd) {

	int sizeatt = board[y0 * DIM + x0].size;
	int sizedef = board[y1 * DIM + x1].size; // ottengo la size di chi viene mangiato
	int i;

	
	if (sizeatt != 3) {
		copy_pawns(&board[y0 * DIM + x0].tower[sizeatt], &board[y1 * DIM + x1].tower[0]);
		board[y0 * DIM + x0].size++;
	}

	/*se ci sono 4 copy_board pedine?
	copio fino alla terza pawns e poi elimino l'altra struct e la sostituisco con quella creata*/

	if (sizeatt > 3) {
		/*mi basta l'array, perchè con la free libero tower e non l'intera struct cols*/
		pawns_t* tower = (struct pawns*)malloc(sizeof(pawns_t) * 3);
		for (int i = 0; i < 3; i++) {
			copy_pawns(&board[y0 * 7 + x0].tower[i], &tower[i]);
		}
		free(board[y0 * 7 + x0].tower);
		board[y0 * 7 + x0].size--;
		board[y0 * 7 + x0].tower = (pawns_t*)malloc(sizeof(pawns_t) * 3);
		//board[y0*7+x0].tower=tower;
		for (int i = 0; i < 3; i++) {
			copy_pawns(&tower[i], &board[y0 * 7 + x0].tower[i]);
		}
		free(tower);
	}


	board[y1 * 7 + x1].size--;
	if (board[y1 * 7 + x1].size != 0) {
		for (i = 0; i < board[y1 * 7 + x1].size; i++) {
			copy_pawns(&board[y1 * 7 + x1].tower[i], &board[y1 * 7 + x1].tower[i + 1]);
		}
	}
	else {
		free(board[y1 * 7 + x1].tower);
	}
	simple_shift(board, x0, y0, xd, yd);
}

list_of_moves_t *destroy (list_of_moves_t* l){ 
    if(l->next!=NULL){						
		destroy(l->next);
	}
	if(l){free(l);}
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

list_of_moves_t *inserimento_simple_shift (list_of_moves_t* l, cols_t* board, int x0, int y0, int xd, int yd){
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

list_of_moves_t *inserimento_mossa_capture (list_of_moves_t* l, cols_t* board, int x0, int y0, int x1, int y1, int xd, int yd){
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


list_of_moves_t* analisi_mosse (list_of_moves_t* l, struct cols* board, int turno){ 
	int i, j;
	char color, color_nemico;

	color = turno;
	color_nemico = 1 - color;
	
	for(i=0;i<7;i++){
		for(j=0;j<7;j++){
			//ciclo di scan della board

			if(board[i*7+j].tower != NULL){ //controllo se c'è una pawns
				
				if(board[i*7+j].tower[0].color == color){ //se la pawns è del giocatore del turno corrente

					if(board[i*7+j].tower[0].rank == OFFICER || (board[i*7+j].tower[0].rank == SOLDIER && color == WHITE)){  //set mosse in giù
						if(i+1 < 7){ 
							//caso spostamento semplice per i bianchi soldati
							if (j-1>=0){
								if(board[(i+1)*7 + (j-1)].size == 0){
									l=inserimento_simple_shift (l, board, j, i, j-1, i+1);	
								}else if(board[(i+1)*7 + (j-1)].tower[0].color == color_nemico){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i+2<=7 && j-2>=0 && board[(i+2)*7 + (j-2)].size == 0){ //se oltre il nemico è libero
										l=inserimento_mossa_capture (l, board, j, i, j-1, i+1, j-2, i+2);			
									}
								}
							}
							if (j+1<7){
								if(board[(i+1)*7 + (j+1)].size == 0){
									l=inserimento_simple_shift (l, board, j, i, j+1, i+1);    
								}else if(board[(i+1)*7 + (j+1)].tower[0].color == color_nemico){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i+2<7 && j+2<7 && board[(i+2)*7 + (j+2)].size == 0){ //se oltre il nemico è libero                                        
											l=inserimento_mossa_capture (l, board, j, i, j+1, i+1, j+2, i+2);    												
									}
								}
							}
						}
					}
					 
					if(board[i*7+j].tower[0].rank==OFFICER || (board[i*7+j].tower[0].rank==SOLDIER && color==BLACK)){ //set di mosse in su
						if(i-1 >= 0){
							if(j-1 >= 0){
								//caso spostamento semplice per i neri soldati
								if(board[(i-1)*7 + (j-1)].size == 0){
									l=inserimento_simple_shift (l, board, j, i, j-1, i-1);
								}else if(board[(i-1)*7 + (j-1)].tower[0].color == color_nemico){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i-2 >= 0 && j-2 >= 0 && board[(i-2)*7 + (j-2)].size == 0){ //se oltre il nemico è libero
											l=inserimento_mossa_capture (l, board, j, i, j-1, i-1, j-2, i-2);					
									}
								}
							}

							if(j+1 < 7){
								if(board[(i-1)*7 + (j+1)].size == 0){
									l=inserimento_simple_shift (l, board, j, i, j+1, i-1);						
								}else if(board[(i-1)*7 + (j+1)].tower[0].color == color_nemico){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i-2 >= 0 && j+2 < 7 && board[(i-2)*7 + (j+2)].size == 0){ //se oltre il nemico è libero
										l=inserimento_mossa_capture (l, board, j, i, j+1, i-1, j+2, i-2);									
		
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

list_of_moves_t* append(list_of_moves_t* l, list_of_moves_t* cella){
	list_of_moves_t* supp= create(cella);
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

list_of_moves_t*  filtrare (list_of_moves_t* l){
	int flag = 0; //variabile bool che diventa 1 se e solo se ci sono mosse nella lista con obbligatorietà == 1
	list_of_moves_t* lista = l;
	list_of_moves_t* supp= NULL;
	if(l!= NULL){
		while(lista!= NULL){
			if(lista->mandatory== 1){
				flag=1;
				supp= append(supp, lista);
			}
			lista= lista->next;
		}
	}
	if(flag==1) return supp;
	return l;
}

void stampa_mosse(list_of_moves_t* l, int* n) {
	int i = 1;
	while (l != NULL ) {
		printf("%2d) %s\n", i,l->description); //qua ho messo 2d così è tutto più ordinato
		i++;
		l = l->next;
	}

	*n = i - 1; //totale mosse
	/*i - 1 perchè abbiamo incrementato all'ultima iterazione senza sapere che era l'ultima*/
}

cols_t* estrai (list_of_moves_t* l, int n){
	int i;

	for(i=1; i<n; i++){ 
		l=l->next;
	}

	return l->board;
}



void check_and_do_promotion (struct cols* board, int stampa){ //possibilmente da scartare e da implementare in spostamento semplice
   	int j;													
	for (j=0; j<7; j++){
    	if(board[6*7 + j].tower != NULL){
        
            if(board[6*7 + j].tower[0].color == WHITE && board[6*7 + j].tower[0].rank == SOLDIER){ //è bianco ed è soldato
                board[6*7 + j].tower[0].rank = OFFICER;
                board[6*7 + j].tower[0].name -= 32;
				if(stampa){
					printf("%c7 è rank promosso", j+65);
				}
                //stampa == 0 solo in minimax() che è ricorsiva
            }
        }
	}    

    
	for (j=0; j<7; j++){
    	if(board[0+j].tower != NULL){   
            if(board[0+j].tower[0].color == 1  && board[0*7 + j].tower[0].rank == 0){ //è nero
                board[0+j].tower[0].rank = 1;
                board[0+j].tower[0].name -= 32;
				if(stampa){
					printf("%c1 è rank promosso", j+65);
				}
                //stampa == 0 solo in minimax() che è ricorsiva
            }
        }
	}    
}

/*funzione che crea la matrice di struct colonne*/
void matrice(cols_t* chessboard) {
	int i, j;
	for(i=0; i<DIM; i++){
		for(j=0; j<DIM; j++){
			if(i<=2){
				if((i*7+j) % 2 == 0){
					chessboard[i*DIM+j].tower = (pawns_t*) malloc(sizeof(pawns_t) * 3);

					if (!chessboard[i * DIM + j].tower) {
						printf("Errore di allocazione di memoria");
						exit(EXIT_FAILURE);
					}

					chessboard[i*DIM+j].tower[0].color = WHITE;
					chessboard[i*DIM+j].tower[0].name = 'w';
					chessboard[i*DIM+j].tower[0].rank = SOLDIER;
					chessboard[i*DIM+j].size = 1;
					      
				}else{
					chessboard[i*DIM+j].tower = NULL;
					chessboard[i*DIM+j].size = -1;
				}
			}else if(i == 3){                                                                     
				if((i*7+j) % 2 == 0){
					chessboard[i*DIM+j].tower = (pawns_t*) malloc(sizeof(pawns_t) * 3);

					if (!chessboard[i * DIM + j].tower) {
						printf("Errore di allocazione di memoria");
						exit(EXIT_FAILURE);
					}
					
					chessboard[i*DIM+j].size = 0;
					
				}else{
					chessboard[i*DIM+j].tower = NULL;
					chessboard[i*DIM+j].size = -1;
					
				}
			}else{
				if((i*7+j) % 2 == 0){
					chessboard[i*DIM+j].tower = (pawns_t*) malloc(sizeof(pawns_t) * 3);

					if (!chessboard[i * DIM + j].tower) {
						printf("Errore di allocazione di memoria");
						exit(EXIT_FAILURE);
					}
					
					
					chessboard[i*DIM+j].tower[0].color = BLACK;
					chessboard[i*DIM+j].tower[0].name = 'b';
					chessboard[i*DIM+j].tower[0].rank = SOLDIER;
					chessboard[i*DIM+j].size = 1;
					
				}else{
					chessboard[i*DIM+j].tower = NULL;
					chessboard[i*DIM+j].size = -1;
					
				}
			}
		}
	}
}


void campo(int lato, int dim, cols_t* partita) {

	int numeri[7] = { 1,2,3,4,5,6,7 };
	char lettere[9] = { '/','a','b','c','d','e','f','g','/' };
	int riga,riga_casella,cols,cols_casella,t;
    printf("\n");
	for(riga=0;riga<9;riga++){
        for(t=0;t<55;t++)printf("-");
        printf("\n");
        if(riga==0 || riga==8){            
            for(riga_casella=0;riga_casella<5;riga_casella++){
                for(cols=0;cols<9;cols++){
                    printf("|");
                    if(riga_casella==2){
                        for(cols_casella=0;cols_casella<5;cols_casella++){
                            printf("%c",lettere[cols]);
                        }
                    }else{
                        for(cols_casella=0;cols_casella<5;cols_casella++){
                            printf(" ");
                        }
                    }

                }
                printf("|\n");
            }
        }else{
            for(riga_casella=0;riga_casella<5;riga_casella++){
                printf("|  %d  |",numeri[riga-1]);
                for(cols=0;cols<7;cols++){
                    for(cols_casella=0;cols_casella<5;cols_casella++){
						if((riga+cols)%2==0){
                            printf("°");
                        }else{
							t=partita[(riga-1)*7+(cols)].size;
							if(cols_casella==2 && riga_casella>0 && riga_casella<4 && t+riga_casella>3){
								printf("%c",partita[(riga-1)*7+(cols)].tower[t+riga_casella-4].name);
							}else{
								printf(" ");
							}
						}
                    }
                    printf("|");
                }
                printf("  %d  |\n",numeri[riga-1]);
            }
        }
	}
	for(t=0;t<55;t++)printf("-");
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
	l_og = analisi_mosse(l_og, board, player);
	l_og = filtrare(l_og);
	
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

char* estrai_description (list_of_moves_t* l, int n){
	int i;

	for(i=1; i<n; i++){ 
		l=l->next;
	}

	return l->description;
}

int macro_ai (cols_t* board, struct list_of_moves* l, int depth, int player){
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
	
	int n=0, scelta=0;
	int player = PLAYER_1;
	int modalita_gioco = 0;

	matrice(board);

	while(modalita_gioco < 1 || modalita_gioco > 2){
		printf("Vuoi giocare contro il COMPUTER (1) o contro un altro giocatore(2)? \n");
		printf("Digita la tua risposta. \n");
		scanf("%d", &modalita_gioco);
	}
	

	if(modalita_gioco == 1){//modalità COMPUTER
		while(player == PLAYER_1 || player == PLAYER_2){
			campo(9,5,board);
			printf("PLAYER 1: AI, PLAYER 2: utente \n");
			printf("player = %d \n", player);
			printf("turno %c\n",87-(player*21));

			l = analisi_mosse(l, board, player);
			if(l == NULL){
				printf("GAME OVER: ha vinto %c\n",87-(!player*21));
				exit(EXIT_FAILURE);
			}
				
			l = filtrare(l);
				
			if(player == PLAYER_1){
				scelta = macro_ai(board, l, DEPTH, player);
				copy_board(estrai(l,scelta),board);
				//stampa description mossa scelta
				printf("W ha giocato %s\n",estrai_description(l, scelta));
			}else{ //se gioca l'utente
				/*stampa lista mosse*/
				stampa_mosse(l, &n);
				
				/*acquisisci ed esegui la mossa scelta*/
				do{
					printf("Digita il numero della mossa che desideri eseguire.\n");
					scanf("%d", &scelta);
				}while(scelta > n || scelta<1);

				copy_board(estrai(l,scelta),board);

			}
		
			l=destroy(l);
					
			/*eventuale promozione*/
			check_and_do_promotion (board, 1);

			/*cambio giocatore*/
			player= !player;
		}
		
	}else{ //per la condizione del while siamo sicuri che o è 1 o è 2
		while(player == PLAYER_1 || player == PLAYER_2){
			campo(9,5,board);
			printf("turno %c\n",87-(player*21));
			l = analisi_mosse(l, board, player);
			if(l == NULL){
				printf("GAME OVER: ha vinto %c\n",87-(!player*21));
				exit(EXIT_FAILURE);
			}

			l = filtrare(l);

			/*stampa lista mosse*/
			stampa_mosse(l, &n);
				

			/*acquisisci ed esegui la mossa scelta*/
			do{
				printf("Digita il numero della mossa che desideri eseguire.\n");
				scanf("%d", &scelta);
			}while(scelta > n || scelta<1);

			copy_board(estrai(l,scelta),board);

			l=destroy(l);
			/*eventuale promozione*/
			check_and_do_promotion (board, 1);

					
			/*cambio giocatore*/
			player= !player;
		}
		
	}

	return 0;
}
