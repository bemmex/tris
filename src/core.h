/*
 * 
 * core.h
 * define type and fuctions
 *
 */

#ifndef _CORE_H
#define _CORE_H 1

#include "common.h"

#define MAX_ELEMENT 3

typedef struct _Cell Cell;
typedef struct _Board Board;
typedef struct _Solution Solution;
typedef struct _Player Player;

struct _Cell {
	int player;
	int value;
	int color;
	int x;
	int y;
};

struct _Board {
	Cell elements[MAX_ELEMENT][MAX_ELEMENT];
	int row;
	int column;
};

struct _Solution {
	int *check;
};

struct _Player
{
	int current;
	int win;
};


void init_board(Board *tris);

void show_board(Board *tris);

Solution *create_empty_solution();

Solution *find_solution(Board *tris, int player, Solution *s);

Solution *clear_solution(Solution *s);

int set_player_move(Board *tris, int x, int y, Player *player);

void toggle_player_state(Player *player);

void set_player_state(Player *player, int state);

int get_player_state(Player *player);

#endif /* _CORE_H */