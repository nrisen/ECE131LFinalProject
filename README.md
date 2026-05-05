# ECE131LFinalProject
WORDLE in C (Group 006 Final Project)
VERSION: 1.0.0
DUE DATE: April 28, 2026
AUTHORS: Gray Sherwood, Matthew Sauceda, Miles Nakai, Nathan Risenhoover, Simon Padilla

------------------------------------------------------------------------

DESCRIPTION:
This is a terminal-based version of the New York Times game "Wordle" 
written in C. The program picks a random 5-letter word from 
a list and the player has six attempts to guess it correctly.

The game features color coded feedback:
- GREEN if Letter is correct and in the right spot.
- YELLOW if Letter is in the word but in the wrong spot.
- RED if Letter is not in the word.

SYSTEM REQUIREMENTS:
- A C compiler
- A terminal that supports ANSI color codes for color feedback (most modern terminals)
- Operating System: Windows (Cygwin), Linux, or macOS

FILES INCLUDED:
- final_project.c : The main source code.
- words.txt       : The dictionary of 5-letter words.
- readme.txt      : Description of project and installation instructions.

INSTALLATION & EXECUTION:
1. Ensure 'words.txt' is in the same folder as the program.
2. Compile the source code (can change name as desired):
   -> gcc final_project.c -o WordleGame
3. Run the program:
   -> ./WordleGame

ECE131LFinalProject/

├── src/                        # Source code and word file

├── ProjectReport.pdf           # Report on lessons learned during project

├── README.md                   # This file

├── README.txt                  # Readme for systems that dont support .md

├── TestPlan.pdf                # List of edge cases to check for bugfixing

├── WordlePresentation.pptx     # Powerpoint presentation on project

└── ...                         # Other files

MENU OPTIONS:
1. Play: Starts a new game with a random word.
2. How to play: View the rules and color meanings.
3. History: View previous games from 'history.txt' (Win %, Tries, etc).
4. Add Word: Adds a custom 5-letter word to 'words.txt'.
5. Reset History: Deletes the 'history.txt' file.
6. Exit: Close the program.

NOTE: 
If 'words.txt' is missing, the game defaults to a secret word/name.

CREDIT:
Wordle word list (modified): tabatkins (GitHub)
