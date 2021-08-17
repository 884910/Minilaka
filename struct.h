#define DIM 7
#define PLAYER_1 0
#define PLAYER_2 1
#define WHITE 0
#define BLACK 1
#define SOLDIER 0
#define OFFICER 1
#define DEPTH 5

/*struct pawn*/
typedef
struct pawn {
	int color;
	char name;
	char rank;
} pawn_t;

/*struct col*/
typedef
struct col {
	int capacity;
	int size;
	//array di struct pawn
	pawn_t* tower;
}col_t;

typedef
struct moves_list{
    char mandatory; //0=no, 1=si
    struct col* board; //qui inseriremo il new possibile rank della board
    char* description; //ex. A5 - C7 
    struct moves_list* next;
}moves_list_t;