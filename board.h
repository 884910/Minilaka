/** @file board.h */

#include "struct.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief copy_pawn copia i valori dei campi della pedina p_src all'interno dei campi della pedina p_dst
 * @param p_dst  struct pedina di destinazione
 * @param p_src  struct pedina da copiare
 */
void copy_pawn (pawn_t* p_dst, pawn_t* p_src);

/**
 * @brief copy_board copia ogni pedina della scacchiera
 * @details copy_board "tocca" ogni pedina presente sulla scacchiera e chiama la funzione copy_pawn per ogni pedina da copiare
 * @param src scacchiera da copiare
 * @param dst scacchiera di destinazione
 */
void copy_board(col_t* src, col_t* dst);

/**
 * @brief check_and_do_promotion promuove una pedina da SOLDIER a OFFICER se si verificano le condizioni necessarie
 * @details controlla se nella prima e nell'ultima riga sono presenti rispettivamente pedine nere e pedine bianche, in caso affermativo ne cambia lo stato e il nome 
 * @param board scacchiera da controllare 
 * @param to_print vale 1 se si vuole stampare un messaggio di avviso e vale 0 altrimenti (nella funzione ricorsiva minimax() to_print è posta pari a 0)
 */
void check_and_do_promotion (col_t* board, int to_print);

/*funzione che crea la matrix di struct colonne*/
/**
 * @brief matrix crea la scacchiera iniziale su cui ha inizio la partita
 * @details Inizializza correttamente ogni casella della scacchiera allocando dinamicamente lo spazio in memoria necessario
 * @param board matrice di 7x7 col_t da inizilizzare a scacchiera
 */
void matrix(col_t* board);

/**
 * @brief field stampa la scacchiera passata in entrata
 * @details field è chiamata dopo ogni mossa per mostrare il campo di gioco aggiornato
 * @param board scacchiera da stampare
 */
void field(col_t* board);

