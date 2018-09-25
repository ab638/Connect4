#include "connect4.h"

// draw layout of board
void drawBoardLayout() {
    clear(); // clear menu
    
    int x, y, xboardmax = 44, yboardmax = 19;
    board = newwin(yboardmax, xboardmax, 4, 3); // new window
    wattron(board, COLOR_PAIR(4)); // yellow board
    for(x = 0; x < xboardmax; x++) {
        mvwaddch(board, 0, x, '*'); // put * at x,0
        mvwaddch(board, yboardmax-1, x, '*'); // put * at x, 18
    }
    for(y = 0; y < yboardmax; y++) {
        mvwaddstr(board, y, 0, "**"); // put ** at 0,y
        mvwaddstr(board, y,xboardmax - 2, "**"); // put ** at 42,y
    }
    for(y = 1; y <= yboardmax - 2; y++)
        for(x = 0; x < xboardmax; x += 6)
            mvwaddstr(board, y, x, "**"); // put ** at x,y

    for(x = 1; x <= xboardmax - 2; x++) // put * at x,y
        for(y = 0; y < yboardmax; y += 3)
            mvwaddch(board, y, x, '*');
   // ^^^^ done twice to make a good looking board
    refresh(); //  refresh
    wrefresh(board); // refreash the board window
}

// color in the board when a piece is dropped
void drawBoard() {
    int x, y;
    for(int i = 1; i <= 6; i++) {
        y=1+3*(i-1);
        for(int j = 1; j <= 7; j++) {
            x=2+6*(j-1);
            if(boardState[i][j] != 0) {
                switch(boardState[i][j]) {
                case 1:
                    wattrset(board, COLOR_PAIR(5)); // player 1
                    break;
                case 2:
                    wattrset(board, COLOR_PAIR(6)); // player 2
                    break;
                }
                mvwaddstr(board, y, x, "****");
                mvwaddstr(board, y+1, x, "****");
                wattrset(board, A_NORMAL);
            }
            else {
                wattrset(board, COLOR_PAIR(1));
                mvwaddstr(board, y,x,"    ");
                mvwaddstr(board,y+1,x,"    ");
            }
        }
    }
    refresh();
    wrefresh(board);
}

void Play() {
    colorChoice[1] = 5;
    colorChoice[2] = 6;
    int c, availableRow, chosenColor = 0, color = colorChoice[1];
    turn = 1;
    nodelay(stdscr, TRUE);
    
    while(1) {
        c = getch();
	printScore();
        if(c == 'q') {
            int ch;
            drawPrompt("Are you sure you want to quit my dude?\n Yay(y)/ Nay(n)");
            do {
                ch = getch();
            } while(ch != 'y' && ch != 'n');

            if(ch == 'y') {
                updatePlayer(p[0]);
		updatePlayer(p[1]);
                Quit();
                break;
            }
            if(ch == 'n') {
                drawBoardLayout();
                drawBoard();
            }


        }
	color = colorChoice[turn];
        if(c == KEY_DOWN ) {
            availableRow = getAvailRow(chosenColor + 1);
            if(availableRow > 0) {
                animatePiece(turn, chosenColor); // let piece drop
		// save who played the last piece in what position
                boardState[availableRow][chosenColor + 1] = turn; 
                drawBoard(); 
		// check if that piece ended the game
                if(checkEoG(availableRow, chosenColor+1)) {
                    gameOver();
                }
                turn = 3 - turn;
                color = colorChoice[turn];
                if(availableRow == 1) {
                    fullColumns++;
                    if(fullColumns == 7) {
                        fullColumns = 0;
                        Draw(); // game is a draw
                    }
                }
            }
        }

        previewPiece(2, chosenColor, color);
        if(c == KEY_LEFT) {
            chosenColor = (chosenColor + 6)%7;
            previewPiece(2, chosenColor, color);
        }
        if(c == KEY_RIGHT) {
            chosenColor = (chosenColor + 1)%7;
            previewPiece(2, chosenColor, color);
        }
    }
}

/* Display a piece above the board */
void previewPiece(int row, int chosenColor, int color) {
    int i;
    for(i = 0; i < 7; i++) {
        if(i == chosenColor) {
            attron(COLOR_PAIR(color));
            mvprintw(row, 5 + 6 * i, "****");
            mvprintw(row + 1, 5 + 6 * i, "****");
            attroff(COLOR_PAIR(color));
        }
        else {
            mvprintw(row, 5 + 6 * i, "    ");
            mvprintw(row + 1, 5 + 6 * i, "    ");
        }
    }
}
// gets empty rows
int getAvailRow(int col) {
  int i = 0;
  while(boardState[i + 1][col] == 0 && i < 6)
    i++;
  return i;
}
// animate piece as it drops
void animatePiece(int turn, int chosenColor) {
    int i = 0, availableRow = getAvailRow(chosenColor + 1);
    while(i < availableRow) {
        boardState[i][chosenColor + 1] = turn;
        drawBoard();
        refresh();
        wrefresh(board);
        napms(120);
        boardState[i][chosenColor + 1] = 0;
        drawBoard();
        i++;
    }
}
// check if game is over
int checkEoG(int row, int col) {
    int ok = 0, count = 0, i = row, j = col;
    initializeWinningPositions();

    /* check vertical */
    while(boardState[i][j] == boardState[row][col] && i <= 6) {
        count++;
        winningPositions[0][count - 1] = i;
        winningPositions[1][count - 1] = j;
        i++;
    }
    if(count >= 4) {
        return 1;
    }

    /* check horizontal */
    count = 0;
    i = row;
    j = col;
    initializeWinningPositions();
    while(boardState[i][j] == boardState[row][col] && j >= 1) {
        count++;
        winningPositions[0][count - 1] = i;
        winningPositions[1][count - 1] = j;
        j--;
    }
    j = col + 1;
    while(boardState[i][j] == boardState[row][col] && j <= 7) {
        count++;
        winningPositions[0][count - 1] = i;
        winningPositions[1][count - 1] = j;
        j++;
    }
    if(count >= 4) {
        return 1;
    }

    /* check first diagonal */
    count = 0;
    i = row;
    j = col;
    initializeWinningPositions();
    while(boardState[i][j] == boardState[row][col] && j <=7 && i >= 1) {
        count++;
        winningPositions[0][count - 1] = i;
        winningPositions[1][count - 1] = j;
        j++;
        i--;
    }
    i = row + 1;
    j = col - 1;
    while(boardState[i][j] == boardState[row][col] && j >=1 && i <= 6) {
        count++;
        winningPositions[0][count - 1] = i;
        winningPositions[1][count - 1] = j;
        j--;
        i++;
    }
    if(count >= 4) {
        return 1;
    }

    /* check second diagonal */
    count = 0;
    i = row;
    j = col;
    initializeWinningPositions();
    while(boardState[i][j] == boardState[row][col] && j >=1 && i >= 1) {
        count++;
        winningPositions[0][count - 1] = i;
        winningPositions[1][count - 1] = j;
        j--;
        i--;
    }
    i = row + 1;
    j = col + 1;
    while(boardState[i][j] == boardState[row][col] && j <= 7 && i <= 6) {
        count++;
        winningPositions[0][count - 1] = i;
        winningPositions[1][count - 1] = j;
        j++;
        i++;
    }
    if(count >= 4) {
        return 1;
    }

    return 0;
}

void initializeWinningPositions() {
    int i, j;
    for(i = 0; i < 2; i++)
        for(j = 0; j < 7; j++)
            winningPositions[i][j] = 0;
}

void blinkWin() {
  int i, blink = 0, prevValue;
  while(blink < 5) {
    i = 0;
    while(i < 7 && winningPositions[0][i] != 0) {
      prevValue = boardState[winningPositions[0][i]][winningPositions[1][i]];
      boardState[winningPositions[0][i]][winningPositions[1][i]] = 3;
      i++;
    }
    drawBoard();
    napms(150);
    i = 0;
    while(i < 7 && winningPositions[0][i] != 0) {
      boardState[winningPositions[0][i]][winningPositions[1][i]] = prevValue;
      i++;
    }
    drawBoard();
    napms(120);
    
    blink++;
  }
}

void printScore(){
 switch(turn) {
    case 1:
      mvprintw(5, 51 + strlen(p[0].name) + 
               strlen(" vs ") + strlen(p[1].name), " ");
      attron(COLOR_PAIR(colorChoice[0]));
      mvprintw(5, 48, "*");
      attroff(COLOR_PAIR(colorChoice[0]));
      break;
    case 2:
      mvprintw(5, 48, " ");
      attron(COLOR_PAIR(colorChoice[0]));
      mvprintw(5, 51 + strlen(p[0].name) + 
               strlen(" vs ") + strlen(p[1].name), "*");
      attroff(COLOR_PAIR(colorChoice[1]));
      break;
  }

  attron(A_BOLD);
  mvprintw(5, 50, "%s VS %s", p[0].name, p[1].name);
  attroff(A_BOLD);
  /* print current score */
  mvprintw(7, 50, "Current points:");
  mvprintw(8, 50, "%s: %d", p[0].name, curPointsPlayer[0]);
  mvprintw(9,50, "%s: %d", p[1].name, curPointsPlayer[1]);
  /* print total score for each player */
  mvprintw(11, 50, "Total points:");
  mvprintw(12, 50, "%s: %d", p[0].name, p[0].score);
  mvprintw(13, 50, "%s: %d", p[1].name, p[1].score);
  mvprintw(18, 50, "Key bindings:");
  
  mvprintw(19, 50, "LEFT:  <-");
  mvprintw(20, 50, "RIGHT: ->");
  mvprintw(21, 50, "ACTION: Down Arrow");
  mvprintw(22, 50, "QUIT:q");
}

void resetBoard(){
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 9; j++)
			boardState[i][j]= 0;
}

void Draw(){

char *msg = "It's a draw!\n Do you want to play another one?\n YES(y) / NO(n)";
  int ch;
  drawPrompt(msg);
  do {
    ch = getch();
  }while(ch != 'y' && ch != 'n');

  if(ch == 'n') {
    updatePlayer(p[0]);
    updatePlayer(p[1]);
    Quit();
    endwin();
    exit(0);
  }
  if(ch == 'y') {
    resetBoard();
    drawBoardLayout();
    drawBoard();
	}

}

/* Update variables and print message when the game is over */
void gameOver() {
  char msg[100];
  int ch;
  fullColumns = 0;
  sprintf(msg, "%s has won!\n Do you want to play again?\n YES(y)/NO(n)",
          p[turn-1].name);
  curPointsPlayer[turn - 1]++;
  p[turn - 1].score++;
  printScore();
  blinkWin();
  drawPrompt(msg);
  while((ch = getch()) != 'y' && ch != 'n');
  if(ch == 'n') {
    updatePlayer(p[0]);
    updatePlayer(p[1]);
    Quit();
    endwin();
    exit(0);
  }
  if(ch == 'y') {
    resetBoard();
    drawBoardLayout();
    drawBoard();
  }
}
