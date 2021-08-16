CC=gcc
FLAGS= -pedantic -Wall

minilaska: main.o ai.o moves_analysis.o board.o
	$(CC) main.o ai.o moves_analysis.o board.o -o minilaska $(FLAGS)

board.o: board.h board.c
	$(CC) -c board.c -o board.o $(FLAGS)

moves_analysis.o: moves_analysis.h moves_analysis.c
	$(CC) -c moves_analysis.c -o moves_analysis.o $(FLAGS)

ai.o: ai.h ai.c
	$(CC) -c ai.c -o ai.o $(FLAGS)

main.o: main.c
	$(CC) -c main.c -o main.o $(FLAGS)

clean:
	rm *.o
