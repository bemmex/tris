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
typedef struct _Move Move;

typedef enum _Gamemode Gamemode;

enum _Gamemode {
	PLAYER_PLAYER,
	PLAYER_PC,
	PC_PC
};

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
	int gamemode;
	int moves_to_end;
};

struct _Solution {
	int *check;
};

struct _Player
{
	int current;
	int win;
};

struct _Move
{
	int score;
	int row;
	int col;
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

Move *find_best_move(Board *tris);

Move *random_walk(Board *tris, Solution *s);

#endif /* _CORE_H */