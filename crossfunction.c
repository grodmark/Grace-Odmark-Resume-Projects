//File Name: crossfunction.c
//Author: Grace Odmark


#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "crossfunction.h"


void display_board(char board[][MAX_LENGTH])
{		
	for (int i = 0; i < MAX_LENGTH; i++) {						//display board with letters if included
		if (i == 0) {
			for (int k = 0; k <= MAX_LENGTH + 1; k++) {
				printf("-");
			}
			printf("\n");
		}
		for (int j = 0; j < MAX_LENGTH; j++) {
			if (j == 0) printf("|");
			printf("%c", board[i][j]);
			if (j == (MAX_LENGTH - 1)) printf("|\n");
		}
		if (i == MAX_LENGTH - 1) {
			for (int k = 0; k <= MAX_LENGTH + 1; k++) {
				printf("-");
			}
			printf("\n");
		}
	}

}


void update_board(char words[][MAX_LENGTH], char board [][MAX_LENGTH], char couldNotPlace [][MAX_LENGTH], int numWords, Clue clues[MAX_LENGTH], int numClues, int returnVals[])
{
	char temp_board[MAX_LENGTH][MAX_LENGTH];					
	int currentWord = -1;
	int wordsNotPlaced = 0;


	for (int i = 0; i < MAX_LENGTH; i++)
		for (int j = 0; j < MAX_LENGTH; j++)
			temp_board[i][j] = board[i][j];

	for (int i = 0; i < numWords; i++)
	{
		clues[i].placed = 'N';								//initialize clues[i].placed as 'N' and update to 'Y' if applicable
		currentWord++;									//increment current word
		int wordLen = strlen(words[i]);
		int centerClear = 1;								//if first word and center is clear (meaning this must be the first round placing
		for (int j = 0; j < MAX_LENGTH; j++)						//words) run loop to print word centered in the middle of the board
			for (int k = 0; k < MAX_LENGTH; k++)
				if (temp_board[j][k] != '.') centerClear = 0;
		if ((i == 0) && (centerClear == 1)) {
					
			int blank_space = (MAX_LENGTH - strlen(words[currentWord]))/2;
			for (int j = blank_space; j < blank_space + wordLen; j++)
			{
				temp_board[MAX_LENGTH/2][j] = words[i][j - blank_space];
				if (j - blank_space == 0) {
					clues[i].x_loc = j;					//update clues struct parameters
					clues[i].y_loc = MAX_LENGTH/2;
					clues[i].orientation = 'H';
					clues[i].placed = 'Y';
					strcpy(clues[i].scramble, words[i]);
					numClues++;						//increment number of clues
				}	
			}
		} else {
			int check_again = 1;							//check again will be set to equal 0 once the word has been placed or end of letter
			int word_placed = 0;							//search is reached
			for (int j = 0; j < wordLen; j++)					//iterate through each letter of the current word until a match is found or end reached
			{
				check_again = 1;
				if (word_placed == 1)
				{
					break;
				}
				char searchChar = words[currentWord][j];			//target character
				while (check_again == 1) {
					for (int row = 0; row < MAX_LENGTH; row++)		//check by row then column
					{
						if (word_placed == 1)				//break loop if word has been placed
						{
							break;
						}
						if (check_again == 0) break;
						for (int col = 0; col < MAX_LENGTH; col++)
						{
							if (word_placed == 1)
							{
								break;							
							}
							if (check_again == 0) break;
							//if end of row and/or column is reached and the search character isn't found there, move on to the next letter
							if ((row == MAX_LENGTH - 1) && (col == MAX_LENGTH - 1) && (searchChar != temp_board[row][col])) {
								check_again = 0;
								break;
							}
					
							int continue_round = 1;
							if ((searchChar == temp_board[row][col]) && (continue_round == 1))
							{
								int left = 0;						//check up/down and left/right to determine orientation
								int right = 0;
								int up = 0;
								int down = 0;
								int surrounding = 0;

								if (temp_board[row][col + 1] != '.') right = 1;
								if (temp_board[row][col-1] != '.') left = 1;
								if (temp_board[row-1][col] != '.') down = 1;
								if (temp_board[row+1][col] != '.') up = 1;
							
								surrounding = left + right + up + down;

								if (check_again == 1)
								{
									if (surrounding >= 1) {

										//check that the entirety of where the word
										//would be printed is clear with at least one 
										//space in between the word and existing words
										
										int start_check = row - j;	//start checking at beginning of word, not just matching row/col
										int valid_loc = 1;
										for (int search = start_check; search < wordLen + start_check; search++)
										{/*
											if ((temp_board[search][col] != '.') && (search != row))
											{
												valid_loc = 0;
											}
											if (temp_board[search+1][col] != '.') valid_loc = 0;
											if (temp_board[search-1][col] != '.') valid_loc = 0;
											if ((search = start_check) && (temp_board[row-1][col] != '.'))
												valid_loc = 0;
											if ((search = start_check + wordLen - 1) && 
													(temp_board[row+1][col] != '.'))
												valid_loc = 0;
										*/

											if ((left == 1) || (right == 1))
											{
												int clear = 1;
												//check that the entirety of where the word
												//would be printed is clear with at least one 
												//space in between the word and existing words
												if ((start_check + wordLen >= MAX_LENGTH) || (start_check < 0)) clear = 0;

												for (int letter = start_check; letter < wordLen + start_check; letter++)
												{
													if (temp_board[letter][col] != '.')
													{
														if (letter != row) {
															clear = 0;
														}
													}

													if ((temp_board[letter][col+1] != '.') || (temp_board[letter][col-1] != '.'))
													{
														if (letter != row) {
															clear = 0;
														}
													}
													if (temp_board[letter+1][col] != '.')
													{
														if (letter != row - 1)
														{
															clear = 0;
														}
													}
													if (temp_board[letter-1][col] != '.')
													{
														if (letter != row + 1)
														{
															clear = 0;
														}
													}
												}
											

												if (clear == 1) {
													//if surroundings meet standards, begin placing word and
													//update clues struct information at current word's location

													for(int letter = start_check; letter < wordLen + start_check; letter++)
													{
														temp_board[letter][col] = 
															words[i][letter - start_check];
													}
													check_again = 0;
													clues[i].x_loc = col;						
													clues[i].y_loc = row - j;
													clues[i].orientation = 'V';
													clues[i].placed = 'Y';
													numClues++;
													check_again = 0;
													strcpy(clues[i].scramble, words[i]);
													continue_round = 0;
													word_placed = 1;
													break;
												}
											} 
										}										
										start_check = col - j;


										//if left/right condition isn't met or if word cannot be placed that way continue
										//to check if word can be placed horizontally	
										for (int search = start_check; search < wordLen + start_check; search++)
										{
												if (((up == 1) || (down == 1)) && (check_again == 1)) {
											
												//check that surrounding conditions are met
												int clear = 1;
												if ((start_check + wordLen >= MAX_LENGTH) || (start_check < 0)) clear = 0;		
												for (int letter = start_check; letter < wordLen + start_check; letter++)
													{
													if (temp_board[row][letter] != '.')
													{
														if (letter != col) {
															clear = 0;					
													}
													}
													if ((temp_board[row+1][letter] != '.') || (temp_board[row-1][letter] != '.'))
													{
														if (letter != col) {
															clear = 0;	
														}
													}
													if (temp_board[row][letter+1] != '.')
													{
														if (letter != col - 1)
														{
															clear = 0;
														}
													}
													if (temp_board[row][letter-1] != '.')
													{
														if (letter != col + 1)
														{
															clear = 0;
														}
													}
												}

												if (clear == 0) break;
												
												//if conditions are met, begin adding letters to the board and then 
												//update clues struct contions

												for (int letter = start_check; letter < wordLen + start_check; letter++)
												{
													temp_board[row][letter] = 
														words[i][letter-start_check];
												}
												check_again = 0;
												clues[i].x_loc = col;
												clues[i].y_loc = row - j;
												clues[i].orientation = 'H';
												clues[i].placed = 'Y';
												strcpy(clues[i].scramble, words[i]);
												numClues++;
												continue_round = 0;
												word_placed = 1;
												break;
											}
										       
											
										}													
									} 							
							
															
								}
							}
								}
					}
					//if the word could not be placed, add the words to the couldNotPlace array and increment the number of
					//words that could not be placed on the board during this round of testing
					if ((word_placed == 0) && (j == wordLen - 1)) {
						strcpy(couldNotPlace[wordsNotPlaced], words[i]);
						wordsNotPlaced++;					}
				}
			
			}
	



		}
		}

	for (int i = 0; i < MAX_LENGTH; i++)
	{
		strcpy(board[i], temp_board[i]);
	}
	
	//update returnVals values

	returnVals[0] = numClues;
	returnVals[1] = wordsNotPlaced;
		
}

void display_game_board(char board[][MAX_LENGTH])
{
	//display board printing a # for wherever the main board contains a . 
	//and a blank space wherever a letter would be printed
	
		for (int i = 0; i < MAX_LENGTH; i++) {
			if (i == 0) {
				for (int k = 0; k <= MAX_LENGTH + 1; k++) {
					printf("-");
				}
				printf("\n");
			}
			for (int j = 0; j < MAX_LENGTH; j++) {
				if (j == 0) printf("|");
				if (board[i][j] == '.') printf("%c", '#');
				else printf("%c", 32);			
				if (j == (MAX_LENGTH - 1)) printf("|\n");
			}
			if (i == MAX_LENGTH - 1) {
				for (int k = 0; k <= MAX_LENGTH + 1; k++) {
					printf("-");
				}
				printf("\n");
		}
	}

}
void sort_by_length (char words[][MAX_LENGTH], int numWords)
{
	char tempWord[MAX_LENGTH];
	char tempList[MAX_WORDS][MAX_LENGTH];
	int currentLoop = 0;
	int max = 0;

	//iterate through each word and if the length is larger than the current max 
	//(which is instantiated to equal i) and replace i with the max from the round
	for (int i = 0; i < numWords; i++)
	{
		max = i;							
		for (int j = i; j < numWords; j++) {
			if (strlen(words[j]) > strlen(words[max])) 
				max = j;
		}



		strcpy(tempWord, words[i]);
		strcpy(words[i], words[max]);
		strcpy(words[max], tempWord);


		max++;
	}


}

void capitalize_words (char word[MAX_LENGTH])
{
	//capitalize the current word by iterating through each letter of the word and capitalizing it
	char tempStr[MAX_LENGTH];

	for(int i = 0; i < strlen(word); i++) {
		word[i] = toupper(word[i]);
	}



}
void print_clues(Clue clues[MAX_LENGTH], int numClues)
{
	//iterate through clues struct and if clues[i].placed == 'Y', print the contents of the struct to provide a clue
	printf("Clues:\n");

	printf("numClues: %d\n", numClues);

	char direction[MAX_LENGTH];

	for (int j = 0; j < numClues; j++) {

			if (clues[j].orientation == 'H') strcpy(direction, "ACROSS");
			else strcpy(direction, "DOWN");
			if (clues[j].placed == 'Y') 
				printf("%2d, %2d %8s %20s\n",  clues[j].y_loc, clues[j].x_loc, direction, strfry(clues[j].scramble));

		}

}

