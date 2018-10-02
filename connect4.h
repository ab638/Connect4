// Austin Brummett
// Project 3 CS37

#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gdbm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifndef CONNECT4_H
#define CONNECT4_H

typedef struct{
	char name[30];
	int score;
}Player;

//extern GDBM_FILE dbf;
extern Player p[2];
extern char player[2][30];
extern int xmax, ymax, boardState[8][9], colorChoice [2], winningPositions[2][7],curPointsPlayer[2], turn, fullColumns;
extern char menuList[3][25];
extern WINDOW *board, *prompt, *title;
extern WINDOW *win;
extern datum key, datain;


// Prompts and Initialization functions

void Initialize(); // turn on curses and set up color pairs
void playerSelect(); // get player names and check if they have played before
void drawPrompt(char *s); // Draw prompt with input string
void drawTitle(int y); // draw the big ol' Connect 4
void drawMenu(int choice); // draw menu with choice
int initMenu(); // initialize the menu
void printPlayers();
// Gameplay functions
void drawBoardLayout(); // draw the star layout
void drawBoard(); // color in the non stars
void printScore(); //  print the current & total score, print the key bindings 
void Play(); // play the game
void previewPiece(int row, int colChosen, int color); // put piece above the board 
int getAvailRow(int col); // look at the available rows
void animatePiece(int turn, int colChosen); // send the piece down the hole
int checkEoG(int row, int col); // check them end of game positions
void initializeWinningPositions(); // set the winning positions up
void blinkWin(); // flip  the colors around so it looks like it is blinking
void resetBoard(); // clear board
void Draw(); // check if the game is a draw
void gameOver(Player p[]); // is the game over??? Pass in the array because it's rude

// score database functions
void addPlayer(Player p); // put player in
int searchPlayer(Player p); // look for a player (true if there, false else)
int getPlayerScore(Player p); // return stored score of the player
void updatePlayer(Player p); // update player score at end of game


// end it
void Quit(); //  draw connect 4 with a pokemon reference goodbye
#endif
