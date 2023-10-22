/* 
 * Tris
 * 2023-10-22
 * 
 * how to compile
 * gcc -o ../obj/tris tris.c core.c layout.c widget.c -lncurses
 * 
 * compile: make
 * run program: make run  
 * clean obj: make clean
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "core.h"
#include "layout.h"

void test_board();

/**
 * main
 */
int main(int argc, char const *argv[])
{
    //test_board();
    //return 0;

    int game_match = 1; // 0: end game, 1: first match, >1: new match
    int status_init_screen = 0;

    Player player;
	Board tris = {};
    Solution *s = NULL;

    s = create_empty_solution();

    while ( game_match > 0 )
    {
        status_init_screen = 0;
        player.current = PLAYER_A;
        player.win = 0;

        init_board(&tris);

        if ( game_match == 1 ){
            // initialize the screen the first time
            status_init_screen = game_init();
        }else{
            // clear current screen
            clear();
        }

        if ( status_init_screen == 0 ) // screen start ok
        {
            game_draw(&tris, BOARD_OFFSET_Y, BOARD_OFFSET_X, &player);

            int choice_loop = game_loop(&tris, &player, s);

            if ( choice_loop == 0 ) // end-game
            { 
                game_match = 0;

            }else if(choice_loop == 2){ // new game match
                game_match += 1;
            }

        }else{ //Error: screen small or no mouse support
            game_match = 0;
        }
    }

    stop("Eng Game!");

    //free Allocate memory
    free(s->check);
    free(s);

	return 0;
}

void test_board()
{
    Player player;
    Board tris = {};
    Solution *s = NULL;

    s = create_empty_solution();

    player.current = PLAYER_A;

    printf("Player state: %d\n", get_player_state(&player));

    init_board(&tris);

    tris.elements[0][0] = (Cell){.player=0, .color=0, .value=0, .x=BOARD_OFFSET_X, .y=BOARD_OFFSET_Y};
    tris.elements[0][1] = (Cell){.player=1, .color=0, .value=1, .x=BOARD_OFFSET_X + CELL_DIMENSION_X + 1, .y=BOARD_OFFSET_Y + CELL_DIMENSION_Y + 1};
    //tris.elements[0][2] = (Cell){.player=1, .color=0, .value=2};
    //tris.elements[1][1] = (Cell){.player=1, .color=0, .value=4};
    //tris.elements[2][0] = (Cell){.player=1, .color=0, .value=6};

    set_player_move(&tris, 13, 6, &player);

    show_board(&tris);

    s = find_solution(&tris, PLAYER_A, s);

    for (int i = 0; i < 8; ++i)
    {
        printf("%d\n", s->check[i]);
    }

    //free solution
    free(s->check);
    free(s);
}
