#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gdbm.h>

#ifndef CONNECT4_H
#define CONNECT4_H

typedef struct{
	char name[30];
	int score;
}Player;

extern GDBM_FILE *f, *saveFile;
extern Player p[2];
extern char player[2][30];
extern int xmax, ymax, boardState[8][9], colorChoice [2], winningPositions[2][7],curPointsPlayer[2], turn, fullColumns;
extern WINDOW *board, *prompt, *title;



// menu functions

void Initialize();
void playerSelect();
void pickColor(int y, int colorChoice);
void drawPrompt(char *s);
void drawTitle(int y);

// Gameplay functions
void drawBoardLayout();
void drawBoard();
void printTime();
void printScore();
void Play();
void previewPiece(int row, int colChosen, int color);
int getAvailRow(int col);
void animatePiece(int turn, int colChosen);
int checkEoG(int row, int col);
void initializeWinningPositions();
void blinkWin();
void resetBoard();
void Draw();
void gameOver();

// score database functions
void addPlayer(Player p);
int searchPlayer(Player p);
int getPlayerScore(Player p);
void updatePlayer(Player p);
void printDatabase();

// end it
void Quit();
void errormsg(char* s);
#endif
