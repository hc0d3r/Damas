#include "damas.h"

#include "draw.h"
#include "game.h"
#include "connection.h"

#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>

#define IN_RANGE(a,b,c) ((a >= b && a <= c) ? 1 : 0)

#define OPT "c:op:l:"

unsigned int player1, player2, turn;
WINDOW *base_win , *info_win;

int servidor_f, offline , client_f;
unsigned int listen_port;
unsigned int connect_port;
char *connect_host;

char ip_connection[INET_ADDRSTRLEN];

struct lol {
	char *x;
	int dec;
};

struct lol values[]={
	{"68",68},
	{"67",67},
	{"66",66},
	{"65",65},
	{"32",32},
	{NULL,0}
};

void check_size(int w){

	endwin();
	refresh();
	wrefresh(stdscr);

	int x,y;

	getmaxyx(stdscr, y, x);

	if(x <= 65 || y <= 23){
		mvprintw(1, 1, "Error !!! Terminal minimum size: 65x24");
		return;
	} else {
		move(1,1);
		clrtoeol();
	}


	if(info_win){
		create_info_win();

	        if(servidor_f){
	                mvwprintw(info_win, 2, 2, "PLAYER NUMBER: 1");
	        }

        	else if(client_f){
        	        mvwprintw(info_win, 2, 2, "PLAYER NUMBER: 2");
        	}

		mvwprintw(info_win, 4, 2, "IP: %s",ip_connection);
        	mvwprintw(info_win, 6, 2, "WARN:");

		wrefresh(info_win);
	}

        if(base_win){
                create_score();
                print_score();
        }


	refresh();


	if(w)
		return;
}

void help(void){
	printf("\n+ Checkers game v1.0\n");
	printf("+ Coder: MMxM @hc0d3r\n\n");

	printf("+ Options:\n\n");
	printf(" -o (Offline match)\n");
	printf(" -l PORT (port to listen)\n");
	printf(" -c IP/Hostname (Server address to connect)\n");
	printf(" -p PORT (port number to connect)\n\n");
	exit(0);
	return;
}

int xgetch(int my_socket, int rock){
	int x = 0,i,check;
	char send_x[10];

	memset(send_x,0,sizeof(send_x));

	if( (offline || (turn == 2 && client_f) || (turn == 1 && servidor_f)) && rock){
		while(1){
			attron(COLOR_PAIR(6));
			print_warn("Continue ? (y|n)");
			refresh();

			x = getch();
			if(x == 'y' || x == 'n')
				break;
		}

		remove_warnings();

		if(my_socket){

			if(x == 'y')
				snprintf(send_x, sizeof(x) ,"y");
			else
				snprintf(send_x, sizeof(x) ,"n");

			check = send(my_socket, send_x, sizeof(send_x), 0);
			if(check <= 0)
				die("send() error");

		}

		return (x == 'y') ? 1 : 0;
	}

	if( ( ( turn == 1 && client_f ) || ( turn == 2 && servidor_f) ) && rock){
		check = recv(my_socket, send_x, sizeof(send_x), 0);

		if(check <= 0)
			die("recv() error");

		if(strcmp(send_x,"y") == 0)
			return 1;
		else
			return 0;
	}

	if(offline == 1){
		return getch();
	}

	memset(send_x,0,sizeof(send_x));

	if( (turn == 1 && servidor_f) || (turn == 2 && client_f )){
		x = getch();
		if((x >= 65 && x <= 68) || x == 32){
			snprintf(send_x,sizeof(x),"%d",(int)x);
			check = send(my_socket, send_x, sizeof(send_x), 0);

			if(check <= 0)
				die("send() error");

		}
	}

	else if( (turn == 1 && client_f) || (turn == 2 && servidor_f ) ){
		check = recv(my_socket, send_x, sizeof(send_x), 0);

		if(check <= 0)
			die("recv() error");

		for(i=0;values[i].x != NULL;i++)
			if(strcmp(send_x,values[i].x) == 0)
				x = values[i].dec;
	}

	return x;

}


int parser_args(int argc, char **argv){
	int c = 0;

	while ((c = getopt (argc, argv, OPT)) != -1){
		switch(c){
			case 'c':
				connect_host = optarg;
				client_f = 1;
			break;

			case 'o':
				offline = 1;
			break;

			case 'l':
				listen_port = (int)strtol(optarg, NULL, 10);
				if(!IN_RANGE(listen_port,1,65535))
					die("Invalid port !!!\n");
				servidor_f = 1;
			break;

			case 'p':
				connect_port = (int)strtol(optarg, NULL, 10);
				if(!IN_RANGE(connect_port,1,65535))
					die("Invalid port !!!\n");
				client_f = 1;
			break;


			default:
				abort();

		}
	}

	if(!offline && !connect_host && !connect_port && !listen_port)
		return 0;

	if( (!connect_port && connect_host)
		|| (connect_port && !connect_host)
	)
		return 0;

	if( servidor_f && client_f )
		return 0;

	if( (servidor_f && offline) || (client_f && offline) )
		return 0;

	return 1;

}


int main(int argc, char **argv){

	if(!parser_args(argc, argv)){
		help();
		return 1;
	}

	int tabuleiro[SIZE][SIZE] = {{0,0}}, i = 0, yes = 0, tmp = 0,
	ok = 0, x = 0, flag = 0, diffx = 0, diffy = 0, xx = 0, yy = 0;

	int sock_con = 0, sock = 0;
	size_t sockaddr_len;
	struct sockaddr cli_addr;
	struct sockaddr_in sockr;

	signal(SIGWINCH, check_size);

	turn = 1;
	player1 = 0;
	player2 = 0;

	init_tabuleiro(tabuleiro);
	init_all();

	Coordinates position = { 4,3 };
	Coordinates possibilidades[4] = { {-1,-1}, {-1,-1}, {-1,-1}, {-1,-1} };
	Coordinates peca_selecionada = { -1, -1 };


	if(servidor_f){
		sockaddr_len = sizeof(cli_addr);
		start_listen(&sock, "0.0.0.0", listen_port);

		mvprintw(5,5,"Waiting connection on port %d...\n",listen_port);
		refresh();

		sock_con = accept(sock, (struct sockaddr *) &cli_addr, &sockaddr_len);

		if(sock_con < 0)
			die("accept() error");

		inet_ntop(cli_addr.sa_family, &((struct sockaddr_in *)&cli_addr)->sin_addr, ip_connection, INET_ADDRSTRLEN);
		servidor_f = 1;

	}

	if(client_f){
		sock_con = socket(AF_INET, SOCK_STREAM, 0);

		sockr = make_addr(connect_host);
		sockr.sin_port = htons(connect_port);

		if( (connect(sock_con, (struct sockaddr *)&sockr, sizeof(sockr) )) == -1)
			die("connect() error");

		inet_ntop(sockr.sin_family, &(sockr.sin_addr), ip_connection, INET_ADDRSTRLEN);
		client_f = 1;
	}

	create_score();
	create_info_win();

	if(servidor_f){
		mvwprintw(info_win, 2, 2, "PLAYER NUMBER: 1");
	}

	else if(client_f){
		mvwprintw(info_win, 2, 2, "PLAYER NUMBER: 2");
	}

	mvwprintw(info_win, 4, 2, "IP: %s",ip_connection);
	mvwprintw(info_win, 6, 2, "WARN:");

	wrefresh(base_win);
	wrefresh(info_win);

	while(1){

		if(player1 == 20 || player2 == 20) break;
		print_checkerboard( tabuleiro, possibilidades, position);
		print_score();
		refresh();

		x = xgetch(sock_con, 0);

		remove_warnings();

		if(x == 68){
			if(position.x-1 >= 0)
				position.x--;
		}

		else if(x == 67){
			if(position.x+1 <= 9)
				position.x++;
		}

		else if(x == 65){
			if(position.y+1 <= 9)
				position.y++;
		}

		else if(x == 66){
			if(position.y-1 >= 0)
				position.y--;
		}

		else if(x == 32){

			if(peca_selecionada.x != -1 && peca_selecionada.y != -1){
				if(position.x == peca_selecionada.x && peca_selecionada.y == position.y && yes == 0){
					change_struct(&peca_selecionada, possibilidades);
				} else {
					for(i = 0; i<4; i++){
						if(position.x == possibilidades[i].x && position.y == possibilidades[i].y){
							mudar_tabuleiro(position, peca_selecionada, tabuleiro, &ok);
							change_struct(&peca_selecionada, possibilidades);
							flag++;
							break;
						}
					}

					if(flag){

						if( can_move(tabuleiro, possibilidades, position.x, position.y) && ok ){

							for(i=0;i<4;i++){
								diffx = abs(position.x-possibilidades[i].x);
								diffy = abs(position.y-possibilidades[i].y);
								xx = ((position.x+possibilidades[i].x+diffx)/2)-2;
								yy = ((position.y+possibilidades[i].y+diffy)/2)-2;

								if( diffx == 2 && xx >= 0 && xx <= 9 && yy >=0 && yy <= 9){
									tmp++;
								} else {
									possibilidades[i].x = -1;
									possibilidades[i].y = -1;
								}
							}
						}

						yes = tmp;

						if(yes == 0){
							if(turn == 1)
								turn++;
							else
								turn--;

							change_struct(&peca_selecionada, possibilidades);

						} else {
							print_checkerboard(tabuleiro, possibilidades, position);
							print_score();
							refresh();
							if(xgetch(sock_con,1)){
								peca_selecionada.x = position.x;
								peca_selecionada.y = position.y;
							} else {
								change_struct(&peca_selecionada, possibilidades);
								yes = 0;
								if(turn == 1) turn++; else turn--;
							}
						}
					}

				}
				ok = 0;
				tmp = 0;
				flag = 0;
			}

			else if( ( (tabuleiro[position.x][position.y] %2 == 1) && turn == 2) || // 1 , 3
				( ( tabuleiro[position.x][position.y] == 2 || tabuleiro[position.x][position.y] == 4)
				 && turn == 1) ){
				if( can_move(tabuleiro, possibilidades, position.x, position.y) ){
					peca_selecionada.x = position.x;
					peca_selecionada.y = position.y;
				} else {
					print_warn("You can't move   this piece");
					refresh();
				}
			} else {
				print_warn("Select a valid  piece");
				refresh();
			}
		}

	}

	clear();

	(player1 > player2) ? player1_win() : player2_win();

	refresh();

	sleep(5);

	endwin();

	return 0;

}

