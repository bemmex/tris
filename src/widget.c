/* ---------------------------------
 * widget.c
 *
 * how to compile for testing
 * gcc -o ../obj/test widget.c -lncurses
 * --------------------------------
 */

#include <stdlib.h>
#include <string.h>
#include "widget.h"

//#define TESTING 1

void get_window_dimension(WINDOW *win, int *x, int *y)
{
	getmaxyx(win,*y,*x);
}

void create_box(WINDOW *win, int type)
{
	if ( type != 0 )
	{
		switch (type)
		{
			case BORDER_DEFAULT:
				box(win, 0, 0);
				break;

			case BORDER_EDITOR:
				box(win, ACS_CKBOARD, ACS_HLINE);
				break;
			
			case BORDER_CHOICE:
				wborder(win, ACS_VLINE, ACS_CKBOARD, ACS_HLINE, ACS_HLINE, 0, 0, 0, 0);
				break;
			
			default:
				box(win, 0, 0);
				break;
		}
	}
}

WINDOW *create_win(int height, int width, int starty, int startx, int type)
{	
	WINDOW *new_win;

	new_win = newwin(height, width, starty, startx);
	create_box(new_win, type);
	wrefresh(new_win);		/* Show that box 		*/

	return new_win;
}

void destroy_win(WINDOW *win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(win);
	delwin(win);
}

WINDOW *create_win_center(WINDOW *win_father, int height, int width, int type)
{
	Center *center = NULL;
	WINDOW *new_win;

	center = get_center(win_father, height, width);

	new_win = create_win(height, width, center->y, center->x, type);

	free(center);

	return new_win;
}

void print_text_center(WINDOW *win, char *text)
{
	Center *center = NULL;
	int height = 0;
	int width  = strlen(text);

	center = get_center(win, height, width);

	mvwprintw(win, center->y, center->x, "%s", text);

	free(center);
}

Center *get_center(WINDOW *win, int height, int width)
{
	Center *center = NULL;
	center = malloc(sizeof(Center));

	if ( center != NULL )
	{
		int num_of_x = 0;
		int num_of_y = 0;

		get_window_dimension(win, &num_of_x, &num_of_y);

		center->x = (num_of_x - width) / 2;
		center->y = (num_of_y - height) / 2;
	}

	return center;
}

WINDOW *modal_win_ok(WINDOW *win_father, int height, int width, int border_type, int background, char *text)
{
	WINDOW *new_win;
	Center *center = NULL;

	new_win = create_win_center(win_father, height, width, border_type);
	center = get_center(new_win, 0, strlen(text));

	wbkgd(new_win, background);

	mvwprintw(new_win, 1, center->x, "%s", text);

	//button Ok
	char *btn_txt = "  Ok  ";
	button(new_win, height-2, (width-strlen(btn_txt))/2, btn_txt);

	//wrefresh(new_win);

	return new_win;
}

WINDOW *modal_win(WINDOW *win_father, int height, int width, int border_type, int background, char *text)
{
	WINDOW *new_win;

	new_win = create_win_center(win_father, height, width, border_type);

	wbkgd(new_win, background);

	print_text_center(new_win, text);

	wrefresh(new_win);

	return new_win;
}

void button(WINDOW *win, int y, int x, char *text)
{
	wattron(win, A_REVERSE);
	mvwprintw(win, y, x, "%s", text);
	wattroff(win, A_REVERSE);
}

// ******************
// for debug
// ******************

#ifdef TESTING

int main(int argc, char const *argv[])
{
	int x, y = 0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	create_box(stdscr, BORDER_DEFAULT);
	refresh();

	WINDOW *test;
	test = create_win_center(stdscr, 7, 25, BORDER_CHOICE);
	wattron(test, A_REVERSE);
	mvwprintw(test, 2, 2, "%s", "qualcosa-test");
	wattroff(test, A_REVERSE);
	wrefresh(test);

	getch();

	endwin();
	return 0;
}

#endif
