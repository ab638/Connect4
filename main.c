#include "connect4.h"

GDBM_FILE dbf;

Player p[2];
WINDOW *board, *prompt, *title, *win;
int xmax, ymax, boardState[8][9], winningPositions[2][7], 
    colorChoice[2] = {0}, curPointsPlayer[2], turn, fullColumns = 0;
char players[2][30];
char menuList[3][25] = {"Play Game", "Show List of Players", "Quit"};
int main(){
	for(int i =0; i < 2; i++)
		curPointsPlayer[i] = 0;	
	int chosen;
	Initialize();
	getmaxyx(stdscr, ymax, xmax);
	chosen = initMenu();
	switch(chosen){
	case 0:
	//drawTitle(0); // draw the title
	//napms(1500); // nap for 1500ms
		playerSelect(); // get names
		drawBoardLayout(); // draw layout
		Play(); // play the game
		break;
	case 1:  // print database
		printPlayers();
		
		break;
	case 2:
		Quit(); // end screen
		break;
	default: break;
	}
	endwin();
	return 0;
}
