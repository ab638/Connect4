#include "connect4.h"
//****************************************************************************
//*                      Game Functions					     *
//****************************************************************************
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
    int c, availableRow, chosenColor = 1, color = colorChoice[1];
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
                // update on quit
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
        //color = colorChoice[turn]; // color of player
        //drop the piece
        if(c == KEY_DOWN ) {
            availableRow = getAvailRow(chosenColor + 1);
            if(availableRow > 0) {
                animatePiece(turn, chosenColor); // let piece drop
                // save who played the last piece in what position
                boardState[availableRow][chosenColor + 1] = turn;
                drawBoard();
                // check if that piece ended the game
                if(checkEoG(availableRow, chosenColor+1)) {
                    gameOver(p);
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

        // piece movement
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
        // show piece if at the correct position
        if(i == chosenColor) {
            attron(COLOR_PAIR(color));
            mvprintw(row, 5 + 6 * i, "****");
            mvprintw(row + 1, 5 + 6 * i, "****");
            attroff(COLOR_PAIR(color));
        }
        else {
            // put whitespace in its trail
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
        // drop down each available row until there is a piece in the way
        boardState[i][chosenColor + 1] = turn;
        drawBoard();
        refresh();
        wrefresh(board);
        napms(100);
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
    //reset winning positions
    int i, j;
    for(i = 0; i < 2; i++)
        for(j = 0; j < 7; j++)
            winningPositions[i][j] = 0;
}

void blinkWin() {
    // while blink is less than 5 blink the pieces that won
    int i, blink = 0, prevValue;
    while(blink < 5) {
        i = 0;
        while(i < 7 && winningPositions[0][i] != 0) {
            prevValue = boardState[winningPositions[0][i]][winningPositions[1][i]];
            boardState[winningPositions[0][i]][winningPositions[1][i]] = 3;
            i++;
        }
        drawBoard();
        napms(120);
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

void printScore() {
    switch(turn) {
    case 1:
        // * indicates which players turn it is
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
    mvprintw(7, 50, "Current Wins:");
    mvprintw(8, 50, "%s: %d", p[0].name, curPointsPlayer[0]);
    mvprintw(9, 50, "%s: %d", p[1].name, curPointsPlayer[1]);
    /* print total score for each player */
    mvprintw(11, 50, "Total Wins:");
    mvprintw(12, 50, "%s: %d", p[0].name, p[0].score);
    mvprintw(13, 50, "%s: %d", p[1].name, p[1].score);
    /* Key bindings */
    mvprintw(18, 50, "Key bindings:");
    mvprintw(19, 50, "LEFT:  <-");
    mvprintw(20, 50, "RIGHT: ->");
    mvprintw(21, 50, "ACTION: Down Arrow");
    mvprintw(22, 50, "QUIT:q");
}

void resetBoard() {
    // set board state to 0
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 9; j++)
            boardState[i][j]= 0;
}

void Draw() { // if game is a draw

    char *msg = "It's a draw!\n Do you want to play another one?\n YES(y) / NO(n)";
    int ch;
    drawPrompt(msg);
    do {
        ch = getch();
    } while(ch != 'y' && ch != 'n');
    // update everything if quit
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
void gameOver(Player p[]) {
    char msg[100];
    int ch;
    fullColumns = 0;
    sprintf(msg, "%s has won!\n Do you want to play again?\n YES(y)/NO(n)",
            p[turn-1].name); // output message to tell of win
    curPointsPlayer[turn - 1]++; // game session wins ++
    p[turn - 1].score++; // total wins ++

    printScore(); // update score on side window
    blinkWin(); // blinkity blink
    drawPrompt(msg);
    while((ch = getch()) != 'y' && ch != 'n');
    if(ch == 'n') {
        // update players
        updatePlayer(p[0]);
        updatePlayer(p[1]);
        Quit();// play output screen
        endwin();
        exit(0);
    }
    if(ch == 'y') {
        colorChoice[1] = 5;
        colorChoice[2] = 6;
        resetBoard(); // lets play again
        drawBoardLayout(); // redraw layout
        drawBoard(); // redraw board
    }
}

//****************************************************************************
//*                      Menu Functions					     *
//****************************************************************************
void Initialize() {
    initscr();
    cbreak();
    noecho();
    curs_set(0); // invisible cursor
    keypad(stdscr, TRUE); // use
    start_color(); // turn on color
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(5, COLOR_RED, COLOR_RED);
    init_pair(6, COLOR_GREEN, COLOR_GREEN);
    init_pair(7, COLOR_BLUE, COLOR_BLUE);
    init_pair(8, COLOR_WHITE, COLOR_WHITE);
    init_pair(9, COLOR_MAGENTA, COLOR_MAGENTA);
}

void playerSelect() {
    nodelay(stdscr, FALSE);
    clear();
    echo();
    mvprintw(ymax/4, xmax/6, "Enter name for Player 1: ");
    refresh();
    getnstr(p[0].name, 10); // get player name

    mvprintw(ymax/4 + 1, xmax/6, "Enter name for Player 2: ");
    getnstr(p[1].name, 10); // get player name

    for(int i = 0; i <= 1; i++) {
        if(searchPlayer(p[i])) // see if player exists
            p[i].score = getPlayerScore(p[i]); // get total wins if so
        else {
            p[i].score = 0; // 0 score then
            addPlayer(p[i]); // add to database
        }
    }
    clear(); // clear screen
    noecho();


}

void Quit() {
    clear();
    char *msg = "Smell ya later!"; // heh pokemon reference
    mvaddstr(ymax/2, (xmax-strlen(msg)-4), msg);
    drawTitle(0);
    refresh();
    wrefresh(title);
    napms(1500);
}

void drawPrompt(char *s) {
    int x, y;
    prompt = newwin(5, 30, 5, 5); // new window
    getmaxyx(prompt, y, x); // put prompt there
    mvwprintw(prompt, 1, 1, "%s", s); // write s
    refresh();
    touchwin(prompt); // entire window mofified
    wrefresh(prompt);
    getch();

}

// THIS TOOK FOREVER
void drawTitle(int y) {
    title = newwin(7, 79, y, 0);
    wattron(title, COLOR_PAIR(9));
    int i;
    for(i = 0; i < 5; i++) {
        refresh();
        wclear(title);
        /* First Row */
        mvwprintw(title, i - 4, 1, "********");
        mvwprintw(title, i - 4, 11, "********");
        mvwprintw(title, i - 4, 21, "********");
        mvwprintw(title, i - 4, 31, "********");
        mvwprintw(title, i - 4, 41, "********");
        mvwprintw(title, i - 4, 51, "********");
        mvwprintw(title, i - 4, 61, "********");
        mvwprintw(title, i - 4, 71, "*");
        mvwprintw(title, i - 4, 78, "*");


        /* Second Row */
        /* C */
        mvwprintw(title, i - 3, 1,  "*");
        /* O */
        mvwprintw(title, i - 3, 11, "*");
        mvwprintw(title, i - 3, 18, "*");
        /* N */
        mvwprintw(title, i - 3, 21, "*");
        mvwprintw(title, i - 3, 28, "*");
        /* N */
        mvwprintw(title, i - 3, 31, "*");
        mvwprintw(title, i - 3, 38, "*");
        /* E */
        mvwprintw(title, i - 3, 41, "*");
        /* C */
        mvwprintw(title, i - 3, 51, "*");
        /* T */
        mvwprintw(title, i - 3, 65, "*");
        /* 4 */
        mvwprintw(title, i - 3, 71, "*");
        mvwprintw(title, i - 3, 78, "*");

        /* Third Row */
        /* C */
        mvwprintw(title, i - 2, 1,  "*");
        /* O */
        mvwprintw(title, i - 2, 11, "*");
        mvwprintw(title, i - 2, 18, "*");
        /* N */
        mvwprintw(title, i - 2, 21, "*");
        mvwprintw(title, i - 2, 28, "*");
        /* N */
        mvwprintw(title, i - 2, 31, "*");
        mvwprintw(title, i - 2, 38, "*");
        /* E */
        mvwprintw(title, i - 2, 41, "********");
        /* C */
        mvwprintw(title, i - 2, 51, "*");
        /* T */
        mvwprintw(title, i - 2, 65, "*");
        /* 4 */
        mvwprintw(title, i - 2, 71, "********");

        /* Fourth Row */
        /* C */
        mvwprintw(title, i - 1, 1,  "*");
        /* O */
        mvwprintw(title, i - 1, 11, "*");
        mvwprintw(title, i - 1, 18, "*");
        /* N */
        mvwprintw(title, i - 1, 21, "*");
        mvwprintw(title, i - 1, 28, "*");
        /* N */
        mvwprintw(title, i - 1, 31, "*");
        mvwprintw(title, i - 1, 38, "*");
        /* E */
        mvwprintw(title, i - 1, 41, "*");
        /* C */
        mvwprintw(title, i - 1, 51, "*");
        /* T */
        mvwprintw(title, i - 1, 65, "*");
        /* 4 */
        mvwprintw(title, i - 1, 78, "*");

        /* Fifth Row */
        /* C */
        mvwprintw(title, i, 1, "********");
        /* O */
        mvwprintw(title, i, 11, "********");
        /* N */
        mvwprintw(title, i, 21, "*");
        mvwprintw(title, i, 28, "*");
        /* N */
        mvwprintw(title, i, 31, "*");
        mvwprintw(title, i, 38, "*");
        /* E */
        mvwprintw(title, i, 41, "********");
        /* C */
        mvwprintw(title, i, 51, "********");
        /* T */
        mvwprintw(title, i, 65, "*");
        /* 4 */
        mvwprintw(title, i, 78, "*");
        napms(150);
        refresh();
        wrefresh(title);
    }

    wattroff(title, COLOR_PAIR(10));
}

void drawMenu(int choice) {
    for(int i = 0; i < 3; i++) {
        move(ymax / 2 + 2 * (i - 1), (xmax - strlen(menuList[1])) / 2 );
        if(i == choice) {
            attron(A_REVERSE);
            printw("%s", menuList[i]);
            attroff(A_REVERSE);
        }
        else
            printw("%s", menuList[i]);
    }
}

int initMenu() {
    int c, i = 0;
    int choice = 0;
    char *s = "Press SPACE or ENTER to choose an option";
    nodelay(stdscr, TRUE);
    drawMenu(choice);
    drawTitle(0);
    while(1) {
        refresh();
        wrefresh(title);

        c = getch();
        if(c == 10 || c == ' ')
            break;
        if(c == KEY_DOWN) {
            choice = (choice + 1) % 3;
            drawMenu(choice);
        }
        if(c == KEY_UP) {
            choice = (choice + 2) % 3;
            drawMenu(choice);
        }


        mvaddstr(ymax - 1, xmax - 1 - 40, s);
        napms(60);

        refresh();
    }
    return choice;
}
void printPlayers() {
    GDBM_FILE dbf = gdbm_open("scoreDatabase.dat", 0, GDBM_READER, 0, NULL);
    Player p[100];
    datum key, data;
    int i = 4;
    nodelay(stdscr, FALSE);
    win = newwin(ymax, xmax, 0, 0);
    mvwprintw(win,2, xmax/2-40, "Name          Wins  ");
    mvwprintw(win,3, xmax/2-40,  "--------------------");
    //fetch info on all info  on players
    for(key = gdbm_firstkey(dbf); key.dptr != NULL; key =gdbm_nextkey(dbf, key)) {
        data =gdbm_fetch(dbf, key);
        strcpy(p[i].name, key.dptr);
        p[i].score = atoi(data.dptr);
        i++;

    }

    // sort and print to scoreboard
    Player t;
    for(int j = 4; j < i; j++) {
        for(int k = j+1; k < i; k++) {
            if(p[k].score > p[j].score) {
                t = p[j];
                p[j] = p[k];
                p[k] = t;
            }
        }
    }
    for(int l = 4; l < i; l++) {
        mvwprintw(win, l, xmax/2-40, "%-5s", p[l].name);
        mvwprintw(win, l, xmax/2-40+ 15, "%-5d", p[l].score);
    }
    gdbm_close(dbf);

    refresh();
    wrefresh(win);
    char c;
    while((c = getch()) != 'q');
    if(c == 'q') {
        char msg[100];
        // Do you still want to play the game after looking at scores?
        sprintf(msg, "Do you want to play the Game?");
        drawPrompt(msg);
        char ch;
        while((ch = getch()) != 'y' && ch != 'n');
        if(ch == 'n') {
            Quit();// play output screen
            endwin();
            exit(0);
        }
        // yeah i want to play
        if(ch == 'y') {
            colorChoice[1] = 5;
            colorChoice[2] = 6;
            playerSelect();
            drawBoardLayout();
            Play();
        }
    }

}
//****************************************************************************
//*                   Database Functions				     *
//****************************************************************************

void addPlayer(Player p) {
    datum key, content;
    key.dptr =  p.name;
    char score[10];
    sprintf(score,"%d",p.score); // convert int to char*
    key.dsize = sizeof(p.name);
    content.dptr= score;
    content.dsize = sizeof(score);

    // open file
    GDBM_FILE dbf = gdbm_open("scoreDatabase.dat", 0, GDBM_WRCREAT, 0666, NULL);
    // storing
    gdbm_store(dbf, key, content, GDBM_REPLACE); // use replace because insert hates me
    gdbm_close(dbf);
}

/* Returns 0 if Player not found
   Returns 1 otherwise */
int searchPlayer(Player p) {
    GDBM_FILE dbf;
    datum key, datain;
    // open file
    dbf = gdbm_open("scoreDatabase.dat", 0, GDBM_READER, 0644, NULL);

    key.dptr = (char*)(p.name); // name to look for
    key.dsize = sizeof(p.name); // size of name

    if(gdbm_exists(dbf, key)) { // check if the key exists
        gdbm_close(dbf); // close it b/c dont want read error
        return 1;
    }
    gdbm_close(dbf); // cloe it b/c dont want read error
    return 0;
}

// returns: total wins
int getPlayerScore(Player p) {
    datum key, datain;
    GDBM_FILE dbf;
    dbf = gdbm_open("scoreDatabase.dat", 0, GDBM_WRCREAT, 0666, NULL); // open
    key.dptr = p.name; // point key to name of player
    key.dsize = sizeof(p.name); // size of char*
    datain = gdbm_fetch(dbf, key); // find that player in the database
    if(datain.dptr == NULL) { // if he isn't there
        free(datain.dptr); // free memory
        p.score = 0; // score is 0
    }
    else {
        p.score=atoi(datain.dptr); // turn the char* to int
        free(datain.dptr); // free memory
    }
    gdbm_close(dbf); // close file
    return p.score; // return the player's score
}

void updatePlayer(Player p) {
    datum key, datain;
    key.dptr = p.name;
    key.dsize = sizeof(p.name);
    char score[10];
    sprintf(score,"%d",p.score); // chr* to int

    datain.dptr = score; // point to new score
    datain.dsize = sizeof(score);
    GDBM_FILE dbf = gdbm_open("scoreDatabase.dat", 0 , GDBM_WRCREAT, 0666, NULL);
    if(datain.dptr != NULL) {
        // store the score
        gdbm_store(dbf, key, datain, GDBM_REPLACE);
    }
    gdbm_close(dbf); // close

}




//****************************************************************************
//*                      End of File             			     *
//****************************************************************************

