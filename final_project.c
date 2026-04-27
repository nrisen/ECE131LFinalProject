/* Gray Sherwood, Matthew Sauceda, Miles Nakai, Nathan Risenhoover, Simon Padilla 
 * 4/28/2026
 * Final Project
 * ECE131L - Section003 - Group 5
 * Wordle Game in C
 * Credit to "tabatkins" on Github for providing the source word list from Wordle, modified to contain more commonly known words

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/


#include <stdio.h>
#include <string.h> //Added for various string operations and functions
#include <stdlib.h> //Added for randomizer functions
#include <time.h>   //Added for the time() function to seed the randomizer


//This struct saves information about the game you are currently playing
struct MyGame {
	char secretWord[6];
	int turnsTaken;
};


//This struct saves old game results for the report
struct GameHistory {
	char wordUsed[6];
	int score;
	int didWin; // 1 for win, 0 for loss
};


//Prototypes
void printMenu();
void printRules();
void play(struct MyGame *game); 
void getSecretWord(char *word);
void saveScore(char *word, int tries, int win);
void showPastGames();
void addNewWord();
void deleteHistory();


//Main Function
int main() {
    system("clear"); //Clears anything in the terminal from before

	srand(time(NULL)); //Use the current time of day to make sure the random word is always different
	int menuChoice = 0;

	while(menuChoice != 6) {
		printMenu();
		printf("\nType a number: ");
		scanf("%d", &menuChoice);
		while (getchar() != '\n'); // Clear buffer after scanf to prevent fgets from skipping

		if(menuChoice == 1) {
			struct MyGame activeGame;
            system("clear");
			activeGame.turnsTaken = 0;
			getSecretWord(activeGame.secretWord);
			
			play(&activeGame); 
		} 
		else if(menuChoice == 2) {
            system("clear");
			printRules();
		} 
		else if(menuChoice == 3) {
            system("clear");
			showPastGames();
		} 
		else if(menuChoice == 4) {
            system("clear");
			addNewWord();
		} 
		else if(menuChoice == 5) {
            system("clear");
			deleteHistory();
		} 
		else if(menuChoice < 1 || menuChoice > 6){
			printf("Error: Not a menu option.\n");
		}
	}

	printf("Closing program...\n");
	return 0;
}


//Function for actual guessing
void play(struct MyGame *game) {
	char guess[10];
	int winFlag = 0;

	//Prompts user for actual word
	while((*game).turnsTaken < 6 && winFlag == 0) {
		printf("\nTurn %d/6. Enter a 5-letter word: ", (*game).turnsTaken + 1);
		fgets(guess, 10, stdin);
		guess[strcspn(guess, "\n")] = 0; // Remove the newline character to clean the string
		if (strlen(guess) == 9) while (getchar() != '\n'); // Clear buffer if input exceedss 9 letters

		int wrongChar = 0;
		
		//Checks if the characters entered are all letters
		for (int i = 0; i < 5; i++) {
			if (guess[i] < 'A' || guess[i] > 'z') {
				//Any character not between A and z will make an error message print
				wrongChar = 1;
			}
			if (guess[i] > 'Z' && guess[i] < 'a') {
				//There are still non-letter characters between A and z, this accounts for those
				wrongChar = 1;
			}
		}

		if(wrongChar == 1) {
			printf("Error: Please only enter letters!\n");
			continue;
		}

		//Double check word length
		if(strlen(guess) != 5) { // Length check is now 5 because newline was removed
			printf("Error: That wasn't 5 letters!\n");
			continue;
		}

		//Make the letters uppercase by changing int value
		for(int i = 0; i < 5; i++) {
			if(guess[i] >= 'a' && guess[i] <= 'z') {
				guess[i] = guess[i] - 32;
			}
		}
		
		int correctInRow = 0;
		printf("Result: ");

        //Tracks result for each grid position (0 = wrong, 1 = almost, 2 = correct)
        int result[5] = {0, 0, 0, 0, 0};

        //Loop that counts how many times a letter is in the word
        int letterCount[26] = {0};
        for(int i = 0; i < 5; i++) {
            letterCount[(*game).secretWord[i] - 'A']++;
        }
		
		//This loop checks if each letter is correct going one at a time and takes away from available letter counts
		for(int i = 0; i < 5; i++) {
			if(guess[i] == (*game).secretWord[i]) {
                result[i] = 2;
				correctInRow++;
                letterCount[guess[i] - 'A']--;
			}
        }
        
        //This checks if the letter is anywhere else in the word
        for(int i = 0; i < 5; i++) {
            if(result[i] == 2) {
                continue;
            }
            if(letterCount[guess[i]- 'A'] > 0) {
                result[i] = 1;
                letterCount[guess[i] - 'A']--;
            }
        }

        // Print loop
        for(int i = 0; i < 5; i++) {
			if(result[i] == 2) {
                printf("\033[1;32m[CORRECT]\033[0m");
			} else if(result[i] == 1) {
				printf("\033[1;33m[ALMOST]\033[0m ");
			} else {
				printf("\033[1;31m[WRONG]\033[0m ");
			}
		
		}
		printf("\n");

		(*game).turnsTaken = (*game).turnsTaken + 1;

		//Win condition, if all 5 letters are correct, word is correct
		if(correctInRow == 5) {
			winFlag = 1;
		}
	}

	if(winFlag == 1) {
		printf("\nYou got it!\n");
		saveScore((*game).secretWord, (*game).turnsTaken, 1);
	} 
	else {
		printf("\nOut of turns! The word was \033[1;34m%s\033[0m\n", (*game).secretWord);
		saveScore((*game).secretWord, (*game).turnsTaken, 0);
	}
}


//Opens the file and picks a random word
void getSecretWord(char *word) {
	FILE *filePtr = fopen("words.txt", "r");
	if(filePtr == NULL) {
		strcpy(word, "PETRO"); //Use a backup word if the file is missing 
		return;
	} 

	int wordCount = 0;
	char tempBuffer[10]; //A temporary spot to hold words while we count them

	//Count every word in the file to know the max value for our random number
	while(fscanf(filePtr, "%s", tempBuffer) != -1) {
		wordCount = wordCount + 1;
	}
	/* --- Random Word Logic: ---
	
	Pick a random number between 0 and the max */
	int randomIndex = rand() % wordCount; 

	//Go back to the very first line of the file so we can start counting again
	rewind(filePtr); 

	//Loop through the file again and skip words until we reach the selected word
	for(int i = 0; i <= randomIndex; i++) { 
		fscanf(filePtr, "%s", word); // Word at our random spot will be saved into the 'word' variable
	}

	fclose(filePtr); 

	//Check and convert the secret word to uppercase so it matches the logic in other functions
	for(int i = 0; i < 5; i++) { // Check each of the letters
		if(word[i] >= 'a' && word[i] <= 'z') {
			word[i] = word[i] - 32;  
		}
	}
}


//Function to save your result to a history file
void saveScore(char *word, int tries, int win) {
	FILE *filePtr = fopen("history.txt", "a"); // "a" to append and prevent overwriting 
	if(filePtr != NULL) {
		fprintf(filePtr, "%s %d %d\n", word, tries, win);
		fclose(filePtr);
	}
}


//Function to show your previous games 
void showPastGames() {
	FILE *filePtr = fopen("history.txt", "r");
	if(filePtr == NULL) {
		printf("\nNo games played yet!\n");
		return;
	}

	struct GameHistory historyList[100]; // Array of structs 
	int total = 0;
	int wins = 0;

	printf("\n--- PAST GAMES REPORT ---\n");
	//This reads the file until it hits the end
	while(fscanf(filePtr, "%s %d %d", historyList[total].wordUsed, &historyList[total].score, &historyList[total].didWin) != -1) { 
		printf("Word: %s | Tries: %d | Status: ", historyList[total].wordUsed, historyList[total].score);
		
		//Counter to count up the total wins
		if(historyList[total].didWin == 1) {
			printf("WIN\n");
			wins = wins + 1;
		} 
		else {
			printf("LOSS\n");
		}
		total = total + 1;
	}
	fclose(filePtr);
	
	//Only prints the totals if there are games played
	if(total > 0) {
		printf("Total Games: %d | Total Wins: %d\n", total, wins);
		printf("Win Percentage: %d%% \n", ((wins * 100) / total)); //No need for divide by zero check as only prints report if total > 0
	}
}


//Function to add a new word to the list
void addNewWord() {
	char newWord[10]; //Set to 10 to avoid memory errors 
	printf("Type a new 5-letter word to add: ");
	fgets(newWord, 10, stdin);  //Same process as prompting user for guess, cleans word and prevents too many chars
	newWord[strcspn(newWord, "\n")] = 0;
	if (strlen(newWord) == 9) while (getchar() != '\n');

	//Checks that the new word is 5 letters long
	if(strlen(newWord) != 5) { // Length check updated to 5 now that newline is gone
		printf("Error: That word is not five letters!\n");
		return;
	}
	
	int wrongChar = 0;
		
	//Checks if the characters entered are all letters
	for (int i = 0; i < 5; i++) {
		if (newWord[i] < 'A' || newWord[i] > 'z') {
			//Any character not between A and z will make an error message print
			wrongChar = 1;
		}
		if (newWord[i] > 'Z' && newWord[i] < 'a') {
			//There are still non-letter characters between A and z, this accounts for those
			wrongChar = 1;
		}
	}

	if(wrongChar == 1) {
		printf("Error: Please only enter letters!\n");  //If its not a valid letter print error messgae
		return;
	}

    //Converts the new word to uppercase for a comparison check in the library
    for(int i = 0; i < 5; i++) {
        if(newWord[i] >= 'a' && newWord[i] <= 'z') {
            newWord[i] = newWord[i] - 32;
        }
    }
    
    //Checks for any duplicate words inside the file
    FILE *checkPtr = fopen("words.txt", "r");
    if(checkPtr != NULL) {
        char existing[10];
        //Scans file for any words matching the user entered word
        while(fscanf(checkPtr, "%s", existing) != -1) {
            //Converts existing to uppercase as well just incase
            for(int i = 0; i < 5; i++) {
                if(existing[i] >= 'a' && existing[i] <= 'z') {
                    existing[i] = existing[i] - 32;
                }
            }
            //If the word exists already
            if(strcmp(existing, newWord) == 0) {
                printf("That word is already in the list!\n");
                fclose(checkPtr);
                return;
            }
        }
        fclose(checkPtr);
    }

	FILE *filePtr = fopen("words.txt", "a"); //Uses append to add word to end of list instead of replacing words
	if(filePtr != NULL) {
		fprintf(filePtr, "\n%s", newWord);
		fclose(filePtr);
		printf("Word saved to list!\n");
	}
}


//Function to clear the history
void deleteHistory() {
	if (remove("history.txt") == 0) { //Checks if the deletion was succesful (returned 0)
        	printf("\nHistory deleted!\n");
	}
	else {
        	printf("\nNo history found to delete.\n");  //Exception if there is no history to print
	}
}


//Function to print to main menu
void printMenu() {
	printf("\n--- WORDLE MAIN MENU ---\n");
	printf("\n1. Play\n2. How to play\n3. History\n4. Add Word\n5. Reset History\n6. Exit\n");
}


//Function to print the rules
void printRules() {
	printf("\nWelcome to Wordle!\n");
	printf("With every new game, a five letter word is randomly picked and you will have six tries to guess the correct word.\n");
	printf("You can only enter one five letter word for each try, no numbers, symbols, shorter words, or sentences.\n");
	printf("After each try, for each letter you will get one result:\n");
	printf("\033[1;32m[CORRECT]\033[0m = Letter is in the right spot.\n");
	printf("\033[1;33m[ALMOST]\033[0m = Letter is in the word, but is in the wrong spot.\n");
	printf("\033[1;31m[WRONG]\033[0m = Letter is not in the word.\n");
	printf("If you guess the word in six tries or less, you win the game! If you don't, you lose.\n");
	printf("To play, enter 1 in the menu.\n");
}
