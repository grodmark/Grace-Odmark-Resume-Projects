//File Name: crossmain.c
//Author: Grace Odmark

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "crossfunction.h"


int main(int argc, char *argv[])
{
											//initialize 2D arrays which will be used to hold the words and boards
	char board[MAX_LENGTH][MAX_LENGTH];
	char userWords [MAX_WORDS][MAX_LENGTH];
	char couldNotPlace [MAX_WORDS][MAX_LENGTH];	
	int numClues = 0;
	char impossibleToPlace[MAX_WORDS][MAX_LENGTH];					//words which could not be placed during the first round of placement

	FILE *ofp;

	int display = 1;

	for (int i = 0; i < MAX_LENGTH; i++)						//initialize board to be empty
		for (int j = 0; j <MAX_LENGTH; j++)
			board[i][j] = '.';



	int numWords = 0;


	if (argc == 1) {								//interactive mode
		printf("Enter your words: \n");
		while (1) {
			if(numWords == MAX_WORDS) {
				printf("too many words\n");				//throw an error if number of words exceeds MAX_WORDS
				break;
			}
			char userInput[50];				
			scanf("%s", &userInput);					//scan user line
			if (strcmp(userInput, ".") == 0) break;				//if user enters . end input loop
			int is_alpha = 1;
			for (int i = 0; i < strlen(userInput); i++)			//check if line is alpha
			{
				if(isalpha(userInput[i]) == 0) is_alpha = 0;
			}
			if((strlen(userInput) <= 15) && (is_alpha == 1))		//if string is alpha and correct length add to userWords
			{
				
					strcpy(userWords[numWords], userInput);
					numWords++;					//increment word counter
			} else if (is_alpha == 0) {					//if string is not alpha return an error and do not add to list
				printf("not alpha characters\n");
			} else {
				printf("word too long\n");				//if string is too long return an error and do not add to list
			}
		}
		display = 1;
	
	} else if((argc == 2) || (argc == 3)) {								//input using text file
		FILE *fp = fopen(argv[1], "r");
		if (fp == NULL) {
			printf("FILE NOT FOUND\n");
			return 0;
		}
		char currentWord[50];

		int wordsEntered = 0;

		while(1) {
			fgets(currentWord, 50, fp);
			char clean[50];
			for (int s = 0; s < strlen(currentWord); s++)	//remove newline
			{
				clean[s] = currentWord[s];
				if (s == strlen(currentWord) -1) clean[s] = '\0';
			}



			if(feof(fp)) break;
			if (clean[0] == '.') break;
			if(wordsEntered >= 20) {					//throw error and end if too many words are entered
				printf("Too many words entered.\n");
				break;
			}
			int is_alpha = 1;
			for (int i = 0; i < strlen(clean); i++)
			{
				if (isalpha(clean[i]) == 0) 
					if (clean[i] != '\n')
						is_alpha = 0;

			}
			if ((strlen(clean) <= 15) && (is_alpha == 1)) {		//if string is alpha and correct length add to userWords
			       strcpy(userWords[numWords], clean);
		      		numWords++;
			} else if (is_alpha == 0) {					//if string is not alpha return an error and do not add to the list
				printf("%s not alpha characters\n", clean);
			} else {
				printf("%s word too long\n", clean);		//if string is too long return an error and do not add to the list
			}
			wordsEntered++;				//increment words counter	
			ofp = fopen(argv[2], "w");

		}
		numWords = wordsEntered;
		if (argc == 2) display = 1;
		else {
			display = 0;
			ofp = fopen(argv[2], "w");
		}


	}


	


	for(int i = 0; i < numWords; i++) 						//capitalize every letter in each word
	{
		capitalize_words(userWords[i]);
	}
	sort_by_length(userWords, numWords);						//sort words by length
	
		
	Clue clues[MAX_WORDS];								//struct clues will hold clues information

	int numLeftover = 0;								//tracks number of words not placed in first round

	int returnVals[] = {numClues, numLeftover};					//use to return both numClues and numLeftover in function
	update_board(userWords, board, couldNotPlace, numWords, clues, returnVals[0], returnVals);	//update board the first time, updating couldNotPlace list with the unused words
	
	update_board(couldNotPlace, board, impossibleToPlace, returnVals[1], clues, returnVals[0], returnVals);	//trying to place words that weren't placed the first time
	
	if ((returnVals[1] != 0) && (argc != 3)) printf("Could not place:" );		//prints words that couldn't be placed after 2 rounds
	else if ((returnVals[1] != 0) && (argc == 3)) {
		fwrite("Could not place:\n", sizeof(char), (strlen("Could not place:\n")), ofp);
	}
	for (int i = 0; i < returnVals[1]; i++)
	{
		if (argc != 3) {
			if (i == returnVals[1] - 1) printf("%s\n", impossibleToPlace[i]);
			else printf("%s,", impossibleToPlace[i]);
		} else {
			fwrite(impossibleToPlace[i], sizeof(char), (strlen(impossibleToPlace[i])), ofp);
		}
	}

	if ((argc == 3) && (returnVals[1] !=0)) {
		fwrite("\n", sizeof(char), 1, ofp);
	}
	
	if (display == 1) {
		display_board(board);
		display_game_board(board);
		print_clues(clues, returnVals[0]);						//print clues for when displaying output
	} else {
		if (ofp == NULL) {								//printing everything to file using similar structures as functions 
			printf("file does not exist\n");					//for stdout scenarios
			fclose(ofp);
			return 0;
		}
		for (int i = 0; i < MAX_LENGTH+2; i++)
			fwrite("-", sizeof(char), 1, ofp);	
		fwrite("\n", sizeof(char), 1, ofp);
		for (int i = 0; i < MAX_LENGTH; i++) {
			fwrite("|", sizeof(char), 1, ofp);
			fwrite(board[i], sizeof(char), MAX_LENGTH, ofp);
			fwrite("|\n", sizeof(char), 2, ofp);
		}
		for (int i = 0; i < MAX_LENGTH+2; i++)
			fwrite("-", sizeof(char), 1, ofp);	
		fwrite("\n", sizeof(char), 1, ofp);

		for (int i = 0; i <= MAX_LENGTH; i++) {
			if ((i == 0) || (i == MAX_LENGTH)) {
				for (int j = 0; j < MAX_LENGTH + 2; j++) {
					fwrite("-", sizeof(char), 1, ofp);
				}
				fwrite("\n", sizeof(char), 1, ofp);
			}
			char str[MAX_LENGTH];
			if (i != MAX_LENGTH) {
				fwrite("|", sizeof(char), 1, ofp);
				for (int j = 0; j < MAX_LENGTH; j++) {
					if (board[i][j] != '.') {
						fwrite(" ", sizeof(char), 1, ofp);
					}
					else {
						fwrite("#", sizeof(char), 1, ofp);
					}
				}
			
				fwrite("|\n", sizeof(char), 2, ofp);
			}
		}



		for (int index = 0; index < MAX_LENGTH; index++) {
			if (index == 0) fwrite("Clues:\n", sizeof(char), strlen("Clues:\n"), ofp);


			char direction[MAX_LENGTH];

		

				if (clues[index].orientation == 'H') strcpy(direction, "ACROSS");
				else strcpy(direction, "DOWN");
				if (clues[index].placed == 'Y'){ 
					int temp;
					char tempStr[1000];
					sprintf(tempStr, "%d",  clues[index].y_loc);
					fwrite(tempStr, sizeof(char), 1, ofp);
					fwrite("| ", sizeof(char), 2, ofp);
					sprintf(tempStr, "%d", clues[index].x_loc);							//attempting to line up clues text
					fwrite(tempStr, sizeof(char), 1, ofp);
					if (direction == "DOWN") fwrite("|    ", sizeof(char), 5, ofp);
					else fwrite("|  ", sizeof(char), 3, ofp);
					fwrite(direction, sizeof(char), strlen(direction), ofp);
					fwrite("| ", sizeof (char), 2, ofp);								//wanted to add spaces and justification
					strfry(clues[index].scramble);									//to the file output here but struggled		
																	//to find a solution to do so
					fwrite(clues[index].scramble, sizeof(char), strlen(clues[index].scramble), ofp);
					fwrite("\n", sizeof(char), 1, ofp);
				

		}
			}
		

	

	}
	
	if( argc == 3) fclose(ofp);
	return 0;
}

# Grace-Odmark-Resume-Projects
