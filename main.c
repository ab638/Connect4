#include "connect4.h"

GDBM_FILE *f;

Player p[2];
WINDOW *board, *prompt, *title;
int xmax, ymax, boardState[8][9], winningPositions[2][7], 
    colorChoice[2] = {0}, curPointsPlayer[2], turn, fullColumns = 0;
char players[2][30];

int main(){
	
	int chosen;
	Initialize();
	getmaxyx(stdscr, ymax, xmax);
	drawTitle(0);
	napms(1500);
	playerSelect();
	drawBoardLayout();
	Play();
	Quit();
	endwin();
	return 0;
}
