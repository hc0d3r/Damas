#include "game.h"

void init_tabuleiro(int tabuleiro[][SIZE]){
	int i = 0, j = 0;

	for(j=6;j<10;j++){
		(j % 2 == 0) ? (i = 0) : (i = 1);

		for(;i<10;i+=2){
			tabuleiro[i][j] = 1;
		}
	}

	for(j=0;j<4;j++){
		(j % 2 == 0) ? (i = 0) : (i = 1);

		for(;i<10;i+=2){
			tabuleiro[i][j] = 2;
		}

	}

}

void change_struct(Coordinates *pos, Coordinates *possibilidades){
	int i;

	pos->x = -1;
	pos->y = -1;

	for(i=0;i<4;i++){
		possibilidades[i].x = -1;
		possibilidades[i].y = -1;
	}
}

void mudar_tabuleiro(Coordinates position, Coordinates peca_selecionada, int tabuleiro[][SIZE], int *ok){
	int v = tabuleiro[peca_selecionada.x][peca_selecionada.y],
	diff = abs(position.x-peca_selecionada.x),
	x = 0, y = 0;

	if( (v == 2 && position.y == 9) || (v == 1 && position.y == 0) ){
		v += 2;
	}

	if( diff == 2 ){
		x = ((position.x+peca_selecionada.x+diff)/2)-1;
		y = ((position.y+peca_selecionada.y+diff)/2)-1;

		tabuleiro[x][y] = 0;

		*ok = 1;

		(turn == 1) ? player1++ : player2++;

	}


	tabuleiro[position.x][position.y] = v;
	tabuleiro[peca_selecionada.x][peca_selecionada.y] = 0;
}

int can_move(int tabuleiro[][10], Coordinates *possibilidades, int x, int y){

	int p = 0 , peca = tabuleiro[x][y],
	nl = -1, nl_plus = -1,
	no = -1, no_plus = -1,
	sl = -1, sl_plus = -1,
	so = -1, so_plus = -1;

	if(x+1 <= 9 && y+1 <= 9)
		nl = tabuleiro[x+1][y+1];

	if(x+2 <= 9 && y+2 <= 9)
		nl_plus = tabuleiro[x+2][y+2];

	if(x-1 >= 0 && y+1 <= 9)
		no = tabuleiro[x-1][y+1];

	if(x-2 >= 0 && y+2 <= 9)
		no_plus = tabuleiro[x-2][y+2];

	if(x+1 <= 9 && y-1 >= 0)
		sl = tabuleiro[x+1][y-1];

	if(x+2 <= 9 && y-2 >= 0)
		sl_plus = tabuleiro[x+2][y-2];

	if(x-1 >= 0 && y-1 >= 0)
		so = tabuleiro[x-1][y-1];

	if(x-2 >= 0 && y-2 >= 0)
		so_plus = tabuleiro[x-2][y-2];

	if( (peca %2 == 0) || peca == 3){ // 2 - 4 - 3

		if(nl == 0){
			possibilidades[p].x = x+1;
			possibilidades[p].y = y+1;
			p++;
		}

		else if( ((nl %2 == 1) && (peca %2 == 0)) ||
			((nl %2 == 0) && (peca == 3)) ){
			if(nl_plus == 0){
				possibilidades[p].x = x+2;
				possibilidades[p].y = y+2;
				p++;
			}
		}

		if(no == 0){
			possibilidades[p].x = x-1;
			possibilidades[p].y = y+1;
			p++;
		}

		else if( ((no %2 == 1) && (peca %2 == 0)) ||
			((no %2 == 0) && (peca == 3)) ){

			if(no_plus == 0){
				possibilidades[p].x = x-2;
				possibilidades[p].y = y+2;
				p++;
			}

		}

	}

	if( (peca %2 == 1) || peca == 4){ // 1 - 3 - 4

		if(sl == 0){
			possibilidades[p].x = x+1;
			possibilidades[p].y = y-1;
			p++;
		}

		else if( ((sl % 2 == 0) && (peca %2 == 1)) ||
			((sl % 2 == 1) && (peca == 4)) ){

			if(sl_plus == 0){
				possibilidades[p].x = x+2;
				possibilidades[p].y = y-2;
				p++;
			}

		}

		if(so == 0){
			possibilidades[p].x = x-1;
			possibilidades[p].y = y-1;
			p++;
		}

		else if(  ((so %2 == 0) && (peca %2 == 1)) ||
				((so %2 == 1) && (peca == 4))
			){

			if(so_plus == 0){
				possibilidades[p].x = x-2;
				possibilidades[p].y = y-2;
				p++;
			}

		}
	}

	return (p > 0) ? 1 : 0;
}
