#ifndef _GAME_H
#define _GAME_H

#include <ncurses.h>
#include <stdlib.h>

#include "damas.h"

typedef struct {
	int x;
	int y;
} Coordinates;

#define SIZE 10

void init_tabuleiro(int tabuleiro[][SIZE]);
void change_struct(Coordinates *pos, Coordinates *possibilidades);
void mudar_tabuleiro(Coordinates position, Coordinates peca_selecionada, int tabuleiro[][10], int *ok);
int can_move(int tabuleiro[][SIZE], Coordinates *possibilidades, int x, int y);


#endif

