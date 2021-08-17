/**
 * @file main.c
 * @authors Andrea Giacomello, Carola Pessotto, Serena Zanon
 * @mainpage Minilaska
 * @section intro Introduzione
 * 
 * Presentiamo un'implementazione di mini-Laska, ovvero una variante del gioco originale http://www.lasca.org/ . \n
 * Rispetto al gioco originale miniLaska prevede le seguenti limitazioni: \n
 *  • si può mangiare/conquistare una sola volta per mossa \n
 *  • le torri possono essere alte al massimo 3 pedine, superato questo limite, la pedina più in basso viene rimossa dalla scacchiera \n
 **/
#include "ai.h"
int main (){  
			
	col_t* board = (col_t*) malloc(sizeof(col_t) * 49);
	moves_list_t* l = NULL;
	
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
				copy_board(estract(l,choice),board);
				//to_print description mossa choice
				printf("W ha giocato %s\n",estract_description(l, choice));
			}else{ //se gioca l'utente
				/*to_print list mosse*/
				print_moves(l, &n);
				
				/*acquisisci ed esegui la mossa choice*/
				do{
					printf("Digita il numero della mossa che desideri eseguire.\n");
					scanf("%d", &choice);
				}while(choice > n || choice<1);

				copy_board(estract(l,choice),board);

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
			print_moves(l, &n);
				

			/*acquisisci ed esegui la mossa choice*/
			do{
				printf("Digita il numero della mossa che desideri eseguire.\n");
				scanf("%d", &choice);
			}while(choice > n || choice<1);

			copy_board(estract(l,choice),board);

			l=destroy(l);
			/*eventuale promozione*/
			check_and_do_promotion (board, 1);

					
			/*cambio giocatore*/
			player= !player;
		}
		
	}

	return 0;
}
