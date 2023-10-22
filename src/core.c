/*
 * core.c
 * 
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "core.h"

void init_board(Board *tris)
{
    int x = 0;
    int y = 0;

    tris->row = MAX_ELEMENT;
    tris->column = MAX_ELEMENT;
    
    for (int i = 0; i < MAX_ELEMENT; ++i) //row
    {
        for (int j = 0; j < MAX_ELEMENT; ++j) //column
        {
            x = BOARD_OFFSET_X + CELL_DIMENSION_X*j + 1*j;
            y = BOARD_OFFSET_Y + CELL_DIMENSION_Y*i + 1*i;

            tris->elements[i][j] = (Cell){.player=0, .color=0, .value=i*3+j, .x=x, .y=y};
        }
    }
}

void show_board(Board *tris)
{    
    for (int i = 0; i < MAX_ELEMENT; ++i)
    {
        for (int j = 0; j < MAX_ELEMENT; ++j)
        {
            Cell elem = tris->elements[i][j];
            printf("%d ", elem.player);
        }
        printf("\n\r");
    }
}

Solution *find_solution(Board *tris, int player, Solution *s)
{
    s = clear_solution(s);

    int offset = 0;

    //find row tris
    for (int i = 0; i < MAX_ELEMENT; ++i)
    {
        for (int j = 0; j < MAX_ELEMENT; ++j)
        {
            Cell elem = tris->elements[i][j];
            //printf("%d#%d#%d\n", elem.player, player, elem.value);
            if ( /*elem.value == 1 &&*/ elem.player == player ){
                s->check[i+offset] += 1;
            }
        }
    }

    //find column tris
    offset = 3;
    for (int i = 0; i < MAX_ELEMENT; ++i)
    {
        for (int j = 0; j < MAX_ELEMENT; ++j)
        {
            Cell elem = tris->elements[j][i];
            if ( /*elem.value == 1 &&*/ elem.player == player ){
                s->check[i+offset] += 1;
            }
        }
    }

    
    for (int i = 0; i < MAX_ELEMENT; ++i)
    {
        for (int j = 0; j < MAX_ELEMENT; ++j)
        {
            //diagonal 1
            if ( i == j ){
                offset = 6;
                Cell elem = tris->elements[j][i];
                if ( /*elem.value == 1 &&*/ elem.player == player ){
                    s->check[offset] += 1;
                }
            }

            //diagonal 2
            if ( i + j == 2 ){
                offset = 7;
                Cell elem = tris->elements[j][i];
                if ( /*elem.value == 1 &&*/ elem.player == player ){
                    s->check[offset] += 1;
                }
            }
            
        }
    }

    return s;
}

Solution *clear_solution(Solution *s)
{
    for (int i = 0; i < 8; ++i)
    {
        s->check[i] = 0;
    }
    return s;
}

Solution *create_empty_solution()
{
    Solution *s = NULL;
    s = malloc(sizeof(Solution));

    if ( s != NULL )
    {
        s->check = malloc(sizeof(int)*8);

        if ( s->check != NULL ){
            s = clear_solution(s);
        }
    }

    return s;
}

int set_player_move(Board *tris, int x, int y, Player *player)
{
    int find = 0;

    if ( player->win == 0 )
    {
        for (int i = 0; i < tris->row; ++i)
        {
            for (int j = 0; j < tris->column; ++j)
            {
                Cell elem = tris->elements[i][j];

                if ( elem.player == 0 && 
                    elem.x <= x && (elem.x + CELL_DIMENSION_X) >= x &&
                    elem.y <= y && (elem.y + CELL_DIMENSION_Y) >= y 
                ){
                    tris->elements[i][j].player = player->current;
                    
                    find = 1;
                }
                //mvaddch(CELL_DIMENSION_Y/2 + offset_y + (CELL_DIMENSION_Y*i) + 1*i, CELL_DIMENSION_X/2 + offset_x + (CELL_DIMENSION_X*j) + 1*j, elem.player+90); // player
            }
        }
    }

    return find;
}

void toggle_player_state(Player *player)
{
    if ( player->current == PLAYER_A ){
        player->current = PLAYER_B;
    }else{
        player->current = PLAYER_A;
    }
}

void set_player_state(Player *player, int state)
{
    if (player->current == 0){
        player->current = state;
    }
}

int get_player_state(Player *player)
{
    return player->current;
}