#include <ncurses.h>
#include <stdarg.h>
#include <string>
#include <vector>
#include <algorithm>
#include "../libtest/test.h"
using std::string;
using std::vector;
using std::find;
using std::max;

WINDOW* loger;
int Height, Width;

struct Menu {
	string title;
	void (*proc)(Menu*);
	vector<Menu*> items;
	Menu* father;
	Menu(const char* t, void (*p)(Menu*), ...) : title(t), proc(p), father(0) {
		va_list lst;
		va_start(lst, p);
		Menu* ptr = va_arg(lst, Menu*);
		while (ptr) {
			items.push_back(ptr);
			ptr->father = this;
			ptr = va_arg(lst, Menu*);
		}
		va_end(lst);
	}
	size_t GetWidth() const {
		size_t l = 0;
		for (size_t i = 0; i < items.size(); ++i) {
			l = max(l, items[i]->title.length());
		}
		return l;
	}
	size_t GetPos() const {
		if (father) {
			return std::find(father->items.begin(), father->items.end(), this) - father->items.begin();
		} else {
			return 0;
		}
	}
	void draw(WINDOW* win, size_t& x, size_t& y) {
		if (!father) {
			for (size_t i = 0; i < items.size(); ++i) {
				wmove(win, i, 0);
				waddstr(win, items[i]->title.c_str());
			}
			x = GetWidth() + 1;
			y = 0;
		} else {
			father->draw(win, x, y);
			y += GetPos();
			for (size_t i = 0; i < items.size(); ++i) {
				wmove(win, y + i, x);
				waddstr(win, items[i]->title.c_str());
			}
			x += GetWidth() + 1;
		}
	}
};

void OnNewContest(Menu*) {}
void OnOpenContest(Menu*) {}

int main() {
	initscr();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	mousemask(BUTTON1_CLICKED, NULL);
	getmaxyx(stdscr, Height, Width);
	loger = newwin(Height - 5, Width, 5, 0);
	WINDOW* menu_win = newwin(Height - 1, Width, 1, 0);
	Menu* menu = new Menu("", 0,
		new Menu("Contest", 0,
			new Menu("New", OnNewContest, 0),
			new Menu("Open", OnOpenContest, 0),
			0
		),
		0
	);
	bool quit = true;
	bool menu_popup = false;
	while (quit) {
		refresh();
		wrefresh(loger);
		size_t _x = 0, _y = 0;
		menu->items[0]->items[0]->draw(menu_win, _x, _y);
		wrefresh(menu_win);

		int key = getch();
		switch (key) {
		case KEY_MOUSE: {
			MEVENT me;
			getmouse(&me);
			if (me.y == 0 && me.x < 7) {
				menu_popup = true;
			}
			break;
		}
		case 27: {
			quit = false;
			break;
		}
		}
	}
	endwin();
}