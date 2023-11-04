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

WINDOW *modal_win_ok(WINDOW *win_father, int height, int width, int border_type, int background, char *win_title, char *text)
{
	WINDOW *new_win;
	Center *center = NULL;
	MEVENT mouseevent;

	int ch;
	int esc = 0;

	new_win = create_win_center(win_father, height, width, border_type);
	center = get_center(new_win, 0, strlen(text));

	wbkgd(new_win, background);

	mvwprintw(new_win, 1, center->x, "%s", text);

	//button Ok	
	char *btn_txt = "  Ok  ";
	int btn_x = (width-strlen(btn_txt))/2;
	int btn_y = height-2;
	button(new_win, btn_y, btn_x, btn_txt);

	keypad(new_win, TRUE);
	wrefresh(new_win);

	do
	{
		ch = wgetch(new_win);

		if (ch == '\n'){ //enter
			esc = 1;
		}
		/*
		if (ch == KEY_MOUSE)
		{
			getmouse(&mouseevent);
			wattron(new_win, A_REVERSE);
			mvwprintw(new_win,2,4,"%d - %d", mouseevent.y, mouseevent.x);
			mvwprintw(new_win,3,4,"%d - %d", height, width);
			mvwprintw(new_win,4,4,"%d - %d", btn_y, btn_x);
			wattroff(new_win, A_REVERSE);
			wrefresh(new_win);

			if (mouseevent.y == btn_y &&
				mouseevent.x >= btn_x && mouseevent.x <= btn_x+strlen(btn_txt)
			){
				esc = 1;
			}
		}*/
	
	} while(esc == 0);

	//remove new_win
	destroy_win(new_win);
	wclear(win_father);
	wrefresh(win_father);

	return new_win;
}

WINDOW *modal_win(WINDOW *win_father, int height, int width, int border_type, int background, char *win_title, char *text)
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

Win_Response *modal_win_choice(WINDOW *win_father, int height, int width, int border_type, int background, char *win_title, char *menu_elements[], int num_of_elements, int default_menu_selected)
{
	WINDOW *new_win;
	int menu_select   = default_menu_selected;
	int button_select = 0;
	int esc           = 0;
	int ch;
	
	Win_Response *win_status = NULL;
	win_status = malloc(sizeof(Win_Response));

	char *button_elements[] = {" Ok ", " Cancel "};

	new_win = create_win_center(win_father, height, width, border_type);

	wbkgd(new_win, background);

	// set title
	attron(A_BOLD);
	mvwprintw(new_win, 0, 2, "[ %s ]", win_title );
	attroff(A_BOLD);

	int menu_center_x = (width - strlen(menu_elements[0])) / 2;
	int btn_center_x = (width - strlen(button_elements[0]) - strlen(button_elements[1]) ) / 2;
	
	keypad(new_win, TRUE);
	wrefresh(new_win);

	do
	{
		for (size_t i = 0; i < num_of_elements; i++)
		{
			if ( menu_select == i ){
				wattron(new_win, A_REVERSE);
				mvwprintw(new_win, i+2, menu_center_x, "%s", menu_elements[i]);
				wattroff(new_win, A_REVERSE);
			}else{
				mvwprintw(new_win, i+2, menu_center_x, "%s", menu_elements[i]);
			}
		}

		for (size_t i = 0; i < 2; i++)
		{
			if ( button_select == i ){
				wattron(new_win, A_REVERSE);
				mvwprintw(new_win, height-2, btn_center_x+ i*8, "%s", button_elements[i]);
				wattroff(new_win, A_REVERSE);
			}else{
				mvwprintw(new_win, height-2, btn_center_x+ i*8, "%s", button_elements[i]);
			}
		}

		ch = wgetch(new_win);

		switch (ch)
		{
		case '\n': //enter
			esc = 1;
			break;
		case KEY_UP:
			menu_select -= 1;
			break;
		case KEY_DOWN:
			menu_select += 1;
			break;
		case KEY_LEFT:
			button_select -=1;
			break;
		case KEY_RIGHT:
			button_select +=1;
			break;
		}

		if ( menu_select > num_of_elements-1 ){
			menu_select = 0;
		}
		if ( menu_select < 0 ){
			menu_select = num_of_elements-1;
		}
		
		if( button_select < 0 ){
			button_select = 1;
		}
		if( button_select > 1 ){
			button_select = 0;
		}

		//wrefresh(new_win);
		
	} while(esc == 0);

	wrefresh(new_win);

	//remove new_win
	destroy_win(new_win);
	wclear(win_father);
	wrefresh(win_father);

	win_status->menu_selected   = menu_select;
	win_status->button_selected = button_select;

	return win_status;
}

// ******************
// for debug
// ******************

//#define TESTING 1

#ifdef TESTING

int main(int argc, char const *argv[])
{
	typedef enum 
	{
		WINDOW_CENTER,
		WINDOW_OK,
		WINDOW_CHOICE,
	} ExampleType;
	
	int x, y = 0;
	mmask_t mmask;
	ExampleType testType = WINDOW_CHOICE;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	start_color();
	init_pair(0, COLOR_BLUE, COLOR_WHITE);
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_CYAN);

	curs_set(0);

	mmask = mousemask(ALL_MOUSE_EVENTS, NULL);

	//bkgd(ACS_HLINE);

	create_box(stdscr, BORDER_DEFAULT);
	refresh();

	// test window center
    
	if ( testType == WINDOW_CENTER )
	{
		WINDOW *test;
		test = create_win_center(stdscr, 7, 25, BORDER_CHOICE);
		wattron(test, A_REVERSE);
		mvwprintw(test, 2, 2, "%s", "Window Center");
		wattroff(test, A_REVERSE);
		wrefresh(test);

		mvwprintw(stdscr, 2, 2, "text choice:  %s", "press a key to exit");
		getch();
	}
	
	if ( testType == WINDOW_OK )
	{
		WINDOW *m_win = NULL;
		m_win = modal_win_ok(stdscr, 7, 30, BORDER_DEFAULT, ' ' | COLOR_PAIR(2), "", "Window OK!");

		mvwprintw(stdscr, 2, 2, "text choice:  %s", "press a key to exit");
		getch();
	}

	if ( testType == WINDOW_CHOICE )
	{
		char *choice_text[] = {"choice 1","choice 2","choice 3","choice 4"};
		int num_of_elements = sizeof(choice_text)/sizeof(choice_text[0]);
		int default_menu_selected = 0;
		Win_Response *choice = modal_win_choice(stdscr, 8, 30, BORDER_CHOICE, ' ' | COLOR_PAIR(2), "Title", choice_text, num_of_elements, default_menu_selected);

		if ( choice->button_selected == BUTTON_OK ){
			mvwprintw(stdscr, 2, 2, "text choice:  %s", choice_text[choice->menu_selected]);
		}
		mvwprintw(stdscr, 4, 2, "%s", "press a key to exit");
		getch();
	}

	endwin();
	return 0;
}

#endif
