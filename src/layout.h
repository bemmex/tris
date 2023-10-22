/*
 * 
 * layout.h
 * define type and function for game
 *
 */

#ifndef _LAYOUT_H
#define _LAYOUT_H 1

#include "common.h"
#include "core.h"
#include "widget.h"  //use ncurses.h

int game_init();

int game_loop(Board *tris, Player *player, Solution *s);

int game_update(Board *tris, Player *player);

int game_draw(Board *tris, int offset_y, int offset_x, Player *player);

void draw_player_move(Cell elem);

int stop(char *msg);

void show_screen_dimension(WINDOW *win);

#endif /* _LAYOUT_H */