/** @file struct.h**/

/** La costante DIM rappresenta la dimensione del lato della scacchiera 7 x 7 **/
#define DIM 7

/** Le costanti PLAYER_1 e PLAYER_2 hanno valore rispettivamente 0 e 1 **/
#define PLAYER_1 0
#define PLAYER_2 1

/** Le costanti WHITE e BLACK hanno valore rispettivamente 0 e 1 **/
#define WHITE 0
#define BLACK 1

/** Le costanti SOLDIER e OFFICER hanno valore rispettivamente 0 e 1 **/
#define SOLDIER 0
#define OFFICER 1

/** La costante DEPTH ha valore 5 e rappresenta la profondità a cui arriva la funzione ricorsiva minimax**/
#define DEPTH 5

/*struct pawn*/

/**
 * @brief Struct pedina (in inglese "pawn")
 * @details E' definita dai campi color (WHITE/BLACK), name (W/B/w/b) e rank (SOLDIER/OFFICER)
 */
typedef
struct pawn {
	int color;
	char name;
	char rank;
} pawn_t;

/*struct col*/

/**
 * @brief Struct colonna (in inglese "column" e abbreviato a "col")
 * @details E' definita dai campi capacity (pari a 3), size e tower 
 */
typedef
struct col {
	int capacity;

	/** IL campo size può assumere i valori: 
	 * 	- 0 se la casella è vuota
	 * 	- 1 se è presente una sola pedina sulla casella
	 * 	- 2 se è presente una torre di due pedine sulla casella
	 * 	- 3 se è presente una torre di tre pedine sulla casella
	*/
	int size;
	
	//array di struct pawn
	/** Il campo tower ha tipo pawn_t* poichè è un array di pedine 
	 *  che vanno a costituire una torre alta al massimo 3
	*/
	pawn_t* tower;
}col_t;


/**
 * @brief Struct moves_list (ovvero "lista mosse")
 * @details E' definita dai campi mandatory, board, description e next 
 */
typedef
struct moves_list{
	/** La variabile mandatory definisce l'obbligatorietà della mossa, pertanto assume i valori:
	 * 	- 0 se la mossa è uno spostamento semplice (senza mangiata)
	 * 	- 1 se la mossa è una mangiata
	*/
    char mandatory; //0=no, 1=si

	/** Il campo board rappresenta come sarebbe il campo di gioco se venisse effettuata la mossa.
	 * 	Ha tipo col_t* poichè ognuna delle 49 caselle della scacchiera è di tipo col_t
	*/
    col_t* board; //qui inseriremo il new possibile rank della board

	/** Il campo description ha tipo char* poichè è un array di caratteri 
	 *  rappresentante le caselle coinvolte nella dinamica della mossa*/
    char* description; //ex. A5 - C7 

	/** Il campo next è il puntatore alla prossima cella della lista */
    struct moves_list* next;
}moves_list_t;