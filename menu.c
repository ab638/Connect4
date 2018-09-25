#include "connect4.h"

void Initialize() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
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

void playerSelect(){
	nodelay(stdscr, FALSE);
	clear();
	echo();
	mvprintw(ymax/4, xmax/6, "Enter name for Player 1: ");
	refresh();
	getnstr(p[0].name, 10);
	
	mvprintw(ymax/4 + 1, xmax/6, "Enter name for Player 2: ");
	getnstr(p[1].name, 10);
	
	/*for(int i = 0; i <= 1; i++){
		if(searchPlayer(p[i]))
			p[i].score = getPlayerScore(p[i]);
		else{
			p[i].score = 0;
			addPlayer(p[i]);
		}
	}
	clear();
	noecho();*/


}

void Quit(){
	clear();
	char *msg = "Smell ya later!";
	mvaddstr(ymax/2, (xmax-strlen(msg)-4), msg);
	drawTitle(0);
	refresh();
	wrefresh(title);
	napms(1500);
}

void drawPrompt(char *s){
	int x, y;
	prompt = newwin(5, 30, 5, 5);
	getmaxyx(prompt, y, x);
	mvwprintw(prompt, 1, 1, "%s", s);
	refresh();
	touchwin(prompt);
	wrefresh(prompt);
	getch();

}

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
