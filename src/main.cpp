#include <ncurses.h>
#include <stdarg.h>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include "../libtest/test.h"
using std::string;
using std::vector;
using std::find;
using std::max;
using std::function;

WINDOW* loger;
int Height, Width;

struct Menu {
	string title;
	function<void(Menu*)> proc;
	vector<Menu*> items;
	Menu* father;
	Menu(const char* t, const function<void(Menu*)>& p, ...) : title(t), proc(p), father(0) {
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
		return l + 1;
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
			x = GetWidth();
			y = 0;
		} else {
			father->draw(win, x, y);
			y += GetPos();
			for (size_t i = 0; i < items.size(); ++i) {
				wmove(win, y + i, x);
				waddstr(win, items[i]->title.c_str());
			}
			x += GetWidth();
		}
	}
	Menu* Translate(int& x, int& y) {
		if (!father) {
			--y;
			if (x < GetWidth() && y < items.size()) {
				return items[y];
			} else {
				x -= GetWidth();
				return 0;
			}
		} else {
			Menu* t = father->Translate(x, y);
			if (t) {
				return t;
			}
			if (x < 0 || y < 0) {
				return 0;
			}
			y -= GetPos();
			if (x < GetWidth() && y < items.size()) {
				return items[y];
			} else {
				x -= GetWidth();
				return 0;
			}
		}
	}
};

int main() {
	initscr();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	mousemask(BUTTON1_CLICKED, NULL);
	getmaxyx(stdscr, Height, Width);
	loger = newwin(Height - 5, Width, 5, 0);
	bool quit = true;
	bool menu_popup = false;
	WINDOW* menu_win = newwin(Height - 1, Width, 1, 0);
	Menu* menu = new Menu("", 0,
		new Menu("File", [](Menu*) {},
			new Menu("New", [](Menu*) {}, 0),
			new Menu("Open", [](Menu*) {}, 0),
			new Menu("Close", [](Menu*) {}, 0),
			new Menu("Exit", [&quit](Menu*) {quit = false;}, 0),
			0
		),
		0
	);
	Menu* cur;
	addch('@');
	while (quit) {
		refresh();
		wrefresh(loger);
		wclear(menu_win);
		if (menu_popup) {
			size_t _x = 0, _y = 0;
			cur->draw(menu_win, _x, _y);
		}
		wrefresh(menu_win);
		int key = getch();
		switch (key) {
		case KEY_MOUSE: {
			MEVENT me;
			getmouse(&me);
			if (me.bstate & BUTTON1_CLICKED) {
				if (me.x == 0 && me.y == 0) {
					menu_popup = !menu_popup;
					if (menu_popup) {
						cur = menu;
					}
				} else if (menu_popup) {
					int x = me.x, y = me.y;
					Menu* m = cur->Translate(x, y);
					if (m) {
						cur = m;
						m->proc(m);
					} else {
						menu_popup = false;
					}
				}
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