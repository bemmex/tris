/*
 * 
 * widget.h
 * define text user interface with ncurses (TUI)
 *
 */

#ifndef _WIDGET_H
#define _WIDGET_H 1

#include <ncurses.h>

#define BORDER_DEFAULT 1
#define BORDER_EDITOR 2
#define BORDER_CHOICE 3

typedef struct _Center Center;
typedef struct _Win_Response Win_Response;
typedef enum _Button_Type Button_Type;

enum _Button_Type {
	BUTTON_OK,
	BUTTON_CANCELL
};

struct _Center
{
    int x;
    int y;
};

struct _Win_Response
{
    int menu_selected; //menu item index
    int button_selected; //button pressed
};



void get_window_dimension(WINDOW *win, int *x, int *y);
Center *get_center(WINDOW *win, int height, int width);
void create_box(WINDOW *win, int type);
WINDOW *create_win(int height, int width, int starty, int startx, int type);
void destroy_win(WINDOW *win);
WINDOW *create_win_center(WINDOW *win_father, int height, int width, int type);
WINDOW *modal_win(WINDOW *win_father, int height, int width, int border_type, int background, char *win_title, char *text);
WINDOW *modal_win_ok(WINDOW *win_father, int height, int width, int border_type, int background, char *win_title, char *text);
Win_Response *modal_win_choice(WINDOW *win_father, int height, int width, int border_type, int background, char *win_title, char *menu_elements[], int num_of_elements, int default_menu_selected);
void print_text_center(WINDOW *win, char *text);
void button();

#endif /* _WIDGET_H */