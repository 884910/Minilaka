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
#define YES 1
#define NO 0



char end_of_game = 0; //varrà 1 quando il gioco sarà finito


/*struct pedina*/
typedef
struct pedina {
	int colore;
	char nome;
	char stato;
} pedina_t;

/*struct colonna*/
typedef
struct colonna {
	int capacity;
	int size;
	//array di struct pedina
	struct pedina* torre;
}colonna_t;

typedef
struct lista_mosse{
    char obbligatorieta; //0=no, 1=si
    struct colonna* scacchiera; //qui inseriremo il nuovo possibile stato della scacchiera
    char* descrizione; //ex. A5 - C7 
    struct lista_mosse* next;
}lista_mosse_t;

void copia_pedina (pedina_t* p_dst, pedina_t* p_src){  
	p_dst->nome = p_src->nome;
	p_dst->stato = p_src->stato;
	p_dst->colore = p_src->colore;
}

void copia_scacchiera(colonna_t* src, colonna_t* dst){ 
	int i,j,k;											
	for(i=0;i<7;i++){									
		for(j=0;j<7;j++){
			if(src[i*DIM+j].torre!=NULL){
				dst[i*DIM+j].torre = (pedina_t*) malloc(sizeof(pedina_t) * 3);
				for(k=0;k<3;k++){
					copia_pedina(&(dst[i*DIM+j].torre[k]),&(src[i*DIM+j].torre[k]));
				}
				dst[i*DIM+j].size = src[i*DIM+j].size;
			}
		}
	}
}

void spostamento_semplice(struct colonna* scacchiera, int x0, int y0, int xd, int yd){ 
	int i;
	for (i=0; i<scacchiera[y0 * DIM +x0].size; i++){
		copia_pedina(&(scacchiera[yd * DIM + xd].torre[i]), &(scacchiera[y0 * DIM + x0].torre[i]));
	}
	
	scacchiera[yd * DIM + xd].size = scacchiera[y0*7+x0].size;
	scacchiera[y0 * DIM + x0].size=0;
	free(scacchiera[y0 * DIM + x0].torre);
}

void mangiata(struct colonna* scacchiera, int x0, int y0, int x1, int y1, int xd, int yd) {

	int sizeatt = scacchiera[y0 * 7 + x0].size;
	int sizedef = scacchiera[y1 * 7 + x1].size; // ottengo la size di chi viene mangiato
	int i;


	if (sizeatt != 3) {
		copia_pedina(&scacchiera[y0 * 7 + x0].torre[sizeatt], &scacchiera[y1 * 7 + x1].torre[0]);
		scacchiera[y0 * 7 + x0].size++;
	}

	/*se ci sono 4 pedine?
	copio fino alla terza pedina e poi elimino l'altra struct e la sostituisco con quella creata*/

	if (sizeatt > 3) {
		/*mi basta l'array, perchè con la free libero torre e non l'intera struct colonna*/
		pedina_t* torre = (struct pedina*)malloc(sizeof(pedina_t) * 3);
		for (int i = 0; i < 3; i++) {
			copia_pedina(&scacchiera[y0 * 7 + x0].torre[i], &torre[i]);
		}
		free(scacchiera[y0 * 7 + x0].torre);
		scacchiera[y0 * 7 + x0].size--;
		scacchiera[y0 * 7 + x0].torre = (pedina_t*)malloc(sizeof(pedina_t) * 3);
		//scacchiera[y0*7+x0].torre=torre;
		for (int i = 0; i < 3; i++) {
			copia_pedina(&torre[i], &scacchiera[y0 * 7 + x0].torre[i]);
		}
		free(torre);
	}


	scacchiera[y1 * 7 + x1].size--;
	if (scacchiera[y1 * 7 + x1].size != 0) {
		for (i = 0; i < scacchiera[y1 * 7 + x1].size; i++) {
			copia_pedina(&scacchiera[y1 * 7 + x1].torre[i], &scacchiera[y1 * 7 + x1].torre[i + 1]);
		}
	}
	else {
		free(scacchiera[y1 * 7 + x1].torre);
	}
	spostamento_semplice(scacchiera, x0, y0, xd, yd);
}

lista_mosse_t *elimina (lista_mosse_t* l){ 
    if(l->next!=NULL){						
		elimina(l->next);
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

lista_mosse_t *aggiungiCoda(lista_mosse_t* testa, lista_mosse_t* nuovo){ 
	lista_mosse_t* temp;
	if(testa==NULL){
		return nuovo;
	}else{
		temp=testa;
		while(temp->next!=NULL) temp=temp->next;
		temp->next=nuovo;
	}
	return testa;
}

lista_mosse_t *inserimento_spostamento_semplice (lista_mosse_t* l, colonna_t* scacchiera, int x0, int y0, int xd, int yd){
	lista_mosse_t *temp = (struct lista_mosse*) malloc(sizeof(struct lista_mosse));
	temp->scacchiera = (colonna_t*) malloc(sizeof(colonna_t) * 49);
    copia_scacchiera(scacchiera, temp->scacchiera);
	spostamento_semplice (temp->scacchiera, x0, y0, xd, yd);
    temp->descrizione = (char*) malloc(sizeof(char)*10 + 1);
	fill_description1(temp->descrizione, x0, y0, xd, yd);
	temp->obbligatorieta = 0;
	temp->next=NULL;
	l=aggiungiCoda(l,temp);
	return l;
}

lista_mosse_t *inserimento_mossa_mangiata (lista_mosse_t* l, colonna_t* scacchiera, int x0, int y0, int x1, int y1, int xd, int yd){
	lista_mosse_t *temp = (struct lista_mosse*) malloc(sizeof(struct lista_mosse));
    temp->scacchiera = (struct colonna*) malloc(sizeof(struct colonna)*49);
    copia_scacchiera(scacchiera, temp->scacchiera);
    mangiata(temp->scacchiera, x0, y0, x1, y1, xd, yd);
    temp->descrizione = (char*) malloc(sizeof(char)*10 + 1);
	fill_description2(temp->descrizione, x0, y0, x1, y1, xd, yd);
    temp->obbligatorieta = 1;
	temp->next=NULL;
    l=aggiungiCoda(l,temp);
	return l;
}


lista_mosse_t* analisi_mosse (lista_mosse_t* l, struct colonna* scacchiera, int turno){ 
	int i, j;
	char colore, colore_nemico;

	colore = turno;
	colore_nemico = 1 - colore;
	
	for(i=0;i<7;i++){
		for(j=0;j<7;j++){
			//ciclo di scan della scacchiera

			if(scacchiera[i*7+j].torre != NULL){ //controllo se c'è una pedina
				
				if(scacchiera[i*7+j].torre[0].colore == colore){ //se la pedina è del giocatore del turno corrente

					if(scacchiera[i*7+j].torre[0].stato == OFFICER || (scacchiera[i*7+j].torre[0].stato == SOLDIER && colore == WHITE)){  //set mosse in giù
						if(i+1 < 7){ 
							//caso spostamento semplice per i bianchi soldati
							if (j-1>=0){
								if(scacchiera[(i+1)*7 + (j-1)].size == 0){
									l=inserimento_spostamento_semplice (l, scacchiera, j, i, j-1, i+1);	
								}else if(scacchiera[(i+1)*7 + (j-1)].torre[0].colore == colore_nemico){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i+2<=7 && j-2>=0 && scacchiera[(i+2)*7 + (j-2)].size == 0){ //se oltre il nemico è libero
										l=inserimento_mossa_mangiata (l, scacchiera, j, i, j-1, i+1, j-2, i+2);			
									}
								}
							}
							if (j+1<7){
								if(scacchiera[(i+1)*7 + (j+1)].size == 0){
									l=inserimento_spostamento_semplice (l, scacchiera, j, i, j+1, i+1);    
								}else if(scacchiera[(i+1)*7 + (j+1)].torre[0].colore == colore_nemico){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i+2<7 && j+2<7 && scacchiera[(i+2)*7 + (j+2)].size == 0){ //se oltre il nemico è libero                                        
											l=inserimento_mossa_mangiata (l, scacchiera, j, i, j+1, i+1, j+2, i+2);    												
									}
								}
							}
						}
					}
					 
					if(scacchiera[i*7+j].torre[0].stato==OFFICER || (scacchiera[i*7+j].torre[0].stato==SOLDIER && colore==BLACK)){ //set di mosse in su
						if(i-1 >= 0){
							if(j-1 >= 0){
								//caso spostamento semplice per i neri soldati
								if(scacchiera[(i-1)*7 + (j-1)].size == 0){
									l=inserimento_spostamento_semplice (l, scacchiera, j, i, j-1, i-1);
								}else if(scacchiera[(i-1)*7 + (j-1)].torre[0].colore == colore_nemico){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i-2 >= 0 && j-2 >= 0 && scacchiera[(i-2)*7 + (j-2)].size == 0){ //se oltre il nemico è libero
											l=inserimento_mossa_mangiata (l, scacchiera, j, i, j-1, i-1, j-2, i-2);					
									}
								}
							}

							if(j+1 < 7){
								if(scacchiera[(i-1)*7 + (j+1)].size == 0){
									l=inserimento_spostamento_semplice (l, scacchiera, j, i, j+1, i-1);						
								}else if(scacchiera[(i-1)*7 + (j+1)].torre[0].colore == colore_nemico){ //la casella è occupata da qualcuno che potrei mangiare?
									if (i-2 >= 0 && j+2 < 7 && scacchiera[(i-2)*7 + (j+2)].size == 0){ //se oltre il nemico è libero
										l=inserimento_mossa_mangiata (l, scacchiera, j, i, j+1, i-1, j+2, i-2);									
		
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


lista_mosse_t*  filtrare (lista_mosse_t* l){
	int flag = 0; //variabile bool che diventa 1 se e solo se ci sono mosse nella lista con obbligatorietà == 1
	lista_mosse_t* lista = l;
	lista_mosse_t* tmp_head = NULL;
	lista_mosse_t* temp = NULL;
	if(l!=NULL){
		while (lista!= NULL && !flag){
			if(lista->obbligatorieta == 1){
				flag=1;
			}
			lista=lista->next;
		}

		if(flag == 1){
			lista = l;

			while (lista!= NULL){
				if(lista->obbligatorieta == 1){
					temp=lista->next;
					lista->next=NULL;
					tmp_head=aggiungiCoda(tmp_head,lista);
					lista=temp;	
				}else{
					temp=lista;
					lista=lista->next;
					free(temp);
				}
			}
			l=tmp_head;
		}
	}
	return l;
}

void stampa_mosse(lista_mosse_t* l, int* n) {
	int i = 1;
	while (l != NULL ) {
		printf("%2d) %s\n", i,l->descrizione); //qua ho messo 2d così è tutto più ordinato
		i++;
		l = l->next;
	}

	*n = i - 1; //totale mosse
	/*i - 1 perchè abbiamo incrementato all'ultima iterazione senza sapere che era l'ultima*/
}

colonna_t* estrai (lista_mosse_t* l, int n){
	int i;

	for(i=1; i<n; i++){ 
		l=l->next;
	}

	return l->scacchiera;
}



void check_and_do_promotion (struct colonna* scacchiera, int stampa){ //possibilmente da scartare e da implementare in spostamento semplice
   	int j;													
	for (j=0; j<7; j++){
    	if(scacchiera[6*7 + j].torre != NULL){
        
            if(scacchiera[6*7 + j].torre[0].colore == WHITE && scacchiera[6*7 + j].torre[0].stato == SOLDIER){ //è bianco ed è soldato
                scacchiera[6*7 + j].torre[0].stato = OFFICER;
                scacchiera[6*7 + j].torre[0].nome -= 32;
				if(stampa){
					printf("%c7 è stato promosso", j+65);
				}
                //stampa == 0 solo in minimax() che è ricorsiva
            }
        }
	}    

    
	for (j=0; j<7; j++){
    	if(scacchiera[0+j].torre != NULL){   
            if(scacchiera[0+j].torre[0].colore == 1  && scacchiera[0*7 + j].torre[0].stato == 0){ //è nero
                scacchiera[0+j].torre[0].stato = 1;
                scacchiera[0+j].torre[0].nome -= 32;
				if(stampa){
					printf("%c1 è stato promosso", j+65);
				}
                //stampa == 0 solo in minimax() che è ricorsiva
            }
        }
	}    
}

/*funzione che crea la matrice di struct colonne*/
void matrice(colonna_t* chessboard) {
	int i, j;
	for(i=0; i<DIM; i++){
		for(j=0; j<DIM; j++){
			if(i<=2){
				if((i*7+j) % 2 == 0){
					chessboard[i*DIM+j].torre = (pedina_t*) malloc(sizeof(pedina_t) * 3);

					if (!chessboard[i * DIM + j].torre) {
						printf("Errore di allocazione di memoria");
						exit(EXIT_FAILURE);
					}

					chessboard[i*DIM+j].torre[0].colore = WHITE;
					chessboard[i*DIM+j].torre[0].nome = 'w';
					chessboard[i*DIM+j].torre[0].stato = SOLDIER;
					chessboard[i*DIM+j].size = 1;
					chessboard[i*DIM+j].capacity = 3;      //capacity non serve a una ceppa
				}else{
					chessboard[i*DIM+j].torre = NULL;
					chessboard[i*DIM+j].size = -1;
					chessboard[i*DIM+j].capacity = 0;
				}
			}else if(i == 3){                                                                     
				if((i*7+j) % 2 == 0){
					chessboard[i*DIM+j].torre = (pedina_t*) malloc(sizeof(pedina_t) * 3);

					if (!chessboard[i * DIM + j].torre) {
						printf("Errore di allocazione di memoria");
						exit(EXIT_FAILURE);
					}
					
					chessboard[i*DIM+j].size = 0;
					chessboard[i*DIM+j].capacity = 3;
				}else{
					chessboard[i*DIM+j].torre = NULL;
					chessboard[i*DIM+j].size = -1;
					chessboard[i*DIM+j].capacity = 0;
				}
			}else{
				if((i*7+j) % 2 == 0){
					chessboard[i*DIM+j].torre = (pedina_t*) malloc(sizeof(pedina_t) * 3);

					if (!chessboard[i * DIM + j].torre) {
						printf("Errore di allocazione di memoria");
						exit(EXIT_FAILURE);
					}
					
					
					chessboard[i*DIM+j].torre[0].colore = BLACK;
					chessboard[i*DIM+j].torre[0].nome = 'b';
					chessboard[i*DIM+j].torre[0].stato = SOLDIER;
					chessboard[i*DIM+j].size = 1;
					chessboard[i*DIM+j].capacity = 3;
				}else{
					chessboard[i*DIM+j].torre = NULL;
					chessboard[i*DIM+j].size = -1;
					chessboard[i*DIM+j].capacity = 0;
				}
			}
		}
	}
}


void campo(int lato, int dim, colonna_t* partita) {

	int numeri[7] = { 1,2,3,4,5,6,7 };
	char lettere[9] = { '/','a','b','c','d','e','f','g','/' };
	int riga,riga_casella,colonna,colonna_casella,t;
    printf("\n");
	for(riga=0;riga<9;riga++){
        for(t=0;t<55;t++)printf("-");
        printf("\n");
        if(riga==0 || riga==8){            
            for(riga_casella=0;riga_casella<5;riga_casella++){
                for(colonna=0;colonna<9;colonna++){
                    printf("|");
                    if(riga_casella==2){
                        for(colonna_casella=0;colonna_casella<5;colonna_casella++){
                            printf("%c",lettere[colonna]);
                        }
                    }else{
                        for(colonna_casella=0;colonna_casella<5;colonna_casella++){
                            printf(" ");
                        }
                    }

                }
                printf("|\n");
            }
        }else{
            for(riga_casella=0;riga_casella<5;riga_casella++){
                printf("|  %d  |",numeri[riga-1]);
                for(colonna=0;colonna<7;colonna++){
                    for(colonna_casella=0;colonna_casella<5;colonna_casella++){
						if((riga+colonna)%2==0){
                            printf("°");
                        }else{
							t=partita[(riga-1)*7+(colonna)].size;
							if(colonna_casella==2 && riga_casella>0 && riga_casella<4 && t+riga_casella>3){
								printf("%c",partita[(riga-1)*7+(colonna)].torre[t+riga_casella-4].nome);
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

int evaluation (colonna_t* scacchiera){
	int i = 0;
	int j = 0;
	int counter = 0;
	
	for(i = 0; i < DIM; i++){
		for(j = 0; j < DIM; j++){
			if(scacchiera[i*DIM + j].size){
				if(scacchiera[i*DIM + j].torre[0].colore == BLACK){
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

int minimax(colonna_t* scacchiera, int depth, int player){
	//i bianchi (0) massimizzano
	
	struct lista_mosse* l_og = NULL;
	struct lista_mosse* l = NULL;
	l_og = analisi_mosse(l_og, scacchiera, player);
	l_og = filtrare(l_og);
	
	if(depth == 0 || l_og == NULL){
		return evaluation(scacchiera);
	}

	if(player == WHITE){
		int maxEval = INT_MIN;
		int eval = 0;
		l = l_og;
		while(l){
			check_and_do_promotion (l->scacchiera, 0);
			eval = minimax(l->scacchiera, depth - 1, !player);
			maxEval = max(maxEval, eval);
			l = l->next;
		}
		l_og = elimina(l_og);
		return maxEval;

	}else{
		int minEval = INT_MAX;
		int eval = 0;
		l = l_og;
		while(l){
			check_and_do_promotion (l->scacchiera, 0);
			eval = minimax(l->scacchiera, depth -1, !player);
			minEval = min(minEval, eval);
			l = l->next;
		}

		l_og = elimina(l_og);
		return minEval;
	}

}

char* estrai_descrizione (lista_mosse_t* l, int n){
	int i;

	for(i=1; i<n; i++){ 
		l=l->next;
	}

	return l->descrizione;
}

int macro_ai (colonna_t* scacchiera, struct lista_mosse* l, int depth, int player){
	//i neri minimizzano e i bianchi massimizzano
	int max = INT_MIN; 
	int pos = 0;
	int  res = 0;
	if(l){
		while(l){
			int new = minimax(l->scacchiera, depth, player);
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
			
	colonna_t* scacchiera = (colonna_t*) malloc(sizeof(colonna_t) * 49);
	struct lista_mosse* l = NULL;
	
	int n=0, scelta=0;
	int player = PLAYER_1;
	int modalita_gioco = 0;

	matrice(scacchiera);

	while(modalita_gioco < 1 || modalita_gioco > 2){
		printf("Vuoi giocare contro il COMPUTER (1) o contro un altro giocatore(2)? \n");
		printf("Digita la tua risposta. \n");
		scanf("%d", &modalita_gioco);
	}
	

	if(modalita_gioco == 1){//modalità COMPUTER
		while(player == PLAYER_1 || player == PLAYER_2){
			campo(9,5,scacchiera);
			printf("PLAYER 1: AI, PLAYER 2: utente \n");
			printf("player = %d \n", player);
			printf("turno %c\n",87-(player*21));

			l = analisi_mosse(l, scacchiera, player);
			if(l == NULL){
				printf("GAME OVER: ha vinto %c\n",87-(!player*21));
				exit(EXIT_FAILURE);
			}
				
			l = filtrare(l);
				
			if(player == PLAYER_1){
				scelta = macro_ai(scacchiera, l, DEPTH, player);
				copia_scacchiera(estrai(l,scelta),scacchiera);
				//stampa descrizione mossa scelta
				printf("W ha giocato %s\n",estrai_descrizione(l, scelta));
			}else{ //se gioca l'utente
				/*stampa lista mosse*/
				stampa_mosse(l, &n);
				
				/*acquisisci ed esegui la mossa scelta*/
				do{
					printf("Digita il numero della mossa che desideri eseguire.\n");
					scanf("%d", &scelta);
				}while(scelta > n || scelta<1);

				copia_scacchiera(estrai(l,scelta),scacchiera);

			}
		
			l=elimina(l);
					
			/*eventuale promozione*/
			check_and_do_promotion (scacchiera, 1);

			/*cambio giocatore*/
			player= !player;
		}
		
	}else{ //per la condizione del while siamo sicuri che o è 1 o è 2
		while(player == PLAYER_1 || player == PLAYER_2){
			campo(9,5,scacchiera);
			printf("turno %c\n",87-(player*21));
			l = analisi_mosse(l, scacchiera, player);
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

			copia_scacchiera(estrai(l,scelta),scacchiera);

			l=elimina(l);
			/*eventuale promozione*/
			check_and_do_promotion (scacchiera, 1);

					
			/*cambio giocatore*/
			player= !player;
		}
		
	}

	return 0;
}
