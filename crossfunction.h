//File Name: crossfunction.h
//Author: Grace Odmark


#define _GNU_SOURCE
#define MAX_WORDS 20
#define MAX_LENGTH 15

typedef struct {								//struct will hold information used for placing clues
	int x_loc;
	int y_loc;
	char orientation;
	char placed;
	char scramble [MAX_LENGTH];
} Clue;

void display_board(char [][MAX_LENGTH]);
void update_board(char [][MAX_LENGTH], char [][MAX_LENGTH], char [][MAX_LENGTH], int, Clue [MAX_LENGTH], int, int *);
void display_game_board(char [][MAX_LENGTH]);void sort_by_length (char [][MAX_LENGTH], int);
void capitalize_words (char [MAX_LENGTH]);
void print_clues(Clue [MAX_LENGTH], int);
								
