/** @file moves_analysis.h**/
#include "board.h"

/**
 * @brief simple_shift esegue lo spostamento semplice da una casella iniziale ad una di destinazione
 * @details copia nella casella di destitazione i campi delle pedine presenti nella casella iniziale e apportando le conseguenti modifiche necessarie 
 * @param board scacchiera su cui eseguire lo spostamento semplice
 * @param x0 coordinata x della casella iniziale 
 * @param y0 coordinata y della casella iniziale 
 * @param xd coordinata x della casella di destinazione 
 * @param yd coordinata y della casella di destinazione
 */
void simple_shift(col_t* board, int x0, int y0, int xd, int yd);

/**
 * @brief capture esegue la mangiata di una pedina/torre da parte di un'altra pedina/torre
 * @details regolandosi a seconda dei valori dei campi size delle due pedine/torri coinvolte, in primo luogo esegue la mangiata e successivamente effettua lo spostamento semplice verso la casella di destinazione
 * @param board scacchiera su cui eseguire la mangiata
 * @param x0 coordinata x della casella iniziale (ovvero quella del mangiante)
 * @param y0 coordinata y della casella iniziale (ovvero quella del mangiante)
 * @param x1 coordinata x della casella del mangiato  
 * @param y1 coordinata y della casella del mangiato  
 * @param xd coordinata x della casella di destinazione su cui termina la mossa
 * @param yd coordinata y della casella di destinazione su cui termina la mossa
 */
void capture(col_t* board, int x0, int y0, int x1, int y1, int xd, int yd);

/**
 * @brief destroy elimina la lista di mosse ricorsivamente
 * @param l lista di mosse da eliminare
 * @return ritorna un puntatore a NULL
 */
moves_list_t *destroy (moves_list_t* l);

/**
 * @brief fill_description1 scrive sul campo description della mossa di spostamento semplice
 * @param desc puntatore all'array di char su cui scrivere (è il campo description della struct moves_list)
 * @param x0 coordinata x della casella iniziale 
 * @param y0 coordinata y della casella iniziale 
 * @param xd coordinata x della casella di destinazione 
 * @param yd coordinata y della casella di destinazione
 */
void fill_description1 (char* desc, int x0, int y0, int xd, int yd);

/**
 * @brief fill_description2 scrive sul campo description della mossa di mangiata
 * @param desc puntatore all'array di char su cui scrivere (è il campo description della struct moves_list)
 * @param x0 coordinata x della casella iniziale (ovvero quella del mangiante)
 * @param y0 coordinata y della casella iniziale (ovvero quella del mangiante)
 * @param x1 coordinata x della casella del mangiato  
 * @param y1 coordinata y della casella del mangiato  
 * @param xd coordinata x della casella di destinazione su cui termina la mossa
 * @param yd coordinata y della casella di destinazione su cui termina la mossa
 */
void fill_description2 (char* desc, int x0, int y0, int x1, int y1, int xd, int yd);

/**
 * @brief union_list, date due liste in entrata, accoda la seconda alla prima
 * @param head puntatore alla prima lista
 * @param new_list puntatore alla seconda lista
 * @return puntatore alla prima lista ora modificata
 */
moves_list_t *union_list(moves_list_t* head, moves_list_t* new_list);

/**
 * @brief insert_simple_shift riempie correttamente i campi della struct moves_list nel caso di spostamento semplice chiamando le funzioni ausiliari sopra descritte
 * @details è chiamata all'interno della funzione moves_analysis
 * @param l puntatore alla lista di mosse a cui aggiungere in coda la nuova cella
 * @param board scacchiera prima dell'esecuzione della mossa di spostamento semplice che viene poi eseguita all'interno di questa funzione
 * @param x0 coordinata x della casella iniziale 
 * @param y0 coordinata y della casella iniziale 
 * @param xd coordinata x della casella di destinazione 
 * @param yd coordinata y della casella di destinazione
 * @return puntatore alla lista di mosse avente ora una cella in più
 */
moves_list_t *insert_simple_shift (moves_list_t* l, col_t* board, int x0, int y0, int xd, int yd);

/**
 * @brief insert_capture riempie correttamente i campi della struct moves_list nel caso di mangiata
 * @details è chiamata all'interno della funzione moves_analysis
 * @param l puntatore alla lista di mosse a cui aggiungere in coda la nuova cella
 * @param board scacchiera prima dell'esecuzione della mossa di mangiata che viene poi eseguita all'interno di questa funzione
 * @param x0 coordinata x della casella iniziale (ovvero quella del mangiante)
 * @param y0 coordinata y della casella iniziale (ovvero quella del mangiante)
 * @param x1 coordinata x della casella del mangiato  
 * @param y1 coordinata y della casella del mangiato  
 * @param xd coordinata x della casella di destinazione su cui termina la mossa
 * @param yd coordinata y della casella di destinazione su cui termina la mossa
 * @return puntatore alla lista di mosse avente ora una cella in più
 */
moves_list_t *insert_capture (moves_list_t* l, col_t* board, int x0, int y0, int x1, int y1, int xd, int yd);

/**
 * @brief moves_analysis, analizzando l'intera scacchiera, controlla che siano verificate le condizioni necessarie per eseguire una mossa (spostamento semplice o mangiata)
 * @param l puntatore alla lista di mosse da popolare
 * @param board scacchiera attuale della partita
 * @param player parametro che indica se è il turno  di PLAYER_1 o di PLAYER_2
 * @return ritorna la lista di mosse popolata (nel caso in cui non ci siano mosse nella lista ritorna il puntatore alla lista vuota)
 */
moves_list_t* moves_analysis (moves_list_t* l, col_t* board, int player);

/**
 * @brief filter scorre la lista di mosse individuando le eventuali mosse obbligatorie presenti
 * @details controlla il campo mandatory di ogni cella della lista, il quale ha valore 1 se la mossa è una mangiata altrimenti vale 0 nel caso di spostamento semplice
 * @param l lista mosse da filtrare
 * @return in presenza di mosse obbligatorie è ritornata la lista di mosse passata in entrata ora filtrata, altrimenti è ritornata la lista passata in entrata e non modificata
 */
moves_list_t*  filter (moves_list_t* l);

/**
 * @brief print_moves scorre la lista di mosse e stampa il contenuto del campo description di ogni cella
 * @param l puntatore alla lista di mosse da stampare
 * @param n parametro in cui verrà posto il numero totale di mosse stampate
 */
void print_moves(moves_list_t* l, int* n);

/**
 * @brief  estract estrae la scacchiera associata alla mossa scelta dal giocatore del turno
 * @details scorre la lista di mosse fino alla cella corrispondente al numero della scelta effettuata dal giocatore del turno da cui estrae la scacchiera  
 * @param l puntatore alla lista di mosse 
 * @param n numero della scelta effettuata dal giocatore del turno
 * @return ritorna la scacchiera associata alla mossa scelta dal giocatore del turno
 */
col_t* estract (moves_list_t* l, int n);

/**
 * @brief estract_description estrae la descrizione associata alla mossa scelta dal giocatore del turno
 * @details scorre la lista di mosse fino alla cella corrispondente al numero della scelta effettuata dal giocatore del turno da cui estrae il campo description
 * @param l puntatore alla lista di mosse 
 * @param n numero della scelta effettuata dal giocatore del turno
 * @return ritorna un array di char contenente la descrizione associata alla mossa scelta dal giocatore del turno
 */
char* estract_description (moves_list_t* l, int n);