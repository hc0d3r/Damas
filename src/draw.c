#include "draw.h"

void wCenterTitle(WINDOW *pwin, const char * title){
	int x, maxy, maxx, stringsize;

	getmaxyx(pwin, maxy, maxx);

	stringsize = (maxy-maxy)+4 + strlen(title);
	x = (maxx - stringsize)/2;

	mvwaddch(pwin, 0, x, ACS_RTEE);
	waddch(pwin, ' ');
	waddstr(pwin, title);
	waddch(pwin, ' ');
	waddch(pwin, ACS_LTEE);
}

void wclrscr(WINDOW * pwin){
	int y, x, maxy, maxx;
	getmaxyx(pwin, maxy, maxx);
	for(y=0; y < maxy; y++)
		for(x=0; x < maxx; x++)
			mvwaddch(pwin, y, x, ' ');
}

void remove_warnings(void){
	int i = 0, j = 0;
	for(i=2;i<19;i++){
		for(j=7;j<13;j++){
			mvwaddch(info_win, j, i, ' ');
		}
	}
	wrefresh(info_win);
}

void create_score(void){

	base_win = newwin(6, 20, 2, 44);
	wattrset(base_win, COLOR_PAIR(1));
	wclrscr(base_win);
	box(base_win, 0, 0);
	wCenterTitle(base_win, "SCORE");

	touchwin(base_win);
	wrefresh(base_win);

}

void create_info_win(void){
	info_win = newwin(14, 20, 9, 44);
	wattrset(info_win, COLOR_PAIR(1));
	wclrscr(info_win);
	box(info_win, 0, 0);
	wCenterTitle(info_win, "INFO");

	touchwin(info_win);
	wrefresh(info_win);
}

void print_score(void){
	attron(COLOR_PAIR(6));
	mvwprintw(base_win, 2, 4, "Player 1: %02d",player1);
	mvwprintw(base_win, 3, 4, "Player 2: %02d",player2);
	mvwprintw(info_win, 3, 2, "PLAYER'S TURN: %d",turn);
	wrefresh(base_win);
	wrefresh(info_win);
}

void print_checkerboard(int tabuleiro[][10], Coordinates *possibilidades, Coordinates position){
	int i, y = 2, j;

	attron(COLOR_PAIR(1));
	move(y,2);
	addch(ACS_ULCORNER);

	for(i=0;i<9;i++){
		addch(ACS_HLINE);
		addch(ACS_HLINE);
		addch(ACS_HLINE);
		addch(ACS_TTEE);
	}

	addch(ACS_HLINE);
	addch(ACS_HLINE);
	addch(ACS_HLINE);

	addch (ACS_URCORNER);

	for(j = 9; j >= 0 ; j--){
		y++;
		move(y,2);
		addch(ACS_VLINE);

		for(i=0;i<10;i++){
			if(position.x == i && position.y == j)
				attron(COLOR_PAIR(4));
			else if(possibilidades[0].x == i && possibilidades[0].y == j)
				attron(COLOR_PAIR(5));
			else if(possibilidades[1].x == i && possibilidades[1].y == j)
				attron(COLOR_PAIR(5));
			else if(possibilidades[2].x == i && possibilidades[2].y == j)
				attron(COLOR_PAIR(5));
			else if(possibilidades[3].x == i && possibilidades[3].y == j)
				attron(COLOR_PAIR(5));
			else if(tabuleiro[i][j] == 1 || tabuleiro[i][j] == 3)
				attron(COLOR_PAIR(3));
			else if(tabuleiro[i][j] == 2 || tabuleiro[i][j] == 4)
				attron(COLOR_PAIR(2));

			addch(' ');
			if(tabuleiro[i][j] == 3 || tabuleiro[i][j] == 4)
				addch(ACS_DIAMOND);
			else
				addch(' ');
			addch(' ');
			attron(COLOR_PAIR(1));
			addch(ACS_VLINE);
		}

		y++;

		move(y,2);
		addch(ACS_LTEE);

		if(j!=0){

			for(i=0;i<9;i++){
				addch(ACS_HLINE);
				addch(ACS_HLINE);
				addch(ACS_HLINE);
				addch(ACS_PLUS);
			}

			addch(ACS_HLINE);
			addch(ACS_HLINE);
			addch(ACS_HLINE);
			addch(ACS_RTEE);
		}
	}


	move(y,2);

	addch(ACS_LLCORNER);

	for(i=0;i<9;i++){
		addch(ACS_HLINE);addch(ACS_HLINE);addch(ACS_HLINE);
		addch(ACS_BTEE);
	}

	addch(ACS_HLINE);addch(ACS_HLINE);addch(ACS_HLINE);
	addch(ACS_LRCORNER);

}

void init_all(void){
	initscr();
	noecho();
	curs_set(FALSE);

	start_color();

	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_RED, COLOR_GREEN);
	init_pair(3, COLOR_GREEN, COLOR_RED);
	init_pair(4, COLOR_MAGENTA, COLOR_BLUE);
	init_pair(5, COLOR_CYAN, COLOR_YELLOW);

	init_pair(6, COLOR_WHITE, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_CYAN);

	bkgd(COLOR_PAIR(6)); // 7
	wrefresh(stdscr);
}

void print_warn(const char *megadeth){
        int i = 0, y = 7;
        wmove(info_win,y,2);

        for(i=0;megadeth[i];i++){
                wprintw(info_win,"%c",megadeth[i]);
                if(i!=0 && i%15 == 0){
                        y++;
                        wmove(info_win,y,2);
                }
        }
	wrefresh(info_win);
}

void player1_win(void){
	printw(" ____  _        _ __   _______ ____  _ \n");
	printw("|  _ \\| |      / \\\\ \\ / / ____|  _ \\/ |\n");
	printw("| |_) | |     / _ \\\\ V /|  _| | |_) | |\n");
	printw("|  __/| |___ / ___ \\| | | |___|  _ <| |\n");
	printw("|_|   |_____/_/   \\_\\_| |_____|_| \\_\\_|\n");
	printw("                                       \n");
	printw("__        ___       \n");
	printw("\\ \\      / (_)_ __  \n");
	printw(" \\ \\ /\\ / /| | '_ \\ \n");
	printw("  \\ V  V / | | | | |\n");
	printw("   \\_/\\_/  |_|_| |_|\n");
	printw("                    \n");
}

void player2_win(void){
	printw(" ____  _        _ __   _______ ____  ____  \n");
	printw("|  _ \\| |      / \\\\ \\ / / ____|  _ \\|___ \\ \n");
	printw("| |_) | |     / _ \\\\ V /|  _| | |_) | __) |\n");
	printw("|  __/| |___ / ___ \\| | | |___|  _ < / __/ \n");
	printw("|_|   |_____/_/   \\_\\_| |_____|_| \\_\\_____|\n");
	printw("                                           \n");
	printw("__        ___       \n");
	printw("\\ \\      / (_)_ __  \n");
	printw(" \\ \\ /\\ / /| | '_ \\ \n");
	printw("  \\ V  V / | | | | |\n");
	printw("   \\_/\\_/  |_|_| |_|\n");
	printw("                    \n");
}
