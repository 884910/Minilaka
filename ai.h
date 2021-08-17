/** @file ai.h**/

#include "moves_analysis.h"
#include <limits.h>
#include <math.h>

/**
 * @brief evaluation conta le pedine in cima ad una torre e le pedine singole presenti sulla scacchiera, distinguendole in base al colore (WHITE/BLACK), per determinare quale dei due giocatori è in vantaggio
 * @details in particolare viene utilizzata una variabile counter che viene incrementata per ogni pedina WHITE trovata e decrementata per ogni pedina BLACK trovata
 * @param board scacchiera da valutare
 * @return ritorna counter, variabile che ha valore positivo se il vantaggio appartiene al COMPUTER altrimenti, se il vantaggio appartiene all' UTENTE, ha valore negativo 
 */
int evaluation (col_t* board);

/**
 * @brief max determina il massimo tra due numeri
 * @param n1 primo dei due numeri passati in ingresso
 * @param n2 secondo dei due numeri passati in ingresso
 * @return ritorna il massimo tra i due numeri passati in ingresso
 */
int max(int n1, int n2);

/**
 * @brief min determina il minimo tra due numeri
 * @param n1 primo dei due numeri passati in ingresso
 * @param n2 secondo dei due numeri passati in ingresso
 * @return ritorna il minimo tra i due numeri passati in ingresso
 */
int min(int n1, int n2);

/**
 * @brief minimax implementa ricorsivamente l'omonimo algoritmo 
 * @param board scacchiera da cui valutare il possibile vantaggio ottenibile a partire da una certa mossa
 * @param depth profondità a cui arrivano le chiamate ricorsive
 * @param player parametro che indica se è il turno del COMPUTER o dell'UTENTE
 * @return ritorna il massimo valore ottenibile (nel caso sia il turno di WHITE) o il minimo valore ottenibile (nel caso sia il turno di BLACK)
 */
int minimax(col_t* board, int depth, int player);

/**
 * @brief macro_ai scorre la lista di mosse e per ciascuna chiama la funzione minimax 
 * @details i valori ritornati dalle varie chiamate alla funzione minimax vengono tra loro confrontati per trovare il maggiore 
 * @param l puntatore alla lista di mosse 
 * @param depth profondità da passare come parametro in ingresso ad ogni chiamata alla funzione minimax 
 * @param player parametro che ha sempre valore 0 in quanto macro_ai è chiamata sempre e solo durante il turno del COMPUTER (che è WHITE di default)
 * @return ritorna il numero corrispondente alla mossa scelta dal COMPUTER
 */
int macro_ai (moves_list_t* l, int depth, int player);