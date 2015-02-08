#ifndef _DRAW_H
#define _DRAW_H

#include <ncurses.h>
#include <string.h>

#include "game.h"

void create_score(void);
void print_score(void);
void print_checkerboard(int tabuleiro[][10], Coordinates *possibilidades, Coordinates position);
void create_info_win(void);
void remove_warnings(void);
void init_all(void);
void print_warn(const char *megadeth);
void player1_win(void);
void player2_win(void);

#endif
