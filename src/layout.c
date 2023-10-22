/* ---------------------------------
 * layout.c
 *
 * how to compile for testing
 * gcc -o ../obj/test layout.c widget.c -lncurses
 * --------------------------------
 */

#include "layout.h"

// remove comment for test
//#define TESTING 1

/**
 * game_init:
 * @param none
 * @return int
 */
int game_init()
{
	int x = 0;
	int y = 0;
	mmask_t mmask;

	initscr();
	noecho();
	//cbreak();
	keypad(stdscr, TRUE);

	get_window_dimension(stdscr, &x, &y);

	if ( x < 70 || y < 25 ){
		stop("Small Screen, stop program. Try X:70, Y:25");
		return -1;
	}

	if (NCURSES_MOUSE_VERSION > 0)
	{
		//"Mouse support OK"

		// enable all event
		mmask = mousemask(ALL_MOUSE_EVENTS, NULL);

		if (mmask != 0)
		{
			int def_col = 1;
			// mouse event ok
			start_color();
			init_pair(0, COLOR_BLUE, COLOR_WHITE);
			init_pair(1, COLOR_WHITE, COLOR_BLUE);
			init_pair(2, COLOR_WHITE, COLOR_CYAN);

			curs_set(0);	// cursor invisible
			attron(A_BOLD); // A_NORMAL
			// set color
			attrset(COLOR_PAIR(def_col) | A_BOLD);
			bkgd(' ' | COLOR_PAIR(def_col));
		}
		else
		{
			stop("NO Mouse Event, stop program");
		}
	}
	else
	{
		stop("NO Mouse support, stop program");
	}

	return 0;
}

/**
 * game_loop:
 * @param Board struct
 * @param Player struct
 * @param Solution struct
 * @return int
 */
int game_loop(Board *tris, Player *player, Solution *s)
{
	int run = 1;
	int ch;
	MEVENT mouseevent;

	while (run == 1)
	{
		ch = getch();

		if ( /*ch == '\n' ||*/ ch == 27 ) //ENTER or ESC or ^C
		{
			run = 0; //end-game
		}

		if (ch == KEY_MOUSE)
		{
			getmouse(&mouseevent);
			move(1, COLS - 25);
			clrtoeol();
			printw("%d - %d", mouseevent.y, mouseevent.x);
			move(mouseevent.y, mouseevent.x);
			int find = set_player_move(tris, mouseevent.x, mouseevent.y, player);
			if ( find == 1 ){

				s = find_solution(tris, PLAYER_A, s);
				for (int i = 0; i < 8; ++i)
				{
					//printf("%d\n", s->check[i]);
					if (s->check[i] == 3){
						player->win = 1;
						set_player_state(player, PLAYER_A);
					}
				}

				s = find_solution(tris, PLAYER_B, s);
				for (int i = 0; i < 8; ++i)
				{
					//printf("%d\n", s->check[i]);
					if (s->check[i] == 3){
						player->win = 1;
						set_player_state(player, PLAYER_B);
					}
				}

				if ( player->win == 0 ){
					toggle_player_state(player);
				}
			}
			//addch('*');
			refresh();
		}

		if (ch == KEY_F(5) ){
			move(1, 25);
			addch('F');
			addch('5');
			refresh();
			run = 2; //new game match
		}

		game_update(tris, player);
	}

	if (run == 0)
	{
		stop("pressed key enter or esc");
	}

	return run;
}

int game_draw(Board *tris, int offset_y, int offset_x, Player *player)
{
	//WINDOW *win_match;

	// clear();
	create_box(stdscr, BORDER_DEFAULT);
	show_screen_dimension(stdscr);

	// title
	attron(A_BOLD);
	mvaddstr(0, (COLS-6)/2, " Tris " );
	attroff(A_BOLD);

	if ( player->current == PLAYER_A ){
		mvaddstr(1, offset_x+CELL_DIMENSION_X*2+2, "            " );
		attron(A_BOLD | A_BLINK);
		mvaddstr(1, offset_x, "PLAYER A (0)" );
		attroff(A_BOLD | A_BLINK);
	}

	if ( player->current == PLAYER_B ){
		mvaddstr(1, offset_x, "            " );
		attron(A_BOLD | A_BLINK);
		mvaddstr(1, offset_x+CELL_DIMENSION_X*2+2, "PLAYER B (X)" );
		attroff(A_BOLD | A_BLINK);
	}

	// show board:
	// draw y line
	for (int i = 0; i < CELL_DIMENSION_X*3+2; i++)
	{
		mvaddch(CELL_DIMENSION_Y + offset_y, i + offset_x, ACS_HLINE); //─
		mvaddch(CELL_DIMENSION_Y*2+1 + offset_y, i + offset_x, ACS_HLINE); //─
	}
	// draw x line 
	for (int j = 0; j < CELL_DIMENSION_Y*3+2; j++)
	{
		mvaddch(j + offset_y, CELL_DIMENSION_X + offset_x, ACS_VLINE); //│
		mvaddch(j + offset_y, CELL_DIMENSION_X*2+1 + offset_x, ACS_VLINE); //│
	}
	mvaddch(CELL_DIMENSION_Y + offset_y, CELL_DIMENSION_X + offset_x, ACS_PLUS); // plus
	mvaddch(CELL_DIMENSION_Y*2+1 + offset_y, CELL_DIMENSION_X + offset_x, ACS_PLUS); // plus
	mvaddch(CELL_DIMENSION_Y + offset_y, CELL_DIMENSION_X*2+1 + offset_x, ACS_PLUS); // plus
	mvaddch(CELL_DIMENSION_Y*2+1 + offset_y, CELL_DIMENSION_X*2+1 + offset_x, ACS_PLUS); // plus

	// menu
	mvaddstr(LINES-6, (COLS-20), "F5:  New Game" );
	mvaddstr(LINES-5, (COLS-20), "ESC: Quit" );

	refresh();

	/*
	win_match = subwin(stdscr,12,20,3,COLS-22);
	create_box(win_match, BORDER_DEFAULT);
	//wattron(win_match,A_BOLD);
	mvwaddstr(win_match, 0, (COLS-10)/2, " Game Win " );
	//wattroff(win_match,A_BOLD);
	wrefresh(win_match);
	*/	

	// show player move
	for (int i = 0; i < tris->row; ++i)
    {
        for (int j = 0; j < tris->column; ++j)
        {
            Cell elem = tris->elements[i][j];
			if ( elem.player != 0 ){
				draw_player_move(elem);
			}
			//mvaddch(CELL_DIMENSION_Y/2 + offset_y + (CELL_DIMENSION_Y*i) + 1*i, CELL_DIMENSION_X/2 + offset_x + (CELL_DIMENSION_X*j) + 1*j, elem.player+90); // player
        }
    }

	if ( player->win == 1 )
	{
		WINDOW *m_win;

		if (player->current == PLAYER_A)
		{
			m_win = modal_win(stdscr, 7, 30, BORDER_DEFAULT, ' ' | COLOR_PAIR(2), "PLAYER A, Win!");
		}
		if (player->current == PLAYER_B)
		{
			m_win = modal_win(stdscr, 7, 30, BORDER_DEFAULT, ' ' | COLOR_PAIR(2), "PLAYER B, Win!");
		}

		wrefresh(m_win);
	}

	return 0;
}

/**
 * game_update:
 * @param Board struct
 * @param Player struct
 * @return int
 */
int game_update(Board *tris, Player *player)
{
	game_draw(tris, BOARD_OFFSET_Y, BOARD_OFFSET_X, player);
	refresh();

	return 0;
}

void draw_player_move(Cell elem)
{
	if ( elem.player == PLAYER_A ){ // 0

	 	attron(A_REVERSE);
		mvaddstr(elem.y+1, elem.x+4, "     ");
		mvaddstr(elem.y+2, elem.x+3, " ");
		mvaddstr(elem.y+2, elem.x+9, " ");
		mvaddstr(elem.y+3, elem.x+3, " ");
		mvaddstr(elem.y+3, elem.x+9, " ");
		mvaddstr(elem.y+4, elem.x+3, " ");
		mvaddstr(elem.y+4, elem.x+9, " ");
		mvaddstr(elem.y+5, elem.x+4, "     ");
		attroff(A_REVERSE);
	}
	if ( elem.player == PLAYER_B ){ // X
		mvaddch(elem.y+1, elem.x+1, ACS_DIAMOND);
		mvaddch(elem.y+1, elem.x+9, ACS_DIAMOND);
		mvaddch(elem.y+2, elem.x+3, ACS_DIAMOND);
		mvaddch(elem.y+2, elem.x+7, ACS_DIAMOND);
		mvaddch(elem.y+3, elem.x+5, ACS_DIAMOND);
		mvaddch(elem.y+3, elem.x+5, ACS_DIAMOND);
		mvaddch(elem.y+4, elem.x+7, ACS_DIAMOND);
		mvaddch(elem.y+4, elem.x+3, ACS_DIAMOND);
		mvaddch(elem.y+5, elem.x+9, ACS_DIAMOND);
		mvaddch(elem.y+5, elem.x+1, ACS_DIAMOND);
	}

	#ifdef DEBUG

	int x = 0;
	int y = 0;
	get_window_dimension(stdscr, &x, &y);
	mvaddstr(3, x-18, "DEBUG:");
	move(4+elem.value, x-18);
	printw("#%d CX:%d - CY:%d", elem.value, elem.x, elem.y);
	//printw("CX:%d - CY:%d", x, y);
	
	#endif // DEBUG

	refresh();
}

/**
 * stop:
 * @param char *
 * @return int
 */
int stop(char *msg)
{
	endwin();
	printf("%s\n", msg);
	return 0;
}

void show_screen_dimension(WINDOW *win)
{
	int x = 0;
	int y = 0;
	get_window_dimension(win, &x, &y);
	move(1, COLS - 15);
	//printw("X:%d - Y:%d", LINES, COLS);
	printw("X:%d - Y:%d", x, y);
}

// ******************
// for debug
// ******************

#ifdef TESTING

int main(int argc, char const *argv[])
{
	int x, y = 0;

	initscr();

	// getmaxyx(stdscr, y, x);
	// printw("Window size is %d rows, %d columns.\n",y,x);
	printw("Window size is %d rows, %d columns.\n", LINES, COLS);

	if (!has_colors())
	{
		stop("No Color!");
	}

	if (start_color() != OK)
	{
		stop("start_color error!");
	}

	printw("NCurses reports that you can use %d colors,\n", COLORS);
	printw("and %d color pairs.", COLOR_PAIRS);

	refresh();
	getch();

	endwin();
	return 0;
}

#endif