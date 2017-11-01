#include <ncurses.h>
#include <stdarg.h>
#include <string.h>
#include <algorithm>
#include "menu.h"
#include "dialog.h"
#include "../libtest/test.h"

WINDOW* loger;
int Width, Height;

struct AboutDlg : public Dialog {
	int px, py;
	AboutDlg() {
		const char* msg = "Judger. a simple oi contest.";
		int l = strlen(msg);
		wnd = newwin(4, l + 2, (Height - 4) >> 1, (Width - l) >> 1);
		box(wnd, 0, 0);
		wmove(wnd, 1, 1);
		waddstr(wnd, msg);
		px = ((Width - l) >> 1) + ((l - 2) >> 1);
		py = ((Height - 4) >> 1) + 2;
		wmove(wnd, 2, (l - 2) >> 1);
		waddstr(wnd, "ok");
	}
	virtual void Input(int k) {
		if (k == '\n' || k == ' ') {
			exit = true;
		} else if (k == KEY_MOUSE) {
			MEVENT me;
			getmouse(&me);
			if ((me.x == px || me.x == px + 1) && py == me.y) {
				exit = true;
			}
		}
	}
};

int main() {
	setlocale(LC_ALL, "");
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
	Menu* menu;
	Menu* menu_en;
	Menu* menu_chs;
	menu_en = new Menu("menu_en", 0,
		new Menu("File", [](Menu*) {},
			new Menu("New", [](Menu*) {}, 0),
			new Menu("Open", [](Menu*) {
				string str;
				(new InputDialog("Test input" ,str))->Loop();
			}, 0),
			new Menu("Close", [](Menu*) {}, 0),
			new Menu("Exit", [&quit](Menu*) {
				quit = false;
			}, 0),
			0
		),
		new Menu("Setting", 0,
			new Menu("Language", 0,
				new Menu("English <", 0, 0),
				new Menu("中文", [&](Menu*) {
					menu = menu_chs;
					menu_popup = false;
				}, 0),
				0
			),
			0
		),
		new Menu("About", [](Menu*) {
			(new AboutDlg)->Loop();
		}, 0),
		0
	);
	menu_chs = new Menu("menu_chs", 0,
		new Menu("文件", [](Menu*) {},
			new Menu("新建", [](Menu*) {}, 0),
			new Menu("打开", [](Menu*) {}, 0),
			new Menu("关闭", [](Menu*) {}, 0),
			new Menu("退出", [&quit](Menu*) {
				quit = false;
			}, 0),
			0
		),
		new Menu("设置", 0,
			new Menu("语言", 0,
				new Menu("English", [&](Menu*) {
					menu = menu_en;
					menu_popup = false;
				}, 0),
				new Menu("中文 <", 0, 0),
				0
			),
			0
		),
		new Menu("关于", [](Menu*) {
			(new AboutDlg)->Loop();
		}, 0),
		0
	);
	menu = menu_en;
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
					if (m->proc) {
						m->proc(m);
					}
				} else {
					menu_popup = false;
				}
			}
			break;
		}
		}
	}
	endwin();
}