#include <ncurses.h>
#include "../libtest/test.h"

int Height, Width;

int main() {
	initscr();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	mousemask(BUTTON1_CLICKED, NULL);
	getmaxyx(stdscr, Height, Width);
	WINDOW* menu = newwin(Height, Width, 0, 0);
	WINDOW* loger = newwin(Height - 5, Width, 5, 0);
	WINDOW* menu_title = derwin(menu, 1, Width, 0, 0);
	waddstr(menu_title, "Contest");
	WINDOW* menu_contest_drop = subwin(menu, Height - 1, Width, 1, 0);
	waddstr(menu_contest_drop, "New");

	bool quit = true;
	bool menu_popup = false;
	while (quit) {
		int key = getch();
		wprintw(loger, "%d ", key);
		switch (key) {
		case KEY_MOUSE: {
			MEVENT me;
			getmouse(&me);
			wprintw(loger, "%d %d ", me.x, me.y);
			break;
		}
		case 27: {
			quit = false;
			break;
		}
		}
		if (menu_popup) {
			mvwin(menu_contest_drop, 1, 0);
		} else {
			mvwin(menu_contest_drop, Height, Width);
		}
		refresh();
		wrefresh(menu);
		wrefresh(loger);
	}
	endwin();
}