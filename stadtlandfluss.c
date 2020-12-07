#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>

#define NAME_LENGTH 32

typedef struct _player{
	char name[NAME_LENGTH];
}player;

int check_finished(const char* abc){
	for (int i = 0; i < strlen(abc); ++i){
		if (abc[i] != ' '){
			return 0;
		}
	}
	return 1;
}

int main(void){

	printf("How many players? ");
	int num_players;
	scanf("%d", &num_players);

	player* players = malloc(sizeof(player) * num_players);
	memset(players, 0, sizeof(player) * num_players);

	for (int i = 0; i < num_players; ++i){
		printf("Player %d/%d name: ", i+1, num_players);
		scanf("%s", players[i].name);
	}

	// consume the last endline
	getchar();

	srand(time(NULL));

	char abc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	int col, row;
	initscr();
	curs_set(0);
	getmaxyx(stdscr,row,col);

	mvprintw(row/2,(col-25)/2,"Press any key to start...");
	refresh();
	getchar();

	int turn = 0;
	while(1){
		clear();

		int current_player_num = turn % num_players;
		char* current_player = players[current_player_num].name;

		int i = rand() % 26;
		while (abc[i] == ' '){
			i = rand() % 26;
		}

		mvprintw(row/2-3,(col-strlen(current_player)-12)/2,"It's %ss turn!", current_player);
		mvprintw(row/2,(col-strlen(abc))/2,"%s",abc);

		refresh();
		getchar();

		int rand_time = rand() % 5 + 2;
		int arrow_moves = i + 26 * rand_time + 1;
		while(arrow_moves > i){
			arrow_moves--;
			if(abc[arrow_moves%26] == ' ') continue;
			clear();

			mvprintw(row/2-2,col/2 - 13 + arrow_moves % 26 - 1," ");
			mvprintw(row/2-2,col/2 - 13 + arrow_moves % 26 + 1," ");
			mvprintw(row/2-2,col/2 - 13 + arrow_moves % 26,"|");

			mvprintw(row/2-1,col/2 - 13 + arrow_moves % 26 - 1," ");
			mvprintw(row/2-1,col/2 - 13 + arrow_moves % 26 + 1," ");
			mvprintw(row/2-1,col/2 - 13 + arrow_moves % 26,"V");

			mvprintw(row/2,(col-strlen(abc))/2,"%s",abc);

			mvprintw(row/2-3,(col-strlen(current_player)-11)/2,"%s rolling... %c", current_player, abc[arrow_moves%26]);

			beep();
			refresh();
			usleep(5000);
		}

		mvprintw(row/2-3,(col-strlen(current_player)-19)/2,"%s rolled the letter: %c", current_player, abc[arrow_moves%26]);
		mvprintw(row/2,(col-strlen(abc))/2,"%s",abc);

		refresh();
		getchar();

		abc[i] = ' ';
		turn++;

		if(check_finished(abc)){
			clear();
			mvprintw(row/2,(col-33)/2,"No letters left. Game finished :)");
			refresh();
			getchar();
			break;
		}
	}

	endwin();

	free(players);

	return 0;
}